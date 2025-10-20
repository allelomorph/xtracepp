#include <cassert>
#include <ctime>  // localtime
#include <cstdint>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/common_types.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/atoms.hpp"
#include "protocol/requests.hpp"
#include "protocol/connection_setup.hpp"


// TBD do partial specialization for std::is_base_of< protocol::impl::XID > in header
//   (ATOM is only XID with exceptional handling)
template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::WINDOW window,
    const std::vector< std::string_view >& enum_names/* = _NO_NAMES*/,
    _IndexRange name_index_range/* = {}*/ ) {
    assert( ( window.data & protocol::WINDOW::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none ||
                enum_names == protocol::requests::SendEvent::destination_names ||
                enum_names == protocol::enum_names::input_focus );
    }
    return _formatInteger( window.data, enum_names, name_index_range );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::PIXMAP pixmap,
    const std::vector< std::string_view >& enum_names/* = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    assert( ( pixmap.data & protocol::PIXMAP::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_copy_from_parent ||
                enum_names == protocol::enum_names::window_attribute_background_pixmap ||
                enum_names == protocol::enum_names::zero_none );
    }
    return _formatInteger( pixmap.data, enum_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::CURSOR cursor,
    const std::vector< std::string_view >& enum_names/* = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    assert( ( cursor.data & protocol::CURSOR::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none );
    }
    return _formatInteger( cursor.data, enum_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::FONT font,
    const std::vector< std::string_view >& enum_names/* = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/) {
    assert( ( font.data & protocol::FONT::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none );
    }
    return _formatInteger( font.data, enum_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::GCONTEXT gcontext,
    const std::vector< std::string_view >&/* enum_names = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    assert( ( gcontext.data & protocol::GCONTEXT::ZERO_BITS ) == 0 );
    return _formatInteger( gcontext.data );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::COLORMAP colormap,
    const std::vector< std::string_view >& enum_names/* = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    assert( ( colormap.data & protocol::COLORMAP::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none ||
                enum_names == protocol::enum_names::zero_copy_from_parent );
    }
    return _formatInteger( colormap.data, enum_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::DRAWABLE drawable,
    const std::vector< std::string_view >& enum_names/* = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    // defaulting to treating like WINDOW
    assert( ( drawable.window.data & protocol::WINDOW::ZERO_BITS ) == 0 );
    return _formatInteger( drawable.window.data, enum_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::FONTABLE fontable,
    const std::vector< std::string_view >& enum_names/* = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    // defaulting to treating like FONT
    assert( ( fontable.font.data & protocol::FONT::ZERO_BITS ) == 0 );
    return _formatInteger( fontable.font.data, enum_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::ATOM atom,
    const std::vector< std::string_view >& enum_names/* = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    assert( ( atom.data & protocol::ATOM::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none ||
                enum_names == protocol::requests::GetProperty::type_names );
    }
    if ( atom.data < enum_names.size() ) {
        return _formatInteger( atom.data, enum_names );
    }
    std::string atom_string {};
    // TBD one of two arrays should store the ATOMs known before starting the
    //   client queue, with a map as backup for any ATOMs identified while parsing
    //   the queue that are not in contiguous index order with the rest
    // TBD this should be simplified into just one map of ATOM to atom, populated by
    //   protocol::atoms::predefined::strings plus optionally prefetched atoms and
    //   anything interned in this session by the user
    if ( settings.prefetchatoms &&
         atom.data < _seq_interned_atoms.size() ) {
        atom_string =
            fmt::format( "{:?}", _seq_interned_atoms[ atom.data ] );
    } else if ( atom.data <= protocol::atoms::predefined::MAX ) {
        atom_string =
            fmt::format( "{:?}", protocol::atoms::predefined::strings[ atom.data ] );
    } else {
        auto it { _nonseq_interned_atoms.find( atom.data ) };
        if ( it == _nonseq_interned_atoms.end() )
            atom_string = "unrecognized atom";
        else
            atom_string = it->second;
    }
    return fmt::format( "{}({})", _formatInteger( atom.data ), atom_string );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::VISUALID visualid,
    const std::vector< std::string_view >& enum_names/* = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    assert( ( visualid.data & protocol::VISUALID::ZERO_BITS ) == 0 );
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::zero_none ||
                enum_names == protocol::enum_names::zero_copy_from_parent );
    }
    return _formatInteger( visualid.data, enum_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::TIMESTAMP time,
    const std::vector< std::string_view >& enum_names/* = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::time );
    }
    std::string fmt_str { _formatBitmask( time.data ) };  // force hex
    if ( time.data < enum_names.size() ) {
        fmt_str.append(
            fmt::format( "({})", enum_names[ time.data ] ) );
    } else if ( settings.relativetimestamps ) {
        static constexpr uint32_t TIMESTAMP_TICKS_PER_SEC { 1000 };
        const std::time_t time_ (
            ( ( time.data - settings.ref_TIMESTAMP ) / TIMESTAMP_TICKS_PER_SEC )
            + settings.ref_unix_time );
        // RFC 3339 UTC format "yyyy-mm-ddThh:mm:ssZ":
        // https://www.rfc-editor.org/rfc/rfc3339#section-5.6
        fmt_str.append(
            fmt::format( "({:%FT:%TZ}UTC)", *std::localtime( &time_ ) ) );
    }
    return fmt_str;
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::BITGRAVITY bitgravity,
    const std::vector< std::string_view >&/* enum_names = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    return _formatInteger( bitgravity.data,
                           protocol::BITGRAVITY::enum_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::WINGRAVITY wingravity,
    const std::vector< std::string_view >&/* enum_names = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    return _formatInteger( wingravity.data,
                           protocol::WINGRAVITY::enum_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::BOOL bool_,
    const std::vector< std::string_view >&/* enum_names = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    return _formatInteger( bool_.data,
                           protocol::BOOL::enum_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofEVENT setofevent,
    const std::vector< std::string_view >&/* enum_names = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    assert( ( setofevent.data & protocol::SETofEVENT::ZERO_BITS ) == 0 );
    return _formatBitmask( setofevent.data,
                           protocol::SETofEVENT::flag_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofPOINTEREVENT setofpointerevent,
    const std::vector< std::string_view >&/* enum_names = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    assert( ( setofpointerevent.data &
              protocol::SETofPOINTEREVENT::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatBitmask( setofpointerevent.data,
                           protocol::SETofPOINTEREVENT::flag_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofDEVICEEVENT setofdeviceevent,
    const std::vector< std::string_view >&/* enum_names = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    assert( ( setofdeviceevent.data &
              protocol::SETofDEVICEEVENT::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatBitmask( setofdeviceevent.data,
                           protocol::SETofDEVICEEVENT::flag_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::KEYSYM keysym,
    const std::vector< std::string_view >&/* enum_names = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    // TBD encoding is convoluted, see:
    //   https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#keysym_encoding
    return _formatBitmask( keysym.data );  // force hex
}

// KEYCODE
template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::KEYCODE keycode,
    const std::vector< std::string_view >& enum_names/* = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::key );
    }
    return _formatInteger( keycode.data, enum_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::BUTTON button,
    const std::vector< std::string_view >& enum_names/* = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    if ( !enum_names.empty() ) {
        assert( enum_names == protocol::enum_names::button );
    }
    return _formatInteger( button.data, enum_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofKEYBUTMASK setofkeybutmask,
    const std::vector< std::string_view >&/* enum_names = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    assert( ( setofkeybutmask.data & protocol::SETofKEYBUTMASK::ZERO_BITS ) == 0 );
    return _formatBitmask( setofkeybutmask.data,
                           protocol::SETofKEYBUTMASK::flag_names );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::SETofKEYMASK setofkeymask,
    const std::vector< std::string_view >&/* enum_names = _NO_NAMES*/,
    _IndexRange/* name_index_range = {}*/ ) {
    if ( setofkeymask.data == protocol::SETofKEYMASK::ANYMODIFIER ) {
        return settings.verbose ?
            fmt::format( "{}({})",
                         _formatBitmask( setofkeymask.data ),
                         protocol::SETofKEYMASK::anymodifier_flag_name ) :
            std::string{ protocol::SETofKEYMASK::anymodifier_flag_name };
    }
    assert( ( setofkeymask.data & protocol::SETofKEYMASK::ZERO_BITS ) == 0 );
    return _formatBitmask( setofkeymask.data,
                           protocol::SETofKEYMASK::flag_names );
}

// TBD no examples of SETofBUTMASK found in core encoding

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::CHAR2B char2b, const _Whitespace&/* ws*/ ) {
    // TBD encoding is convoluted, see:
    //   https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Common_Types
    return _formatBitmask( *reinterpret_cast< const uint16_t* >( &char2b ) );  // force hex
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::POINT point, const _Whitespace& ws ) {
    static constexpr uint32_t name_width ( sizeof( "x" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x", name_width, ws.equals,
        _formatInteger( point.x ), ws.separator,
        ws.memb_indent, "y", name_width, ws.equals,
        _formatInteger( point.y ), ws.separator,
        ws.encl_indent
        );
}

// RECTANGLE
template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::RECTANGLE rectangle, const _Whitespace& ws ) {
    static constexpr uint32_t name_width ( sizeof( "height" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x", name_width, ws.equals,
        _formatInteger( rectangle.x ), ws.separator,
        ws.memb_indent, "y", name_width, ws.equals,
        _formatInteger( rectangle.y ), ws.separator,
        ws.memb_indent, "width", name_width, ws.equals,
        _formatInteger( rectangle.width ), ws.separator,
        ws.memb_indent, "height", name_width, ws.equals,
        _formatInteger( rectangle.height ), ws.separator,
        ws.encl_indent
        );
}

// ARC
template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::ARC arc, const _Whitespace& ws ) {
    static constexpr uint32_t name_width ( sizeof( "height" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x", name_width, ws.equals,
        _formatInteger( arc.x ), ws.separator,
        ws.memb_indent, "y", name_width, ws.equals,
        _formatInteger( arc.y ), ws.separator,
        ws.memb_indent, "width", name_width, ws.equals,
        _formatInteger( arc.width ), ws.separator,
        ws.memb_indent, "height", name_width, ws.equals,
        _formatInteger( arc.height ), ws.separator,
        ws.memb_indent, "angle1", name_width, ws.equals,
        _formatInteger( arc.angle1 ), ws.separator,
        ws.memb_indent, "angle2", name_width, ws.equals,
        _formatInteger( arc.angle2 ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::QueryFont::Reply::CHARINFO charinfo,
    const _Whitespace& ws ) {
    static constexpr uint32_t name_width ( sizeof( "right-side-bearing" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "left-side-bearing", name_width, ws.equals,
        _formatInteger( charinfo.left_side_bearing ), ws.separator,
        ws.memb_indent, "right-side-bearing", name_width, ws.equals,
        _formatInteger( charinfo.right_side_bearing ), ws.separator,
        ws.memb_indent, "character-width", name_width, ws.equals,
        _formatInteger( charinfo.character_width ), ws.separator,
        ws.memb_indent, "ascent", name_width, ws.equals,
        _formatInteger( charinfo.ascent ), ws.separator,
        ws.memb_indent, "descent", name_width, ws.equals,
        _formatInteger( charinfo.descent ), ws.separator,
        ws.memb_indent, "attributes", name_width, ws.equals,
        _formatInteger( charinfo.attributes ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::QueryFont::Reply::FONTPROP fontprop,
    const _Whitespace& ws ) {
    static constexpr uint32_t name_width ( sizeof( "value" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "name", name_width, ws.equals,
        _formatProtocolType( fontprop.name ), ws.separator,
        ws.memb_indent, "value", name_width, ws.equals,
        _formatInteger( fontprop.value ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::connection_setup::ConnAcceptance::FORMAT format,
    const _Whitespace& ws ) {
    static constexpr uint32_t name_width ( sizeof( "bits-per-pixel" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "depth", name_width, ws.equals,
        _formatInteger( format.depth ), ws.separator,
        ws.memb_indent, "bits-per-pixel", name_width, ws.equals,
        _formatInteger( format.bits_per_pixel ), ws.separator,
        ws.memb_indent, "scanline-pad", name_width, ws.equals,
        _formatInteger( format.scanline_pad ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::connection_setup::ConnAcceptance::SCREEN::DEPTH::\
    VISUALTYPE visualtype, const _Whitespace& ws ) {
    using VISUALTYPE =
        protocol::connection_setup::ConnAcceptance::SCREEN::DEPTH::VISUALTYPE;

    const uint32_t name_width (
        ws.multiline ? sizeof( "bits-per-rgb-value" ) - 1 : 0 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "visual-id", name_width, ws.equals,
        _formatProtocolType( visualtype.visual_id ), ws.separator,
        ws.memb_indent, "class", name_width, ws.equals,
        _formatInteger( visualtype.class_,
                        VISUALTYPE::class_names ), ws.separator,
        ws.memb_indent, "bits-per-rgb-value", name_width, ws.equals,
        _formatInteger( visualtype.bits_per_rgb_value ), ws.separator,
        ws.memb_indent, "colormap-entries", name_width, ws.equals,
        _formatInteger( visualtype.colormap_entries ), ws.separator,
        ws.memb_indent, "red-mask", name_width, ws.equals,
        _formatBitmask( visualtype.red_mask ), ws.separator,
        ws.memb_indent, "green-mask", name_width, ws.equals,
        _formatBitmask( visualtype.green_mask ), ws.separator,
        ws.memb_indent, "blue-mask", name_width, ws.equals,
        _formatBitmask( visualtype.blue_mask ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::PolySegment::SEGMENT segment,
    const _Whitespace& ws ) {
    static constexpr uint32_t name_width ( sizeof( "x1" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x1", name_width, ws.equals,
        _formatInteger( segment.x1 ), ws.separator,
        ws.memb_indent, "y1", name_width, ws.equals,
        _formatInteger( segment.y1 ), ws.separator,
        ws.memb_indent, "x2", name_width, ws.equals,
        _formatInteger( segment.x2 ), ws.separator,
        ws.memb_indent, "y2", name_width, ws.equals,
        _formatInteger( segment.y2 ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::StoreColors::COLORITEM coloritem,
    const _Whitespace& ws ) {
    using COLORITEM = protocol::requests::StoreColors::COLORITEM;
    assert( ( coloritem.do_rgb_mask & COLORITEM::DO_RGB_ZERO_BITS ) == 0 );
    static constexpr uint32_t name_width ( sizeof( "(do rgb mask)" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "pixel", name_width, ws.equals,
        _formatInteger( coloritem.pixel ), ws.separator,
        ws.memb_indent, "red", name_width, ws.equals,
        _formatInteger( coloritem.red ), ws.separator,
        ws.memb_indent, "green", name_width, ws.equals,
        _formatInteger( coloritem.green ), ws.separator,
        ws.memb_indent, "blue", name_width, ws.equals,
        _formatInteger( coloritem.blue ), ws.separator,
        ws.memb_indent, "(do rgb mask)", name_width, ws.equals,
        _formatBitmask( coloritem.do_rgb_mask,
                        COLORITEM::do_rgb_names ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::GetMotionEvents::Reply::TIMECOORD timecoord,
    const _Whitespace& ws ) {
    static constexpr uint32_t name_width ( sizeof( "time" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( timecoord.time ), ws.separator,
        ws.memb_indent, "x", name_width, ws.equals,
        _formatInteger( timecoord.x ), ws.separator,
        ws.memb_indent, "y", name_width, ws.equals,
        _formatInteger( timecoord.y ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatProtocolType(
    const protocol::requests::QueryColors::Reply::RGB rgb,
    const _Whitespace& ws ) {
    static constexpr uint32_t name_width ( sizeof( "green" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "red", name_width, ws.equals,
        _formatInteger( rgb.red ), ws.separator,
        ws.memb_indent, "green", name_width, ws.equals,
        _formatInteger( rgb.green ), ws.separator,
        ws.memb_indent, "blue", name_width, ws.equals,
        _formatInteger( rgb.blue ), ws.separator,
        ws.encl_indent
        );
}
