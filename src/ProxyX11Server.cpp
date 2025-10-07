#include <atomic>
#include <string>        // stoi to_string
#include <string_view>
#include <optional>
#include <filesystem>    // copy remove

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

#include <fmt/format.h>

#include "ProxyX11Server.hpp"
#include "Connection.hpp"
#include "errors.hpp"


std::atomic_bool child_running {};
std::atomic_int  child_retval  {};

void handleSIGCHLD( int sig, siginfo_t* info, void*/* ucontext*/ ) {
    assert( sig == SIGCHLD );
    assert( info != nullptr );
    assert( child_running.load() == true );
    child_running.store( false );
    switch ( info->si_code ) {
    case CLD_EXITED:
        child_retval.store( info->si_status );
        break;
    case CLD_KILLED: [[fallthrough]];
    case CLD_DUMPED:
        static constexpr int _SIGNAL_RETVAL_OFFSET { 128 };
        child_retval.store(
            _SIGNAL_RETVAL_OFFSET + info->si_status );
        break;
    default:
        // CLD_TRAPPED CLD_STOPPED CLD_CONTINUED
        break;
    }
}

std::atomic<const char*> in_display_sun_path {};
std::atomic<const char*> out_display_sun_path {};
std::atomic<const char*> xauth_path {};
std::atomic<const char*> xauth_bup_path {};

// TBD can't use std::filesystem::remove as is calls remove(3) which is not
//   listed as signal-safe
void handleTerminatingSignal( int sig ) {
    assert( sig == SIGINT || sig == SIGTERM ||
            sig == SIGABRT || sig == SIGSEGV );
    // No error checking on unlink as we are are about to make a sudden
    //   _exit anyway
    if ( const char* indisp_sun_path { in_display_sun_path.load() };
         indisp_sun_path != nullptr ) {
        unlink( indisp_sun_path );
    }
    if ( const char* outdisp_sun_path { out_display_sun_path.load() };
         outdisp_sun_path != nullptr ) {
        unlink( outdisp_sun_path );
    }
    // restore original xauth file
    if ( const char* xauth_path_ { xauth_path.load() },
         * xauth_bup_path_ { xauth_bup_path.load() };
         xauth_path_ != nullptr && xauth_bup_path_ != nullptr ) {
        rename( xauth_bup_path_, xauth_path_ );
    }
    static constexpr int _SIGNAL_RETVAL_OFFSET { 128 };
    _exit( _SIGNAL_RETVAL_OFFSET + sig );
}

ProxyX11Server::~ProxyX11Server() {
    close( _listener_fd );
    if ( _in_display.family == AF_UNIX )
        std::filesystem::remove( _in_display.unaddr.sun_path );
    if ( _out_display.family == AF_UNIX )
        std::filesystem::remove( _out_display.unaddr.sun_path );
    if ( !_xauth_path.empty() && !_xauth_bup_path.empty() )
        std::filesystem::rename( _xauth_bup_path, _xauth_path );
}

void ProxyX11Server::init( const int argc, char* const* argv ) {
    settings.parseFromArgv( argc, argv );
    _parseDisplayNames();

    if ( settings.copyauth )
        _copyAuthentication();
    if ( settings.relativetimestamps )
        _fetchCurrentServerTime();
    if ( settings.prefetchatoms )
        _fetchInternedAtoms();

    _parser.importSettings( settings );
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
        out_displayname = getenv( _OUT_DISPLAYNAME_ENV_VAR.data() );
    }
    assert( out_displayname != nullptr );
    _out_display = DisplayInfo( out_displayname, DisplayInfo::Direction::OUT );
    if ( _out_display.family == AF_UNIX ) {
        out_display_sun_path.store( _out_display.unaddr.sun_path );
    }

    const char* in_displayname { nullptr };
    if( settings.in_displayname != nullptr ) {
        in_displayname = settings.in_displayname;
    } else {
        in_displayname = getenv( _IN_DISPLAYNAME_ENV_VAR.data() );
        if ( in_displayname == nullptr ) {
            fmt::print( stderr, "No display specified via --proxydisplay or {}, "
                        "defaulting to {}\n",
                        _IN_DISPLAYNAME_ENV_VAR, _DEFAULT_IN_DISPLAYNAME );
            in_displayname = _DEFAULT_IN_DISPLAYNAME.data();
        }
    }
    assert( in_displayname != nullptr );
    _in_display = DisplayInfo( in_displayname, DisplayInfo::Direction::IN );
    if ( _in_display.family == AF_UNIX ) {
        in_display_sun_path.store( _in_display.unaddr.sun_path );
    }

    if ( out_display_sun_path.load() != nullptr ||
         in_display_sun_path.load() != nullptr ) {
        // `struct` needed to disambiguate from sigaction(2)
        struct sigaction act {};
        act.sa_handler = &handleTerminatingSignal;
        if ( sigaction( SIGINT, &act, nullptr ) == -1 ||
             sigaction( SIGTERM, &act, nullptr ) == -1 ||
             sigaction( SIGABRT, &act, nullptr ) == -1 ||
             sigaction( SIGSEGV, &act, nullptr ) == -1 ) {
            fmt::println( ::stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::system::message( "sigaction" ) );
            exit( EXIT_FAILURE );
        }
    }
}

// per https://gitlab.freedesktop.org/alanc/libxau/-/blob/master/README
// """
// The .Xauthority file is a binary file consisting of a sequence of entries
// in the following format:

// 2 bytes      Family value (second byte is as in protocol HOST)
// 2 bytes	address length (always MSB first)
// A bytes	host address (as in protocol HOST)
// 2 bytes	display "number" length (always MSB first)
// S bytes	display "number" string
// 2 bytes	name length (always MSB first)
// N bytes	authorization name string
// 2 bytes	data length (always MSB first)
// D bytes	authorization data string
// """
struct ProxyX11Server::_XAuthInfo {
private:
    static constexpr uint8_t _NAME_MAX { 255 };  // TBD should be same as POSIX NAME_MAX
    static constexpr uint8_t _HOST_NAME_MAX { 64 };  // TBD should be same as POSIX HOST_NAME_MAX
public:
    // TBD nbo = network byte order (MSB first)
    // TBD important to note that family in .Xauthority refers to Xlib Family*
    //   values (eg FamilyInternet, see generated X.h,) and does not correspond
    //   with AF_* socket.h macros
    // TBD in testing xauth reads/writes family as MSB first, even if README
    //   above is not explicit
    uint16_t family          {};
    uint16_t family_nbo      {};
    uint16_t addr_len        {};
    uint16_t addr_len_nbo    {};
    uint16_t display_len     {};
    uint16_t display_len_nbo {};
    uint16_t name_len        {};
    uint16_t name_len_nbo    {};
    uint16_t data_len        {};
    uint16_t data_len_nbo    {};
    // TBD if using tcp without ssh, a URI could be much longer than 64 chars
    char addr[_HOST_NAME_MAX]    {};
    char display[_HOST_NAME_MAX] {};
    char name[_NAME_MAX]         {};
    char data[_AUTH_DATA_SZ]     {};
};

void ProxyX11Server::_copyAuthentication() {
    assert( !_in_display.name.empty() &&
            !_out_display.name.empty() );

    ////// get auth path

    static constexpr std::string_view XAUTHORITY_ENV_VAR { "XAUTHORITY" };
    static constexpr std::string_view HOME_ENV_VAR { "HOME" };
    static constexpr std::string_view XAUTHORITY_DEFAULT_FILENAME { ".Xauthority" };

    const char* xauthority_value { getenv( XAUTHORITY_ENV_VAR.data() ) };
    if ( xauthority_value == nullptr ) {
        const char* home_value { getenv( HOME_ENV_VAR.data() ) };
        if ( home_value == nullptr ) {
            fmt::println(
                stderr, "Could not get HOME environmental variable value to resolve auth path" );
            exit( EXIT_FAILURE );
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

    std::ifstream ifs( xauth_path, std::ios::binary );
    if ( !ifs.good() ) {
        fmt::println(
            stderr, "Could not open auth file for reading, expected paths: ${} or ${}/{}",
            XAUTHORITY_ENV_VAR, HOME_ENV_VAR, XAUTHORITY_DEFAULT_FILENAME );
        exit( EXIT_FAILURE );
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

    // TBD actual DISPLAY and PROXYDISPLAY hostname(addr)/family for this run
    //   may differ from that currently recorded in the auth file for DISPLAY,
    //   but in testing all that really matters is that each display name has an
    //   entry with a valid cookie (even when it is not unique, as we are here
    //   duplicating the DISPLAY auth to use for PROXYDISPLAY)
    // TBD if one were to validate the auth file family values, we would need to
    //   map AF_* macros to libX11's Family* values
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
                    stderr, "No support for display \"{}\" auth method {} (expected {})",
                    _out_display.name, auth.name, _AUTH_NAME );
                exit( EXIT_FAILURE );
            }
            assert( auth.data_len == _AUTH_DATA_SZ );
            out_display_auth = &auth;
        }
        if ( in_display_auth != nullptr && out_display_auth != nullptr )
            break;
    }
    if ( out_display_auth == nullptr ) {
        fmt::println( stderr, "Could not find auth data for display \"{}\"",
                      _out_display.name );
        exit( EXIT_FAILURE );
    }
    _XAuthInfo dup_auth { *out_display_auth };
    const std::string display_str { fmt::format( "{}", _in_display.display ) };
    dup_auth.display_len = display_str.size();
    dup_auth.display_len_nbo = htons( dup_auth.display_len );
    memcpy( dup_auth.display, display_str.data(), display_str.size() );
    dup_auth.display[ dup_auth.display_len ] = '\0';
    if ( in_display_auth != nullptr ) {  // revise existing PROXYDISPLAY entry
        *in_display_auth = dup_auth;
    } else {                             // append new PROXYDISPLAY entry
        xauth_entries.emplace_back( dup_auth );
    }
    // Collect copy of auth data for use by setup clients run prior to main
    //   queue (eg atom prefetching)
    memcpy( _auth_data, out_display_auth->data, _AUTH_DATA_SZ );

    ////// write auth entries back to file

    std::filesystem::copy( _xauth_path, _xauth_bup_path,
                           std::filesystem::copy_options::overwrite_existing );
    std::ofstream ofs( _xauth_path, std::ios::binary );
    if ( !ofs.good() ) {
        fmt::println(
            stderr, "Could not open auth file for writing, expected paths: ${} or ${}/{}",
            XAUTHORITY_ENV_VAR, HOME_ENV_VAR, XAUTHORITY_DEFAULT_FILENAME );
        exit( EXIT_FAILURE );
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
    struct sigaction act {};
    // registering a signal handler which should upon SIGCHLD populate its
    //   siginfo_t* parameter with the same info as waitid()
    act.sa_sigaction = &handleSIGCHLD;
    // waive need to wait for child processes terminating normally or via signal
    act.sa_flags = SA_SIGINFO | SA_NOCLDSTOP | SA_NOCLDWAIT;
    if ( sigaction( SIGCHLD, &act, nullptr ) == -1 ) {
        fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                      errors::system::message( "sigaction" ) );
        exit( EXIT_FAILURE );
    }
    _child_pid = fork();
    switch ( _child_pid ) {
    case 0:  // fork succeeded, now in child
        if ( setenv( _OUT_DISPLAYNAME_ENV_VAR.data(),
                     _in_display.name.data(), 1 ) != 0 ) {
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::system::message( "setenv" ) );
            exit( EXIT_FAILURE );
        }
        execvp( settings.subcmd_argv[0], settings.subcmd_argv );
        // child has failed to overtake parent process
        fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                      errors::system::message( "execvp" ) );
        exit( EXIT_FAILURE );
    case -1:  // still in parent, fork failed
        fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                      errors::system::message( "fork" ) );
        exit( EXIT_FAILURE );
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
        ( revents & POLLHUP )  ? "hang up" :
        ( revents & POLLNVAL ) ? "invalid fd (not open)" :
        ( revents & POLLPRI )  ? "exceptional condition" :
                                 "" };
    if ( err_msg.empty() )
        return std::nullopt;
    return err_msg;
}

void ProxyX11Server::_addSocketToPoll( const int fd, const short events/* = _POLLNONE*/ ) {
    assert( _pfds_i_by_fd.find( fd ) == _pfds_i_by_fd.end() );
    _pfds.emplace_back( pollfd{ fd, events, 0 } );
    _pfds_i_by_fd.emplace( fd, _pfds.size() - 1 );
}

void ProxyX11Server::_updatePollFlags() {
    for ( auto& [ id, conn ] : _connections ) {
        pollfd& client_pfd {
            _pfds.at( _pfds_i_by_fd.at( conn.client_fd ) ) };
        pollfd& server_pfd {
            _pfds.at( _pfds_i_by_fd.at( conn.server_fd ) ) };
        client_pfd.events = POLLPRI;
        server_pfd.events = POLLPRI;
        if ( conn.client_buffer.empty() ) {
            client_pfd.events |= POLLIN;
        } else {
            server_pfd.events |= POLLOUT;
        }
        if ( conn.server_buffer.empty() ) {
            server_pfd.events |= POLLIN;
        } else {
            client_pfd.events |= POLLOUT;
        }
    }
}

/*
 * second loop over connections to:
 *   - receive new packets
 *   - parse current packets
 *   - send current packets
 *   - close any connections that failed or detected EOF in either socket
 */
void ProxyX11Server::_processPolledSockets() {
    std::vector< int > conns_to_close;
    for ( auto& [ id, conn ] : _connections ) {
        assert( conn.clientSocketIsOpen() );
        assert( conn.serverSocketIsOpen() );
        if ( _socketReadReady( conn.client_fd ) ) {
            size_t bytes_read {};
            try {
                bytes_read = conn.bufferPacketFromClient();
            } catch ( const std::system_error& e ) {
                if ( settings.readwritedebug ) {
                    fmt::println(
                        settings.log_fs,
                        "C{:03d}:{}: error reading from client: {}",
                        conn.id, _parser._CLIENT_TO_SERVER, e.what() );
                }
                conns_to_close.emplace_back( id );
                continue;
            }
            if ( bytes_read == 0 ) {
                if ( settings.readwritedebug ) {
                    fmt::println( settings.log_fs,
                                  "C{:03d}:{}: got EOF",
                                  conn.id, _parser._CLIENT_TO_SERVER );
                }
                conns_to_close.emplace_back( id );
                continue;
            }
            if ( settings.readwritedebug ) {
                fmt::println( settings.log_fs,
                              "C{:03d}:{:04d}B:{}: packet read from client into buffer",
                              conn.id, bytes_read, _parser._CLIENT_TO_SERVER );
            }
            assert( !conn.client_buffer.empty() );
            const size_t bytes_parsed { _parser.logClientPackets( &conn ) };
            assert( bytes_parsed == bytes_read );
        } else if ( _socketWriteReady( conn.client_fd ) ) {
            assert( !conn.server_buffer.empty() );
            size_t bytes_written {};
            try {
                bytes_written = conn.forwardPacketToClient();
            } catch ( const std::system_error& e ) {
                if ( settings.readwritedebug ) {
                    fmt::println( settings.log_fs,
                                  "C{:03d}:{}: error writing to client: {}",
                                  conn.id, _parser._SERVER_TO_CLIENT, e.what() );
                }
                conns_to_close.emplace_back( id );
                continue;
            }
            assert( bytes_written > 0 );
            if ( settings.readwritedebug ) {
                fmt::println( settings.log_fs,
                              "C{:03d}:{:04d}B:{}: wrote packet from buffer to client",
                              conn.id, bytes_written, _parser._SERVER_TO_CLIENT );
            }
        } else if ( const auto error { _socketPollError( _listener_fd ) }; error ) {
            fmt::println(
                settings.log_fs,
                "C{:03d}: poll failure on client socket: {}",
                conn.id, *error );
            conns_to_close.emplace_back( id );
            continue;
        }
        if ( _socketReadReady( conn.server_fd ) ) {
            size_t bytes_read {};
            try {
                bytes_read = conn.bufferPacketFromServer();
            } catch ( const std::system_error& e ) {
                if ( settings.readwritedebug ) {
                    fmt::println(
                        settings.log_fs,
                        "C{:03d}:{}: error reading from server: {}",
                        conn.id, _parser._SERVER_TO_CLIENT, e.what() );
                }
                conns_to_close.emplace_back( id );
                continue;
            }
            if ( bytes_read == 0 ) {
                if ( settings.readwritedebug ) {
                    fmt::println( settings.log_fs,
                                  "C{:03d}:{}: got EOF",
                                  conn.id, _parser._SERVER_TO_CLIENT );
                }
                conns_to_close.emplace_back( id );
                continue;
            }
            if ( settings.readwritedebug ) {
                fmt::println( settings.log_fs,
                              "C{:03d}:{:04d}B:{}: packet read from server into buffer",
                              conn.id, bytes_read, _parser._SERVER_TO_CLIENT );
            }
            assert( !conn.server_buffer.empty() );
            const size_t bytes_parsed { _parser.logServerPackets( &conn ) };
            assert( bytes_parsed == bytes_read );
        } else if ( _socketWriteReady( conn.server_fd ) ) {
            assert( !conn.client_buffer.empty() );
            size_t bytes_written {};
            try {
                bytes_written = conn.forwardPacketToServer();
            } catch ( const std::system_error& e ) {
                if ( settings.readwritedebug ) {
                    fmt::println( settings.log_fs,
                                  "C{:03d}:{}: error writing to server: {}",
                                  conn.id, _parser._CLIENT_TO_SERVER, e.what() );
                }
                conns_to_close.emplace_back( id );
                continue;
            }
            assert( bytes_written > 0 );
            if ( settings.readwritedebug ) {
                fmt::println( settings.log_fs,
                              "C{:03d}:{:04d}B:{}: wrote packet from buffer to server",
                              conn.id, bytes_written, _parser._CLIENT_TO_SERVER );
            }
        } else if ( const auto error { _socketPollError( _listener_fd ) }; error ) {
            fmt::println(
                settings.log_fs,
                "C{:03d}: poll failure on server socket: {}",
                conn.id, *error );
            conns_to_close.emplace_back( id );
            continue;
        }
    }
    _closeConnections( conns_to_close );

    if ( _socketReadReady( _listener_fd ) ) {
        _openConnection();
    } else if ( const auto error { _socketPollError( _listener_fd ) }; error ) {
        fmt::println( stderr, "{}: poll failure on listening socket: {}",
                      __PRETTY_FUNCTION__, *error );
    }
}

void ProxyX11Server::_listenForClients() {
    // TBD TCP if AF_INET, but SOCK_STREAM still supported if AF_UNIX, see:
    //   https://oswalt.dev/2025/08/unix-domain-sockets/
    const int fd { socket( _in_display.family, SOCK_STREAM,
                           _SOCKET_DEFAULT_PROTOCOL ) };
    if ( fd < 0 )  {
        fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                      errors::system::message( "socket" ) );
        exit( EXIT_FAILURE );
    }
    socklen_t addr_sz {};
    switch ( _in_display.family ) {
    case AF_INET: {
        addr_sz = sizeof( _in_display.inaddr );
        // At sockets API level, enable sending of keep-alive messages on
        //   connection-oriented sockets.
        const int on { 1 };
        if ( setsockopt( fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on) ) != 0 ) {
            close( fd );
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::system::message( "setsockopt" ) );
            exit( EXIT_FAILURE );
        }
    }   break;
    case AF_UNIX:
        addr_sz = sizeof( _in_display.unaddr );
        // TBD should be deleted as part of both normal shutdown and signal
        //   interruption, here yet again for edge case of SIGKILL
        std::filesystem::remove( _in_display.unaddr.sun_path );
        break;
    default:
        break;
    }
    if ( bind( fd, &_in_display.addr, addr_sz ) < 0 ) {
        close( fd );
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::system::message( "bind" ) );
        exit( EXIT_FAILURE );
    }
    if ( listen( fd, _MAX_PENDING_CONNECTIONS ) < 0 ) {
        close( fd );
        fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                      errors::system::message( "listen" ) );
        exit( EXIT_FAILURE );
    }
    _listener_fd = fd;
}

// TBD two outputs:
//   - fd of client at top of pending connection stack for listener (c->client_fd)
//   - allocated string describing client (c->from) (x.x.x.x:port for AF_INET, socket file path or "unknown(local)" for AF_UNIX)
bool ProxyX11Server::_acceptClient( Connection* conn ) {
    assert( conn != nullptr );
    std::string client_desc;
    union {
        sockaddr addr;
        sockaddr_in inaddr;
        sockaddr_un unaddr {};
    };
    socklen_t addr_sz {};
    switch ( _in_display.family ) {
    case AF_INET: addr_sz = sizeof( inaddr ); break;
    case AF_UNIX: addr_sz = sizeof( unaddr ); break;
    default: break;
    }
    const int fd { accept( _listener_fd, &addr, &addr_sz ) };
    if ( fd < 0 ) {
        fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                      errors::system::message( "accept" ) );
        return false;
    }
    switch ( addr.sa_family ) {
    case AF_INET: {
        assert( addr_sz == sizeof( inaddr ) );
        char ipv4_addr[ INET_ADDRSTRLEN ] {};
        if ( inet_ntop( _in_display.family, &(inaddr.sin_addr),
                        ipv4_addr, INET_ADDRSTRLEN ) == nullptr ) {
            close( fd );
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::system::message( "inet_ntop" ) );
            return false;
        }
        client_desc = fmt::format( "{}:{}", ipv4_addr,
                                   ntohs( inaddr.sin_port ) );
    }   break;
    case AF_UNIX: {
        // TBD sun_path will likely always be unpopulated by connect(2), see:
        //   - https://stackoverflow.com/q/17090043
        //   we could call getsockname, but that would only provide the socket
        //   path given to bind(2) before listen(2) (would be the same for all
        //   clients connecting via _in_display, so not a great client id)
        assert( std::string_view( unaddr.sun_path ).empty() );
        client_desc = { "unknown(local)" };
    }   break;
    default:
        break;
    }
    conn->client_fd   = fd;
    conn->client_desc = client_desc;
    return true;
}

int ProxyX11Server::_connectToServer() {
    // TBD TCP if AF_INET, but SOCK_STREAM still supported if AF_UNIX, see:
    //   https://oswalt.dev/2025/08/unix-domain-sockets/
    const int fd { socket( _out_display.family, SOCK_STREAM,
                     _SOCKET_DEFAULT_PROTOCOL ) };
    if ( fd < 0 )  {
        fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                      errors::system::message( "socket" ) );
        return -1;
    }
    socklen_t addr_sz {};
    std::string connect_err;
    switch ( _out_display.family ) {
    case AF_INET: {
        // At sockets API level, enable sending of keep-alive messages on
        //   connection-oriented sockets.
        const int on { 1 };
        if ( setsockopt( fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on) ) != 0 ||
             setsockopt( fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on) ) != 0 ) {
            close( fd );
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::system::message( "setsockopt" ) );
            return -1;
        }
        addr_sz = sizeof( _out_display.inaddr );
        connect_err = fmt::format(
            "{}: error connecting to '{}' (resolved to '{}') for display '{}', {}",
            __PRETTY_FUNCTION__, _out_display.hostname,
            _out_display.ipv4_addr, _out_display.name,
            errors::system::message( "connect" ) );
    }   break;
    case AF_UNIX: {
        addr_sz = sizeof( _out_display.unaddr );
        connect_err = fmt::format(
            "{}: error connecting to unix socket '{}' for display '{}'), {}",
            __PRETTY_FUNCTION__, _out_display.unaddr.sun_path,
            _out_display.name, errors::system::message( "connect" ) );
    }   break;
    default:
        break;
    }
    if ( connect( fd, &_out_display.addr, addr_sz ) != 0 ) {
        close( fd );
        fmt::println( stderr, connect_err );
        return -1;
    }
    return fd;
}

void ProxyX11Server::_openConnection() {
    Connection conn {};
    if ( !_acceptClient( &conn ) ) {
        fmt::println( stderr, "{}: failure to accept client connection",
                      __PRETTY_FUNCTION__ );
        return;
    }
    assert( conn.client_fd > _listener_fd );
    assert( !conn.client_desc.empty() );
    fmt::println( stderr, "Connected to client: {}",
                  conn.client_desc );

    conn.server_fd = _connectToServer();
    if ( conn.server_fd == -1 ) {
        fmt::println( stderr, "{}: failure to connect to X server for display: {}",
                      __PRETTY_FUNCTION__, _out_display.name );
        conn.closeClientSocket();
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
                conn.id, conn.client_buffer.size(), _parser._CLIENT_TO_SERVER );
        }
        if ( !conn.server_buffer.empty() ) {
            fmt::println(
                settings.log_fs,
                "C{:03d}:{:04d}B:{}: discarded unsent buffer",
                conn.id, conn.server_buffer.size(), _parser._SERVER_TO_CLIENT );
        }
        _pfds_i_by_fd.erase( conn.client_fd );
        conn.closeClientSocket();
        _pfds_i_by_fd.erase( conn.server_fd );
        conn.closeServerSocket();
        _connections.erase( id );
    }
    // zip _pfds array to _pfds_i_by_fd keys
    _pfds.resize( _pfds_i_by_fd.size() );
    int i {};
    for ( auto& [ fd, pfds_i ] : _pfds_i_by_fd ) {
        _pfds[i].fd = fd;
        pfds_i = i;
        ++i;
    }
    assert( !_pfds_i_by_fd.empty() );
}

int ProxyX11Server::_processClientQueue() {
    _addSocketToPoll( _listener_fd, POLLPRI | POLLIN );

    static constexpr int NO_TIMEOUT { -1 };
    while ( child_running.load() || !_connections.empty() || settings.keeprunning ) {
        _updatePollFlags();
        // TBD blocks until polled fds have new events or interrupted by signal;
        //   most frequently expected signals: SIGINT (user), SIGCHLD (child exits)
        if ( poll( _pfds.data(), nfds_t( _pfds.size() ), NO_TIMEOUT ) == -1 ) {
            if ( errno != 0 && errno != EINTR ) {
                fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
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
