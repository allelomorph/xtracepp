#include <cassert>
#include <ctime>  // strftime gmtime
#include <cstdint>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
//#include "Settings.hpp"
#include "protocol/common_types.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/predefined_atoms.hpp"


// TBD assert( _top_three_bits_zero() ) should maybe happen instead at parse time

std::string
X11ProtocolParser::_formatCommonType(
    const protocol::TIMESTAMP time,
    const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::time );
    }
    // RFC 3339 UTC format:
    // https://www.rfc-editor.org/rfc/rfc3339#section-5.6
    char time_str [ sizeof( "yyyy-mm-ddThh:mm:ssZ" ) ] {};
    // TBD check return val? should == sizeof( time_str )
    const std::time_t time_ ( time.data );
    strftime( time_str, sizeof( time_str ), "%FT%TZ",
              gmtime( &time_ ) );
    if ( _verbose ) {
        // fmt counts "0x" as part of width when using '#'
        static constexpr size_t hex_width { ( sizeof( time.data ) * 2 ) + 2 };
        return fmt::format( "{:#0{}x}({})", time.data, hex_width,
                            time.data < enum_names.size() ?
                            enum_names[ time.data ].data() : time_str );
    }
    return time.data < enum_names.size() ?
        enum_names[ time.data ].data() : time_str;
}

std::string
X11ProtocolParser::_formatCommonType(
    const protocol::CURSOR cursor,
    const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none );
    }
    return _formatInteger( cursor.data, enum_names );
}

// COLORMAP could use zero_none or zero_copy_from_parent
std::string
X11ProtocolParser::_formatCommonType( const protocol::COLORMAP colormap,
                                      const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none ||
                enum_names == protocol::enum_names::zero_copy_from_parent );
    }
    return _formatInteger( colormap.data, enum_names );
}

std::string
X11ProtocolParser::_formatCommonType(
    Connection* conn,
    const protocol::ATOM atom,
    const std::vector< std::string_view >& enum_names /* ={}*/) {
    assert( _top_three_bits_zero( atom.data ) );
    // TBD solve GetProperty
    if ( atom.data < enum_names.size() ) {
        return _formatInteger( atom.data, enum_names );
    }
    std::string atom_string {};
    if ( atom.data <= protocol::atoms::PREDEFINED_MAX ) {
        atom_string = ( atom.data == 0 ) ?
            "unrecognized atom" :
            fmt::format( "\"{}\"", protocol::atoms::predefined[ atom.data ] );
    } else {
        auto interned_atom { conn->getInternedAtom(atom) };
        atom_string = ( interned_atom == std::nullopt ) ?
            "unrecognized atom" :
            fmt::format( "\"{}\"", *interned_atom );
    }
    if ( _verbose ) {
        // fmt counts "0x" as part of width when using '#'
        static constexpr size_t hex_width { ( sizeof( atom.data ) * 2 ) + 2 };
        return fmt::format( "{:#0{}x}({})", atom.data, hex_width, atom_string );
    }
    return atom_string;
}

// VISUALID could use zero_none or zero_copy_from_parent
std::string
X11ProtocolParser::_formatCommonType( const protocol::VISUALID visualid,
                                      const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none ||
                enum_names == protocol::enum_names::zero_copy_from_parent );
    }
    return _formatInteger( visualid.data, enum_names );
}

// WINDOW could use zero_none, event_destination, or input_focus
std::string
X11ProtocolParser::_formatCommonType( const protocol::WINDOW window,
                                      const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none ||
                enum_names == protocol::enum_names::event_destination ||
                enum_names == protocol::enum_names::input_focus );
    }
    return _formatInteger( window.data, enum_names );
}

// PIXMAP could use zero_copy_from_parent, window_attribute_background_pixmap, or zero_none
std::string
X11ProtocolParser::_formatCommonType( const protocol::PIXMAP pixmap,
                                      const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_copy_from_parent ||
                enum_names == protocol::enum_names::window_attribute_background_pixmap ||
                enum_names == protocol::enum_names::zero_none );
    }
    return _formatInteger( pixmap.data, enum_names );
}

std::string
X11ProtocolParser::_formatCommonType(
    const protocol::FONT font,
    const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none );
    }
    return _formatInteger( font.data, enum_names );
}

std::string
X11ProtocolParser::_formatCommonType(
    const protocol::SETofEVENT setofevent ) {
    // SETofEVENT
    //     #xFE000000     unused but must be zero
    static constexpr uint32_t UNUSED_BITS { 0xfe000000 };
    assert( ( setofevent.data & UNUSED_BITS ) == 0 );
    return _formatBitmask( setofevent.data,
                           protocol::enum_names::set_of_event );
}

std::string
X11ProtocolParser::_formatCommonType(
    const protocol::SETofPOINTEREVENT setofpointerevent ) {
    // SETofPOINTEREVENT
    //      encodings are the same as for SETofEVENT, except with
    //      #xFFFF8003     unused but must be zero
    static constexpr uint32_t UNUSED_BITS { 0xffff8003 };
    assert( ( setofpointerevent.data & UNUSED_BITS ) == 0 );
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
    static constexpr uint32_t UNUSED_BITS { 0xffffc0b0 };
    assert( ( setofdeviceevent.data & UNUSED_BITS ) == 0 );
    static constexpr size_t MAX_FLAG_I { 13 };
    return _formatBitmask( setofdeviceevent.data,
                           protocol::enum_names::set_of_event, MAX_FLAG_I );
}

std::string
X11ProtocolParser::_formatCommonType(
    const protocol::KEYCODE keycode,
    const std::vector< std::string_view >& enum_names /*= {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::key );
    }
    return _formatInteger( keycode.data, enum_names );
}

std::string
X11ProtocolParser::_formatCommonType(
    const protocol::BUTTON button,
    const std::vector< std::string_view >& enum_names /*= {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::button );
    }
    return _formatInteger( button.data, enum_names );
}

std::string
X11ProtocolParser::_formatCommonType(
    const protocol::SETofKEYMASK setofkeymask ) {
    // SETofKEYMASK
    //      encodings are the same as for SETofKEYBUTMASK, except with
    //      #xFF00          unused but must be zero
    // TBD exception in Requests 28 GrabButton 29 UngrabButton 33 GrabKey 34 UngrabKey: 0x8000 == AnyModifier
    // TBD this breaks current code paradigm in two ways:
    //    - where to keep "AnyModifier" string in enum_names?
    //    - have to bypass _formatInteger due to its only accommodating enums that start at 0
    // TBD could make some sort of list of exception flags for mask types that result in enum string
    //     this seems to be the only exception to normal SETof(KEY|BUTTON|KEYBUT)MASK formatting
    if ( setofkeymask.data == 0x8000 ) {
        const std::string name_str { "AnyModifier" };
        if ( _verbose ) {
            // fmt counts "0x" as part of width when using '#'
            static constexpr size_t hex_width { ( sizeof( setofkeymask.data ) * 2 ) + 2 };
            const std::string hex_str { fmt::format( "{:#0{}x}", setofkeymask.data, hex_width ) };
            return fmt::format( "{}({})", hex_str, name_str );
        }
        return name_str;
    }
    static constexpr uint16_t UNUSED_BITS { 0xff00 };
    assert( ( setofkeymask.data & UNUSED_BITS ) == 0 );
    static constexpr size_t MAX_FLAG_I { 7 };
    return _formatBitmask( setofkeymask.data,
                           protocol::enum_names::set_of_keybutmask, MAX_FLAG_I );
}

std::string
X11ProtocolParser::_formatCommonType(
    const protocol::SETofKEYBUTMASK setofkeybutmask ) {
    // SETofKEYBUTMASK
    //   #xE000     unused but must be zero
    static constexpr uint16_t UNUSED_BITS { 0xe000 };
    assert( ( setofkeybutmask.data & UNUSED_BITS ) == 0 );
    return _formatBitmask( setofkeybutmask.data,
                           protocol::enum_names::set_of_keybutmask );
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
