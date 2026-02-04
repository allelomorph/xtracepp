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
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    // defaulting to treating like WINDOW
    assert( ( drawable.window.data & protocol::WINDOW::ZERO_BITS ) == 0 );
    return _formatVariable( drawable.window.data, byteswap, name_range );
}

std::string
X11ProtocolParser::_formatVariable(
    const protocol::FONTABLE fontable,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    // defaulting to treating like FONT
    assert( ( fontable.font.data & protocol::FONT::ZERO_BITS ) == 0 );
    return _formatVariable( fontable.font.data, byteswap, name_range );
}

std::string
X11ProtocolParser::_formatVariable(
    const protocol::ATOM atom,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    assert( ( atom.data & protocol::ATOM::ZERO_BITS ) == 0 );
    if ( !name_range.empty() ) {
        assert( name_range.names == protocol::enum_names::zero_none ||
                  name_range.names == protocol::requests::GetProperty::type_names );
    }
    if ( name_range.in( atom.data ) ) {
        return _formatVariable( atom.data, byteswap, name_range );
    }
    const auto it { _interned_atoms.find( atom.data ) };
    return fmt::format( "{}({})", _formatVariable( atom.data, byteswap ),
                        it == _interned_atoms.end() ? "unrecognized atom" :
                        fmt::format( "{:?}", it->second ) );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::TIMESTAMP time,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    if ( !name_range.empty() ) {
        assert( name_range.names == protocol::enum_names::time );
    }
    const std::string hex_str {
        _formatVariable( time.data, byteswap,
                         {}, _ValueTraits::BITMASK ) };  // force hex
    if ( name_range.in( time.data ) ) {
        return fmt::format( "{}({})", hex_str, name_range.at( time.data ) );
    }
    if ( settings.relativetimestamps ) {
        static constexpr uint32_t TIMESTAMP_TICKS_PER_SEC { 1000 };
        const int64_t server_time_delta {
            static_cast< int64_t >( time.data ) - settings.ref_TIMESTAMP };
        const std::time_t time_ (
            ( server_time_delta / TIMESTAMP_TICKS_PER_SEC )
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
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    return _formatVariable( bitgravity.data, byteswap,
                            { protocol::BITGRAVITY::enum_names } );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::WINGRAVITY wingravity,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    return _formatVariable( wingravity.data, byteswap,
                            { protocol::WINGRAVITY::enum_names } );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::BOOL bool_,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    return _formatVariable( bool_.data, byteswap,
                            { protocol::BOOL::enum_names } );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::SETofEVENT setofevent,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    assert( ( setofevent.data & protocol::SETofEVENT::ZERO_BITS ) == 0 );
    return _formatVariable( setofevent.data, byteswap,
                            { protocol::SETofEVENT::flag_names },
                            _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::SETofPOINTEREVENT setofpointerevent,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    assert( ( setofpointerevent.data &
                protocol::SETofPOINTEREVENT::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatVariable( setofpointerevent.data, byteswap,
                            { protocol::SETofPOINTEREVENT::flag_names },
                            _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::SETofDEVICEEVENT setofdeviceevent,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    assert( ( setofdeviceevent.data &
                protocol::SETofDEVICEEVENT::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatVariable( setofdeviceevent.data, byteswap,
                            { protocol::SETofDEVICEEVENT::flag_names },
                            _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::KEYSYM keysym,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    // encoding is convoluted, see:
    //   https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#keysym_encoding
    return _formatVariable( keysym.data, byteswap,
                            {}, _ValueTraits::BITMASK );  // force hex
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::KEYCODE keycode,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    if ( !name_range.empty() ) {
        assert( name_range.names == protocol::enum_names::key );
    }
    return _formatVariable( keycode.data, byteswap, name_range );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::BUTTON button,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    if ( !name_range.empty() ) {
        assert( name_range.names == protocol::enum_names::button );
    }
    return _formatVariable( button.data, byteswap, name_range );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::SETofKEYBUTMASK setofkeybutmask,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    assert( ( setofkeybutmask.data & protocol::SETofKEYBUTMASK::ZERO_BITS ) == 0 );
    return _formatVariable( setofkeybutmask.data, byteswap,
                            { protocol::SETofKEYBUTMASK::flag_names },
                            _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::SETofKEYMASK setofkeymask,
    const bool byteswap,
    const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    if ( setofkeymask.data == protocol::SETofKEYMASK::ANYMODIFIER ) {
        if ( settings.verbose ) {
            return fmt::format(
                "{}({})", _formatVariable( setofkeymask.data, byteswap,
                                           {}, _ValueTraits::BITMASK ),
                protocol::SETofKEYMASK::anymodifier_flag_name );
        }
        return std::string( protocol::SETofKEYMASK::anymodifier_flag_name );
    }
    assert( ( setofkeymask.data & protocol::SETofKEYMASK::ZERO_BITS ) == 0 );
    return _formatVariable( setofkeymask.data, byteswap,
                            { protocol::SETofKEYMASK::flag_names },
                            _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::CHAR2B char2B, const bool byteswap,
    const _Whitespace&/* ws*/ ) {
    // default to printing as hex due to convoluted encoding, see:
    //   https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Common_Types
    const uint16_t char16_eqv { *reinterpret_cast< const uint16_t* >( &char2B ) };
    return _formatVariable( char16_eqv, byteswap,
                            {}, _ValueTraits::BITMASK );  // force hex
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::POINT point, const bool byteswap,
    const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "x" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( point.x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( point.y, byteswap ), ws.separator,
        ws.encl_indent
        );
}

// RECTANGLE
template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::RECTANGLE rectangle, const bool byteswap,
    const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "height" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( rectangle.x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( rectangle.y, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( rectangle.width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( rectangle.height, byteswap ), ws.separator,
        ws.encl_indent
        );
}

// ARC
template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::ARC arc, const bool byteswap,
    const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "height" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( arc.x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( arc.y, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( arc.width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( arc.height, byteswap ), ws.separator,
        ws.memb_indent, "angle1", memb_name_w, ws.equals,
        _formatVariable( arc.angle1, byteswap ), ws.separator,
        ws.memb_indent, "angle2", memb_name_w, ws.equals,
        _formatVariable( arc.angle2, byteswap ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::requests::QueryFont::Reply::CHARINFO charinfo,
    const bool byteswap, const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "right-side-bearing" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "left-side-bearing", memb_name_w, ws.equals,
        _formatVariable( charinfo.left_side_bearing, byteswap ), ws.separator,
        ws.memb_indent, "right-side-bearing", memb_name_w, ws.equals,
        _formatVariable( charinfo.right_side_bearing, byteswap ), ws.separator,
        ws.memb_indent, "character-width", memb_name_w, ws.equals,
        _formatVariable( charinfo.character_width, byteswap ), ws.separator,
        ws.memb_indent, "ascent", memb_name_w, ws.equals,
        _formatVariable( charinfo.ascent, byteswap ), ws.separator,
        ws.memb_indent, "descent", memb_name_w, ws.equals,
        _formatVariable( charinfo.descent, byteswap ), ws.separator,
        ws.memb_indent, "attributes", memb_name_w, ws.equals,
        _formatVariable( charinfo.attributes, byteswap ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::requests::QueryFont::Reply::FONTPROP fontprop,
    const bool byteswap, const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "value" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "name", memb_name_w, ws.equals,
        _formatVariable( fontprop.name, byteswap ), ws.separator,
        ws.memb_indent, "value", memb_name_w, ws.equals,
        _formatVariable( fontprop.value, byteswap ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::connection_setup::Acceptance::FORMAT format,
    const bool byteswap, const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "bits-per-pixel" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatVariable( format.depth, byteswap ), ws.separator,
        ws.memb_indent, "bits-per-pixel", memb_name_w, ws.equals,
        _formatVariable( format.bits_per_pixel, byteswap ), ws.separator,
        ws.memb_indent, "scanline-pad", memb_name_w, ws.equals,
        _formatVariable( format.scanline_pad, byteswap ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::connection_setup::Acceptance::SCREEN::DEPTH::\
    VISUALTYPE visualtype, const bool byteswap,
    const _Whitespace& ws ) {
    using VISUALTYPE =
        protocol::connection_setup::Acceptance::SCREEN::DEPTH::VISUALTYPE;
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "bits-per-rgb-value" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "visual-id", memb_name_w, ws.equals,
        _formatVariable( visualtype.visual_id, byteswap ), ws.separator,
        ws.memb_indent, "class", memb_name_w, ws.equals,
        _formatVariable( visualtype.class_, byteswap,
                         VISUALTYPE::class_names ), ws.separator,
        ws.memb_indent, "bits-per-rgb-value", memb_name_w, ws.equals,
        _formatVariable( visualtype.bits_per_rgb_value, byteswap ), ws.separator,
        ws.memb_indent, "colormap-entries", memb_name_w, ws.equals,
        _formatVariable( visualtype.colormap_entries, byteswap ), ws.separator,
        ws.memb_indent, "red-mask", memb_name_w, ws.equals,
        _formatVariable( visualtype.red_mask, byteswap,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "green-mask", memb_name_w, ws.equals,
        _formatVariable( visualtype.green_mask, byteswap,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "blue-mask", memb_name_w, ws.equals,
        _formatVariable( visualtype.blue_mask, byteswap,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::requests::PolySegment::SEGMENT segment,
    const bool byteswap, const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "x1" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "x1", memb_name_w, ws.equals,
        _formatVariable( segment.x1, byteswap ), ws.separator,
        ws.memb_indent, "y1", memb_name_w, ws.equals,
        _formatVariable( segment.y1, byteswap ), ws.separator,
        ws.memb_indent, "x2", memb_name_w, ws.equals,
        _formatVariable( segment.x2, byteswap ), ws.separator,
        ws.memb_indent, "y2", memb_name_w, ws.equals,
        _formatVariable( segment.y2, byteswap ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::requests::StoreColors::COLORITEM coloritem,
    const bool byteswap, const _Whitespace& ws ) {
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
        _formatVariable( coloritem.pixel, byteswap ), ws.separator,
        ws.memb_indent, "red", memb_name_w, ws.equals,
        _formatVariable( coloritem.red, byteswap ), ws.separator,
        ws.memb_indent, "green", memb_name_w, ws.equals,
        _formatVariable( coloritem.green, byteswap ), ws.separator,
        ws.memb_indent, "blue", memb_name_w, ws.equals,
        _formatVariable( coloritem.blue, byteswap ), ws.separator,
        ws.memb_indent, "(do rgb mask)", memb_name_w, ws.equals,
        _formatVariable( coloritem.do_rgb_mask, byteswap,
                         { COLORITEM::do_rgb_names },
                         _ValueTraits::BITMASK ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::requests::GetMotionEvents::Reply::TIMECOORD timecoord,
    const bool byteswap, const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "time" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( timecoord.time, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( timecoord.x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( timecoord.y, byteswap ), ws.separator,
        ws.encl_indent
        );
}

template<>
std::string
X11ProtocolParser::_formatVariable(
    const protocol::requests::QueryColors::Reply::RGB rgb,
    const bool byteswap, const _Whitespace& ws ) {
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "green" ) - 1 );
    return fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "red", memb_name_w, ws.equals,
        _formatVariable( rgb.red, byteswap ), ws.separator,
        ws.memb_indent, "green", memb_name_w, ws.equals,
        _formatVariable( rgb.green, byteswap ), ws.separator,
        ws.memb_indent, "blue", memb_name_w, ws.equals,
        _formatVariable( rgb.blue, byteswap ), ws.separator,
        ws.encl_indent
        );
}
