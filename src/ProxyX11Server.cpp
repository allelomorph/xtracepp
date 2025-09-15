#include <atomic>
#include <string>        // stoi to_string
#include <string_view>
#include <optional>
#include <filesystem>    // remove

#include <cassert>
#include <cstdlib>       // getenv setenv
#include <cstring>       // memcpy
#include <cstdint>

#include <sys/socket.h>  // 'struct sockaddr' SOCK_STREAM AF_INET AF_UNIX setsockopt SOL_SOCKET SO_KEEPALIVE 
#include <arpa/inet.h>   // htons htonl inet_ntoa ntohs
#include <netinet/in.h>  // 'struct sockaddr_in'
#include <sys/un.h>      // 'struct sockaddr_un'
#include <unistd.h>      // unlink close pid_t fork STDERR_FILENO STDIN_FILENO
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
    if ( info->si_code == CLD_EXITED ) {
        child_retval.store( int8_t( info->si_status ) );
    } else {  // info->si_code == CLD_KILLED, CLD_STOPPED, CLD_DUMPED, etc
        child_retval.store( -int8_t( info->si_status ) );
    }
}

ProxyX11Server::~ProxyX11Server() {
    close( _listener_fd );
    if ( !_in_display.unix_socket_path.empty() )
        unlink( _in_display.unix_socket_path.data() );
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
    _out_display = DisplayInfo( out_displayname );

    const char* in_displayname { nullptr };
    if( settings.in_displayname != nullptr ) {
        in_displayname = settings.in_displayname;
    } else {
        in_displayname = getenv( _IN_DISPLAYNAME_ENV_VAR.data() );
        if ( in_displayname == nullptr ) {
            fmt::print(
                stderr,
                "No display specified via --proxydisplay or {}, defaulting to {}\n",
                _IN_DISPLAYNAME_ENV_VAR, _DEFAULT_IN_DISPLAYNAME );
            in_displayname = _DEFAULT_IN_DISPLAYNAME.data();
        }
    }
    assert( in_displayname != nullptr );
    _in_display = DisplayInfo( in_displayname );
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
    uint16_t family          {};
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
    assert( !_in_display.unparsed_name.empty() &&
            !_out_display.unparsed_name.empty() );

    ////// get auth path

    static constexpr std::string_view XAUTHORITY_ENV_VAR { "XAUTHORITY" };
    static constexpr std::string_view HOME_ENV_VAR { "HOME" };
    static constexpr std::string_view XAUTHORITY_DEFAULT_FILENAME { ".Xauthority" };

    std::string xauth_path {};
    const char* xauthority_value { getenv( XAUTHORITY_ENV_VAR.data() ) };
    if ( xauthority_value == nullptr ) {
        const char* home_value { getenv( HOME_ENV_VAR.data() ) };
        if ( home_value == nullptr ) {
            fmt::println(
                stderr, "Could not get HOME environmental variable value to resolve auth path" );
            exit( EXIT_FAILURE );
        }
        xauth_path =
            fmt::format( "{}/{}", home_value, XAUTHORITY_DEFAULT_FILENAME );
    } else {
        xauth_path = std::string( xauthority_value );
    }

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
        ifs.read( (char*)&auth.family, sizeof(auth.family) );

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

    _XAuthInfo* in_display_auth {};  // FAKEDISPLAY
    _XAuthInfo* out_display_auth {};  // DISPLAY
    for ( _XAuthInfo& auth : xauth_entries ) {
        if ( std::strtol( auth.display, nullptr, 10 ) == _in_display.display ) {
            assert( auth.family == _in_display.family );
            assert( auth.name == _AUTH_NAME );
            assert( auth.data_len == _AUTH_DATA_SZ );
            in_display_auth = &auth;
        }
        if ( std::strtol( auth.display, nullptr, 10 ) == _out_display.display ) {
            // TBD DisplayInfo() will set family to 2/AF_INET even if DISPLAY starts with "localhost"
            // TBD should we update the xtrace parsing of DISPLAY?
            //   - localhost:n.m values should have hostname localhost, protocol local, family AF_UNIX
            //   - will this break socket programming? theoretically ssh -X is already handling
            //     forwarding X11 comms sent to DISPLAY to the remote server
            // TBD X11 over TCP without ssh is deprecated, but we should update DisplayInfo() regex to
            //   accept URIs and not just "localhost", see:
            //   - https://github.com/mviereck/x11docker/wiki/How-to-access-X-over-TCP-IP-network
            //assert( auth.family == _out_display.family );
            if ( auth.name != _AUTH_NAME ) {
                fmt::println(
                    stderr, "No support for display \"{}\" auth method {} (expected {})",
                    _out_display.unparsed_name, auth.name, _AUTH_NAME );
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
                      _out_display.unparsed_name );
        exit( EXIT_FAILURE );
    }
    if ( in_display_auth != nullptr ) {  // revise existing FAKEDISPLAY entry
        memcpy( in_display_auth->data, out_display_auth->data, _AUTH_DATA_SZ );
    } else {                             // append new FAKEDISPLAY entry
        _XAuthInfo auth { *out_display_auth };
        // TBD hostname/addr may also be distinct from DISPLAY when using tcp with no ssh
        std::string display_str { fmt::format( "{}", _in_display.display ) };
        auth.display_len = display_str.size();
        auth.display_len_nbo = htons( auth.display_len );
        memcpy( auth.display, display_str.data(), display_str.size() );
        auth.display[auth.display_len] = '\0';
        xauth_entries.emplace_back( auth );
    }
    // TBD for later use in atom prefetching
    memcpy( this->_auth_data, out_display_auth->data, _AUTH_DATA_SZ );

    ////// write auth entries back to file

    std::ofstream ofs( xauth_path, std::ios::binary );
    if ( !ofs.good() ) {
        fmt::println(
            stderr, "Could not open auth file for writing, expected paths: ${} or ${}/{}",
            XAUTHORITY_ENV_VAR, HOME_ENV_VAR, XAUTHORITY_DEFAULT_FILENAME );
        exit( EXIT_FAILURE );
    }
    for ( const _XAuthInfo& auth : xauth_entries ) {
        ofs.write( (char*)&auth.family,
                   sizeof(auth.family) );

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

// TBD inaddr.sin_port never changes, can be part of DisplayInfo
//    (really the sockaddr_in and sockaddr_un both do not change, and can be set once only (static const set by lambda?))
//    (less crucial here as this will only be called once per process execution)
void ProxyX11Server::_listenForClients() {
    /*struct */sockaddr* address;
    std::size_t address_sz;

    // open sequenced, reliable, two-way, connection-based byte stream
    static constexpr int SOCKET_DEFAULT_PROTOCOL { 0 };
    const int fd { socket( _in_display.family, SOCK_STREAM, SOCKET_DEFAULT_PROTOCOL ) };
    if ( fd < 0 )  {
        // TBD exception?
        fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                      errors::system::message( "socket" ) );
        exit( EXIT_FAILURE );
    }
    assert( _in_display.family == AF_INET || _in_display.family == AF_UNIX );
    if ( _in_display.family == AF_INET ) {
        // At sockets API level, enable sending of keep-alive messages on
        //   connection-oriented sockets.
        const int on { 1 };
        if ( setsockopt( fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on) ) != 0 ) {
            close( fd );
            // TBD exception
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::system::message( "setsockopt" ) );
            exit( EXIT_FAILURE );
        }
        /*struct */sockaddr_in inaddr;
        // use domain of IPv4 Internet protocol
        inaddr.sin_family      = _in_display.family;
        // use port 6000 + display number (network byte order)
        inaddr.sin_port        = htons( _X_TCP_PORT + _in_display.display ); // TBD calculateTCPport();
        // bind socket to all local interfaces (network byte order)
        inaddr.sin_addr.s_addr = htonl( INADDR_ANY );
        address    = reinterpret_cast< struct sockaddr* >( &inaddr );
        address_sz = sizeof( inaddr );
    } else {  // _in_display.family == AF_UNIX
        /*struct */sockaddr_un unaddr;
        // Unix domain (local communication)
        unaddr.sun_family = _in_display.family;
        std::memcpy( unaddr.sun_path, _in_display.unix_socket_path.data(),
                     _in_display.unix_socket_path.size() + 1 );
        // TBD necessary if deleted as part of shutdown?
        std::filesystem::remove( unaddr.sun_path );
        address    = reinterpret_cast< struct sockaddr* >( &unaddr );
        address_sz = sizeof( unaddr );
    }

    if ( bind( fd, address, address_sz ) < 0 ) {
        close( fd );
        // TBD exception
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::system::message( "bind" ) );
        exit( EXIT_FAILURE );
    }

    if ( listen( fd, _MAX_PENDING_CONNECTIONS ) < 0 ) {
        close( fd );
        // TBD exception
        fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                      errors::system::message( "listen" ) );
        exit( EXIT_FAILURE );
    }

    _listener_fd = fd;
    _open_fds.emplace( _listener_fd );
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

// TBD this is called as part of openConnection, and effectively has two outputs:
//   - fd of client at top of pending connection stack for listener (c->client_fd)
//   - allocated string describing client (c->from) (x.x.x.x:port for AF_INET, socket file path or "unknown(local)" for AF_UNIX)
bool ProxyX11Server::_acceptClient(Connection* conn) {
    int fd;
    socklen_t len;
    std::string client_desc;

    assert( conn != nullptr );
    assert( _in_display.family == AF_INET || _in_display.family == AF_UNIX );
    // TBD is the name string always going to be the same for _listener_fd? Is it something
    //   we could just get once during _in_display init? maybe getsockname on _listener_fd after successful bind?
    if ( _in_display.family == AF_INET ) {
        /*struct */sockaddr_in inaddr;
        len = sizeof( inaddr );
        fd = accept( _listener_fd,
                     reinterpret_cast< struct sockaddr* >( &inaddr ), &len );
        if ( fd < 0 ) {
            // TBD exception
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::system::message( "accept" ) );
            return false;
        }
        char client_addrstr[INET_ADDRSTRLEN] { 0 };
        if ( inet_ntop( _in_display.family, &(inaddr.sin_addr),
                        client_addrstr, INET_ADDRSTRLEN ) == nullptr ) {
            close( fd );
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::system::message( "inet_ntop" ) );
            return false;
        }
        client_desc = std::string( client_addrstr ) + ':' +
            std::to_string( ntohs( inaddr.sin_port ) );
    } else {  // _in_display.family == AF_UNIX
        /*struct */sockaddr_un unaddr;
        len = sizeof( unaddr );
        fd = accept( _listener_fd,
                     reinterpret_cast< struct sockaddr* >( &unaddr ), &len );
        if ( fd < 0 ) {
            // TBD exception
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::system::message( "accept" ) );
            return false;
        }
        if ( len > sizeof( sa_family_t ) )  // flexible array unaddr.sun_path len > 0
            client_desc = std::string( unaddr.sun_path, len - sizeof( sa_family_t ) );
        else
            client_desc = std::string( "unknown(local)" );
    }
    conn->client_fd   = fd;
    conn->client_desc = client_desc;
    return true;
}

// TBD addr.sin_port never changes, can be part of DisplayInfo or static func var
//    (really the sockaddr_in and sockaddr_un both do not change, and can be set once only (static const set by lambda?))
// connectToServer(out_displayname,out_family,out_hostname,out_display)
int ProxyX11Server::_connectToServer() {
    // TCP socket to connect to X server as client
    int fd { socket( _out_display.family, SOCK_STREAM, 0 ) };
    if( fd < 0 )  {
        // TBD exeception
        fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                      errors::system::message( "socket" ) );
        return -1;
    }
    assert( _out_display.family == AF_INET || _out_display.family == AF_UNIX );
    if( _out_display.family == AF_INET ) {
        /*struct */addrinfo    hints  {};
        hints.ai_family   = _out_display.family;
        hints.ai_socktype = SOCK_STREAM;
        /*struct */addrinfo*   res    {};
        // TBD this call to getaddrinfo only needs to happen once (?), in _out_display init
        const int gai_ret {
            getaddrinfo( _out_display.hostname.data(), NULL, &hints, &res ) };
        if ( gai_ret != 0 ) {
            close( fd );
            // TBD exception
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::system::message( "getaddrinfo" ) );
            return -1;
        }
        assert( res != nullptr );
        /*struct */sockaddr_in inaddr {};
        assert( res->ai_addrlen == sizeof( inaddr ) );
        memcpy( &inaddr, res->ai_addr, res->ai_addrlen );
        freeaddrinfo( res );
        inaddr.sin_port = htons( _X_TCP_PORT + _out_display.display ); // TBD calculateTCPport();
        const int on { 1 };
        setsockopt( fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof( on ) );
        if( connect( fd, ( struct sockaddr* )&inaddr, sizeof( inaddr ) ) < 0 ) {
            close( fd );
            // TBD no exception? this seems like a non-fatal error to report to normal user
            char server_addrstr[INET_ADDRSTRLEN] { 0 };
            fmt::println(
                stderr,
                "{}: error connecting to '{}' (resolved to '{}') for display '{}', {}",
                __PRETTY_FUNCTION__, _out_display.hostname,
                inet_ntop( inaddr.sin_family, &(inaddr.sin_addr),
                           server_addrstr, INET_ADDRSTRLEN ),
                _out_display.name,
                errors::system::message( "connect" ) );
            return -1;
        }
    } else {
        /*struct */sockaddr_un unaddr;
        // TBD no need to set unaddr.sun_family?
        memcpy( unaddr.sun_path, _out_display.unix_socket_path.data(),
                _out_display.unix_socket_path.size() + 1 );

        if ( connect( fd, ( struct sockaddr* )&unaddr, sizeof( unaddr ) ) < 0 ) {
            close( fd );
            // TBD no exception? this seems like a non-fatal error to report to normal user
            fmt::println(
                stderr,
                "{}: error connecting to unix socket '{}' for display '{}'), {}",
                __PRETTY_FUNCTION__, unaddr.sun_path, _out_display.name,
                errors::system::message( "connect" ) );
            return -1;
        }
    }
    return fd;
}

void ProxyX11Server::_addSocketToPoll( const int fd, const short events/* = _POLLNONE*/ ) {
    assert( _pfds_i_by_fd.find( fd ) == _pfds_i_by_fd.end() );
    _pfds.emplace_back( pollfd{ fd, events, 0 } );
    _pfds_i_by_fd.emplace( fd, _pfds.size() - 1 );
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

void ProxyX11Server::_openConnection() {
    Connection conn {};
    if ( !_acceptClient( &conn ) ) {
        // TBD exception
        fmt::println( stderr, "{}: failure to accept client connection",
            __PRETTY_FUNCTION__ );
        return;
    }
    assert( conn.client_fd > _listener_fd );
    assert( !conn.client_desc.empty() );
    fmt::println( stderr, "Connected to client: {}",
                  conn.client_desc );

    conn.server_fd = _connectToServer();
    if( conn.server_fd < 0 ) {
        // TBD exception
        fmt::println(
            stderr, "{}: failure to connect to X server for display: {}",
            __PRETTY_FUNCTION__, _out_display.name );
        conn.closeClientSocket();
        return;
    }
    assert( conn.server_fd > _listener_fd );

    _connections.emplace( conn.id, conn );
    _addSocketToPoll( conn.client_fd );
    _addSocketToPoll( conn.server_fd );
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

int ProxyX11Server::_processClientQueue() {
    _addSocketToPoll( _listener_fd, POLLPRI | POLLIN );

    static constexpr int NO_TIMEOUT { -1 };
    while ( child_running.load() || !_connections.empty() || settings.keeprunning ) {
        _updatePollFlags();
        // TBD hangs until polled fds have new events or interrupted by signal;
        //   expected signals: SIGINT (user) or SIGCHLD (child exits)
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
    return _child_used ? child_retval.load() : EXIT_SUCCESS;
}
