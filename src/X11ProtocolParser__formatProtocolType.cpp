#include <cassert>
#include <ctime>  // localtime
#include <cstdint>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include "X11ProtocolParser.hpp"
//#include "Settings.hpp"
#include "Connection.hpp"
#include "protocol/common_types.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/atoms.hpp"
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
    // TBD one of two arrays should store the ATOMs known before starting the
    //   client queue, with a map as backup for any ATOMs identified while parsing
    //   the queue that are not in contiguous index order with the rest
    if ( settings.prefetchatoms &&
         atom.data < _seq_interned_atoms.size() ) {
        atom_string =
            fmt::format( "\"{}\"", _seq_interned_atoms[ atom.data ] );
    } else if ( atom.data <= protocol::atoms::predefined::MAX ) {
        atom_string =
            fmt::format( "\"{}\"", protocol::atoms::predefined::strings[ atom.data ] );
    } else {
        auto it { _nonseq_interned_atoms.find( atom.data ) };
        if ( it == _nonseq_interned_atoms.end() )
            atom_string = "unrecognized atom";
        else
            atom_string = it->second;
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
    std::string fmt_str {
        fmt::format( "{:#0{}x}", time.data, _fmtHexWidth( time.data ) ) };
    if ( time.data < enum_names.size() ) {
        fmt_str.append(
            fmt::format( "({})", enum_names[ time.data ] ) );
    } else if ( settings.relativetimestamps ) {
        // RFC 3339 UTC format "yyyy-mm-ddThh:mm:ssZ":
        // https://www.rfc-editor.org/rfc/rfc3339#section-5.6
        static constexpr uint32_t TIMESTAMP_TICKS_PER_SEC { 1000 };
        const std::time_t time_ (
            ( ( time.data - settings.ref_TIMESTAMP ) / TIMESTAMP_TICKS_PER_SEC )
            + settings.ref_unix_time );
        fmt_str.append(
            fmt::format( "({:%FT:%TZ}UTC)", *std::localtime( &time_ ) ) );
    }
    return fmt_str;
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
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::CHAR2B char2b, const _Whitespace&/* ws*/ ) {
    // printing char16_t vals as hex, same as xtrace
    // TBD is there a way to convert to printable UTF-8 (with c16rtomb, for example)?
    // TBD STRING16 encoding seems quite tangled, from standard:
    //     "The primary interpretation of large characters in a STRING16 is that they are
    // composed of two bytes used to index a two-dimensional matrix, hence, the use of
    // CHAR2B rather than CARD16. This corresponds to the JIS/ISO method of indexing
    // 2-byte characters. It is expected that most large fonts will be defined with 2-byte
    // matrix indexing. For large fonts constructed with linear indexing, a CHAR2B can
    // be interpreted as a 16-bit number by treating byte1 as the most significant byte.
    // This means that clients should always transmit such 16-bit character values most
    // significant byte first, as the server will never byte-swap CHAR2B quantities."
    return _formatInteger( *reinterpret_cast< const uint16_t* >( &char2b ) );
}

// POINT
// TBD indent
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::POINT point, const _Whitespace&/* ws*/ ) {
    return fmt::format( "{{ x={} y={} }}",
                        _formatInteger( point.x ),
                        _formatInteger( point.y ) );
}

// RECTANGLE
template <>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::RECTANGLE rectangle, const _Whitespace&/* ws*/ ) {
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
    const protocol::ARC arc, const _Whitespace&/* ws*/ ) {
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
    const _Whitespace&/* ws*/ ) {
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
    const _Whitespace&/* ws*/ ) {
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
    const _Whitespace&/* ws*/ ) {
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
    const _Whitespace&/* ws*/ ) {
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
    const _Whitespace&/* ws*/ ) {
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
    const _Whitespace&/* ws*/ ) {
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
    const _Whitespace&/* ws*/ ) {
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
    const _Whitespace&/* ws*/ ) {
    return fmt::format(
        " {{ red={} green={} blue={} }}",
        _formatInteger( rgb.red ),
        _formatInteger( rgb.green ),
        _formatInteger( rgb.blue ) );
}
