#include <string_view>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/connection_setup.hpp"


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
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (status: failed){}",
            _ROOT_WS.memb_indent, "success", name_width, _ROOT_WS.equals,
            _formatInteger( header->success ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length of reason in bytes){}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( header->n ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "protocol-major-version", name_width, _ROOT_WS.equals,
            _formatInteger( header->protocol_major_version ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "protocol-minor-version", name_width, _ROOT_WS.equals,
            _formatInteger( header->protocol_minor_version ), _ROOT_WS.separator ) :
        fmt::format(
            "{}{: <{}}{}{:d}.{:d}{}",
            _ROOT_WS.memb_indent, "protocol version", name_width, _ROOT_WS.equals,
            header->protocol_major_version,
            header->protocol_minor_version, _ROOT_WS.separator ),
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (padded length of reason in 4 byte units){}",
            _ROOT_WS.memb_indent, "reason aligned units", name_width, _ROOT_WS.equals,
            _formatInteger( header->reason_aligned_units ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "reason", name_width, _ROOT_WS.equals,
        reason, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
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
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (status: authentication){}",
            _ROOT_WS.memb_indent, "success", name_width, _ROOT_WS.equals,
            _formatInteger( header->success ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (padded length of reason in 4 byte units){}",
            _ROOT_WS.memb_indent, "reason aligned units", name_width, _ROOT_WS.equals,
            _formatInteger( header->reason_aligned_units ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "reason", name_width, _ROOT_WS.equals,
        reason, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
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
            _ROOT_WS.nested() ) };
    bytes_parsed += pixmap_formats_outputs.bytes_parsed;

    // followed by LISTofSCREEN roots of m bytes (m is always a multiple of 4)
    _ParsingOutputs roots_outputs {
        _parseLISTof < ServerAcceptance::SCREEN >(
            data + bytes_parsed, sz - bytes_parsed, header->r,
            _ROOT_WS.nested() ) };
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
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (status: accepted){}",
            _ROOT_WS.memb_indent, "success", name_width, _ROOT_WS.equals,
            _formatInteger( header->success ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "protocol-major-version", name_width, _ROOT_WS.equals,
            _formatInteger( header->protocol_major_version ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "protocol-minor-version", name_width, _ROOT_WS.equals,
            _formatInteger( header->protocol_minor_version ), _ROOT_WS.separator ) :
        fmt::format(
            "{}{: <{}}{}{:d}.{:d}{}",
            _ROOT_WS.memb_indent, "protocol version", name_width, _ROOT_WS.equals,
            header->protocol_major_version,
            header->protocol_minor_version, _ROOT_WS.separator ),
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length of (padded) vendor + pixmap-formats + roots, in 4-byte units){}",
            _ROOT_WS.memb_indent, "ad", name_width, _ROOT_WS.equals,
            _formatInteger( header->ad ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "release-number", name_width, _ROOT_WS.equals,
        _formatInteger( header->release_number ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "resource-id-base", name_width, _ROOT_WS.equals,
        _formatInteger( header->resource_id_base ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "resource-id-mask", name_width, _ROOT_WS.equals,
        _formatInteger( header->resource_id_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "motion-buffer-size", name_width, _ROOT_WS.equals,
        _formatInteger( header->motion_buffer_size ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length of vendor in bytes){}",
            _ROOT_WS.memb_indent, "v", name_width, _ROOT_WS.equals,
            _formatInteger( header->v ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "maximum-request-length", name_width, _ROOT_WS.equals,
        _formatInteger( header->maximum_request_length ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (number of SCREENs in roots){}",
                     _ROOT_WS.memb_indent, "r", name_width, _ROOT_WS.equals,
            _formatInteger( header->r ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (number of FORMATs in pixmap-formats){}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( header->n ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "image-byte-order", name_width, _ROOT_WS.equals,
        _formatInteger( header->image_byte_order,
                        ServerAcceptance::byte_order_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "bitmap-format-bit-order", name_width, _ROOT_WS.equals,
        _formatInteger( header->bitmap_format_bit_order,
                        ServerAcceptance::bit_order_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "bitmap-format-scanline-unit", name_width, _ROOT_WS.equals,
        _formatInteger( header->bitmap_format_scanline_unit ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "bitmap-format-scanline-pad", name_width, _ROOT_WS.equals,
        _formatInteger( header->bitmap_format_scanline_pad ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "min-keycode", name_width, _ROOT_WS.equals,
        _formatProtocolType( header->min_keycode ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "max-keycode", name_width, _ROOT_WS.equals,
        _formatProtocolType( header->max_keycode ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "vendor", name_width, _ROOT_WS.equals,
        vendor, _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "pixmap-formats", name_width, _ROOT_WS.equals,
        pixmap_formats_outputs.str, _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "roots", name_width, _ROOT_WS.equals,
        roots_outputs.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
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
