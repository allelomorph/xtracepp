#include <arpa/inet.h>  // ntohl

#include <cassert>
#include <cstdint>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
//#include "Settings.hpp"
#include "Connection.hpp"
#include "protocol/common_types.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/requests.hpp"
#include "protocol/connection_setup.hpp"


// TBD zero bit validation should maybe happen instead at parse time


template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::STR >(
        const uint8_t* data, const size_t sz, const _Whitespace&/* ws*/ ) {
    assert( data != nullptr );

    _ParsingOutputs outputs;
    assert( sz >= sizeof( protocol::STR ) );
    const protocol::STR str {
        *reinterpret_cast< const protocol::STR* >( data ) };
    assert( str.name_len > 0 );
    outputs.bytes_parsed += sizeof( protocol::STR );
    const std::string_view name {
        reinterpret_cast< const char* >( data + outputs.bytes_parsed ),
        str.name_len };
    // note that LISTofSTR will often be padded, but not single STR
    outputs.bytes_parsed += str.name_len;

    // TBD leaving hard-coded singleline for now
    outputs.str += fmt::format(
        "{{ {}name={:?} }}",
        !settings.verbose ? "" : fmt::format(
            "(name length)={} ", _formatInteger( str.name_len ) ),
        name );
    return outputs;
}

// protocol::HOST (followed by n LISTofBYTE)
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::HOST >(
        const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    assert( data != nullptr );
    assert( sz >= sizeof( protocol::HOST ) );

    _ParsingOutputs outputs;
    const protocol::HOST host {
        *reinterpret_cast< const protocol::HOST* >( data ) };
    assert( host.family < protocol::HOST::family_names.size() );
    // TBD what would we name constants for unused values?
    assert( host.family != 3 && host.family != 4 );
    outputs.bytes_parsed += sizeof( protocol::HOST );
    const _ParsingOutputs address {
        _parseLISTof< protocol::BYTE >(
            data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
            host.address_len, ws.nested( _Whitespace::SINGLELINE ) ) };
    outputs.bytes_parsed += _pad( host.address_len );

    // TBD leaving hard-coded singleline for now
    outputs.str += fmt::format(
        "{{ family={}{} address={} }}",
        _formatInteger( host.family, protocol::HOST::family_names ),
        !settings.verbose ? "" :
        fmt::format( " (address length)={}",
                     _formatInteger( host.address_len ) ),
        address.str );
    return outputs;
}

// ==============================

template <>
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

    const uint32_t name_width (
        settings.multiline ? sizeof( "height-in-millimeters" ) - 1 : 0 );
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
        ws.memb_indent, "root", name_width, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "default-colormap", name_width, ws.equals,
        _formatProtocolType( encoding->default_colormap ), ws.separator,
        ws.memb_indent, "white-pixel", name_width, ws.equals,
        _formatInteger( encoding->white_pixel ), ws.separator,
        ws.memb_indent, "black-pixel", name_width, ws.equals,
        _formatInteger( encoding->black_pixel ), ws.separator,
        ws.memb_indent, "current-input-masks", name_width, ws.equals,
        _formatProtocolType( encoding->current_input_masks ), ws.separator,
        ws.memb_indent, "width-in-pixels", name_width, ws.equals,
        _formatInteger( encoding->width_in_pixels ), ws.separator,
        ws.memb_indent, "height-in-pixels", name_width, ws.equals,
        _formatInteger( encoding->height_in_pixels ), ws.separator,
        ws.memb_indent, "width-in-millimeters", name_width, ws.equals,
        _formatInteger( encoding->width_in_millimeters ), ws.separator,
        ws.memb_indent, "height-in-millimeters", name_width, ws.equals,
        _formatInteger( encoding->height_in_millimeters ), ws.separator,
        ws.memb_indent, "min-installed-maps", name_width, ws.equals,
        _formatInteger( encoding->min_installed_maps ), ws.separator,
        ws.memb_indent, "max-installed-maps", name_width, ws.equals,
        _formatInteger( encoding->max_installed_maps ), ws.separator,
        ws.memb_indent, "root-visual", name_width, ws.equals,
        _formatProtocolType( encoding->root_visual ), ws.separator,
        ws.memb_indent, "backing-stores", name_width, ws.equals,
        _formatInteger( encoding->backing_stores,
                        SCREEN::backing_stores_names ), ws.separator,
        ws.memb_indent, "save-unders", name_width, ws.equals,
        _formatProtocolType( encoding->save_unders ), ws.separator,
        ws.memb_indent, "root-depth", name_width, ws.equals,
        _formatInteger( encoding->root_depth ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(DEPTHs in allowed-depths)",
            name_width, ws.equals,
            _formatInteger( encoding->allowed_depths_ct ), ws.separator ),
        ws.memb_indent, "allowed-depths", name_width, ws.equals,
        allowed_depths.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
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
            encoding->visuals_ct, ws.nested(), _Whitespace::SINGLELINE ) };
    outputs.bytes_parsed += visuals.bytes_parsed;
    assert( outputs.bytes_parsed <= sz );

    const uint32_t name_width (
        settings.multiline ? sizeof( "visuals" ) - 1 : 0 );
    outputs.str += fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}"
        "{}"  // visuals_ct
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "depth", name_width, ws.equals,
        _formatInteger( encoding->depth ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(VISUALs in visuals)", name_width, ws.equals,
            _formatInteger( encoding->visuals_ct ), ws.separator ),
        ws.memb_indent, "visuals", name_width, ws.equals,
        visuals.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

// requests::PolyText8::TEXTITEM8 (need parsing due to TEXTITEM*.TEXTELT8 being followed by STRING8)
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::requests::PolyText8::TEXTITEM8 >(
        const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    assert( data != nullptr );

    _ParsingOutputs outputs;
    using protocol::requests::PolyText8;
    assert( sz >= sizeof( PolyText8::TEXTITEM8 ) );
    const PolyText8::TEXTITEM8 item {
        *reinterpret_cast< const PolyText8::TEXTITEM8* >( data ) };
    if ( item.font.font_shift == PolyText8::FONT_SHIFT ) {
        outputs.bytes_parsed += sizeof( PolyText8::TEXTITEM8::FONT );
        // font bytes in array from MSB to LSB
        const protocol::FONT font {
            ntohl( *reinterpret_cast< const uint32_t* >(
                       item.font.font_bytes ) ) };

        const uint32_t name_width (
            settings.multiline ? sizeof( "font-shift" ) - 1 : 0 );
        outputs.str += fmt::format(
            "{{{}"
            "{}"
            "{}{: <{}}{}{}{}"
            "{}}}",
            ws.separator,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "font-shift", name_width, ws.equals,
                _formatInteger( item.font.font_shift ), ws.separator ),
            ws.memb_indent, "font", name_width, ws.equals,
            _formatProtocolType( font ), ws.separator,
            ws.encl_indent
            );
    } else {
        assert( item.text_element.string_len > 0 );
        outputs.bytes_parsed += sizeof( PolyText8::TEXTITEM8::TEXTELT8 );
        std::string_view string {
            reinterpret_cast< const char* >( data + outputs.bytes_parsed ),
            item.text_element.string_len };
        outputs.bytes_parsed += item.text_element.string_len;

        const uint32_t name_width (
            settings.multiline ? sizeof( "string" ) - 1 : 0 );
        outputs.str += fmt::format(
            "{{{}"
            "{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{:?}{}"
            "{}}}",
            ws.separator,
            !settings.verbose ? "" : fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "(string len)", name_width, ws.equals,
                _formatInteger( item.text_element.string_len ), ws.separator ),
            ws.memb_indent, "delta", name_width, ws.equals,
            _formatInteger( item.text_element.delta ), ws.separator,
            ws.memb_indent, "string", name_width, ws.equals,
            string, ws.separator,
            ws.encl_indent
            );
    }
    return outputs;
}

// requests::PolyText16::TEXTITEM16 (need parsing due to TEXTITEM*.TEXTELT16 being followed by STRING16)
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::requests::PolyText16::TEXTITEM16 >(
        const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    assert( data != nullptr );

    _ParsingOutputs outputs;
    using protocol::requests::PolyText16;
    assert( sz >= sizeof( PolyText16::TEXTITEM16 ) );
    const PolyText16::TEXTITEM16 item {
        *reinterpret_cast< const PolyText16::TEXTITEM16* >( data ) };
    const uint16_t first_byte { *data };
    if ( first_byte == PolyText16::FONT_SHIFT ) {
        outputs.bytes_parsed += sizeof( PolyText16::TEXTITEM16::FONT );
        // font bytes in array from MSB to LSB
        const protocol::FONT font {
            htonl( *reinterpret_cast< const uint32_t* >(
                       item.font.font_bytes ) ) };

        const uint32_t name_width (
            settings.multiline ? sizeof( "font-shift" ) - 1 : 0 );
        outputs.str += fmt::format(
            "{{{}"
            "{}"
            "{}{: <{}}{}{}{}"
            "{}}}",
            ws.separator,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "font-shift", name_width, ws.equals,
                _formatInteger( item.font.font_shift ), ws.separator ),
            ws.memb_indent, "font", name_width, ws.equals,
            _formatProtocolType( font ), ws.separator,
            ws.encl_indent
            );
    } else {
        outputs.bytes_parsed += sizeof( PolyText16::TEXTITEM16::TEXTELT16 );

        const _ParsingOutputs string {
            _parseLISTof< protocol::CHAR2B >(
                data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
                item.text_element.string_2B_len, ws.nested( _Whitespace::SINGLELINE ) ) };
        outputs.bytes_parsed += string.bytes_parsed;

        const uint32_t name_width (
            settings.multiline ? sizeof( "string" ) - 1 : 0 );
        outputs.str += fmt::format(
            "{{{}"
            "{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}}}",
            ws.separator,
            !settings.verbose ? "" : fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "(string length (CHAR2B))", name_width, ws.equals,
                _formatInteger( item.text_element.string_2B_len ), ws.separator ),
            ws.memb_indent, "delta", name_width, ws.equals,
            _formatInteger( item.text_element.delta ), ws.separator,
            ws.memb_indent, "string", name_width, ws.equals,
            string.str, ws.separator,
            ws.encl_indent
            );
    }
    return outputs;
}
