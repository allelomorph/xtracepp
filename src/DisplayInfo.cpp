#include <string>        // stoi
#include <regex>         // smatch regex regex_search
#include <algorithm>     // transform
#include <locale>

#include <cassert>
#include <cctype>        // tolower

#include <sys/socket.h>  // sockaddr AF_INET AF_UNIX
#include <sys/types.h>
#include <netdb.h>       // getaddrinfo
#include <netinet/in.h>  // sockaddr_in
#include <sys/un.h>      // sockaddr_un
#include <arpa/inet.h>   // inet_ntop

#include <fmt/format.h>

#include "DisplayInfo.hpp"
#include "errors.hpp"


DisplayInfo::DisplayInfo( const char* display_name, const Direction direction ) {
    assert( display_name != nullptr );

    ////// Extract tokens from name string

    name = std::string( display_name );
    // extract protocol token if present, skip over hostname token if present
    std::string dname { display_name };
    std::smatch dname_match;
    // Xlib reads display names as:
    //     [protocol/][hostname]:[:]displaynumber[.screennumber]
    //   see:
    //   - https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html#Opening_the_Display
    std::regex dname_regex {
        "^\\s*"
        "(?:([a-zA-Z]+)/)?\\s*"  // protocol/
        "([a-zA-Z]+)?\\s*"       // hostname
        "(:{1,2})\\s*"           // :[:]
        "([0-9]+)\\s*"           // displaynum
        "(?:\\.([0-9]+))?\\s*"   // .screennum
        "$"
    };
    const bool valid_dname_format {
        std::regex_search( dname, dname_match, dname_regex ) };
    assert( valid_dname_format );
    name = dname;
    protocol = dname_match[ _PROTOCOL_GROUP_I ];
    std::transform( protocol.begin(), protocol.end(), protocol.begin(),
                    []( unsigned char c ){ return std::tolower( c ); } );
    // TBD X11 over TCP without ssh is deprecated, but could create an edge case
    //   of a hostname URI and and not just "localhost", see:
    //   - https://github.com/mviereck/x11docker/wiki/How-to-access-X-over-TCP-IP-network
    hostname = dname_match[ _HOSTNAME_GROUP_I ];
    std::transform( hostname.begin(), hostname.end(), hostname.begin(),
                    []( unsigned char c ){ return std::tolower( c ); } );
    // default protocol for hostname "unix" or missing hostname is "local"
    // default protocol for hostnames other than "unix" is "tcp"
    // TBD protocol in the context of an X display name does not seem to map
    //   directly to protocols in the context of addrinfo.ai_protocol IPP_*
    //   macros
    // TBD note that protocol token seems to be completely ignored by libxcb:
    //     xcb_parse_display(name, host, displayp, screenp) <
    //     _xcb_parse_display(name, host, protocol (NULL), displayp, screenp)
    //   and thus also libX11:
    //     XOpenDisplay(display) < _XConnectXCB(dpy, display, &iscreen) <
    //     xcb_parse_display(display, &host, &n, screenp)
    if ( protocol.empty() ) {
        protocol = ( hostname.empty() || hostname == _UNIX ) ?
            std::string( _LOCAL ) :
            std::string( _TCP );
    }
    // TBD better context in error about meaning of colons (DECnet, etc)
    // TBD why is family in auth file FamilyDECnet?
    if ( dname_match[ _COLON_GROUP_I ] != ":" ) {
        fmt::println( stderr, "Expecting display names with single colon, "
                      "got \"{}\"", name );
        exit( EXIT_FAILURE );
    }

    ////// Assign numerical values from tokens

    assert( !dname_match[ _DISPLAY_GROUP_I ].str().empty() );
    display = std::stoi( dname_match[ _DISPLAY_GROUP_I ] );
    // TBD libxcb/libX11 default to screen 0 in _xcb_parse_display
    if ( !dname_match[ _SCREEN_GROUP_I ].str().empty() )
        screen = std::stoi( dname_match[ _SCREEN_GROUP_I ] );
    // TBD later implement ipv6 support for "inet6" protocol
    // only allow inet, tcp, unix, or local protocols
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
        const std::string unix_path {
            fmt::format( "{}{}",
            _UNIX_SOCKET_PATH_PREFIX, dname_match[ _DISPLAY_GROUP_I ].str() ) };
        assert( unix_path.size() < _UNIX_PATH_MAX );
        memcpy( unaddr.sun_path, unix_path.data(), unix_path.size() + 1 );
        unix_socket_path = { unaddr.sun_path };

        // fmt::println( stderr, "name {:?} protocol {:?} hostname {:?} display {} screen {} family {}",
        //               name, protocol, hostname, display, screen, family );
        // fmt::println( stderr, "unaddr {{ sun_family {} sun_path {:?} }}",
        //               unaddr.sun_family, unaddr.sun_path );
        // fmt::println( stderr, "unix_socket_path {:?}",
        //               unix_socket_path );
        return;
    }
    assert( family == AF_INET );
    // for bind(2) on listening socket
    if ( direction == Direction::IN ) {
        // TBD could also use getaddrinfo with NULL node and
        //   hints.ai_flags = AI_PASSIVE, but would still need to manually
        //   assign port
        inaddr.sin_family      = family;
        // bind socket to all local interfaces (network byte order)
        inaddr.sin_addr.s_addr = htonl( INADDR_ANY );
    }
    // for connect(2) on server-facing socket
    if ( direction == Direction::OUT ) {
        addrinfo hints  {};
        hints.ai_family   = family;
        hints.ai_socktype = SOCK_STREAM;
        addrinfo* res {};
        // NULL service skips setting port number
        const int gai_ret {
            getaddrinfo( hostname.data(), nullptr, &hints, &res ) };
        if ( gai_ret != 0 ) {
            fmt::println( stderr, "{}: {}", __PRETTY_FUNCTION__,
                          errors::getaddrinfo::message( gai_ret ) );
            exit( EXIT_FAILURE );
        }
        assert( res != nullptr );
        const addrinfo* res_match {};
        for ( addrinfo* res_memb { res }; res_memb != nullptr;
              res_memb = res_memb->ai_next ) {
            if ( res_memb->ai_family == family &&
                 res_memb->ai_socktype == SOCK_STREAM /*&&
                 res_memb->ai_protocol == IPP_PROTOTCP*/ ) {
                res_match = res_memb;
            }
        }
        assert( res_match != nullptr );
        assert( res_match->ai_addrlen == sizeof( inaddr ) );
        memcpy( &inaddr, res_match->ai_addr, res_match->ai_addrlen );
        freeaddrinfo( res );
    }
    inaddr.sin_port = htons( _X_TCP_PORT + display );
    inet_ntop( inaddr.sin_family, &(inaddr.sin_addr),
               _ipv4_addrstr_buf, INET_ADDRSTRLEN );

    // fmt::println( stderr, "name {:?} protocol {:?} hostname {:?} display {} screen {} family {}",
    //               name, protocol, hostname, display, screen, family );
    // fmt::println( stderr, "inaddr {{ sin_family {} sin_port {} sin_addr.s_addr {:#x} }}",
    //               inaddr.sin_family, ntohs( inaddr.sin_port ), inaddr.sin_addr.s_addr );
    // fmt::println( stderr, "ipv4_addr {:?}",
    //                   ipv4_addr );
}
