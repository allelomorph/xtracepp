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
X11ProtocolParser::_formatTIMESTAMP( const protocol::TIMESTAMP time,
                                     const Settings::Verbosity verbosity ) {
    static const std::vector<std::string_view>& enum_names {
        protocol::enum_names::time };
    static const size_t max_enum {
        protocol::enum_names::time.size() - 1 };
    // RFC 3339 UTC format:
    // https://www.rfc-editor.org/rfc/rfc3339#section-5.6
    char time_str [ sizeof( "yyyy-mm-ddThh:mm:ssZ" ) ] {};
    // TBD check return val? should == sizeof( time_str )
    strftime( time_str, sizeof( time_str ), "%FT%TZ",
              gmtime( reinterpret_cast< time_t* >( const_cast< protocol::TIMESTAMP* >( &time ) ) ) );
    const std::string name_str {
        ( time <= max_enum ) ? enum_names[ time ] : "" };
    if ( verbosity == Settings::Verbosity::Debug ) {
        // fmt counts "0x" as part of width when using '#'
        static constexpr size_t hex_width { ( sizeof( time ) * 2 ) + 2 };
        return fmt::format( "{:#0{}x} ({})", time, hex_width,
                            name_str.empty() ? time_str : name_str );
    }
    return name_str.empty() ? time_str : name_str;
}

std::string
X11ProtocolParser::_formatATOM( const protocol::ATOM atom,
                                const Settings::Verbosity verbosity ) {
    assert( _top_three_bits_zero( atom ) );
    // TBD need to handle interned atoms
    assert( atom <= protocol::atoms::PREDEFINED_MAX );
    const std::string_view& atom_strv {
        protocol::atoms::predefined[ atom ] };
    if ( verbosity == Settings::Verbosity::Debug ) {
        // fmt counts "0x" as part of width when using '#'
        static constexpr size_t hex_width { ( sizeof( atom ) * 2 ) + 2 };
        return fmt::format( "{:#0{}x} (\"{}\")", atom, hex_width, atom_strv );
    }
    return fmt::format( "\"{}\"", atom_strv );
}

std::string
X11ProtocolParser::_formatSETofPOINTEREVENT( const protocol::SETofPOINTEREVENT setofpointerevent,
                                             const Settings::Verbosity verbosity ) {
    // SETofPOINTEREVENT
    //      encodings are the same as for SETofEVENT, except with
    //      #xFFFF8003     unused but must be zero
    static constexpr size_t MAX_FLAG_I { 14 };
    return _formatBitmask( setofpointerevent, verbosity,
                           protocol::enum_names::set_of_event, MAX_FLAG_I );
}

std::string
X11ProtocolParser::_formatSETofDEVICEEVENT( const protocol::SETofDEVICEEVENT setofdeviceevent,
                                            const Settings::Verbosity verbosity ) {
    // SETofDEVICEEVENT
    //      encodings are the same as for SETofEVENT, except with
    //      #xFFFFC0B0     unused but must be zero
    static constexpr size_t MAX_FLAG_I { 13 };
    return _formatBitmask( setofdeviceevent, verbosity,
                           protocol::enum_names::set_of_event, MAX_FLAG_I );
}

std::string
X11ProtocolParser::_formatSETofKEYMASK( const protocol::SETofKEYMASK setofkeymask,
                                        const Settings::Verbosity verbosity ) {
    // SETofKEYMASK
    //      encodings are the same as for SETofKEYBUTMASK, except with
    //      #xFF00          unused but must be zero
    static constexpr size_t MAX_FLAG_I { 7 };
    return _formatBitmask( setofkeymask, verbosity,
                           protocol::enum_names::set_of_keybutmask, MAX_FLAG_I );
}

// TBD STR parsed differently
// TBD CHAR2B parsed differently

// TBD indent
std::string
X11ProtocolParser::_formatPOINT( const protocol::POINT point,
                                 const Settings::Verbosity verbosity ) {
    return fmt::format( "{{ x: {}, y: {} }}",
                        _formatInteger( point.x, verbosity ),
                        _formatInteger( point.y, verbosity ) );
}

// TBD indent
std::string
X11ProtocolParser::_formatRECTANGLE( const protocol::RECTANGLE rectangle,
                                     const Settings::Verbosity verbosity ) {
    return fmt::format( "{{ x: {}, y: {}, width: {}, height: {} }}",
                        _formatInteger( rectangle.x, verbosity ),
                        _formatInteger( rectangle.y, verbosity ),
                        _formatInteger( rectangle.width, verbosity ),
                        _formatInteger( rectangle.height, verbosity ) );
}

// TBD indent
std::string
X11ProtocolParser::_formatARC( const protocol::ARC arc,
                               const Settings::Verbosity verbosity ) {
    return fmt::format(
        "{{ x: {}, y: {}, width: {}, height: {}, angle1: {}, angle2: {} }}",
        _formatInteger( arc.x, verbosity ),
        _formatInteger( arc.y, verbosity ),
        _formatInteger( arc.width, verbosity ),
        _formatInteger( arc.height, verbosity ),
        _formatInteger( arc.angle1, verbosity ),
        _formatInteger( arc.angle2, verbosity ) );
}
