#include <arpa/inet.h>  // ntohl

#include <cassert>
#include <cstdint>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/common_types.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/requests.hpp"
#include "protocol/connection_setup.hpp"


template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::STR >(
        const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using protocol::STR;
    assert( data != nullptr );
    assert( sz >= sizeof( STR::Header ) );

    _ParsingOutputs outputs;
    const STR::Header* header {
        reinterpret_cast< const STR::Header* >( data ) };
    outputs.bytes_parsed += sizeof( STR::Header );
    // followed by STRING8 name
    const std::string_view name {
        reinterpret_cast< const char* >( data + outputs.bytes_parsed ),
        header->name_len };
    // note that LISTofSTR will often be padded, but not single STR
    outputs.bytes_parsed += header->name_len;

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(name length)" ) :
                              sizeof( "name" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatInteger( header->name_len ), ws.separator ),
        ws.memb_indent, "name", memb_name_w, ws.equals,
        name, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::HOST >(
        const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using protocol::HOST;
    assert( data != nullptr );
    assert( sz >= sizeof( HOST::Header ) );

    _ParsingOutputs outputs;
    const HOST::Header* header {
        reinterpret_cast< const HOST::Header* >( data ) };
    outputs.bytes_parsed += sizeof( HOST::Header );
    assert( header->family < HOST::family_names.size() );
    // check for skipped enum values 4, 5
    assert( !HOST::family_names[ header->family ].empty() );
    // no HOST::Encoding
    // followed by LISTofBYTE address
    const _ParsingOutputs address {
        _parseLISTof< protocol::BYTE >(
            data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
            header->address_len, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    outputs.bytes_parsed += alignment.pad( header->address_len );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(address length)" ) :
                              sizeof( "address" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "family", memb_name_w, ws.equals,
        _formatInteger( header->family,
                        HOST::family_names ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(address length)", memb_name_w, ws.equals,
            _formatInteger( header->address_len ), ws.separator ),
        ws.memb_indent, "address", memb_name_w, ws.equals,
        address.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::connection_setup::ConnAcceptance::SCREEN >(
        const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using SCREEN =
        protocol::connection_setup::ConnAcceptance::SCREEN;
    assert( data != nullptr );
    assert( sz >= sizeof( SCREEN::Encoding ) );

    _ParsingOutputs outputs;
    const SCREEN::Encoding* encoding {
        reinterpret_cast< const SCREEN::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SCREEN::Encoding );
    // followed by LISTofDEPTH allowed-depths
    const _ParsingOutputs allowed_depths {
        _parseLISTof< SCREEN::DEPTH >(
            data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
            encoding->allowed_depths_ct, ws.nested() ) };
    outputs.bytes_parsed += allowed_depths.bytes_parsed;
    assert( outputs.bytes_parsed <= sz );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(DEPTHs in allowed-depths)" ) :
                              sizeof( "height-in-millimeters" ) ) - 1 );
    outputs.str += fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"  // allowed_depths_ct
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatCommonType( encoding->root ), ws.separator,
        ws.memb_indent, "default-colormap", memb_name_w, ws.equals,
        _formatCommonType( encoding->default_colormap ), ws.separator,
        ws.memb_indent, "white-pixel", memb_name_w, ws.equals,
        _formatInteger( encoding->white_pixel ), ws.separator,
        ws.memb_indent, "black-pixel", memb_name_w, ws.equals,
        _formatInteger( encoding->black_pixel ), ws.separator,
        ws.memb_indent, "current-input-masks", memb_name_w, ws.equals,
        _formatCommonType( encoding->current_input_masks ), ws.separator,
        ws.memb_indent, "width-in-pixels", memb_name_w, ws.equals,
        _formatInteger( encoding->width_in_pixels ), ws.separator,
        ws.memb_indent, "height-in-pixels", memb_name_w, ws.equals,
        _formatInteger( encoding->height_in_pixels ), ws.separator,
        ws.memb_indent, "width-in-millimeters", memb_name_w, ws.equals,
        _formatInteger( encoding->width_in_millimeters ), ws.separator,
        ws.memb_indent, "height-in-millimeters", memb_name_w, ws.equals,
        _formatInteger( encoding->height_in_millimeters ), ws.separator,
        ws.memb_indent, "min-installed-maps", memb_name_w, ws.equals,
        _formatInteger( encoding->min_installed_maps ), ws.separator,
        ws.memb_indent, "max-installed-maps", memb_name_w, ws.equals,
        _formatInteger( encoding->max_installed_maps ), ws.separator,
        ws.memb_indent, "root-visual", memb_name_w, ws.equals,
        _formatCommonType( encoding->root_visual ), ws.separator,
        ws.memb_indent, "backing-stores", memb_name_w, ws.equals,
        _formatInteger( encoding->backing_stores,
                        SCREEN::backing_stores_names ), ws.separator,
        ws.memb_indent, "save-unders", memb_name_w, ws.equals,
        _formatCommonType( encoding->save_unders ), ws.separator,
        ws.memb_indent, "root-depth", memb_name_w, ws.equals,
        _formatInteger( encoding->root_depth ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(DEPTHs in allowed-depths)",
            memb_name_w, ws.equals,
            _formatInteger( encoding->allowed_depths_ct ), ws.separator ),
        ws.memb_indent, "allowed-depths", memb_name_w, ws.equals,
        allowed_depths.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::connection_setup::ConnAcceptance::SCREEN::DEPTH >(
        const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using DEPTH = protocol::connection_setup::ConnAcceptance::SCREEN::DEPTH;
    assert( data != nullptr );
    assert( sz >= sizeof( DEPTH::Encoding ) );

    _ParsingOutputs outputs;
    const DEPTH::Encoding* encoding {
        reinterpret_cast< const DEPTH::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( DEPTH::Encoding );
    // followed by LISTofVISUALTYPE visuals
    const _ParsingOutputs visuals {
        _parseLISTof< DEPTH::VISUALTYPE >(
            data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
            encoding->visuals_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    outputs.bytes_parsed += visuals.bytes_parsed;
    assert( outputs.bytes_parsed <= sz );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(VISUALs in visuals)" ) :
                              sizeof( "visuals" ) ) - 1 );
    outputs.str += fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}"
        "{}"  // visuals_ct
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatInteger( encoding->depth ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(VISUALs in visuals)", memb_name_w, ws.equals,
            _formatInteger( encoding->visuals_ct ), ws.separator ),
        ws.memb_indent, "visuals", memb_name_w, ws.equals,
        visuals.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::requests::PolyText8::TEXTITEM8 >(
        const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using protocol::requests::PolyText8;
    assert( data != nullptr );
    assert( sz >= sizeof( PolyText8::TEXTITEM8::TEXTELT8 ) );

    _ParsingOutputs outputs;
    const PolyText8::TEXTITEM8 item {
        *reinterpret_cast< const PolyText8::TEXTITEM8* >( data ) };
    if ( item.font.font_shift == PolyText8::FONT_SHIFT ) {
        outputs.bytes_parsed += sizeof( PolyText8::TEXTITEM8::FONT );
        // font bytes in array from MSB to LSB
        // TBD (can't use brace init here due to issues with
        //   protocol::impl::Integer ctor and how its children must stay
        //   literal types for use in std::tuple<>)
        protocol::FONT font;
        font.data = ntohl(
            *reinterpret_cast< const uint32_t* >( item.font.font_bytes ) );

        const uint32_t memb_name_w (
            !ws.multiline ? 0 :
            sizeof( settings.verbose ? "font-shift" : "font" ) - 1 );
        outputs.str += fmt::format(
            "{{{}"
            "{}"
            "{}{: <{}}{}{}{}"
            "{}}}",
            ws.separator,
            !settings.verbose ? "" : fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "font-shift", memb_name_w, ws.equals,
                _formatInteger( item.font.font_shift ), ws.separator ),
            ws.memb_indent, "font", memb_name_w, ws.equals,
            _formatCommonType( font ), ws.separator,
            ws.encl_indent
            );
        return outputs;
    }
    outputs.bytes_parsed += sizeof( PolyText8::TEXTITEM8::TEXTELT8 );
    // followed by STRING8 string
    const std::string_view string {
        reinterpret_cast< const char* >( data + outputs.bytes_parsed ),
        item.text_element.string_len };
    outputs.bytes_parsed += item.text_element.string_len;

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(string len)" ) :
                              sizeof( "string" ) ) - 1 );
    outputs.str += fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(string len)", memb_name_w, ws.equals,
            _formatInteger( item.text_element.string_len ), ws.separator ),
        ws.memb_indent, "delta", memb_name_w, ws.equals,
        _formatInteger( item.text_element.delta ), ws.separator,
        ws.memb_indent, "string", memb_name_w, ws.equals,
        string, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

// requests::PolyText16::TEXTITEM16 (need parsing due to TEXTITEM*.TEXTELT16 being followed by STRING16)
template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::requests::PolyText16::TEXTITEM16 >(
        const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using protocol::requests::PolyText16;
    assert( data != nullptr );
    assert( sz >= sizeof( PolyText16::TEXTITEM16::TEXTELT16 ) );

    _ParsingOutputs outputs;
    const PolyText16::TEXTITEM16 item {
        *reinterpret_cast< const PolyText16::TEXTITEM16* >( data ) };
    if ( item.font.font_shift == PolyText16::FONT_SHIFT ) {
        outputs.bytes_parsed += sizeof( PolyText16::TEXTITEM16::FONT );
        // font bytes in array from MSB to LSB
        // TBD (can't use brace init here due to issues with
        //   protocol::impl::Integer ctor and how its children must stay
        //   literal types for use in std::tuple<>)
        protocol::FONT font;
        font.data = ntohl(
            *reinterpret_cast< const uint32_t* >( item.font.font_bytes ) );

        const uint32_t memb_name_w (
            !ws.multiline ? 0 : ( settings.verbose ?
                                  sizeof( "font-shift" ) :
                                  sizeof( "font" ) ) - 1 );
        outputs.str += fmt::format(
            "{{{}"
            "{}"
            "{}{: <{}}{}{}{}"
            "{}}}",
            ws.separator,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "font-shift", memb_name_w, ws.equals,
                _formatInteger( item.font.font_shift ), ws.separator ),
            ws.memb_indent, "font", memb_name_w, ws.equals,
            _formatCommonType( font ), ws.separator,
            ws.encl_indent
            );
        return outputs;
    }
    outputs.bytes_parsed += sizeof( PolyText16::TEXTITEM16::TEXTELT16 );
    // followed by STRING16 string
    const _ParsingOutputs string {
        _parseLISTof< protocol::CHAR2B >(
            data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
            item.text_element.string_2B_len,
            ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    outputs.bytes_parsed += string.bytes_parsed;

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(string length (CHAR2B))" ) :
                              sizeof( "string" ) ) - 1 );
    outputs.str += fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(string length (CHAR2B))", memb_name_w, ws.equals,
            _formatInteger( item.text_element.string_2B_len ), ws.separator ),
        ws.memb_indent, "delta", memb_name_w, ws.equals,
            _formatInteger( item.text_element.delta ), ws.separator,
        ws.memb_indent, "string", memb_name_w, ws.equals,
        string.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType< protocol::CHAR2B >(
    const uint8_t* data, const size_t sz, const _Whitespace&/* ws*/ ) {
    using protocol::CHAR2B;
    assert( data != nullptr );
    assert( sz >= sizeof( CHAR2B ) );

    _ParsingOutputs outputs;
    // TBD default to printing as hex due to convoluted encoding, see:
    //   https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Common_Types
    assert( sizeof( CHAR2B ) == sizeof( uint16_t ) );
    const uint16_t char2B { *reinterpret_cast< const uint16_t* >( data ) };
    outputs.bytes_parsed = sizeof( char2B );
    outputs.str = _formatInteger( char2B,
                                  {}, _ValueTraits::BITMASK );  // force hex
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType< protocol::POINT >(
    const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using protocol::POINT;
    assert( data != nullptr );
    assert( sz >= sizeof( POINT ) );

    _ParsingOutputs outputs;
    const POINT point {
        *reinterpret_cast< const POINT* >( data ) };
    outputs.bytes_parsed = sizeof( point );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "x" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatInteger( point.x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatInteger( point.y ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType< protocol::RECTANGLE >(
    const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using protocol::RECTANGLE;
    assert( data != nullptr );
    assert( sz >= sizeof( RECTANGLE ) );

    _ParsingOutputs outputs;
    const RECTANGLE rectangle {
        *reinterpret_cast< const RECTANGLE* >( data ) };
    outputs.bytes_parsed = sizeof( rectangle );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "height" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatInteger( rectangle.x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatInteger( rectangle.y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatInteger( rectangle.width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatInteger( rectangle.height ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType< protocol::ARC >(
    const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using protocol::ARC;
    assert( data != nullptr );
    assert( sz >= sizeof( protocol::ARC ) );

    _ParsingOutputs outputs;
    const ARC arc {
        *reinterpret_cast< const ARC* >( data ) };
    outputs.bytes_parsed = sizeof( arc );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "height" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatInteger( arc.x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatInteger( arc.y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatInteger( arc.width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatInteger( arc.height ), ws.separator,
        ws.memb_indent, "angle1", memb_name_w, ws.equals,
        _formatInteger( arc.angle1 ), ws.separator,
        ws.memb_indent, "angle2", memb_name_w, ws.equals,
        _formatInteger( arc.angle2 ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType< protocol::requests::QueryFont::Reply::CHARINFO >(
    const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using CHARINFO = protocol::requests::QueryFont::Reply::CHARINFO;
    assert( data != nullptr );
    assert( sz >= sizeof( CHARINFO ) );

    _ParsingOutputs outputs;
    const CHARINFO charinfo {
        *reinterpret_cast< const CHARINFO* >( data ) };
    outputs.bytes_parsed = sizeof( charinfo );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "right-side-bearing" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "left-side-bearing", memb_name_w, ws.equals,
        _formatInteger( charinfo.left_side_bearing ), ws.separator,
        ws.memb_indent, "right-side-bearing", memb_name_w, ws.equals,
        _formatInteger( charinfo.right_side_bearing ), ws.separator,
        ws.memb_indent, "character-width", memb_name_w, ws.equals,
        _formatInteger( charinfo.character_width ), ws.separator,
        ws.memb_indent, "ascent", memb_name_w, ws.equals,
        _formatInteger( charinfo.ascent ), ws.separator,
        ws.memb_indent, "descent", memb_name_w, ws.equals,
        _formatInteger( charinfo.descent ), ws.separator,
        ws.memb_indent, "attributes", memb_name_w, ws.equals,
        _formatInteger( charinfo.attributes ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType< protocol::requests::QueryFont::Reply::FONTPROP >(
    const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using FONTPROP = protocol::requests::QueryFont::Reply::FONTPROP;
    assert( data != nullptr );
    assert( sz >= sizeof( FONTPROP ) );

    _ParsingOutputs outputs;
    const FONTPROP fontprop {
        *reinterpret_cast< const FONTPROP* >( data ) };
    outputs.bytes_parsed = sizeof( fontprop );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "value" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "name", memb_name_w, ws.equals,
        _formatCommonType( fontprop.name ), ws.separator,
        ws.memb_indent, "value", memb_name_w, ws.equals,
        _formatInteger( fontprop.value ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType< protocol::connection_setup::ConnAcceptance::FORMAT >(
    const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using FORMAT = protocol::connection_setup::ConnAcceptance::FORMAT;
    assert( data != nullptr );
    assert( sz >= sizeof( FORMAT ) );

    _ParsingOutputs outputs;
    const FORMAT format {
        *reinterpret_cast< const FORMAT* >( data ) };
    outputs.bytes_parsed = sizeof( format );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "bits-per-pixel" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatInteger( format.depth ), ws.separator,
        ws.memb_indent, "bits-per-pixel", memb_name_w, ws.equals,
        _formatInteger( format.bits_per_pixel ), ws.separator,
        ws.memb_indent, "scanline-pad", memb_name_w, ws.equals,
        _formatInteger( format.scanline_pad ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::connection_setup::ConnAcceptance::SCREEN::DEPTH::VISUALTYPE >(
    const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using VISUALTYPE =
        protocol::connection_setup::ConnAcceptance::SCREEN::DEPTH::VISUALTYPE;
    assert( data != nullptr );
    assert( sz >= sizeof( VISUALTYPE ) );

    _ParsingOutputs outputs;
    const VISUALTYPE visualtype {
        *reinterpret_cast< const VISUALTYPE* >( data ) };
    outputs.bytes_parsed = sizeof( visualtype );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "bits-per-rgb-value" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "visual-id", memb_name_w, ws.equals,
        _formatCommonType( visualtype.visual_id ), ws.separator,
        ws.memb_indent, "class", memb_name_w, ws.equals,
        _formatInteger( visualtype.class_,
                        { VISUALTYPE::class_names } ), ws.separator,
        ws.memb_indent, "bits-per-rgb-value", memb_name_w, ws.equals,
        _formatInteger( visualtype.bits_per_rgb_value ), ws.separator,
        ws.memb_indent, "colormap-entries", memb_name_w, ws.equals,
        _formatInteger( visualtype.colormap_entries ), ws.separator,
        ws.memb_indent, "red-mask", memb_name_w, ws.equals,
        _formatInteger( visualtype.red_mask,
                        {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "green-mask", memb_name_w, ws.equals,
        _formatInteger( visualtype.green_mask,
                       {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "blue-mask", memb_name_w, ws.equals,
        _formatInteger( visualtype.blue_mask,
                        {}, _ValueTraits::BITMASK ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::requests::PolySegment::SEGMENT >(
    const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using SEGMENT = protocol::requests::PolySegment::SEGMENT;
    assert( data != nullptr );
    assert( sz >= sizeof( SEGMENT ) );

    _ParsingOutputs outputs;
    const SEGMENT segment {
        *reinterpret_cast< const SEGMENT* >( data ) };
    outputs.bytes_parsed = sizeof( segment );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "x1" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x1", memb_name_w, ws.equals,
        _formatInteger( segment.x1 ), ws.separator,
        ws.memb_indent, "y1", memb_name_w, ws.equals,
        _formatInteger( segment.y1 ), ws.separator,
        ws.memb_indent, "x2", memb_name_w, ws.equals,
        _formatInteger( segment.x2 ), ws.separator,
        ws.memb_indent, "y2", memb_name_w, ws.equals,
        _formatInteger( segment.y2 ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::requests::StoreColors::COLORITEM >(
    const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using COLORITEM = protocol::requests::StoreColors::COLORITEM;
    assert( data != nullptr );
    assert( sz >= sizeof( COLORITEM ) );

    _ParsingOutputs outputs;
    const COLORITEM coloritem {
        *reinterpret_cast< const COLORITEM* >( data ) };
    outputs.bytes_parsed = sizeof( coloritem );
    assert( ( coloritem.do_rgb_mask & COLORITEM::DO_RGB_ZERO_BITS ) == 0 );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "(do rgb mask)" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "pixel", memb_name_w, ws.equals,
        _formatInteger( coloritem.pixel ), ws.separator,
        ws.memb_indent, "red", memb_name_w, ws.equals,
        _formatInteger( coloritem.red ), ws.separator,
        ws.memb_indent, "green", memb_name_w, ws.equals,
        _formatInteger( coloritem.green ), ws.separator,
        ws.memb_indent, "blue", memb_name_w, ws.equals,
        _formatInteger( coloritem.blue ), ws.separator,
        ws.memb_indent, "(do rgb mask)", memb_name_w, ws.equals,
        _formatInteger( coloritem.do_rgb_mask,
                        { COLORITEM::do_rgb_names },
                        _ValueTraits::BITMASK ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::requests::GetMotionEvents::Reply::TIMECOORD >(
    const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using TIMECOORD = protocol::requests::GetMotionEvents::Reply::TIMECOORD;
    assert( data != nullptr );
    assert( sz >= sizeof( TIMECOORD ) );

    _ParsingOutputs outputs;
    const TIMECOORD timecoord {
        *reinterpret_cast< const TIMECOORD* >( data ) };
    outputs.bytes_parsed = sizeof( timecoord );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "time" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatCommonType( timecoord.time ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatInteger( timecoord.x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatInteger( timecoord.y ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::requests::QueryColors::Reply::RGB >(
    const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    using RGB = protocol::requests::QueryColors::Reply::RGB;
    assert( data != nullptr );
    assert( sz >= sizeof( RGB ) );

    _ParsingOutputs outputs;
    const RGB rgb {
        *reinterpret_cast< const RGB* >( data ) };
    outputs.bytes_parsed = sizeof( rgb );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "green" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "red", memb_name_w, ws.equals,
        _formatInteger( rgb.red ), ws.separator,
        ws.memb_indent, "green", memb_name_w, ws.equals,
        _formatInteger( rgb.green ), ws.separator,
        ws.memb_indent, "blue", memb_name_w, ws.equals,
        _formatInteger( rgb.blue ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}
