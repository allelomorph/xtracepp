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
#include "protocol/connection_setup.hpp"


// TBD zero bit validation should maybe happen instead at parse time
// TBD consider passing vector pointer instead of reference to allow for null
//   checks (use std::optional?) and have lighter weight default ctor

// WINDOW
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::WINDOW window,
    const std::vector< std::string_view >& enum_names ) {
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
    const std::vector< std::string_view >& enum_names ) {
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
    const std::vector< std::string_view >& enum_names ) {
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
    const std::vector< std::string_view >& enum_names ) {
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
    const std::vector< std::string_view >& /*enum_names*/ ) {
    assert( ( gcontext.data & protocol::GCONTEXT::ZERO_BITS ) == 0 );
    return _formatInteger( gcontext.data );
}

// COLORMAP
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::COLORMAP colormap,
    const std::vector< std::string_view >& enum_names ) {
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
    const std::vector< std::string_view >& enum_names ) {
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
    const std::vector< std::string_view >& enum_names ) {
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
    const std::vector< std::string_view >& enum_names ) {
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
    const std::vector< std::string_view >& enum_names ) {
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
    if ( settings.verbose ) {
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
    const std::vector< std::string_view >& /*enum_names*/ ) {
    return _formatInteger( bitgravity.data,
                           protocol::BITGRAVITY::enum_names );
}

// TBD linker error if inline
// WINGRAVITY
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::WINGRAVITY wingravity,
    const std::vector< std::string_view >& /*enum_names*/ ) {
    return _formatInteger( wingravity.data,
                           protocol::WINGRAVITY::enum_names );
}

// TBD linker error if inline
// BOOL
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::BOOL bool_,
    const std::vector< std::string_view >& /*enum_names*/ ) {
    return _formatInteger( bool_.data,
                           protocol::BOOL::enum_names );
}

// EVENT
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofEVENT setofevent,
    const std::vector< std::string_view >& /*enum_names*/ ) {
    assert( ( setofevent.data & protocol::SETofEVENT::ZERO_BITS ) == 0 );
    return _formatBitmask( setofevent.data,
                           protocol::SETofEVENT::flag_names );
}

// POINTEREVENT
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofPOINTEREVENT setofpointerevent,
    const std::vector< std::string_view >& /*enum_names*/ ) {
    assert( ( setofpointerevent.data & protocol::SETofPOINTEREVENT::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatBitmask( setofpointerevent.data,
                           protocol::SETofPOINTEREVENT::flag_names );
}

// DEVICEEVENT
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofDEVICEEVENT setofdeviceevent,
    const std::vector< std::string_view >& /*enum_names*/ ) {
    assert( ( setofdeviceevent.data & protocol::SETofDEVICEEVENT::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatBitmask( setofdeviceevent.data,
                           protocol::SETofDEVICEEVENT::flag_names );
}

// KEYSYM
// TBD print as hex for now, as encoding is convoluted, see Appendix A. KEYSYM Encoding
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::KEYSYM keysym,
    const std::vector< std::string_view >&/* enum_names*/ ) {
    return _formatInteger( keysym.data );
}

// KEYCODE
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::KEYCODE keycode,
    const std::vector< std::string_view >& enum_names ) {
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
    const std::vector< std::string_view >& enum_names ) {
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
    const std::vector< std::string_view >& /*enum_names*/ ) {
    // TBD exception in Requests 28 GrabButton 29 UngrabButton 33 GrabKey 34 UngrabKey: 0x8000 == AnyModifier
    // TBD this breaks current code paradigm in two ways:
    //    - where to keep "AnyModifier" string in enum_names?
    //    - have to bypass _formatInteger due to its only accommodating enums that start at 0
    // TBD could make some sort of list of exception flags for mask types that result in enum string
    //     this seems to be the only exception to normal SETof(KEY|BUTTON|KEYBUT)MASK formatting
    if ( setofkeymask.data == protocol::SETofKEYMASK::ANYMODIFIER ) {
        if ( settings.verbose ) {
            const std::string hex_str {
                fmt::format( "{:#0{}x}", setofkeymask.data,
                             _fmtHexWidth( setofkeymask.data ) ) };
            return fmt::format( "{}({})", hex_str,
                                protocol::SETofKEYMASK::anymodifier_flag_name );
        }
        return std::string( protocol::SETofKEYMASK::anymodifier_flag_name );
    }
    assert( ( setofkeymask.data & protocol::SETofKEYMASK::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatBitmask( setofkeymask.data,
                           protocol::SETofKEYMASK::flag_names );
}

// BUTMASK
// TBD no examples of SETofBUTMASK found in core encoding

// KEYBUTMASK
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofKEYBUTMASK setofkeybutmask,
    const std::vector< std::string_view >& /*enum_names*/ ) {
    assert( ( setofkeybutmask.data & protocol::SETofKEYBUTMASK::ZERO_BITS ) == 0 );
    return _formatBitmask( setofkeybutmask.data,
                           protocol::SETofKEYBUTMASK::flag_names );
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
    const protocol::POINT point, const _Indentation&/* indents*/ ) {
    return fmt::format( "{{ x={} y={} }}",
                        _formatInteger( point.x ),
                        _formatInteger( point.y ) );
}

// RECTANGLE
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::RECTANGLE rectangle, const _Indentation&/* indents*/ ) {
    return fmt::format( "{{ x={} y={} width={} height={} }}",
                        _formatInteger( rectangle.x ),
                        _formatInteger( rectangle.y ),
                        _formatInteger( rectangle.width ),
                        _formatInteger( rectangle.height ) );
}

// ARC
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::ARC arc, const _Indentation&/* indents*/ ) {
    return fmt::format(
        "{{ x={} y={} width={} height={} angle1={} angle2={} }}",
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
    const _Indentation&/* indents*/ ) {
    return fmt::format(
        "{{ {}={} {}={} {}={} {}={} {}={} {}={} }}",
        "left-side-bearing",  _formatInteger( charinfo.left_side_bearing ),
        "right-side-bearing", _formatInteger( charinfo.right_side_bearing ),
        "character-width",    _formatInteger( charinfo.character_width ),
        "ascent",             _formatInteger( charinfo.ascent ),
        "descent",            _formatInteger( charinfo.descent ),
        "attributes",         _formatInteger( charinfo.attributes ) );
}

template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::QueryFont::FONTPROP fontprop,
    const _Indentation&/* indents*/ ) {
    return fmt::format(
        "{{ name={} value={} }}",
        _formatProtocolType( fontprop.name ),
        _formatInteger( fontprop.value ) );
}

// TBD belong here:
// ServerAcceptance::Format (rename to FORMAT)
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::connection_setup::ServerAcceptance::FORMAT format,
    const _Indentation&/* indents*/ ) {
    return fmt::format(
        "{{ depth={} bits-per-pixel={} scanline-pad={} }}",
        _formatInteger( format.depth ),
        _formatInteger( format.bits_per_pixel ),
        _formatInteger( format.scanline_pad ) );
}

// ServerAcceptance::Screen::Depth::VisualType (rename to VISUALTYPE)
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::connection_setup::ServerAcceptance::SCREEN::DEPTH::VISUALTYPE visualtype,
    const _Indentation&/* indents*/ ) {
    return fmt::format(
        "{{ {}={} {}={} {}={} {}={} {}={} {}={} {}={} }}",
        "visual-id",          _formatProtocolType( visualtype.visual_id ),
        "class",              _formatInteger(
            visualtype.class_,
            protocol::connection_setup::ServerAcceptance::SCREEN::DEPTH::VISUALTYPE::class_names ),
        "bits-per-rgb-value", _formatInteger( visualtype.bits_per_rgb_value ),
        "colormap-entries",   _formatInteger( visualtype.colormap_entries ),
        "red-mask",           _formatBitmask( visualtype.red_mask ),
        "green-mask",         _formatBitmask( visualtype.green_mask ),
        "blue-mask",          _formatBitmask( visualtype.blue_mask ) );
}

// requests::PolySegment::SEGMENT
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::PolySegment::SEGMENT segment,
    const _Indentation&/* indents*/ ) {
    return fmt::format(
        "{{ x1={} y1={} x2={} y2={} }}",
        _formatInteger( segment.x1 ),
        _formatInteger( segment.y1 ),
        _formatInteger( segment.x2 ),
        _formatInteger( segment.y2 ) );
}

// requests::StoreColors::COLORITEM
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::StoreColors::COLORITEM coloritem,
    const _Indentation&/* indents*/ ) {
    return fmt::format(
        " {{ pixel={} red={} green={} blue={} do rgb={} }}",
        _formatInteger( coloritem.pixel ),
        _formatInteger( coloritem.red ),
        _formatInteger( coloritem.green ),
        _formatInteger( coloritem.blue ),
        _formatBitmask( coloritem.do_rgb_mask,
                        protocol::requests::StoreColors::do_rgb_names ) );
}

// requests::GetMotionEvents::TIMECOORD
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::GetMotionEvents::TIMECOORD timecoord,
    const _Indentation&/* indents*/ ) {
    return fmt::format(
        " {{ time={} x={} y={} }}",
        _formatProtocolType( timecoord.time ),
        _formatInteger( timecoord.x ),
        _formatInteger( timecoord.y ) );
}

// requests::QueryColors::RGB
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::QueryColors::RGB rgb,
    const _Indentation&/* indents*/ ) {
    return fmt::format(
        " {{ red={} green={} blue={} }}",
        _formatInteger( rgb.red ),
        _formatInteger( rgb.green ),
        _formatInteger( rgb.blue ) );
}
