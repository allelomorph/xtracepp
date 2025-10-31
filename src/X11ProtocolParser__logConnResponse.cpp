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
    const bool byteswap { conn->byteswap };
    const ConnRefusal::Header* header {
        reinterpret_cast< const ConnRefusal::Header* >( data ) };
    bytes_parsed += sizeof( ConnRefusal::Header );
    assert( _hostByteOrder( header->success, byteswap ) ==
            protocol::connection_setup::ConnResponse::FAILED );
    // followed by STRING8 reason
    auto reason_len { _hostByteOrder( header->reason_len, byteswap ) };
    const std::string_view reason {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        reason_len };
    bytes_parsed += alignment.pad( reason_len );
    assert( _hostByteOrder( header->following_aligned_units, byteswap ) ==
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
            _formatVariable( header->success, byteswap,
                             { ConnRefusal::success_names } ),
            ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(reason length)", memb_name_w, ws.equals,
            _formatVariable( header->reason_len, byteswap ), ws.separator ),
        ws.memb_indent, "protocol-major-version", memb_name_w, ws.equals,
        _formatVariable( header->protocol_major_version, byteswap ), ws.separator,
        ws.memb_indent, "protocol-minor-version", memb_name_w, ws.equals,
        _formatVariable( header->protocol_minor_version, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(post-header aligned units)",
            memb_name_w, ws.equals,
            _formatVariable( header->following_aligned_units,
                             byteswap ), ws.separator ),
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
    const bool byteswap { conn->byteswap };
    const ConnRequireFurtherAuthentication::Header* header {
        reinterpret_cast<
        const ConnRequireFurtherAuthentication::Header* >( data ) };
    bytes_parsed += sizeof( ConnRequireFurtherAuthentication::Header );
    assert( _hostByteOrder( header->success, byteswap ) ==
            protocol::connection_setup::ConnResponse::AUTHENTICATE );
    // followed by STRING8 reason
    const auto following_aligned_units {
        _hostByteOrder( header->following_aligned_units, byteswap ) };
    const size_t reason_padded_len {
        alignment.size( following_aligned_units ) };
    const std::string_view reason {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        reason_padded_len };
    bytes_parsed += reason_padded_len;
    assert( following_aligned_units ==
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
            _formatVariable( header->success, byteswap,
                             { ConnRequireFurtherAuthentication::success_names } ),
            ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(post-header aligned units)",
            memb_name_w, ws.equals,
            _formatVariable( header->following_aligned_units, byteswap ), ws.separator ),
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
    const bool byteswap { conn->byteswap };
    const ConnAcceptance::Header* header {
        reinterpret_cast< const ConnAcceptance::Header* >( data ) };
    bytes_parsed += sizeof( ConnAcceptance::Header );
    assert( _hostByteOrder( header->success, byteswap ) ==
            protocol::connection_setup::ConnResponse::SUCCESS );
    const ConnAcceptance::Encoding* encoding {
        reinterpret_cast< const ConnAcceptance::Encoding* >(
            data + bytes_parsed ) };
    bytes_parsed += sizeof( ConnAcceptance::Encoding );
    // followed by STRING8 vendor
    const uint16_t vendor_len { _hostByteOrder( encoding->vendor_len, byteswap ) };
    const std::string_view vendor {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        vendor_len };
    bytes_parsed += alignment.pad( vendor_len );
    // followed by LISTofFORMAT pixmap-formats
    const _ParsingOutputs pixmap_formats {
        _parseLISTof< ConnAcceptance::FORMAT >(
            data + bytes_parsed, sz - bytes_parsed,
            _hostByteOrder( encoding->pixmap_formats_ct, byteswap ),
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    bytes_parsed += pixmap_formats.bytes_parsed;
    // followed by LISTofSCREEN roots
    const _ParsingOutputs roots {
        _parseLISTof< ConnAcceptance::SCREEN >(
            data + bytes_parsed, sz - bytes_parsed,
            _hostByteOrder( encoding->roots_ct, byteswap ),
            byteswap, ws.nested() ) };
    bytes_parsed += roots.bytes_parsed;
    assert( _hostByteOrder( header->following_aligned_units, byteswap ) ==
            alignment.units( bytes_parsed - sizeof( ConnAcceptance::Header ) ) );

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
            _formatVariable( header->success, byteswap,
                             { ConnAcceptance::success_names } ),
            ws.separator ),
        ws.memb_indent, "protocol-major-version", memb_name_w, ws.equals,
        _formatVariable( header->protocol_major_version, byteswap ), ws.separator,
        ws.memb_indent, "protocol-minor-version", memb_name_w, ws.equals,
        _formatVariable( header->protocol_minor_version, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(post-header aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->following_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "release-number", memb_name_w, ws.equals,
        _formatVariable( encoding->release_number, byteswap ), ws.separator,
        ws.memb_indent, "resource-id-base", memb_name_w, ws.equals,
        _formatVariable( encoding->resource_id_base, byteswap ), ws.separator,
        ws.memb_indent, "resource-id-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->resource_id_mask, byteswap,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "motion-buffer-size", memb_name_w, ws.equals,
        _formatVariable( encoding->motion_buffer_size, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(vendor length)",
            memb_name_w, ws.equals,
            _formatVariable( encoding->vendor_len, byteswap ), ws.separator ),
        ws.memb_indent, "maximum-request-length", memb_name_w, ws.equals,
        _formatVariable( encoding->maximum_request_length, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(SCREENs in roots)",
            memb_name_w, ws.equals,
            _formatVariable( encoding->roots_ct, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(FORMATs in pixmap-formats)",
            memb_name_w, ws.equals,
            _formatVariable( encoding->pixmap_formats_ct, byteswap ), ws.separator ),
        ws.memb_indent, "image-byte-order", memb_name_w, ws.equals,
        _formatVariable( encoding->image_byte_order, byteswap,
                         { ConnAcceptance::image_byte_order_names } ), ws.separator,
        ws.memb_indent, "bitmap-format-bit-order", memb_name_w, ws.equals,
        _formatVariable( encoding->bitmap_format_bit_order, byteswap,
                         { ConnAcceptance::bitmap_format_bit_order_names } ), ws.separator,
        ws.memb_indent, "bitmap-format-scanline-unit", memb_name_w, ws.equals,
        _formatVariable( encoding->bitmap_format_scanline_unit, byteswap ), ws.separator,
        ws.memb_indent, "bitmap-format-scanline-pad", memb_name_w, ws.equals,
        _formatVariable( encoding->bitmap_format_scanline_pad, byteswap ), ws.separator,
        ws.memb_indent, "min-keycode", memb_name_w, ws.equals,
        _formatVariable( encoding->min_keycode, byteswap ), ws.separator,
        ws.memb_indent, "max-keycode", memb_name_w, ws.equals,
        _formatVariable( encoding->max_keycode, byteswap ), ws.separator,
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
    using protocol::connection_setup::ConnResponse;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( ConnResponse::Header ) );

    size_t bytes_parsed {};
    const ConnResponse::Header* header {
        reinterpret_cast< const ConnResponse::Header* >( data ) };
    switch ( _hostByteOrder( header->success, conn->byteswap ) ) {
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
