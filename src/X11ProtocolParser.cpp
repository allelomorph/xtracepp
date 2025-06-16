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

    const uint8_t* _data { data };
    size_t bytes_parsed {};
    using namespace protocol::connection_setup;
    const ClientInitiation::Header* header {
        reinterpret_cast< const ClientInitiation::Header* >( _data )
    };
    // TBD error if protocol is not 11.0
    _data += sizeof( ClientInitiation::Header );
    bytes_parsed += sizeof( ClientInitiation::Header );
    // STRING8 of d bytes authorization-protocol-name
    const std::string_view auth_protocol_name {
        reinterpret_cast< const char* >( _data ), header->n };
    _data += _pad( header->n );
    bytes_parsed += _pad( header->n );
    // STRING8 of d bytes authorization-protocol-data (may not be printable)
    const uint8_t* auth_protocol_data { _data };
    _data += _pad( header->d );
    bytes_parsed += _pad( header->d );
    assert( bytes_parsed == sz );

    // TBD asserts close log file without logging anything
    assert( header->byte_order == 'B' || header->byte_order == 'l' );
    static const uint32_t tab_ct { 1 };
    const std::string_view indent { _tabIndent( tab_ct ) };
    fmt::print( _log_fs, "{:03d}:<:client \"{}\" requesting connection ",
                conn->id, conn->client_desc );
    // 000:<: am lsb-first want 11:0 authorising with 'MIT-MAGIC-COOKIE-1' of length 16
    if ( _verbosity == Settings::Verbosity::Singleline ) {
        fmt::println( _log_fs,
                      "{{ byte order={}, protocol version={:d}.{:d}, "
                      "auth protocol=\"{}\", auth data={:d} bytes }}",
                      _formatInteger( ( header->byte_order == 'l' ) ? 0 : 1,
                                      protocol::enum_names::image_byte_order ),
                      header->protocol_major_version,
                      header->protocol_minor_version,
                      auth_protocol_name,
                      header->d
            );
    } else {
        std::string auth_data_hex;
        if ( _verbosity == Settings::Verbosity::Debug ) {
            for ( uint16_t i {}, sz { header->d }; i < sz; ++i ) {
                auth_data_hex += fmt::format(
                    "{}{:02X}", auth_data_hex.empty() ? "" : " ",
                    auth_protocol_data[i] );
            }
        }
        fmt::print( _log_fs, R"({{
{}byte-order       = {}{}
{}protocol version = {:d}.{:d}
{}auth protocol    = "{}"
{}auth data        = {} bytes{}
{}}}
)",
                    indent,
                    protocol::enum_names::image_byte_order[
                        header->byte_order == 'l' ? 0 : 1 ],
                    _verbosity != Settings::Verbosity::Debug ? "" :
                    fmt::format( " ({})", _formatInteger( header->byte_order ) ),
                    indent, header->protocol_major_version,
                    header->protocol_minor_version,
                    indent, auth_protocol_name,
                    indent, header->d, auth_data_hex.empty() ? "" :
                    fmt::format( " [\n{}\n{}]", auth_data_hex, indent ),
                    _tabIndent( tab_ct - 1 )
            );
    }
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerRefusal(
    Connection* conn, const uint8_t* data ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    const uint8_t* data_ { data };
    size_t bytes_parsed {};
    using namespace protocol::connection_setup;
    const ServerRefusal::Header* header {
        reinterpret_cast< const ServerRefusal::Header* >( data_ ) };
    data_ += sizeof( ServerRefusal::Header );
    bytes_parsed += sizeof( ServerRefusal::Header );
    assert( header->reason_aligned_units == _pad( header->n ) / _ALIGN );
    // STRING8 of n bytes reason
    std::string_view reason {
        reinterpret_cast< const char* >( data_ ), header->n };
    data_ += _pad( header->n );
    bytes_parsed += _pad( header->n );
//    assert( bytes_parsed == sz );

    fmt::print( _log_fs, "{:03d}:>:server refused connection ",
                conn->id );
    if ( _verbosity == Settings::Verbosity::Singleline ) {
        fmt::println( _log_fs, "{{ protocol version={:d}.{:d}, reason=\"{}\" }}",
                      header->protocol_major_version,
                      header->protocol_minor_version, reason );
        return bytes_parsed;
    }
    const uint32_t tab_ct { 1 };
    const std::string_view indent { _tabIndent( tab_ct ) };
    if ( _verbosity == Settings::Verbosity::Debug ) {
        constexpr uint32_t name_width { sizeof( "protocol-major-version" ) - 1 };
        fmt::print(
            _log_fs, R"({{
{}{: <{}} = {} (status: failed)
{}{: <{}} = {} (length of reason in bytes)
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {} (padded length of reason in 4-byte units)
{}{: <{}} = "{}"
{}}}
)",
            indent, "success", name_width, _formatInteger( header->success ),
            indent, "n", name_width, _formatInteger( header->n ),
            indent, "protocol-major-version", name_width,
            _formatInteger( header->protocol_major_version ),
            indent, "protocol-minor-version", name_width,
            _formatInteger( header->protocol_minor_version ),
            indent, "reason_aligned_units", name_width,
            _formatInteger( header->reason_aligned_units ),
            indent, "reason", name_width, reason,
            _tabIndent( tab_ct - 1 )
            );
    } else {
        constexpr uint32_t name_width { sizeof( "protocol version" ) - 1 };
        fmt::print(
            _log_fs, R"({{
{}{: <{}} = {:d}.{:d}
{}{: <{}} = "{}"
{}}}
)",
            indent, "protocol version", name_width,
            header->protocol_major_version, header->protocol_minor_version,
            indent, "reason", name_width, reason,
            _tabIndent( tab_ct - 1 )
            );
    }
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerRequireFurtherAuthentication(
    Connection* conn, const uint8_t* data ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    size_t bytes_parsed {};
    const uint8_t* data_ { data };
    using namespace protocol::connection_setup;
    const ServerRequireFurtherAuthentication::Header* header {
        reinterpret_cast<
        const ServerRequireFurtherAuthentication::Header* >( data_ ) };
    data_ += sizeof( ServerRequireFurtherAuthentication::Header );
    bytes_parsed += sizeof( ServerRequireFurtherAuthentication::Header );
    const size_t reason_padded_sz { header->reason_aligned_units * _ALIGN };
    // STRING8 of n bytes reason
    std::string_view reason {
        reinterpret_cast< const char* >( data_ ), reason_padded_sz };
    data_ += reason_padded_sz;
    bytes_parsed += reason_padded_sz;

    fmt::print( _log_fs, "{:03d}:>:server requested further authentication ",
                conn->id );
    if ( _verbosity == Settings::Verbosity::Singleline ) {
        fmt::println( "{{ reason=\"{}\" }}", reason );
        return bytes_parsed;
    }
    const uint32_t tab_ct { 1 };
    const std::string_view indent { _tabIndent( tab_ct ) };
    if ( _verbosity == Settings::Verbosity::Debug ) {
        constexpr uint32_t name_width { sizeof( "reason_aligned_units" ) - 1 };
        fmt::print(
            _log_fs, R"({{
{}{: <{}} = {} (status: authentication)
{}{: <{}} = {} (padded length of reason in 4-byte units)
{}{: <{}} = "{}"
{}}}
)",
            indent, "success", name_width, _formatInteger( header->success ),
            indent, "reason_aligned_units", name_width,
            _formatInteger( header->reason_aligned_units ),
            indent, "reason", name_width, reason,
            _tabIndent( tab_ct - 1 )
            );
    } else {
        constexpr uint32_t name_width { sizeof( "reason" ) - 1 };
        fmt::print(
            _log_fs, R"({{
{}{: <{}} = "{}"
{}}}
)",
            indent, "reason", name_width, reason,
            _tabIndent( tab_ct - 1 )
            );
    }
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
    const std::string_view indent { _tabIndent( tab_ct ) };
    // TBD for now FORMAT struct is single line regardless of verbosity
    outputs.str += '[';
    for ( uint32_t i {}; i < format_ct; ++i ) {
        outputs.str += fmt::format(
            "{}{}{{ depth={} bits-per-pixel={} scanline-pad={} }}",
            i == 0 ? "" : ",",
            _verbosity == Settings::Verbosity::Singleline ? " " :
            fmt::format( "\n{}", indent ),
            _formatInteger( pixmap_formats[i].depth ),
            _formatInteger( pixmap_formats[i].bits_per_pixel ),
            _formatInteger( pixmap_formats[i].scanline_pad )
            );
    }
    if ( format_ct > 0 ) {
        outputs.str +=
            _verbosity == Settings::Verbosity::Singleline ? " " :
            fmt::format( "\n{}", _tabIndent( tab_ct - 1 ) );
    }
    outputs.str += ']';
    outputs.bytes_parsed = sizeof( ServerAcceptance::Format ) * format_ct;
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

    std::cout << "_parseLISTofDEPTH(tab_ct: " << tab_ct << ")\n";
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
        sizeof( "depth" ) - 1
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
    const std::string memb_whtspc {
        _verbosity == Settings::Verbosity::Singleline ? " " :
        fmt::format( "\n{}", _tabIndent( tab_ct + 1 ) ) };
    const std::string_view equals {
        _verbosity == Settings::Verbosity::Singleline ? "=" : " = " };
    const uint32_t name_width (
        _verbosity == Settings::Verbosity::Singleline ? 0 :
        sizeof( "bits-per-rgb-value" ) - 1
        );
    using namespace protocol::connection_setup;

    outputs.str += '[';
    for ( uint32_t i {}; i < vt_ct; ++i ) {
        const ServerAcceptance::Screen::Depth::VisualType::Encoding* visualtype {
            reinterpret_cast< const ServerAcceptance::Screen::Depth::VisualType::Encoding* >(
                data + outputs.bytes_parsed ) };
        outputs.bytes_parsed += sizeof( ServerAcceptance::Screen::Depth::VisualType::Encoding );

        // TBD for now VISUALTYPE struct is single line regardless of verbosity
        outputs.str += fmt::format(
            "{}{{"
            " {}={} {}={} {}={} {}={}"
            " {}={} {}={} {}={} "
            "}}{}",
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

    const uint32_t tab_ct { 1 };
    // followed by LISTofFORMAT pixmap-formats of n * sizeof(FORMAT) bytes
    _LISTParsingOutputs pixmap_formats_outputs {
        _parseLISTofFORMAT( data + bytes_parsed, header->n, tab_ct + 1 ) };
    bytes_parsed += pixmap_formats_outputs.bytes_parsed;

    // followed by LISTofSCREEN roots of m bytes (m is always a multiple of 4)
    _LISTParsingOutputs roots_outputs {
        _parseLISTofSCREEN( data + bytes_parsed, header->r, tab_ct + 1 ) };
    bytes_parsed += roots_outputs.bytes_parsed;

    fmt::print( _log_fs, "{:03d}:>:server accepted connection ",
                conn->id );
    if ( _verbosity == Settings::Verbosity::Singleline ) {
        fmt::println(
            _log_fs, "{{ protocol version={:d}.{:d}, release=number={}, "
            "release-id-base={}, release-id-mask={}, motion-buffer-size={}, "
            "maximum-request-length={}, image-byte-order={}, "
            "bitmap-format-bit-order={}, bitmap-format-scanline-unit={}, "
            "bitmap-format-scanline-pad={}, min-keycode={}, max-keycode={}, "
            "vendor=\"{}\", pixmap-formats={}, roots={} }}",
            header->protocol_major_version, header->protocol_minor_version,
            _formatInteger( header->release_number ),
            _formatInteger( header->release_id_base ),
            _formatInteger( header->release_id_mask ),
            _formatInteger( header->motion_buffer_size ),
            _formatInteger( header->maximum_request_length ),
            _formatInteger( header->image_byte_order,
                            ServerAcceptance::byte_order_names ),
            _formatInteger( header->bitmap_format_bit_order,
                            ServerAcceptance::bit_order_names ),
            _formatInteger( header->bitmap_format_scanline_unit ),
            _formatInteger( header->bitmap_format_scanline_pad ),
            _formatCommonType( header->min_keycode ),
            _formatCommonType( header->max_keycode ),
            vendor,
            pixmap_formats_outputs.str, // pixmap_formats_str = _formatLISTofFORMAT()
            roots_outputs.str  // roots_str = _formatLISTofSCREEN()
            );
        return bytes_parsed;
    }
    const std::string_view indent { _tabIndent( tab_ct ) };
    constexpr uint32_t name_width { sizeof( "bitmap-format-scanline-unit" ) - 1 };
    if ( _verbosity == Settings::Verbosity::Debug ) {
        fmt::print(
            _log_fs, R"({{
{}{: <{}} = {} (status: accepted)
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {} (length of (padded) vendor + pixmap-formats + roots, in 4-byte units)
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {} (length of vendor in bytes)
{}{: <{}} = {}
{}{: <{}} = {} (number of SCREENs in roots)
{}{: <{}} = {} (number of FORMATs in pixmap-formats)
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = "{}"
{}{: <{}} = {}
{}{: <{}} = {}
}}
)",
            indent, "success", name_width,
            _formatInteger( header->success ),
            indent, "protocol-major-version", name_width,
            _formatInteger( header->protocol_major_version ),
            indent, "protocol-minor-version", name_width,
            _formatInteger( header->protocol_minor_version ),
            indent, "ad", name_width,
            _formatInteger( header->ad ),
            indent, "release-number", name_width,
            _formatInteger( header->release_number ),
            indent, "release-id-base", name_width,
            _formatInteger( header->release_id_base ),
            indent, "release-id-mask", name_width,
            _formatInteger( header->release_id_mask ),
            indent, "motion-buffer-size", name_width,
            _formatInteger( header->motion_buffer_size ),
            indent, "v", name_width,
            _formatInteger( header->v ),
            indent, "maximum-request-length", name_width,
            _formatInteger( header->maximum_request_length ),
            indent, "r", name_width,
            _formatInteger( header->r ),
            indent, "n", name_width,
            _formatInteger( header->n ),
            indent, "image-byte-order", name_width,
            _formatInteger( header->image_byte_order,
                            ServerAcceptance::byte_order_names ),
            indent, "bitmap-format-bit-order", name_width,
            _formatInteger( header->bitmap_format_bit_order,
                            ServerAcceptance::bit_order_names ),
            indent, "bitmap-format-scanline-unit", name_width,
            _formatInteger( header->bitmap_format_scanline_unit ),
            indent, "bitmap-format-scanline-pad", name_width,
            _formatInteger( header->bitmap_format_scanline_pad ),
            indent, "min-keycode", name_width,
            _formatCommonType( header->min_keycode ),
            indent, "max-keycode", name_width,
            _formatCommonType( header->max_keycode ),
            indent, "vendor", name_width,
            vendor,
            indent, "pixmap-formats", name_width,
            pixmap_formats_outputs.str, // pixmap_formats_str = _formatLISTofFORMAT()
            indent, "roots", name_width,
            roots_outputs.str  // roots_str = _formatLISTofSCREEN()
            );
    } else {
        fmt::print(
            _log_fs, R"({{
{}{: <{}} = {:d}.{:d}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = {}
{}{: <{}} = "{}"
{}{: <{}} = {}
{}{: <{}} = {}
}}
)",
            indent, "protocol version", name_width,
            header->protocol_major_version, header->protocol_minor_version,
            indent, "release-number", name_width,
            _formatInteger( header->release_number ),
            indent, "release-id-base", name_width,
            _formatInteger( header->release_id_base ),
            indent, "release-id-mask", name_width,
            _formatInteger( header->release_id_mask ),
            indent, "motion-buffer-size", name_width,
            _formatInteger( header->motion_buffer_size ),
            indent, "maximum-request-length", name_width,
            _formatInteger( header->maximum_request_length ),
            indent, "image-byte-order", name_width,
            _formatInteger( header->image_byte_order,
                            ServerAcceptance::byte_order_names ),
            indent, "bitmap-format-bit-order", name_width,
            _formatInteger( header->bitmap_format_bit_order,
                            ServerAcceptance::bit_order_names ),
            indent, "bitmap-format-scanline-unit", name_width,
            _formatInteger( header->bitmap_format_scanline_unit ),
            indent, "bitmap-format-scanline-pad", name_width,
            _formatInteger( header->bitmap_format_scanline_pad ),
            indent, "min-keycode", name_width,
            _formatCommonType( header->min_keycode ),
            indent, "max-keycode", name_width,
            _formatCommonType( header->max_keycode ),
            indent, "vendor", name_width,
            vendor,
            indent, "pixmap-formats", name_width,
            pixmap_formats_outputs.str, // pixmap_formats_str = _formatLISTofFORMAT()
            indent, "roots", name_width,
            roots_outputs.str  // roots_str = _formatLISTofSCREEN()
            );
    }
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerResponse(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );

    const uint8_t success { *data };
    size_t bytes_parsed {};
    using namespace protocol::connection_setup;
    switch ( success ) {
    case FAILED:
        bytes_parsed = _logServerRefusal( conn, data );
        conn->status = Connection::FAILED;
        break;
    case AUTHENTICATE:
        bytes_parsed = _logServerRequireFurtherAuthentication( conn, data );
        conn->status = Connection::AUTHENTICATION;
        break;
    case SUCCESS:
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
            bytes_parsed = _logServerError( conn, data, sz );
            // assert( bytes_parsed == 32 );
            break;
        case 1:   // Reply
            return sz;  // TBD deactivated for testing
            // log reply func distingusishing by using provided sequence number to look up request opcode,
            //   then call func based on opcode
            // TBD modification of QueryExtension replies should happen here to filter extensions
            bytes_parsed = _logServerReply( conn, data, sz );
            // assert( bytes_parsed >= 32 );
            break;
        default:  // Event (2-35)
            return sz;  // TBD deactivated for testing
            bytes_parsed = _logServerEvent( conn, data, sz, first_byte );
            // assert( bytes_parsed == 32 );
            break;
        }
    }
        break;
    default:
        break;
    }
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
    return tl_bytes_parsed;
}
