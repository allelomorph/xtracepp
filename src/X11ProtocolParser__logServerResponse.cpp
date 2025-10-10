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
    const _Whitespace& ws { _ROOT_WS };
    using namespace protocol::connection_setup;
    const ServerRefusal::Header* header {
        reinterpret_cast< const ServerRefusal::Header* >( data ) };
    bytes_parsed += sizeof( ServerRefusal::Header );
    assert( header->success == ServerResponse::FAILED );
    // followed by STRING8 reason
    std::string_view reason {
        reinterpret_cast< const char* >( data + bytes_parsed ), header->reason_len };
    bytes_parsed += _pad( header->reason_len );
    assert( header->following_aligned_units ==
            _alignedUnits( bytes_parsed - sizeof( header ) ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "protocol-major-version" ) - 1 : 0 );

    fmt::println(
        settings.log_fs,
        "C{:03d}:{:04d}B:{}: server refused connection: "
        "{{{}"
        "{}{}"               // success, reason_len
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"                 // total_aligned_units
        "{}{: <{}}{}{:?}{}"  // reason
        "{}}}",
        conn->id, bytes_parsed, _SERVER_TO_CLIENT,
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "success", name_width, ws.equals,
            _formatInteger( header->success, ServerRefusal::success_names ),
            ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(reason length in bytes)", name_width, ws.equals,
            _formatInteger( header->reason_len ), ws.separator ),
        ws.memb_indent, "protocol-major-version", name_width, ws.equals,
        _formatInteger( header->protocol_major_version ), ws.separator,
        ws.memb_indent, "protocol-minor-version", name_width, ws.equals,
        _formatInteger( header->protocol_minor_version ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(message length after header in 4B units)",
            name_width, ws.equals,
            _formatInteger( header->following_aligned_units ), ws.separator ),
        ws.memb_indent, "reason", name_width, ws.equals,
        reason, ws.separator,
        ws.encl_indent
        );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerRequireFurtherAuthentication(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    size_t bytes_parsed {};
    const _Whitespace& ws { _ROOT_WS };
    using namespace protocol::connection_setup;
    const ServerRequireFurtherAuthentication::Header* header {
        reinterpret_cast<
        const ServerRequireFurtherAuthentication::Header* >( data ) };
    bytes_parsed += sizeof( ServerRequireFurtherAuthentication::Header );
    assert( header->success == ServerResponse::AUTHENTICATE );
    // followed by STRING8 reason
    const size_t reason_padded_len {
        header->following_aligned_units * _ALIGN };
    std::string_view reason {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        reason_padded_len };
    bytes_parsed += reason_padded_len;
    assert( header->following_aligned_units ==
            _alignedUnits( bytes_parsed - sizeof( header ) ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "success" ) - 1 : 0 );

    fmt::println(
        settings.log_fs,
        "C{:03d}:{:04d}B:{}: server requested further authentication: "
        "{{{}"
        "{}{}"               // success, total_aligned_units
        "{}{: <{}}{}{:?}{}"  // reason
        "{}}}",
        conn->id, bytes_parsed, _SERVER_TO_CLIENT,
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "success", name_width, ws.equals,
            _formatInteger( header->success, ServerRequireFurtherAuthentication::success_names ),
            ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(message length after header in 4B units)",
            name_width, ws.equals,
            _formatInteger( header->following_aligned_units ), ws.separator ),
        ws.memb_indent, "reason", name_width, ws.equals,
        reason, ws.separator,
        ws.encl_indent
        );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerAcceptance(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    size_t bytes_parsed {};
    const _Whitespace& ws { _ROOT_WS };
    using namespace protocol::connection_setup;
    const ServerAcceptance::Header* header {
        reinterpret_cast< const ServerAcceptance::Header* >( data ) };
    bytes_parsed += sizeof( ServerAcceptance::Header );
    // followed by FixedLengthEncoding
    const ServerAcceptance::FixedLengthEncoding* encoding {
        reinterpret_cast< const ServerAcceptance::FixedLengthEncoding* >(
            data + bytes_parsed ) };
    bytes_parsed += sizeof( ServerAcceptance::FixedLengthEncoding );
    // followed by STRING8 vendor
    std::string_view vendor {
        reinterpret_cast< const char* >( data + bytes_parsed ), encoding->vendor_len };
    bytes_parsed += _pad( encoding->vendor_len );
    // followed by LISTofFORMAT pixmap-formats
    _ParsingOutputs pixmap_formats_outputs {
        _parseLISTof< ServerAcceptance::FORMAT >(
            data + bytes_parsed, sz - bytes_parsed,
            encoding->pixmap_formats_ct, ws.nested() ) };
    bytes_parsed += pixmap_formats_outputs.bytes_parsed;
    // followed by LISTofSCREEN roots
    _ParsingOutputs roots_outputs {
        _parseLISTof< ServerAcceptance::SCREEN >(
            data + bytes_parsed, sz - bytes_parsed,
            encoding->roots_ct, ws.nested() ) };
    bytes_parsed += roots_outputs.bytes_parsed;
    assert( header->following_aligned_units ==
            _alignedUnits( bytes_parsed - sizeof( ServerAcceptance::Header ) ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "bitmap-format-scanline-unit" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
        "C{:03d}:{:04d}B:{}: server accepted connection: "
        "{{{}"
        "{}"                 // success
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"                 // total_aligned_units
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"                 // vendor_len
        "{}{: <{}}{}{}{}"
        "{}{}"               // roots_ct, pixmap_formats_ct
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{:?}{}"  // vendor
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        conn->id, bytes_parsed, _SERVER_TO_CLIENT,
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "success", name_width, ws.equals,
            _formatInteger( header->success, ServerAcceptance::success_names ),
            ws.separator ),
        ws.memb_indent, "protocol-major-version", name_width, ws.equals,
        _formatInteger( header->protocol_major_version ), ws.separator,
        ws.memb_indent, "protocol-minor-version", name_width, ws.equals,
        _formatInteger( header->protocol_minor_version ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(message length after header in 4B units)",
            name_width, ws.equals,
            _formatInteger( header->following_aligned_units ), ws.separator ),
        ws.memb_indent, "release-number", name_width, ws.equals,
        _formatInteger( encoding->release_number ), ws.separator,
        ws.memb_indent, "resource-id-base", name_width, ws.equals,
        _formatInteger( encoding->resource_id_base ), ws.separator,
        ws.memb_indent, "resource-id-mask", name_width, ws.equals,
        _formatInteger( encoding->resource_id_mask ), ws.separator,
        ws.memb_indent, "motion-buffer-size", name_width, ws.equals,
        _formatInteger( encoding->motion_buffer_size ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(vendor length in bytes)",
            name_width, ws.equals,
            _formatInteger( encoding->vendor_len ), ws.separator ),
        ws.memb_indent, "maximum-request-length", name_width, ws.equals,
        _formatInteger( encoding->maximum_request_length ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(roots length in SCREENs)",
            name_width, ws.equals,
            _formatInteger( encoding->roots_ct ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(pixmap-formats length in FORMATs)",
            name_width, ws.equals,
            _formatInteger( encoding->pixmap_formats_ct ), ws.separator ),
        ws.memb_indent, "image-byte-order", name_width, ws.equals,
        _formatInteger( encoding->image_byte_order,
                        ServerAcceptance::byte_order_names ), ws.separator,
        ws.memb_indent, "bitmap-format-bit-order", name_width, ws.equals,
        _formatInteger( encoding->bitmap_format_bit_order,
                        ServerAcceptance::bit_order_names ), ws.separator,
        ws.memb_indent, "bitmap-format-scanline-unit", name_width, ws.equals,
        _formatInteger( encoding->bitmap_format_scanline_unit ), ws.separator,
        ws.memb_indent, "bitmap-format-scanline-pad", name_width, ws.equals,
        _formatInteger( encoding->bitmap_format_scanline_pad ), ws.separator,
        ws.memb_indent, "min-keycode", name_width, ws.equals,
        _formatProtocolType( encoding->min_keycode ), ws.separator,
        ws.memb_indent, "max-keycode", name_width, ws.equals,
        _formatProtocolType( encoding->max_keycode ), ws.separator,
        ws.memb_indent, "vendor", name_width, ws.equals,
        vendor, ws.separator,
        ws.memb_indent, "pixmap-formats", name_width, ws.equals,
        pixmap_formats_outputs.str, ws.separator,
        ws.memb_indent, "roots", name_width, ws.equals,
        roots_outputs.str, ws.separator,
        ws.encl_indent
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
    using protocol::connection_setup::ServerResponse;
    switch ( success ) {
    case ServerResponse::FAILED:
        bytes_parsed = _logServerRefusal( conn, data, sz );
        conn->status = Connection::FAILED;
        break;
    case ServerResponse::AUTHENTICATE:
        bytes_parsed = _logServerRequireFurtherAuthentication( conn, data, sz );
        conn->status = Connection::AUTHENTICATION;
        break;
    case ServerResponse::SUCCESS:
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
