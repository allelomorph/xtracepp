#include <cassert>
#include <ctime>  // strftime gmtime
#include <cstdint>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
//#include "Settings.hpp"
#include "Connection.hpp"
#include "protocol/common_types.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/predefined_atoms.hpp"
#include "protocol/requests.hpp"


// TBD assert( _top_three_bits_zero() ) should maybe happen instead at parse time
// TBD consider passing vector pointer instead of reference to allow for null
//   checks (use std::optional?) and have lighter weight default ctor

// WINDOW
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::WINDOW window,
    const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    assert( ( window.data & protocol::WINDOW::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none ||
                enum_names == protocol::enum_names::event_destination ||
                enum_names == protocol::enum_names::input_focus );
    }
    return _formatInteger( window.data );
}

// PIXMAP
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::PIXMAP pixmap,
    const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    assert( ( pixmap.data & protocol::PIXMAP::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_copy_from_parent ||
                enum_names == protocol::enum_names::window_attribute_background_pixmap ||
                enum_names == protocol::enum_names::zero_none );
    }
    return _formatInteger( pixmap.data, enum_names );
}

// CURSOR
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::CURSOR cursor,
    const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    assert( ( cursor.data & protocol::CURSOR::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none );
    }
    return _formatInteger( cursor.data, enum_names );
}

// FONT
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::FONT font,
    const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    assert( ( font.data & protocol::FONT::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none );
    }
    return _formatInteger( font.data, enum_names );
}

// TBD linker error if inline
// GCONTEXT
// TBD want to inline due to no examples found in standard of named enum
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::GCONTEXT gcontext,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    assert( ( gcontext.data & protocol::GCONTEXT::ZERO_BITS ) == 0 );
    return _formatInteger( gcontext.data );
}

// COLORMAP
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::COLORMAP colormap,
    const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    assert( ( colormap.data & protocol::COLORMAP::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none ||
                enum_names == protocol::enum_names::zero_copy_from_parent );
    }
    return _formatInteger( colormap.data, enum_names );
}

// TBD linker error if inline
// DRAWABLE WINDOW or PIXMAP
// TBD seemingly never used with enum names in protocol?
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::DRAWABLE drawable,
    const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    assert( ( drawable.window.data & protocol::WINDOW::ZERO_BITS ) == 0 );
    return _formatInteger( drawable.window.data, enum_names );
}

// FONTABLE FONT or GCONTEXT
// TBD FONTABLE does not appear in core protocol encoding?

// ATOM
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::ATOM atom,
    const std::vector< std::string_view >& enum_names /* ={}*/) {
    assert( ( atom.data & protocol::ATOM::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none ||
                enum_names == protocol::enum_names::property_atom );
    }
    if ( atom.data < enum_names.size() ) {
        return _formatInteger( atom.data, enum_names );
    }
    std::string atom_string {};
    if ( atom.data <= protocol::atoms::PREDEFINED_MAX ) {
        atom_string = ( atom.data == 0 ) ?
            "unrecognized atom" :
            fmt::format( "\"{}\"", protocol::atoms::predefined[ atom.data ] );
    } else {
        auto interned_atom { _getInternedAtom(atom) };
        atom_string = ( interned_atom == std::nullopt ) ?
            "unrecognized atom" :
            fmt::format( "\"{}\"", *interned_atom );
    }
    return fmt::format( "{}({})", _formatInteger( atom.data ), atom_string );
}

// VISUALID
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::VISUALID visualid,
    const std::vector< std::string_view >& enum_names/* = {}*/ ) {
    assert( ( visualid.data & protocol::VISUALID::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none ||
                enum_names == protocol::enum_names::zero_copy_from_parent );
    }
    return _formatInteger( visualid.data, enum_names );
}

// BYTE
// INT8
// INT16
// INT32
// CARD8
// CARD16
// CARD32

// TIMESTAMP
template <>
std::string
X11ProtocolParser::_formatProtocolType(
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

// TBD linker error if inline
// BITGRAVITY
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::BITGRAVITY bitgravity,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    return _formatInteger( bitgravity.data,
                           protocol::enum_names::bitgravity );
}

// TBD linker error if inline
// WINGRAVITY
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::WINGRAVITY wingravity,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    return _formatInteger( wingravity.data,
                           protocol::enum_names::wingravity );
}

// TBD linker error if inline
// BOOL
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::BOOL bool_,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    return _formatInteger( bool_.data,
                           protocol::enum_names::bool_ );
}

// EVENT
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofEVENT setofevent,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    assert( ( setofevent.data & protocol::SETofEVENT::ZERO_BITS ) == 0 );
    return _formatBitmask( setofevent.data,
                           protocol::enum_names::set_of_event );
}

// POINTEREVENT
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofPOINTEREVENT setofpointerevent,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    assert( ( setofpointerevent.data & protocol::SETofPOINTEREVENT::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatBitmask( setofpointerevent.data,
                           protocol::enum_names::set_of_event );
}

// DEVICEEVENT
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofDEVICEEVENT setofdeviceevent,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    assert( ( setofdeviceevent.data & protocol::SETofDEVICEEVENT::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatBitmask( setofdeviceevent.data,
                           protocol::enum_names::set_of_event );
}

// KEYSYM
// TBD print as byte for now

// KEYCODE
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::KEYCODE keycode,
    const std::vector< std::string_view >& enum_names /*= {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::key );
    }
    return _formatInteger( keycode.data, enum_names );
}

// BUTTON
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::BUTTON button,
    const std::vector< std::string_view >& enum_names /*= {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::button );
    }
    return _formatInteger( button.data, enum_names );
}

// KEYMASK
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofKEYMASK setofkeymask,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    // TBD exception in Requests 28 GrabButton 29 UngrabButton 33 GrabKey 34 UngrabKey: 0x8000 == AnyModifier
    // TBD this breaks current code paradigm in two ways:
    //    - where to keep "AnyModifier" string in enum_names?
    //    - have to bypass _formatInteger due to its only accommodating enums that start at 0
    // TBD could make some sort of list of exception flags for mask types that result in enum string
    //     this seems to be the only exception to normal SETof(KEY|BUTTON|KEYBUT)MASK formatting
    if ( setofkeymask.data == protocol::SETofKEYMASK::ANYMODIFIER ) {
        const std::string name_str { "AnyModifier" };
        if ( _verbose ) {
            // fmt counts "0x" as part of width when using '#'
            static constexpr size_t hex_width { ( sizeof( setofkeymask.data ) * 2 ) + 2 };
            const std::string hex_str { fmt::format( "{:#0{}x}", setofkeymask.data, hex_width ) };
            return fmt::format( "{}({})", hex_str, name_str );
        }
        return name_str;
    }
    assert( ( setofkeymask.data & protocol::SETofKEYMASK::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatBitmask( setofkeymask.data,
                           protocol::enum_names::set_of_keybutmask );
}

// BUTMASK
// TBD no examples of SETofBUTMASK found in core encoding

// KEYBUTMASK
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofKEYBUTMASK setofkeybutmask,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    assert( ( setofkeybutmask.data & protocol::SETofKEYBUTMASK::ZERO_BITS ) == 0 );
    return _formatBitmask( setofkeybutmask.data,
                           protocol::enum_names::set_of_keybutmask );
}

// STRING8
// STRING16
// CHAR2B
// TBD CHAR2B parsed differently

// POINT
// TBD indent
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::POINT point,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    return fmt::format( "{{ x: {}, y: {} }}",
                        _formatInteger( point.x ),
                        _formatInteger( point.y ) );
}

// RECTANGLE
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::RECTANGLE rectangle,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    return fmt::format( "{{ x: {}, y: {}, width: {}, height: {} }}",
                        _formatInteger( rectangle.x ),
                        _formatInteger( rectangle.y ),
                        _formatInteger( rectangle.width ),
                        _formatInteger( rectangle.height ) );
}

// ARC
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::ARC arc,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    return fmt::format(
        "{{ x: {}, y: {}, width: {}, height: {}, angle1: {}, angle2: {} }}",
        _formatInteger( arc.x ),
        _formatInteger( arc.y ),
        _formatInteger( arc.width ),
        _formatInteger( arc.height ),
        _formatInteger( arc.angle1 ),
        _formatInteger( arc.angle2 ) );
}

// HOST
// TBD need to break out from current _parseLISTofHOST

// STR
// TBD need to break out from current _parseLISTofSTR

// ==============================

template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::QueryFont::CHARINFO charinfo,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    return fmt::format(
        "{{ left-side-bearing: {}, right-side-bearing: {}, character-width: {}, ascent: {} descent: {} attributes: {} }}",
        _formatInteger( charinfo.left_side_bearing ),
        _formatInteger( charinfo.right_side_bearing ),
        _formatInteger( charinfo.character_width ),
        _formatInteger( charinfo.ascent ),
        _formatInteger( charinfo.descent ),
        _formatInteger( charinfo.attributes ) );
}

template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::QueryFont::FONTPROP fontprop,
    const std::vector< std::string_view >& /*enum_names = {}*/ ) {
    return fmt::format(
        "{{ name: {}, value: {} }}",
        _formatProtocolType( fontprop.name ),
        _formatInteger( fontprop.value ) );
}

// TBD belong here:
// ServerAcceptance::Format (rename to FORMAT)
// ServerAcceptance::Screen::Depth::VisualType (rename to VISUALTYPE)
// requests::PolySegment::SEGMENT
// requests::StoreColors::COLORITEM
// requests::GetMotionEvents::TIMECOORD
// requests::QueryColors::RGB


// TBD need parsing (return _ParsingOutputs) instead of just formatting:
// STR (only in LISTofSTR? may need custom func _parseSTR)
// ServerAcceptance::Screen (rename to SCREEN)
// ServerAcceptance::Screen::Depth (rename to DEPTH)
// requests::PolyText8::TEXTITEM8 (need parsing due to TEXTITEM*.TEXTELT8 being followed by STRING8)
// requests::PolyText16::TEXTITEM16 (need parsing due to TEXTITEM*.TEXTELT16 being followed by STRING16)
// protocol::HOST (followed by n LISTofBYTE)
