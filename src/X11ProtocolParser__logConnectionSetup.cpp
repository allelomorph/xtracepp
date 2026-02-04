#include <string_view>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/connection_setup.hpp"
#include "protocol/common_types.hpp"  // CARD16
#include "protocol/version.hpp"


template<>
size_t X11ProtocolParser::_logConnectionSetup<
    protocol::connection_setup::Initiation >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::connection_setup::Initiation;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( Initiation::Header ) );

    size_t bytes_parsed {};
    const _Whitespace& ws { _ROOT_WS };
    const Initiation::Header* header {
        reinterpret_cast< const Initiation::Header* >( data ) };
    assert( header->byte_order == Initiation::MSBFIRST ||
            header->byte_order == Initiation::LSBFIRST );
    const bool byteswap { conn->byteswap };
    assert( byteswap ==
            ( little_endian != ( header->byte_order == Initiation::LSBFIRST ) ) );
    bytes_parsed += sizeof( Initiation::Header );
    // followed by STRING8 authorization-protocol-name
    const uint16_t name_len { _ordered( header->name_len, byteswap ) };
    const std::string_view auth_protocol_name {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        name_len };
    bytes_parsed += alignment.pad( name_len );
    // followed by STRING8 authorization-protocol-data
    const uint16_t data_len { _ordered( header->data_len, byteswap ) };
    const _ParsingOutputs authorization_protocol_data {
        _parseLISTof< protocol::CARD8 >(
            data + bytes_parsed, data_len, data_len,
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    bytes_parsed += alignment.pad( data_len );
    assert( bytes_parsed == sz );  // (should not be batched with other messages)

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(authorization-protocol-name length)" ) :
                              sizeof( "authorization-protocol-name" ) ) - 1 );
    fmt::println(
        settings.log_fs,
        "C{:03d}:{:04d}B:{}: client {:?} attempting connection: "
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{}"                       // name_len, data_len
        "{}{: <{}}{}{:?}{}"          // authorization-protocol-name
        "{}{: <{}}{}({:d} bytes){}"  // authorization-protocol-data (hidden)
        "{}}}",
        conn->id, bytes_parsed, CLIENT_TO_SERVER, conn->client_desc,
        ws.separator,
        ws.memb_indent, "byte-order", memb_name_w, ws.equals,
        _formatVariable( uint8_t( header->byte_order ==
                                  Initiation::MSBFIRST ), byteswap,
                         { Initiation::byte_order_names } ), ws.separator,
        ws.memb_indent, "protocol-major-version", memb_name_w, ws.equals,
        _formatVariable( header->protocol_major_version, byteswap ), ws.separator,
        ws.memb_indent, "protocol-minor-version", memb_name_w, ws.equals,
        _formatVariable( header->protocol_minor_version, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(authorization-protocol-name length)",
            memb_name_w, ws.equals,
            _formatVariable( header->name_len, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(authorization-protocol-data length)",
            memb_name_w, ws.equals,
            _formatVariable( header->data_len, byteswap ), ws.separator ),
        ws.memb_indent, "authorization-protocol-name", memb_name_w, ws.equals,
        auth_protocol_name, ws.separator,
        ws.memb_indent, "authorization-protocol-data", memb_name_w, ws.equals,
        header->data_len, ws.separator,
        ws.encl_indent
        );
    return bytes_parsed;
}

template<>
size_t X11ProtocolParser::_logConnectionSetup<
    protocol::connection_setup::Refusal >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::connection_setup::Refusal;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( Refusal::Header ) );

    size_t bytes_parsed {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const Refusal::Header* header {
        reinterpret_cast< const Refusal::Header* >( data ) };
    bytes_parsed += sizeof( Refusal::Header );
    assert( _ordered( header->success, byteswap ) ==
            protocol::connection_setup::InitResponse::FAILED );
    // followed by STRING8 reason
    auto reason_len { _ordered( header->reason_len, byteswap ) };
    const std::string_view reason {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        reason_len };
    bytes_parsed += alignment.pad( reason_len );
    assert( _ordered( header->following_aligned_units, byteswap ) ==
            alignment.units( bytes_parsed - sizeof( Refusal::Header ) ) );

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
                             { Refusal::success_names } ),
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

template<>
size_t X11ProtocolParser::_logConnectionSetup<
    protocol::connection_setup::RequireFurtherAuthentication >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::connection_setup::RequireFurtherAuthentication;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( RequireFurtherAuthentication::Header ) );

    size_t bytes_parsed {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const RequireFurtherAuthentication::Header* header {
        reinterpret_cast<
        const RequireFurtherAuthentication::Header* >( data ) };
    bytes_parsed += sizeof( RequireFurtherAuthentication::Header );
    assert( _ordered( header->success, byteswap ) ==
            protocol::connection_setup::InitResponse::AUTHENTICATE );
    // followed by STRING8 reason
    const auto following_aligned_units {
        _ordered( header->following_aligned_units, byteswap ) };
    const size_t reason_padded_len {
        alignment.size( following_aligned_units ) };
    const std::string_view reason {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        reason_padded_len };
    bytes_parsed += reason_padded_len;
    assert( following_aligned_units ==
            alignment.units( bytes_parsed -
                             sizeof( RequireFurtherAuthentication::Header ) ) );

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
                             { RequireFurtherAuthentication::success_names } ),
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

template<>
size_t X11ProtocolParser::_logConnectionSetup<
    protocol::connection_setup::Acceptance >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::connection_setup::Acceptance;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( Acceptance::Encoding ) );

    size_t bytes_parsed {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const Acceptance::Header* header {
        reinterpret_cast< const Acceptance::Header* >( data ) };
    if ( const protocol::CARD16 major_ver {
            _ordered( header->protocol_major_version, byteswap ) },
                                minor_ver {
            _ordered( header->protocol_minor_version, byteswap ) };
        major_ver != protocol::MAJOR_VERSION ||
        minor_ver != protocol::MINOR_VERSION ) {
        fmt::println( ::stderr, "{}: {}: cannot parse any messages as server will "
                      "encode in X version {}.{} and parser only supports {}.{}",
                      settings.process_name, __PRETTY_FUNCTION__,
                      major_ver, minor_ver,
                      protocol::MAJOR_VERSION, protocol::MINOR_VERSION );
        ::exit( EXIT_FAILURE );
    }
    bytes_parsed += sizeof( Acceptance::Header );
    assert( _ordered( header->success, byteswap ) ==
            protocol::connection_setup::InitResponse::SUCCESS );
    const Acceptance::Encoding* encoding {
        reinterpret_cast< const Acceptance::Encoding* >(
            data + bytes_parsed ) };
    bytes_parsed += sizeof( Acceptance::Encoding );
    // followed by STRING8 vendor
    const uint16_t vendor_len { _ordered( encoding->vendor_len, byteswap ) };
    const std::string_view vendor {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        vendor_len };
    bytes_parsed += alignment.pad( vendor_len );
    // followed by LISTofFORMAT pixmap-formats
    const _ParsingOutputs pixmap_formats {
        _parseLISTof< Acceptance::FORMAT >(
            data + bytes_parsed, sz - bytes_parsed,
            _ordered( encoding->pixmap_formats_ct, byteswap ),
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    bytes_parsed += pixmap_formats.bytes_parsed;
    // followed by LISTofSCREEN roots
    const _ParsingOutputs roots {
        _parseLISTof< Acceptance::SCREEN >(
            data + bytes_parsed, sz - bytes_parsed,
            _ordered( encoding->roots_ct, byteswap ),
            byteswap, ws.nested() ) };
    bytes_parsed += roots.bytes_parsed;
    assert( _ordered( header->following_aligned_units, byteswap ) ==
            alignment.units( bytes_parsed - sizeof( Acceptance::Header ) ) );

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
                             { Acceptance::success_names } ),
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
                         { Acceptance::image_byte_order_names } ), ws.separator,
        ws.memb_indent, "bitmap-format-bit-order", memb_name_w, ws.equals,
        _formatVariable( encoding->bitmap_format_bit_order, byteswap,
                         { Acceptance::bitmap_format_bit_order_names } ), ws.separator,
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

template<>
size_t X11ProtocolParser::_logConnectionSetup<
    protocol::connection_setup::InitResponse >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::connection_setup::InitResponse;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( InitResponse::Header ) );

    size_t bytes_parsed {};
    const InitResponse::Header* header {
        reinterpret_cast< const InitResponse::Header* >( data ) };
    switch ( _ordered( header->success, conn->byteswap ) ) {
    case InitResponse::FAILED:
        bytes_parsed = _logConnectionSetup<
            protocol::connection_setup::Refusal >( conn, data, sz );
        conn->status = Connection::FAILED;
        break;
    case InitResponse::AUTHENTICATE:
        bytes_parsed = _logConnectionSetup<
            protocol::connection_setup::RequireFurtherAuthentication >( conn, data, sz );
        conn->status = Connection::AUTHENTICATION;
        break;
    case InitResponse::SUCCESS:
        bytes_parsed = _logConnectionSetup<
            protocol::connection_setup::Acceptance >( conn, data, sz );
        conn->status = Connection::OPEN;
        break;
    default:
        break;
    }
    assert( bytes_parsed <= sz );
    return bytes_parsed;
}
