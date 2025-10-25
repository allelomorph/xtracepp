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
    bytes_parsed += alignment.pad( header->reason_len );
    assert( header->following_aligned_units ==
            alignment.units( bytes_parsed - sizeof( ConnRefusal::Header ) ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(post-header aligned units)" ) :
                              sizeof( "protocol-major-version" ) ) - 1 );
    fmt::println(
        settings.log_fs,
        "C{:03d}:{:04d}B:{}: server refused connection: "
        "{{{}"
        "{}{}"               // success, reason_len
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"                 // total_aligned_units
        "{}{: <{}}{}{:?}{}"  // reason
        "{}}}",
        conn->id, bytes_parsed, SERVER_TO_CLIENT,
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "success", memb_name_w, ws.equals,
            _formatVariable( header->success,
                             { ConnRefusal::success_names } ),
            ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(reason length)", memb_name_w, ws.equals,
            _formatVariable( header->reason_len ), ws.separator ),
        ws.memb_indent, "protocol-major-version", memb_name_w, ws.equals,
        _formatVariable( header->protocol_major_version ), ws.separator,
        ws.memb_indent, "protocol-minor-version", memb_name_w, ws.equals,
        _formatVariable( header->protocol_minor_version ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(post-header aligned units)",
            memb_name_w, ws.equals,
            _formatVariable( header->following_aligned_units ), ws.separator ),
        ws.memb_indent, "reason", memb_name_w, ws.equals,
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
        alignment.size( header->following_aligned_units ) };
    const std::string_view reason {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        reason_padded_len };
    bytes_parsed += reason_padded_len;
    assert( header->following_aligned_units ==
            alignment.units( bytes_parsed -
                             sizeof( ConnRequireFurtherAuthentication::Header ) ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(post-header aligned units)" ) :
                              sizeof( "success" ) ) - 1 );
    fmt::println(
        settings.log_fs,
        "C{:03d}:{:04d}B:{}: server requested further authentication: "
        "{{{}"
        "{}{}"               // success, total_aligned_units
        "{}{: <{}}{}{:?}{}"  // reason
        "{}}}",
        conn->id, bytes_parsed, SERVER_TO_CLIENT,
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "success", memb_name_w, ws.equals,
            _formatVariable( header->success,
                             { ConnRequireFurtherAuthentication::success_names } ),
            ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(post-header aligned units)",
            memb_name_w, ws.equals,
            _formatVariable( header->following_aligned_units ), ws.separator ),
        ws.memb_indent, "reason", memb_name_w, ws.equals,
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
    const ConnAcceptance::Header* header {
        reinterpret_cast< const ConnAcceptance::Header* >( data ) };
    bytes_parsed += sizeof( ConnAcceptance::Header );
    assert( header->success ==
            protocol::connection_setup::ConnResponse::SUCCESS );
    const ConnAcceptance::Encoding* encoding {
        reinterpret_cast< const ConnAcceptance::Encoding* >(
            data + bytes_parsed ) };
    bytes_parsed += sizeof( ConnAcceptance::Encoding );
    // followed by STRING8 vendor
    const std::string_view vendor {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        encoding->vendor_len };
    bytes_parsed += alignment.pad( encoding->vendor_len );
    // followed by LISTofFORMAT pixmap-formats
    const _ParsingOutputs pixmap_formats {
        _parseLISTof< ConnAcceptance::FORMAT >(
            data + bytes_parsed, sz - bytes_parsed,
            encoding->pixmap_formats_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    bytes_parsed += pixmap_formats.bytes_parsed;
    // followed by LISTofSCREEN roots
    const _ParsingOutputs roots {
        _parseLISTof< ConnAcceptance::SCREEN >(
            data + bytes_parsed, sz - bytes_parsed,
            encoding->roots_ct, ws.nested() ) };
    bytes_parsed += roots.bytes_parsed;
    assert( header->following_aligned_units == alignment.units(
                bytes_parsed - sizeof( ConnAcceptance::Header ) ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(post-header aligned units)" ) :
                              sizeof( "bitmap-format-scanline-unit" ) ) - 1 );
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
        conn->id, bytes_parsed, SERVER_TO_CLIENT,
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "success", memb_name_w, ws.equals,
            _formatVariable( header->success,
                             { ConnAcceptance::success_names } ),
            ws.separator ),
        ws.memb_indent, "protocol-major-version", memb_name_w, ws.equals,
        _formatVariable( header->protocol_major_version ), ws.separator,
        ws.memb_indent, "protocol-minor-version", memb_name_w, ws.equals,
        _formatVariable( header->protocol_minor_version ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(post-header aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->following_aligned_units ), ws.separator ),
        ws.memb_indent, "release-number", memb_name_w, ws.equals,
        _formatVariable( encoding->release_number ), ws.separator,
        ws.memb_indent, "resource-id-base", memb_name_w, ws.equals,
        _formatVariable( encoding->resource_id_base ), ws.separator,
        ws.memb_indent, "resource-id-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->resource_id_mask ), ws.separator,
        ws.memb_indent, "motion-buffer-size", memb_name_w, ws.equals,
        _formatVariable( encoding->motion_buffer_size ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(vendor length)",
            memb_name_w, ws.equals,
            _formatVariable( encoding->vendor_len ), ws.separator ),
        ws.memb_indent, "maximum-request-length", memb_name_w, ws.equals,
        _formatVariable( encoding->maximum_request_length ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(SCREENs in roots)",
            memb_name_w, ws.equals,
            _formatVariable( encoding->roots_ct ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(FORMATs in pixmap-formats)",
            memb_name_w, ws.equals,
            _formatVariable( encoding->pixmap_formats_ct ), ws.separator ),
        ws.memb_indent, "image-byte-order", memb_name_w, ws.equals,
        _formatVariable( encoding->image_byte_order,
                         { ConnAcceptance::image_byte_order_names } ), ws.separator,
        ws.memb_indent, "bitmap-format-bit-order", memb_name_w, ws.equals,
        _formatVariable( encoding->bitmap_format_bit_order,
                         { ConnAcceptance::bitmap_format_bit_order_names } ), ws.separator,
        ws.memb_indent, "bitmap-format-scanline-unit", memb_name_w, ws.equals,
        _formatVariable( encoding->bitmap_format_scanline_unit ), ws.separator,
        ws.memb_indent, "bitmap-format-scanline-pad", memb_name_w, ws.equals,
        _formatVariable( encoding->bitmap_format_scanline_pad ), ws.separator,
        ws.memb_indent, "min-keycode", memb_name_w, ws.equals,
        _formatVariable( encoding->min_keycode ), ws.separator,
        ws.memb_indent, "max-keycode", memb_name_w, ws.equals,
        _formatVariable( encoding->max_keycode ), ws.separator,
        ws.memb_indent, "vendor", memb_name_w, ws.equals,
        vendor, ws.separator,
        ws.memb_indent, "pixmap-formats", memb_name_w, ws.equals,
        pixmap_formats.str, ws.separator,
        ws.memb_indent, "roots", memb_name_w, ws.equals,
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
