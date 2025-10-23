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
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "default-colormap", memb_name_w, ws.equals,
        _formatProtocolType( encoding->default_colormap ), ws.separator,
        ws.memb_indent, "white-pixel", memb_name_w, ws.equals,
        _formatInteger( encoding->white_pixel ), ws.separator,
        ws.memb_indent, "black-pixel", memb_name_w, ws.equals,
        _formatInteger( encoding->black_pixel ), ws.separator,
        ws.memb_indent, "current-input-masks", memb_name_w, ws.equals,
        _formatProtocolType( encoding->current_input_masks ), ws.separator,
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
        _formatProtocolType( encoding->root_visual ), ws.separator,
        ws.memb_indent, "backing-stores", memb_name_w, ws.equals,
        _formatInteger( encoding->backing_stores,
                        SCREEN::backing_stores_names ), ws.separator,
        ws.memb_indent, "save-unders", memb_name_w, ws.equals,
        _formatProtocolType( encoding->save_unders ), ws.separator,
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
        const protocol::FONT font {
            ntohl( *reinterpret_cast< const uint32_t* >(
                       item.font.font_bytes ) ) };

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
            _formatProtocolType( font ), ws.separator,
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
        const protocol::FONT font {
            htonl( *reinterpret_cast< const uint32_t* >(
                       item.font.font_bytes ) ) };

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
            _formatProtocolType( font ), ws.separator,
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
