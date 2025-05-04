#include <iostream>  // TBD testing only
#include <iomanip>   // TBD only if log_os remains C++ output stream

#include <string>        // stoi to_string
#include <algorithm>     // transform min
#include <regex>         // smatch regex regex_search
#include <filesystem>    // remove
//#include <optional>

#include <cassert>
#include <cstdlib>       // getenv setenv
#include <cstring>       // memcpy

#include <sys/socket.h>  // 'struct sockaddr' SOCK_STREAM AF_INET AF_UNIX setsockopt SOL_SOCKET SO_KEEPALIVE 
#include <arpa/inet.h>   // htons htonl inet_ntoa ntohs
#include <netinet/in.h>  // 'struct sockaddr_in'
#include <sys/un.h>      // 'struct sockaddr_un'
#include <unistd.h>      // unlink close pid_t fork STDERR_FILENO STDIN_FILENO
#include <sys/wait.h>    // waitpid WNOHANG WIFEXITED WEXITSTATUS WTERMSIG
#include <signal.h>      // sigaction
// #ifndef _GNU_SOURCE
//   #define _GNU_SOURCE      // asprintf
// #endif
//#include <stdio.h>       // asprintf

//#include <sys/types.h>
#include <netdb.h> // 'struct addrinfo' getaddrinfo freeaddrinfo

#include <fmt/format.h>

#include "ProxyX11Server.hpp"
#include "Connection.hpp"
#include "errors.hpp"


volatile bool caught_SIGCHLD { false };

void catchSIGCHLD(int signum) {
    caught_SIGCHLD = true;
}

void ProxyX11Server::__debugOutput() {
    std::string subcmd_argv {};
    for (int i {}; i < settings.subcmd_argc; ++i)
        subcmd_argv += fmt::format( R"("{}" )", settings.subcmd_argv[i] );
    std::cerr << fmt::format(
        R"(settings:
    readwritedebug:      {}
    stopifnoactiveconnx: {}
    waitforclient:       {}
    denyallextensions:   {}
    out_displayname:     "{}"
    in_displayname:      "{}"
    subcmd_argc:         {}
    subcmd_argv:         [ {}]
)",
        settings.readwritedebug,
        settings.stopifnoactiveconnx,
        settings.waitforclient,
        settings.denyallextensions,
        ( settings.out_displayname == nullptr ) ? "(null)" : settings.out_displayname,
        ( settings.in_displayname == nullptr ) ? "(null)" : settings.in_displayname,
        settings.subcmd_argc,
        subcmd_argv );
    std::cerr << fmt::format(
        R"(_in_display:
    name:             "{}"
    protocol:         "{}"
    hostname:         "{}"
    display:          "{}"
    screen:           {}
    family:           {}
    unix_socket_path: "{}"
)",
        _in_display.name,
        _in_display.protocol,
        _in_display.hostname,
        _in_display.display,
        _in_display.screen,
        _in_display.family,
        _in_display.unix_socket_path );
    std::cerr << fmt::format(
        R"(_out_display:
    name:             "{}"
    protocol:         "{}"
    hostname:         "{}"
    display:          "{}"
    screen:           {}
    family:           {}
    unix_socket_path: "{}"
)",
        _out_display.name,
        _out_display.protocol,
        _out_display.hostname,
        _out_display.display,
        _out_display.screen,
        _out_display.family,
        _out_display.unix_socket_path );
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
            std::cerr << fmt::format(
                "No display specified via --proxydisplay or {}, defaulting to {}\n",
                _IN_DISPLAYNAME_ENV_VAR, _DEFAULT_IN_DISPLAYNAME );
            in_displayname = _DEFAULT_IN_DISPLAYNAME.data();
        }
    }
    assert( in_displayname != nullptr );
    _in_display = DisplayInfo( in_displayname );
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
        std::cerr << errors::system::message(
            fmt::format( "{}: socket", __PRETTY_FUNCTION__ )
            ) << std::endl;
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
            std::cerr << errors::system::message(
                fmt::format( "{}: setsockopt", __PRETTY_FUNCTION__ )
                ) << std::endl;
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
        std::cerr << errors::system::message(
            fmt::format( "{}: bind", __PRETTY_FUNCTION__ )
            ) << std::endl;
        exit( EXIT_FAILURE );
    }

    if ( listen( fd, _MAX_PENDING_CONNECTIONS ) < 0 ) {
        close( fd );
        // TBD exception
        std::cerr << errors::system::message(
            fmt::format( "{}: listen", __PRETTY_FUNCTION__ )
            ) << std::endl;
        exit( EXIT_FAILURE );
    }

    _listener_fd = fd;
    _open_fds.emplace( _listener_fd );
}

void ProxyX11Server::_startSubcommandClient() {
    if ( settings.subcmd_argc == 0 )
        return;

    // While `struct` can be omitted when using C structs as C++ types, here it
    //   is necessary to disambiguate the sigaction struct and function
    struct sigaction act {};
    act.sa_handler = &catchSIGCHLD;
    if ( sigaction( SIGCHLD, &act, nullptr ) == -1 ) {
        // TBD exception
        std::cerr << errors::system::message(
            fmt::format( "{}: sigaction", __PRETTY_FUNCTION__ )
            ) << std::endl;
        exit( EXIT_FAILURE );
    }

    _child_pid = fork();
    if( _child_pid == -1 ) {  // fork failed, still in parent
        // TBD exception
        std::cerr << errors::system::message(
            fmt::format( "{}: fork", __PRETTY_FUNCTION__ )
            ) << std::endl;
        exit( EXIT_FAILURE );
    }
    if( _child_pid == 0 ) {   // fork succeeded, in child
        if ( setenv( _OUT_DISPLAYNAME_ENV_VAR.data(),
                     _in_display.name.data(), 1 ) != 0 ) {
            // TBD exception
            std::cerr << errors::system::message(
                fmt::format( "{}: setenv", __PRETTY_FUNCTION__ )
                ) << std::endl;
            exit( EXIT_FAILURE );
        }
        execvp( settings.subcmd_argv[0], settings.subcmd_argv );
        // child has failed to overtake parent process
        // TBD exception
        std::cerr << errors::system::message(
            fmt::format( "{}: execvp", __PRETTY_FUNCTION__ )
            ) << std::endl;
        exit( EXIT_FAILURE );
    }
    assert( _child_pid != 0 );
}

// TBD this is called as part of acceptConnection, and effectively has two outputs:
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
            std::cerr << errors::system::message(
                fmt::format( "{}: accept", __PRETTY_FUNCTION__ )
                ) << std::endl;
            return false;
        }
        char client_addrstr[INET_ADDRSTRLEN] { 0 };
        if ( inet_ntop( _in_display.family, &(inaddr.sin_addr),
                        client_addrstr, INET_ADDRSTRLEN ) == nullptr ) {
            close( fd );
            std::cerr << errors::system::message(
                fmt::format( "{}: inet_ntop", __PRETTY_FUNCTION__ )
                ) << std::endl;
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
            std::cerr << errors::system::message(
                fmt::format( "{}: accept", __PRETTY_FUNCTION__ )
                ) << std::endl;
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
        std::cerr << errors::system::message(
            fmt::format( "{}: socket", __PRETTY_FUNCTION__ )
            ) << std::endl;
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
            std::cerr << errors::getaddrinfo::message(
                gai_ret, fmt::format( "{}: getaddrinfo", __PRETTY_FUNCTION__ )
                ) << std::endl;
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
            std::cerr << errors::system::message(
                fmt::format(
                    "{}: error connecting to '{}' (resolved to '{}') for display '{}' connect",
                    __PRETTY_FUNCTION__, _out_display.hostname,
                    inet_ntop( inaddr.sin_family, &(inaddr.sin_addr),
                               server_addrstr, INET_ADDRSTRLEN ),
                    _out_display.name )
                ) << std::endl;
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
            std::cerr << errors::system::message(
                fmt::format (
                    "{}: error connecting to unix socket '{}' for display '{}') connect",
                    __PRETTY_FUNCTION__, unaddr.sun_path, _out_display.name )
                ) << std::endl;
            return -1;
        }
    }
    return fd;
}

void ProxyX11Server::_acceptConnection() {
    Connection conn {};

    if ( !_acceptClient( &conn ) ) {
        // TBD exception
        std::cerr << fmt::format(
            "{}: failure to accept client connection",
            __PRETTY_FUNCTION__ ) << std::endl;
        return;
    }
    assert( conn.client_fd > _listener_fd );
    assert( !conn.client_desc.empty() );
    // TBD why is a global user setting arbitrarily set here, at every new connection?
    //   would it not be better somewhere in mainqueue before the first call to acceptConnection?
    settings.waitforclient = false;
    std::cerr << "Connected to client: " << conn.client_desc << std::endl;

    conn.server_fd = _connectToServer();
    if( conn.server_fd < 0 ) {
        // TBD exception
        std::cerr << fmt::format(
            "{}: failure to connect to X server for display: {}",
            __PRETTY_FUNCTION__, _out_display.name ) << std::endl;
        conn.closeClientSocket();
        return;
    }
    assert( conn.server_fd > _listener_fd );

    _connections.emplace( conn.id, conn );
    _open_fds.emplace( conn.client_fd );
    _open_fds.emplace( conn.server_fd );
}

// !!! caret notation: server <> client
//   000:<:received 48 bytes    (buffered 48 bytes from client on connection 0)
//   000:<:wrote 48 bytes       (forwarded 48 bytes from client to x server on connection 0)
//   000:>:received 6740 bytes  (buffered 6740 bytes from x server on connection 0)
//   000:>:wrote 6740 bytes     (forwarded 6740 bytes from x server to client on connection 0)

// returns max_fd (max_fd + 1 becomes nfds for select)
/*
 * first loop over connections to:
 *   - set client and server sockets into readfds, writefds, exceptfds
 *   - mark closed connections for deletion
 */
int ProxyX11Server::_prepareSocketFlagging( fd_set* readfds, fd_set* writefds,
                                            fd_set* exceptfds ) {
    assert( readfds != nullptr );
    assert( writefds != nullptr );
    assert( exceptfds != nullptr );

    static std::vector<int> closed_connection_ids;
    closed_connection_ids.clear();

    // fd_sets are modified in place by select(2), and need to be zeroed out before each use
    FD_ZERO( readfds );
    FD_ZERO( writefds );
    FD_ZERO( exceptfds );
    // monitor _listener_fd for input (read readiness)
    FD_SET( _listener_fd, readfds );

    for ( auto& [ id, conn ] : _connections ) {
        if ( conn.clientSocketIsOpen() ) {
            if ( conn.serverSocketIsClosed() && conn.server_buffer.empty() ) {
                _open_fds.erase( conn.client_fd );
                conn.closeClientSocket();
                if ( settings.readwritedebug ) {
                    settings.log_os << fmt::format(
                        "{:03d}:>:sent EOF\n", conn.id );
                }
            } else {
                // monitor client socket for exceptional conditions (see poll(2) POLLPRI)
                FD_SET( conn.client_fd, exceptfds );

                // TBD having SocketBuffer that is extenible and supports appending reads means that
                //   we can always be ready to read
                if ( conn.client_buffer.empty() ) {
                    // monitor client socket for input (read readiness)
                    FD_SET( conn.client_fd, readfds );
                }
                if ( !conn.server_buffer.empty() ) {
                    // monitor client socket for output (write readiness)
                    FD_SET( conn.client_fd, writefds );
                }
            }
        }
        if ( conn.serverSocketIsOpen() ) {
            if ( conn.clientSocketIsClosed() && conn.client_buffer.empty() ) {
                _open_fds.erase( conn.server_fd );
                conn.closeServerSocket();
                if ( settings.readwritedebug ) {
                    settings.log_os << fmt::format(
                        "{:03d}:<:sent EOF\n", conn.id );
                }
            } else {
                // monitor client socket for exceptional conditions (see poll(2) POLLPRI)
                FD_SET( conn.server_fd, exceptfds );

                if ( conn.server_buffer.empty() ) {
                    // monitor server socket for input (read readiness)
                    FD_SET( conn.server_fd, readfds );
                }
                if ( !conn.client_buffer.empty() ) {
                    // monitor server socket for output (write readiness)
                    FD_SET( conn.server_fd, writefds );
                }
            }
        }

        if ( conn.clientSocketIsClosed() && conn.serverSocketIsClosed() ) {
            closed_connection_ids.emplace_back( conn.id );
        }
    }

    // TBD assuming we cannot erase while iterating in above loop
    for ( const int id : closed_connection_ids ) {
        _connections.erase( id );
    }

    assert( !_open_fds.empty() );
    return *( _open_fds.begin() );
}

// TBD need to incorporate read/write of 0 (EOF) into logic
/*
 * second loop over connections to:
 *   - receive new packets
 *   - parse current packets
 *   - send current packets
 */
void ProxyX11Server::_processFlaggedSockets( fd_set* readfds, fd_set* writefds,
                                             fd_set* exceptfds ) {
    assert( readfds != nullptr );
    assert( writefds != nullptr );
    assert( exceptfds != nullptr );

    for ( auto& [ id, conn ] : _connections ) {
        if ( conn.clientSocketIsOpen() ) {
            if ( FD_ISSET( conn.client_fd, exceptfds ) ) {
                _open_fds.erase( conn.client_fd );
                conn.closeClientSocket();
                settings.log_os << fmt::format(
                    "{:03d}: exceptional condition in communication with client\n",
                    conn.id );
                continue;
            }
            if ( FD_ISSET( conn.client_fd, writefds ) ) {
                // TBD we listen for client write readiness only if server buffer is not empty
                assert( !conn.server_buffer.empty() );
                size_t bytes_written {};
                try {
                    bytes_written = conn.forwardPacketToClient();  // conn.server_buffer.write( conn.client_fd )
                } catch ( const std::system_error& e ) {
                    if ( settings.readwritedebug ) {
                        settings.log_os << fmt::format(
                            "{:03d}:>:error writing to client: {}\n",
                            conn.id, e.code().message() );
                    }
                    _open_fds.erase( conn.client_fd );
                    conn.closeClientSocket();
                    continue;
                }
                assert( bytes_written > 0 );
                // TBD should already be parsed, immediately after read
                if ( settings.readwritedebug ) {
                    settings.log_os << fmt::format(
                        "{:03d}:>:wrote    {:4d} bytes\n",
                        conn.id, bytes_written );
                }
            }
            if ( FD_ISSET( conn.client_fd, readfds ) ) {
                size_t bytes_read {};
                try {
                    bytes_read = conn.bufferPacketFromClient();  // conn.client_buffer.read( conn.client_fd )
                } catch ( const std::system_error& e ) {
                    if ( settings.readwritedebug ) {
                        settings.log_os << fmt::format(
                            "{:03d}:<:error reading from client buffer: {}\n",
                            conn.id, e.code().message() );
                    }
                    _open_fds.erase( conn.client_fd );
                    conn.closeClientSocket();
                    continue;
                }
                if ( bytes_read == 0 ) {
                    if ( settings.readwritedebug ) {
                        settings.log_os << fmt::format(
                            "{:03d}:<:got EOF\n", conn.id );
                    }
                    _open_fds.erase( conn.client_fd );
                    conn.closeClientSocket();
                    continue;
                }
                if ( settings.readwritedebug ) {
                    settings.log_os << fmt::format(
                        "{:03d}:<:received {:4d} bytes\n",
                        conn.id, bytes_read );
                }
                assert( !conn.client_buffer.empty() );
                // TBD parse immediately after reading, as we may need to alter contents
                size_t bytes_parsed { parser.logClientPackets( &conn, &settings ) };
                assert( bytes_parsed == bytes_read );
            }
        }
        if ( conn.clientSocketIsClosed() && !conn.server_buffer.empty() ) {
            settings.log_os << fmt::format(
                "{:03d}:>: discarded {} bytes sent from server to client\n",
                conn.id, conn.server_buffer.size() );
        }
        if ( conn.serverSocketIsOpen() ) {
            if ( FD_ISSET( conn.server_fd, exceptfds ) ) {
                _open_fds.erase( conn.server_fd );
                conn.closeServerSocket();
                settings.log_os << fmt::format(
                    "{:03d}: exceptional condition in communication with server\n",
                    conn.id );
                continue;
            }
            if ( FD_ISSET( conn.server_fd, writefds ) ) {
                // TBD we listen for server write readiness only if client buffer is not empty
                assert( !conn.client_buffer.empty() );
                size_t bytes_written {};
                try {
                    bytes_written = conn.forwardPacketToServer();  // conn.client_buffer.write( conn.server_fd )
                } catch ( const std::system_error& e ) {
                    if ( settings.readwritedebug ) {
                        settings.log_os << fmt::format(
                            "{:03d}:<:error writing to server: {}\n",
                            conn.id, e.code().message() );
                    }
                    _open_fds.erase( conn.server_fd );
                    conn.closeServerSocket();
                    continue;
                }
                assert( bytes_written > 0 );
                // TBD should already be parsed, immediately after read
                if ( settings.readwritedebug ) {
                    settings.log_os << fmt::format(
                        "{:03d}:<:wrote    {:4d} bytes\n",
                        conn.id, bytes_written );
                }
            }
            if ( FD_ISSET( conn.server_fd, readfds ) ) {
                size_t bytes_read {};
                try {
                    bytes_read = conn.bufferPacketFromServer();  // conn.server_buffer.read( conn.server_fd )
                } catch ( const std::system_error& e ) {
                    if ( settings.readwritedebug ) {
                        settings.log_os << fmt::format(
                            "{:03d}:>:error reading from server buffer: {}\n",
                            conn.id, e.code().message() );
                    }
                    _open_fds.erase( conn.server_fd );
                    conn.closeServerSocket();
                    continue;
                }
                if ( bytes_read == 0 ) {
                    if ( settings.readwritedebug ) {
                        settings.log_os << fmt::format(
                            "{:03d}:>:got EOF\n", conn.id );
                    }
                    _open_fds.erase( conn.server_fd );
                    conn.closeServerSocket();
                    continue;
                }
                if ( settings.readwritedebug ) {
                    settings.log_os << fmt::format(
                        "{:03d}:>:received {:4d} bytes\n",
                        conn.id, bytes_read );
                }
                assert( !conn.server_buffer.empty() );
                // TBD parse immediately after reading, as we may need to alter contents
                size_t bytes_parsed { parser.logServerPackets( &conn, &settings ) };
                assert( bytes_parsed == bytes_read );
            }
        }
        if ( conn.serverSocketIsClosed() && !conn.client_buffer.empty() ) {
            settings.log_os << fmt::format(
                "{:03d}:<: discarded {} bytes sent from client to server\n",
                conn.id, conn.client_buffer.size() );
        }
    }

    // new connections are driven by listener socket accept(2)ing clients
    if ( FD_ISSET( _listener_fd, readfds ) ) {
        _acceptConnection();
    }
}

// TBD mainqueue() retval is passed as retval of xtrace
// TBD notice printing to stdout, stderr and out (may be stdout)
int ProxyX11Server::_processClientQueue() {
    static constexpr int CHILD_EXITED { 0 };   // sentinel for child pid

    // NOTE: FD_SET and FD_CLR are idempotent
    fd_set readfds, writefds, exceptfds;
    for ( int select_ret {}; true; ) {
        /*
         * first loop over connections to:
         *   - set client and server sockets into readfds, writefds, exceptfds
         *   - delete closed connections
         */
        const int max_fd {
            _prepareSocketFlagging( &readfds, &writefds, &exceptfds ) };

        // If no connections left and subprocess has exited: exit
        // TBD stopifnoactiveconnx true by default, name opposite? continueifnoconnx?
        if ( _connections.empty() && settings.stopifnoactiveconnx &&
             _child_pid == CHILD_EXITED ) {
            return EXIT_SUCCESS;
        }

        // TBD run in separate thread? https://stackoverflow.com/a/11679770
        /*
         * Check if subprocess finished (child from cli `--`)
         */
        // If child has not exited, and select failed on previous loop when SIGCHLD caught
        // TBD is checking select_ret == -1 redundant here, as we are already catching the signal?
        if ( _child_pid != CHILD_EXITED &&
             ( select_ret == -1 || caught_SIGCHLD ) ) {
            caught_SIGCHLD = false;
            int status;
            // TBD consider sigaction and SA_NOCLDWAIT:
            //   - https://unix.stackexchange.com/a/616607
            // Check to see if child exited, but do not hang (wait until so)
            if ( waitpid( _child_pid, &status, WNOHANG ) == _child_pid ) {
                _child_pid = CHILD_EXITED;
                if ( _connections.empty() && !settings.waitforclient ) {
                    /* TODO: instead wait a bit before terminating? */
                    // Return child exit code or number of signal that terminated
                    // TBD why +128?
                    if ( WIFEXITED( status ) )
                        return WEXITSTATUS( status );
                    else  // WIFSIGNALED( status ) == true
                        return WTERMSIG( status ) + 128;
                }
            }
        }

        /*
         * Idle until any of the fds in the provided fd_sets becomes ready for
         *   reading, writing, or indicating exceptional conditions
         */
        // select(2) expects first param to be highest fd in any of readfds, writefds, exceptfds + 1
        select_ret = select( max_fd + 1, &readfds, &writefds, &exceptfds, NULL );
        // TBD Note that:
        //   - select failure resets but does not break loop
        //   - SIGINT forces select failure, but does not print error
        //   - EINTR for select(2) means signal was caught (likely SIGCHLD)
        if ( select_ret == -1 ) {
            if ( errno != 0 && errno != EINTR ) {
                std::cerr << errors::system::message(
                    fmt::format( "{}: select", __PRETTY_FUNCTION__ )
                    ) << std::endl;
                return EXIT_FAILURE;
            }
            // TBD SIGINT or SIGCHLD during select
            continue;
        }

        /*
         * second loop over connections to:
         *   - receive new packets
         *   - parse current packets
         *   - send current packets
         */
        _processFlaggedSockets( &readfds, &writefds, &exceptfds );
    }

    return EXIT_SUCCESS;
}

ProxyX11Server::ProxyX11Server() {
}

ProxyX11Server::~ProxyX11Server() {
    close( _listener_fd );
    if ( !_in_display.unix_socket_path.empty() )
        unlink( _in_display.unix_socket_path.data() );
}

void ProxyX11Server::init( const int argc, char* const* argv ) {
    settings.parseFromArgv( argc, argv );
    // if ( server.settings.copyauth )
    //     copy_authentication();
    // setvbuf(out, NULL, buffered?_IOFBF:_IOLBF, BUFSIZ);
    _parseDisplayNames();
    parser.syncLogStream( settings.log_os );
}

int ProxyX11Server::run() {
    _listenForClients();
    _startSubcommandClient();
//    __debugOutput();
    return _processClientQueue();
}
