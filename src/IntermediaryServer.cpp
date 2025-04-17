#include <iostream>  // TBD testing only

#include <string>        // stoi to_string
#include <algorithm>     // transform
#include <regex>         // smatch regex regex_search
//#include <filesystem>    // remove

#include <cassert>
#include <cstdlib>       // getenv
#include <cstring>       // memcpy

#include <sys/socket.h>  // 'struct sockaddr' SOCK_STREAM AF_INET AF_UNIX setsockopt SOL_SOCKET SO_KEEPALIVE 
#include <arpa/inet.h>   // htons htonl
#include <netinet/in.h>  // 'struct sockaddr_in'
#include <sys/un.h>      // 'struct sockaddr_un'
#include <unistd.h>      // unlink close

#include "IntermediaryServer.hpp"
#include "typeName.hh"

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
        std::endl;

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
        "\tunix_socket_path: " << _in_display.unix_socket_path <<
        std::endl;
}

IntermediaryServer::IntermediaryServer() {
}

IntermediaryServer::~IntermediaryServer() {
    close( _in_fd );
    if ( !_in_display.unix_socket_path.empty() )
        unlink( _in_display.unix_socket_path.data() );
    // close( _out_fd );
    // if ( !_out_display.unix_socket_path.empty() )
    //     unlink( _out_display.unix_socket_path.data() );
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
        inaddr.sin_port        = htons( _X_TCP_PORT + _in_display.display ); // calculateTcpPort();
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

    _in_fd = fd;
}
