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


std::string
X11ProtocolParser::_formatVariable(
    const protocol::DRAWABLE drawable,
    const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    // defaulting to treating like WINDOW
    assert( ( drawable.window.data & protocol::WINDOW::ZERO_BITS ) == 0 );
    return _formatVariable( drawable.window.data, name_range );
}

std::string
X11ProtocolParser::_formatVariable(
    const protocol::FONTABLE fontable,
    const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    // defaulting to treating like FONT
    assert( ( fontable.font.data & protocol::FONT::ZERO_BITS ) == 0 );
    return _formatVariable( fontable.font.data, name_range );
}

std::string
X11ProtocolParser::_formatVariable(
    const protocol::ATOM atom,
    const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    assert( ( atom.data & protocol::ATOM::ZERO_BITS ) == 0 );
    if ( !name_range.empty() ) {
        assert( name_range.names == protocol::enum_names::zero_none ||
                name_range.names == protocol::requests::GetProperty::type_names );
    }
    if ( name_range.in( atom.data ) ) {
        return _formatVariable( atom.data, name_range );
    }
    const auto it { _interned_atoms.find( atom.data ) };
    return fmt::format( "{}({})", _formatVariable( atom.data ),
                        it == _interned_atoms.end() ? "unrecognized atom" :
                        fmt::format( "{:?}", it->second ) );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::TIMESTAMP time,
    const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    if ( !name_range.empty() ) {
        assert( name_range.names == protocol::enum_names::time );
    }
    const std::string hex_str {
        _formatVariable( time.data, {}, _ValueTraits::BITMASK ) };  // force hex
    if ( name_range.in( time.data ) ) {
        return fmt::format( "{}({})", hex_str, name_range.at( time.data ) );
    }
    if ( settings.relativetimestamps ) {
        static constexpr uint32_t TIMESTAMP_TICKS_PER_SEC { 1000 };
        const std::time_t time_ (
            ( ( time.data - settings.ref_TIMESTAMP ) / TIMESTAMP_TICKS_PER_SEC )
            + settings.ref_unix_time );
        // RFC 3339 UTC format "yyyy-mm-ddThh:mm:ssZ":
        // https://www.rfc-editor.org/rfc/rfc3339#section-5.6
        return fmt::format( "{}({:%FT:%TZ}UTC)", hex_str,
                            *std::localtime( &time_ ) );
    }
    return hex_str;
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::BITGRAVITY bitgravity,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    return _formatVariable( bitgravity.data,
                           { protocol::BITGRAVITY::enum_names } );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::WINGRAVITY wingravity,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    return _formatVariable( wingravity.data,
                           { protocol::WINGRAVITY::enum_names } );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::BOOL bool_,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    return _formatVariable( bool_.data,
                           { protocol::BOOL::enum_names } );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::SETofEVENT setofevent,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    assert( ( setofevent.data & protocol::SETofEVENT::ZERO_BITS ) == 0 );
    return _formatVariable( setofevent.data,
                           { protocol::SETofEVENT::flag_names },
                           _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::SETofPOINTEREVENT setofpointerevent,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    assert( ( setofpointerevent.data &
              protocol::SETofPOINTEREVENT::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatVariable( setofpointerevent.data,
                           { protocol::SETofPOINTEREVENT::flag_names },
                           _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::SETofDEVICEEVENT setofdeviceevent,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    assert( ( setofdeviceevent.data &
              protocol::SETofDEVICEEVENT::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatVariable( setofdeviceevent.data,
                           { protocol::SETofDEVICEEVENT::flag_names },
                           _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::KEYSYM keysym,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    // TBD encoding is convoluted, see:
    //   https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#keysym_encoding
    return _formatVariable( keysym.data,
                           {}, _ValueTraits::BITMASK );  // force hex
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::KEYCODE keycode,
    const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    if ( !name_range.empty() ) {
        assert( name_range.names == protocol::enum_names::key );
    }
    return _formatVariable( keycode.data, name_range );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::BUTTON button,
    const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    if ( !name_range.empty() ) {
        assert( name_range.names == protocol::enum_names::button );
    }
    return _formatVariable( button.data, name_range );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::SETofKEYBUTMASK setofkeybutmask,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    assert( ( setofkeybutmask.data & protocol::SETofKEYBUTMASK::ZERO_BITS ) == 0 );
    return _formatVariable( setofkeybutmask.data,
                           { protocol::SETofKEYBUTMASK::flag_names },
                           _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::SETofKEYMASK setofkeymask,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    if ( setofkeymask.data == protocol::SETofKEYMASK::ANYMODIFIER ) {
        if ( settings.verbose ) {
            return fmt::format(
                "{}({})", _formatVariable( setofkeymask.data,
                                          {}, _ValueTraits::BITMASK ),
                protocol::SETofKEYMASK::anymodifier_flag_name );
        }
        return std::string( protocol::SETofKEYMASK::anymodifier_flag_name );
    }
    assert( ( setofkeymask.data & protocol::SETofKEYMASK::ZERO_BITS ) == 0 );
    return _formatVariable( setofkeymask.data,
                           { protocol::SETofKEYMASK::flag_names },
                           _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::CHAR2B char2B, const _Whitespace&/* ws*/ ) {
    // TBD default to printing as hex due to convoluted encoding, see:
    //   https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Common_Types
    const uint16_t char16_eqv { *reinterpret_cast< const uint16_t* >( &char2B ) };
    return _formatVariable( char16_eqv, {}, _ValueTraits::BITMASK );  // force hex
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::POINT point, const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "x" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( point.x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( point.y ), ws.separator,
        ws.encl_indent
        );
}

// RECTANGLE
template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::RECTANGLE rectangle, const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "height" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( rectangle.x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( rectangle.y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( rectangle.width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( rectangle.height ), ws.separator,
        ws.encl_indent
        );
}

// ARC
template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::ARC arc, const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "height" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( arc.x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( arc.y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( arc.width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( arc.height ), ws.separator,
        ws.memb_indent, "angle1", memb_name_w, ws.equals,
        _formatVariable( arc.angle1 ), ws.separator,
        ws.memb_indent, "angle2", memb_name_w, ws.equals,
        _formatVariable( arc.angle2 ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::requests::QueryFont::Reply::CHARINFO charinfo,
    const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "right-side-bearing" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "left-side-bearing", memb_name_w, ws.equals,
        _formatVariable( charinfo.left_side_bearing ), ws.separator,
        ws.memb_indent, "right-side-bearing", memb_name_w, ws.equals,
        _formatVariable( charinfo.right_side_bearing ), ws.separator,
        ws.memb_indent, "character-width", memb_name_w, ws.equals,
        _formatVariable( charinfo.character_width ), ws.separator,
        ws.memb_indent, "ascent", memb_name_w, ws.equals,
        _formatVariable( charinfo.ascent ), ws.separator,
        ws.memb_indent, "descent", memb_name_w, ws.equals,
        _formatVariable( charinfo.descent ), ws.separator,
        ws.memb_indent, "attributes", memb_name_w, ws.equals,
        _formatVariable( charinfo.attributes ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::requests::QueryFont::Reply::FONTPROP fontprop,
    const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "value" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "name", memb_name_w, ws.equals,
        _formatVariable( fontprop.name ), ws.separator,
        ws.memb_indent, "value", memb_name_w, ws.equals,
        _formatVariable( fontprop.value ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::connection_setup::ConnAcceptance::FORMAT format,
    const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "bits-per-pixel" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatVariable( format.depth ), ws.separator,
        ws.memb_indent, "bits-per-pixel", memb_name_w, ws.equals,
        _formatVariable( format.bits_per_pixel ), ws.separator,
        ws.memb_indent, "scanline-pad", memb_name_w, ws.equals,
        _formatVariable( format.scanline_pad ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::connection_setup::ConnAcceptance::SCREEN::DEPTH::\
    VISUALTYPE visualtype, const _Whitespace& ws ) {
    using VISUALTYPE =
        protocol::connection_setup::ConnAcceptance::SCREEN::DEPTH::VISUALTYPE;
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "bits-per-rgb-value" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "visual-id", memb_name_w, ws.equals,
        _formatVariable( visualtype.visual_id ), ws.separator,
        ws.memb_indent, "class", memb_name_w, ws.equals,
        _formatVariable( visualtype.class_,
                        VISUALTYPE::class_names ), ws.separator,
        ws.memb_indent, "bits-per-rgb-value", memb_name_w, ws.equals,
        _formatVariable( visualtype.bits_per_rgb_value ), ws.separator,
        ws.memb_indent, "colormap-entries", memb_name_w, ws.equals,
        _formatVariable( visualtype.colormap_entries ), ws.separator,
        ws.memb_indent, "red-mask", memb_name_w, ws.equals,
        _formatVariable( visualtype.red_mask,
                        {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "green-mask", memb_name_w, ws.equals,
        _formatVariable( visualtype.green_mask,
                       {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "blue-mask", memb_name_w, ws.equals,
        _formatVariable( visualtype.blue_mask,
                        {}, _ValueTraits::BITMASK ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::requests::PolySegment::SEGMENT segment,
    const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "x1" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x1", memb_name_w, ws.equals,
        _formatVariable( segment.x1 ), ws.separator,
        ws.memb_indent, "y1", memb_name_w, ws.equals,
        _formatVariable( segment.y1 ), ws.separator,
        ws.memb_indent, "x2", memb_name_w, ws.equals,
        _formatVariable( segment.x2 ), ws.separator,
        ws.memb_indent, "y2", memb_name_w, ws.equals,
        _formatVariable( segment.y2 ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::requests::StoreColors::COLORITEM coloritem,
    const _Whitespace& ws ) {
    using COLORITEM = protocol::requests::StoreColors::COLORITEM;
    assert( ( coloritem.do_rgb_mask & COLORITEM::DO_RGB_ZERO_BITS ) == 0 );
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "(do rgb mask)" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "pixel", memb_name_w, ws.equals,
        _formatVariable( coloritem.pixel ), ws.separator,
        ws.memb_indent, "red", memb_name_w, ws.equals,
        _formatVariable( coloritem.red ), ws.separator,
        ws.memb_indent, "green", memb_name_w, ws.equals,
        _formatVariable( coloritem.green ), ws.separator,
        ws.memb_indent, "blue", memb_name_w, ws.equals,
        _formatVariable( coloritem.blue ), ws.separator,
        ws.memb_indent, "(do rgb mask)", memb_name_w, ws.equals,
        _formatVariable( coloritem.do_rgb_mask,
                         { COLORITEM::do_rgb_names },
                         _ValueTraits::BITMASK ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::requests::GetMotionEvents::Reply::TIMECOORD timecoord,
    const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "time" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( timecoord.time ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( timecoord.x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( timecoord.y ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::requests::QueryColors::Reply::RGB rgb,
    const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "green" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "red", memb_name_w, ws.equals,
        _formatVariable( rgb.red ), ws.separator,
        ws.memb_indent, "green", memb_name_w, ws.equals,
        _formatVariable( rgb.green ), ws.separator,
        ws.memb_indent, "blue", memb_name_w, ws.equals,
        _formatVariable( rgb.blue ), ws.separator,
        ws.encl_indent
        );
}
