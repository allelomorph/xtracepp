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

#include <sys/time.h>    // 'struct timeval' gettimeofday

//#include <sys/types.h>
#include <netdb.h> // 'struct addrinfo' getaddrinfo freeaddrinfo

#include "IntermediaryServer.hpp"
//#include "typeName.hh"
#include "Connection.hpp"


IntermediaryServer::_DisplayInfo::_DisplayInfo(const char* displayname) {
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

void IntermediaryServer::__debugOutput() {
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

IntermediaryServer::IntermediaryServer() {
}

IntermediaryServer::~IntermediaryServer() {
    close( _listener_fd );
    if ( !_in_display.unix_socket_path.empty() )
        unlink( _in_display.unix_socket_path.data() );
}

void IntermediaryServer::parseDisplayNames() {
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
void IntermediaryServer::listenForClients() {
    /*struct*/ sockaddr* address;
    std::size_t address_sz;

    // open sequenced, reliable, two-way, connection-based byte stream
    static constexpr int SOCKET_DEFAULT_PROTOCOL { 0 };
    const int fd { socket( _in_display.family, SOCK_STREAM, SOCKET_DEFAULT_PROTOCOL ) };
    if ( fd < 0 )  {
        // TBD exception
        //fprintf(stderr,"Error opening socket for '%s': %d=%s\n",_in_display.name,errno,strerror(errno));
        std::cerr << "IntermediaryServer::listenForClients: socket\n";
        exit( EXIT_FAILURE );
    }
    assert( _in_display.family == AF_INET || _in_display.family == AF_UNIX );
    if ( _in_display.family == AF_INET ) {
        // At sockets API level, enable sending of keep-alive messages on
        //   connection-oriented sockets.
        const int on { 1 };
        if ( setsockopt( fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on) ) != 0 ) {
            // TBD exception using errno
            std::cerr << "IntermediaryServer::listenForClients: setsockopt\n";
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
        std::cerr << "IntermediaryServer::listenForClients: bind\n";
        exit( EXIT_FAILURE );
    }

    static constexpr int MAX_PENDING_CONNECTIONS { 20 };
    if ( listen( fd, MAX_PENDING_CONNECTIONS ) < 0 ) {
        close(fd);
        // TBD exception
        // fprintf(stderr,"Error listening for '%s': %d=%s\n",_in_display.name,errno ,strerror(errno));
        std::cerr << "IntermediaryServer::listenForClients: listen\n";
        exit( EXIT_FAILURE );
    }

    _listener_fd = fd;
}

void IntermediaryServer::startClient() {
    if ( settings.cli_subcmd_argc == 0 )
        return;
    _child_pid = fork();
    if( _child_pid == -1 ) {  // fork failed, still in parent
        // TBD exception
        //fprintf(stderr, "Error forking: %s\n", strerror(errno));
        std::cerr << "IntermediaryServer::startClient fork\n";
        exit( EXIT_FAILURE );
    }
    if( _child_pid == 0 ) {   // fork succeeded, in child
        if ( setenv( "DISPLAY", _in_display.name.data(), 1 ) != 0 ) {
            // TBD exception
            //fprintf(stderr,"Error setting $DISPLAY: %s\n", strerror(errno));
            std::cerr << "IntermediaryServer::startClient setenv\n";
            exit( EXIT_FAILURE );
        }
        execvp( settings.cli_subcmd_argv[0], settings.cli_subcmd_argv );
        // child has failed to overtake parent process
        // TBD exception
        //fprintf(stderr, "Could not exec '%s': %s\n", argv[0], strerror(errno));
        std::cerr << "IntermediaryServer::startClient execvp\n";
        exit( EXIT_FAILURE );
    }
    assert( _child_pid != 0 );
}

// acceptClient(in_family,listener, &c->from)
// TBD this is called as part of acceptConnection, and effectively has two outputs:
//   - fd of client at top of pending connection stack for listener (c->client_fd)
//   - allocated string describing client (c->from) (x.x.x.x:port for AF_INET, socket file path or "unknown(local)" for AF_UNIX)
bool IntermediaryServer::acceptClient(Connection* conn) {
    int fd;
    socklen_t len;
    std::string from;

    assert( conn != nullptr );
    assert( _in_display.family == AF_INET || _in_display.family == AF_UNIX );
    if ( _in_display.family == AF_INET ) {
        /*struct */sockaddr_in inaddr;
        len = sizeof( inaddr );
        fd = accept( _listener_fd,
                     reinterpret_cast< struct sockaddr* >( &inaddr ), &len );
        if ( fd < 0 ) {
            // TBD exception using errno
            std::cerr << "IntermediaryServer::acceptClient accept\n";
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
            std::cerr << "IntermediaryServer::acceptClient accept\n";
            return false;
        }
        if ( len > sizeof( sa_family_t ) )  // flexible array unaddr.sun_path len > 0
            from = std::string( unaddr.sun_path, len - sizeof( sa_family_t ) );
        else
            from = std::string( "unknown(local)" );
    }
    conn->client_fd = fd;
    conn->from      = from;
    return true;
}

// TBD addr.sin_port never changes, can be part of DisplayInfo or static func var
//    (really the sockaddr_in and sockaddr_un both do not change, and can be set once only (static const set by lambda?))
// connectToServer(out_displayname,out_family,out_hostname,out_display)
int IntermediaryServer::connectToServer() {
    // TCP socket to connect to X server as client
    int fd { socket( _out_display.family, SOCK_STREAM, 0 ) };
    if( fd < 0 )  {
        // TBD exeception
        // fprintf(stderr,"Error opening socket for '%s': %d=%s\n",_out_display.name,errno,strerror(errno));
        std::cerr << "IntermediaryServer::connectToServer: socket\n";
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
        int getaddrinfo_ret { getaddrinfo( _out_display.hostname.data(), NULL, &hints, &res ) };
        if ( getaddrinfo_ret != 0 ) {
            close( fd );
            // TBD exception
            //fprintf(stderr,"Error resolving hostname '%s' taken from '%s'\nError was: %s\n",_out_display.hostname,_out_display.name,gai_strerror(getaddrinfo_ret));
            std::cerr << "IntermediaryServer::connectToServer: getaddrinfo\n";
            return -1;
        }
        assert( res->ai_addrlen == sizeof(addr) );
        // TBD so we're using getaddrinfo to get a list of addresses that are AF_INET/SOCK_STREAM (IPv4/TCP,) and
        //   taking the first one? How are we sure its the first?
        std::cout << "IntermediaryServer::connectToServer:\n";
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
            std::cerr << "IntermediaryServer::connectToServer: connect\n";
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
            std::cerr << "IntermediaryServer::connectToServer: connect\n";
            return -1;
        }
    }
    close(fd);  // TBD temp for testing
//    return fd;
    return 0;
}

// acceptConnection(int listener)
void IntermediaryServer::acceptConnection() {
    static int id = 0;
    Connection c;
    c.id = id++;

    // TBD make connections a vector of Connection
    if( settings.print_reltimestamps ) {
        /*struct */timeval tv;
        if( gettimeofday( &tv, NULL ) != 0 ) {
            // TBD exception
            // fprintf(stderr, "gettimeofday error %d : %s!\n", errno, strerror(errno));
            std::cerr << "IntermediaryServer::acceptConnection() gettimeofday\n";
            exit(EXIT_FAILURE);
        }
        c.start_time = tv.tv_sec * uint64_t{ 1000 } + tv.tv_usec / 1000;
    }
    // TBD original connection is a stack (new pushed to front) (better as queue?)
    //c->next = connections;
    if ( !acceptClient( &c ) )
        return;
    assert( c.client_fd > STDERR_FILENO );
    assert( !c.from.empty() );
    // TBD why is a global user setting arbitrarily set here, at every new connection?
    //   would it not be better somewhere in mainqueue before the first call to acceptConnection?
    settings.waitforclient = false;
    std::cerr << "Got connection from " << c.from << std::endl;

    c.server_fd = connectToServer();
    if( c.server_fd < 0 ) {
        close(c.client_fd);
        // TBD exception
        //fprintf(stderr,"Error connecting to server %s\n",out_displayname);
        std::cerr << "Error connecting to server " << _out_display.name << std::endl;
    }
}

static ssize_t doread(const int fd, void* buf, const size_t n,
                      /*struct */fdqueue* fdq) {
#if HAVE_SENDMSG
    /*
      // man iovec
       #include <sys/uio.h>
       struct iovec {
           void   *iov_base;  // Starting address
           size_t  iov_len;   // Size of the memory pointed to by iov_base.
       };
       // iovec seems to be used with "scatter-gather arrays," see:
       //   - man readv
       //   - https://stackoverflow.com/a/10520793
    */
    /*struct */iovec iov {};
    iov.iov_base = buf;
    iov.iov_len  = n;
    /*
      // man CMSG_SPACE
       #include <sys/socket.h>

x       struct cmsghdr *CMSG_FIRSTHDR(struct msghdr *msgh);
x       struct cmsghdr *CMSG_NXTHDR(struct msghdr *msgh,
                                   struct cmsghdr *cmsg);
       size_t CMSG_ALIGN(size_t length);
x       size_t CMSG_SPACE(size_t length);
x       size_t CMSG_LEN(size_t length);
x       unsigned char *CMSG_DATA(struct cmsghdr *cmsg);
     */

    // TBD see man CMSG_SPACE example:
    //    "Ancillary data buffer, wrapped in a union in order to ensure it is suitably aligned"
    union {
        /*struct */cmsghdr cmsghdr;
        char buf[CMSG_SPACE(FDQUEUE_MAX_FD * sizeof(int))];
    } cmsgbuf;
    /*
      // man recvmsg
      struct msghdr {
          void         *msg_name;       // Optional address
          socklen_t     msg_namelen;    // Size of address
          struct iovec *msg_iov;        // Scatter/gather array
          size_t        msg_iovlen;     // # elements in msg_iov
          void         *msg_control;    // Ancillary data, see below
          size_t        msg_controllen; // Ancillary data buffer len
          int           msg_flags;      // Flags on received message
      };

      struct cmsghdr {
          size_t cmsg_len;    // Data byte count, including header (type is socklen_t in POSIX)
          int    cmsg_level;  // Originating protocol
          int    cmsg_type;   // Protocol-specific type
      // followed by
      //    unsigned char cmsg_data[];
      };
     */
    /*struct */msghdr msg {};
    //msg.msg_name = NULL;
    //msg.msg_namelen = 0;
    // TBD why use scatter-gather arrays (struct iovec*) when we have a vector of 1 (same as single buffer?)
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = cmsgbuf.buf;
    // TBD why set this to contain the _difference_ in max fdqueue size and current fdqueue count?
    //   would it not make more sense to use CMSG_SPACE(sizeof(int) * fdq->nfd), or even sizeof(int) * fdq->nfd?
    //   example in man CMSG_SPACE sets msg_controllen = sizeof(cmsgbuf.buf)
    msg.msg_controllen = CMSG_SPACE(sizeof(int) * (FDQUEUE_MAX_FD - fdq->nfd));
    int ret { recvmsg(fd, &msg, 0) };

    /* Check for truncation errors. Only MSG_CTRUNC is
     * probably possible here, which would indicate that
     * the sender tried to transmit more than FDQUEUE_MAX_FD
     * file descriptors.
     */
    /*
      MSG_TRUNC
          indicates that the trailing portion of a datagram was discarded because the datagram was larger than the buffer
      supplied.

      MSG_CTRUNC
          indicates that some control data was discarded due to lack of space in the buffer for ancillary data.
     */
    if (msg.msg_flags & (MSG_TRUNC|MSG_CTRUNC))
        return 0;

    // If flexible length array cmsghdr.cmsg_data is more than 0 bytes:
    if ( msg.msg_controllen >= sizeof (/*struct */cmsghdr) ) {
        for ( /*struct */cmsghdr* hdr { CMSG_FIRSTHDR( &msg ) }; hdr; hdr = CMSG_NXTHDR( &msg, hdr ) ) {
            if ( hdr->cmsg_level == SOL_SOCKET &&
                 hdr->cmsg_type == SCM_RIGHTS ) {
                int nfd { ( hdr->cmsg_len - CMSG_LEN(0) ) / sizeof (int) };
                memcpy(fdq->fd + fdq->nfd, CMSG_DATA(hdr), nfd * sizeof (int));
                fdq->nfd += nfd;
            }
        }
    }
    return ret;
#else
    return read(fd, buf, n);
#endif
}

static ssize_t dowrite(const int fd, const void *buf, const size_t n,
                       /*struct */fdqueue* fdq) {
#if HAVE_SENDMSG
    if ( fdq->nfd ) {
        union {
            /*struct */cmsghdr cmsghdr;
            char buf[CMSG_SPACE(FDQUEUE_MAX_FD * sizeof(int))];
        } cmsgbuf;

        /*struct */iovec iov {};
        iov.iov_base = (void*)buf;
        iov.iov_len = n;

        /*struct */msghdr msg {};
        msg.msg_name = NULL;
        msg.msg_namelen = 0;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_control = cmsgbuf.buf;
        msg.msg_controllen = CMSG_LEN( fdq->nfd * sizeof(int) );

        /*struct */cmsghdr* hdr { CMSG_FIRSTHDR(&msg) };

/*
  // man 7 unix
     Ancillary messages
       Ancillary  data  is  sent  and received using sendmsg(2) and recvmsg(2).  For historical reasons, the ancillary message
       types listed below are specified with a SOL_SOCKET type even though they are AF_UNIX specific.  To send them,  set  the
       cmsg_level  field  of  the struct cmsghdr to SOL_SOCKET and the cmsg_type field to the type.  For more information, see
       cmsg(3).

       SCM_RIGHTS
              Send or receive a set of open file descriptors from another process.  The data portion contains an integer array
              of the file descriptors.

              Commonly, this operation is referred to as "passing a file descriptor" to another process.  However, more  accu‐
              rately,  what  is  being  passed  is a reference to an open file description (see open(2)), and in the receiving
              process it is likely that a different file descriptor number will be  used.   Semantically,  this  operation  is
              equivalent to duplicating (dup(2)) a file descriptor into the file descriptor table of another process.

              If  the  buffer used to receive the ancillary data containing file descriptors is too small (or is absent), then
              the ancillary data is truncated (or discarded) and the excess file descriptors are automatically closed  in  the
              receiving process.

              If  the  number  of  file  descriptors  received  in  the  ancillary  data would cause the process to exceed its
              RLIMIT_NOFILE resource limit (see getrlimit(2)), the excess file descriptors are automatically closed in the re‐
              ceiving process.

              The kernel constant SCM_MAX_FD defines a limit on the number of file descriptors in the  array.   Attempting  to
              send  an array larger than this limit causes sendmsg(2) to fail with the error EINVAL.  SCM_MAX_FD has the value
              253 (or 255 before Linux 2.6.38).
 */

        hdr->cmsg_len = msg.msg_controllen;
        hdr->cmsg_level = SOL_SOCKET;
        hdr->cmsg_type = SCM_RIGHTS;
        memcpy( CMSG_DATA(hdr), fdq->fd, fdq->nfd * sizeof(int) );

        int ret { sendmsg(fd, &msg, 0) };
        if (ret < 0)
            return ret;
        // TBD note that a successful send closes all fds - but will they live on after being sent in ancillary data?
        for (int i {}; i < fdq->nfd; i++)
            close( fdq->fd[i] );
        fdq->nfd = 0;
        return ret;
    } else {
#endif
        return write(fd, buf, n);
#if HAVE_SENDMSG
    }
#endif
}

// TBD mainqueue() retval is passed as retval of xtrace
// TBD notice printing to stdout, stderr and out (may be stdout)
int IntermediaryServer::processClientQueue() {
    int nfds;          // n
    int select_ret {};  // r
    fd_set readfds, writefds, exceptfds;
    // When in interactive mode, program will hang waiting for stdin input once
    //   per connection in the second connection pass. Simply pressing enter
    //   increases allowsent by 1, entering a positive integer adds that much
    //   to allowsent. allowsent represents the total number of times packets
    //   can be sent to the X server.
    unsigned int allowsent { 1 };

    static constexpr int FD_CLOSED    { -1 };  // sentinel for fds
    static constexpr int CHILD_EXITED { 0 };   // sentinel for child pid

    // NOTE: FD_SET and FD_CLR are idempotent

    while ( 1 ) {
        /*
         * setup
         */
        // find number of highest fd in use, +1
        //    (select(2) expects first param to be highest fd in any of readfds, writefds, exceptfds + 1)
        nfds = _listener_fd + 1;  // n
        // fd_sets are modified in place by select(2), and need to be zeroed out before each use
        FD_ZERO( &readfds );
        FD_ZERO( &writefds );
        FD_ZERO( &exceptfds );
        // monitor _listener_fd for input (read readiness)
        FD_SET( _listener_fd, &readfds );

        /*
         * first loop over connections to:
         *   - set client and server sockets into readfds, writefds, exceptfds
         *   - pop closed connections off the stack
         */
        for ( c = connections; c != NULL; c = c->next ) {
            // If client socket is still open but connection is otherwise closed:
            if ( c->client_fd != FD_CLOSED && c->server_fd == FD_CLOSED &&
                c->servercount == 0 && c->serverfdq.nfd == 0 ) {
                // close the client socket and log as "sent EOF"
                close( c->client_fd );
                c->client_fd = FD_CLOSED;
                if ( settings.readwritedebug )
                    fprintf(out,"%03d:>:sent EOF\n",c->id);
            }
            // If client socket is still open with some sign of an open connection
            //   (open server socket, some server bytes read, some server fds queued):
            if ( c->client_fd != FD_CLOSED ) {
                // TBD If client has not maxed out buffer space or queued fds:
                if ( sizeof( c->clientbuffer ) > c->clientcount &&
                    FDQUEUE_MAX_FD > c->clientfdq.nfd ) {
                    // monitor client socket for input (read readiness)
                    FD_SET( c->client_fd, &readfds );
                }
                // monitor client socket for exceptional conditions (see poll(2) POLLPRI)
                FD_SET( c->client_fd, &exceptfds );
                // update nfds to include client socket
                if ( c->client_fd >= nfds )                               // n
                    nfds = c->client_fd + 1;                              // n
                // TBD what is role of serverignore?
                // TBD If server has queued fds, or has already had some bytes read:
                if ( ( c->serverignore > 0 && c->servercount > 0 ) ||
                     c->serverfdq.nfd > 0 ) {
                    // monitor client socket for output (write readiness)
                    FD_SET( c->client_fd, &writefds );
                }
                // TBD this clause could be modified to match first top level if in loop,
                //   notice parallel logic for client vs server sockets
                // If client socket is closed, server socket is open, client has no
                //   bytes read, and client has no fds queued:
            } else if ( c->server_fd != FD_CLOSED && c->clientcount == 0 &&
                        c->clientfdq.nfd == 0 ) {
                // close the server socket and log as "sent EOF"
                close( c->server_fd );
                c->server_fd = FD_CLOSED;
                if ( settings.readwritedebug )
                    fprintf(out,"%03d:<:sent EOF\n",c->id);
            }
            // If server socket is open:
            if ( c->server_fd != FD_CLOSED ) {
                // TBD If server has not maxed out buffer space or queued fds:
                if ( sizeof( c->serverbuffer ) > c->servercount &&
                     FDQUEUE_MAX_FD > c->serverfdq.nfd )
                    // monitor server socket for input (read readiness)
                    FD_SET( c->server_fd, &readfds );
                // monitor client socket for exceptional conditions (see poll(2) POLLPRI)
                FD_SET( c->server_fd, &exceptfds );
                // update nfds to include server socket
                if ( c->server_fd >= nfds )                               // n
                    nfds = c->server_fd + 1;                              // n
                // TBD what is role of clientignore?
                // TBD If client has queued fds, or has already had some bytes read:
                if ( ( ( c->clientignore > 0 && c->clientcount > 0 )
                       || c->clientfdq.nfd > 0 ) && allowsent > 0 ) {      // allowsent
                    // monitor server socket for output (write readiness)
                    FD_SET( c->server_fd, &writefds );
                }
            }
            // If connection is completely closed:
            if ( c->client_fd == FD_CLOSED && c->server_fd == FD_CLOSED ) {
                // If connection is at top of stack:
                if ( c == connections ) {
                    // Pop connection off stack
                    for ( int i {}; i < c->clientfdq.nfd; i++ )
                        close( c->clientfdq.fd[i] );
                    for ( int i {}; i < c->serverfdq.nfd; i++ )
                        close( c->serverfdq.fd[i] );
                    free_usedextensions( c->usedextensions );
                    free_unknownextensions( c->unknownextensions );
                    free_unknownextensions( c->waiting );
                    free( c->from );
                    connections = c->next;
                    free(c);
                    c = connections;
                    // If connection stack is empty and subprocess has exited: exit
                    // TBD note stopwhennone true by default
                    if ( connections == NULL && settings.stopwhennone &&
                        _child_pid == CHILD_EXITED ) {                     // _child_pid
                        return EXIT_SUCCESS;
                    }
                    continue;
                }
            }
        }

        /*
         * Set up interactive mode
         */
        // man xtrace:
        //     -i | --interactive
        //       Only sent(sic, send) requests from the client to the server after
        //       interactive confirmation.  Confirmation is given by pressing enter
        //       or a number followed by enter via stdin.  This can give funny results
        //       when multiple clients are tunneled.
        // If in interactive mode:
        if ( settings.interactive ) {
            // monitor stdin for input (read readiness)
            FD_SET( STDIN_FILENO, &readfds );
        }

        /*
         * Check if subprocess finished (child from cli `--`)
         */
        // If child has not exited, and select failed on previous loop or SIGCHLD caught
        if ( _child_pid != CHILD_EXITED &&
             ( select_ret == -1 || caught_child_signal ) ) {               // r  // _child_pid
            caught_child_signal = false;
            int status;
            // Check to see if child exited, but do not hang (wait until so)
            if ( waitpid( _child_pid, &status, WNOHANG ) == _child_pid ) {  // _child_pid
                _child_pid = CHILD_EXITED;                                 // _child_pid
                // If connection stack is empty and not set to wait after subcommand
                if ( connections == NULL && !settings.waitforclient ) {
                    /* TODO: instead wait a bit before terminating? */
                    // Return child exit code or number of signal that terminated
                    // TBD why +128?
                    if ( WIFEXITED( status ) )
                        return WEXITSTATUS( status );
                    else
                        return WTERMSIG( status ) + 128;
                }
            }
        }

        /*
         * Idle until any of the fds in the provided fd_sets becomes ready for
         *   reading, writing, or indicating exceptional conditions
         */
        select_ret = select( nfds, &readfds, &writefds, &exceptfds, NULL );  // n  // r
        // TBD Note that:
        //   - select failure resets but does not break loop
        //   - SIGINT forces select failure, but does not print error
        if ( select_ret == -1 ) {                                            // r
            if ( errno != 0 && errno != EINTR ) {
                // TBD exception
                //fprintf( stderr, "Error %d in select: %s\n", errno, strerror(errno) );
            }
            continue;
        }

        /*
         * second loop over connections to:
         *   - 
         */
        for ( c = connections; c != NULL; c = c->next ) {
            // TBD this prompting is meant to increment logging output
            // If in interactive mode:
            if ( settings.interactive && FD_ISSET( 0, &readfds ) ) {
                // Read integer from stdin
                // TBD may want to confirm that stdin is terminal with isatty(STDIN_FILENO):
                //    https://stackoverflow.com/a/1312957
                char buffer[201];
                ssize_t isread { read(STDIN_FILENO, buffer, 200) };
                // If stdin given EOF, end process
                if ( isread == 0 )
                    exit( EXIT_SUCCESS );
                if ( isread > 0 ) {
                    buffer[isread] = '\0';
                    int number { atoi(buffer) };
                    // TBD no error on failure to parse input as integer
                    // TBD no error if integer not positive
                    if ( number <= 0 )
                        number = 1;
                    // TBD !!! why does input modify allowsent (not documented in --help or man page)
                    allowsent += number;  // allowsent
                }
                // TBD no explicit handling of read(2) error (-1 return, errno)
            }
            // If client socket is open:
            if ( c->client_fd != FD_CLOSED ) {
                // If client socket had exceptional condition:
                if ( FD_ISSET( c->client_fd, &exceptfds ) ) {
                    // Close socket with message
                    close( c->client_fd );
                    c->client_fd = FD_CLOSED;
                    fprintf(stdout,"%03d: exception in communication with client\n",c->id);
                    continue;
                }
                // If client socket has pending write:
                if ( FD_ISSET( c->client_fd, &writefds ) ) {
                    // TBD servercount seems to be total bytes read from server
                    size_t towrite = c->servercount;
                    ssize_t written;
                    // TBD towrite set to min of serverignore and servercount
                    if ( c->serverignore < towrite )
                        towrite = c->serverignore;
                    // TBD Write server buffer to client socket
                    written = dowrite( c->client_fd, c->serverbuffer, towrite, &c->serverfdq );
                    // If any bytes were written to client socket:
                    if ( written >= 0 ) {
                        if ( settings.readwritedebug )
                            fprintf(stdout,"%03d:>:wrote %u bytes\n",c->id,(unsigned int)written);
                        // Remove successfully written bytes from buffer (slide bytes after written to offset 0)
                        if ( (size_t)written < c->servercount )
                            memmove(c->serverbuffer, c->serverbuffer + written, c->servercount - written);
                        // Update total bytes in buffer (more evidence that *count vars track total bytes in buffer)
                        c->servercount -= written;
                        // TBD how do *ignore values differ from *written when they are similarly decreased here?
                        c->serverignore -= written;
                        // If server buffer is empty:
                        if ( c->servercount == 0 ) {
                            // If server socket is closed:
                            // TBD resist urge to combine these if statements: the buffer could at times be empty
                            //   even if the server socket is not closed
                            if ( c->server_fd == FD_CLOSED ) {
                                // Close client socket
                                close( c->client_fd );
                                c->client_fd = FD_CLOSED;
                                if ( settings.readwritedebug )
                                    fprintf(stdout,"%03d:>:send EOF\n",c->id);
                                continue;
                            }
                            // TBD this should be a clue to the difference of *count and *ignore
                        } else if ( c->serverignore == 0 ) {
                            parse_server( c );
                        }
                        // 0 bytes written, client socket is at EOF or last write failed
                    } else {
                        // close client socket, continue to next connection
                        close( c->client_fd );
                        c->client_fd = FD_CLOSED;
                        if ( settings.readwritedebug ) {
                            fprintf(stdout,"%03d: error writing to client: %d=%s\n",
                                    c->id, errno, strerror(errno));
                        }
                        continue;
                    }
                }
                // If client socket has pending read:
                if ( FD_ISSET( c->client_fd, &readfds ) ) {
                    // Read from client into clientbuffer
                    size_t toread = sizeof(c->clientbuffer) - c->clientcount;
                    ssize_t wasread = doread( c->client_fd, c->clientbuffer + c->clientcount, toread, &c->clientfdq );
                    assert( toread > 0 );
                    // If any bytes read:
                    if ( wasread > 0 ) {
                        // Update client buffer size
                        if ( settings.readwritedebug )
                            fprintf(stdout,"%03d:<:received %u bytes\n",c->id,(unsigned int)wasread);
                        c->clientcount += wasread;
                        // Read 0 (EOF) or -1 (fail):
                    } else {
                        // Close client socket, continue to next connection
                        if ( settings.readwritedebug )
                            fprintf(stdout,"%03d:<:got EOF\n",c->id);
                        close( c->client_fd );
                        c->client_fd = FD_CLOSED;
                        continue;
                    }
                    // TBD we can see that *ignore needs to be 0 to do any printing of the buffer
                    if ( c->clientignore == 0 && c->clientcount > 0) {
                        parse_client( c );
                    }
                }
                // If client socket is closed, and server bytes have been read:
            } else if ( c->servercount > 0 && c->serverignore > 0 ) {
                /* discard additional events */
                unsigned int min { std::min( c->servercount, c->serverignore ) };
                fprintf(stdout, "%03d:s->?: discarded last answer of %u bytes\n", c->id, min);
                // TBD ??? if serverignore < servercount, remove serverignore bytes from server buffer
                if ( min < c->servercount )  // ( c->serverignore < c->servercount )
                    memmove( c->serverbuffer, c->serverbuffer + min, c->servercount - min);
                // TBD why are both of these unconditionally decreased, when removing serverignore bytes is
                //   conditional?
                c->servercount -= min;
                c->serverignore -= min;
                if ( c->serverignore == 0 && c->servercount > 0 ) {
                    parse_server( c );
                }
            }
            // If server socket is open:
            if ( c->server_fd != FD_CLOSED ) {
                // If server socket had exceptional condition:
                if ( FD_ISSET( c->client_fd, &exceptfds ) ) {
                    // Close socket with message, continue to next connection
                    close(c->server_fd);
                    c->server_fd = FD_CLOSED;
                    fprintf( stdout, "%03d: exception in communication with server\n", c->id );
                    continue;
                }
                // If server socket has pending write:
                if ( FD_ISSET( c->server_fd, &writefds ) ) {
                    size_t towrite { std::min( c->clientcount, c->clientignore ) };
                    ssize_t written;
                    written = dowrite( c->server_fd, c->clientbuffer, towrite, &c->clientfdq );
                    if ( settings.interactive && allowsent > 0 )   // allowsent
                        allowsent--;                               // allowsent
                    // If bytes written to server socket:
                    if ( written >= 0 ) {
                        if ( settings.readwritedebug )
                            fprintf(stdout,"%03d:<:wrote %u bytes\n",c->id,(unsigned int)written);
                        // If client buffer was not completely emptied:
                        if ( (size_t)written < c->clientcount )
                            // remove written bytes from client buffer
                            memmove( c->clientbuffer, c->clientbuffer + written, c->clientcount - written );
                        c->clientcount -= written;
                        c->clientignore -= written;
                        // If bytes remain in client buffer: log print next packet
                        if ( c->clientcount != 0 && c->clientignore == 0 ) {
                            parse_client( c );
                        }
                    } else {
                        close( c->server_fd );
                        c->server_fd = FD_CLOSED;
                        if ( settings.readwritedebug )
                            fprintf(stdout,"%03d: error writing to server: %d=%s\n",c->id,e,strerror(e));
                        continue;
                    }
                }
                // If server socket has pending read:
                if ( FD_ISSET( c->server_fd, &readfds ) ) {
                    size_t toread = sizeof(c->serverbuffer) - c->servercount;
                    ssize_t wasread = doread( c->server_fd, c->serverbuffer + c->servercount, toread, &c->serverfdq );
                    assert( toread > 0 );
                    if ( wasread > 0 ) {
                        if ( settings.readwritedebug )
                            fprintf(stdout,"%03d:>:received %u bytes\n",c->id,(unsigned int)wasread);
                        c->servercount += wasread;
                    } else {
                        if ( settings.readwritedebug )
                            fprintf(stdout,"%03d:>:got EOF\n",c->id);
                        close( c->server_fd );
                        c->server_fd = FD_CLOSED;
                    }
                    if ( c->serverignore == 0 && c->servercount > 0 ) {
                        parse_server( c );
                    }
                }
            } else if ( c->clientcount > 0 && c->clientignore > 0 ) {
                unsigned int min { c->clientcount };
                /* discard additional events */
                if ( min > c->clientignore )
                    min = c->clientignore;
                fprintf(stdout,"%03d:<: discarding last request of %u bytes\n",c->id,min);
                if ( min < c->clientcount )
                    memmove( c->clientbuffer, c->clientbuffer + min, c->clientcount - min );
                c->clientcount -= min;
                c->clientignore -= min;
                if ( c->clientignore == 0 && c->clientcount > 0 ) {
                    parse_client( c );
                }
            }
        }

        // new connections are driven by listener socket accept(2)ing clients
        if ( FD_ISSET( _listener_fd, &readfds ) ) {
            acceptConnection();
        }

    }
    return EXIT_SUCCESS;
}
