#include <cassert>
#include <ctime>  // strftime gmtime
#include <cstdint>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Settings.hpp"
#include "protocol/common_types.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/predefined_atoms.hpp"


// TBD assert( _top_three_bits_zero() ) should maybe happen instead at parse time

std::string
X11ProtocolParser::_formatCommonType( const protocol::TIMESTAMP time ) {
    static const std::vector<std::string_view>& enum_names {
        protocol::enum_names::time };
    static const size_t max_enum {
        protocol::enum_names::time.size() - 1 };
    // RFC 3339 UTC format:
    // https://www.rfc-editor.org/rfc/rfc3339#section-5.6
    char time_str [ sizeof( "yyyy-mm-ddThh:mm:ssZ" ) ] {};
    // TBD check return val? should == sizeof( time_str )
    const std::time_t _time ( time.data );
    strftime( time_str, sizeof( time_str ), "%FT%TZ",
              gmtime( &_time ) );
    const std::string name_str {
        ( time.data <= max_enum ) ? enum_names[ time.data ] : "" };
    if ( _verbosity == Settings::Verbosity::Debug ) {
        // fmt counts "0x" as part of width when using '#'
        static constexpr size_t hex_width { ( sizeof( time.data ) * 2 ) + 2 };
        return fmt::format( "{:#0{}x} ({})", time.data, hex_width,
                            name_str.empty() ? time_str : name_str );
    }
    return name_str.empty() ? time_str : name_str;
}

std::string
X11ProtocolParser::_formatCommonType( const protocol::ATOM atom ) {
    assert( _top_three_bits_zero( atom.data ) );
    // TBD need to handle interned atoms
    assert( atom.data <= protocol::atoms::PREDEFINED_MAX );
    const std::string_view& atom_strv {
        protocol::atoms::predefined[ atom.data ] };
    if ( _verbosity == Settings::Verbosity::Debug ) {
        // fmt counts "0x" as part of width when using '#'
        static constexpr size_t hex_width { ( sizeof( atom.data ) * 2 ) + 2 };
        return fmt::format( "{:#0{}x} (\"{}\")", atom.data, hex_width, atom_strv );
    }
    return fmt::format( "\"{}\"", atom_strv );
}

std::string
X11ProtocolParser::_formatCommonType(
    const protocol::SETofPOINTEREVENT setofpointerevent ) {
    // SETofPOINTEREVENT
    //      encodings are the same as for SETofEVENT, except with
    //      #xFFFF8003     unused but must be zero
    static constexpr size_t MAX_FLAG_I { 14 };
    return _formatBitmask( setofpointerevent.data,
                           protocol::enum_names::set_of_event, MAX_FLAG_I );
}

std::string
X11ProtocolParser::_formatCommonType(
    const protocol::SETofDEVICEEVENT setofdeviceevent ) {
    // SETofDEVICEEVENT
    //      encodings are the same as for SETofEVENT, except with
    //      #xFFFFC0B0     unused but must be zero
    static constexpr size_t MAX_FLAG_I { 13 };
    return _formatBitmask( setofdeviceevent.data,
                           protocol::enum_names::set_of_event, MAX_FLAG_I );
}

std::string
X11ProtocolParser::_formatCommonType(
    const protocol::SETofKEYMASK setofkeymask ) {
    // SETofKEYMASK
    //      encodings are the same as for SETofKEYBUTMASK, except with
    //      #xFF00          unused but must be zero
    static constexpr size_t MAX_FLAG_I { 7 };
    return _formatBitmask( setofkeymask.data,
                           protocol::enum_names::set_of_keybutmask, MAX_FLAG_I );
}

// TBD STR parsed differently
// TBD CHAR2B parsed differently

// TBD indent
std::string
X11ProtocolParser::_formatCommonType( const protocol::POINT point ) {
    return fmt::format( "{{ x: {}, y: {} }}",
                        _formatInteger( point.x ),
                        _formatInteger( point.y ) );
}

// TBD indent
std::string
X11ProtocolParser::_formatCommonType( const protocol::RECTANGLE rectangle ) {
    return fmt::format( "{{ x: {}, y: {}, width: {}, height: {} }}",
                        _formatInteger( rectangle.x ),
                        _formatInteger( rectangle.y ),
                        _formatInteger( rectangle.width ),
                        _formatInteger( rectangle.height ) );
}

// TBD indent
std::string
X11ProtocolParser::_formatCommonType( const protocol::ARC arc ) {
    return fmt::format(
        "{{ x: {}, y: {}, width: {}, height: {}, angle1: {}, angle2: {} }}",
        _formatInteger( arc.x ),
        _formatInteger( arc.y ),
        _formatInteger( arc.width ),
        _formatInteger( arc.height ),
        _formatInteger( arc.angle1 ),
        _formatInteger( arc.angle2 ) );
}
