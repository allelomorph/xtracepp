#include <vector>

#include <cassert>

#include <poll.h>    // nfds_t pollfd POLL(IN|OUT|ERR|HUP|NVAL)
#include <unistd.h>  // STDERR_FILENO EXIT_FAILURE
#include <signal.h>  // sigaction
#include <stdio.h>   // write
#include <time.h>    // time

#include <fmt/format.h>

#include "ProxyX11Server.hpp"
#include "errors.hpp"

#include "protocol/version.hpp"
#include "protocol/connection_setup.hpp"
#include "protocol/requests.hpp"
#include "protocol/events.hpp"
#include "protocol/errors.hpp"


static void pollSingleSocket(
    const int socket_fd, const short events, int timeout = -1 ) {
    assert( events == POLLIN || events == POLLOUT ||
            events == ( POLLIN | POLLOUT ) );
    const      nfds_t   nfds { 1 };
    /*struct */pollfd   pfds[1];
    pfds[0].fd = socket_fd;
    static constexpr int DEFAULT_POLL_TIMEOUT_MS { 3000 };
    if ( timeout == -1 )
        timeout = DEFAULT_POLL_TIMEOUT_MS;

    pfds[0].events = events;
    switch ( ::poll( pfds, nfds, timeout ) ) {
    case 0:
        throw std::runtime_error(
            fmt::format( "{}: poll timeout in {} ms",
                         __PRETTY_FUNCTION__, timeout ) );
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

bool ProxyX11Server::_authenticateServerConnection(
    const int server_fd, protocol::WINDOW* screen0_root/* = nullptr*/ ) {

    SocketBuffer sbuffer;
    using protocol::connection_setup::Initiation;
    Initiation::Encoding init_encoding {};
    init_encoding.byte_order = Initiation::LSBFIRST;
    init_encoding.protocol_major_version = protocol::MAJOR_VERSION;
    init_encoding.protocol_minor_version = protocol::MINOR_VERSION;
    init_encoding.name_len = _AUTH_NAME.size();
    init_encoding.data_len = _AUTH_DATA_SZ;
    sbuffer.load( &init_encoding, sizeof(init_encoding) );
    // TBD padded sz will copy up to 3 junk bytes
    sbuffer.load( _AUTH_NAME.data(), _parser.alignment.pad( _AUTH_NAME.size() ) );
    sbuffer.load( _auth_data, _parser.alignment.pad( _AUTH_DATA_SZ ) );
    assert( sbuffer.size() ==
              sizeof(init_encoding) + _parser.alignment.pad( _AUTH_NAME.size() ) +
              _parser.alignment.pad( _AUTH_DATA_SZ ) );
    try {
        pollSingleSocket( server_fd, POLLOUT );
    } catch ( const std::exception& e ) {
        fmt::println( ::stderr, "{}: {}",
                      settings.process_name, e.what() );
        return false;
    }
    sbuffer.write( server_fd );
    assert( sbuffer.empty() );

    try {
        pollSingleSocket( server_fd, POLLIN );
    } catch ( const std::exception& e ) {
        fmt::println( ::stderr, "{}: {}",
                      settings.process_name, e.what() );
        return false;
    }
    sbuffer.read( server_fd );
    using protocol::connection_setup::Response;
    using protocol::connection_setup::Acceptance;
    assert( sbuffer.size() >= sizeof( Acceptance::Header ) );
    Acceptance::Header accept_header {};
    sbuffer.unload( &accept_header, sizeof( Acceptance::Header ) );
    if ( accept_header.success != Response::SUCCESS )
        return false;
    if ( accept_header.protocol_major_version !=
         init_encoding.protocol_major_version )
        return false;
    if ( accept_header.protocol_minor_version !=
         init_encoding.protocol_minor_version )
        return false;
    assert( sbuffer.size() == _parser.alignment.size(
                  accept_header.following_aligned_units ) );
    Acceptance::Encoding accept_encoding {};
    sbuffer.unload( &accept_encoding, sizeof( Acceptance::Encoding ) );
    // skip over vendor
    sbuffer.unload( _parser.alignment.pad( accept_encoding.vendor_len ) );
    // skip over pixmap-formats
    sbuffer.unload( accept_encoding.pixmap_formats_ct *
                    sizeof( Acceptance::FORMAT ) );
    // get WINDOW for root window of first screen
    assert( accept_encoding.roots_ct >= 1 );
    Acceptance::SCREEN::Encoding screen_encoding;
    sbuffer.unload( &screen_encoding, sizeof( Acceptance::SCREEN::Encoding ) );
    if ( screen0_root != nullptr )
        *screen0_root = screen_encoding.root;
    sbuffer.clear();
    return true;
}

void ProxyX11Server::_fetchCurrentServerTime() {
    const int server_fd { _connectToServer() };
    if( server_fd < 0 ) {
        // TBD exception
        fmt::println(
            ::stderr, "{}: {}: failure to connect to X server for display: {:?}",
            settings.process_name, __PRETTY_FUNCTION__, _out_display.name );
        return;
    }

    SocketBuffer sbuffer;
    protocol::WINDOW screen0_root;
    if ( !_authenticateServerConnection( server_fd, &screen0_root ) ) {
        fmt::println(
            ::stderr, "{}: {}: failed to authenticate connection to X server",
            settings.process_name, __PRETTY_FUNCTION__ );
        goto close_socket;
    }

    ////// Send ChangeWindowAttributes for root window on screen 0 to toggle
    //////   reporting PropertyNotify events
    {
        using protocol::requests::ChangeWindowAttributes;
        ChangeWindowAttributes::Header cwa_header {};
        cwa_header.opcode = protocol::requests::opcodes::CHANGEWINDOWATTRIBUTES;
        cwa_header.tl_aligned_units = _parser.alignment.units(
            ChangeWindowAttributes::BASE_ENCODING_SZ + sizeof( protocol::VALUE ) );
        sbuffer.load( &cwa_header, sizeof( cwa_header ) );
        ChangeWindowAttributes::Encoding cwa_encoding {};
        cwa_encoding.window = screen0_root;
        cwa_encoding.value_mask = /*XCB_CW_EVENT_MASK*/ 1 << 11;
        sbuffer.load( &cwa_encoding, sizeof( cwa_encoding ) );
        const protocol::VALUE value_list[1] {
            /*XCB_EVENT_MASK_PROPERTY_CHANGE*/ 1 << 22 };
        sbuffer.load( value_list, sizeof( value_list ) );
        assert( sbuffer.size() == ChangeWindowAttributes::BASE_ENCODING_SZ +
                  sizeof( protocol::VALUE ) );
        try {
            pollSingleSocket( server_fd, POLLOUT );
        } catch ( const std::exception& e ) {
            fmt::println( ::stderr, "{}: {}",
                          settings.process_name, e.what() );
            goto close_socket;
        }
        sbuffer.write( server_fd );
        assert( sbuffer.empty() );
    }

    ////// Send ChangeProperty with 0-length append as no-op
    // TBD see https://stackoverflow.com/questions/61849695/get-current-x11-server-time
    {
        using protocol::requests::ChangeProperty;
        ChangeProperty::Header cp_header {};
        cp_header.opcode = protocol::requests::opcodes::CHANGEPROPERTY;
        // TBD currenly no enum constants in procotol::, only name arrays
        cp_header.mode = /*XCB_PROP_MODE_APPEND 2*/0x02;
        cp_header.tl_aligned_units =
            _parser.alignment.units( ChangeProperty::BASE_ENCODING_SZ );
        sbuffer.load( &cp_header, sizeof( cp_header ) );
        ChangeProperty::Encoding cp_encoding {};
        cp_encoding.window = screen0_root;
        cp_encoding.property.data = /*XCB_ATOM_WM_NAME 39*/0x27;
        cp_encoding.type.data = /*XCB_ATOM_STRING 31*/0x1f;
        cp_encoding.format = 8;  // (bits per fmt unit)
        cp_encoding.data_fmt_unit_len = 0;  // 0-length append to act as noop
        sbuffer.load( &cp_encoding, sizeof( cp_encoding ) );
        assert( sbuffer.size() == ChangeProperty::BASE_ENCODING_SZ );
        try {
            pollSingleSocket( server_fd, POLLOUT );
        } catch ( const std::exception& e ) {
            fmt::println( ::stderr, "{}: {}",
                          settings.process_name, e.what() );
            goto close_socket;
        }
        sbuffer.write( server_fd );
        assert( sbuffer.empty() );
    }

    ////// Parse PropertyNotify event, collect reference TIMESTAMP
    {
        try {
            pollSingleSocket( server_fd, POLLIN );
        } catch ( const std::exception& e ) {
            fmt::println( ::stderr, "{}: {}",
                          settings.process_name, e.what() );
            goto close_socket;
        }
        sbuffer.read( server_fd );
        using protocol::events::PropertyNotify;
        PropertyNotify::Encoding pn_encoding;
        assert( sbuffer.size() == sizeof( PropertyNotify::Encoding ) );
        sbuffer.unload( &pn_encoding, sizeof( pn_encoding ) );
        assert( pn_encoding.header.code == protocol::events::codes::PROPERTYNOTIFY );
        assert( pn_encoding.header.sequence_num == 2 );  // event from second Request
        assert( pn_encoding.window.data == screen0_root.data );
        assert( pn_encoding.atom.data == /*XCB_ATOM_WM_NAME 39*/0x27 );
        assert( pn_encoding.state == /*XCB_PROPERTY_NEW_VALUE*/0 );

        settings.ref_TIMESTAMP = pn_encoding.time.data;
        settings.ref_unix_time = time(nullptr);
    }
    sbuffer.clear();

close_socket:
    // sends EOF
    close( server_fd );
}

static void handleTerminatingSignal( int sig ) {
    assert( sig == SIGINT || sig == SIGTERM ||
            sig == SIGABRT || sig == SIGSEGV );
    // show cursor if currently hidden
    ::write( STDERR_FILENO, "\x1b[?25h", sizeof("\x1b[?25h") );
    ::_exit( ProxyX11Server::SIGNAL_RETVAL_OFFSET + sig );
}

std::vector< std::string >
ProxyX11Server::_fetchInternedAtoms() {
    using protocol::requests::GetAtomName;

    const int server_fd { _connectToServer() };
    if( server_fd < 0 ) {
        // TBD exception
        fmt::println(
            ::stderr, "{}: {}: failure to connect to X server for display: {:?}",
            settings.process_name, __PRETTY_FUNCTION__, _out_display.name );
        ::exit( EXIT_FAILURE );
    }
    if ( !_authenticateServerConnection( server_fd ) ) {
        fmt::println(
            ::stderr, "{}: {}: failed to authenticate connection to X server",
            settings.process_name, __PRETTY_FUNCTION__ );
        ::close( server_fd );  // sends EOF
        ::exit( EXIT_FAILURE );
    }

    constexpr char CSI[ sizeof( "\x1b[" ) ] { "\x1b[" };
    // TBD standardize which stream all non-log messages are going to
    fmt::print( ::stderr, "fetching interned ATOMs: " );
    fmt::print( ::stderr, "{}?25l", CSI );  // hide cursor
    // ensure we unhide cursor if process exits unexpectedly ( unhide is idempotent )
    // `struct` needed to disambiguate from sigaction(2)
    struct ::sigaction act {};
    act.sa_handler = &handleTerminatingSignal;
    if ( ::sigaction( SIGABRT, &act, nullptr ) == -1 ||
         ::sigaction( SIGINT, &act, nullptr ) == -1  ||
         ::sigaction( SIGSEGV, &act, nullptr ) == -1 ||
         ::sigaction( SIGTERM, &act, nullptr ) == -1 ) {
        fmt::println(
            ::stderr, "{}: {}", __PRETTY_FUNCTION__,
            errors::system::message( "sigaction" ) );
        ::close( server_fd );  // sends EOF
        ::exit( EXIT_FAILURE );
    }

    GetAtomName::Header req_header {};
    req_header.opcode = protocol::requests::opcodes::GETATOMNAME;
    req_header.tl_aligned_units =
        _parser.alignment.units( GetAtomName::BASE_ENCODING_SZ );
    GetAtomName::Encoding req_encoding {};
    GetAtomName::Reply::Encoding rep_encoding {};
    std::vector< std::string > fetched_atoms ( 1 );  // indices start at 1
    static constexpr size_t STRINGBUF_SZ { 1000 };
    char stringbuf[ STRINGBUF_SZ ];
    SocketBuffer sbuffer;
    for ( uint32_t i { 1 }; true; ++i ) {
        ////// Send GetAtomName request on ATOMs starting with 1
        //////   ( expecting large contiguous region of ATOM ids )
        sbuffer.load( &req_header, sizeof( req_header ) );
        req_encoding.atom.data = i;
        sbuffer.load( &req_encoding, sizeof( req_encoding ) );
        assert( sbuffer.size() == GetAtomName::BASE_ENCODING_SZ );
        try {
            pollSingleSocket( server_fd, POLLOUT, 500 );
        } catch ( const std::exception& e ) {
            fmt::println( ::stderr, "{}", e.what() );
            ::close( server_fd );  // sends EOF
            ::exit( EXIT_FAILURE );
        }
        sbuffer.write( server_fd );

        ////// Parse GetAtomName reply to get string interned at ATOM i
        //////   ( or parse first error and break loop )
        assert( sbuffer.empty() );
        try {
            pollSingleSocket( server_fd, POLLIN, 500 );
        } catch ( const std::exception& e ) {
            fmt::println( ::stderr, "{}", e.what() );
            close( server_fd );  // sends EOF
            ::exit( EXIT_FAILURE );
        }
        sbuffer.read( server_fd );
        assert( sbuffer.size() >= sizeof( rep_encoding ) );
        if ( reinterpret_cast< protocol::Response::Header* >(
                 sbuffer.data() )->prefix == protocol::errors::Error::ERROR ) {
            assert( sbuffer.size() == protocol::errors::Error::ENCODING_SZ );
            protocol::errors::Error::Encoding err_encoding;
            sbuffer.unload( &err_encoding, sizeof( err_encoding ) );
            // expect Atom error at end of first contiguous region of server's ATOMs
            if ( err_encoding.header.code == protocol::errors::codes::ATOM ) {
                // need newline after cursor looping horizontally
                fmt::println( ::stderr, "" );
            } else {
                fmt::println( ::stderr, "failed atom prefech with X error {}, "
                              "reverting to default atom lookup",
                              protocol::errors::names[ err_encoding.header.code ] );
                fetched_atoms.clear();
            }
            break;
        }
        sbuffer.unload( &rep_encoding, sizeof( rep_encoding ) );
        assert( rep_encoding.header.reply == protocol::requests::Reply::REPLY );
        assert( rep_encoding.header.sequence_num == i );
        assert( rep_encoding.header.extra_aligned_units ==
                  _parser.alignment.units( _parser.alignment.pad( rep_encoding.name_len ) ) );
        assert( sbuffer.size() < STRINGBUF_SZ );
        assert( sbuffer.size() == _parser.alignment.pad( rep_encoding.name_len ) );
        sbuffer.unload( stringbuf, sbuffer.size() );
        stringbuf[ rep_encoding.name_len ] = '\0';
        fetched_atoms.emplace_back( stringbuf );
        assert( sbuffer.empty() );

        // Update ATOM counter in place to keep user aware of progress
        static constexpr int COUNTER_W { 5 };
        fmt::print( ::stderr, "{:{}d}{}{}D",  // \x1b[#D right # cols
                    i, COUNTER_W, CSI, COUNTER_W );
    }

    fmt::print( ::stderr, "{}?25h", CSI );  // show cursor
    // restore default signal behavior
    act.sa_handler = SIG_DFL;
    if ( ::sigaction( SIGABRT, &act, nullptr ) == -1 ||
         ::sigaction( SIGINT, &act, nullptr ) == -1  ||
         ::sigaction( SIGSEGV, &act, nullptr ) == -1 ||
         ::sigaction( SIGTERM, &act, nullptr ) == -1 ) {
        fmt::println(
            ::stderr, "{}: {}: {}", settings.process_name, __PRETTY_FUNCTION__,
            errors::system::message( "sigaction" ) );
        ::close( server_fd );  // sends EOF
        ::exit( EXIT_FAILURE );
    }

    return fetched_atoms;
}
