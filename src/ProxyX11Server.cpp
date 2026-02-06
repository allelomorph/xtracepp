#include <atomic>
#include <string>        // stoi to_string
#include <string_view>
#include <optional>
#include <filesystem>    // copy remove
#include <fstream>
#include <exception>     // runtime_error system_error

#include <cassert>
#include <cstdlib>       // getenv setenv
#include <cstring>       // memcpy
#include <cstdint>

#include <sys/socket.h>  // 'struct sockaddr' SOCK_STREAM AF_INET AF_UNIX setsockopt SOL_SOCKET SO_KEEPALIVE 
#include <linux/tcp.h>   // TCP_NODELAY
#include <arpa/inet.h>   // htons htonl inet_ntoa ntohs
#include <netinet/in.h>  // 'struct sockaddr_in'
#include <sys/un.h>      // 'struct sockaddr_un'
#include <unistd.h>      // unlink close pid_t fork STDERR_FILENO STDIN_FILENO _exit
#include <signal.h>      // sigaction sigaction() SIGCHLD CLD_EXITED siginfo_t
#include <netdb.h>       // 'struct addrinfo' getaddrinfo freeaddrinfo
#include <stdio.h>       // rename

#include <fmt/format.h>

#include "ProxyX11Server.hpp"
#include "Connection.hpp"
#include "errors.hpp"


/** @brief Whether child process has closed; made signal handler-accessible. */
static std::atomic_bool child_running {};
/** @brief Return value of child process; made signal handler-accessible. */
static std::atomic_int  child_retval  {};
static_assert( decltype( child_running )::is_always_lock_free );
static_assert( decltype( child_retval )::is_always_lock_free );

/**
 * @brief Signal handler for `SIGCHLD`.
 * @param sig number of signal intercepted
 * @param info contains further information about the signal
 * @param ucontext signal context information (`ucontext_t*` cast to `void*`)
 * @see `sigaction(2)` for prototype `sa_sigaction`
 */
static void handleSIGCHLD( int sig, ::siginfo_t* info, void*/* ucontext*/ ) {
    assert( sig == SIGCHLD );
    assert( info != nullptr );
    assert( child_running.load() == true );
    child_running.store( false );
    switch ( info->si_code ) {
    case CLD_EXITED:
        child_retval.store( info->si_status );
        break;
    case CLD_KILLED:
        [[fallthrough]];
    case CLD_DUMPED:
        child_retval.store(
            ProxyX11Server::SIGNAL_RETVAL_OFFSET + info->si_status );
        break;
    default:
        // CLD_TRAPPED CLD_STOPPED CLD_CONTINUED
        break;
    }
}

/** @brief File path if [_in_display](#ProxyX11Server::_in_display) uses Unix
 *    socket; made signal handler-accessible. */
static std::atomic<const char*> in_display_sun_path {};
/** @brief File path if [_out_display](#ProxyX11Server::_out_display) uses Unix
 *    socket; made signal handler-accessible. */
static std::atomic<const char*> out_display_sun_path {};
/** @brief X auth file path; made signal handler-accessible. */
static std::atomic<const char*> xauth_path {};
/** @brief X auth file backup path; made signal handler-accessible. */
static std::atomic<const char*> xauth_bup_path {};
static_assert( decltype( in_display_sun_path )::is_always_lock_free );
static_assert( decltype( out_display_sun_path )::is_always_lock_free );
static_assert( decltype( xauth_path )::is_always_lock_free );
static_assert( decltype( xauth_bup_path )::is_always_lock_free );

/**
 * @brief Signal handler for terminating signals `SIGINT`, `SIGTERM`,
 *   `SIGABRT`, and `SIGSEGV`.
 * @param sig number of signal intercepted
 * @see `sigaction(2)` for prototype `sa_handler`
 */
static void handleTerminatingSignal( int sig ) {
    assert( sig == SIGINT || sig == SIGTERM ||
            sig == SIGABRT || sig == SIGSEGV );
    // std::filesystem::remove inappropriate due to its call to remove(3)
    //   which is not listed as signal-safe
    // No error checking on ::unlink as we are are about to make a sudden
    //   _exit anyway
    if ( const char* indisp_sun_path { in_display_sun_path.load() };
         indisp_sun_path != nullptr ) {
        ::unlink( indisp_sun_path );
    }
    if ( const char* outdisp_sun_path { out_display_sun_path.load() };
         outdisp_sun_path != nullptr ) {
        ::unlink( outdisp_sun_path );
    }
    // restore original xauth file
    if ( const char* xauth_path_ { xauth_path.load() },
         * xauth_bup_path_ { xauth_bup_path.load() };
         xauth_path_ != nullptr && xauth_bup_path_ != nullptr ) {
        ::rename( xauth_bup_path_, xauth_path_ );
    }
    ::_exit( ProxyX11Server::SIGNAL_RETVAL_OFFSET + sig );
}

ProxyX11Server::ProxyX11Server( const int argc, const char* argv[] ) :
    settings( argc, argv ), _parser( settings ) {
    // strict, cheap alternative to singleton pattern
    static bool instantiated {};
    if ( instantiated ) {
        fmt::println( ::stderr, "{}: {}: constructor called more than once",
                      settings.process_name, __PRETTY_FUNCTION__ );
        ::exit( EXIT_FAILURE );
    }
    instantiated = true;

    _parseDisplayNames();

    if ( settings.copyauth )
        _copyAuthentication();
    if ( settings.systemtimeformat )
        _fetchCurrentServerTime();
    if ( settings.prefetchatoms )
        _parser.importFetchedAtoms( _fetchInternedAtoms() );
}

ProxyX11Server::~ProxyX11Server() {
    ::close( _listener_fd );
    // delete unix sockets
    if ( _in_display.ai_family == AF_UNIX )
        std::filesystem::remove( _in_display.unaddr.sun_path );
    if ( _out_display.ai_family == AF_UNIX )
        std::filesystem::remove( _out_display.unaddr.sun_path );
    // restore .Xauthority if modified by _copyAuthentication()
    if ( !_xauth_path.empty() && !_xauth_bup_path.empty() )
        std::filesystem::rename( _xauth_bup_path, _xauth_path );
}

int ProxyX11Server::run() {
    _listenForClients();
    _startSubcommandClient();
    return _processClientQueue();
}

void ProxyX11Server::_parseDisplayNames() {
    const char* out_displayname { nullptr };
    if ( settings.out_displayname != nullptr ) {
        out_displayname = settings.out_displayname;
    } else {
        out_displayname = ::getenv( _OUT_DISPLAYNAME_ENV_VAR.data() );
    }
    assert( out_displayname != nullptr );
    _out_display = DisplayInfo( out_displayname, DisplayInfo::Direction::OUT,
                                settings.process_name );
    if ( _out_display.ai_family == AF_UNIX ) {
        out_display_sun_path.store( _out_display.unaddr.sun_path );
    }

    const char* in_displayname { nullptr };
    if( settings.in_displayname != nullptr ) {
        in_displayname = settings.in_displayname;
    } else {
        in_displayname = ::getenv( _IN_DISPLAYNAME_ENV_VAR.data() );
        if ( in_displayname == nullptr ) {
            fmt::println( ::stderr, "No display specified via --proxydisplay or "
                          "${}, defaulting to {:?}",
                          _IN_DISPLAYNAME_ENV_VAR, _DEFAULT_IN_DISPLAYNAME );
            in_displayname = _DEFAULT_IN_DISPLAYNAME.data();
        }
    }
    assert( in_displayname != nullptr );
    _in_display = DisplayInfo( in_displayname, DisplayInfo::Direction::IN,
                               settings.process_name );
    if ( _in_display.ai_family == AF_UNIX ) {
        in_display_sun_path.store( _in_display.unaddr.sun_path );
    }

    if ( out_display_sun_path.load() != nullptr ||
         in_display_sun_path.load() != nullptr ) {
        // `struct` needed to disambiguate from sigaction(2)
        struct ::sigaction act {};
        act.sa_handler = &handleTerminatingSignal;
        if ( ::sigaction( SIGINT, &act, nullptr ) == -1 ||
             ::sigaction( SIGTERM, &act, nullptr ) == -1 ||
             ::sigaction( SIGABRT, &act, nullptr ) == -1 ||
             ::sigaction( SIGSEGV, &act, nullptr ) == -1 ) {
            fmt::println( ::stderr, "{}: {}: {}",
                          settings.process_name, __PRETTY_FUNCTION__,
                          errors::system::message( "sigaction" ) );
            ::exit( EXIT_FAILURE );
        }
    }
}

/**
 * @brief Contains data to encode an `xauth(1)` file [entry].
 * ```
 * The .Xauthority file is a binary file consisting of a sequence of entries
 * in the following format:
 *
 * 	2 bytes		Family value (second byte is as in protocol HOST)
 * 	2 bytes		address length (always MSB first)
 * 	A bytes		host address (as in protocol HOST)
 * 	2 bytes		display "number" length (always MSB first)
 * 	S bytes		display "number" string
 * 	2 bytes		name length (always MSB first)
 * 	N bytes		authorization name string
 * 	2 bytes		data length (always MSB first)
 * 	D bytes		authorization data string
 * ```
 * [entry]: https://gitlab.freedesktop.org/alanc/libxau/-/blob/master/README
 */
struct ProxyX11Server::_XAuthInfo {
private:
    /**
     * @brief Local copy of POSIX `NAME_MAX`, or max bytes in a filename.
     */
    static constexpr uint8_t _NAME_MAX { 255 };
    /**
     * @brief Local copy of POSIX `HOST_NAME_MAX`, or max bytes in a hostname.
     */
    static constexpr uint8_t _HOST_NAME_MAX { 64 };

public:
    /**
     * @brief Xlib Family value.
     * @note
     *   1. `family` in `xauth(1)` file entries refers to Xlib `Family*` values,
     *   (eg `FamilyInternet`) and does not correspond with `AF_*` `socket(2)` macros
     *   2. during development it was found that `family` is also encoded MSB first,
     *   despite its [description] ambiguously referencing [HOST](#protocol::HOST)
     * [description]: https://gitlab.freedesktop.org/alanc/libxau/-/blob/master/README
     */
    uint16_t family          {};
    /**
     * @brief [family](#family) in network byte order, or MSB first.
     */
    uint16_t family_nbo      {};
    /**
     * @brief Host address string length in bytes.
     */
    uint16_t addr_len        {};
    /**
     * @brief [addr_len](#addr_len) in network byte order, or MSB first.
     */
    uint16_t addr_len_nbo    {};
    /**
     * @brief Display number string length in bytes.
     */
    uint16_t display_len     {};
    /**
     * @brief [display_len](#display_len) in network byte order, or MSB first.
     */
    uint16_t display_len_nbo {};
    /**
     * @brief Authorization name string length in bytes.
     */
    uint16_t name_len        {};
    /**
     * @brief [name_len](#name_len) in network byte order, or MSB first.
     */
    uint16_t name_len_nbo    {};
    /**
     * @brief Authorization data length in bytes.
     */
    uint16_t data_len        {};
    /**
     * @brief [data_len](#data_len) in network byte order, or MSB first.
     */
    uint16_t data_len_nbo    {};
    /**
     * @brief Host address C string buffer.
     * @note `HOST_NAME_MAX` (see `gethostname(2)`) bytes should hold unix
     *   hostname or IPv4/IPv6 address plus port number
     */
    char addr[_HOST_NAME_MAX]    {};
    /**
     * @brief Display number C string buffer.
     */
    char display[_HOST_NAME_MAX] {};
    /**
     * @brief Authorization name C string buffer.
     */
    char name[_NAME_MAX]         {};
    /**
     * @brief Authorization data C string buffer.
     */
    char data[_AUTH_DATA_SZ]     {};
};

void ProxyX11Server::_copyAuthentication() {
    assert( !_in_display.name.empty() && !_out_display.name.empty() );

    ////// get auth path

    static constexpr std::string_view XAUTHORITY_ENV_VAR { "XAUTHORITY" };
    static constexpr std::string_view HOME_ENV_VAR { "HOME" };
    static constexpr std::string_view XAUTHORITY_DEFAULT_FILENAME { ".Xauthority" };

    const char* xauthority_value { getenv( XAUTHORITY_ENV_VAR.data() ) };
    if ( xauthority_value == nullptr ) {
        const char* home_value { getenv( HOME_ENV_VAR.data() ) };
        if ( home_value == nullptr ) {
            fmt::println( ::stderr, "{}: Could not get HOME environmental variable "
                          "value to resolve auth path", settings.process_name );
            ::exit( EXIT_FAILURE );
        }
        _xauth_path =
            fmt::format( "{}/{}", home_value, XAUTHORITY_DEFAULT_FILENAME );
    } else {
        _xauth_path = std::string( xauthority_value );
    }
    xauth_path.store( _xauth_path.data() );
    _xauth_bup_path = fmt::format( "{}.bup", _xauth_path );
    xauth_bup_path.store( _xauth_bup_path.data() );

    ////// read auth entries from file

    std::ifstream ifs( _xauth_path, std::ios::binary );
    if ( !ifs.good() ) {
        fmt::println(
            ::stderr, "{}: Could not open auth file for reading, expected "
            "paths: ${} or ${}/{}", settings.process_name,
            XAUTHORITY_ENV_VAR, HOME_ENV_VAR, XAUTHORITY_DEFAULT_FILENAME );
        ::exit( EXIT_FAILURE );
    }

    std::vector< _XAuthInfo > xauth_entries;
    for ( ; !ifs.eof(); ifs.peek() ) {
        _XAuthInfo auth;
        ifs.read( (char*)&auth.family_nbo, sizeof(auth.family_nbo) );
        auth.family = ntohs( auth.family_nbo );

        ifs.read( (char*)&auth.addr_len_nbo, sizeof(auth.addr_len_nbo) );
        auth.addr_len = ntohs( auth.addr_len_nbo );
        assert( auth.addr_len < sizeof(auth.addr) );
        ifs.read( auth.addr, auth.addr_len );
        auth.addr[auth.addr_len] = '\0';

        ifs.read( (char*)&auth.display_len_nbo, sizeof(auth.display_len_nbo) );
        auth.display_len = ntohs( auth.display_len_nbo );
        assert( auth.display_len < sizeof(auth.display) );
        ifs.read( auth.display, auth.display_len );
        auth.display[auth.display_len] = '\0';

        ifs.read( (char*)&auth.name_len_nbo, sizeof(auth.name_len_nbo) );
        auth.name_len = ntohs( auth.name_len_nbo );
        assert( auth.name_len < sizeof(auth.name) );
        ifs.read( auth.name, auth.name_len );
        auth.name[auth.name_len] = '\0';

        ifs.read( (char*)&auth.data_len_nbo, sizeof(auth.data_len_nbo) );
        auth.data_len = ntohs( auth.data_len_nbo );
        assert( auth.data_len == sizeof(auth.data) );
        ifs.read( auth.data, auth.data_len );

        xauth_entries.emplace_back( auth );
    }
    ifs.close();

    ////// edit auth entries

    // Testing has shown that xauth cookies need not be unique to a display, so
    //   here we duplicate the DISPLAY auth for use by PROXYDISPLAY
    _XAuthInfo* in_display_auth {};   // PROXYDISPLAY
    _XAuthInfo* out_display_auth {};  // DISPLAY
    for ( _XAuthInfo& auth : xauth_entries ) {
        if ( std::strtol( auth.display, nullptr, 10 ) == _in_display.display ) {
            assert( auth.name == _AUTH_NAME );
            assert( auth.data_len == _AUTH_DATA_SZ );
            in_display_auth = &auth;
        }
        if ( std::strtol( auth.display, nullptr, 10 ) == _out_display.display ) {
            if ( auth.name != _AUTH_NAME ) {
                fmt::println(
                    ::stderr, "{}: No support for display {:?} auth method {:?} "
                    "(expected {:?})", settings.process_name, _out_display.name,
                    auth.name, _AUTH_NAME );
                ::exit( EXIT_FAILURE );
            }
            assert( auth.data_len == _AUTH_DATA_SZ );
            out_display_auth = &auth;
        }
        if ( in_display_auth != nullptr && out_display_auth != nullptr )
            break;
    }
    if ( out_display_auth == nullptr ) {
        fmt::println( ::stderr, "{}: Could not find auth data for display {:?}",
                      settings.process_name, _out_display.name );
        ::exit( EXIT_FAILURE );
    }
    _XAuthInfo dup_auth { *out_display_auth };
    const std::string display_str { fmt::format( "{}", _in_display.display ) };
    dup_auth.display_len = display_str.size();
    dup_auth.display_len_nbo = ::htons( dup_auth.display_len );
    ::memcpy( dup_auth.display, display_str.data(), display_str.size() );
    dup_auth.display[ dup_auth.display_len ] = '\0';
    if ( in_display_auth != nullptr ) {  // revise existing PROXYDISPLAY entry
        *in_display_auth = dup_auth;
    } else {                             // append new PROXYDISPLAY entry
        xauth_entries.emplace_back( dup_auth );
    }
    // Collect copy of auth data for use by setup clients run prior to main
    //   queue (eg atom prefetching)
    ::memcpy( _auth_data, out_display_auth->data, _AUTH_DATA_SZ );

    ////// write auth entries back to file

    std::filesystem::copy( _xauth_path, _xauth_bup_path,
                           std::filesystem::copy_options::overwrite_existing );
    std::ofstream ofs( _xauth_path, std::ios::binary );
    if ( !ofs.good() ) {
        fmt::println(
            ::stderr, "{}: Could not open auth file for writing, expected "
            "paths: ${} or ${}/{}", settings.process_name,
            XAUTHORITY_ENV_VAR, HOME_ENV_VAR, XAUTHORITY_DEFAULT_FILENAME );
        ::exit( EXIT_FAILURE );
    }
    for ( const _XAuthInfo& auth : xauth_entries ) {
        ofs.write( (char*)&auth.family_nbo,
                   sizeof(auth.family_nbo) );

        ofs.write( (char*)&auth.addr_len_nbo,
                   sizeof(auth.addr_len_nbo) );
        ofs.write( auth.addr,
                   auth.addr_len );

        ofs.write( (char*)&auth.display_len_nbo,
                   sizeof(auth.display_len_nbo) );
        ofs.write( auth.display,
                   auth.display_len );

        ofs.write( (char*)&auth.name_len_nbo,
                   sizeof(auth.name_len_nbo) );
        ofs.write( auth.name,
                   auth.name_len );

        ofs.write( (char*)&auth.data_len_nbo,
                   sizeof(auth.data_len_nbo) );
        ofs.write( auth.data,
                   auth.data_len );
    }
    ofs.close();
}

void ProxyX11Server::_startSubcommandClient() {
    if ( settings.subcmd_argc == 0 )
        return;
    // `struct` needed to disambiguate from sigaction(2)
    struct ::sigaction act {};
    // registering a signal handler which should upon SIGCHLD populate its
    //   siginfo_t* parameter with the same info as waitid()
    act.sa_sigaction = &handleSIGCHLD;
    // waive need to wait for child processes terminating normally or via signal
    act.sa_flags = SA_SIGINFO | SA_NOCLDSTOP | SA_NOCLDWAIT;
    if ( ::sigaction( SIGCHLD, &act, nullptr ) == -1 ) {
        fmt::println( ::stderr, "{}: {}: {}",
                      settings.process_name, __PRETTY_FUNCTION__,
                      errors::system::message( "sigaction" ) );
        ::exit( EXIT_FAILURE );
    }
    _child_pid = ::fork();
    switch ( _child_pid ) {
    case 0:  // fork succeeded, now in child
        if ( ::setenv( _OUT_DISPLAYNAME_ENV_VAR.data(),
                       _in_display.name.data(), 1 ) != 0 ) {
        fmt::println( ::stderr, "{}: {}: {}",
                      settings.process_name, __PRETTY_FUNCTION__,
                      errors::system::message( "setenv" ) );
            ::exit( EXIT_FAILURE );
        }
        ::execvp( settings.subcmd_argv[0],
                  const_cast< char* const* >( settings.subcmd_argv ) );
        // child has failed to overtake parent process
        fmt::println( ::stderr, "{}: {}: {}",
                      settings.process_name, __PRETTY_FUNCTION__,
                      errors::system::message( "execvp" ) );
        ::exit( EXIT_FAILURE );
    case -1:  // still in parent, fork failed
        fmt::println( ::stderr, "{}: {}: {}",
                      settings.process_name, __PRETTY_FUNCTION__,
                      errors::system::message( "fork" ) );
        ::exit( EXIT_FAILURE );
    default:  // still in parent, fork success
        _child_used = true;
        child_running.store( true );
        break;
    }
}

std::optional< std::string_view >
ProxyX11Server::_socketPollError( const int fd ) {
    const short revents { _pfds.at( _pfds_i_by_fd.at( fd ) ).revents };
    std::string_view err_msg {
        ( revents & POLLERR )  ? "error condition" :
        // ( revents & POLLHUP )  ? "hang up" :
        ( revents & POLLNVAL ) ? "invalid fd (not open)" :
        ( revents & POLLPRI )  ? "exceptional condition" :
                                 "" };
    if ( err_msg.empty() )
        return std::nullopt;
    return err_msg;
}

void ProxyX11Server::_addSocketToPoll( const int fd, const short events/* = _POLLNONE*/ ) {
    assert( _pfds_i_by_fd.find( fd ) == _pfds_i_by_fd.end() );
    _pfds.emplace_back( ::pollfd{ fd, events, 0 } );
    _pfds_i_by_fd.emplace( fd, _pfds.size() - 1 );
}

void ProxyX11Server::_updatePollFlags() {
    for ( auto& [ id, conn ] : _connections ) {
        ::pollfd& client_pfd {
            _pfds.at( _pfds_i_by_fd.at( conn.client_fd ) ) };
        ::pollfd& server_pfd {
            _pfds.at( _pfds_i_by_fd.at( conn.server_fd ) ) };
        client_pfd.events = POLLPRI;
        server_pfd.events = POLLPRI;
        if ( conn.client_buffer.readReady() ) {
            client_pfd.events |= POLLIN;
        } else {
            assert( conn.client_buffer.writeReady() );
            server_pfd.events |= POLLOUT;
        }
        if ( conn.server_buffer.readReady() ) {
            server_pfd.events |= POLLIN;
        } else {
            assert( conn.server_buffer.writeReady() );
            client_pfd.events |= POLLOUT;
        }
    }
}

void ProxyX11Server::_processPolledSockets() {
    std::vector< int > conns_to_close;
    for ( auto& [ id, conn ] : _connections ) {
        assert( conn.clientSideOpen() );
        assert( conn.serverSideOpen() );
        if ( _socketReadReady( conn.client_fd ) ) {
            const auto& [ bytes_read, read_error ] {
                conn.client_buffer.read( conn.client_fd ) };  // bufferFromClient();
            if ( read_error ) {
                fmt::println( ::stderr, "C{:03d}:{}: error reading from client: "
                              "{}, closing connection",
                              conn.id, _parser.CLIENT_TO_SERVER, *read_error );
                goto close_connection;
            }
            if ( bytes_read == 0 ) {
                if ( settings.readwritedebug ) {
                    fmt::println( settings.log_fs,
                                  "C{:03d}:{}: got EOF, closing connection",
                                  conn.id, _parser.CLIENT_TO_SERVER );
                }
                goto close_connection;
            }
            assert( !conn.client_buffer.empty() );
            if ( settings.readwritedebug ) {
                fmt::println( settings.log_fs,
                              "C{:03d}:{:04d}B:{}: read from client into buffer",
                              conn.id, bytes_read, _parser.CLIENT_TO_SERVER );
            }
            const auto& [ bytes_parsed, parse_error ] {
                _parser.logClientMessages( &conn ) };
            if ( parse_error ) {
                fmt::println( ::stderr, "C{:03d}:{}: error parsing client "
                              "messages: {}, closing connection",
                              conn.id, _parser.CLIENT_TO_SERVER, *parse_error );
                // without knowing that this connection is to the child process,
                //   we rely on the expectation that closing the connection
                //   should force EOF and thus exit for any client X app
                goto close_connection;
            }
        } else if ( _socketWriteReady( conn.client_fd ) &&
                    conn.server_buffer.writeReady() ) {
            const auto& [ bytes_written, write_error ] {
                conn.server_buffer.write( conn.client_fd ) }; // forwardToClient();
            if ( write_error ) {
                fmt::println( ::stderr, "C{:03d}:{}: error writing to client: "
                              "{}, closing connection",
                              conn.id, _parser.SERVER_TO_CLIENT, *write_error );
                goto close_connection;
            }
            assert( bytes_written > 0 );
            if ( settings.readwritedebug ) {
                fmt::println( settings.log_fs,
                              "C{:03d}:{:04d}B:{}: wrote from buffer to client",
                              conn.id, bytes_written, _parser.SERVER_TO_CLIENT );
            }
        } else if ( const auto poll_error { _socketPollError( conn.client_fd ) };
                    poll_error ) {
            fmt::println( ::stderr, "C{:03d}: client socket poll error: {}, "
                          "closing connection",
                          conn.id, *poll_error );
            goto close_connection;
        }
        if ( _socketReadReady( conn.server_fd ) ) {
            const auto& [ bytes_read, read_error ] {
                conn.server_buffer.read( conn.server_fd ) };  // bufferFromServer();
            if ( read_error ) {
                fmt::println( ::stderr, "C{:03d}:{}: error reading from server: "
                              "{}, closing connection",
                              conn.id, _parser.SERVER_TO_CLIENT, *read_error );
                goto close_connection;
            }
            if ( bytes_read == 0 ) {
                if ( settings.readwritedebug ) {
                    fmt::println( settings.log_fs,
                                  "C{:03d}:{}: got EOF, closing connection",
                                  conn.id, _parser.SERVER_TO_CLIENT );
                }
                goto close_connection;
            }
            assert( !conn.server_buffer.empty() );
            if ( settings.readwritedebug ) {
                fmt::println( settings.log_fs,
                              "C{:03d}:{:04d}B:{}: read from server into buffer",
                              conn.id, bytes_read, _parser.SERVER_TO_CLIENT );
            }
            const auto& [ bytes_parsed, parse_error ] {
                _parser.logServerMessages( &conn ) };
            if ( parse_error ) {
                fmt::println( ::stderr, "C{:03d}:{}: error parsing server "
                              "messages: {}, closing connection",
                              conn.id, _parser.SERVER_TO_CLIENT, *parse_error );
                goto close_connection;
            }
        } else if ( _socketWriteReady( conn.server_fd ) &&
                    conn.client_buffer.writeReady() ) {
            const auto& [ bytes_written, write_error ] {
                conn.client_buffer.write( conn.server_fd ) }; // forwardToServer();
            if ( write_error ) {
                fmt::println( ::stderr, "C{:03d}:{}: error writing to server: "
                              "{}, closing connection",
                              conn.id, _parser.CLIENT_TO_SERVER, *write_error );
                goto close_connection;
            }
            assert( bytes_written > 0 );
            if ( settings.readwritedebug ) {
                fmt::println( settings.log_fs,
                              "C{:03d}:{:04d}B:{}: wrote from buffer to server",
                              conn.id, bytes_written, _parser.CLIENT_TO_SERVER );
            }
        } else if ( const auto poll_error { _socketPollError( conn.server_fd ) };
                    poll_error ) {
            fmt::println( ::stderr, "C{:03d}: server socket poll error: {}, "
                          "closing connection",
                          conn.id, *poll_error );
            goto close_connection;
        }
        continue;
    close_connection:
        conns_to_close.emplace_back( id );
    }
    _closeConnections( conns_to_close );

    if ( _socketReadReady( _listener_fd ) ) {
        _openConnection();
    } else if ( const auto error { _socketPollError( _listener_fd ) }; error ) {
        fmt::println( ::stderr, "{}: {}: listening socket poll error: {}",
                      settings.process_name, __PRETTY_FUNCTION__, *error );
    }
}

void ProxyX11Server::_listenForClients() {
    const int fd { ::socket(
            _in_display.ai_family, _in_display.ai_socktype,
            _in_display.ai_protocol ) };
    if ( fd < 0 )  {
        fmt::println( ::stderr, "{}: {}: {}",
                      settings.process_name, __PRETTY_FUNCTION__,
                      errors::system::message( "socket" ) );
        ::exit( EXIT_FAILURE );
    }
    switch ( _in_display.ai_family ) {
    case AF_INET6: {
        // `ipv6(7)` IPV6_V6ONLY: if false, then can send and receive messages to
        //   and from an IPv6 address or an IPv4-mapped IPv6 address
        const int off {};
        if ( ::setsockopt( fd, IPPROTO_IPV6, IPV6_V6ONLY, &off, sizeof(off) ) ) {
            ::close( fd );
            fmt::println( ::stderr, "{}: {}: {}",
                          settings.process_name, __PRETTY_FUNCTION__,
                          errors::system::message( "setsockopt" ) );
            ::exit( EXIT_FAILURE );
        }
    }   [[fallthrough]];
    case AF_INET: {
        // `socket(7)` SO_KEEPALIVE: enable sending of keep-alive messages on
        //     connection-oriented sockets
        const int on { 1 };
        if ( ::setsockopt( fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on) ) != 0 ) {
            ::close( fd );
            fmt::println( ::stderr, "{}: {}: {}",
                          settings.process_name, __PRETTY_FUNCTION__,
                          errors::system::message( "setsockopt" ) );
            ::exit( EXIT_FAILURE );
        }
    }   break;
    case AF_UNIX:
        // While any PROXYDISPLAY unix socket from previous runs should have
        //   been deleted as as part of both normal exit and signal interruption,
        //   in the edge case of a SIGKILL termination, it may still exist.
        std::filesystem::remove( _in_display.unaddr.sun_path );
        break;
    default:
        break;
    }
    if ( ::bind( fd, &_in_display.ai_addr, _in_display.ai_addrlen ) != 0 ) {
        ::close( fd );
        fmt::println( ::stderr, "{}: {}: {}",
                      settings.process_name, __PRETTY_FUNCTION__,
                      errors::system::message( "bind" ) );
        ::exit( EXIT_FAILURE );
    }
    if ( ::listen( fd, _MAX_PENDING_CONNECTIONS ) != 0 ) {
        ::close( fd );
        fmt::println( ::stderr, "{}: {}: {}",
                      settings.process_name, __PRETTY_FUNCTION__,
                      errors::system::message( "listen" ) );
        ::exit( EXIT_FAILURE );
    }
    _listener_fd = fd;
}

bool ProxyX11Server::_acceptClient( Connection* conn ) {
    assert( conn != nullptr );
    std::string client_desc;
    union {
        ::sockaddr     addr;
        ::sockaddr_in  inaddr;
        ::sockaddr_in6 in6addr;
        ::sockaddr_un  unaddr {};
    };
    static_assert( sizeof( unaddr ) > sizeof( addr ) &&
                   sizeof( unaddr ) > sizeof( inaddr ) &&
                   sizeof( unaddr ) > sizeof( in6addr ) );
    ::socklen_t addrlen { _in_display.ai_addrlen };
    const int fd { ::accept( _listener_fd, &addr, &addrlen ) };
    if ( fd < 0 ) {
        fmt::println( ::stderr, "{}: {}: {}",
                      settings.process_name, __PRETTY_FUNCTION__,
                      errors::system::message( "accept" ) );
        return false;
    }
    switch ( addr.sa_family ) {
    case AF_INET6: {
        assert( addrlen == sizeof( in6addr ) );
        char addrstr[ INET6_ADDRSTRLEN ] {};
        if ( ::inet_ntop( in6addr.sin6_family, &(in6addr.sin6_addr),
                          addrstr, INET6_ADDRSTRLEN ) == nullptr ) {
            ::close( fd );
            fmt::println( ::stderr, "{}: {}: {}",
                          settings.process_name, __PRETTY_FUNCTION__,
                          errors::system::message( "inet_ntop" ) );
            return false;
        }
        client_desc = fmt::format( "[{}]:{}", addrstr,
                                   ntohs( in6addr.sin6_port ) );
    }   break;
    case AF_INET: {
        assert( addrlen == sizeof( inaddr ) );
        char addrstr[ INET_ADDRSTRLEN ] {};
        if ( ::inet_ntop( inaddr.sin_family, &(inaddr.sin_addr),
                          addrstr, INET_ADDRSTRLEN ) == nullptr ) {
            ::close( fd );
            fmt::println( ::stderr, "{}: {}: {}",
                          settings.process_name, __PRETTY_FUNCTION__,
                          errors::system::message( "inet_ntop" ) );
            return false;
        }
        client_desc = fmt::format( "{}:{}", addrstr,
                                   ntohs( inaddr.sin_port ) );
    }   break;
    case AF_UNIX: {
        // sun_path will likely always be unpopulated by connect(2), see:
        //   - https://stackoverflow.com/q/17090043
        assert( std::string_view( unaddr.sun_path ).empty() );
        // In testing, getpeername(2) also returns an empty unix socket path,
        //   so instead we assign a generic name
        client_desc = std::string{ "unknown(local)" };
    }   break;
    default:
        break;
    }
    conn->client_fd   = fd;
    conn->client_desc = client_desc;
    return true;
}

int ProxyX11Server::_connectToServer() {
    const int fd { ::socket( _out_display.ai_family, _out_display.ai_socktype,
                             _out_display.ai_protocol) };
    if ( fd < 0 )  {
        fmt::println( ::stderr, "{}: {}: {}",
                      settings.process_name, __PRETTY_FUNCTION__,
                      errors::system::message( "socket" ) );
        return _UNINIT_FD;
    }
    std::string connect_err;
    switch ( _out_display.ai_family ) {
    case AF_INET6:
        [[fallthrough]];
    case AF_INET: {
        // `tcp(7)` TCP_NODELAY: segments are always sent as soon as possible,
        //   even if there is only a small amount of data
        // `socket(7)` SO_KEEPALIVE: enable sending of keep-alive messages on
        //    connection-oriented sockets
        const int on { 1 };
        if ( ::setsockopt( fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on) ) != 0 ||
             ::setsockopt( fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on) ) != 0 ) {
            ::close( fd );
            fmt::println( ::stderr, "{}: {}: {}",
                          settings.process_name, __PRETTY_FUNCTION__,
                          errors::system::message( "setsockopt" ) );
            return _UNINIT_FD;
        }
        connect_err = fmt::format(
            "{}: {}: error connecting to {:?} (resolved to {:?}) for display {:?}: {}",
            settings.process_name, __PRETTY_FUNCTION__,
            _out_display.hostname, _out_display.addrstr, _out_display.name,
            errors::system::message( "connect" ) );
    }   break;
    case AF_UNIX: {
        connect_err = fmt::format(
            "{}: {}: error connecting to unix socket {:?} for display {:?}: {}",
            settings.process_name, __PRETTY_FUNCTION__,
            _out_display.unaddr.sun_path, _out_display.name,
            errors::system::message( "connect" ) );
    }   break;
    default:
        break;
    }
    if ( ::connect( fd, &_out_display.ai_addr, _out_display.ai_addrlen ) != 0 ) {
        ::close( fd );
        fmt::println( ::stderr, connect_err );
        return _UNINIT_FD;
    }
    return fd;
}

void ProxyX11Server::_openConnection() {
    Connection conn {};
    if ( !_acceptClient( &conn ) ) {
        fmt::println( ::stderr, "{}: {}: failure to accept client connection",
                      settings.process_name, __PRETTY_FUNCTION__ );
        return;
    }
    assert( conn.client_fd > _listener_fd );
    assert( !conn.client_desc.empty() );
    fmt::println( ::stderr, "Connected to client: {}", conn.client_desc );
    conn.server_fd = _connectToServer();
    if ( conn.server_fd == -1 ) {
        fmt::println( ::stderr, "{}: {}: failure to connect to X server for display: {:?}",
                      settings.process_name, __PRETTY_FUNCTION__,
                      _out_display.name );
        conn.closeClientSide();
        return;
    }
    assert( conn.server_fd > _listener_fd );

    _connections.emplace( conn.id, conn );
    _addSocketToPoll( conn.client_fd );
    _addSocketToPoll( conn.server_fd );
}

void ProxyX11Server::_closeConnections( const std::vector< int >& ids ) {
    for ( const int id : ids ) {
        Connection& conn { _connections.at( id ) };
        if ( !conn.client_buffer.empty() ) {
            fmt::println(
                settings.log_fs,
                "C{:03d}:{:04d}B:{}: discarded unsent buffer",
                conn.id, conn.client_buffer.size(), _parser.CLIENT_TO_SERVER );
        }
        if ( !conn.server_buffer.empty() ) {
            fmt::println(
                settings.log_fs,
                "C{:03d}:{:04d}B:{}: discarded unsent buffer",
                conn.id, conn.server_buffer.size(), _parser.SERVER_TO_CLIENT );
        }
        _pfds_i_by_fd.erase( conn.client_fd );
        conn.closeClientSide();
        _pfds_i_by_fd.erase( conn.server_fd );
        conn.closeServerSide();
        _connections.erase( id );
    }
    // zip _pfds array to _pfds_i_by_fd keys
    std::vector< ::pollfd > new_pfds;
    int i {};
    for ( auto& [ fd, pfds_i ] : _pfds_i_by_fd ) {
        const ::pollfd& pfd { _pfds.at( pfds_i ) };
        assert( pfd.fd == fd );
        new_pfds.emplace_back( pfd );
        pfds_i = i;
        ++i;
    }
    _pfds = std::move( new_pfds );
    // listener should still be open even if all connections were closed
    assert( !_pfds_i_by_fd.empty() );
}

int ProxyX11Server::_processClientQueue() {
    _addSocketToPoll( _listener_fd, POLLPRI | POLLIN );

    static constexpr int NO_TIMEOUT { -1 };
    while ( child_running.load() || !_connections.empty() || settings.keeprunning ) {
        _updatePollFlags();
        // blocks until polled fds have new events or interrupted by signal
        if ( ::poll( _pfds.data(), nfds_t( _pfds.size() ), NO_TIMEOUT ) == -1 ) {
            if ( errno != 0 && errno != EINTR ) {
                fmt::println( ::stderr, "{}: {}: {}",
                              settings.process_name, __PRETTY_FUNCTION__,
                              errors::system::message( "poll" ) );
                return EXIT_FAILURE;
            }
            continue;
        }
        _processPolledSockets();
    }
    if ( _child_used && !settings.keeprunning )
        return child_retval.load();
    return EXIT_SUCCESS;
}
