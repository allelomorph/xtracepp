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


#include <sys/select.h>


// no forking or threading, want this to be completed syncronously before main queue begins

// TBD can we be friends with X11ProtocolParser without circular include?
// "where E is some expression, and pad(E) is the number of bytes needed to
//   round E up to a multiple of four."
inline static constexpr size_t _ALIGN { 4 };
inline static size_t _padToAlignment( const size_t n, const size_t align ) {
    return n + ( ( align - ( n % align ) ) % align );
};
inline static size_t _pad( const size_t n ) {
    return _padToAlignment( n, _ALIGN );
}
inline static size_t _alignedUnits( const size_t sz ) {
    return sz / _ALIGN;
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
    const      nfds_t   nfds { 1 };
    /*struct */pollfd   pfds[1];
    pfds[0].fd = server_fd;
    static constexpr int POLL_TIMEOUT_MS { 5000 };

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

    pfds[0].events = POLLOUT;
    switch ( poll( pfds, nfds, POLL_TIMEOUT_MS ) ) {
    case 0:
        // timeout err ret
        fmt::println( stderr, "{}: poll timeout on write", __PRETTY_FUNCTION__ );
        goto close_socket;
        break;
    case -1:
        // errno err ret
        fmt::println( stderr, "{}: {}",
                      __PRETTY_FUNCTION__, ::errors::system::message( "poll" ) );
        goto close_socket;
        break;
    default:
        if ( pfds[0].revents & POLLOUT ) {
            // TBD try/except?
            sbuffer.write( server_fd );
            assert( sbuffer.empty() );
        } else {
            // POLLERR/POLLHUP/POLLNVAL err ret
            fmt::println( stderr, "{}: poll marked failure from {}",
                          __PRETTY_FUNCTION__,
                          pfds[0].revents & POLLERR ? "error condition" :
                          pfds[0].revents & POLLHUP ? "hangup" :
                          /*pfds[0].revents & POLLNVAL ?*/ "invalid (closed) fd" );
            goto close_socket;
        }
        break;
    }

    protocol::WINDOW screen0_root;
    pfds[0].events = POLLIN;
    switch ( poll(pfds, nfds, POLL_TIMEOUT_MS ) ) {
    case 0:
        // timeout err ret
        fmt::println( stderr, "{}: poll timeout on read", __PRETTY_FUNCTION__ );
        goto close_socket;
        break;
    case -1:
        // errno err ret
        break;
    default:
        if ( pfds[0].revents & POLLIN ) {
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
        } else {
            // POLLERR/POLLHUP/POLLNVAL err ret
            fmt::println( stderr, "{}: poll marked failure from {}",
                          __PRETTY_FUNCTION__,
                          pfds[0].revents & POLLERR ? "error condition" :
                          pfds[0].revents & POLLHUP ? "hangup" :
                          /*pfds[0].revents & POLLNVAL ?*/ "invalid (closed) fd" );
            goto close_socket;
        }
        break;
    }

    ////// Send ChangeWindowAttributes on screen->root to toggle reporting PropertNotify events

    pfds[0].events = POLLOUT;
    switch ( poll(pfds, nfds, POLL_TIMEOUT_MS ) ) {
    case 0:
        // timeout err ret
        fmt::println( stderr, "{}: poll timeout on write", __PRETTY_FUNCTION__ );
        goto close_socket;
        break;
    case -1:
        // errno err ret
        fmt::println( stderr, "{}: {}",
                      __PRETTY_FUNCTION__, ::errors::system::message( "poll" ) );
        goto close_socket;
        break;
    default:
        if ( pfds[0].revents & POLLOUT ) {
            using protocol::requests::ChangeWindowAttributes;
            ChangeWindowAttributes::Encoding encoding {};
            encoding.opcode = protocol::requests::opcodes::CHANGEWINDOWATTRIBUTES;
            encoding.request_length = _alignedUnits(
                sizeof( ChangeWindowAttributes::Encoding ) + sizeof( protocol::VALUE ) );
            encoding.window = screen0_root;
            encoding.value_mask = /*XCB_CW_EVENT_MASK*/ 1 << 11;
            sbuffer.load( &encoding, sizeof( encoding ) );
            const protocol::VALUE value_list[1] { /*XCB_EVENT_MASK_PROPERTY_CHANGE*/ 1 << 22 };
            sbuffer.load( value_list, sizeof( protocol::VALUE ) );
            assert( sbuffer.size() == sizeof( encoding ) + sizeof( protocol::VALUE ) );
            sbuffer.write( server_fd );
            assert( sbuffer.empty() );
        } else {
            // POLLERR/POLLHUP/POLLNVAL err ret
             fmt::println( stderr, "{}: poll marked failure from {}",
                          __PRETTY_FUNCTION__,
                          pfds[0].revents & POLLERR ? "error condition" :
                          pfds[0].revents & POLLHUP ? "hangup" :
                          /*pfds[0].revents & POLLNVAL ?*/ "invalid (closed) fd" );
            goto close_socket;
        }
        break;
    }

    ////// Send ChangeProperty with 0-length append  // TBD need noop that has reply or event with current time

    pfds[0].events = POLLOUT;
    switch ( poll(pfds, nfds, POLL_TIMEOUT_MS ) ) {
    case 0:
        // timeout err ret
        fmt::println( stderr, "{}: poll timeout on write", __PRETTY_FUNCTION__ );
        goto close_socket;
        break;
    case -1:
        // errno err ret
        fmt::println( stderr, "{}: {}",
                      __PRETTY_FUNCTION__, ::errors::system::message( "poll" ) );
        goto close_socket;
        break;
    default:
        if ( pfds[0].revents & POLLOUT ) {
            using protocol::requests::ChangeProperty;
            ChangeProperty::Encoding encoding {};
            encoding.opcode = protocol::requests::opcodes::CHANGEPROPERTY;
            // TBD currenly no enum constants in procotol::, only name arrays
            encoding.mode = /*XCB_PROP_MODE_APPEND 2*/0x02;
            encoding.request_length = _alignedUnits( sizeof( encoding ) );
            encoding.window = screen0_root;
            encoding.property.data = /*XCB_ATOM_WM_NAME 39*/0x27;
            encoding.type.data = /*XCB_ATOM_STRING 31*/0x1f;
            encoding.format = 8;  // (bits per fmt unit)
            encoding.fmt_unit_ct = 0;  // 0-length append to act as noop
            sbuffer.load( &encoding, sizeof( encoding ) );
            assert( sbuffer.size() == sizeof( encoding ) );
            sbuffer.write( server_fd );
            assert( sbuffer.empty() );
        } else {
            // POLLERR/POLLHUP/POLLNVAL err ret
            fmt::println( stderr, "{}: poll marked failure from {}",
                          __PRETTY_FUNCTION__,
                          pfds[0].revents & POLLERR ? "error condition" :
                          pfds[0].revents & POLLHUP ? "hangup" :
                          /*pfds[0].revents & POLLNVAL ?*/ "invalid (closed) fd" );
            goto close_socket;
        }
        break;
    }

    ////// verify properly formatted PropertyNotify reply, collect timestamp

    pfds[0].events = POLLIN;
    switch ( poll(pfds, nfds, POLL_TIMEOUT_MS ) ) {
    case 0:
        // timeout err ret
        fmt::println( stderr, "{}: poll timeout on read", __PRETTY_FUNCTION__ );
        goto close_socket;
        break;
    case -1:
        // errno err ret
        fmt::println( stderr, "{}: {}",
                      __PRETTY_FUNCTION__, ::errors::system::message( "poll" ) );
        goto close_socket;
        break;
    default:
        if ( pfds[0].revents & POLLIN ) {
            sbuffer.read( server_fd );
            using protocol::events::PropertyNotify;
            PropertyNotify::Encoding encoding;
            assert( sbuffer.size() == sizeof( PropertyNotify::Encoding ) );
            sbuffer.unload( &encoding, sizeof( encoding ) );
            assert( encoding.code == protocol::events::codes::PROPERTYNOTIFY );
            assert( encoding.sequence_number == 2 );  // event from second Request
            assert( encoding.window.data == screen0_root.data );
            assert( encoding.atom.data == /*XCB_ATOM_WM_NAME 39*/0x27 );
            assert( encoding.state == /*XCB_PROPERTY_NEW_VALUE*/0 );
            // assign reference points
            settings.ref_TIMESTAMP = encoding.time.data;
            settings.ref_unix_time = time(nullptr);
            sbuffer.clear();
        } else {
            // POLLERR/POLLHUP/POLLNVAL err ret
            fmt::println( stderr, "{}: poll marked failure from {}",
                          __PRETTY_FUNCTION__,
                          pfds[0].revents & POLLERR ? "error condition" :
                          pfds[0].revents & POLLHUP ? "hangup" :
                          /*pfds[0].revents & POLLNVAL ?*/ "invalid (closed) fd" );
            goto close_socket;
        }
        break;
    }

close_socket:
    // sends EOF
    close( server_fd );
}
