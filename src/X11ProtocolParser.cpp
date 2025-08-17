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

    const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "authorization-protocol-data" ) - 1 : 0 );

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
        "{{{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}"
        "{}{: <{}}{}\"{}\"{}"
        "{}"
        "{}{: <{}}{}({:d} bytes){}"
        "{}}}",
        _separator,
        memb_indent, "byte-order", name_width, _equals,
        _formatInteger( ( header->byte_order == 'l' ) ? 0 : 1,
                        protocol::enum_names::image_byte_order ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}",
            memb_indent, "protocol-major-version", name_width, _equals,
            _formatInteger( header->protocol_major_version ), _separator,
            memb_indent, "protocol-minor-version", name_width, _equals,
            _formatInteger( header->protocol_minor_version ), _separator ) :
        fmt::format(
            "{}{: <{}}{}{:d}.{:d}{}",
            memb_indent, "protocol version", name_width, _equals,
            header->protocol_major_version,
            header->protocol_minor_version, _separator ),
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length in bytes of authorization-protocol-name){}",
            memb_indent, "n", name_width, _equals,
            _formatInteger( header->n ), _separator ) : "",
        memb_indent, "authorization-protocol-name", name_width, _equals,
        auth_protocol_name, _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length in bytes of authorization-protocol-data){}",
            memb_indent, "d", name_width, _equals,
            _formatInteger( header->d ), _separator ) : "",
        // TBD may be security concerns with logging auth data
        memb_indent, "authorization-protocol-data", name_width, _equals,
        header->d, _separator,
        struct_indent
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

    const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "protocol-major-version" ) - 1 : 0 );

    fmt::print( _log_fs, "{:03d}:>:server refused connection ",
                conn->id );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}"
        "{}"
        "{}"
        "{}{: <{}}{}\"{}\"{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{} (status: failed){}",
            memb_indent, "success", name_width, _equals,
            _formatInteger( header->success ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length of reason in bytes){}",
            memb_indent, "n", name_width, _equals,
            _formatInteger( header->n ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}",
            memb_indent, "protocol-major-version", name_width, _equals,
            _formatInteger( header->protocol_major_version ), _separator,
            memb_indent, "protocol-minor-version", name_width, _equals,
            _formatInteger( header->protocol_minor_version ), _separator ) :
        fmt::format(
            "{}{: <{}}{}{:d}.{:d}{}",
            memb_indent, "protocol version", name_width, _equals,
            header->protocol_major_version,
            header->protocol_minor_version, _separator ),
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{} (padded length of reason in 4 byte units){}",
            memb_indent, "reason aligned units", name_width, _equals,
            _formatInteger( header->reason_aligned_units ), _separator ) : "",
        memb_indent, "reason", name_width, _equals,
        reason, _separator,
        struct_indent
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

    const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "success" ) - 1 : 0 );

    fmt::print( _log_fs, "{:03d}:>:server requested further authentication ",
                conn->id );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}"
        "{}{: <{}}{}\"{}\"{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{} (status: authentication){}",
            memb_indent, "success", name_width, _equals,
            _formatInteger( header->success ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{} (padded length of reason in 4 byte units){}",
            memb_indent, "reason aligned units", name_width, _equals,
            _formatInteger( header->reason_aligned_units ), _separator ) : "",
        memb_indent, "reason", name_width, _equals,
        reason, _separator,
        struct_indent
        );
    return bytes_parsed;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofFORMAT(
    const uint8_t* data, const uint32_t format_ct, const uint32_t tab_ct ) {
    assert( data != nullptr );
    assert( tab_ct > 0 );

    X11ProtocolParser::_ParsingOutputs outputs;
    using namespace protocol::connection_setup;
    const ServerAcceptance::Format* pixmap_formats {
        reinterpret_cast< const ServerAcceptance::Format* >( data ) };

    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    // const std::string_view memb_indent {
    //     _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "bits-per-pixel" ) - 1 : 0 );
    // TBD for now FORMAT struct is single line regardless of verbosity
    outputs.str += '[';
    if ( format_ct > 0 )
        outputs.str += _separator;
    for ( uint32_t i {}; i < format_ct; ++i ) {
        outputs.str += fmt::format(
            "{}{{ depth={} bits-per-pixel={} scanline-pad={} }}{}{}",
            struct_indent,
            _formatInteger( pixmap_formats[i].depth ),
            _formatInteger( pixmap_formats[i].bits_per_pixel ),
            _formatInteger( pixmap_formats[i].scanline_pad ),
            i == format_ct - 1 ? "" : ",",
            _separator
            );
        outputs.bytes_parsed += sizeof( ServerAcceptance::Format );
    }
    if ( format_ct > 0 )
        outputs.str += _multiline ? _tabIndent( tab_ct - 1 ) : "";
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofSCREEN(
    const uint8_t* data, const uint32_t screen_ct, const uint32_t tab_ct ) {
    assert( data != nullptr );
    assert( tab_ct > 0 );

    X11ProtocolParser::_ParsingOutputs outputs;
    const std::string_view list_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "height-in-millimeters" ) - 1 : 0 );
    using namespace protocol::connection_setup;

    outputs.str += '[';
    if ( screen_ct > 0 )
        outputs.str += _separator;
    for ( uint32_t i {}; i < screen_ct; ++i ) {
        const ServerAcceptance::Screen::Header* header {
            reinterpret_cast< const ServerAcceptance::Screen::Header* >(
                data + outputs.bytes_parsed ) };
        outputs.bytes_parsed += sizeof( ServerAcceptance::Screen::Header );

        // followed by LISTofDEPTH allowed-depths of n bytes (n is always a multiple of 4)
        _ParsingOutputs allowed_depths_outputs {
            _parseLISTofDEPTH( data + outputs.bytes_parsed, header->d, tab_ct + 2 ) };
        outputs.bytes_parsed += allowed_depths_outputs.bytes_parsed;

        outputs.str += fmt::format(
            "{}{{{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}}}{}{}",
            list_indent, _separator,
            memb_indent, "root", name_width, _equals,
            _formatCommonType( header->root ), _separator,
            memb_indent, "default-colormap", name_width, _equals,
            _formatCommonType( header->default_colormap ), _separator,
            memb_indent, "white-pixel", name_width, _equals,
            _formatInteger( header->white_pixel ), _separator,
            memb_indent, "black-pixel", name_width, _equals,
            _formatInteger( header->black_pixel ), _separator,
            memb_indent, "current-input-masks", name_width, _equals,
            _formatCommonType( header->current_input_masks ), _separator,
            memb_indent, "width-in-pixels", name_width, _equals,
            _formatInteger( header->width_in_pixels ), _separator,
            memb_indent, "height-in-pixels", name_width, _equals,
            _formatInteger( header->height_in_pixels ), _separator,
            memb_indent, "width-in-millimeters", name_width, _equals,
            _formatInteger( header->width_in_millimeters ), _separator,
            memb_indent, "height-in-millimeters", name_width, _equals,
            _formatInteger( header->height_in_millimeters ), _separator,
            memb_indent, "min-installed-maps", name_width, _equals,
            _formatInteger( header->min_installed_maps ), _separator,
            memb_indent, "max-installed-maps", name_width, _equals,
            _formatInteger( header->max_installed_maps ), _separator,
            memb_indent, "root-visual", name_width, _equals,
            _formatCommonType( header->root_visual ), _separator,
            memb_indent, "backing-stores", name_width, _equals,
            _formatInteger( header->backing_stores,
                            ServerAcceptance::Screen::backing_stores_names ),
            _separator,
            memb_indent, "save-unders", name_width, _equals,
            _formatCommonType( header->save_unders ), _separator,
            memb_indent, "root-depth", name_width, _equals,
            _formatInteger( header->root_depth ), _separator,
            memb_indent, "allowed-depths", name_width, _equals,
            allowed_depths_outputs.str, _separator,
            list_indent, i == screen_ct - 1 ? "" : ",", _separator
            );
    }
    if ( screen_ct > 0 )
        outputs.str += _multiline ? _tabIndent( tab_ct - 1 ) : "";
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofDEPTH(
    const uint8_t* data, const uint32_t depth_ct, const uint32_t tab_ct ) {
    assert( data != nullptr );
    assert( tab_ct > 0 );

    X11ProtocolParser::_ParsingOutputs outputs;
    const std::string_view list_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "visuals" ) - 1 : 0 );
    using namespace protocol::connection_setup;

    outputs.str += '[';
    if ( depth_ct > 0 )
        outputs.str += _separator;
    for ( uint32_t i {}; i < depth_ct; ++i ) {
        const ServerAcceptance::Screen::Depth::Header* header {
            reinterpret_cast< const ServerAcceptance::Screen::Depth::Header* >(
                data + outputs.bytes_parsed ) };
        outputs.bytes_parsed += sizeof( ServerAcceptance::Screen::Depth::Header );

        // followed by LISTofVISUALTYPE visuals of n * sizeof(VISUALTYPE) bytes
        _ParsingOutputs visuals_outputs {
            _parseLISTofVISUALTYPE( data + outputs.bytes_parsed, header->n, tab_ct + 2 ) };
        outputs.bytes_parsed += visuals_outputs.bytes_parsed;

        outputs.str += fmt::format(
            "{}{{{}"
            "{}{: <{}}{}{}{}"
            "{}"
            "{}{: <{}}{}{}{}"
            "{}}}{}{}",
            list_indent, _separator,
            memb_indent, "depth", name_width, _equals,
            _formatInteger( header->depth ), _separator,
            _verbose ?
            fmt::format(
                "{}{: <{}}{}{}{}",
                memb_indent, "n", name_width, _equals,
                _formatInteger( header->n ), _separator ) : "",
            memb_indent, "visuals", name_width, _equals,
            visuals_outputs.str, _separator,
            list_indent, i == depth_ct - 1 ? "" : ",", _separator
            );
    }
    if ( depth_ct > 0 )
        outputs.str += _multiline ? _tabIndent( tab_ct - 1 ) : "";
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofVISUALTYPE(
    const uint8_t* data, const uint32_t vt_ct, const uint32_t tab_ct ) {
    assert( data != nullptr );
    assert( tab_ct > 0 );

    X11ProtocolParser::_ParsingOutputs outputs;
    const std::string_view list_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "bits-per-rgb-value" ) - 1 : 0 );
    using namespace protocol::connection_setup;

    outputs.str += '[';
    if ( vt_ct > 0 )
        outputs.str += _separator;
    for ( uint32_t i {}; i < vt_ct; ++i ) {
        const ServerAcceptance::Screen::Depth::VisualType::Encoding* visualtype {
            reinterpret_cast< const ServerAcceptance::Screen::Depth::VisualType::Encoding* >(
                data + outputs.bytes_parsed ) };
        outputs.bytes_parsed += sizeof( ServerAcceptance::Screen::Depth::VisualType::Encoding );

        // TBD for now VISUALTYPE struct is single line regardless of verbosity
        outputs.str += fmt::format(
            "{}{{ {}={} {}={} {}={} {}={} {}={} {}={} {}={} }}{}{}",
            list_indent,
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
            i == vt_ct - 1 ? "" : ",", _separator
            );
        // outputs.str += fmt::format(
        //     "{}{{{}"
        //     "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        //     "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        //     "{}}}{}{}",
        //     list_indent, _separator,
        //     memb_indent, "visual-id", name_width, _equals,
        //     _formatCommonType( visualtype->visual_id ), _separator,
        //     memb_indent, "class", name_width, _equals,
        //     _formatInteger( visualtype->class_,
        //                     ServerAcceptance::Screen::Depth::VisualType::class_names ),
        //      _separator,
        //     memb_indent, "bits-per-rgb-value", name_width, _equals,
        //     _formatInteger( visualtype->bits_per_rgb_value ), _separator,
        //     memb_indent, "colormap-entries", name_width, _equals,
        //     _formatInteger( visualtype->colormap_entries ), _separator,
        //     memb_indent, "red-mask", name_width, _equals,
        //     _formatBitmask( visualtype->red_mask ), _separator,
        //     memb_indent, "green-mask", name_width, _equals,
        //     _formatBitmask( visualtype->green_mask ), _separator,
        //     memb_indent, "blue-mask", name_width, _equals,
        //     _formatBitmask( visualtype->blue_mask ), _separator,
        //     list_indent, i == vt_ct - 1 ? "" : ",", _separator
        //     );
    }
    if ( vt_ct > 0 )
        outputs.str += _multiline ? _tabIndent( tab_ct - 1 ) : "";
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofSTR( const uint8_t* data, const uint16_t str_ct ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    outputs.str += '[';
    for ( uint16_t i {}; i < str_ct; ++i ) {
        const uint8_t n { *( data + outputs.bytes_parsed ) };
        outputs.bytes_parsed++;
        outputs.str += fmt::format(
            "{}\"{}\"", outputs.str.size() > 1 ? " " : "",
            std::string_view(
                reinterpret_cast< const char* >( data + outputs.bytes_parsed ), n ) );
        outputs.bytes_parsed += n;
    }
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofCARD8( const uint8_t* data, const uint16_t n ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    outputs.str += '[';
    for ( uint16_t i {}; i < n; ++i ) {
        outputs.str += fmt::format(
            "{}{}", outputs.str.size() > 1 ? " " : "",
            _formatInteger( *( data + outputs.bytes_parsed ) ) );
        outputs.bytes_parsed++;
    }
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofCARD32( const uint8_t* data, const uint16_t n ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    outputs.str += '[';
    for ( uint16_t i {}; i < n; ++i ) {
        outputs.str += fmt::format(
            "{}{}", outputs.str.size() > 1 ? " " : "",
            _formatInteger( *reinterpret_cast< const protocol::CARD32* >(
                                data + outputs.bytes_parsed ) ) );
        outputs.bytes_parsed += sizeof( protocol::CARD32 );
    }
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofRECTANGLE( const uint8_t* data, const uint16_t n ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    outputs.str += '[';
    for ( uint16_t i {}; i < n; ++i ) {
        const protocol::RECTANGLE* rectangle {
            reinterpret_cast< const protocol::RECTANGLE* >( data + outputs.bytes_parsed )
        };
        outputs.bytes_parsed += sizeof( protocol::RECTANGLE );
        outputs.str += fmt::format(
            " {{ x={} y={} width={} height={} }}",
            _formatInteger( rectangle->x ),
            _formatInteger( rectangle->y ),
            _formatInteger( rectangle->width ),
            _formatInteger( rectangle->height ) );
    }
    outputs.str += n > 0 ? " ]" : "]";
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofPOINT( const uint8_t* data, const uint16_t n ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    outputs.str += '[';
    for ( uint16_t i {}; i < n; ++i ) {
        const protocol::POINT* point {
            reinterpret_cast< const protocol::POINT* >( data + outputs.bytes_parsed )
        };
        outputs.bytes_parsed += sizeof( protocol::POINT );
        outputs.str += fmt::format(
            " {{ x={} y={} }}",
            _formatInteger( point->x ),
            _formatInteger( point->y ) );
    }
    outputs.str += n > 0 ? " ]" : "]";
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofSEGMENT( const uint8_t* data, const uint16_t n ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    using protocol::requests::PolySegment;
    outputs.str += '[';
    for ( uint16_t i {}; i < n; ++i ) {
        const PolySegment::SEGMENT* segment {
            reinterpret_cast< const PolySegment::SEGMENT* >( data + outputs.bytes_parsed )
        };
        outputs.bytes_parsed += sizeof( PolySegment::SEGMENT );
        outputs.str += fmt::format(
            " {{ x1={} y1={} x2={} y2={} }}",
            _formatInteger( segment->x1 ),
            _formatInteger( segment->y1 ),
            _formatInteger( segment->x2 ),
            _formatInteger( segment->y2 ) );
    }
    outputs.str += n > 0 ? " ]" : "]";
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofARC( const uint8_t* data, const uint16_t n ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    outputs.str += '[';
    for ( uint16_t i {}; i < n; ++i ) {
        const protocol::ARC* arc {
            reinterpret_cast< const protocol::ARC* >( data + outputs.bytes_parsed )
        };
        outputs.bytes_parsed += sizeof( protocol::ARC );
        outputs.str += fmt::format(
            " {{ x={} y={} width={} height={} angle1={} angle2={} }}",
            _formatInteger( arc->x ),
            _formatInteger( arc->y ),
            _formatInteger( arc->width ),
            _formatInteger( arc->height ),
            _formatInteger( arc->angle1 ),
            _formatInteger( arc->angle2 ) );
    }
    outputs.str += n > 0 ? " ]" : "]";
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofCOLORITEM( const uint8_t* data, const uint16_t n ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    outputs.str += '[';
    using protocol::requests::StoreColors;
    for ( uint16_t i {}; i < n; ++i ) {
        const StoreColors::COLORITEM* item {
            reinterpret_cast< const StoreColors::COLORITEM* >( data + outputs.bytes_parsed )
        };
        outputs.bytes_parsed += sizeof( StoreColors::COLORITEM );
        outputs.str += fmt::format(
            " {{ pixel={} red={} green={} blue={} do rgb={} }}",
            _formatInteger( item->pixel ),
            _formatInteger( item->red ),
            _formatInteger( item->green ),
            _formatInteger( item->blue ),
            _formatBitmask( item->do_rgb_mask, StoreColors::do_rgb_names ) );
    }
    outputs.str += n > 0 ? " ]" : "]";
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofKEYSYM( const uint8_t* data, const uint16_t n ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    outputs.str += '[';
    for ( uint16_t i {}; i < n; ++i ) {
        outputs.str += fmt::format(
            "{}{}", outputs.str.size() > 1 ? " " : "",
            _formatInteger(
                reinterpret_cast< const protocol::KEYSYM* >(
                    data + outputs.bytes_parsed )->data ) );
        outputs.bytes_parsed += sizeof( protocol::KEYSYM );
    }
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofATOM( const uint8_t* data, const uint16_t n ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    outputs.str += '[';
    for ( uint16_t i {}; i < n; ++i ) {
        outputs.str += fmt::format(
            "{}{}", outputs.str.size() > 1 ? " " : "",
            _formatCommonType(
                *reinterpret_cast< const protocol::ATOM* >(
                    data + outputs.bytes_parsed ) ) );
        outputs.bytes_parsed += sizeof( protocol::ATOM );
    }
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofKEYCODE( const uint8_t* data, const uint16_t n ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    outputs.str += '[';
    for ( uint16_t i {}; i < n; ++i ) {
        outputs.str += fmt::format(
            "{}{}", outputs.str.size() > 1 ? " " : "",
            _formatCommonType(
                *reinterpret_cast< const protocol::KEYCODE* >(
                    data + outputs.bytes_parsed ) ) );
        outputs.bytes_parsed += sizeof( protocol::KEYCODE );
    }
    outputs.str += ']';
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofTEXTITEM8( const uint8_t* data, const size_t sz ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    outputs.str += '[';
    using protocol::requests::PolyText8;
    while ( _pad( outputs.bytes_parsed ) < sz ) {
        const PolyText8::TEXTITEM8* item {
            reinterpret_cast< const PolyText8::TEXTITEM8* >( data + outputs.bytes_parsed )
        };
        const uint8_t first_byte { *( data + outputs.bytes_parsed ) };
        if ( first_byte == PolyText8::FONT_SHIFT ) {
            outputs.bytes_parsed += sizeof( PolyText8::TEXTITEM8::FONT );
            // font bytes in array from MSB to LSB
            const protocol::FONT font {
                uint32_t( item->font.font_bytes[0] ) << 24 |
                ( item->font.font_bytes[1] << 16  ) |
                ( item->font.font_bytes[2] << 8  ) |
                item->font.font_bytes[3]
            };
            // TBD font_shift only if verbose
            outputs.str += fmt::format(
                " {{ font_shift={} font={} }}",
                _formatInteger( item->font.font_shift ),
                _formatCommonType( font ) );
        } else if ( first_byte == 0 ) {
            // padding byte
            // TBD is this case not needed?
            break;
        } else {
            outputs.bytes_parsed += sizeof( PolyText8::TEXTITEM8::TEXTELT8 );
            std::string_view string {
                reinterpret_cast< const char* >( data + outputs.bytes_parsed ),
                item->text_element.m };
            outputs.bytes_parsed += item->text_element.m;
            // TBD m only if verbose
            outputs.str += fmt::format(
                " {{ m={} delta={} string=\"{}\" }}",
                _formatInteger( item->text_element.m ),
                _formatInteger( item->text_element.delta ),
                string );
        }
    }
    outputs.str += outputs.bytes_parsed > 0 ? " ]" : "]";
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseLISTofTEXTITEM16( const uint8_t* data, const size_t sz ) {
    assert( data != nullptr );
    _ParsingOutputs outputs {};
    outputs.str += '[';
    using protocol::requests::PolyText16;
    while ( _pad( outputs.bytes_parsed ) < sz ) {
        const PolyText16::TEXTITEM16* item {
            reinterpret_cast< const PolyText16::TEXTITEM16* >( data + outputs.bytes_parsed )
        };
        const uint16_t first_byte { *( data + outputs.bytes_parsed ) };
        if ( first_byte == PolyText16::FONT_SHIFT ) {
            outputs.bytes_parsed += sizeof( PolyText16::TEXTITEM16::FONT );
            // font bytes in array from MSB to LSB
            const protocol::FONT font {
                uint32_t( item->font.font_bytes[0] ) << 24 |
                ( item->font.font_bytes[1] << 16  ) |
                ( item->font.font_bytes[2] << 8  ) |
                item->font.font_bytes[3]
            };
            // TBD font_shift only if verbose
            outputs.str += fmt::format(
                " {{ font_shift={} font={} }}",
                _formatInteger( item->font.font_shift ),
                _formatCommonType( font ) );
        } else if ( first_byte == 0 ) {
            // padding byte
            // TBD is this case not needed?
            break;
        } else {
            outputs.bytes_parsed += sizeof( PolyText16::TEXTITEM16::TEXTELT16 );
            std::string hex_str {};
            // TBD treating CHAR2B as CARD16, but encoding may be more complicated in standard
            const uint32_t hex_width { ( sizeof( protocol::CHAR2B ) * 2 ) + 2 };
            for ( uint8_t i {}, m { item->text_element.m }; i < m; ++i ) {
                hex_str += fmt::format(
                    "{}{:#0{}x}", hex_str.empty() ? "" : " ",
                    *reinterpret_cast< const uint16_t* >(
                        data + outputs.bytes_parsed ), hex_width );
                outputs.bytes_parsed += sizeof( protocol::CHAR2B );
            }
            // TBD m only if verbose
            outputs.str += fmt::format(
                " {{ m={} delta={} string=[{}] }}",
                _formatInteger( item->text_element.m ),
                _formatInteger( item->text_element.delta ),
                hex_str );
        }
    }
    outputs.str += outputs.bytes_parsed > 0 ? " ]" : "]";
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

    const uint32_t tab_ct { 0 };
    // followed by LISTofFORMAT pixmap-formats of n * sizeof(FORMAT) bytes
    _ParsingOutputs pixmap_formats_outputs {
        _parseLISTofFORMAT( data + bytes_parsed, header->n, tab_ct + 2 ) };
    bytes_parsed += pixmap_formats_outputs.bytes_parsed;

    // followed by LISTofSCREEN roots of m bytes (m is always a multiple of 4)
    _ParsingOutputs roots_outputs {
        _parseLISTofSCREEN( data + bytes_parsed, header->r, tab_ct + 2 ) };
    bytes_parsed += roots_outputs.bytes_parsed;

    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "bitmap-format-scanline-unit" ) - 1 : 0 );

    fmt::print( _log_fs, "{:03d}:>:server accepted connection ",
                conn->id );
    fmt::println(
        _log_fs,
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
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{} (status: accepted){}",
            memb_indent, "success", name_width, _equals,
            _formatInteger( header->success ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}",
            memb_indent, "protocol-major-version", name_width, _equals,
            _formatInteger( header->protocol_major_version ), _separator,
            memb_indent, "protocol-minor-version", name_width, _equals,
            _formatInteger( header->protocol_minor_version ), _separator ) :
        fmt::format(
            "{}{: <{}}{}{:d}.{:d}{}",
            memb_indent, "protocol version", name_width, _equals,
            header->protocol_major_version,
            header->protocol_minor_version, _separator ),
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length of (padded) vendor + pixmap-formats + roots, in 4-byte units){}",
            memb_indent, "ad", name_width, _equals,
            _formatInteger( header->ad ), _separator ) : "",
        memb_indent, "release-number", name_width, _equals,
        _formatInteger( header->release_number ), _separator,
        memb_indent, "resource-id-base", name_width, _equals,
        _formatInteger( header->resource_id_base ), _separator,
        memb_indent, "resource-id-mask", name_width, _equals,
        _formatInteger( header->resource_id_mask ), _separator,
        memb_indent, "motion-buffer-size", name_width, _equals,
        _formatInteger( header->motion_buffer_size ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length of vendor in bytes){}",
            memb_indent, "v", name_width, _equals,
            _formatInteger( header->v ), _separator ) : "",
        memb_indent, "maximum-request-length", name_width, _equals,
        _formatInteger( header->maximum_request_length ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{} (number of SCREENs in roots){}",
                     memb_indent, "r", name_width, _equals,
            _formatInteger( header->r ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{} (number of FORMATs in pixmap-formats){}",
            memb_indent, "n", name_width, _equals,
            _formatInteger( header->n ), _separator ) : "",
        memb_indent, "image-byte-order", name_width, _equals,
        _formatInteger( header->image_byte_order,
                        ServerAcceptance::byte_order_names ), _separator,
        memb_indent, "bitmap-format-bit-order", name_width, _equals,
        _formatInteger( header->bitmap_format_bit_order,
                        ServerAcceptance::bit_order_names ), _separator,
        memb_indent, "bitmap-format-scanline-unit", name_width, _equals,
        _formatInteger( header->bitmap_format_scanline_unit ), _separator,
        memb_indent, "bitmap-format-scanline-pad", name_width, _equals,
        _formatInteger( header->bitmap_format_scanline_pad ), _separator,
        memb_indent, "min-keycode", name_width, _equals,
        _formatCommonType( header->min_keycode ), _separator,
        memb_indent, "max-keycode", name_width, _equals,
        _formatCommonType( header->max_keycode ), _separator,
        memb_indent, "vendor", name_width, _equals,
        vendor, _separator,
        memb_indent, "pixmap-formats", name_width, _equals,
        pixmap_formats_outputs.str, _separator,
        memb_indent, "roots", name_width, _equals,
        roots_outputs.str, _separator,
        struct_indent
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

void X11ProtocolParser::importSettings(
    FILE* log_fs, const bool multiline,
    const bool verbose, const bool readwritedebug ) {
    assert( log_fs != nullptr );
    assert( !feof( log_fs ) && !ferror( log_fs ) );

    _log_fs = log_fs;
    _multiline = multiline;
    _verbose = verbose;
    _readwritedebug = readwritedebug;

    if ( _multiline ) {
        _separator = '\n';
        _equals    = " = ";
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
        if ( _readwritedebug ) {
            fmt::println( _log_fs, "{:03d}:<:parsed   {:4d} bytes",
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
        if ( _readwritedebug ) {
            fmt::println( _log_fs, "{:03d}:>:parsed   {:4d} bytes",
                          conn->id, bytes_parsed );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    assert( tl_bytes_parsed == conn->server_buffer.size() );
    return tl_bytes_parsed;
}
