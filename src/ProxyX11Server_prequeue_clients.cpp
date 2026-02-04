#include <vector>
#include <optional>
#include <string>

#include <cassert>

#include <poll.h>    // nfds_t pollfd POLL(IN|OUT|ERR|HUP|NVAL)
#include <unistd.h>  // STDERR_FILENO EXIT_FAILURE
#include <signal.h>  // sigaction
#include <stdio.h>   // write
#include <time.h>    // time time_t

#include <fmt/format.h>

#include "ProxyX11Server.hpp"
#include "errors.hpp"

#include "protocol/version.hpp"
#include "protocol/connection_setup.hpp"
#include "protocol/requests.hpp"
#include "protocol/events.hpp"
#include "protocol/errors.hpp"


static std::optional< std::string >
pollSingleSocket(
    const int socket_fd, const short events, int timeout = -1 ) {
    assert( events == POLLIN || events == POLLOUT ||
            events == ( POLLIN | POLLOUT ) );
    ::pollfd pfd {};
    pfd.fd = socket_fd;
    pfd.events = events;
    static constexpr int DEFAULT_POLL_TIMEOUT_MS { 3000 };
    if ( timeout == -1 )
        timeout = DEFAULT_POLL_TIMEOUT_MS;
    switch ( ::poll( &pfd, 1, timeout ) ) {
    case -1:
        return errors::system::message(
            fmt::format( "{}: poll", __PRETTY_FUNCTION__ ) );
        break;
    case 0:
        return fmt::format( "{}: poll timeout in {} ms",
                            __PRETTY_FUNCTION__, timeout );
        break;
    default: {
        const std::string_view err_msg {
            ( pfd.revents & POLLERR )  ? "error condition" :
         // ( pfd.revents & POLLHUP )  ? "hang up" :
            ( pfd.revents & POLLNVAL ) ? "invalid fd (not open)" :
            ( pfd.revents & POLLPRI )  ? "exceptional condition" :
                                         "" };
        if ( !err_msg.empty() )
            return fmt::format( "{}: poll marked failure from {}",
                                __PRETTY_FUNCTION__, err_msg );
    }
        break;
    }
    return std::nullopt;
}

static std::pair< size_t, std::optional< std::string > >
polledWriteMessage( SocketBuffer& buffer, const int fd,
                    const size_t message_sz, const int timeout = -1 ) {
    assert( buffer.size() == message_sz );
    assert( buffer.parsed() == message_sz );
    if ( const auto poll_error { pollSingleSocket( fd, POLLOUT, timeout ) };
         poll_error ) {
        return { 0, poll_error };
    }
    const auto [ bytes_written, write_error ] {
        buffer.write( fd, message_sz ) };
    if ( write_error ) {
        return { bytes_written, write_error };
    }
    assert( buffer.empty() );
    return { bytes_written, std::nullopt };
}

static std::pair< size_t, std::optional< std::string > >
polledRead( SocketBuffer& buffer, const int fd, const int timeout = -1 ) {
    assert( buffer.empty() );
    if ( const auto poll_error { pollSingleSocket( fd, POLLIN, timeout ) };
         poll_error ) {
        return { 0, poll_error };
    }
    return buffer.read( fd );
}

static std::pair< size_t, std::optional< std::string > >
polledReadMessage( SocketBuffer& buffer, const int fd,
                   const size_t message_sz, const int timeout = -1 ) {
    if ( !buffer.messageSizeSet() )
        buffer.setMessageSize( message_sz );
    assert( buffer.messageSize() == message_sz );
    const size_t initial_sz { buffer.size() };
    size_t tl_bytes_read {};
    while ( buffer.size() < buffer.messageSize() ) {
        if ( const auto poll_error { pollSingleSocket( fd, POLLIN, timeout ) };
             poll_error ) {
            return { tl_bytes_read, poll_error };
        }
        const auto [ bytes_read, read_error ] { buffer.read( fd, message_sz ) };
        if ( read_error )
            return { tl_bytes_read, read_error };
        tl_bytes_read += bytes_read;
    }
    assert( initial_sz + tl_bytes_read == buffer.messageSize() );
    return { tl_bytes_read, std::nullopt };
}

bool ProxyX11Server::_authenticateServerConnection(
    const int server_fd, protocol::WINDOW* screen0_root/* = nullptr*/ ) {
    SocketBuffer buffer;

    ////// send Initiation
    using protocol::connection_setup::Initiation;
    Initiation::Header init_header {};
    init_header.byte_order = _parser.little_endian ?
        Initiation::LSBFIRST : Initiation::MSBFIRST;
    init_header.protocol_major_version = protocol::MAJOR_VERSION;
    init_header.protocol_minor_version = protocol::MINOR_VERSION;
    init_header.name_len = _AUTH_NAME.size();
    init_header.data_len = _AUTH_DATA_SZ;
    buffer.load( &init_header, sizeof(init_header) );
    // note: padded sz will copy up to 3 junk bytes
    buffer.load( _AUTH_NAME.data(), _parser.alignment.pad( _AUTH_NAME.size() ) );
    buffer.load( _auth_data, _parser.alignment.pad( _AUTH_DATA_SZ ) );
    const size_t bytes_loaded {
        sizeof(init_header) + _parser.alignment.pad( _AUTH_NAME.size() ) +
        _parser.alignment.pad( _AUTH_DATA_SZ ) };
    assert( buffer.size() == bytes_loaded );
    buffer.setMessageSize( bytes_loaded );
    buffer.markMessageParsed();
    const auto [ bytes_written, write_error ] {
        polledWriteMessage( buffer, server_fd, bytes_loaded ) };
    if ( write_error ) {
        fmt::println( ::stderr, "{}: {}", settings.process_name, *write_error );
        return false;
    }

    ////// recv Acceptance
    const auto [ first_bytes_read, first_read_error ] {
        polledRead( buffer, server_fd ) };
    if ( first_read_error ) {
        fmt::println( ::stderr, "{}: {}", settings.process_name, *first_read_error );
        return false;
    }
    using protocol::connection_setup::Acceptance;
    assert( first_bytes_read >= sizeof( Acceptance::Header ) );
    const Acceptance::Header* accept_header {
        reinterpret_cast< const Acceptance::Header* >( buffer.data() ) };
    using protocol::connection_setup::InitResponse;
    if ( accept_header->success != InitResponse::SUCCESS )
        return false;
    if ( accept_header->protocol_major_version !=
         init_header.protocol_major_version )
        return false;
    if ( accept_header->protocol_minor_version !=
         init_header.protocol_minor_version )
        return false;
    const size_t acceptance_sz {
        sizeof( Acceptance::Header ) +
        _parser.alignment.size( accept_header->following_aligned_units ) };
    buffer.setMessageSize( acceptance_sz );
    const auto [ remaining_bytes_read, remaining_read_error ] {
        polledReadMessage( buffer, server_fd, acceptance_sz ) };
    if ( remaining_read_error ) {
        fmt::println( ::stderr, "{}: {}", settings.process_name, *remaining_read_error );
        return false;
    }
    assert( first_bytes_read + remaining_bytes_read == acceptance_sz );

    ////// parse Acceptance
    // skip header
    buffer.unload( sizeof( Acceptance::Header ) );
    Acceptance::Encoding accept_encoding {};
    buffer.unload( &accept_encoding, sizeof( Acceptance::Encoding ) );
    // skip suffix `vendor`
    buffer.unload( _parser.alignment.pad( accept_encoding.vendor_len ) );
    // skip suffix `pixmap-formats`
    buffer.unload( accept_encoding.pixmap_formats_ct *
                   sizeof( Acceptance::FORMAT ) );
    // in suffix `roots`, get WINDOW `root` from first SCREEN
    assert( accept_encoding.roots_ct >= 1 );
    Acceptance::SCREEN::Header screen_header;
    buffer.unload( &screen_header, sizeof( Acceptance::SCREEN::Header ) );
    if ( screen0_root != nullptr )
        *screen0_root = screen_header.root;
    return true;
}

void ProxyX11Server::_fetchCurrentServerTime() {
    const int server_fd { _connectToServer() };
    if( server_fd < 0 ) {
        fmt::println(
            ::stderr, "{}: {}: failure to connect to X server for display: {:?}",
            settings.process_name, __PRETTY_FUNCTION__, _out_display.name );
        return;
    }
    SocketBuffer buffer;
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
        ChangeWindowAttributes::Prefix prefix {};
        prefix.opcode = protocol::requests::opcodes::CHANGEWINDOWATTRIBUTES;
        buffer.load( &prefix, sizeof( prefix ) );
        ChangeWindowAttributes::Length length {};
        length.tl_aligned_units = _parser.alignment.units(
            ChangeWindowAttributes::BASE_ENCODING_SZ + sizeof( protocol::VALUE ) );
        buffer.load( &length, sizeof( length ) );
        ChangeWindowAttributes::Encoding encoding {};
        encoding.window = screen0_root;
        encoding.value_mask = /*XCB_CW_EVENT_MASK*/ 1 << 11;
        buffer.load( &encoding, sizeof( encoding ) );
        const protocol::VALUE value_list[1] {
            /*XCB_EVENT_MASK_PROPERTY_CHANGE*/ 1 << 22 };
        buffer.load( value_list, sizeof( value_list ) );
        const size_t bytes_loaded {
            ChangeWindowAttributes::BASE_ENCODING_SZ + sizeof( value_list ) };
        assert( buffer.size() == bytes_loaded );
        buffer.setMessageSize( bytes_loaded );
        buffer.markMessageParsed();
        const auto [ bytes_written, write_error ] {
            polledWriteMessage( buffer, server_fd, bytes_loaded ) };
        if ( write_error ) {
            fmt::println( ::stderr, "{}: {}", settings.process_name, *write_error );
            goto close_socket;
        }
    }
    ////// Send ChangeProperty with 0-length append as no-op
    // see: - https://tronche.com/gui/x/icccm/sec-2.html#s-2.1 ("Convention")
    //      - https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#events:PropertyNotify
    {
        using protocol::requests::ChangeProperty;
        ChangeProperty::Prefix prefix {};
        prefix.opcode = protocol::requests::opcodes::CHANGEPROPERTY;
        prefix.mode = /*XCB_PROP_MODE_APPEND 2*/0x02;
        buffer.load( &prefix, sizeof( prefix ) );
        ChangeProperty::Length length {};
        length.tl_aligned_units =
            _parser.alignment.units( ChangeProperty::BASE_ENCODING_SZ );
        buffer.load( &length, sizeof( length ) );
        ChangeProperty::Encoding encoding {};
        encoding.window = screen0_root;
        encoding.property.data = /*XCB_ATOM_WM_NAME 39*/0x27;
        encoding.type.data = /*XCB_ATOM_STRING 31*/0x1f;
        encoding.format = 8;  // (bits per fmt unit)
        encoding.data_fmt_unit_len = 0;  // 0-length append to act as noop
        buffer.load( &encoding, sizeof( encoding ) );
        const size_t bytes_loaded { ChangeProperty::BASE_ENCODING_SZ };
        assert( buffer.size() == bytes_loaded );
        buffer.setMessageSize( bytes_loaded );
        buffer.markMessageParsed();
        // fmt::println( ::stderr, "_fetchCurrentServerTime() ChangeProperty:\n{}",
        //               bufferHexDump( buffer.data(), buffer.size() ) );
        const auto [ bytes_written, write_error ] {
            polledWriteMessage( buffer, server_fd, bytes_loaded ) };
        if ( write_error ) {
            fmt::println( ::stderr, "{}: {}", settings.process_name, *write_error );
            goto close_socket;
        }
    }

    ////// Parse PropertyNotify event, collect reference TIMESTAMP
    {
        using protocol::events::PropertyNotify;
        buffer.setMessageSize( PropertyNotify::ENCODING_SZ );
        const auto [ bytes_read, read_error ] {
            polledReadMessage( buffer, server_fd, PropertyNotify::ENCODING_SZ ) };
        if ( read_error ) {
            fmt::println( ::stderr, "{}: {}", settings.process_name, *read_error );
            goto close_socket;
        }
        assert( bytes_read == PropertyNotify::ENCODING_SZ );
        PropertyNotify::Encoding pn_encoding;
        buffer.unload( &pn_encoding, sizeof( pn_encoding ) );
        assert( pn_encoding.header.code == protocol::events::codes::PROPERTYNOTIFY );
        assert( pn_encoding.header.sequence_num == 2 );  // event from second Request
        assert( pn_encoding.window.data == screen0_root.data );
        assert( pn_encoding.atom.data == /*XCB_ATOM_WM_NAME 39*/0x27 );
        assert( pn_encoding.state == /*XCB_PROPERTY_NEW_VALUE*/0 );

        settings.ref_TIMESTAMP = pn_encoding.time.data;
        settings.ref_unix_time = ::time( nullptr );
    }

close_socket:
    // sends EOF
    close( server_fd );
}

static void handleTerminatingSignal( int sig ) {
    assert( sig == SIGINT  || sig == SIGTERM ||
            sig == SIGABRT || sig == SIGSEGV );
    // show cursor if currently hidden
    ::write( STDERR_FILENO, "\x1b[?25h", sizeof("\x1b[?25h") );
    ::_exit( ProxyX11Server::SIGNAL_RETVAL_OFFSET + sig );
}

std::vector< std::string >
ProxyX11Server::_fetchInternedAtoms() {
    const int server_fd { _connectToServer() };
    if( server_fd < 0 ) {
        fmt::println(
            ::stderr, "{}: {}: failure to connect to X server for display: {:?}",
            settings.process_name, __PRETTY_FUNCTION__, _out_display.name );
        ::exit( EXIT_FAILURE );
    }
    std::vector< std::string > fetched_atoms ( 1 );  // indices start at 1
    if ( !_authenticateServerConnection( server_fd ) ) {
        fmt::println(
            ::stderr, "{}: {}: failed to authenticate connection to X server",
            settings.process_name, __PRETTY_FUNCTION__ );
        goto failure;
    }
    {
        ////// prepare for loop
        constexpr char CSI[ sizeof( "\x1b[" ) ] { "\x1b[" };
        fmt::print( ::stderr, "fetching interned ATOMs: " );
        fmt::print( ::stderr, "{}?25l", CSI );  // hide cursor
        // ensure we unhide cursor if process exits unexpectedly ( unhide is idempotent )
        struct ::sigaction act {};  // `struct` needed to disambiguate from sigaction(2)
        act.sa_handler = &handleTerminatingSignal;
        if ( ::sigaction( SIGABRT, &act, nullptr ) == -1 ||
             ::sigaction( SIGINT, &act, nullptr ) == -1  ||
             ::sigaction( SIGSEGV, &act, nullptr ) == -1 ||
             ::sigaction( SIGTERM, &act, nullptr ) == -1 ) {
            fmt::println( ::stderr, "{}: {}: {}",
                          settings.process_name, __PRETTY_FUNCTION__,
                          errors::system::message( "sigaction" ) );
            goto failure;
        }
        ////// loop GetAtomName request for first region of contiguous ATOM ids
        //////   starting at 1
        using protocol::requests::GetAtomName;
        GetAtomName::Prefix req_prefix {};
        req_prefix.opcode = protocol::requests::opcodes::GETATOMNAME;
        GetAtomName::Length req_length {};
        req_length.tl_aligned_units =
            _parser.alignment.units( GetAtomName::BASE_ENCODING_SZ );
        GetAtomName::Encoding req_encoding {};
        GetAtomName::Reply::Encoding rep_encoding {};
        static constexpr size_t STRINGBUF_SZ { 1000 };
        char stringbuf[ STRINGBUF_SZ ];
        SocketBuffer buffer;
        for ( uint32_t atom_i { 1 }; true; ++atom_i ) {
            ////// send GetAtomName request
            buffer.load( &req_prefix, sizeof( req_prefix ) );
            buffer.load( &req_length, sizeof( req_length ) );
            req_encoding.atom.data = atom_i;
            buffer.load( &req_encoding, sizeof( req_encoding ) );
            const size_t bytes_loaded { GetAtomName::BASE_ENCODING_SZ };
            assert( buffer.size() == bytes_loaded );
            buffer.setMessageSize( bytes_loaded );
            buffer.markMessageParsed();
            const auto [ bytes_written, write_error ] {
                polledWriteMessage( buffer, server_fd, bytes_loaded ) };
            if ( write_error ) {
                fmt::println( ::stderr, "{}: {}",
                              settings.process_name, *write_error );
                goto failure;
            }
            ////// parse GetAtomName reply to get string interned at ATOM id
            //////   (first protocol error ends loop)
            const auto [ first_bytes_read, first_read_error ] {
                polledRead( buffer, server_fd ) };
            if ( first_read_error ) {
                fmt::println( ::stderr, "{}: {}",
                              settings.process_name, *first_read_error );
                goto failure;
            }
            assert( first_bytes_read >= sizeof( protocol::Response::Header ) );
            size_t response_sz {};
            bool protocol_error {};
            using protocol::Response;
            if ( const Response::Header* header {
                    reinterpret_cast< const Response::Header* >( buffer.data() ) };
                header->prefix == protocol::errors::Error::ERROR ) {
                response_sz = protocol::errors::Error::ENCODING_SZ;
                protocol_error = true;
            } else {
                assert( header->prefix == protocol::requests::Reply::REPLY );
                assert( buffer.size() >= sizeof( GetAtomName::Reply::Header ) );
                const GetAtomName::Reply::Header* rep_header {
                    reinterpret_cast< const GetAtomName::Reply::Header* >(
                        buffer.data() ) };
                response_sz = GetAtomName::Reply::DEFAULT_ENCODING_SZ +
                    _parser.alignment.size( rep_header->extra_aligned_units );
            }
            buffer.setMessageSize( response_sz );
            const auto [ remaining_bytes_read, remaining_read_error ] {
                polledReadMessage( buffer, server_fd, response_sz ) };
            if ( remaining_read_error ) {
                fmt::println( ::stderr, "{}: {}",
                              settings.process_name, *remaining_read_error );
                goto failure;
            }
            assert( first_bytes_read + remaining_bytes_read == response_sz );
            if ( protocol_error ) {
                protocol::errors::Error::Encoding err_encoding;
                buffer.unload( &err_encoding, sizeof( err_encoding ) );
                assert( buffer.empty() );
                // expect Atom error at end of first contiguous region of server's ATOMs
                if ( err_encoding.header.code != protocol::errors::codes::ATOM ) {
                    fmt::println( ::stderr, "{}: failed atom prefech with X error {}, "
                                  "reverting to default atom lookup",
                                  settings.process_name,
                                  protocol::errors::names[ err_encoding.header.code ] );
                    fetched_atoms.clear();
                }
                break;
            }
            buffer.unload( &rep_encoding, sizeof( rep_encoding ) );
            assert( rep_encoding.header.reply == protocol::requests::Reply::REPLY );
            assert( rep_encoding.header.sequence_num == atom_i );
            assert( rep_encoding.header.extra_aligned_units ==
                    _parser.alignment.units( _parser.alignment.pad( rep_encoding.name_len ) ) );
            assert( buffer.size() < STRINGBUF_SZ );
            assert( buffer.size() == _parser.alignment.pad( rep_encoding.name_len ) );
            buffer.unload( stringbuf, buffer.size() );
            stringbuf[ rep_encoding.name_len ] = '\0';
            fetched_atoms.emplace_back( stringbuf );
            assert( buffer.empty() );
            // Update ATOM counter in place to keep user aware of progress
            static constexpr int COUNTER_W { 5 };
            fmt::print( ::stderr, "{:{}d}{}{}D",  // \x1b[#D right # cols
                        atom_i, COUNTER_W, CSI, COUNTER_W );
        }
        ////// loop cleanup
        // need newline after cursor looping horizontally
        fmt::println( ::stderr, "" );
        // show cursor
        fmt::print( ::stderr, "{}?25h", CSI );
        // restore default signal behavior
        act.sa_handler = SIG_DFL;
        if ( ::sigaction( SIGABRT, &act, nullptr ) == -1 ||
             ::sigaction( SIGINT, &act, nullptr ) == -1  ||
             ::sigaction( SIGSEGV, &act, nullptr ) == -1 ||
             ::sigaction( SIGTERM, &act, nullptr ) == -1 ) {
            fmt::println(
                ::stderr, "{}: {}: {}",
                settings.process_name, __PRETTY_FUNCTION__,
                errors::system::message( "sigaction" ) );
            goto failure;
        }
    }

    ::close( server_fd );
    return fetched_atoms;
failure:
    ::close( server_fd );
    ::exit( EXIT_FAILURE );
}
