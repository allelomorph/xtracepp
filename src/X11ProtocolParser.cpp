#include <string_view>

#include <cassert>

#include <stdio.h>  // FILE feof ferror

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "Settings.hpp"
#include "protocol/common_types.hpp"
#include "protocol/predefined_atoms.hpp"
#include "protocol/requests.hpp"  // PolySegment::SEGMENT
#include "protocol/events.hpp"  // codes::MAX events::ENCODING_SZ
#include "protocol/errors.hpp"  // errors::ENCODING_SZ


std::string_view
X11ProtocolParser::_Whitespace::_tabIndent( const uint8_t tab_ct ) {
    static constexpr std::string_view WHITESPACE {
        "                                                           "
        "                                                           "
    };
    const size_t indent_sz ( tab_ct * _TAB_SZ );
    assert( indent_sz <= WHITESPACE.size() );
    return { WHITESPACE.data(), indent_sz };
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
        bytes_parsed = _logClientInitiation( conn, data, sz );
        break;
    case Connection::AUTHENTICATION:
        // authentication negotiation
        break;
    case Connection::OPEN:
        assert( sz >= 4 );  // TBD min size for Reuqests (1B opcode, 1B unused, 2B request length (with no extensions))
        bytes_parsed = _logClientRequest( conn, data, sz );
        //assert( bytes_parsed == 32 );
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
        bytes_parsed = _logServerResponse( conn, data, sz );
        break;
    case Connection::AUTHENTICATION:
        // authentication negotiation
        break;
    case Connection::OPEN: {
        assert( sz >= 32 );  // TBD size of Error and Event, min size for Reply (outside of extensions)
        // TBD when parsing replies, we will need to fetch the appropriate struct
        //   via the corresponding request, so we may need to keep a map of request
        //   opcodes by sequence number & 0000FFFF; which will let us look up
        //   an opcode for each seq num, and then a request by that opcode
        // TBD map could be a vector, as protocol dicates that on each client connx
        //   requests are 1-indexed by the server
        // server packets begin with:
        //   - errors: 0
        //   - replies: 1
        //   - events: 2-34
        const uint8_t first_byte { *data };
        assert( first_byte <= protocol::events::codes::MAX );
        switch ( first_byte ) {
        case 0:   // Error
            bytes_parsed = _logServerError( conn, data, sz );
            assert( bytes_parsed == protocol::errors::ENCODING_SZ );
            break;
        case 1:   // Reply
            // TBD modification of QueryExtension replies should happen here to filter extensions
            bytes_parsed = _logServerReply( conn, data, sz );
            assert( bytes_parsed >= protocol::requests::DEFAULT_REPLY_ENCODING_SZ );
            break;
        default:  // Event (2-35)
            bytes_parsed = _logServerEvent( conn, data, sz );
            assert( bytes_parsed == protocol::events::ENCODING_SZ );
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

// TBD this should all happen in parser ctor to allow const settings
void X11ProtocolParser::importSettings(
    const Settings& settings_ ) {

    settings = settings_;
    assert( settings.log_fs != nullptr );
    assert( !feof( settings.log_fs ) && !ferror( settings_.log_fs ) );

    _ROOT_WS = _Whitespace( 0, settings.multiline );
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
            fmt::println( settings.log_fs, "{:03d}:<:parsed   {:4d} bytes",
                          conn->id, bytes_parsed );
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
            fmt::println( settings.log_fs, "{:03d}:>:parsed   {:4d} bytes",
                          conn->id, bytes_parsed );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    assert( tl_bytes_parsed == conn->server_buffer.size() );
    return tl_bytes_parsed;
}

void
X11ProtocolParser::_stashAtom( const _StashedAtomID sa_id,
                               const std::string_view atom_str ) {
    assert( _stashed_atoms.find(sa_id) == _stashed_atoms.end() );
    _stashed_atoms.emplace( sa_id, atom_str );
}

void
X11ProtocolParser::_internStashedAtom( const _StashedAtomID sa_id,
                                       const protocol::ATOM atom ) {
    assert( atom.data != protocol::atoms::NONE );
    auto sa_it { _stashed_atoms.find(sa_id) };
    assert( sa_it != _stashed_atoms.end() );
    bool intern { true };
    if ( atom.data <= protocol::atoms::PREDEFINED_MAX ) {
        for ( const std::string_view& pd_atom_str : protocol::atoms::predefined ) {
            if ( sa_it->second == pd_atom_str ) {
                intern = false;
                break;
            }
        }
    }
    if ( intern ) {
        // operator[] is preferable to .emplace() here in case of server
        //   reusing ATOMs
        _interned_atoms[ atom.data ] = sa_it->second;
    }
    _stashed_atoms.erase( sa_it );
}

std::optional<std::string_view>
X11ProtocolParser::_getInternedAtom(
    const protocol::ATOM atom) {
    auto ia_it { _interned_atoms.find( atom.data ) };
    if ( ia_it == _interned_atoms.end() )
        return std::nullopt;
    return ia_it->second;
}
