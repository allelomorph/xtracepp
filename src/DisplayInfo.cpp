#include <string_view>
#include <string>        // stoi
#include <regex>         // smatch regex_search
#include <algorithm>     // transform

#include <cassert>
#include <cctype>        // tolower

#include <sys/socket.h>  // sockaddr AF_INET AF_UNIX
#include <sys/types.h>
#include <sys/stat.h>    // stat S_IFSOCK
#include <unistd.h>      // close ::exit
#include <netdb.h>       // addrinfo getaddrinfo AI_PASSIVE
#include <netinet/in.h>  // sockaddr_in INET_ADDRSTRLEN INET6_ADDRSTRLEN
#include <sys/un.h>      // sockaddr_un
#include <arpa/inet.h>   // inet_ntop

#include <fmt/format.h>

#include "DisplayInfo.hpp"
#include "errors.hpp"


DisplayInfo::DisplayInfo( const char* display_name, const Direction direction,
                          const std::string_view& process_name ) {
    assert( display_name != nullptr );

    ////// Parse display name into relevant fields

    name = std::string( display_name );
    std::smatch match;
    // TBD not providing default display or screen of 0 as in libxcb/libX11
    // `struct` needed to disambiguate from stat(2)
    if ( struct ::stat st {};
         std::regex_search( name, match, _UNIX_REGEX ) &&
         ::stat( match[ _UNIX_SOCKET_PATH_GROUP_I ].str().data(), &st ) == 0 &&
         st.st_mode & S_IFSOCK ) {
        _unix_pattern = true;
        protocol      = _UNIX;
        socket_path   = match[ _UNIX_SOCKET_PATH_GROUP_I ];
        if ( const std::string screen_str { match[ _UNIX_SCREEN_GROUP_I ] };
             !screen_str.empty() ) {
            screen  = std::stoi( screen_str );
        }
    } else if ( std::regex_search( name, match, _DEFAULT_REGEX ) ) {
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
        fmt::println( ::stderr,
                      "{}: Could not parse display name {:?} as one of two valid patterns:\n"
                      "    [unix:]<socket path>[.<screen number>]\n"
                      "    [[<protocol>/]<hostname>]:<display number>[.<screen number>]",
                      process_name, name );
        ::exit( EXIT_FAILURE );
    }
    // default to IPv6 with IPv4 support, unless explicitly passing "inet" as protocol
    if ( protocol == _INET6 || protocol == _TCP ) {
        ai_family = AF_INET6;
    } else if ( protocol == _INET ) {
        ai_family = AF_INET;
    } else if ( protocol == _UNIX || protocol == _LOCAL ) {
        ai_family = AF_UNIX;
    } else {
        fmt::println( ::stderr, "{}: Unrecognized protocol {:?} in display name {:?}",
                      process_name, protocol, name );
        ::exit( EXIT_FAILURE );
    }

    ////// Get socket info for later calls to bind(2) or connect(2)

    if ( ai_family == AF_UNIX ) {
        ai_socktype = ::SOCK_STREAM;
        ai_protocol = _SOCKET_DEFAULT_PROTOCOL;
        ai_addrlen  = sizeof( unaddr );

        unaddr.sun_family = ai_family;
        if ( socket_path.empty() ) {
            assert( !_unix_pattern );
            socket_path = fmt::format(
                "{}{}", _UNIX_SOCKET_PATH_PREFIX,
                match[ _DISPLAY_GROUP_I ].str() );
            assert( socket_path.size() < _UNIX_PATH_MAX );
            assert( socket_path.size() < sizeof( unaddr.sun_path ) );
        }
        memcpy( unaddr.sun_path, socket_path.data(), socket_path.size() + 1 );
        return;
    }
    assert( ai_family == AF_INET || ai_family == AF_INET6 );
    ::addrinfo hints {};
    hints.ai_flags    = ( direction == Direction::IN ) ? AI_PASSIVE : 0;
    hints.ai_socktype = ::SOCK_STREAM;
    hints.ai_protocol = _SOCKET_DEFAULT_PROTOCOL;
    const char* node {
        ( direction == Direction::IN ) ? nullptr : hostname.data() };
    ::addrinfo* results {};
    // node and service params can't both be NULL
    if ( const int gai_ret { ::getaddrinfo( node, "", &hints, &results ) };
         gai_ret != 0 ) {
        fmt::println( ::stderr, "{}: {}: {}", process_name, __PRETTY_FUNCTION__,
                      errors::getaddrinfo::message( gai_ret ) );
        ::exit( EXIT_FAILURE );
    }
    const ::addrinfo* viable_result {};
    for ( ::addrinfo* result { results };
          result != nullptr; result = result->ai_next ) {
        if ( direction == Direction::OUT ) {
            if ( result->ai_family == AF_INET ) {
                reinterpret_cast< sockaddr_in* >( result->ai_addr )->sin_port =
                    htons( _X_TCP_PORT + display );
            } else {  // result->ai_family == AF_INET6
                reinterpret_cast< sockaddr_in6* >( result->ai_addr )->sin6_port =
                    htons( _X_TCP_PORT + display );
            }
        }
        if ( const int fd { socket( result->ai_family, result->ai_socktype,
                                    result->ai_protocol ) };
            ( direction == Direction::IN &&
              ::bind( fd, result->ai_addr, result->ai_addrlen ) == 0 ) ||
            ( direction == Direction::OUT &&
              ::connect( fd, result->ai_addr, result->ai_addrlen ) == 0 ) ) {
            ::close( fd );
            viable_result = result;
            break;
        }
    }
    if ( viable_result == nullptr ) {
        fmt::println( ::stderr, "{}: Could not find viable address for hostname {:?} "
                      "in display name {:?}", process_name, hostname, name );
        ::exit( EXIT_FAILURE );
    }
    ai_family   = results->ai_family;
    ai_socktype = results->ai_socktype;
    ai_protocol = results->ai_protocol;
    ai_addrlen  = results->ai_addrlen;
    if ( ai_family == AF_INET ) {
        const ::sockaddr_in* inaddr_ {
            reinterpret_cast< ::sockaddr_in* >( viable_result->ai_addr ) };
        inaddr = *inaddr_;
        assert( ai_addrlen == sizeof( inaddr ) );
        ::inet_ntop( ai_family, &(inaddr.sin_addr),
                     _addrstr_buf, INET_ADDRSTRLEN );
    } else {  // ai_family == AF_INET6
        const ::sockaddr_in6* in6addr_ {
            reinterpret_cast< ::sockaddr_in6* >( viable_result->ai_addr ) };
        in6addr = *in6addr_;
        assert( viable_result->ai_addrlen == sizeof( in6addr ) );
        ::inet_ntop( ai_family, &(in6addr.sin6_addr),
                     _addrstr_buf, INET6_ADDRSTRLEN );
    }
    ::freeaddrinfo( results );
}
