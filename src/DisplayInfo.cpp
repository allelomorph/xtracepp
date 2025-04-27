#include <string>        // stoi
#include <regex>         // smatch regex regex_search
#include <algorithm>     // transform
#include <iostream>      // cout
#include <locale>

#include <cassert>
#include <cctype>        // tolower

#include <sys/socket.h>  // AF_INET AF_UNIX

#include "DisplayInfo.hpp"


DisplayInfo::DisplayInfo(const char* display_name) {
    assert( display_name != nullptr );

    // extract protocol token if present, skip over hostname token if present
    std::string dname { display_name };
    std::smatch dname_match;
    // Xlib reads display names as:
    //     [protocol/][hostname]:[:]displaynumber[.screennumber]
    //   here we are assuming TCP (single colon), see:
    //   - https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html#Opening_the_Display
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
                    []( unsigned char c ){ return std::tolower( c ); } );
    hostname = dname_match[2];
    std::transform( hostname.begin(), hostname.end(), hostname.begin(),
                    []( unsigned char c ){ return std::tolower( c ); } );
    display = std::stoi( dname_match[3] );
    if ( dname_match[4] != "" )
        screen = std::stoi( dname_match[4] );
    // Implementing the TODO from xtrace:
    //   "make sure we are in C locale, otherwise this can go wrong"
    //   (alt assertion: std::cout.getloc().name() == "C")
    // TBD is this still necessary?
    assert( std::cout.getloc() == std::locale::classic() );
    // default protocol for hostnames other than unix is "tcp"
    // default protocol for hostname unix or missing hostname is "local"
    if ( !hostname.empty() && hostname != "unix") {
        if ( protocol.empty() )
            protocol = std::string( "tcp" );
    } else {
        // clearing of hostname "unix" as in xtrace source
        // TBD is this still necessary?
        hostname.clear();
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
