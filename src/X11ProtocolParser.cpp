#include <string>
#include <string_view>
#include <optional>                              // nullopt
#include <utility>                               // move pair
#include <vector>

#include <cassert>
#include <cstdint>
#include <cstdio>                                // feof, ferror
#include <cstdlib>                               // exit, EXIT_FAILURE

#include <fmt/format.h>

#include "Connection.hpp"
#include "Settings.hpp"
#include "SocketBuffer.hpp"
#include "X11ProtocolParser.hpp"

#include "protocol/Response.hpp"
#include "protocol/atoms.hpp"
#include "protocol/common_types.hpp"
#include "protocol/connection_setup.hpp"
#include "protocol/errors.hpp"
#include "protocol/events.hpp"
#include "protocol/requests.hpp"
#include "protocol/extensions/requests.hpp"
#include "protocol/extensions/big_requests.hpp"


void
X11ProtocolParser::_stashString(
    const uint32_t conn_id, const uint16_t seq_num,
    const std::string_view str ) {
    const _StashedStringID ss_id { conn_id, seq_num };
    assert( _stashed_strings.find( ss_id ) == _stashed_strings.end() );
    _stashed_strings.emplace( ss_id, str );
}

std::string_view
X11ProtocolParser::_unstashString( const uint32_t conn_id,
                                   const uint16_t seq_num ) {
    auto ss_it { _stashed_strings.find( { conn_id, seq_num } ) };
    assert( ss_it != _stashed_strings.end() );
    std::string_view str { ss_it->second };
    _stashed_strings.erase( ss_it );
    return str;
}

X11ProtocolParser::X11ProtocolParser( const Settings& settings_ ) :
    _ROOT_WS( 0, settings_.multiline ), settings( settings_ ) {
    // strict, cheap alternative to singleton pattern
    static bool instantiated {};
    if ( instantiated ) {
        fmt::println( ::stderr, "{}: {}: constructor called more than once",
                      settings.process_name, __PRETTY_FUNCTION__ );
        ::exit( EXIT_FAILURE );
    }
    instantiated = true;

    assert( settings.log_fs != nullptr );
    assert( !::feof( settings.log_fs ) && !::ferror( settings_.log_fs ) );

    for ( uint32_t i { 1 }; i <= protocol::atoms::predefined::MAX; ++i ) {
        _interned_atoms.emplace( i, protocol::atoms::predefined::strings.at( i ) );
    }
}

void X11ProtocolParser::importFetchedAtoms(
    const std::vector< std::string >& fetched_atoms ) {
    assert( settings.prefetchatoms );
    // by default core predefined ATOMS 1..68 should be interned
    assert( _interned_atoms.size() == protocol::atoms::predefined::MAX );
    for ( uint32_t i { protocol::atoms::predefined::MAX + 1 },
              sz ( fetched_atoms.size() ); i < sz; ++i ) {
        _interned_atoms.emplace( i, fetched_atoms.at( i ) );
    }
}

size_t
X11ProtocolParser::_logRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    namespace ext = protocol::extensions;
    assert( sz >= sizeof( ext::requests::Request::Prefix ) +
            sizeof( ext::requests::Request::Length ) );

    const ext::requests::Request::Prefix* prefix {
        reinterpret_cast< const ext::requests::Request::Prefix* >( data ) };
    const uint8_t major_opcode { _ordered( prefix->major_opcode, conn->byteswap ) };
    const uint8_t minor_opcode { _ordered( prefix->minor_opcode, conn->byteswap ) };
    // map opcode to sequence number to aid in parsing request errors and replies
    const protocol::CARD16 sequence { conn->registerRequest( major_opcode ) };
    std::string_view request_name { "(unknown opcode)" };
    _RequestOpcodeTraits::ParseFuncT request_parse_func {
        &X11ProtocolParser::_parseRequest< X11ProtocolParser::_UnknownRequest > };
    std::string_view extension_name {};
    if ( const auto major_oc_it { _major_opcodes.find( major_opcode ) };
         major_oc_it != _major_opcodes.end() ) {
        const _MajorOpcodeTraits& major_oct { major_oc_it->second };
        if ( major_oct.extension ) {
            extension_name = major_oct.extension.name;
            if ( const auto minor_oc_it {
                    major_oct.extension.requests.find( minor_opcode ) };
                minor_oc_it != major_oct.extension.requests.end() ) {
                request_name = minor_oc_it->second.name;
                request_parse_func = minor_oc_it->second.request_parse_func;
            }
        } else {
            assert( major_oct.request );
            request_name = major_oct.request.name;
            request_parse_func = major_oct.request.request_parse_func;
        }
    }
    // pointer-to-member access operator
    const _ParsingOutputs request { ( this->*request_parse_func )( conn, data, sz ) };
    if ( !extension_name.empty() ) {
        fmt::println( settings.log_fs,
                      "C{:03d}:{:04d}B:{}:S{:05d}: Request {}({})-{}({}): {}",
                      conn->id, request.bytes_parsed, CLIENT_TO_SERVER, sequence,
                      extension_name, major_opcode,
                      request_name, minor_opcode, request.str );
    } else {
        fmt::println( settings.log_fs,
                      "C{:03d}:{:04d}B:{}:S{:05d}: Request {}({}): {}",
                      conn->id, request.bytes_parsed, CLIENT_TO_SERVER, sequence,
                      request_name, major_opcode, request.str );
    }
    assert( request.bytes_parsed != 0 );
    assert( request.bytes_parsed <= sz );
    return request.bytes_parsed;
}

size_t X11ProtocolParser::_logReply(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    using protocol::requests::Reply;
    const bool byteswap { conn->byteswap };
    const Reply::Header* header {
        reinterpret_cast< const Reply::Header* >( data ) };
    assert( _ordered( header->reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // get request opcode via sequence number
    const protocol::CARD16 sequence { _ordered( header->sequence_num, byteswap ) };
    const Connection::RequestOpcodes opcodes { conn->lookupRequest( sequence ) };
    std::string_view request_name { "(unknown opcode)" };
    _RequestOpcodeTraits::ParseFuncT reply_parse_func {
        &X11ProtocolParser::_parseReply< X11ProtocolParser::_UnknownRequest > };
    std::string_view extension_name {};
    if ( const auto major_oc_it { _major_opcodes.find( opcodes.major ) };
         major_oc_it != _major_opcodes.end() ) {
        const _MajorOpcodeTraits& major_oct { major_oc_it->second };
        if ( major_oct.extension ) {
            extension_name = major_oct.extension.name;
            if ( const auto minor_oc_it {
                    major_oct.extension.requests.find( opcodes.minor ) };
                 minor_oc_it != major_oct.extension.requests.end() ) {
                request_name = minor_oc_it->second.name;
                reply_parse_func = minor_oc_it->second.reply_parse_func;
                assert( reply_parse_func != nullptr );
            }
        } else {
            assert( major_oct.request );
            request_name = major_oct.request.name;
            reply_parse_func = major_oct.request.reply_parse_func;
            assert( reply_parse_func != nullptr );
        }
    }
    // pointer-to-member access operator
    const _ParsingOutputs reply { ( this->*reply_parse_func )( conn, data, sz ) };
    if ( !extension_name.empty() ) {
        fmt::println( settings.log_fs,
                      "C{:03d}:{:04d}B:{}:S{:05d}: Reply to {}({})-{}({}): {}",
                      conn->id, reply.bytes_parsed, CLIENT_TO_SERVER, sequence,
                      extension_name, opcodes.major,
                      request_name, opcodes.minor, reply.str );
    } else {
        fmt::println( settings.log_fs,
                      "C{:03d}:{:04d}B:{}:S{:05d}: Reply to {}({}): {}",
                      conn->id, reply.bytes_parsed, CLIENT_TO_SERVER, sequence,
                      request_name, opcodes.major, reply.str );
    }
    // ListFontsWithInfo presents edge case as it issues a series of replies
    using protocol::requests::ListFontsWithInfo;
    if ( opcodes.major != protocol::requests::opcodes::LISTFONTSWITHINFO ||
         _ordered( reinterpret_cast< const ListFontsWithInfo::Reply::Header* >(
                       data )->last_reply, byteswap ) ==
         ListFontsWithInfo::Reply::LAST_REPLY ) {
        conn->unregisterRequest( sequence );
    }
    assert( reply.bytes_parsed != 0 );
    assert( reply.bytes_parsed <= sz );
    return reply.bytes_parsed;
}

size_t X11ProtocolParser::_logEvent(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SendEvent;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::events::Event::ENCODING_SZ );

    const bool byteswap { conn->byteswap };
    const protocol::events::Event::Header* header {
        reinterpret_cast< const protocol::events::Event::Header* >( data ) };
    // event codes with msb turned on are generated by request SendEvent
    const uint8_t code ( _ordered( header->code, byteswap ) &
                         SendEvent::EVENT_CODE_MASK );
    const bool generated ( _ordered( header->code, byteswap ) &
                           SendEvent::GENERATED_EVENT_FLAG );
    // KeymapNotify presents edge case, as it does not encode a sequence number
    const std::string sequence_str {
        ( code == protocol::events::codes::KEYMAPNOTIFY ) ? "?????" :
        fmt::format( "{:05d}", _ordered( header->sequence_num, byteswap ) ) };
    const _ParsingOutputs event {
        _parseEvent( conn, data, sz, _ROOT_WS ) };
    assert( event.bytes_parsed == protocol::events::Event::ENCODING_SZ );
    const _EventCodeTraits& code_traits { _event_codes.at( code ) };
    if ( code_traits.extension ) {
        fmt::println( settings.log_fs,
                      "C{:03d}:{:04d}B:{}:S{}: Event {}-{}({}){}: {}",
                      conn->id, event.bytes_parsed, SERVER_TO_CLIENT,
                      sequence_str, code_traits.extension_name,
                      code_traits.name, code,
                      generated ? " (generated)" : "", event.str );
    } else {
        fmt::println( settings.log_fs,
                      "C{:03d}:{:04d}B:{}:S{}: Event {}({}){}: {}",
                      conn->id, event.bytes_parsed, SERVER_TO_CLIENT,
                      sequence_str, code_traits.name, code,
                      generated ? " (generated)" : "", event.str );
    }
    return event.bytes_parsed;
}

size_t
X11ProtocolParser::_logError(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Error;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( Error::Encoding ) );

    const bool byteswap { conn->byteswap };
    const Error::Encoding* encoding {
        reinterpret_cast< const Error::Encoding* >( data ) };
    assert( _ordered( encoding->header.error, byteswap ) == Error::ERROR );
    const uint8_t code {
        _ordered( encoding->header.code, byteswap ) };
    const protocol::CARD16 sequence {
        _ordered( encoding->header.sequence_num, byteswap ) };
    const _ErrorCodeTraits& code_traits { _error_codes.at( code ) };
    _ParsingOutputs error {
        // pointer-to-member access operator
        (this->*code_traits.parse_func)( conn, data, sz ) };
    if ( code_traits.extension ) {
        fmt::println( settings.log_fs,
                      "C{:03d}:{:04d}B:{}:S{:05d}: Error {}-{}({}): {}",
                      conn->id, error.bytes_parsed, CLIENT_TO_SERVER, sequence,
                      code_traits.extension_name,
                      code_traits.name, code, error.str );
    } else {
        fmt::println( settings.log_fs,
                      "C{:03d}:{:04d}B:{}:S{:05d}: Error {}({}): {}",
                      conn->id, error.bytes_parsed, SERVER_TO_CLIENT, sequence,
                      code_traits.name, code, error.str );
    }
    // presume that no more messages will relate to this request
    conn->unregisterRequest( sequence );
    return error.bytes_parsed;
}

X11ProtocolParser::_CodeTraits::~_CodeTraits() = default;
X11ProtocolParser::_SingleCodeTraits::~_SingleCodeTraits() = default;

void X11ProtocolParser::_enableExtensionParsing(
    const std::string_view& name,
    const protocol::CARD8 major_opcode, const protocol::CARD8 first_event,
    const protocol::CARD8 first_error ) {

    // No complex validation of extension name, as we expect the X server to
    //   have already replied to QueryExtension with `present` == True
    assert( !settings.extensionDenied( name ) );
    auto ext_it { _extensions.find( name ) };
    assert( ext_it != _extensions.end() );
    const auto& extension { ext_it->second };

    //// Set up extension parsing for all connections if not already done
    if ( auto it { _major_opcodes.find( major_opcode ) };
         it == _major_opcodes.end() ) {
        _major_opcodes.emplace(
            major_opcode, _MajorOpcodeTraits( name, extension.requests ) );
        for ( const auto& [ offset, event ] : extension.events ) {
            assert( event.extension_name == name );
            _event_codes.emplace( first_event + offset, event );
        }
        for ( const auto& [ offset, error ] : extension.errors ) {
            assert( error.extension_name == name );
            _error_codes.emplace( first_error + offset, error );
        }
    }
}

std::pair< size_t, std::optional< std::string > >
X11ProtocolParser::logClientMessages( Connection* conn ) {
    assert( conn != nullptr );

    size_t tl_bytes_parsed {};
    SocketBuffer& buffer { conn->client_buffer };
    uint8_t* data { buffer.data() };
    while ( buffer.parsed() < buffer.size() ) {
        if ( !buffer.messageSizeSet() ) {
            switch ( conn->status ) {
            case Connection::UNESTABLISHED: {
                using protocol::connection_setup::Initiation;
                if ( buffer.size() < sizeof( Initiation::Header ) ) {
                    goto length_checks;
                }
                const Initiation::Header* header {
                    reinterpret_cast< const Initiation::Header* >( data ) };
                assert( header->byte_order == Initiation::MSBFIRST ||
                        header->byte_order == Initiation::LSBFIRST );
                // determine if host byte order is same as client ( potentially
                //   different with remote clients )
                conn->byteswap =
                    ( little_endian != ( header->byte_order == Initiation::LSBFIRST ) );
                buffer.setMessageSize(
                    sizeof( protocol::connection_setup::Initiation::Header ) +
                    alignment.pad( _ordered( header->name_len, conn->byteswap ) ) +
                    alignment.pad( _ordered( header->data_len, conn->byteswap ) ) );
            }
                break;
            case Connection::AUTHENTICATION:
                return {
                    tl_bytes_parsed, fmt::format(
                        "must close connection - cannot parse messages as X11 "
                        "protocol does not specify an encoding for "
                        "authentication negotiation" ) };
            case Connection::OPEN: {
                using protocol::requests::Request;
                if ( buffer.size() <
                     sizeof( Request::Prefix ) + sizeof( Request::Length ) ) {
                    goto length_checks;
                }
                const Request::BigLength* big_length {
                    reinterpret_cast< const Request::BigLength* >(
                        data + sizeof( Request::Prefix ) ) };
                if ( big_length->extended_length_flag ==
                     protocol::extensions::big_requests::EXTENDED_LENGTH_FLAG ) {
                    if ( buffer.size() <
                         sizeof( Request::Prefix ) + sizeof( Request::BigLength ) ) {
                        goto length_checks;
                    }
                    buffer.setMessageSize(
                        alignment.size(
                            _ordered( big_length->tl_aligned_units,
                                      conn->byteswap ) ) );
                } else {
                    const Request::Length* length {
                        reinterpret_cast< const Request::Length* >(
                            data + sizeof( Request::Prefix ) ) };
                    buffer.setMessageSize(
                        alignment.size(
                            _ordered( length->tl_aligned_units,
                                      conn->byteswap ) ) );
                }
            }
                break;
            default:
                break;
            }
        }
    length_checks:
        if ( !buffer.messageSizeSet() ) {
            if ( settings.readwritedebug ) {
                fmt::println( settings.log_fs,
                              "C{:03d}:{}: waiting on incomplete message, "
                              "{:04d}B insufficent to determine length",
                              conn->id, CLIENT_TO_SERVER, buffer.unparsed() );
            }
            break;
        }
        if ( buffer.incompleteMessage() ) {
            if ( settings.readwritedebug ) {
                fmt::println( settings.log_fs,
                              "C{:03d}:{}: waiting on incomplete message, "
                              "received {:04d} of expected {:04d}B",
                              conn->id, CLIENT_TO_SERVER, buffer.unparsed(),
                              buffer.messageSize() );
            }
            break;
        }
        assert( conn->status != Connection::AUTHENTICATION );
        size_t bytes_parsed {};
        switch ( conn->status ) {
        case Connection::UNESTABLISHED:
            bytes_parsed = _logConnectionSetup<
                protocol::connection_setup::Initiation >(
                    conn, data, buffer.messageSize() );
            break;
        case Connection::OPEN:
            bytes_parsed = _logRequest(
                conn, data, buffer.messageSize() );
            break;
        default:
            break;
        }
        assert( bytes_parsed == buffer.messageSize() );
        buffer.markMessageParsed();
        if ( settings.readwritedebug ) {
            fmt::println( settings.log_fs,
                          "C{:03d}:{:04d}B:{}: parsed message in buffer",
                          conn->id, bytes_parsed, CLIENT_TO_SERVER );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    return { tl_bytes_parsed, std::nullopt };
}

std::pair< size_t, std::optional< std::string > >
X11ProtocolParser::logServerMessages( Connection* conn ) {
    assert( conn != nullptr );

    size_t tl_bytes_parsed {};
    SocketBuffer& buffer { conn->server_buffer };
    uint8_t* data { buffer.data() };
    while ( buffer.parsed() < buffer.size() ) {
        if ( !buffer.messageSizeSet() ) {
            switch ( conn->status ) {
            case Connection::UNESTABLISHED: {
                using protocol::connection_setup::InitResponse;
                if ( buffer.size() < sizeof( InitResponse::Header ) ) {
                    goto length_checks;
                }
                const InitResponse::Header* header {
                    reinterpret_cast< const InitResponse::Header* >( data ) };
                buffer.setMessageSize(
                    sizeof( InitResponse::Header ) +
                    alignment.size(
                        _ordered( header->following_aligned_units,
                                  conn->byteswap ) ) );
            }
                break;
            case Connection::AUTHENTICATION:
                return {
                    tl_bytes_parsed, fmt::format(
                        "must close connection - cannot parse messages as X11 "
                        "protocol does not specify an encoding for "
                        "authentication negotiation" ) };
            case Connection::OPEN: {
                using protocol::Response;
                if ( buffer.size() < sizeof( Response::Header ) ) {
                    goto length_checks;
                }
                const Response::Header* resp_header {
                    reinterpret_cast< const Response::Header* >( data ) };
                switch ( _ordered( resp_header->prefix, conn->byteswap ) ) {
                case Response::ERROR_PREFIX:
                    buffer.setMessageSize(
                        protocol::errors::Error::ENCODING_SZ );
                    break;
                case Response::REPLY_PREFIX: {
                    using protocol::requests::Reply;
                    const Reply::Header* reply_header {
                        reinterpret_cast< const Reply::Header* >( data ) };
                    buffer.setMessageSize(
                        Reply::DEFAULT_ENCODING_SZ +
                        alignment.size(
                            _ordered( reply_header->extra_aligned_units,
                                      conn->byteswap ) ) );
                }
                    break;
                default:  // event codes
                    buffer.setMessageSize(
                        protocol::events::Event::ENCODING_SZ );
                    break;
                }
            }
                break;
            default:
                break;
            }
        }
    length_checks:
        if ( !buffer.messageSizeSet() ) {
            if ( settings.readwritedebug ) {
                fmt::println( settings.log_fs,
                              "C{:03d}:{}: waiting on incomplete message, "
                              "{:04d}B insufficent to determine length",
                              conn->id, SERVER_TO_CLIENT, buffer.unparsed() );
            }
            break;
        }
        if ( buffer.incompleteMessage() ) {
            if ( settings.readwritedebug ) {
                fmt::println( settings.log_fs,
                              "C{:03d}:{}: waiting on incomplete message, "
                              "received {:04d} of expected {:04d}B",
                              conn->id, SERVER_TO_CLIENT, buffer.unparsed(),
                              buffer.messageSize() );
            }
            break;
        }
        assert( conn->status != Connection::AUTHENTICATION );
        size_t bytes_parsed {};
        switch ( conn->status ) {
        case Connection::UNESTABLISHED:
            bytes_parsed = _logConnectionSetup<
                protocol::connection_setup::InitResponse >(
                    conn, data, buffer.messageSize() );
            assert( conn->status == Connection::OPEN );
            break;
        case Connection::OPEN: {
            const protocol::Response::Header* resp_header {
                reinterpret_cast< const protocol::Response::Header* >( data ) };
            switch ( _ordered( resp_header->prefix, conn->byteswap ) ) {
            case protocol::Response::ERROR_PREFIX:
                bytes_parsed = _logError(
                    conn, data, buffer.messageSize() );
                break;
            case protocol::Response::REPLY_PREFIX:
                bytes_parsed = _logReply(
                    conn, data, buffer.messageSize() );
                break;
            default: // event codes
                bytes_parsed = _logEvent(
                    conn, data, buffer.messageSize() );
                break;
            }
        }   break;
        default:
            break;
        }
        assert( bytes_parsed == buffer.messageSize() );
        buffer.markMessageParsed();
        if ( settings.readwritedebug ) {
            fmt::println( settings.log_fs,
                          "C{:03d}:{:04d}B:{}: parsed message in buffer",
                          conn->id, bytes_parsed, SERVER_TO_CLIENT );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    return { tl_bytes_parsed, std::nullopt };
}
