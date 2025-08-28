#include <cassert>
#include <cstdint>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
//#include "Settings.hpp"
#include "Connection.hpp"
#include "protocol/common_types.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/predefined_atoms.hpp"
#include "protocol/requests.hpp"
#include "protocol/connection_setup.hpp"


// TBD zero bit validation should maybe happen instead at parse time
// TBD consider passing vector pointer instead of reference to allow for null
//   checks (use std::optional?) and have lighter weight default ctor


template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::STR >(
        const uint8_t* data, const size_t sz, const _Indentation&/* indents*/ ) {
    assert( data != nullptr );
    assert( sz >= sizeof( protocol::STR ) );

    _ParsingOutputs outputs;
    const protocol::STR str {
        *reinterpret_cast< const protocol::STR* >( data ) };
    assert( str.n > 0 );
    outputs.bytes_parsed += sizeof( protocol::STR );
    const std::string_view name {
        reinterpret_cast< const char* >( data + outputs.bytes_parsed ),
        str.n };
    // note that LISTofSTR will often be padded, but not single STR
    outputs.bytes_parsed += str.n;
    outputs.str += fmt::format(
        "{{ {}name=\"{}\" }}",
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
        const uint8_t* data, const size_t sz, const _Indentation&/* indents*/ ) {
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
        _parseLISTofBYTE( data + outputs.bytes_parsed, host.n ) };
    outputs.bytes_parsed += _pad( host.n );
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
        const uint8_t* data, const size_t sz, const _Indentation& indents ) {
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
        _parseLISTofDEPTH( data + outputs.bytes_parsed, header->d, 4 /* soon _Indentation */ ) };
    outputs.bytes_parsed += allowed_depths.bytes_parsed;

    outputs.str += fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        indents.member, "root", name_width, _EQUALS,
        _formatProtocolType( header->root ), _SEPARATOR,
        indents.member, "default-colormap", name_width, _EQUALS,
        _formatProtocolType( header->default_colormap ), _SEPARATOR,
        indents.member, "white-pixel", name_width, _EQUALS,
        _formatInteger( header->white_pixel ), _SEPARATOR,
        indents.member, "black-pixel", name_width, _EQUALS,
        _formatInteger( header->black_pixel ), _SEPARATOR,
        indents.member, "current-input-masks", name_width, _EQUALS,
        _formatProtocolType( header->current_input_masks ), _SEPARATOR,
        indents.member, "width-in-pixels", name_width, _EQUALS,
        _formatInteger( header->width_in_pixels ), _SEPARATOR,
        indents.member, "height-in-pixels", name_width, _EQUALS,
        _formatInteger( header->height_in_pixels ), _SEPARATOR,
        indents.member, "width-in-millimeters", name_width, _EQUALS,
        _formatInteger( header->width_in_millimeters ), _SEPARATOR,
        indents.member, "height-in-millimeters", name_width, _EQUALS,
        _formatInteger( header->height_in_millimeters ), _SEPARATOR,
        indents.member, "min-installed-maps", name_width, _EQUALS,
        _formatInteger( header->min_installed_maps ), _SEPARATOR,
        indents.member, "max-installed-maps", name_width, _EQUALS,
        _formatInteger( header->max_installed_maps ), _SEPARATOR,
        indents.member, "root-visual", name_width, _EQUALS,
        _formatProtocolType( header->root_visual ), _SEPARATOR,
        indents.member, "backing-stores", name_width, _EQUALS,
        _formatInteger( header->backing_stores,
                        SCREEN::backing_stores_names ), _SEPARATOR,
        indents.member, "save-unders", name_width, _EQUALS,
        _formatProtocolType( header->save_unders ), _SEPARATOR,
        indents.member, "root-depth", name_width, _EQUALS,
        _formatInteger( header->root_depth ), _SEPARATOR,
        indents.member, "allowed-depths", name_width, _EQUALS,
        allowed_depths.str, _SEPARATOR,
        indents.enclosure
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
        const uint8_t* data, const size_t sz, const _Indentation& indents ) {
    assert( data != nullptr );

    _ParsingOutputs outputs;
    const uint32_t name_width (
        settings.multiline ? sizeof( "visuals" ) - 1 : 0 );

    using DEPTH = protocol::connection_setup::ServerAcceptance::SCREEN::DEPTH;
    assert( sz >= sizeof( DEPTH::Header ));
    const DEPTH::Header* header {
        reinterpret_cast< const DEPTH::Header* >( data ) };
    outputs.bytes_parsed += sizeof( DEPTH::Header );

    // followed by LISTofVISUALTYPE visuals of n * sizeof(VISUALTYPE) bytes
    const _ParsingOutputs visuals {
        _parseLISTofVISUALTYPE( data + outputs.bytes_parsed, header->n,
                                4 /* TBD soon _Indentation */ ) };
    outputs.bytes_parsed += visuals.bytes_parsed;

    outputs.str += fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        indents.member, "depth", name_width, _EQUALS,
        _formatInteger( header->depth ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            indents.member, "n", name_width, _EQUALS,
            _formatInteger( header->n ), _SEPARATOR ),
        indents.member, "visuals", name_width, _EQUALS,
        visuals.str, _SEPARATOR,
        indents.enclosure
        );
    return outputs;
}

// requests::PolyText8::TEXTITEM8 (need parsing due to TEXTITEM*.TEXTELT8 being followed by STRING8)
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::requests::PolyText8::TEXTITEM8 >(
        const uint8_t* data, const size_t sz, const _Indentation&/* indents*/ ) {
    assert( data != nullptr );

    _ParsingOutputs outputs;
    using protocol::requests::PolyText8;
    assert( sz >= sizeof( PolyText8::TEXTITEM8 ) );
    const PolyText8::TEXTITEM8 item {
        *reinterpret_cast< const PolyText8::TEXTITEM8* >( data ) };
    const uint8_t first_byte { *data };
    if ( first_byte == PolyText8::FONT_SHIFT ) {
        outputs.bytes_parsed += sizeof( PolyText8::TEXTITEM8::FONT );
        // font bytes in array from MSB to LSB
        // TBD assume little endian for now, will change when server byte order is addressed
        const protocol::FONT font {
            __builtin_bswap32(
                *reinterpret_cast< const uint32_t* >( item.font.font_bytes ) ) };
        outputs.str += fmt::format(
            "{{ {}font={} }}",
            !settings.verbose ? "" :
            fmt::format( "font-shift={} ",
                         _formatInteger( item.font.font_shift ) ),
            _formatProtocolType( font ) );
    } else {
        assert( first_byte > 0 );
        outputs.bytes_parsed += sizeof( PolyText8::TEXTITEM8::TEXTELT8 );
        std::string_view string {
            reinterpret_cast< const char* >( data + outputs.bytes_parsed ),
            item.text_element.m };
        outputs.bytes_parsed += item.text_element.m;
        outputs.str += fmt::format(
            " {{ {}delta={} string=\"{}\" }}",
            !settings.verbose ? "" :
            fmt::format( "m={} ", _formatInteger( item.text_element.m ) ),
            _formatInteger( item.text_element.delta ),
            string );
    }
    return outputs;
}

// requests::PolyText16::TEXTITEM16 (need parsing due to TEXTITEM*.TEXTELT16 being followed by STRING16)
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseProtocolType<
    protocol::requests::PolyText16::TEXTITEM16 >(
        const uint8_t* data, const size_t sz, const _Indentation&/* indents*/ ) {
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
        // TBD assume little endian for now, will change when server byte order is addressed
        const protocol::FONT font {
            __builtin_bswap32(
                *reinterpret_cast< const uint32_t* >( item.font.font_bytes ) ) };
        outputs.str += fmt::format(
            "{{ {}font={} }}",
            !settings.verbose ? "" :
            fmt::format( "font-shift={} ",
                         _formatInteger( item.font.font_shift ) ),
            _formatProtocolType( font ) );
    } else {
        assert( first_byte > 0 );
        outputs.bytes_parsed += sizeof( PolyText16::TEXTITEM16::TEXTELT16 );
        std::string hex_str {};
        for ( uint8_t i {}, m { item.text_element.m }; i < m; ++i ) {
            // TBD treating CHAR2B as CARD16, but encoding may be more
            //   complicated in standard
            const uint16_t char2b {
                *reinterpret_cast< const uint16_t* >(
                    data + outputs.bytes_parsed ) };
            hex_str += fmt::format(
                "{}{:#0{}x}", hex_str.empty() ? "" : " ",
                char2b, _fmtHexWidth( char2b ) );
            outputs.bytes_parsed += sizeof( protocol::CHAR2B );
        }
        outputs.str += fmt::format(
            " {{ {}delta={} string=[{}] }}",
            !settings.verbose ? "" :
            fmt::format( "m={} ", _formatInteger( item.text_element.m ) ),
            _formatInteger( item.text_element.delta ),
            hex_str );
    }
    return outputs;
}
