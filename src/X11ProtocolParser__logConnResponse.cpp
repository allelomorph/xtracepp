#include <string_view>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/connection_setup.hpp"


size_t X11ProtocolParser::_logConnRefusal(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::connection_setup::ConnRefusal;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( ConnRefusal::Header ) );

    size_t bytes_parsed {};
    const _Whitespace& ws { _ROOT_WS };
    const ConnRefusal::Header* header {
        reinterpret_cast< const ConnRefusal::Header* >( data ) };
    bytes_parsed += sizeof( ConnRefusal::Header );
    assert( header->success ==
            protocol::connection_setup::ConnResponse::FAILED );
    // followed by STRING8 reason
    const std::string_view reason {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        header->reason_len };
    bytes_parsed += _pad( header->reason_len );
    assert( header->following_aligned_units ==
            _alignedUnits( bytes_parsed - sizeof( ConnRefusal::Header ) ) );

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
            _formatInteger( header->success,
                            ConnRefusal::success_names ),
            ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(reason length)", name_width, ws.equals,
            _formatInteger( header->reason_len ), ws.separator ),
        ws.memb_indent, "protocol-major-version", name_width, ws.equals,
        _formatInteger( header->protocol_major_version ), ws.separator,
        ws.memb_indent, "protocol-minor-version", name_width, ws.equals,
        _formatInteger( header->protocol_minor_version ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(4B units after header)",
            name_width, ws.equals,
            _formatInteger( header->following_aligned_units ), ws.separator ),
        ws.memb_indent, "reason", name_width, ws.equals,
        reason, ws.separator,
        ws.encl_indent
        );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logConnRequireFurtherAuthentication(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::connection_setup::ConnRequireFurtherAuthentication;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( ConnRequireFurtherAuthentication::Header ) );

    size_t bytes_parsed {};
    const _Whitespace& ws { _ROOT_WS };
    const ConnRequireFurtherAuthentication::Header* header {
        reinterpret_cast<
        const ConnRequireFurtherAuthentication::Header* >( data ) };
    bytes_parsed += sizeof( ConnRequireFurtherAuthentication::Header );
    assert( header->success ==
            protocol::connection_setup::ConnResponse::AUTHENTICATE );
    // followed by STRING8 reason
    const size_t reason_padded_len {
        header->following_aligned_units * _ALIGN };
    std::string_view reason {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        reason_padded_len };
    bytes_parsed += reason_padded_len;
    assert( header->following_aligned_units ==
            _alignedUnits( bytes_parsed -
                           sizeof( ConnRequireFurtherAuthentication::Header ) ) );

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
            _formatInteger( header->success,
                            ConnRequireFurtherAuthentication::success_names ),
            ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(4B units after header)",
            name_width, ws.equals,
            _formatInteger( header->following_aligned_units ), ws.separator ),
        ws.memb_indent, "reason", name_width, ws.equals,
        reason, ws.separator,
        ws.encl_indent
        );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logConnAcceptance(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::connection_setup::ConnAcceptance;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( ConnAcceptance::Encoding ) );

    size_t bytes_parsed {};
    const _Whitespace& ws { _ROOT_WS };
    const ConnAcceptance::Encoding* encoding {
        reinterpret_cast< const ConnAcceptance::Encoding* >( data ) };
    bytes_parsed += sizeof( ConnAcceptance::Encoding );
    assert( encoding->header.success ==
            protocol::connection_setup::ConnResponse::SUCCESS );
    // followed by STRING8 vendor
    std::string_view vendor {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        encoding->vendor_len };
    bytes_parsed += _pad( encoding->vendor_len );
    // followed by LISTofFORMAT pixmap-formats
    const _ParsingOutputs pixmap_formats {
        _parseLISTof< ConnAcceptance::FORMAT >(
            data + bytes_parsed, sz - bytes_parsed,
            encoding->pixmap_formats_ct, ws.nested(), _Whitespace::SINGLELINE ) };
    bytes_parsed += pixmap_formats.bytes_parsed;
    // followed by LISTofSCREEN roots
    const _ParsingOutputs roots {
        _parseLISTof< ConnAcceptance::SCREEN >(
            data + bytes_parsed, sz - bytes_parsed,
            encoding->roots_ct, ws.nested() ) };
    bytes_parsed += roots.bytes_parsed;
    assert( encoding->header.following_aligned_units == _alignedUnits(
                bytes_parsed - sizeof( ConnAcceptance::Header ) ) );

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
            _formatInteger( encoding->header.success,
                            ConnAcceptance::success_names ),
            ws.separator ),
        ws.memb_indent, "protocol-major-version", name_width, ws.equals,
        _formatInteger( encoding->header.protocol_major_version ), ws.separator,
        ws.memb_indent, "protocol-minor-version", name_width, ws.equals,
        _formatInteger( encoding->header.protocol_minor_version ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(4B units after header)",
            name_width, ws.equals,
            _formatInteger( encoding->header.following_aligned_units ), ws.separator ),
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
            ws.memb_indent, "(vendor length)",
            name_width, ws.equals,
            _formatInteger( encoding->vendor_len ), ws.separator ),
        ws.memb_indent, "maximum-request-length", name_width, ws.equals,
        _formatInteger( encoding->maximum_request_length ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(SCREENs in roots)",
            name_width, ws.equals,
            _formatInteger( encoding->roots_ct ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(FORMATs in pixmap-formats)",
            name_width, ws.equals,
            _formatInteger( encoding->pixmap_formats_ct ), ws.separator ),
        ws.memb_indent, "image-byte-order", name_width, ws.equals,
        _formatInteger( encoding->image_byte_order,
                        ConnAcceptance::image_byte_order_names ), ws.separator,
        ws.memb_indent, "bitmap-format-bit-order", name_width, ws.equals,
        _formatInteger( encoding->bitmap_format_bit_order,
                        ConnAcceptance::bitmap_format_bit_order_names ), ws.separator,
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
        pixmap_formats.str, ws.separator,
        ws.memb_indent, "roots", name_width, ws.equals,
        roots.str, ws.separator,
        ws.encl_indent
        );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logConnResponse(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );

    const uint8_t success { *data };
    size_t bytes_parsed {};
    using protocol::connection_setup::ConnResponse;
    switch ( success ) {
    case ConnResponse::FAILED:
        bytes_parsed = _logConnRefusal( conn, data, sz );
        conn->status = Connection::FAILED;
        break;
    case ConnResponse::AUTHENTICATE:
        bytes_parsed = _logConnRequireFurtherAuthentication( conn, data, sz );
        conn->status = Connection::AUTHENTICATION;
        break;
    case ConnResponse::SUCCESS:
        bytes_parsed = _logConnAcceptance( conn, data, sz );
        conn->status = Connection::OPEN;
        break;
    default:
        break;
    }
    assert( bytes_parsed <= sz );
    return bytes_parsed;
}
