#include <string_view>
#include <utility>   // move

#include <cassert>

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


void
X11ProtocolParser::_stashString( const _StashedStringID ss_id,
                                 const std::string_view str ) {
    assert( _stashed_strings.find( ss_id ) == _stashed_strings.end() );
    _stashed_strings.emplace( ss_id, str );
}

void
X11ProtocolParser::_internStashedAtom( const _StashedStringID ss_id,
                                       const protocol::ATOM atom ) {
    assert( atom.data != protocol::atoms::NONE );
    auto ss_it { _stashed_strings.find( ss_id ) };
    assert( ss_it != _stashed_strings.end() );
    // TBD not sure if server will reuse ATOMs, so we allow for it in our
    //   mirroring of internments
    _interned_atoms[ atom.data ] = ss_it->second;
    _stashed_strings.erase( ss_it );
}

void X11ProtocolParser::importSettings(
    const Settings& settings_,
    const std::vector< std::string >& fetched_atoms ) {

    _ROOT_WS = _Whitespace{ 0, settings_.multiline };
    settings = settings_;
    assert( settings.log_fs != nullptr );
    assert( !feof( settings.log_fs ) && !ferror( settings_.log_fs ) );

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
        assert( sz >=
                sizeof( protocol::connection_setup::ConnInitiation::Encoding ) );
        bytes_parsed = _logConnInitiation( conn, data, sz );
        break;
    case Connection::AUTHENTICATION:
        // authentication negotiation
        break;
    case Connection::OPEN:
        assert( sz >= sizeof( protocol::requests::Request::Header ) );
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
        bytes_parsed = _logConnResponse( conn, data, sz );
        break;
    case Connection::AUTHENTICATION:
        // TBD authentication negotiation, not sure how to parse packets
        break;
    case Connection::OPEN: {
        assert( sz >= sizeof( protocol::Response::Header ) );
        switch ( reinterpret_cast< const protocol::Response::Header* >(
                     data )->prefix ) {
        case protocol::Response::ERROR_PREFIX:
            bytes_parsed = _logError( conn, data, sz );
            assert( bytes_parsed == protocol::errors::Error::ENCODING_SZ );
            break;
        case protocol::Response::REPLY_PREFIX:
            bytes_parsed = _logReply( conn, data, sz );
            assert( bytes_parsed >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );
            break;
        default:
            // TBD event codes (2-35), will change with extensions
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

// TBD pass in readwritedebug or keep as private parser var like verbosity?
// client packets begin with:
//   - first contact: 'B' or 'l' (66 or 108)
//   - requests: opcode of 1-119, 127
//   - extension enable: ??? (logs of xtrace indicate packets sent after
//       QueryExtension gets positive response from server, to then enable
//       that extension)
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
                          conn->id, bytes_parsed, _CLIENT_TO_SERVER );
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
                          conn->id, bytes_parsed, _SERVER_TO_CLIENT );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    assert( tl_bytes_parsed == conn->server_buffer.size() );
    return tl_bytes_parsed;
}
