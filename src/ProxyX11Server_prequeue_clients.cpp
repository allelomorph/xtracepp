#include <vector>

#include <cassert>

#include <poll.h>    // nfds_t pollfd POLL(IN|OUT|ERR|HUP|NVAL)
#include <unistd.h>  // STDERR_FILENO EXIT_FAILURE
#include <signal.h>  // sigaction
#include <stdio.h>   // write

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
            events == POLLIN | POLLOUT );
    const      nfds_t   nfds { 1 };
    /*struct */pollfd   pfds[1];
    pfds[0].fd = socket_fd;
    static constexpr int DEFAULT_POLL_TIMEOUT_MS { 3000 };
    if ( timeout == -1 )
        timeout = DEFAULT_POLL_TIMEOUT_MS;

    pfds[0].events = events;
    switch ( poll( pfds, nfds, timeout ) ) {
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
    using protocol::connection_setup::ClientInitiation;
    ClientInitiation::Header init_header;
    init_header.byte_order = ClientInitiation::Header::LSB_FIRST;
    init_header.protocol_major_version = protocol::MAJOR_VERSION;
    init_header.protocol_minor_version = protocol::MINOR_VERSION;
    init_header.n = _AUTH_NAME.size();
    init_header.d = _AUTH_DATA_SZ;
    sbuffer.load( &init_header, sizeof(init_header) );
    // TBD padded sz will copy up to 3 junk bytes
    sbuffer.load( _AUTH_NAME.data(), _parser._pad( _AUTH_NAME.size() ) );
    sbuffer.load( _auth_data, _parser._pad( _AUTH_DATA_SZ ) );
    assert( sbuffer.size() ==
        sizeof(init_header) + _parser._pad( _AUTH_NAME.size() ) + _parser._pad( _AUTH_DATA_SZ ) );
    try {
        pollSingleSocket( server_fd, POLLOUT );
    } catch ( const std::exception& e ) {
        fmt::println( stderr, "{}", e.what() );
        return false;
    }
    sbuffer.write( server_fd );
    assert( sbuffer.empty() );

    try {
        pollSingleSocket( server_fd, POLLIN );
    } catch ( const std::exception& e ) {
        fmt::println( stderr, "{}", e.what() );
        return false;
    }
    sbuffer.read( server_fd );
    using protocol::connection_setup::ServerAcceptance;
    assert( sbuffer.size() >= sizeof( ServerAcceptance::Header ) );
    ServerAcceptance::Header acceptance_header;
    sbuffer.unload( &acceptance_header, sizeof( ServerAcceptance::Header ) );
    assert( sbuffer.size() >= _parser._pad( acceptance_header.v ) +
            acceptance_header.n * sizeof( ServerAcceptance::FORMAT ) +
            sizeof( ServerAcceptance::SCREEN::Header ) );
    if ( acceptance_header.success != protocol::connection_setup::SUCCESS )
        return false;
    if ( acceptance_header.protocol_major_version !=
         init_header.protocol_major_version )
        return false;
    if( acceptance_header.protocol_minor_version !=
        init_header.protocol_minor_version )
        return false;
    // TBD skip over vendor
    sbuffer.unload( _parser._pad( acceptance_header.v ) );
    // TBD skip over pixmap-formats
    sbuffer.unload( acceptance_header.n * sizeof( ServerAcceptance::FORMAT ) );
    // TBD get WINDOW for root window of first screen
    ServerAcceptance::SCREEN::Header screen_header;
    sbuffer.unload( &screen_header, sizeof( ServerAcceptance::SCREEN::Header ) );
    if ( screen0_root != nullptr )
        *screen0_root = screen_header.root;
    sbuffer.clear();
    return true;
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
    protocol::WINDOW screen0_root;
    if ( !_authenticateServerConnection( server_fd, &screen0_root ) ) {
        fmt::println( stderr, "{}: failed to authenticate connection to X server",
                      __PRETTY_FUNCTION__ );
        goto close_socket;
    }

    ////// Send ChangeWindowAttributes for root window on screen 0 to toggle
    //////   reporting PropertNotify events
    {
        using protocol::requests::ChangeWindowAttributes;
        ChangeWindowAttributes::Encoding cwa_encoding {};
        cwa_encoding.opcode = protocol::requests::opcodes::CHANGEWINDOWATTRIBUTES;
        cwa_encoding.request_length = _parser._alignedUnits(
            sizeof( ChangeWindowAttributes::Encoding ) + sizeof( protocol::VALUE ) );
        cwa_encoding.window = screen0_root;
        cwa_encoding.value_mask = /*XCB_CW_EVENT_MASK*/ 1 << 11;
        sbuffer.load( &cwa_encoding, sizeof( cwa_encoding ) );
        const protocol::VALUE value_list[1] { /*XCB_EVENT_MASK_PROPERTY_CHANGE*/ 1 << 22 };
        sbuffer.load( value_list, sizeof( protocol::VALUE ) );
        assert( sbuffer.size() == sizeof( cwa_encoding ) + sizeof( protocol::VALUE ) );
        try {
            pollSingleSocket( server_fd, POLLOUT );
        } catch ( const std::exception& e ) {
            fmt::println( stderr, "{}", e.what() );
            goto close_socket;
        }
        sbuffer.write( server_fd );
        assert( sbuffer.empty() );
    }

    ////// Send ChangeProperty with 0-length append as no-op
    // TBD see https://stackoverflow.com/questions/61849695/get-current-x11-server-time
    {
        using protocol::requests::ChangeProperty;
        ChangeProperty::Encoding cp_encoding {};
        cp_encoding.opcode = protocol::requests::opcodes::CHANGEPROPERTY;
        // TBD currenly no enum constants in procotol::, only name arrays
        cp_encoding.mode = /*XCB_PROP_MODE_APPEND 2*/0x02;
        cp_encoding.request_length = _parser._alignedUnits( sizeof( cp_encoding ) );
        cp_encoding.window = screen0_root;
        cp_encoding.property.data = /*XCB_ATOM_WM_NAME 39*/0x27;
        cp_encoding.type.data = /*XCB_ATOM_STRING 31*/0x1f;
        cp_encoding.format = 8;  // (bits per fmt unit)
        cp_encoding.fmt_unit_ct = 0;  // 0-length append to act as noop
        sbuffer.load( &cp_encoding, sizeof( cp_encoding ) );
        assert( sbuffer.size() == sizeof( cp_encoding ) );
        try {
            pollSingleSocket( server_fd, POLLOUT );
        } catch ( const std::exception& e ) {
            fmt::println( stderr, "{}", e.what() );
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

        settings.ref_TIMESTAMP = pn_encoding.time.data;
        settings.ref_unix_time = time(nullptr);
    }
    sbuffer.clear();

close_socket:
    // sends EOF
    close( server_fd );
}

void catchProcessEndingSignal( int signum ) {
    write( STDERR_FILENO, "\x1b[?25h", sizeof("\x1b[?25h") );
    _exit( signum );
}

void ProxyX11Server::_fetchInternedAtoms() {
    const int server_fd { _connectToServer() };
    if( server_fd < 0 ) {
        // TBD exception
        fmt::println(
            stderr, "{}: failure to connect to X server for display: {}",
            __PRETTY_FUNCTION__, _out_display.name );
        return;
    }

    // TBD put most initializations first to avoid being crossed by goto
    // indices start at 1
    std::vector<std::string> fetched_atoms ( 1 );
    using protocol::requests::GetAtomName;
    GetAtomName::Encoding req_encoding {};
    GetAtomName::ReplyEncoding rep_encoding {};
    constexpr int STRINGBUF_SZ { 1000 };
    char stringbuf[STRINGBUF_SZ];
    constexpr int readout_int_width { 5 };
    constexpr char CSI[sizeof("\x1b[")] { "\x1b[" };
    SocketBuffer sbuffer;
    struct sigaction act {};

    if ( !_authenticateServerConnection( server_fd ) ) {
        fmt::println( stderr, "{}: failed to authenticate connection to X server",
                      __PRETTY_FUNCTION__ );
        goto close_socket;
    }
    req_encoding.opcode = protocol::requests::opcodes::GETATOMNAME;
    req_encoding.request_length = _parser._alignedUnits( sizeof( req_encoding ) );
    // TBD standardize which stream all non-log messages are going to
    fmt::print( stderr, "fetching interned ATOMs: " );
    fmt::print( stderr, "{}?25l", CSI );  // hide cursor
    // ensure we unhide cursor if process exits unexpectedly ( unhide is idempotent )
    act.sa_handler = &catchProcessEndingSignal;
    if ( sigaction( SIGABRT, &act, nullptr ) == -1 ||
         sigaction( SIGINT, &act, nullptr ) == -1  ||
         sigaction( SIGSEGV, &act, nullptr ) == -1 ||
         sigaction( SIGTERM, &act, nullptr ) == -1 ) {
        fmt::println(
            stderr, "{}: {}", __PRETTY_FUNCTION__,
            errors::system::message( "sigaction" ) );
        exit( EXIT_FAILURE );
    }
    for ( uint32_t i { 1 }; true; ++i ) {
        ////// Send InternAtom request on ATOMs starting with 1
        //////   ( expecting large contiguous region of ATOM ids )
        req_encoding.atom.data = i;
        sbuffer.load( &req_encoding, sizeof( req_encoding ) );
        assert( sbuffer.size() == sizeof( req_encoding ) );
        try {
            pollSingleSocket( server_fd, POLLOUT, 500 );
        } catch ( const std::exception& e ) {
            fmt::println( stderr, "{}", e.what() );
            goto close_socket;
        }
        sbuffer.write( server_fd );

        ////// Parse InternAtom reply to get string interned at ATOM i
        //////   ( or parse first error and break loop )
        assert( sbuffer.empty() );
        try {
            pollSingleSocket( server_fd, POLLIN, 500 );
        } catch ( const std::exception& e ) {
            fmt::println( stderr, "{}", e.what() );
            goto close_socket;
        }
        sbuffer.read( server_fd );
        assert( sbuffer.size() >= sizeof( rep_encoding ) );
        if ( *sbuffer.data() == protocol::errors::PREFIX ) {
            protocol::errors::Encoding err_encoding;
            assert( sbuffer.size() == sizeof( err_encoding ) );
            sbuffer.unload( &err_encoding, sizeof( err_encoding ) );
            // expect Atom error at end of first contiguous region of server's ATOMs
            if ( err_encoding.code == protocol::errors::codes::ATOM ) {
                // need newline after cursor looping horizontally
                fmt::println( stderr, "" );
            } else {
                fmt::println( stderr, "failed atom prefech with X error {}, "
                              "reverting to default atom lookup",
                              protocol::errors::names[err_encoding.code] );
                fetched_atoms.clear();
            }
            break;
        }
        sbuffer.unload( &rep_encoding, sizeof( rep_encoding ) );
        assert( rep_encoding.reply == protocol::requests::REPLY_PREFIX );
        assert( rep_encoding.sequence_number == i );
        assert( rep_encoding.reply_length == _parser._alignedUnits( _parser._pad( rep_encoding.n ) ) );
        assert( sbuffer.size() < STRINGBUF_SZ );
        assert( sbuffer.size() == _parser._pad( rep_encoding.n ) );
        sbuffer.unload( stringbuf, sbuffer.size() );
        stringbuf[rep_encoding.n] = '\0';
        fetched_atoms.emplace_back( stringbuf );
        assert( sbuffer.empty() );

        // Update ATOM counter in place to keep user aware of progress
        // \x1b[#D right # cols
        fmt::print( stderr, "{:{}d}{}{}D",
                    i, readout_int_width, CSI, readout_int_width );
    }

    fmt::print( stderr, "{}?25h", CSI );  // show cursor
    // restore default signal behavior
    act.sa_handler = SIG_DFL;
    if ( sigaction( SIGABRT, &act, nullptr ) == -1 ||
         sigaction( SIGINT, &act, nullptr ) == -1  ||
         sigaction( SIGSEGV, &act, nullptr ) == -1 ||
         sigaction( SIGTERM, &act, nullptr ) == -1 ) {
        fmt::println(
            stderr, "{}: {}", __PRETTY_FUNCTION__,
            errors::system::message( "sigaction" ) );
        exit( EXIT_FAILURE );
    }
    if ( fetched_atoms.size() > 1 ) {
        _parser._seq_interned_atoms = std::move( fetched_atoms );
    }

close_socket:
    // sends EOF
    close( server_fd );
}
