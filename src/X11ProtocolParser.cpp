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


std::string_view
X11ProtocolParser::_tabIndent( const uint32_t tab_ct ) {
    static constexpr std::string_view WHITESPACE {
        "                                                           "
        "                                                           "
    };
    assert( tab_ct * _TAB_SZ <= WHITESPACE.size() );
    return { WHITESPACE.data(), tab_ct * _TAB_SZ };
}

// TBD for debugging buffer parsing
std::string
X11ProtocolParser::_bufferHexDump( const uint8_t* data, const size_t sz ) {
    assert( data != nullptr );

    static constexpr int BYTES_PER_ROW   { 16 };
    static constexpr int BYTES_PER_GROUP { 8 };

    std::string hex_dump;
    const uint8_t* _data { data };
    for ( size_t bytes_to_parse { sz }, address_index {}; bytes_to_parse > 0;
        address_index += BYTES_PER_ROW ) {
        std::string group1, group2, as_ascii;
        size_t group_sz {
            ( bytes_to_parse < BYTES_PER_GROUP ) ? bytes_to_parse : BYTES_PER_GROUP };
        for ( size_t i {}; i < group_sz;
              ++i, ++_data, --bytes_to_parse ) {
            group1 += fmt::format( "{:02X}{}",
                                   *_data, ( i < group_sz - 1 ) ? " " : "" );
            as_ascii += fmt::format( "{:c}",
                                     ( std::isprint( *_data ) ) ? *_data : '.' );
        }
        group_sz =
                ( bytes_to_parse < BYTES_PER_GROUP ) ? bytes_to_parse : BYTES_PER_GROUP;
        for ( size_t i {}; i < group_sz;
              ++i, ++_data, --bytes_to_parse ) {
            group2 += fmt::format( "{:02X}{}",
                                   *_data, ( i < group_sz - 1 ) ? " " : "" );
            as_ascii += fmt::format( "{:c}",
                                     ( std::isprint( *_data ) ) ? *_data : '.' );
        }
        hex_dump += fmt::format( "{:08x}  {: <23}  {: <23}  {}",
                                 address_index, group1, group2, as_ascii );
    }
    return hex_dump;
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

    const uint32_t tab_ct {};
    const std::string struct_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct ) ) };
    const std::string memb_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct + 1 ) ) };
    const std::string_view equals {
        _verbosity == Settings::Verbosity::Singleline ? "=" : " = " };
    const uint32_t name_width (
        _verbosity == Settings::Verbosity::Singleline ? 0 :
        sizeof( "authorization-protocol-data" ) - 1
        );
    fmt::print( _log_fs, "{:03d}:<:client \"{}\" requesting connection ",
                conn->id, conn->client_desc );
    // TBD may be security concerns with logging auth data
    // std::string auth_data_hex;
    // for ( uint16_t i {}, sz { header->d }; i < sz; ++i ) {
    //     auth_data_hex += fmt::format(
    //         "{}{:02X}", auth_data_hex.empty() ? "" : " ",
    //         auth_protocol_data[i] );
    // }
    fmt::println(
        _log_fs,
        "{{"
        "{}{: <{}}{}{}"
        "{}{}"
        "{}{: <{}}{}\"{}\""
        "{}"
        "{}{: <{}}{}({:d} bytes)"
        "{}}}",
        memb_whtspc, "byte-order", name_width, equals,
        _formatInteger( ( header->byte_order == 'l' ) ? 0 : 1,
                        protocol::enum_names::image_byte_order ),
        _verbosity == Settings::Verbosity::Debug ?
        fmt::format( "{}{: <{}}{}{}{}{: <{}}{}{}",
                     memb_whtspc, "protocol-major-version", name_width, equals,
                     _formatInteger( header->protocol_major_version ),
                     memb_whtspc, "protocol-minor-version", name_width, equals,
                     _formatInteger( header->protocol_minor_version ) ) :
        fmt::format( "{}{: <{}}{}{:d}.{:d}",
                     memb_whtspc, "protocol version", name_width, equals,
                     header->protocol_major_version,
                     header->protocol_minor_version ),
        _verbosity != Settings::Verbosity::Debug ? "" :
        fmt::format( "{}{: <{}}{}{} (length in bytes of authorization-protocol-name)",
            memb_whtspc, "n", name_width, equals,
                     _formatInteger( header->n ) ),
        memb_whtspc, "authorization-protocol-name", name_width, equals,
        auth_protocol_name,
        _verbosity != Settings::Verbosity::Debug ? "" :
        fmt::format( "{}{: <{}}{}{} (length in bytes of authorization-protocol-data)",
                     memb_whtspc, "d", name_width, equals,
                     _formatInteger( header->d ) ),
        // TBD may be security concerns with logging auth data
        memb_whtspc, "authorization-protocol-data", name_width, equals,
        header->d,
        struct_whtspc
        );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerRefusal(
    Connection* conn, const uint8_t* data ) {
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

    const uint32_t tab_ct {};
    const std::string struct_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct ) ) };
    const std::string memb_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct + 1 ) ) };
    const std::string_view equals {
        _verbosity == Settings::Verbosity::Singleline ? "=" : " = " };
    const uint32_t name_width (
        _verbosity == Settings::Verbosity::Singleline ? 0 :
        sizeof( "protocol-major-version" ) - 1
        );
    fmt::print( _log_fs, "{:03d}:>:server refused connection ",
                conn->id );
    fmt::println(
        _log_fs,
        "{{"
        "{}{}{}{}"
        "{}{: <{}}{}\"{}\""
        "{}}}",
        _verbosity != Settings::Verbosity::Debug ? "" :
        fmt::format( "{}{: <{}}{}{} (status: failed)",
                     memb_whtspc, "success", name_width, equals,
                     _formatInteger( header->success ) ),
        _verbosity != Settings::Verbosity::Debug ? "" :
        fmt::format( "{}{: <{}}{}{} (length of reason in bytes)",
                     memb_whtspc, "n", name_width, equals,
                     _formatInteger( header->n ) ),
        _verbosity == Settings::Verbosity::Debug ?
        fmt::format( "{}{: <{}}{}{}{}{: <{}}{}{}",
                     memb_whtspc, "protocol-major-version", name_width, equals,
                     _formatInteger( header->protocol_major_version ),
                     memb_whtspc, "protocol-minor-version", name_width, equals,
                     _formatInteger( header->protocol_minor_version ) ) :
        fmt::format( "{}{: <{}}{}{:d}.{:d}",
                     memb_whtspc, "protocol version", name_width, equals,
                     header->protocol_major_version,
                     header->protocol_minor_version ),
        _verbosity != Settings::Verbosity::Debug ? "" :
        fmt::format( "{}{: <{}}{}{} (padded length of reason in 4 byte units)",
                     memb_whtspc, "reason aligned units", name_width, equals,
                     _formatInteger( header->reason_aligned_units ) ),
        memb_whtspc, "reason", name_width, equals,
        reason,
        struct_whtspc
        );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerRequireFurtherAuthentication(
    Connection* conn, const uint8_t* data ) {
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

    const uint32_t tab_ct {};
    const std::string struct_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct ) ) };
    const std::string memb_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct + 1 ) ) };
    const std::string_view equals {
        _verbosity == Settings::Verbosity::Singleline ? "=" : " = " };
    const uint32_t name_width (
        _verbosity == Settings::Verbosity::Singleline ? 0 :
        sizeof( "success" ) - 1
        );
    fmt::print( _log_fs, "{:03d}:>:server requested further authentication ",
                conn->id );
    fmt::println(
        _log_fs,
        "{{"
        "{}{}"
        "{}{: <{}}{}\"{}\""
        "{}}}",
        _verbosity != Settings::Verbosity::Debug ? "" :
        fmt::format( "{}{: <{}}{}{} (status: authentication)",
                     memb_whtspc, "success", name_width, equals,
                     _formatInteger( header->success ) ),
        _verbosity != Settings::Verbosity::Debug ? "" :
        fmt::format( "{}{: <{}}{}{} (padded length of reason in 4 byte units)",
                     memb_whtspc, "reason aligned units", name_width, equals,
                     _formatInteger( header->reason_aligned_units ) ),
        memb_whtspc, "reason", name_width, equals,
        reason,
        struct_whtspc
        );
    return bytes_parsed;
}

X11ProtocolParser::_LISTParsingOutputs
X11ProtocolParser::_parseLISTofFORMAT(
    const uint8_t* data, const uint32_t format_ct, const uint32_t tab_ct ) {
    assert( data != nullptr );
    assert( tab_ct > 0 );

    X11ProtocolParser::_LISTParsingOutputs outputs;
    using namespace protocol::connection_setup;
    const ServerAcceptance::Format* pixmap_formats {
        reinterpret_cast< const ServerAcceptance::Format* >( data ) };
    const std::string list_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct ) ) };
    // const std::string memb_whtspc {
    //     _verbosity == Settings::Verbosity::Singleline ? " " :
    //     fmt::format( "\n{}", _tabIndent( tab_ct + 1 ) ) };
    // const std::string_view equals {
    //     _verbosity == Settings::Verbosity::Singleline ? "=" : " = " };
    // const uint32_t name_width (
    //     _verbosity == Settings::Verbosity::Singleline ? 0 :
    //     sizeof( "bits-per-pixel" ) - 1 );
    // TBD for now FORMAT struct is single line regardless of verbosity
    outputs.str += '[';
    for ( uint32_t i {}; i < format_ct; ++i ) {
        outputs.str += fmt::format(
            "{}{{ depth={} bits-per-pixel={} scanline-pad={} }}{}",
            list_whtspc,
            _formatInteger( pixmap_formats[i].depth ),
            _formatInteger( pixmap_formats[i].bits_per_pixel ),
            _formatInteger( pixmap_formats[i].scanline_pad ),
            i == format_ct - 1 ? "" : ","
            );
        outputs.bytes_parsed += sizeof( ServerAcceptance::Format );
    }
    if ( format_ct > 0 ) {
        outputs.str +=
            _verbosity == Settings::Verbosity::Singleline ? " " :
            fmt::format( "\n{}", _tabIndent( tab_ct - 1 ) );
    }
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_LISTParsingOutputs
X11ProtocolParser::_parseLISTofSCREEN(
    const uint8_t* data, const uint32_t screen_ct, const uint32_t tab_ct ) {
    assert( data != nullptr );
    assert( tab_ct > 0 );

    X11ProtocolParser::_LISTParsingOutputs outputs;
    const std::string list_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct ) ) };
    const std::string memb_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct + 1 ) ) };
    const std::string_view equals {
        _verbosity == Settings::Verbosity::Singleline ? "=" : " = " };
    const uint32_t name_width (
        _verbosity == Settings::Verbosity::Singleline ? 0 :
        sizeof( "height-in-millimeters" ) - 1
        );
    using namespace protocol::connection_setup;

    outputs.str += '[';
    for ( uint32_t i {}; i < screen_ct; ++i ) {
        const ServerAcceptance::Screen::Header* header {
            reinterpret_cast< const ServerAcceptance::Screen::Header* >(
                data + outputs.bytes_parsed ) };
        outputs.bytes_parsed += sizeof( ServerAcceptance::Screen::Header );

        // followed by LISTofDEPTH allowed-depths of n bytes (n is always a multiple of 4)
        _LISTParsingOutputs allowed_depths_outputs {
            _parseLISTofDEPTH( data + outputs.bytes_parsed, header->d, tab_ct + 2 ) };
        outputs.bytes_parsed += allowed_depths_outputs.bytes_parsed;

        outputs.str += fmt::format(
            "{}{{"
            "{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}"
            "{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}"
            "{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}"
            "{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}"
            "{}}}{}",
            list_whtspc,
            memb_whtspc, "root", name_width, equals,
            _formatCommonType( header->root ),
            memb_whtspc, "default-colormap", name_width, equals,
            _formatCommonType( header->default_colormap ),
            memb_whtspc, "white-pixel", name_width, equals,
            _formatInteger( header->white_pixel ),
            memb_whtspc, "black-pixel", name_width, equals,
            _formatInteger( header->black_pixel ),
            memb_whtspc, "current-input-masks", name_width, equals,
            _formatCommonType( header->current_input_masks ),
            memb_whtspc, "width-in-pixels", name_width, equals,
            _formatInteger( header->width_in_pixels ),
            memb_whtspc, "height-in-pixels", name_width, equals,
            _formatInteger( header->height_in_pixels ),
            memb_whtspc, "width-in-millimeters", name_width, equals,
            _formatInteger( header->width_in_millimeters ),
            memb_whtspc, "height-in-millimeters", name_width, equals,
            _formatInteger( header->height_in_millimeters ),
            memb_whtspc, "min-installed-maps", name_width, equals,
            _formatInteger( header->min_installed_maps ),
            memb_whtspc, "max-installed-maps", name_width, equals,
            _formatInteger( header->max_installed_maps ),
            memb_whtspc, "root-visual", name_width, equals,
            _formatCommonType( header->root_visual ),
            memb_whtspc, "backing-stores", name_width, equals,
            _formatInteger( header->backing_stores,
                            ServerAcceptance::Screen::backing_stores_names ),
            memb_whtspc, "save-unders", name_width, equals,
            _formatCommonType( header->save_unders ),
            memb_whtspc, "root-depth", name_width, equals,
            _formatInteger( header->root_depth ),
            memb_whtspc, "allowed-depths", name_width, equals,
            allowed_depths_outputs.str,
            list_whtspc, i == screen_ct - 1 ? "" : ","
            );
    }
    if ( screen_ct > 0 ) {
        outputs.str +=
            _verbosity == Settings::Verbosity::Singleline ? " " :
            fmt::format( "\n{}", _tabIndent( tab_ct - 1 ) );
    }
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_LISTParsingOutputs
X11ProtocolParser::_parseLISTofDEPTH(
    const uint8_t* data, const uint32_t depth_ct, const uint32_t tab_ct ) {
    assert( data != nullptr );
    assert( tab_ct > 0 );

    X11ProtocolParser::_LISTParsingOutputs outputs;
    const std::string list_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct ) ) };
    const std::string memb_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct + 1 ) ) };
    const std::string_view equals {
        _verbosity == Settings::Verbosity::Singleline ? "=" : " = " };
    const uint32_t name_width (
        _verbosity == Settings::Verbosity::Singleline ? 0 :
        sizeof( "visuals" ) - 1
        );
    using namespace protocol::connection_setup;

    outputs.str += '[';
    for ( uint32_t i {}; i < depth_ct; ++i ) {
        const ServerAcceptance::Screen::Depth::Header* header {
            reinterpret_cast< const ServerAcceptance::Screen::Depth::Header* >(
                data + outputs.bytes_parsed ) };
        outputs.bytes_parsed += sizeof( ServerAcceptance::Screen::Depth::Header );

        // followed by LISTofVISUALTYPE visuals of n * sizeof(VISUALTYPE) bytes
        _LISTParsingOutputs visuals_outputs {
            _parseLISTofVISUALTYPE( data + outputs.bytes_parsed, header->n, tab_ct + 2 ) };
        outputs.bytes_parsed += visuals_outputs.bytes_parsed;

        outputs.str += fmt::format(
            "{}{{"
            "{}{: <{}}{}{}"
            "{}"
            "{}{: <{}}{}{}"
            "{}}}{}",
            list_whtspc,
            memb_whtspc, "depth", name_width, equals,
            _formatInteger( header->depth ),
            _verbosity != Settings::Verbosity::Debug ? "" :
            fmt::format( "{}{: <{}}{}{}",
                         memb_whtspc, "n", name_width, equals,
                         _formatInteger( header->n ) ),
            memb_whtspc, "visuals", name_width, equals,
            visuals_outputs.str,
            list_whtspc, i == depth_ct - 1 ? "" : ","
            );
    }
    if ( depth_ct > 0 ) {
        outputs.str +=
            _verbosity == Settings::Verbosity::Singleline ? " " :
            fmt::format( "\n{}", _tabIndent( tab_ct - 1 ) );
    }
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_LISTParsingOutputs
X11ProtocolParser::_parseLISTofVISUALTYPE(
    const uint8_t* data, const uint32_t vt_ct, const uint32_t tab_ct ) {
    assert( data != nullptr );
    assert( tab_ct > 0 );

    X11ProtocolParser::_LISTParsingOutputs outputs;
    const std::string list_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct ) ) };
    // const std::string memb_whtspc {
    //     _verbosity == Settings::Verbosity::Singleline ? " " :
    //     fmt::format( "\n{}", _tabIndent( tab_ct + 1 ) ) };
    // const std::string_view equals {
    //     _verbosity == Settings::Verbosity::Singleline ? "=" : " = " };
    // const uint32_t name_width (
    //     _verbosity == Settings::Verbosity::Singleline ? 0 :
    //     sizeof( "bits-per-rgb-value" ) - 1
    //     );
    using namespace protocol::connection_setup;

    outputs.str += '[';
    for ( uint32_t i {}; i < vt_ct; ++i ) {
        const ServerAcceptance::Screen::Depth::VisualType::Encoding* visualtype {
            reinterpret_cast< const ServerAcceptance::Screen::Depth::VisualType::Encoding* >(
                data + outputs.bytes_parsed ) };
        outputs.bytes_parsed += sizeof( ServerAcceptance::Screen::Depth::VisualType::Encoding );

        // TBD for now VISUALTYPE struct is single line regardless of verbosity
        outputs.str += fmt::format(
            "{}{{ {}={} {}={} {}={} {}={} {}={} {}={} {}={} }}{}",
            list_whtspc,
            "visual-id",
            _formatCommonType( visualtype->visual_id ),
            "class",
            _formatInteger( visualtype->class_,
                            ServerAcceptance::Screen::Depth::VisualType::class_names ),
            "bits-per-rgb-value",
            _formatInteger( visualtype->bits_per_rgb_value ),
            "colormap-entries",
            _formatInteger( visualtype->colormap_entries ),
            "red-mask",
            _formatBitmask( visualtype->red_mask ),
            "green-mask",
            _formatBitmask( visualtype->green_mask ),
            "blue-mask",
            _formatBitmask( visualtype->blue_mask ),
            i == vt_ct - 1 ? "" : ","
            );
        // outputs.str += fmt::format(
        //     "{}{{"
        //     "{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}"
        //     "{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}"
        //     "{}}}{}",
        //     list_whtspc,
        //     memb_whtspc, "visual-id", name_width, equals,
        //     _formatCommonType( visualtype->visual_id ),
        //     memb_whtspc, "class", name_width, equals,
        //     _formatInteger( visualtype->class_,
        //                     ServerAcceptance::Screen::Depth::VisualType::class_names ),
        //     memb_whtspc, "bits-per-rgb-value", name_width, equals,
        //     _formatInteger( visualtype->bits_per_rgb_value ),
        //     memb_whtspc, "colormap-entries", name_width, equals,
        //     _formatInteger( visualtype->colormap_entries ),
        //     memb_whtspc, "red-mask", name_width, equals,
        //     _formatBitmask( visualtype->red_mask ),
        //     memb_whtspc, "green-mask", name_width, equals,
        //     _formatBitmask( visualtype->green_mask ),
        //     memb_whtspc, "blue-mask", name_width, equals,
        //     _formatBitmask( visualtype->blue_mask ),
        //     list_whtspc, i == vt_ct - 1 ? "" : ","
        //     );
    }
    if ( vt_ct > 0 ) {
        outputs.str +=
            _verbosity == Settings::Verbosity::Singleline ? " " :
            fmt::format( "\n{}", _tabIndent( tab_ct - 1 ) );
    }
    outputs.str += ']';
    return outputs;
}


size_t X11ProtocolParser::_logServerAcceptance(
    Connection* conn, const uint8_t* data ) {
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

    const uint32_t tab_ct {};
    // followed by LISTofFORMAT pixmap-formats of n * sizeof(FORMAT) bytes
    _LISTParsingOutputs pixmap_formats_outputs {
        _parseLISTofFORMAT( data + bytes_parsed, header->n, tab_ct + 2 ) };
    bytes_parsed += pixmap_formats_outputs.bytes_parsed;

    // followed by LISTofSCREEN roots of m bytes (m is always a multiple of 4)
    _LISTParsingOutputs roots_outputs {
        _parseLISTofSCREEN( data + bytes_parsed, header->r, tab_ct + 2 ) };
    bytes_parsed += roots_outputs.bytes_parsed;

    const std::string struct_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct ) ) };
    const std::string memb_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct + 1 ) ) };
    const std::string_view equals {
        _verbosity == Settings::Verbosity::Singleline ? "=" : " = " };
    const uint32_t name_width (
        _verbosity == Settings::Verbosity::Singleline ? 0 :
        sizeof( "bitmap-format-scanline-unit" ) - 1
        );

    fmt::print( _log_fs, "{:03d}:>:server accepted connection ",
                conn->id );
    fmt::println(
        _log_fs,
        "{{"
        "{}{}{}"
        "{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}"
        "{}"
        "{}{: <{}}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}{}"
        "{}{: <{}}{}{}{}{: <{}}{}{}{}{: <{}}{}\"{}\""
        "{}{: <{}}{}{}{}{: <{}}{}{}"
        "{}}}",
        _verbosity != Settings::Verbosity::Debug ? "" :
        fmt::format( "{}{: <{}}{}{} (status: accepted)",
                     memb_whtspc, "success", name_width, equals,
                     _formatInteger( header->success ) ),
        _verbosity == Settings::Verbosity::Debug ?
        fmt::format( "{}{: <{}}{}{}{}{: <{}}{}{}",
                     memb_whtspc, "protocol-major-version", name_width, equals,
                     _formatInteger( header->protocol_major_version ),
                     memb_whtspc, "protocol-minor-version", name_width, equals,
                     _formatInteger( header->protocol_minor_version ) ) :
        fmt::format( "{}{: <{}}{}{:d}.{:d}",
                     memb_whtspc, "protocol version", name_width, equals,
                     header->protocol_major_version,
                     header->protocol_minor_version ),
        _verbosity != Settings::Verbosity::Debug ? "" :
        fmt::format(
            "{}{: <{}}{}{} (length of (padded) vendor + pixmap-formats + roots, in 4-byte units)",
            memb_whtspc, "ad", name_width, equals,
            _formatInteger( header->ad ) ),
        memb_whtspc, "release-number", name_width, equals,
        _formatInteger( header->release_number ),
        memb_whtspc, "release-id-base", name_width, equals,
        _formatInteger( header->release_id_base ),
        memb_whtspc, "release-id-mask", name_width, equals,
        _formatInteger( header->release_id_mask ),
        memb_whtspc, "motion-buffer-size", name_width, equals,
        _formatInteger( header->motion_buffer_size ),
        _verbosity != Settings::Verbosity::Debug ? "" :
        fmt::format( "{}{: <{}}{}{} (length of vendor in bytes)",
                     memb_whtspc, "v", name_width, equals,
                     _formatInteger( header->v ) ),
        memb_whtspc, "maximum-request-length", name_width, equals,
        _formatInteger( header->maximum_request_length ),
        _verbosity != Settings::Verbosity::Debug ? "" :
        fmt::format( "{}{: <{}}{}{} (number of SCREENs in roots)",
                     memb_whtspc, "r", name_width, equals,
                     _formatInteger( header->r ) ),
        _verbosity != Settings::Verbosity::Debug ? "" :
        fmt::format( "{}{: <{}}{}{} (number of FORMATs in pixmap-formats)",
                     memb_whtspc, "n", name_width, equals,
                     _formatInteger( header->n ) ),
        memb_whtspc, "image-byte-order", name_width, equals,
        _formatInteger( header->image_byte_order,
                        ServerAcceptance::byte_order_names ),
        memb_whtspc, "bitmap-format-bit-order", name_width, equals,
        _formatInteger( header->bitmap_format_bit_order,
                        ServerAcceptance::bit_order_names ),
        memb_whtspc, "bitmap-format-scanline-unit", name_width, equals,
        _formatInteger( header->bitmap_format_scanline_unit ),
        memb_whtspc, "bitmap-format-scanline-pad", name_width, equals,
        _formatInteger( header->bitmap_format_scanline_pad ),
        memb_whtspc, "min-keycode", name_width, equals,
        _formatCommonType( header->min_keycode ),
        memb_whtspc, "max-keycode", name_width, equals,
        _formatCommonType( header->max_keycode ),
        memb_whtspc, "vendor", name_width, equals,
        vendor,
        memb_whtspc, "pixmap-formats", name_width, equals,
        pixmap_formats_outputs.str,
        memb_whtspc, "roots", name_width, equals,
        roots_outputs.str,
        struct_whtspc
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
        bytes_parsed = _logServerRefusal( conn, data );
        conn->status = Connection::FAILED;
        break;
    case protocol::connection_setup::AUTHENTICATE:
        bytes_parsed = _logServerRequireFurtherAuthentication( conn, data );
        conn->status = Connection::AUTHENTICATION;
        break;
    case protocol::connection_setup::SUCCESS:
        bytes_parsed = _logServerAcceptance( conn, data );
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
        assert( *data != 0 );
//        const uint8_t first_byte { *data };
        // TBD use enum instead of magic values
        bytes_parsed = _logClientRequest( conn, data, sz, *data );
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
        // TBD use enum instead of magic values
        assert( first_byte <= 35 );
        switch ( first_byte ) {
        case 0:   // Error
            return sz;  // TBD deactivated for testing
            // log error func distinguishing by `code` byte
            //bytes_parsed = _logServerError( conn, data, sz );
            // assert( bytes_parsed == 32 );
            break;
        case 1:   // Reply
            return sz;  // TBD deactivated for testing
            // log reply func distingusishing by using provided sequence number to look up request opcode,
            //   then call func based on opcode
            // TBD modification of QueryExtension replies should happen here to filter extensions
            //bytes_parsed = _logServerReply( conn, data, sz );
            // assert( bytes_parsed >= 32 );
            break;
        default:  // Event (2-35)
            return sz;  // TBD deactivated for testing
            //bytes_parsed = _logServerEvent( conn, data, sz, first_byte );
            // assert( bytes_parsed == 32 );
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

size_t X11ProtocolParser::_logServerError(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= 32 );  // TBD

    // using namespace protocol::errors;
    // const Error* error {
    //     reinterpret_cast< Error* >( data ) };
    return sz;
}

size_t X11ProtocolParser::_logServerReply(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= 32 ); // TBD some extension replies may be smaller, eg BigReqEnable

    return sz;
}

size_t X11ProtocolParser::_logServerEvent(
    Connection* conn, uint8_t* data, const size_t sz,
    const uint8_t code ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= 32 ); // TBD

    return sz;
}

void X11ProtocolParser::setLogFileStream( FILE* log_fs ) {
    assert( log_fs != nullptr );
    assert( !feof( log_fs ) && !ferror( log_fs ) );
    _log_fs = log_fs;
}

// TBD pass in readwritedebug or keep as private parser var like verbosity?
// client packets begin with:
//   - first contact: 'B' or 'l' (66 or 108)
//   - requests: opcode of 1-119, 127
//   - extension enable: ??? (logs of xtrace indicate packets sent after
//       QueryExtension gets positive response from server, to then enable
//       that extension)
size_t X11ProtocolParser::logClientPackets( Connection* conn,
                                            const Settings& settings ) {
    assert( conn != nullptr );

    uint8_t* data { conn->client_buffer.data() };
    size_t   tl_bytes_parsed {};
    for ( const size_t bytes_to_parse { conn->client_buffer.size() };
          tl_bytes_parsed < bytes_to_parse; ) {
        size_t bytes_parsed {
            _logClientPacket( conn, data, bytes_to_parse - tl_bytes_parsed ) };
        if ( settings.readwritedebug ) {
            fmt::println( _log_fs, "{:03d}:<:parsed   {:4d} bytes",
                          conn->id, bytes_parsed );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    assert( tl_bytes_parsed == conn->client_buffer.size() );
    return tl_bytes_parsed;
}

size_t X11ProtocolParser::logServerPackets( Connection* conn,
                                            const Settings& settings ) {
    assert( conn != nullptr );

    uint8_t* data { conn->server_buffer.data() };
    size_t   tl_bytes_parsed {};
    for ( const size_t bytes_to_parse { conn->server_buffer.size() };
          tl_bytes_parsed < bytes_to_parse; ) {
        size_t bytes_parsed {
            _logServerPacket( conn, data, bytes_to_parse - tl_bytes_parsed ) };
        if ( settings.readwritedebug ) {
            fmt::println( _log_fs, "{:03d}:>:parsed   {:4d} bytes",
                          conn->id, bytes_parsed );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    assert( tl_bytes_parsed == conn->server_buffer.size() );
    return tl_bytes_parsed;
}
