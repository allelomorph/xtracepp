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
    assert( str.n > 0 );
    outputs.bytes_parsed += sizeof( protocol::STR );
    const std::string_view name {
        reinterpret_cast< const char* >( data + outputs.bytes_parsed ),
        str.n };
    // note that LISTofSTR will often be padded, but not single STR
    outputs.bytes_parsed += str.n;

    // TBD leaving hard-coded singleline for now
    outputs.str += fmt::format(
        "{{ {}name={:?} }}",
        !settings.verbose ? "" :
        fmt::format( "n={} ", _formatInteger( str.n ) ),
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
            data + outputs.bytes_parsed, sz - outputs.bytes_parsed, host.n,
            ws.nested( _Whitespace::SINGLELINE ) ) };
    outputs.bytes_parsed += _pad( host.n );

    // TBD leaving hard-coded singleline for now
    outputs.str += fmt::format(
        "{{ family={}{} address={} }}",
        _formatInteger( host.family, protocol::HOST::family_names ),
        !settings.verbose ? "" :
        fmt::format( " n address bytes={}",
                     _formatInteger( host.n ) ),
        address.str );
    return outputs;
}

// ==============================

// ServerAcceptance::SCREEN
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::connection_setup::ServerAcceptance::SCREEN >(
        const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    assert( data != nullptr );

    const uint32_t name_width (
        settings.multiline ? sizeof( "height-in-millimeters" ) - 1 : 0 );

    _ParsingOutputs outputs;
    using SCREEN = protocol::connection_setup::ServerAcceptance::SCREEN;
    assert( sz >= sizeof( SCREEN::Header ));
    const SCREEN::Header* header { reinterpret_cast< const SCREEN::Header* >( data ) };
    outputs.bytes_parsed += sizeof( SCREEN::Header );

    // followed by LISTofDEPTH allowed-depths of n bytes (n is always a multiple of 4)
    const _ParsingOutputs allowed_depths {
        _parseLISTof< SCREEN::DEPTH >(
            data + outputs.bytes_parsed, sz - outputs.bytes_parsed, header->d,
            ws.nested() ) };
    outputs.bytes_parsed += allowed_depths.bytes_parsed;

    outputs.str += fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "root", name_width, ws.equals,
        _formatProtocolType( header->root ), ws.separator,
        ws.memb_indent, "default-colormap", name_width, ws.equals,
        _formatProtocolType( header->default_colormap ), ws.separator,
        ws.memb_indent, "white-pixel", name_width, ws.equals,
        _formatInteger( header->white_pixel ), ws.separator,
        ws.memb_indent, "black-pixel", name_width, ws.equals,
        _formatInteger( header->black_pixel ), ws.separator,
        ws.memb_indent, "current-input-masks", name_width, ws.equals,
        _formatProtocolType( header->current_input_masks ), ws.separator,
        ws.memb_indent, "width-in-pixels", name_width, ws.equals,
        _formatInteger( header->width_in_pixels ), ws.separator,
        ws.memb_indent, "height-in-pixels", name_width, ws.equals,
        _formatInteger( header->height_in_pixels ), ws.separator,
        ws.memb_indent, "width-in-millimeters", name_width, ws.equals,
        _formatInteger( header->width_in_millimeters ), ws.separator,
        ws.memb_indent, "height-in-millimeters", name_width, ws.equals,
        _formatInteger( header->height_in_millimeters ), ws.separator,
        ws.memb_indent, "min-installed-maps", name_width, ws.equals,
        _formatInteger( header->min_installed_maps ), ws.separator,
        ws.memb_indent, "max-installed-maps", name_width, ws.equals,
        _formatInteger( header->max_installed_maps ), ws.separator,
        ws.memb_indent, "root-visual", name_width, ws.equals,
        _formatProtocolType( header->root_visual ), ws.separator,
        ws.memb_indent, "backing-stores", name_width, ws.equals,
        _formatInteger( header->backing_stores,
                        SCREEN::backing_stores_names ), ws.separator,
        ws.memb_indent, "save-unders", name_width, ws.equals,
        _formatProtocolType( header->save_unders ), ws.separator,
        ws.memb_indent, "root-depth", name_width, ws.equals,
        _formatInteger( header->root_depth ), ws.separator,
        ws.memb_indent, "allowed-depths", name_width, ws.equals,
        allowed_depths.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

// ServerAcceptance( tab_ct 0 ) _parseLISTofFORMAT( tab_ct 2 )
// ServerAcceptance( tab_ct 0 ) _parseLISTofSCREEN( tab_ct 2 ) _parseLISTofDEPTH( tab_ct 4 ) _parseLISTofVISUALTYPE( tab_ct 6 )
// ServerAcceptance::Screen::DEPTH
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::connection_setup::ServerAcceptance::SCREEN::DEPTH >(
        const uint8_t* data, const size_t sz, const _Whitespace& ws ) {
    assert( data != nullptr );

    _ParsingOutputs outputs;
    using DEPTH = protocol::connection_setup::ServerAcceptance::SCREEN::DEPTH;
    assert( sz >= sizeof( DEPTH::Header ));
    const DEPTH::Header* header {
        reinterpret_cast< const DEPTH::Header* >( data ) };
    outputs.bytes_parsed += sizeof( DEPTH::Header );

    // followed by LISTofVISUALTYPE visuals of n * sizeof(VISUALTYPE) bytes
    const _ParsingOutputs visuals {
        _parseLISTof< DEPTH::VISUALTYPE >(
            data + outputs.bytes_parsed, sz - outputs.bytes_parsed, header->n,
            ws.nested() ) };
    outputs.bytes_parsed += visuals.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "visuals" ) - 1 : 0 );
    outputs.str += fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "depth", name_width, ws.equals,
        _formatInteger( header->depth ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "n", name_width, ws.equals,
            _formatInteger( header->n ), ws.separator ),
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
        assert( item.text_element.m > 0 );
        outputs.bytes_parsed += sizeof( PolyText8::TEXTITEM8::TEXTELT8 );
        std::string_view string {
            reinterpret_cast< const char* >( data + outputs.bytes_parsed ),
            item.text_element.m };
        outputs.bytes_parsed += item.text_element.m;

        const uint32_t name_width (
            settings.multiline ? sizeof( "string" ) - 1 : 0 );
        outputs.str += fmt::format(
            "{{{}"
            "{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{:?}{}"
            "{}}}",
            ws.separator,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "m", name_width, ws.equals,
                _formatInteger( item.text_element.m ), ws.separator ),
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
                item.text_element.m, ws.nested( _Whitespace::SINGLELINE ) ) };
        outputs.bytes_parsed += string.bytes_parsed;

        const uint32_t name_width (
            settings.multiline ? sizeof( "string" ) - 1 : 0 );
        outputs.str += fmt::format(
            "{{{}"
            "{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}}}",
            ws.separator,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "m", name_width, ws.equals,
                _formatInteger( item.text_element.m ), ws.separator ),
            ws.memb_indent, "delta", name_width, ws.equals,
            _formatInteger( item.text_element.delta ), ws.separator,
            ws.memb_indent, "string", name_width, ws.equals,
            string.str, ws.separator,
            ws.encl_indent
            );
    }
    return outputs;
}
