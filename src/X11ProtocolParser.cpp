#include <string_view>
#include <array>

#include <cctype>      // isprint

//#include <sys/stat.h>  // stat struct stat
#include <stdio.h>  // FILE feof ferror

#include <fmt/format.h>
//#include <fmt/printf.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "Settings.hpp"
#include "protocol/common_types.hpp"
#include "protocol/connection_setup.hpp"
#include "protocol/requests.hpp"  // PolySegment::SEGMENT
#include "protocol/events.hpp"  // codes::MAX events::ENCODING_SZ
#include "protocol/errors.hpp"  // errors::ENCODING_SZ


std::string_view
X11ProtocolParser::_Indentation::_tabIndent( const uint8_t tab_ct ) {
    static constexpr std::string_view WHITESPACE {
        "                                                           "
        "                                                           "
    };
    const size_t indent_sz ( tab_ct * _TAB_SZ );
    assert( indent_sz <= WHITESPACE.size() );
    return { WHITESPACE.data(), indent_sz };
}

size_t X11ProtocolParser::_logClientInitiation(
    const Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );

    size_t bytes_parsed {};
    using namespace protocol::connection_setup;
    const ClientInitiation::Header* header {
        reinterpret_cast< const ClientInitiation::Header* >( data ) };
    assert( header->byte_order == 'B' || header->byte_order == 'l' );
    // TBD error if protocol is not 11.0
    bytes_parsed += sizeof( ClientInitiation::Header );
    // STRING8 of d bytes authorization-protocol-name
    const std::string_view auth_protocol_name {
        reinterpret_cast< const char* >( data + bytes_parsed ), header->n };
    bytes_parsed += _pad( header->n );
    // STRING8 of d bytes authorization-protocol-data (may not be printable)
    const uint8_t* auth_protocol_data { data + bytes_parsed };
    bytes_parsed += _pad( header->d );
    assert( bytes_parsed == sz );

    const uint32_t name_width (
        settings.multiline ? sizeof( "authorization-protocol-data" ) - 1 : 0 );

    fmt::print( settings.log_fs, "{:03d}:<:client \"{}\" requesting connection ",
                conn->id, conn->client_desc );
    // TBD may be security concerns with logging auth data
    // std::string auth_data_hex;
    // for ( uint16_t i {}, sz { header->d }; i < sz; ++i ) {
    //     auth_data_hex += fmt::format(
    //         "{}{:02X}", auth_data_hex.empty() ? "" : " ",
    //         auth_protocol_data[i] );
    // }
    fmt::println(
        settings.log_fs,
        "{{{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}"
        "{}{: <{}}{}\"{}\"{}"
        "{}"
        "{}{: <{}}{}({:d} bytes){}"
        "{}}}",
        _SEPARATOR,
        _BASE_INDENTS.member, "byte-order", name_width, _EQUALS,
        _formatInteger( ( header->byte_order == 'l' ) ? 0 : 1,
                        protocol::enum_names::image_byte_order ), _SEPARATOR,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "protocol-major-version", name_width, _EQUALS,
            _formatInteger( header->protocol_major_version ), _SEPARATOR,
            _BASE_INDENTS.member, "protocol-minor-version", name_width, _EQUALS,
            _formatInteger( header->protocol_minor_version ), _SEPARATOR ) :
        fmt::format(
            "{}{: <{}}{}{:d}.{:d}{}",
            _BASE_INDENTS.member, "protocol version", name_width, _EQUALS,
            header->protocol_major_version,
            header->protocol_minor_version, _SEPARATOR ),
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length in bytes of authorization-protocol-name){}",
            _BASE_INDENTS.member, "n", name_width, _EQUALS,
            _formatInteger( header->n ), _SEPARATOR ) : "",
        _BASE_INDENTS.member, "authorization-protocol-name", name_width, _EQUALS,
        auth_protocol_name, _SEPARATOR,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length in bytes of authorization-protocol-data){}",
            _BASE_INDENTS.member, "d", name_width, _EQUALS,
            _formatInteger( header->d ), _SEPARATOR ) : "",
        // TBD may be security concerns with logging auth data
        _BASE_INDENTS.member, "authorization-protocol-data", name_width, _EQUALS,
        header->d, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerRefusal(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    size_t bytes_parsed {};
    using namespace protocol::connection_setup;
    const ServerRefusal::Header* header {
        reinterpret_cast< const ServerRefusal::Header* >( data ) };
    bytes_parsed += sizeof( ServerRefusal::Header );
    assert( header->reason_aligned_units == _pad( header->n ) / _ALIGN );
    // STRING8 of n bytes reason
    std::string_view reason {
        reinterpret_cast< const char* >( data + bytes_parsed ), header->n };
    bytes_parsed += _pad( header->n );

    const uint32_t name_width (
        settings.multiline ? sizeof( "protocol-major-version" ) - 1 : 0 );

    fmt::print( settings.log_fs, "{:03d}:>:server refused connection ",
                conn->id );
    fmt::println(
        settings.log_fs,
        "{{{}"
        "{}"
        "{}"
        "{}"
        "{}"
        "{}{: <{}}{}\"{}\"{}"
        "{}}}",
        _SEPARATOR,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (status: failed){}",
            _BASE_INDENTS.member, "success", name_width, _EQUALS,
            _formatInteger( header->success ), _SEPARATOR ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length of reason in bytes){}",
            _BASE_INDENTS.member, "n", name_width, _EQUALS,
            _formatInteger( header->n ), _SEPARATOR ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "protocol-major-version", name_width, _EQUALS,
            _formatInteger( header->protocol_major_version ), _SEPARATOR,
            _BASE_INDENTS.member, "protocol-minor-version", name_width, _EQUALS,
            _formatInteger( header->protocol_minor_version ), _SEPARATOR ) :
        fmt::format(
            "{}{: <{}}{}{:d}.{:d}{}",
            _BASE_INDENTS.member, "protocol version", name_width, _EQUALS,
            header->protocol_major_version,
            header->protocol_minor_version, _SEPARATOR ),
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (padded length of reason in 4 byte units){}",
            _BASE_INDENTS.member, "reason aligned units", name_width, _EQUALS,
            _formatInteger( header->reason_aligned_units ), _SEPARATOR ) : "",
        _BASE_INDENTS.member, "reason", name_width, _EQUALS,
        reason, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerRequireFurtherAuthentication(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    size_t bytes_parsed {};
    using namespace protocol::connection_setup;
    const ServerRequireFurtherAuthentication::Header* header {
        reinterpret_cast<
        const ServerRequireFurtherAuthentication::Header* >( data ) };
    bytes_parsed += sizeof( ServerRequireFurtherAuthentication::Header );
    const size_t reason_padded_sz { header->reason_aligned_units * _ALIGN };
    // STRING8 of n bytes reason
    std::string_view reason {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        reason_padded_sz };
    bytes_parsed += reason_padded_sz;

    const uint32_t name_width (
        settings.multiline ? sizeof( "success" ) - 1 : 0 );

    fmt::print( settings.log_fs, "{:03d}:>:server requested further authentication ",
                conn->id );
    fmt::println(
        settings.log_fs,
        "{{{}"
        "{}"
        "{}"
        "{}{: <{}}{}\"{}\"{}"
        "{}}}",
        _SEPARATOR,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (status: authentication){}",
            _BASE_INDENTS.member, "success", name_width, _EQUALS,
            _formatInteger( header->success ), _SEPARATOR ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (padded length of reason in 4 byte units){}",
            _BASE_INDENTS.member, "reason aligned units", name_width, _EQUALS,
            _formatInteger( header->reason_aligned_units ), _SEPARATOR ) : "",
        _BASE_INDENTS.member, "reason", name_width, _EQUALS,
        reason, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerAcceptance(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    size_t bytes_parsed {};
    using namespace protocol::connection_setup;
    const ServerAcceptance::Header* header {
        reinterpret_cast< const ServerAcceptance::Header* >( data ) };
    bytes_parsed += sizeof( ServerAcceptance::Header );
    // followed by STRING8 vendor of v bytes, plus p bytes to round up to multiple of 4
    std::string_view vendor {
        reinterpret_cast< const char* >( data + bytes_parsed ), header->v };
    bytes_parsed += _pad( header->v );

    const uint32_t tab_ct { 0 };
    // followed by LISTofFORMAT pixmap-formats of n * sizeof(FORMAT) bytes
    _ParsingOutputs pixmap_formats_outputs {
        _parseLISTof< ServerAcceptance::FORMAT >(
            data + bytes_parsed, sz - bytes_parsed, header->n,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += pixmap_formats_outputs.bytes_parsed;

    // followed by LISTofSCREEN roots of m bytes (m is always a multiple of 4)
    _ParsingOutputs roots_outputs {
        _parseLISTof < ServerAcceptance::SCREEN >(
            data + bytes_parsed, sz - bytes_parsed, header->r,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += roots_outputs.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "bitmap-format-scanline-unit" ) - 1 : 0 );

    fmt::print( settings.log_fs, "{:03d}:>:server accepted connection ",
                conn->id );
    fmt::println(
        settings.log_fs,
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}\"{}\"{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (status: accepted){}",
            _BASE_INDENTS.member, "success", name_width, _EQUALS,
            _formatInteger( header->success ), _SEPARATOR ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "protocol-major-version", name_width, _EQUALS,
            _formatInteger( header->protocol_major_version ), _SEPARATOR,
            _BASE_INDENTS.member, "protocol-minor-version", name_width, _EQUALS,
            _formatInteger( header->protocol_minor_version ), _SEPARATOR ) :
        fmt::format(
            "{}{: <{}}{}{:d}.{:d}{}",
            _BASE_INDENTS.member, "protocol version", name_width, _EQUALS,
            header->protocol_major_version,
            header->protocol_minor_version, _SEPARATOR ),
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length of (padded) vendor + pixmap-formats + roots, in 4-byte units){}",
            _BASE_INDENTS.member, "ad", name_width, _EQUALS,
            _formatInteger( header->ad ), _SEPARATOR ) : "",
        _BASE_INDENTS.member, "release-number", name_width, _EQUALS,
        _formatInteger( header->release_number ), _SEPARATOR,
        _BASE_INDENTS.member, "resource-id-base", name_width, _EQUALS,
        _formatInteger( header->resource_id_base ), _SEPARATOR,
        _BASE_INDENTS.member, "resource-id-mask", name_width, _EQUALS,
        _formatInteger( header->resource_id_mask ), _SEPARATOR,
        _BASE_INDENTS.member, "motion-buffer-size", name_width, _EQUALS,
        _formatInteger( header->motion_buffer_size ), _SEPARATOR,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length of vendor in bytes){}",
            _BASE_INDENTS.member, "v", name_width, _EQUALS,
            _formatInteger( header->v ), _SEPARATOR ) : "",
        _BASE_INDENTS.member, "maximum-request-length", name_width, _EQUALS,
        _formatInteger( header->maximum_request_length ), _SEPARATOR,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (number of SCREENs in roots){}",
                     _BASE_INDENTS.member, "r", name_width, _EQUALS,
            _formatInteger( header->r ), _SEPARATOR ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (number of FORMATs in pixmap-formats){}",
            _BASE_INDENTS.member, "n", name_width, _EQUALS,
            _formatInteger( header->n ), _SEPARATOR ) : "",
        _BASE_INDENTS.member, "image-byte-order", name_width, _EQUALS,
        _formatInteger( header->image_byte_order,
                        ServerAcceptance::byte_order_names ), _SEPARATOR,
        _BASE_INDENTS.member, "bitmap-format-bit-order", name_width, _EQUALS,
        _formatInteger( header->bitmap_format_bit_order,
                        ServerAcceptance::bit_order_names ), _SEPARATOR,
        _BASE_INDENTS.member, "bitmap-format-scanline-unit", name_width, _EQUALS,
        _formatInteger( header->bitmap_format_scanline_unit ), _SEPARATOR,
        _BASE_INDENTS.member, "bitmap-format-scanline-pad", name_width, _EQUALS,
        _formatInteger( header->bitmap_format_scanline_pad ), _SEPARATOR,
        _BASE_INDENTS.member, "min-keycode", name_width, _EQUALS,
        _formatProtocolType( header->min_keycode ), _SEPARATOR,
        _BASE_INDENTS.member, "max-keycode", name_width, _EQUALS,
        _formatProtocolType( header->max_keycode ), _SEPARATOR,
        _BASE_INDENTS.member, "vendor", name_width, _EQUALS,
        vendor, _SEPARATOR,
        _BASE_INDENTS.member, "pixmap-formats", name_width, _EQUALS,
        pixmap_formats_outputs.str, _SEPARATOR,
        _BASE_INDENTS.member, "roots", name_width, _EQUALS,
        roots_outputs.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerResponse(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );

    const uint8_t success { *data };
    size_t bytes_parsed {};
    switch ( success ) {
    case protocol::connection_setup::FAILED:
        bytes_parsed = _logServerRefusal( conn, data, sz );
        conn->status = Connection::FAILED;
        break;
    case protocol::connection_setup::AUTHENTICATE:
        bytes_parsed = _logServerRequireFurtherAuthentication( conn, data, sz );
        conn->status = Connection::AUTHENTICATION;
        break;
    case protocol::connection_setup::SUCCESS:
        bytes_parsed = _logServerAcceptance( conn, data, sz );
        conn->status = Connection::OPEN;
        break;
    default:
        assert( 0 );
        break;
    }
    assert( bytes_parsed <= sz );
    return bytes_parsed;
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

    if ( settings.multiline ) {
        _BASE_INDENTS = _Indentation( 0, settings.multiline );
        _SEPARATOR = "\n";
        _EQUALS    = " = ";
    }
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
