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
X11ProtocolParser::_parseListMember<
    protocol::STR >(
        const uint8_t* data, const size_t sz, const bool byteswap,
        const _Whitespace& ws ) {
    using protocol::STR;
    assert( data != nullptr );
    assert( sz >= sizeof( STR::Header ) );

    _ParsingOutputs outputs;
    const STR::Header* header {
        reinterpret_cast< const STR::Header* >( data ) };
    outputs.bytes_parsed += sizeof( STR::Header );
    // followed by STRING8 name
    const auto name_len { _hostByteOrder( header->name_len, byteswap ) };
    const std::string_view name {
        reinterpret_cast< const char* >( data + outputs.bytes_parsed ),
        name_len };
    // note that LISTofSTR will often be padded, but not single STR
    outputs.bytes_parsed += name_len;

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
            _formatVariable( header->name_len, byteswap ), ws.separator ),
        ws.memb_indent, "name", memb_name_w, ws.equals,
        name, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseListMember<
    protocol::HOST >(
        const uint8_t* data, const size_t sz,  const bool byteswap,
        const _Whitespace& ws ) {
    using protocol::HOST;
    assert( data != nullptr );
    assert( sz >= sizeof( HOST::Header ) );

    _ParsingOutputs outputs;
    const HOST::Header* header {
        reinterpret_cast< const HOST::Header* >( data ) };
    outputs.bytes_parsed += sizeof( HOST::Header );
    const auto family { _hostByteOrder( header->family, byteswap ) };
    assert( family < HOST::family_names.size() );
    // check for skipped enum values 4, 5
    assert( !HOST::family_names[ family ].empty() );
    // no HOST::Encoding
    // followed by LISTofBYTE address
    const auto address_len {
        _hostByteOrder( header->address_len, byteswap ) };
    const _ParsingOutputs address {
        _parseLISTof< protocol::BYTE >(
            data + outputs.bytes_parsed, sz - outputs.bytes_parsed, address_len,
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    outputs.bytes_parsed += alignment.pad( address_len );

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
        _formatVariable( header->family, byteswap,
                         HOST::family_names ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(address length)", memb_name_w, ws.equals,
            _formatVariable( header->address_len, byteswap ), ws.separator ),
        ws.memb_indent, "address", memb_name_w, ws.equals,
        address.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseListMember<
    protocol::connection_setup::Acceptance::SCREEN >(
        const uint8_t* data, const size_t sz, const bool byteswap,
        const _Whitespace& ws ) {
    using SCREEN =
        protocol::connection_setup::Acceptance::SCREEN;
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
            _hostByteOrder( encoding->allowed_depths_ct, byteswap ),
            byteswap, ws.nested() ) };
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
        _formatVariable( encoding->root, byteswap ), ws.separator,
        ws.memb_indent, "default-colormap", memb_name_w, ws.equals,
        _formatVariable( encoding->default_colormap, byteswap ), ws.separator,
        ws.memb_indent, "white-pixel", memb_name_w, ws.equals,
        _formatVariable( encoding->white_pixel, byteswap ), ws.separator,
        ws.memb_indent, "black-pixel", memb_name_w, ws.equals,
        _formatVariable( encoding->black_pixel, byteswap ), ws.separator,
        ws.memb_indent, "current-input-masks", memb_name_w, ws.equals,
        _formatVariable( encoding->current_input_masks, byteswap ), ws.separator,
        ws.memb_indent, "width-in-pixels", memb_name_w, ws.equals,
        _formatVariable( encoding->width_in_pixels, byteswap ), ws.separator,
        ws.memb_indent, "height-in-pixels", memb_name_w, ws.equals,
        _formatVariable( encoding->height_in_pixels, byteswap ), ws.separator,
        ws.memb_indent, "width-in-millimeters", memb_name_w, ws.equals,
        _formatVariable( encoding->width_in_millimeters, byteswap ), ws.separator,
        ws.memb_indent, "height-in-millimeters", memb_name_w, ws.equals,
        _formatVariable( encoding->height_in_millimeters, byteswap ), ws.separator,
        ws.memb_indent, "min-installed-maps", memb_name_w, ws.equals,
        _formatVariable( encoding->min_installed_maps, byteswap ), ws.separator,
        ws.memb_indent, "max-installed-maps", memb_name_w, ws.equals,
        _formatVariable( encoding->max_installed_maps, byteswap ), ws.separator,
        ws.memb_indent, "root-visual", memb_name_w, ws.equals,
        _formatVariable( encoding->root_visual, byteswap ), ws.separator,
        ws.memb_indent, "backing-stores", memb_name_w, ws.equals,
        _formatVariable( encoding->backing_stores, byteswap,
                         SCREEN::backing_stores_names ), ws.separator,
        ws.memb_indent, "save-unders", memb_name_w, ws.equals,
        _formatVariable( encoding->save_unders, byteswap ), ws.separator,
        ws.memb_indent, "root-depth", memb_name_w, ws.equals,
        _formatVariable( encoding->root_depth, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(DEPTHs in allowed-depths)",
            memb_name_w, ws.equals,
            _formatVariable( encoding->allowed_depths_ct, byteswap ), ws.separator ),
        ws.memb_indent, "allowed-depths", memb_name_w, ws.equals,
        allowed_depths.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseListMember<
    protocol::connection_setup::Acceptance::SCREEN::DEPTH >(
        const uint8_t* data, const size_t sz, const bool byteswap,
        const _Whitespace& ws ) {
    using DEPTH = protocol::connection_setup::Acceptance::SCREEN::DEPTH;
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
            _hostByteOrder( encoding->visuals_ct, byteswap ),
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
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
        _formatVariable( encoding->depth, byteswap ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(VISUALs in visuals)", memb_name_w, ws.equals,
            _formatVariable( encoding->visuals_ct, byteswap ), ws.separator ),
        ws.memb_indent, "visuals", memb_name_w, ws.equals,
        visuals.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseListMember<
    protocol::requests::PolyText8::TEXTITEM8 >(
        const uint8_t* data, const size_t sz, const bool byteswap,
        const _Whitespace& ws ) {
    using protocol::requests::PolyText8;
    assert( data != nullptr );
    assert( sz >= sizeof( PolyText8::TEXTITEM8::TEXTELT8 ) );

    _ParsingOutputs outputs;
    const PolyText8::TEXTITEM8 item {
        *reinterpret_cast< const PolyText8::TEXTITEM8* >( data ) };
    if ( _hostByteOrder( item.font.font_shift, byteswap ) ==
         PolyText8::FONT::FONT_SHIFT ) {
        outputs.bytes_parsed += sizeof( PolyText8::FONT );
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
            !settings.verbose ? "" : fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "font-shift", memb_name_w, ws.equals,
                _formatVariable( item.font.font_shift, byteswap ), ws.separator ),
            ws.memb_indent, "font", memb_name_w, ws.equals,
            _formatVariable( font, byteswap ), ws.separator,
            ws.encl_indent
            );
        return outputs;
    }
    outputs.bytes_parsed += sizeof( PolyText8::TEXTITEM8::TEXTELT8 );
    // followed by STRING8 string
    const auto string_len {
        _hostByteOrder( item.text_element.string_len, byteswap ) };
    const std::string_view string {
        reinterpret_cast< const char* >( data + outputs.bytes_parsed ),
        string_len };
    outputs.bytes_parsed += string_len;

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
            _formatVariable( item.text_element.string_len, byteswap ), ws.separator ),
        ws.memb_indent, "delta", memb_name_w, ws.equals,
        _formatVariable( item.text_element.delta, byteswap ), ws.separator,
        ws.memb_indent, "string", memb_name_w, ws.equals,
        string, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseListMember<
    protocol::requests::PolyText16::TEXTITEM16 >(
        const uint8_t* data, const size_t sz, const bool byteswap,
        const _Whitespace& ws ) {
    using protocol::requests::PolyText16;
    assert( data != nullptr );
    assert( sz >= sizeof( PolyText16::TEXTITEM16::TEXTELT16 ) );

    _ParsingOutputs outputs;
    const PolyText16::TEXTITEM16 item {
        *reinterpret_cast< const PolyText16::TEXTITEM16* >( data ) };
    if ( _hostByteOrder( item.font.font_shift, byteswap ) ==
         PolyText16::FONT::FONT_SHIFT ) {
        outputs.bytes_parsed += sizeof( PolyText16::FONT );
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
                _formatVariable( item.font.font_shift, byteswap ), ws.separator ),
            ws.memb_indent, "font", memb_name_w, ws.equals,
            _formatVariable( font, byteswap ), ws.separator,
            ws.encl_indent
            );
        return outputs;
    }
    outputs.bytes_parsed += sizeof( PolyText16::TEXTITEM16::TEXTELT16 );
    // followed by STRING16 string
    const _ParsingOutputs string {
        _parseLISTof< protocol::CHAR2B >(
            data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
            _hostByteOrder( item.text_element.string_2B_len, byteswap ),
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
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
            _formatVariable( item.text_element.string_2B_len, byteswap ), ws.separator ),
        ws.memb_indent, "delta", memb_name_w, ws.equals,
        _formatVariable( item.text_element.delta, byteswap ), ws.separator,
        ws.memb_indent, "string", memb_name_w, ws.equals,
        string.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}
