#include <iostream>  // TBD testing only

#include <string>        // stoi to_string
#include <algorithm>     // transform min
#include <regex>         // smatch regex regex_search
//#include <filesystem>    // remove

#include <cassert>
#include <cstdlib>       // getenv setenv
#include <cstring>       // memcpy

#include <sys/socket.h>  // 'struct sockaddr' SOCK_STREAM AF_INET AF_UNIX setsockopt SOL_SOCKET SO_KEEPALIVE 
#include <arpa/inet.h>   // htons htonl inet_ntoa ntohs
#include <netinet/in.h>  // 'struct sockaddr_in'
#include <sys/un.h>      // 'struct sockaddr_un'
#include <unistd.h>      // unlink close pid_t fork STDERR_FILENO STDIN_FILENO
// #ifndef _GNU_SOURCE
//   #define _GNU_SOURCE      // asprintf
// #endif
//#include <stdio.h>       // asprintf

//#include <sys/types.h>
#include <netdb.h> // 'struct addrinfo' getaddrinfo freeaddrinfo

#include "ProxyX11Server.hpp"
//#include "typeName.hh"
#include "Connection.hpp"


ProxyX11Server::_DisplayInfo::_DisplayInfo(const char* displayname) {
    assert( displayname != nullptr );

    // extract protocol token if present, skip over hostname token if present
    std::string dname { displayname };
    std::smatch dname_match;
    // Xlib reads display names as:
    //     [protocol/] [hostname] : displaynumber [.screennumber]
    std::regex dname_regex {
        "^\\s*"
        "(?:([a-zA-Z]+)/)?\\s*"  // protocol/
        "([a-zA-Z]+)?\\s*"       // hostname
        ":\\s*([0-9]+)\\s*"      // :displaynum
        "(?:\\.([0-9]+))?\\s*"   // .screennum
        "$"
    };
    const bool valid_dname_format {
        std::regex_search(dname, dname_match, dname_regex)
    };
    assert( valid_dname_format );
    name = dname;
    protocol = dname_match[1];
    std::transform( protocol.begin(), protocol.end(), protocol.begin(),
                    [](unsigned char c){ return std::tolower(c); } );
    hostname = dname_match[2];
    std::transform( hostname.begin(), hostname.end(), hostname.begin(),
                    [](unsigned char c){ return std::tolower(c); } );
    display = std::stoi( dname_match[3] );
    if ( dname_match[4] != "" )
        screen = std::stoi( dname_match[4] );
    // default protocol for hostnames other than unix is "tcp"
    // default protocol for hostname unix or missing hostname is "local"
    // TBD TODO from xtrace: make sure we are in C locale, otherwise this can go wrong
    // std::cout.getloc().name() == "C"
    assert( std::cout.getloc() == std::locale::classic() );
    if ( !hostname.empty() && hostname != "unix") {
        if ( protocol.empty() )
            protocol = std::string( "tcp" );
    } else {
        hostname.clear();   // TBD matching xtrace, necessary?
        protocol = std::string( "local" );
    }
    // only allow inet, tcp, unix, or local protocols, set family accordingly
    if( protocol == "inet" || protocol == "tcp" ) {
        family = AF_INET;
    } else if ( protocol == "unix" || protocol == "local" ) {
        family = AF_UNIX;
    } else {
        assert( 0 );
    }
    if ( family == AF_UNIX ) {
        unix_socket_path = std::string(
            _UNIX_SOCKET_PATH_PREFIX.data() + dname_match[3].str() );
    }
    assert( display != _UNSET );
    assert( family != _UNSET );
}

void ProxyX11Server::__debugOutput() {
    std::cout << std::boolalpha <<
        "settings:\n" <<
        "\treadwritedebug: " << settings.readwritedebug << '\n' <<
        "\tcopyauth: " << settings.copyauth << '\n' <<
        "\tstopwhennone: " << settings.stopwhennone << '\n' <<
        "\twaitforclient: " << settings.waitforclient << '\n' <<
        "\tdenyallextensions: " << settings.denyallextensions << '\n' <<
        "\tinteractive: " << settings.interactive << '\n' <<
        "\tprint_timestamps: " << settings.print_timestamps << '\n' <<
        "\tprint_reltimestamps: " << settings.print_reltimestamps << '\n' <<
        "\tprint_uptimestamps: " << settings.print_uptimestamps << '\n' <<
        "\tbuffered: " << settings.buffered << '\n' <<
        "\tmaxshownlistlen: " << settings.maxshownlistlen << '\n' <<
        "\tprint_counts: " << settings.print_counts << '\n' <<
        "\tprint_offsets: " << settings.print_offsets << '\n' <<
        // FILE *out { nullptr };
        "\tout_displayname: \"" << (settings.out_displayname == nullptr ? "(null)" : settings.out_displayname) << "\"\n" <<
        "\tin_displayname: \"" << (settings.in_displayname == nullptr ? "(null)" : settings.in_displayname) << "\"\n" <<
        "\tout_authfile: \"" << (settings.out_authfile == nullptr ? "(null)" : settings.out_authfile) << "\"\n" <<
        "\tin_authfile: \"" << (settings.in_authfile == nullptr ? "(null)" : settings.in_authfile) << "\"\n" <<
        "\tcli_subcmd_argc: " << settings.cli_subcmd_argc << '\n' <<
        "\tcli_subcmd_argv: ";
    for (int i {}; i < settings.cli_subcmd_argc; ++i)
        std::cout << "\"" << settings.cli_subcmd_argv[i] << "\" ";
    std::cout << std::endl;

    std::cout <<
        "_in_display:\n" <<
        "\tname: " << _in_display.name << '\n' <<
        "\tprotocol: " << _in_display.protocol << '\n' <<
        "\thostname: " << _in_display.hostname << '\n' <<
        "\tdisplay: " << _in_display.display << '\n' <<
        "\tscreen: " << _in_display.screen << '\n' <<
        "\tfamily: " << _in_display.family << '\n' <<
        "\tunix_socket_path: " << _in_display.unix_socket_path <<
        std::endl;

    std::cout <<
        "_out_display:\n" <<
        "\tname: " << _out_display.name << '\n' <<
        "\tprotocol: " << _out_display.protocol << '\n' <<
        "\thostname: " << _out_display.hostname << '\n' <<
        "\tdisplay: " << _out_display.display << '\n' <<
        "\tscreen: " << _out_display.screen << '\n' <<
        "\tfamily: " << _out_display.family << '\n' <<
        "\tunix_socket_path: " << _out_display.unix_socket_path <<
        std::endl;
}

ProxyX11Server::ProxyX11Server() {
}

ProxyX11Server::~ProxyX11Server() {
    close( _listener_fd );
    if ( !_in_display.unix_socket_path.empty() )
        unlink( _in_display.unix_socket_path.data() );
}

void ProxyX11Server::parseDisplayNames() {
    const char* out_displayname { nullptr };
    if ( settings.out_displayname != nullptr ) {
        out_displayname = settings.out_displayname;
    } else {
        out_displayname = getenv( _OUT_DISPLAYNAME_ENV_VAR.data() );
    }
    assert( out_displayname != nullptr );
    _out_display = _DisplayInfo( out_displayname );

    const char* in_displayname { nullptr };
    if( settings.in_displayname != nullptr ) {
        in_displayname = settings.in_displayname;
    } else {
        in_displayname = getenv( _IN_DISPLAYNAME_ENV_VAR.data() );
        if ( in_displayname == nullptr ) {
            std::cerr << "No display name to create specified, trying " <<
                _DEFAULT_IN_DISPLAYNAME << "\n";
            in_displayname = _DEFAULT_IN_DISPLAYNAME.data();
        }
    }
    assert( in_displayname != nullptr );
    _in_display = _DisplayInfo( in_displayname );
}

// TBD inaddr.sin_port never changes, can be part of DisplayInfo
//    (really the sockaddr_in and sockaddr_un both do not change, and can be set once only (static const set by lambda?))
//    (less crucial here as this will only be called once per process execution)
// listenForClients(in_displayname,in_family,in_display)
void ProxyX11Server::listenForClients() {
    /*struct*/ sockaddr* address;
    std::size_t address_sz;

    // open sequenced, reliable, two-way, connection-based byte stream
    static constexpr int SOCKET_DEFAULT_PROTOCOL { 0 };
    const int fd { socket( _in_display.family, SOCK_STREAM, SOCKET_DEFAULT_PROTOCOL ) };
    if ( fd < 0 )  {
        // TBD exception
        //fprintf(stderr,"Error opening socket for '%s': %d=%s\n",_in_display.name,errno,strerror(errno));
        std::cerr << "ProxyX11Server::listenForClients: socket\n";
        exit( EXIT_FAILURE );
    }
    assert( _in_display.family == AF_INET || _in_display.family == AF_UNIX );
    if ( _in_display.family == AF_INET ) {
        // At sockets API level, enable sending of keep-alive messages on
        //   connection-oriented sockets.
        const int on { 1 };
        if ( setsockopt( fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on) ) != 0 ) {
            // TBD exception using errno
            std::cerr << "ProxyX11Server::listenForClients: setsockopt\n";
            exit( EXIT_FAILURE );
        }
        /*struct*/ sockaddr_in inaddr;
        // use domain of IPv4 Internet protocols
        inaddr.sin_family      = _in_display.family;
        // use port 6000 + display number (network byte order)
        inaddr.sin_port        = htons( _X_TCP_PORT + _in_display.display ); // calculateTCPport();
        // bind socket to all local interfaces (network byte order)
        inaddr.sin_addr.s_addr = htonl( INADDR_ANY );
        address    = reinterpret_cast< struct sockaddr* >( &inaddr );
        address_sz = sizeof( inaddr );
    } else {  // _in_display.family == AF_UNIX
        /*struct*/ sockaddr_un unaddr;
        // Unix domain (local communication)
        unaddr.sun_family = _in_display.family;
        // TBD generateSocketName
        // TBD sockaddr_un.sun_path is documented as a flexible array (char[],) but seems to be implemented as
        //   a fixed size array, char[108] in this case
        std::memcpy( unaddr.sun_path, _in_display.unix_socket_path.data(),
                     _in_display.unix_socket_path.size() + 1 );
        // TBD necessary if deleted as part of shutdown?
        unlink( unaddr.sun_path );
        address    = reinterpret_cast< struct sockaddr* >( &unaddr );
        address_sz = sizeof( unaddr );
    }

    if ( bind( fd, address, address_sz ) < 0 ) {
        close(fd);
        // TBD exception
        // fprintf(stderr,"Error binding socket for '%s': %d=%s\n",_in_display.name,errno,strerror(errno));
        std::cerr << "ProxyX11Server::listenForClients: bind\n";
        exit( EXIT_FAILURE );
    }

    static constexpr int MAX_PENDING_CONNECTIONS { 20 };
    if ( listen( fd, MAX_PENDING_CONNECTIONS ) < 0 ) {
        close(fd);
        // TBD exception
        // fprintf(stderr,"Error listening for '%s': %d=%s\n",_in_display.name,errno ,strerror(errno));
        std::cerr << "ProxyX11Server::listenForClients: listen\n";
        exit( EXIT_FAILURE );
    }

    _listener_fd = fd;
}

void ProxyX11Server::startClient() {
    if ( settings.cli_subcmd_argc == 0 )
        return;
    _child_pid = fork();
    if( _child_pid == -1 ) {  // fork failed, still in parent
        // TBD exception
        //fprintf(stderr, "Error forking: %s\n", strerror(errno));
        std::cerr << "ProxyX11Server::startClient fork\n";
        exit( EXIT_FAILURE );
    }
    if( _child_pid == 0 ) {   // fork succeeded, in child
        if ( setenv( "DISPLAY", _in_display.name.data(), 1 ) != 0 ) {
            // TBD exception
            //fprintf(stderr,"Error setting $DISPLAY: %s\n", strerror(errno));
            std::cerr << "ProxyX11Server::startClient setenv\n";
            exit( EXIT_FAILURE );
        }
        execvp( settings.cli_subcmd_argv[0], settings.cli_subcmd_argv );
        // child has failed to overtake parent process
        // TBD exception
        //fprintf(stderr, "Could not exec '%s': %s\n", argv[0], strerror(errno));
        std::cerr << "ProxyX11Server::startClient execvp\n";
        exit( EXIT_FAILURE );
    }
    assert( _child_pid != 0 );
}

// acceptClient(in_family,listener, &c->from)
// TBD this is called as part of acceptConnection, and effectively has two outputs:
//   - fd of client at top of pending connection stack for listener (c->client_fd)
//   - allocated string describing client (c->from) (x.x.x.x:port for AF_INET, socket file path or "unknown(local)" for AF_UNIX)
bool ProxyX11Server::acceptClient(Connection* conn) {
    int fd;
    socklen_t len;
    std::string from;

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
            // TBD exception using errno
            std::cerr << "ProxyX11Server::acceptClient accept\n";
            return false;
        }
        // TBD inet_ntoa deprecated for inet_ntop:
        /*
          #include <arpa/inet.h>

          const char *inet_ntop(int af, const void *restrict src,
                                char dst[restrict .size], socklen_t size);
        */
        from = std::string( inet_ntoa( inaddr.sin_addr ) ) + ':' +
            std::to_string( ntohs( inaddr.sin_port ) );
    } else {  // _in_display.family == AF_UNIX
        /*struct */sockaddr_un unaddr;
        len = sizeof( unaddr );
        fd = accept( _listener_fd,
                     reinterpret_cast< struct sockaddr* >( &unaddr ), &len );
        if ( fd < 0 ) {
            // TBD exception using errno
            std::cerr << "ProxyX11Server::acceptClient accept\n";
            return false;
        }
        if ( len > sizeof( sa_family_t ) )  // flexible array unaddr.sun_path len > 0
            from = std::string( unaddr.sun_path, len - sizeof( sa_family_t ) );
        else
            from = std::string( "unknown(local)" );
    }
    conn->client_fd   = fd;
    conn->client_desc = from;
    return true;
}

// TBD addr.sin_port never changes, can be part of DisplayInfo or static func var
//    (really the sockaddr_in and sockaddr_un both do not change, and can be set once only (static const set by lambda?))
// connectToServer(out_displayname,out_family,out_hostname,out_display)
int ProxyX11Server::connectToServer() {
    // TCP socket to connect to X server as client
    int fd { socket( _out_display.family, SOCK_STREAM, 0 ) };
    if( fd < 0 )  {
        // TBD exeception
        // fprintf(stderr,"Error opening socket for '%s': %d=%s\n",_out_display.name,errno,strerror(errno));
        std::cerr << "ProxyX11Server::connectToServer: socket\n";
        return -1;
    }
    assert( _out_display.family == AF_INET || _out_display.family == AF_UNIX );
    if( _out_display.family == AF_INET ) {
        /*struct */sockaddr_in addr  {};
        /*struct */addrinfo    hints {};
        /*struct */addrinfo*   res   {};
        //memset(&hints,0,sizeof(struct addrinfo));
        hints.ai_family   = _out_display.family;
        hints.ai_socktype = SOCK_STREAM;
        // TBD this call to getaddrinfo only needs to happen once (?), in _out_display init
        int getaddrinfo_ret { getaddrinfo( _out_display.hostname.data(), NULL, &hints, &res ) };
        if ( getaddrinfo_ret != 0 ) {
            close( fd );
            // TBD exception
            //fprintf(stderr,"Error resolving hostname '%s' taken from '%s'\nError was: %s\n",_out_display.hostname,_out_display.name,gai_strerror(getaddrinfo_ret));
            std::cerr << "ProxyX11Server::connectToServer: getaddrinfo\n";
            return -1;
        }
        assert( res->ai_addrlen == sizeof(addr) );
        // TBD so we're using getaddrinfo to get a list of addresses that are AF_INET/SOCK_STREAM (IPv4/TCP,) and
        //   taking the first one? How are we sure its the first?
        std::cout << "ProxyX11Server::connectToServer:\n";
        for (/*struct */addrinfo* node { res }; node; node = node->ai_next) {
            std::cout << "node: 0x" << (void*)node << "\n" <<
                "\tai_flags: " << node->ai_flags << '\n' <<
                "\tai_family: " << node->ai_family << '\n' <<
                "\tai_socktype: " << node->ai_socktype << '\n' <<
                "\tai_protocol: " << node->ai_protocol << '\n' <<
                "\tai_addrlen: " << node->ai_addrlen << '\n' <<
                "\tai_addr:\n" <<
                "\t\tsin_family: " << ((sockaddr_in*)(node->ai_addr))->sin_family << '\n' <<
                "\t\tsin_port: " << ((sockaddr_in*)(node->ai_addr))->sin_port << '\n' <<
                "\t\tsin_addr.s_addr: " << inet_ntoa(((sockaddr_in*)(node->ai_addr))->sin_addr) << '\n' <<
                "\tai_canonname: " << (node->ai_canonname ? node->ai_canonname : "(null)") << std::endl;
        }
        memcpy( &addr, res->ai_addr, sizeof(addr) );
        freeaddrinfo( res );
        addr.sin_port = htons( _X_TCP_PORT + _out_display.display ); // calculateTCPport();
        const int on { 1 };
        setsockopt( fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on) );
        if( connect( fd, (struct sockaddr*)&addr, sizeof(addr) ) < 0 ) {
            close(fd);
            // TBD exception
            // fprintf(stderr,"Error connecting to '%s' (resolved to '%s') for '%s': %d=%s\n",_out_display.hostname,inet_ntoa(addr.sin_addr),_out_display.name,errno,strerror(errno));
            std::cerr << "ProxyX11Server::connectToServer: connect\n";
            return -1;
        }
    } else {
        /*struct */sockaddr_un addr;
        // TBD no need to set addr.sun_family?
        // TBD generateSocketName
        // TBD sockaddr_un.sun_path is documented as a flexible array (char[],) but seems to be implemented as
        //   a fixed size array, char[108] in this case
        memcpy( addr.sun_path, _out_display.unix_socket_path.data(),
                _out_display.unix_socket_path.size() + 1 );

        if( connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0 ) {
            close(fd);
            // TBD exception
            // fprintf(stderr,"Error connecting to unix socket '%s' for '%s': %d=%s\n",addr.sun_path,_out_display.name,errno,strerror(errno));
            std::cerr << "ProxyX11Server::connectToServer: connect\n";
            return -1;
        }
    }
    return fd;
}

// acceptConnection(int listener)
Connection ProxyX11Server::acceptConnection() {
    Connection c {};

    // TBD make connections a vector of Connection
    // TBD currently recording timestamp for all Connection in ctor
    // if( settings.print_reltimestamps ) {
    //     /*struct */timeval tv;
    //     if( gettimeofday( &tv, NULL ) != 0 ) {
    //         // TBD exception
    //         // fprintf(stderr, "gettimeofday error %d : %s!\n", errno, strerror(errno));
    //         std::cerr << "ProxyX11Server::acceptConnection() gettimeofday\n";
    //         exit(EXIT_FAILURE);
    //     }
    //     c.start_time = tv.tv_sec * uint64_t{ 1000 } + tv.tv_usec / 1000;
    // }

    // TBD original connection is a stack (new pushed to front) (better as queue?)
    //c->next = connections;
    // TBD need error handling here
    if ( !acceptClient( &c ) )
        return c;
    assert( c.client_fd > _listener_fd );
    assert( !c.client_desc.empty() );
    // TBD why is a global user setting arbitrarily set here, at every new connection?
    //   would it not be better somewhere in mainqueue before the first call to acceptConnection?
    settings.waitforclient = false;
    std::cerr << "Got connection from " << c.client_desc << std::endl;

    c.server_fd = connectToServer();
    if( c.server_fd < 0 ) {
        close(c.client_fd);
        // TBD exception
        //fprintf(stderr,"Error connecting to server %s\n",out_displayname);
        std::cerr << "Error connecting to server " << _out_display.name << std::endl;
    }
    assert( c.server_fd > _listener_fd );

    return c;
}

// // TBD mainqueue() retval is passed as retval of xtrace
// // TBD notice printing to stdout, stderr and out (may be stdout)
// int ProxyX11Server::processClientQueue() {
//     static constexpr int FD_CLOSED    { -1 };  // sentinel for fds
//     static constexpr int CHILD_EXITED { 0 };   // sentinel for child pid

//     // TBD make class member?
//     std::unordered_map<int, Connection> connections;
//     std::vector<int> closed_connection_ids;

//     // NOTE: FD_SET and FD_CLR are idempotent
//     fd_set readfds, writefds, exceptfds;
//     for ( int max_fd, select_ret; true; )
//         /*
//          * setup
//          */
//         // select(2) expects first param to be highest fd in any of readfds, writefds, exceptfds + 1
//         max_fd = _listener_fd;
//         // fd_sets are modified in place by select(2), and need to be zeroed out before each use
//         FD_ZERO( &readfds );
//         FD_ZERO( &writefds );
//         FD_ZERO( &exceptfds );

//         // monitor _listener_fd for input (read readiness)
//         FD_SET( _listener_fd, &readfds );

//         closed_connection_ids.clear();
//         /*
//          * first loop over connections to:
//          *   - set client and server sockets into readfds, writefds, exceptfds
//          *   - mark closed connections for deletion
//          */
//         for ( auto& [ id, conn ] : connections ) {
//             if ( !conn.clientSocketIsClosed() ) {
//                 // If connection is otherwise closed:
//                 if ( conn.serverSocketIsClosed() && conn.server_buffer.empty() ) {
//                     conn.closeClientSocket();
//                     //if ( settings.readwritedebug )
//                     //    fprintf(out,"%03d:>:sent EOF\n",c->id);
//                 } else {
//                     // monitor client socket for exceptional conditions (see poll(2) POLLPRI)
//                     FD_SET( conn.client_fd, &exceptfds );
//                     max_fd = std::max( max_fd, conn.client_fd );

//                     if ( conn.client_buffer.empty() ) {
//                         // monitor client socket for input (read readiness)
//                         FD_SET( conn.client_fd, &readfds );
//                     }
//                     if ( !conn.server_buffer.empty() ) {
//                         // monitor client socket for output (write readiness)
//                         FD_SET( conn.client_fd, &writefds );
//                     }
//                 }
//             }
//             if ( !conn.serverSocketIsClosed() ) {
//                 // If connection is otherwise closed:
//                 if ( !conn.clientSocketIsClosed() && conn.client_buffer.empty() ) {
//                     conn.closeServerSocket();
//                     // if ( settings.readwritedebug )
//                     //     fprintf(out,"%03d:<:sent EOF\n",conn.id);
//                 } else {
//                     // monitor client socket for exceptional conditions (see poll(2) POLLPRI)
//                     FD_SET( conn.server_fd, &exceptfds );
//                     max_fd = std::max( max_fd, conn.server_fd );

//                     if ( conn.server_buffer.empty() ) {
//                         // monitor server socket for input (read readiness)
//                         FD_SET( conn.server_fd, &readfds );
//                     }
//                     if ( !conn.client_buffer.empty() ) {
//                         // monitor server socket for output (write readiness)
//                         FD_SET( conn.server_fd, &writefds );
//                     }
//                 }
//             }
//             // If connection is completely closed:
//             if ( conn.clientSocketIsClosed() && conn.serverSocketIsClosed() ) {
//                 closed_connection_ids.emplace_back( conn.id );
//             }
//         }

//         // TBD assuming we cannot erase while iterating in above loop
//         for ( const int id : closed_connection_ids )
//             connections.erase( id );

//         // If no connections left and subprocess has exited: exit
//         // TBD note stopwhennone true by default
//         if ( connections.empty() && settings.stopwhennone &&
//              _child_pid == CHILD_EXITED ) {
//             return EXIT_SUCCESS;
//         }

//         // TBD run in separate thread? https://stackoverflow.com/a/11679770
//         /*
//          * Check if subprocess finished (child from cli `--`)
//          */
//         // If child has not exited, and select failed on previous loop when SIGCHLD caught
//         if ( _child_pid != CHILD_EXITED &&
//              ( select_ret == -1 || caught_child_signal ) ) {
//             caught_child_signal = false;
//             int status;
//             // TBD consider sigaction and SA_NOCLDWAIT:
//             //   - https://unix.stackexchange.com/a/616607
//             // Check to see if child exited, but do not hang (wait until so)
//             if ( waitpid( _child_pid, &status, WNOHANG ) == _child_pid ) {
//                 _child_pid = CHILD_EXITED;
//                 // If connections empty and not set to wait after subcommand
//                 if ( connections.empty() && !settings.waitforclient ) {
//                     /* TODO: instead wait a bit before terminating? */
//                     // Return child exit code or number of signal that terminated
//                     // TBD why +128?
//                     if ( WIFEXITED( status ) )
//                         return WEXITSTATUS( status );
//                     else  // WIFSIGNALED( status ) == true
//                         return WTERMSIG( status ) + 128;
//                 }
//             }
//         }

//         /*
//          * Idle until any of the fds in the provided fd_sets becomes ready for
//          *   reading, writing, or indicating exceptional conditions
//          */
//         select_ret = select( max_fd + 1, &readfds, &writefds, &exceptfds, NULL );
//         // TBD Note that:
//         //   - select failure resets but does not break loop
//         //   - SIGINT forces select failure, but does not print error
//         if ( select_ret == -1 ) {
//             if ( errno != 0 && errno != EINTR ) {
//                 // TBD exception
//                 //fprintf( stderr, "Error %d in select: %s\n", errno, strerror(errno) );
//             }
//             continue;
//         }

//         // TBD need to incorporate read/write of 0 (EOF) into logic
//         /*
//          * second loop over connections to:
//          *   - receive new packets
//          *   - parse current packets
//          *   - send current packets
//          */
//         for ( auto& [ id, conn ] : connections ) {
//             if ( !conn.clientSocketIsClosed() ) {
//                 // If client socket had exceptional condition:
//                 if ( FD_ISSET( conn.client_fd, &exceptfds ) ) {
//                     conn.closeClientSocket();
//                     //fprintf(stdout,"%03d: exception in communication with client\n",conn.id);
//                     continue;
//                 }
//                 // If client socket has pending write:
//                 if ( FD_ISSET( conn.client_fd, &writefds ) ) {
//                     /*
//                      * TBD if client is ready for a write, but there is no data to write (server buffer is empty,)
//                      *   should we use 0 bytes written as an indicator to close client_fd?
//                      */

//                     if ( conn.server_buffer.empty() ) {
//                         // TBD edge case (?): the buffer could at times be empty
//                         //   even if the server socket is not closed
//                         if ( conn.serverSocketIsClosed() ) {
//                             // Close client socket
//                             conn.closeClientSocket();
//                             // if ( settings.readwritedebug )
//                             //     fprintf(stdout,"%03d:>:send EOF\n",conn.id);
//                             continue;
//                         }
//                     } else {
//                         // TBD parse first before sending out, as parsing may involve editing buffer
//                         //parse_server( c );
//                         // TBD Write server buffer to client socket
//                         try {
//                             ssize_t bytes_written { conn.server_buffer.write( conn.client_fd ) };
//                         } catch ( const std::system_error& e ) {
//                             // close client socket, continue to next connection
//                             conn.closeClientSocket();
//                             // if ( settings.readwritedebug ) {
//                             //     fprintf(stdout,"%03d: error writing to client: %d=%s\n",
//                             //             conn.id, errno, strerror(errno));
//                             // }
//                             continue;
//                         }
//                         assert( bytes_written != -1 );  // SocketBufffer.write() has -1 protection
//                         // If any bytes were written to client socket:
//                         if ( bytes_written > 0 ) {
//                             // if ( settings.readwritedebug )
//                             //     fprintf(stdout,"%03d:>:wrote %u bytes\n",conn.id,(unsigned int)bytes_written);
//                         }
//                     }
//                 }
//                 // If client socket has pending read:
//                 if ( FD_ISSET( conn.client_fd, &readfds ) ) {
//                     // Read from client into clientbuffer
//                     try {
//                         ssize_t bytes_read { conn.client_buffer.read( conn.client_fd ) };
//                     } catch ( const std::system_error& e ) {
//                         // Close client socket, continue to next connection
//                         // if ( settings.readwritedebug )
//                         //     fprintf(stdout,"%03d:<:error reading from client buffer\n",conn.id);
//                         conn.closeClientSocket();
//                         continue;
//                     }
//                     // If any bytes read:
//                     if ( bytes_read > 0 ) {
//                         // // Update client buffer size
//                         // if ( settings.readwritedebug )
//                         //     fprintf(stdout,"%03d:<:received %u bytes\n",conn.id,(unsigned int)wasread);
//                     } else {
//                         // Close client socket, continue to next connection
//                         // if ( settings.readwritedebug )
//                         //     fprintf(stdout,"%03d:<:got EOF\n",conn.id);
//                         conn.closeClientSocket();
//                         continue;
//                     }
//                     // TBD parse immediately after reading, as we may need to alter contents
//                     if ( !conn.client_buffer.empty() ) {
//                         // parse_client( c );
//                     }
//                 }
//                 // If client socket is closed, and server bytes have been read:
//             } else if ( !conn.server_buffer.empty() ) {
//                 /* discard additional events */
//                 fprintf(stdout, "%03d:s->?: discarded last answer of %u bytes\n", conn.id, conn.server_buffer.size() );
//                 // TBD xtrace has case here where there may be bytes left after serverignore
//                 // if ( conn.serverignore == 0 && conn.servercount > 0 ) {
//                 //     parse_server( c );
//                 // }
//             }
//             // If server socket is open:
//             if ( conn.server_fd != FD_CLOSED ) {
//                 // If server socket had exceptional condition:
//                 if ( FD_ISSET( conn.client_fd, &exceptfds ) ) {
//                     // Close socket with message, continue to next connection
//                     close( conn.server_fd );
//                     conn.server_fd = FD_CLOSED;
//                     // fprintf( stdout, "%03d: exception in communication with server\n", conn.id );
//                     continue;
//                 }
//                 // If server socket has pending write:
//                 if ( FD_ISSET( conn.server_fd, &writefds ) ) {
//                     try {
//                         ssize_t bytes_written { conn.client_buffer.write( conn.server_fd ) };
//                     } catch ( const std::system_error& e ) {
//                         close( conn.server_fd );
//                         conn.server_fd = FD_CLOSED;
//                         // if ( settings.readwritedebug )
//                         //     fprintf(stdout,"%03d: error writing to server: %d=%s\n",conn.id,e,strerror(e));
//                         continue;
//                     }
//                     if ( settings.interactive && allowsent > 0 )   // allowsent
//                         allowsent--;                               // allowsent
//                     // If bytes written to server socket:
//                     if ( bytes_written > 0 ) {
//                         // if ( settings.readwritedebug )
//                         //     fprintf(stdout,"%03d:<:wrote %u bytes\n",conn.id,(unsigned int)written);
//                         // If client buffer was not completely emptied:
//                         // If bytes remain in client buffer: log print next packet
//                         // if ( conn.clientcount != 0 && conn.clientignore == 0 ) {
//                         //     parse_client( c );
//                         // }
//                     }
//                 }
//                 // If server socket has pending read:
//                 if ( FD_ISSET( conn.server_fd, &readfds ) ) {
//                     try {
//                         ssize_t bytes_read { conn.server_buffer.read( conn.server_fd ) };
//                     } catch ( const std::system_error& e ) {
//                         // if ( settings.readwritedebug )
//                         //     fprintf(stdout,"%03d:>:error reading from server bufer\n",conn.id);
//                         close( conn.server_fd );
//                         conn.server_fd = FD_CLOSED;
//                     }
//                     if ( bytes_read > 0 ) {
//                         // if ( settings.readwritedebug )
//                         //     fprintf(stdout,"%03d:>:received %u bytes\n",conn.id,(unsigned int)wasread);
//                     } else {
//                         // if ( settings.readwritedebug )
//                         //     fprintf(stdout,"%03d:>:got EOF\n",conn.id);
//                         conn.closeServerSocket();
//                     }
//                     if ( !conn.server_buffer.empty() ) {
//                         //parse_server( c );
//                     }
//                 }
//             } else if ( !conn.client_buffer.empty() ) {
//                 /* discard additional events */
//                 fprintf(stdout,"%03d:<: discarding last request of %u bytes\n",conn.id, conn.client_buffer.size() );
//                 // if ( conn.clientignore == 0 && conn.clientcount > 0 ) {
//                 //     parse_client( c );
//                 // }
//             }
//         }

//         // new connections are driven by listener socket accept(2)ing clients
//         if ( FD_ISSET( _listener_fd, &readfds ) ) {
//             acceptConnection();
//         }

//     }
//     return EXIT_SUCCESS;
// }
