#include <string>        // stoi
#include <regex>         // smatch regex regex_search
#include <algorithm>     // transform

#include <cassert>
#include <cctype>        // tolower

#include <sys/socket.h>  // sockaddr AF_INET AF_UNIX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>      // close
#include <netdb.h>       // getaddrinfo
#include <netinet/in.h>  // sockaddr_in
#include <sys/un.h>      // sockaddr_un
#include <arpa/inet.h>   // inet_ntop

#include <fmt/format.h>

#include "DisplayInfo.hpp"
#include "errors.hpp"


DisplayInfo::DisplayInfo( const char* display_name, const Direction direction ) {
    assert( display_name != nullptr );

    ////// Parse display name into relevant fields

    name = std::string( display_name );
    // libX11 and libxcb both parse display names with call chains ending in
    //   _xcb_parse_display, which supports two methods: an undocumented pattern
    //   for explicit passing of a unix socket:
    //
    //     [unix:]<socket path>[.<screen number>]
    //
    //   and the documented pattern for TCP/DECnet (defaulting to unix sockets
    //   when no hostname is provided):
    //
    //     [[<protocol>/]<hostname>]:[:]<display number>[.<screen number>]
    //
    //   see:
    //     - https://gitlab.freedesktop.org/xorg/lib/libxcb/-/blob/e81b999a/src/xcb_util.c#L160
    //     - https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html#Opening_the_Display
    std::regex unix_regex {
        "^(?:(unix):)?"      // [<protocol>:]
        "(.+?)"              // <socket path>
        "(?:\\.([0-9]+))?$"  // [.<screen number>]
    };
    std::regex default_regex {
        "^(?:"
            "(?:([a-zA-Z]+)/)?"    // [ [<protocol>/]
            "([^\\s]+?)"           // <hostname/URI> ]
        ")?"
        "(?="
            ":([0-9]+)"            // :<display number>
            "(?:\\.([0-9]+))?"     // [.<screen number>]
        "$)"
    };
    std::smatch match;
    // TBD not providing default display or screen of 0 as in libxcb/libX11
    if ( struct stat st {};
         std::regex_search( name, match, unix_regex ) &&
         stat( match[ _UNIX_SOCKET_PATH_GROUP_I ].str().data(), &st ) == 0 &&
         st.st_mode & S_IFSOCK ) {
        _unix_pattern = true;
        protocol      = _UNIX;
        socket_path   = match[ _UNIX_SOCKET_PATH_GROUP_I ];
        if ( const std::string screen_str { match[ _UNIX_SCREEN_GROUP_I ] };
             !screen_str.empty() ) {
            screen  = std::stoi( screen_str );
        }
    } else if ( std::regex_search( name, match, default_regex ) ) {
        protocol = match[ _PROTOCOL_GROUP_I ];
        std::transform( protocol.begin(), protocol.end(), protocol.begin(),
                        []( unsigned char c ){ return std::tolower( c ); } );
        hostname = match[ _HOSTNAME_GROUP_I ];
        // "protocol" here is only for internal reference, as it:
        //   - may not map to IPPROTO macros one would pass to socket(2), see:
        //     - ip(7)
        //     - protocols(5)
        //   - is ignored by libX11/libxcb, see:
        //     - https://gitlab.freedesktop.org/xorg/lib/libxcb/-/blob/e81b999a/src/xcb_util.c#L238
        if ( protocol.empty() ) {
            protocol = hostname.empty() ? _LOCAL : _TCP;
        }
        assert( !match[ _DISPLAY_GROUP_I ].str().empty() );
        display = std::stoi( match[ _DISPLAY_GROUP_I ] );
        if ( const std::string screen_str { match[ _SCREEN_GROUP_I ] };
             !screen_str.empty() ) {
            screen  = std::stoi( screen_str );
        }
    } else {
        fmt::println(
            stderr, "Could not parse display name {:?} as one of two valid patterns:\n"
            "    [unix:]<socket path>[.<screen number>]\n"
            "    [[<protocol>/]<hostname>]:<display number>[.<screen number>]", name );
        exit( EXIT_FAILURE );
    }
    // TBD later implement ipv6 support for "inet6" protocol
    if ( protocol == _INET || protocol == _TCP ) {
         family = AF_INET;
    } else if ( protocol == _UNIX || protocol == _LOCAL ) {
         family = AF_UNIX;
    } else {
        fmt::println( stderr, "Unrecognized protocol {:?} in display name {:?}",
                      protocol, name );
        exit( EXIT_FAILURE );
    }

    ////// Get socket info for later calls to bind(2) or connect(2)

    if ( family == AF_UNIX ) {
        unaddr.sun_family = family;
        if ( socket_path.empty() ) {
            assert( !_unix_pattern );
            socket_path = fmt::format(
                "{}{}", _UNIX_SOCKET_PATH_PREFIX,
                match[ _DISPLAY_GROUP_I ].str() );
            assert( socket_path.size() < _UNIX_PATH_MAX );
        }
        memcpy( unaddr.sun_path, socket_path.data(), socket_path.size() + 1 );
        return;
    }
    assert( family == AF_INET );
    // for bind(2) on listening socket
    if ( direction == Direction::IN ) {
        // TBD could also use getaddrinfo with NULL node and hints.ai_flags =
        //   AI_PASSIVE, but would still need to manually assign port
        inaddr.sin_family      = family;
        // bind socket to all local interfaces (network byte order)
        inaddr.sin_addr.s_addr = htonl( INADDR_ANY );
    }
    // for connect(2) on server-facing socket
    if ( direction == Direction::OUT ) {
        addrinfo hints  {};
        hints.ai_family   = family;
        hints.ai_socktype = SOCK_STREAM;
        addrinfo* results {};
        // getaddrinfo with NULL service param skips setting port number
        if ( const int gai_ret { getaddrinfo(
                    hostname.data(), nullptr, &hints, &results ) };
            gai_ret != 0 ) {
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::getaddrinfo::message( gai_ret ) );
            exit( EXIT_FAILURE );
        }
        assert( results != nullptr );
        const addrinfo* result {};
        for ( addrinfo* addr { results };
              addr != nullptr; addr = addr->ai_next ) {
            reinterpret_cast< sockaddr_in* >( addr->ai_addr )->sin_port =
                htons( _X_TCP_PORT + display );
            if ( const int fd { socket(
                        addr->ai_family, addr->ai_socktype, addr->ai_protocol ) };
                connect( fd, addr->ai_addr, addr->ai_addrlen ) == 0 ) {
                close( fd );
                result = addr;
                break;
            }
        }
        if ( result == nullptr ) {
            fmt::println( stderr, "Could not find viable address for hostname {:?} "
                          "in display name {:?}",
                          hostname, name );
            exit( EXIT_FAILURE );
        }
        assert( result->ai_addrlen == sizeof( inaddr ) );
        inaddr = *reinterpret_cast< sockaddr_in* >( result->ai_addr );
        freeaddrinfo( results );
    }
    inet_ntop( inaddr.sin_family, &(inaddr.sin_addr),
               _ipv4_addrstr_buf, INET_ADDRSTRLEN );
}
