#include <cassert>

#include <poll.h>  // nfds_t pollfd POLL(IN|OUT|ERR|HUP|NVAL)

#include <fmt/format.h>

#include "ProxyX11Server.hpp"
#include "errors.hpp"

#include "protocol/version.hpp"
#include "protocol/connection_setup.hpp"
#include "protocol/requests.hpp"
#include "protocol/events.hpp"
#include "protocol/errors.hpp"


// no forking or threading, want this to be completed syncronously before main queue begins


void ProxyX11Server::_pollSingleSocket(
    const int socket_fd, const short events ) {
    assert( events == POLLIN || events == POLLOUT ||
            events == POLLIN | POLLOUT );
    const      nfds_t   nfds { 1 };
    /*struct */pollfd   pfds[1];
    pfds[0].fd = socket_fd;
    static constexpr int POLL_TIMEOUT_MS { 3000 };

    pfds[0].events = events;
    switch ( poll( pfds, nfds, POLL_TIMEOUT_MS ) ) {
    case 0:
        throw std::runtime_error(
            fmt::format( "{}: poll timeout in {} ms",
                         __PRETTY_FUNCTION__, POLL_TIMEOUT_MS ) );
        break;
    case -1:
        throw errors::system::exception(
            fmt::format( "{}: poll", __PRETTY_FUNCTION__ ) );
        break;
    default:
        if ( !( pfds[0].revents & events ) ) {
            throw std::runtime_error(
                fmt::format(
                    "{}: poll marked failure from {}",
                    __PRETTY_FUNCTION__,
                    pfds[0].revents & POLLERR ? "error condition" :
                    pfds[0].revents & POLLHUP ? "hangup" :
                    /*pfds[0].revents & POLLNVAL ?*/ "invalid (closed) fd" ) );
        }
        break;
    }
}

void ProxyX11Server::_fetchCurrentServerTime() {
    const int server_fd { _connectToServer() };
    if( server_fd < 0 ) {
        // TBD exception
        fmt::println(
            stderr, "{}: failure to connect to X server for display: {}",
            __PRETTY_FUNCTION__, _out_display.name );
        return;
    }
    SocketBuffer sbuffer;

    ////// Establish connection // TBD ( could be subroutine to DRY with atom prefetching )

    using protocol::connection_setup::ClientInitiation;
    ClientInitiation::Header init_header;
    init_header.byte_order = ClientInitiation::Header::LSB_FIRST;
    init_header.protocol_major_version = protocol::MAJOR_VERSION;
    init_header.protocol_minor_version = protocol::MINOR_VERSION;
    init_header.n = _AUTH_NAME.size();
    init_header.d = _AUTH_DATA_SZ;
    sbuffer.load( &init_header, sizeof(init_header) );
    // TBD padded sz will copy up to 3 junk bytes
    sbuffer.load( _AUTH_NAME.data(), _pad( _AUTH_NAME.size() ) );
    sbuffer.load( auth_data, _pad( _AUTH_DATA_SZ ) );
    assert( sbuffer.size() ==
        sizeof(init_header) + _pad( _AUTH_NAME.size() ) + _pad( _AUTH_DATA_SZ ) );
    {
        try {
            _pollSingleSocket( server_fd, POLLOUT );
        } catch ( const std::exception& e ) {
            fmt::println( stderr, "{}", e.what() );
            goto close_socket;
        }
        sbuffer.write( server_fd );
        assert( sbuffer.empty() );
    }

    protocol::WINDOW screen0_root;
    {
        try {
            _pollSingleSocket( server_fd, POLLIN );
        } catch ( const std::exception& e ) {
            fmt::println( stderr, "{}", e.what() );
            goto close_socket;
        }
        sbuffer.read( server_fd );
        using protocol::connection_setup::ServerAcceptance;
        assert( sbuffer.size() >= sizeof( ServerAcceptance::Header ) );
        ServerAcceptance::Header acceptance_header;
        sbuffer.unload( &acceptance_header, sizeof( ServerAcceptance::Header ) );
        assert( sbuffer.size() >= _pad( acceptance_header.v ) +
                acceptance_header.n * sizeof( ServerAcceptance::FORMAT ) +
                sizeof( ServerAcceptance::SCREEN::Header ) );
        // TBD use err fail instead of assert
        assert( acceptance_header.success == protocol::connection_setup::SUCCESS );
        assert( acceptance_header.protocol_major_version ==
                init_header.protocol_major_version );
        assert( acceptance_header.protocol_minor_version ==
                init_header.protocol_minor_version );
        // TBD skip over vendor
        sbuffer.unload( _pad( acceptance_header.v ) );
        // TBD skip over pixmap-formats
        sbuffer.unload( acceptance_header.n * sizeof( ServerAcceptance::FORMAT ) );
        // TBD get WINDOW for root window of first screen
        ServerAcceptance::SCREEN::Header screen_header;
        sbuffer.unload( &screen_header, sizeof( ServerAcceptance::SCREEN::Header ) );
        screen0_root = screen_header.root;
        sbuffer.clear();
    }

    ////// Send ChangeWindowAttributes on screen->root to toggle reporting PropertNotify events

    {
        using protocol::requests::ChangeWindowAttributes;
        ChangeWindowAttributes::Encoding cwa_encoding {};
        cwa_encoding.opcode = protocol::requests::opcodes::CHANGEWINDOWATTRIBUTES;
        cwa_encoding.request_length = _alignedUnits(
            sizeof( ChangeWindowAttributes::Encoding ) + sizeof( protocol::VALUE ) );
        cwa_encoding.window = screen0_root;
        cwa_encoding.value_mask = /*XCB_CW_EVENT_MASK*/ 1 << 11;
        sbuffer.load( &cwa_encoding, sizeof( cwa_encoding ) );
        const protocol::VALUE value_list[1] { /*XCB_EVENT_MASK_PROPERTY_CHANGE*/ 1 << 22 };
        sbuffer.load( value_list, sizeof( protocol::VALUE ) );
        assert( sbuffer.size() == sizeof( cwa_encoding ) + sizeof( protocol::VALUE ) );
        try {
            _pollSingleSocket( server_fd, POLLOUT );
        } catch ( const std::exception& e ) {
            fmt::println( stderr, "{}", e.what() );
            goto close_socket;
        }
        sbuffer.write( server_fd );
        assert( sbuffer.empty() );
    }

    ////// Send ChangeProperty with 0-length append as noop
    // TBD see https://stackoverflow.com/questions/61849695/get-current-x11-server-time

    {
        using protocol::requests::ChangeProperty;
        ChangeProperty::Encoding cp_encoding {};
        cp_encoding.opcode = protocol::requests::opcodes::CHANGEPROPERTY;
        // TBD currenly no enum constants in procotol::, only name arrays
        cp_encoding.mode = /*XCB_PROP_MODE_APPEND 2*/0x02;
        cp_encoding.request_length = _alignedUnits( sizeof( cp_encoding ) );
        cp_encoding.window = screen0_root;
        cp_encoding.property.data = /*XCB_ATOM_WM_NAME 39*/0x27;
        cp_encoding.type.data = /*XCB_ATOM_STRING 31*/0x1f;
        cp_encoding.format = 8;  // (bits per fmt unit)
        cp_encoding.fmt_unit_ct = 0;  // 0-length append to act as noop
        sbuffer.load( &cp_encoding, sizeof( cp_encoding ) );
        assert( sbuffer.size() == sizeof( cp_encoding ) );
        try {
            _pollSingleSocket( server_fd, POLLOUT );
        } catch ( const std::exception& e ) {
            fmt::println( stderr, "{}", e.what() );
            goto close_socket;
        }
        sbuffer.write( server_fd );
        assert( sbuffer.empty() );
    }

    ////// verify properly formatted PropertyNotify reply, collect timestamp

    {
        try {
            _pollSingleSocket( server_fd, POLLIN );
        } catch ( const std::exception& e ) {
            fmt::println( stderr, "{}", e.what() );
            goto close_socket;
        }
        sbuffer.read( server_fd );
        using protocol::events::PropertyNotify;
        PropertyNotify::Encoding pn_encoding;
        assert( sbuffer.size() == sizeof( PropertyNotify::Encoding ) );
        sbuffer.unload( &pn_encoding, sizeof( pn_encoding ) );
        assert( pn_encoding.code == protocol::events::codes::PROPERTYNOTIFY );
        assert( pn_encoding.sequence_number == 2 );  // event from second Request
        assert( pn_encoding.window.data == screen0_root.data );
        assert( pn_encoding.atom.data == /*XCB_ATOM_WM_NAME 39*/0x27 );
        assert( pn_encoding.state == /*XCB_PROPERTY_NEW_VALUE*/0 );
        // assign reference points
        settings.ref_TIMESTAMP = pn_encoding.time.data;
        settings.ref_unix_time = time(nullptr);
        sbuffer.clear();
    }

close_socket:
    // sends EOF
    close( server_fd );
}
