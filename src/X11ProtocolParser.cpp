#include <string>
#include <string_view>
#include <utility>   // move

#include <cassert>
#include <cstdint>

#include <stdio.h>  // FILE feof ferror

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "Settings.hpp"
#include "protocol/common_types.hpp"
#include "protocol/atoms.hpp"
#include "protocol/Response.hpp"
#include "protocol/requests.hpp"  // PolySegment::SEGMENT
#include "protocol/events.hpp"  // codes::MAX events::ENCODING_SZ
#include "protocol/errors.hpp"  // errors::ENCODING_SZ
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

void X11ProtocolParser::importSettings(
    const Settings& settings_,
    const std::vector< std::string >& fetched_atoms ) {

    _ROOT_WS = _Whitespace{ 0, settings_.multiline };
    settings = settings_;
    assert( settings.log_fs != nullptr );
    assert( !::feof( settings.log_fs ) && !::ferror( settings_.log_fs ) );

    uint32_t i { 1 };
    for ( ; i <= protocol::atoms::predefined::MAX; ++i ) {
        _interned_atoms.emplace( i, protocol::atoms::predefined::strings[ i ] );
    }
    if ( settings.prefetchatoms ) {
        for ( uint32_t sz ( fetched_atoms.size() ); i < sz; ++i ) {
            _interned_atoms.emplace( i, fetched_atoms[ i ] );
        }
    }
}

size_t
X11ProtocolParser::_logRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( protocol::requests::Request::Prefix ) +
            sizeof( protocol::requests::Request::Length ) );

    const protocol::requests::Request::Prefix* prefix {
        reinterpret_cast< const protocol::requests::Request::Prefix* >( data ) };
    const uint8_t major_opcode { _ordered( prefix->opcode, conn->byteswap ) };
    // map opcode to sequence number to aid in parsing request errors and replies
    const protocol::CARD16 sequence { conn->registerRequest( major_opcode ) };
    const _MajorOpcodeTraits& opcode_traits { _major_opcodes.at( major_opcode ) };
    _ParsingOutputs request {};
    if ( opcode_traits.extension ) {
        // TBD
    } else {
        assert( opcode_traits.request );
        // pointer-to-member access operator
        request = (this->*opcode_traits.request.request_parse_func)( conn, data, sz );
        fmt::println( settings.log_fs,
                      "C{:03d}:{:04d}B:{}:S{:05d}: Request {}({}): {}",
                      conn->id, request.bytes_parsed, CLIENT_TO_SERVER, sequence,
                      opcode_traits.request.name, major_opcode, request.str );
    }
    return request.bytes_parsed;
}

size_t X11ProtocolParser::_logReply(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    // get request opcode via sequence number
    using protocol::requests::Reply;
    const bool byteswap { conn->byteswap };
    const Reply::Header* header {
        reinterpret_cast< const Reply::Header* >( data ) };
    assert( _ordered( header->reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    const protocol::CARD16 sequence { _ordered( header->sequence_num, byteswap ) };
    const Connection::RequestOpcodes opcodes { conn->lookupRequest( sequence ) };
    const _MajorOpcodeTraits& opcode_traits { _major_opcodes.at( opcodes.major ) };
    _ParsingOutputs reply;
    if ( opcode_traits.extension ) {
        // TBD
    } else {
        assert( opcode_traits.request );
        assert( opcode_traits.request.reply_parse_func != nullptr );
        // pointer-to-member access operator
        reply = (this->*opcode_traits.request.reply_parse_func)( conn, data, sz );
        fmt::println( settings.log_fs,
                      "C{:03d}:{:04d}B:{}:S{:05d}: Reply to {}({}): {}",
                      conn->id, reply.bytes_parsed, SERVER_TO_CLIENT, sequence,
                      opcode_traits.request.name, opcodes.major, reply.str );
    }
    // ListFontsWithInfo presents edge case as it issues a series of replies
    using protocol::requests::ListFontsWithInfo;
    if ( opcodes.major != protocol::requests::opcodes::LISTFONTSWITHINFO ||
         _ordered( reinterpret_cast< const ListFontsWithInfo::Reply::Header* >(
                       data )->last_reply, byteswap ) ==
         ListFontsWithInfo::Reply::LAST_REPLY ) {
        conn->unregisterRequest( sequence );
    }
    assert( reply.bytes_parsed >=
            protocol::requests::Reply::DEFAULT_ENCODING_SZ );
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
    const std::string sequence_num_str {
        ( code == protocol::events::codes::KEYMAPNOTIFY ) ? "?????" :
        fmt::format( "{:05d}", _ordered( header->sequence_num, byteswap ) ) };
    const _ParsingOutputs event {
        _parseEvent( conn, data, sz, _ROOT_WS ) };
    assert( event.bytes_parsed == protocol::events::Event::ENCODING_SZ );
    const _EventCodeTraits& code_traits { _event_codes.at( code ) };
    if ( code_traits.extension ) {
        // TBD
    } else {
        fmt::println( settings.log_fs,
                      "C{:03d}:{:04d}B:{}:S{}: Event {}({}){}: {}",
                      conn->id, event.bytes_parsed, SERVER_TO_CLIENT,
                      sequence_num_str, code_traits.name, code,
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
        // TBD
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

size_t X11ProtocolParser::_logClientPacket(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    assert( conn->status != Connection::CLOSED &&
            conn->status != Connection::FAILED );

    size_t bytes_parsed {};
    switch ( conn->status ) {
    case Connection::UNESTABLISHED:
        bytes_parsed = _logConnectionSetup<
            protocol::connection_setup::Initiation >( conn, data, sz );
        break;
    case Connection::AUTHENTICATION:
        throw std::runtime_error(
            fmt::format( "{}: must close connection {} - cannot parse packets "
                         "as X11 protocol does not specify an encoding for "
                         "authentication negotiation",
                         __PRETTY_FUNCTION__, conn->id ) );
        break;
    case Connection::OPEN:
        bytes_parsed = _logRequest( conn, data, sz );
        break;
    default:
        break;
    }
    assert( bytes_parsed <= sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerPacket(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    assert( conn->status != Connection::CLOSED &&
            conn->status != Connection::FAILED );

    size_t bytes_parsed {};
    switch ( conn->status ) {
    case Connection::UNESTABLISHED:
        bytes_parsed = _logConnectionSetup<
            protocol::connection_setup::InitResponse >( conn, data, sz );
        break;
    case Connection::AUTHENTICATION:
        throw std::runtime_error(
            fmt::format( "{}: must close connection {} - cannot parse packets "
                         "as X11 protocol does not specify an encoding for "
                         "authentication negotiation",
                         __PRETTY_FUNCTION__, conn->id ) );
        break;
    case Connection::OPEN: {
        assert( sz >= sizeof( protocol::Response::Header ) );
        switch ( auto header {
                reinterpret_cast< const protocol::Response::Header* >( data ) };
            _ordered( header->prefix, conn->byteswap ) ) {
        case protocol::Response::ERROR_PREFIX:
            bytes_parsed = _logError( conn, data, sz );
            assert( bytes_parsed == protocol::errors::Error::ENCODING_SZ );
            break;
        case protocol::Response::REPLY_PREFIX:
            bytes_parsed = _logReply( conn, data, sz );
            assert( bytes_parsed >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );
            break;
        default:
            // event codes
            bytes_parsed = _logEvent( conn, data, sz );
            assert( bytes_parsed == protocol::events::Event::ENCODING_SZ );
            break;
        }
    }
        break;
    default:
        break;
    }
    assert( bytes_parsed <= sz );
    return bytes_parsed;
}

X11ProtocolParser::_CodeTraits::~_CodeTraits() = default;
X11ProtocolParser::_SingleCodeTraits::~_SingleCodeTraits() = default;

void X11ProtocolParser::_enableExtensionParsing(
    const std::string_view& name, Connection* conn,
    const protocol::CARD8 major_opcode, const protocol::CARD8 first_event,
    const protocol::CARD8 first_error ) {
    assert( conn != nullptr );

    auto ext_it { _extensions.find( name ) };
    // No complex validation of extension name, as we expect the X server to
    //   have already replied to QueryExtension with `present` == True
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

size_t X11ProtocolParser::logClientPackets( Connection* conn ) {
    assert( conn != nullptr );

    uint8_t* data { conn->client_buffer.data() };
    size_t   tl_bytes_parsed {};
    for ( const size_t bytes_to_parse { conn->client_buffer.size() };
          tl_bytes_parsed < bytes_to_parse; ) {
        size_t bytes_parsed {
            _logClientPacket( conn, data, bytes_to_parse - tl_bytes_parsed ) };
        if ( settings.readwritedebug ) {
            fmt::println( settings.log_fs, "C{:03d}:{:04d}B:{}: parsed buffer segment",
                          conn->id, bytes_parsed, CLIENT_TO_SERVER );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    assert( tl_bytes_parsed == conn->client_buffer.size() );
    return tl_bytes_parsed;
}

size_t X11ProtocolParser::logServerPackets( Connection* conn ) {
    assert( conn != nullptr );

    uint8_t* data { conn->server_buffer.data() };
    size_t   tl_bytes_parsed {};
    for ( const size_t bytes_to_parse { conn->server_buffer.size() };
          tl_bytes_parsed < bytes_to_parse; ) {
        size_t bytes_parsed {
            _logServerPacket( conn, data, bytes_to_parse - tl_bytes_parsed ) };
        if ( settings.readwritedebug ) {
            fmt::println( settings.log_fs, "C{:03d}:{:04d}B:{}: parsed buffer segment",
                          conn->id, bytes_parsed, SERVER_TO_CLIENT );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    assert( tl_bytes_parsed == conn->server_buffer.size() );
    return tl_bytes_parsed;
}
