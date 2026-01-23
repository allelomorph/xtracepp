#include <string_view>

#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/events.hpp"


template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::impl::InputEvent >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::impl::InputEvent;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= InputEvent::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const InputEvent::Encoding* encoding {
        reinterpret_cast< const InputEvent::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( InputEvent::Encoding );
    assert( outputs.bytes_parsed == InputEvent::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "same-screen" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        ws.memb_indent, "detail", memb_name_w, ws.equals,
        _formatVariable( encoding->header.detail, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time, byteswap ), ws.separator,
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatVariable( encoding->root, byteswap ), ws.separator,
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatVariable( encoding->event, byteswap ), ws.separator,
        ws.memb_indent, "child", memb_name_w, ws.equals,
        _formatVariable( encoding->child, byteswap,
                         { InputEvent::child_names } ), ws.separator,
        ws.memb_indent, "root-x", memb_name_w, ws.equals,
        _formatVariable( encoding->root_x, byteswap ), ws.separator,
        ws.memb_indent, "root-y", memb_name_w, ws.equals,
        _formatVariable( encoding->root_y, byteswap ), ws.separator,
        ws.memb_indent, "event-x", memb_name_w, ws.equals,
        _formatVariable( encoding->event_x, byteswap ), ws.separator,
        ws.memb_indent, "event-y", memb_name_w, ws.equals,
        _formatVariable( encoding->event_y, byteswap ), ws.separator,
        ws.memb_indent, "state", memb_name_w, ws.equals,
        _formatVariable( encoding->state, byteswap ), ws.separator,
        ws.memb_indent, "same-screen", memb_name_w, ws.equals,
        _formatVariable( encoding->same_screen, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::KeyPress >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::KeyPress;
    using protocol::events::impl::InputEvent;
    static_assert( std::is_base_of_v< InputEvent, KeyPress > );
    assert( data != nullptr );
    assert( sz >= sizeof( KeyPress::Header ) );
    assert( ( _ordered(
                  reinterpret_cast< const KeyPress::Header* >(
                      data )->code, conn->byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::KEYPRESS );
    return _parseEvent< InputEvent >( conn, data, sz, ws );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::KeyRelease >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::KeyRelease;
    using protocol::events::impl::InputEvent;
    static_assert( std::is_base_of_v< InputEvent, KeyRelease > );
    assert( data != nullptr );
    assert( sz >= sizeof( KeyRelease::Header ) );
    assert( ( _ordered(
                  reinterpret_cast< const KeyRelease::Header* >(
                      data )->code, conn->byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::KEYRELEASE );
    return _parseEvent< InputEvent >( conn, data, sz, ws );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ButtonPress >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::ButtonPress;
    using protocol::events::impl::InputEvent;
    static_assert( std::is_base_of_v< InputEvent, ButtonPress > );
    assert( data != nullptr );
    assert( sz >= sizeof( ButtonPress::Header ) );
    assert( ( _ordered(
                  reinterpret_cast< const ButtonPress::Header* >(
                      data )->code, conn->byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::BUTTONPRESS );
    return _parseEvent< InputEvent >( conn, data, sz, ws );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ButtonRelease >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::ButtonRelease;
    using protocol::events::impl::InputEvent;
    static_assert( std::is_base_of_v< InputEvent, ButtonRelease > );
    assert( data != nullptr );
    assert( sz >= sizeof( ButtonRelease::Header ) );
    assert( ( _ordered(
                  reinterpret_cast< const ButtonRelease::Header* >(
                      data )->code, conn->byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::BUTTONRELEASE );
    return _parseEvent< InputEvent >( conn, data, sz, ws );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MotionNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::MotionNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= MotionNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const MotionNotify::Encoding* encoding {
        reinterpret_cast< const MotionNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MotionNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::MOTIONNOTIFY );
    assert( outputs.bytes_parsed == MotionNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "same-screen" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        ws.memb_indent, "detail", memb_name_w, ws.equals,
        _formatVariable( encoding->header.detail, byteswap,
                         { MotionNotify::detail_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time, byteswap ), ws.separator,
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatVariable( encoding->root, byteswap ), ws.separator,
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatVariable( encoding->event, byteswap ), ws.separator,
        ws.memb_indent, "child", memb_name_w, ws.equals,
        _formatVariable( encoding->child, byteswap,
                         { MotionNotify::child_names } ), ws.separator,
        ws.memb_indent, "root-x", memb_name_w, ws.equals,
        _formatVariable( encoding->root_x, byteswap ), ws.separator,
        ws.memb_indent, "root-y", memb_name_w, ws.equals,
        _formatVariable( encoding->root_y, byteswap ), ws.separator,
        ws.memb_indent, "event-x", memb_name_w, ws.equals,
        _formatVariable( encoding->event_x, byteswap ), ws.separator,
        ws.memb_indent, "event-y", memb_name_w, ws.equals,
        _formatVariable( encoding->event_y, byteswap ), ws.separator,
        ws.memb_indent, "state", memb_name_w, ws.equals,
        _formatVariable( encoding->state, byteswap ), ws.separator,
        ws.memb_indent, "same-screen", memb_name_w, ws.equals,
        _formatVariable( encoding->same_screen, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::impl::BoundaryEvent >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::impl::BoundaryEvent;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= BoundaryEvent::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const BoundaryEvent::Encoding* encoding {
        reinterpret_cast< const BoundaryEvent::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( BoundaryEvent::Encoding );
    assert( outputs.bytes_parsed == BoundaryEvent::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "(same-screen/focus mask)" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        ws.memb_indent, "detail", memb_name_w, ws.equals,
        _formatVariable( encoding->header.detail, byteswap,
                         { BoundaryEvent::detail_names,
                           _EnumNameRange::Bound::MAX,
                           BoundaryEvent::DETAIL_ENUM_MAX } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time, byteswap ), ws.separator,
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatVariable( encoding->root, byteswap ), ws.separator,
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatVariable( encoding->event, byteswap ), ws.separator,
        ws.memb_indent, "child", memb_name_w, ws.equals,
        _formatVariable( encoding->child, byteswap,
                         { BoundaryEvent::child_names } ), ws.separator,
        ws.memb_indent, "root-x", memb_name_w, ws.equals,
        _formatVariable( encoding->root_x, byteswap ), ws.separator,
        ws.memb_indent, "root-y", memb_name_w, ws.equals,
        _formatVariable( encoding->root_y, byteswap ), ws.separator,
        ws.memb_indent, "event-x", memb_name_w, ws.equals,
        _formatVariable( encoding->event_x, byteswap ), ws.separator,
        ws.memb_indent, "event-y", memb_name_w, ws.equals,
        _formatVariable( encoding->event_y, byteswap ), ws.separator,
        ws.memb_indent, "state", memb_name_w, ws.equals,
        _formatVariable( encoding->state, byteswap ), ws.separator,
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( encoding->mode, byteswap,
                         { BoundaryEvent::mode_names,
                           _EnumNameRange::Bound::MAX,
                           BoundaryEvent::MODE_ENUM_MAX } ), ws.separator,
        ws.memb_indent, "(same-screen/focus mask)", memb_name_w, ws.equals,
        _formatVariable( encoding->focus_same_screen_mask, byteswap,
                         { BoundaryEvent::focus_same_screen_names },
                         _ValueTraits::BITMASK ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::EnterNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::EnterNotify;
    using protocol::events::impl::BoundaryEvent;
    static_assert( std::is_base_of_v< BoundaryEvent, EnterNotify > );
    assert( data != nullptr );
    assert( sz >= sizeof( EnterNotify::Header ) );
    assert( ( _ordered(
                  reinterpret_cast< const EnterNotify::Header* >(
                      data )->code, conn->byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::ENTERNOTIFY );
    return _parseEvent< BoundaryEvent >( conn, data, sz, ws );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::LeaveNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::LeaveNotify;
    using protocol::events::impl::BoundaryEvent;
    static_assert( std::is_base_of_v< BoundaryEvent, LeaveNotify > );
    assert( data != nullptr );
    assert( sz >= sizeof( LeaveNotify::Header ) );
    assert( ( _ordered(
                  reinterpret_cast< const LeaveNotify::Header* >(
                      data )->code, conn->byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::LEAVENOTIFY );
    return _parseEvent< BoundaryEvent >( conn, data, sz, ws );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::impl::FocusEvent >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::impl::FocusEvent;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= FocusEvent::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    assert( sz >= sizeof( FocusEvent::Encoding ) );
    const FocusEvent::Encoding* encoding {
        reinterpret_cast< const FocusEvent::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( FocusEvent::Encoding );
    assert( outputs.bytes_parsed == FocusEvent::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "detail" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        ws.memb_indent, "detail", memb_name_w, ws.equals,
        _formatVariable( encoding->header.detail, byteswap,
                         { FocusEvent::detail_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatVariable( encoding->event, byteswap ), ws.separator,
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( encoding->mode, byteswap,
                         { FocusEvent::mode_names } ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::FocusIn >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::FocusIn;
    using protocol::events::impl::FocusEvent;
    static_assert( std::is_base_of_v< FocusEvent, FocusIn > );
    assert( data != nullptr );
    assert( sz >= sizeof( FocusIn::Header ) );
    assert( ( _ordered(
                  reinterpret_cast< const FocusIn::Header* >(
                      data )->code, conn->byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::FOCUSIN );
    return _parseEvent< FocusEvent >( conn, data, sz, ws );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::FocusOut >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::FocusOut;
    using protocol::events::impl::FocusEvent;
    static_assert( std::is_base_of_v< FocusEvent, FocusOut > );
    assert( data != nullptr );
    assert( sz >= sizeof( FocusOut::Header ) );
    assert( ( _ordered(
                  reinterpret_cast< const FocusOut::Header* >(
                      data )->code, conn->byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::FOCUSOUT );
    return _parseEvent< FocusEvent >( conn, data, sz, ws );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::KeymapNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::KeymapNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= KeymapNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const KeymapNotify::Encoding* encoding {
        reinterpret_cast< const KeymapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( KeymapNotify::Encoding );
    assert( outputs.bytes_parsed == KeymapNotify::ENCODING_SZ );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::KEYMAPNOTIFY );
    const _ParsingOutputs keys {
        _parseLISTof< protocol::CARD8 >(
            encoding->keys, sizeof( encoding->keys ), sizeof( encoding->keys ),
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "keys(0-7 omitted)" ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        ws.memb_indent, "keys(0-7 omitted)", memb_name_w, ws.equals,
        keys.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::Expose >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::Expose;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= Expose::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const Expose::Encoding* encoding {
        reinterpret_cast< const Expose::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( Expose::Encoding );
    assert( outputs.bytes_parsed == Expose::ENCODING_SZ );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::EXPOSE );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "window" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height, byteswap ), ws.separator,
        ws.memb_indent, "count", memb_name_w, ws.equals,
        _formatVariable( encoding->count, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::GraphicsExposure >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::GraphicsExposure;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GraphicsExposure::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const GraphicsExposure::Encoding* encoding {
        reinterpret_cast< const GraphicsExposure::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( GraphicsExposure::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::GRAPHICSEXPOSURE );
    assert( outputs.bytes_parsed == GraphicsExposure::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "minor-opcode" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height, byteswap ), ws.separator,
        ws.memb_indent, "minor-opcode", memb_name_w, ws.equals,
        _formatVariable( encoding->minor_opcode, byteswap ), ws.separator,
        ws.memb_indent, "count", memb_name_w, ws.equals,
        _formatVariable( encoding->count, byteswap ), ws.separator,
        ws.memb_indent, "major-opcode", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::NoExposure >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::NoExposure;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= NoExposure::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const NoExposure::Encoding* encoding {
        reinterpret_cast< const NoExposure::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( NoExposure::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::NOEXPOSURE );
    assert( outputs.bytes_parsed == NoExposure::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "minor-opcode" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "minor-opcode", memb_name_w, ws.equals,
        _formatVariable( encoding->minor_opcode, byteswap ), ws.separator,
        ws.memb_indent, "major-opcode", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::VisibilityNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::VisibilityNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= VisibilityNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const VisibilityNotify::Encoding* encoding {
        reinterpret_cast< const VisibilityNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( VisibilityNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::VISIBILITYNOTIFY );
    assert( outputs.bytes_parsed == VisibilityNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "window" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "state", memb_name_w, ws.equals,
        _formatVariable( encoding->state, byteswap,
                         { VisibilityNotify::state_names } ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::CreateNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::CreateNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CreateNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const CreateNotify::Encoding* encoding {
        reinterpret_cast< const CreateNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CreateNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::CREATENOTIFY );
    assert( outputs.bytes_parsed == CreateNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "override-redirect" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatVariable( encoding->parent, byteswap ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height, byteswap ), ws.separator,
        ws.memb_indent, "border-width", memb_name_w, ws.equals,
        _formatVariable( encoding->border_width, byteswap ), ws.separator,
        ws.memb_indent, "override-redirect", memb_name_w, ws.equals,
        _formatVariable( encoding->override_redirect, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::DestroyNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::DestroyNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= DestroyNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const DestroyNotify::Encoding* encoding {
        reinterpret_cast< const DestroyNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( DestroyNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::DESTROYNOTIFY );
    assert( outputs.bytes_parsed == DestroyNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "window" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatVariable( encoding->event, byteswap ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::UnmapNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::UnmapNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= UnmapNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const UnmapNotify::Encoding* encoding {
        reinterpret_cast< const UnmapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( UnmapNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::UNMAPNOTIFY );
    assert( outputs.bytes_parsed == UnmapNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "from-configure" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatVariable( encoding->event, byteswap ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "from-configure", memb_name_w, ws.equals,
        _formatVariable( encoding->from_configure, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MapNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::MapNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= MapNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const MapNotify::Encoding* encoding {
        reinterpret_cast< const MapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MapNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::MAPNOTIFY );
    assert( outputs.bytes_parsed == MapNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "override-redirect" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatVariable( encoding->event, byteswap ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "override-redirect", memb_name_w, ws.equals,
        _formatVariable( encoding->override_redirect, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MapRequest >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::MapRequest;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= MapRequest::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const MapRequest::Encoding* encoding {
        reinterpret_cast< const MapRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MapRequest::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::MAPREQUEST );
    assert( outputs.bytes_parsed == MapRequest::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "parent" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatVariable( encoding->parent, byteswap ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ReparentNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::ReparentNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ReparentNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const ReparentNotify::Encoding* encoding {
        reinterpret_cast< const ReparentNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ReparentNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::REPARENTNOTIFY );
    assert( outputs.bytes_parsed == ReparentNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "override-redirect" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatVariable( encoding->event, byteswap ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatVariable( encoding->parent, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "override-redirect", memb_name_w, ws.equals,
        _formatVariable( encoding->override_redirect, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ConfigureNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::ConfigureNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ConfigureNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const ConfigureNotify::Encoding* encoding {
        reinterpret_cast< const ConfigureNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ConfigureNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::CONFIGURENOTIFY );
    assert( outputs.bytes_parsed == ConfigureNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "override-redirect" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatVariable( encoding->event, byteswap ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "above-sibling", memb_name_w, ws.equals,
        _formatVariable( encoding->above_sibling, byteswap,
                         { ConfigureNotify::above_sibling_names } ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height, byteswap ), ws.separator,
        ws.memb_indent, "border-width", memb_name_w, ws.equals,
        _formatVariable( encoding->border_width, byteswap ), ws.separator,
        ws.memb_indent, "override-redirect", memb_name_w, ws.equals,
        _formatVariable( encoding->override_redirect, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ConfigureRequest >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::ConfigureRequest;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ConfigureRequest::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    assert( sz >= sizeof( ConfigureRequest::Encoding ) );
    const ConfigureRequest::Encoding* encoding {
        reinterpret_cast< const ConfigureRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ConfigureRequest::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::CONFIGUREREQUEST );
    assert( outputs.bytes_parsed == ConfigureRequest::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "border-width" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        ws.memb_indent, "stack-mode", memb_name_w, ws.equals,
        _formatVariable( encoding->header.stack_mode, byteswap,
                         { ConfigureRequest::stack_mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatVariable( encoding->parent, byteswap ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "sibling", memb_name_w, ws.equals,
        _formatVariable( encoding->sibling, byteswap,
                         { ConfigureRequest::sibling_names } ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height, byteswap ), ws.separator,
        ws.memb_indent, "border-width", memb_name_w, ws.equals,
        _formatVariable( encoding->border_width, byteswap ), ws.separator,
        ws.memb_indent, "value-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->value_mask, byteswap,
                         { ConfigureRequest::value_names },
                         _ValueTraits::BITMASK ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::GravityNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::GravityNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GravityNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const GravityNotify::Encoding* encoding {
        reinterpret_cast< const GravityNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( GravityNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::GRAVITYNOTIFY );
    assert( outputs.bytes_parsed == GravityNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "window" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatVariable( encoding->event, byteswap ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ResizeRequest >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::ResizeRequest;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ResizeRequest::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const ResizeRequest::Encoding* encoding {
        reinterpret_cast< const ResizeRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ResizeRequest::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::RESIZEREQUEST );
    assert( outputs.bytes_parsed == ResizeRequest::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "window" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::CirculateNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::CirculateNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CirculateNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const CirculateNotify::Encoding* encoding {
        reinterpret_cast< const CirculateNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CirculateNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::CIRCULATENOTIFY );
    assert( outputs.bytes_parsed == CirculateNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "window" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatVariable( encoding->event, byteswap ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "place", memb_name_w, ws.equals,
        _formatVariable( encoding->place, byteswap,
                         { CirculateNotify::place_names } ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::CirculateRequest >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::CirculateRequest;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CirculateRequest::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const CirculateRequest::Encoding* encoding {
        reinterpret_cast< const CirculateRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CirculateRequest::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::CIRCULATEREQUEST );
    assert( outputs.bytes_parsed == CirculateRequest::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "parent" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatVariable( encoding->parent, byteswap ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "place", memb_name_w, ws.equals,
        _formatVariable( encoding->place, byteswap,
                         { CirculateRequest::place_names } ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::PropertyNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::PropertyNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= PropertyNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const PropertyNotify::Encoding* encoding {
        reinterpret_cast< const PropertyNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( PropertyNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::PROPERTYNOTIFY );
    assert( outputs.bytes_parsed == PropertyNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "atom", memb_name_w, ws.equals,
        _formatVariable( encoding->atom, byteswap ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time, byteswap ), ws.separator,
        ws.memb_indent, "state", memb_name_w, ws.equals,
        _formatVariable( encoding->state, byteswap,
                         { PropertyNotify::state_names } ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::SelectionClear >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::SelectionClear;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SelectionClear::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const SelectionClear::Encoding* encoding {
        reinterpret_cast< const SelectionClear::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionClear::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::SELECTIONCLEAR );
    assert( outputs.bytes_parsed == SelectionClear::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "selection" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time, byteswap ), ws.separator,
        ws.memb_indent, "owner", memb_name_w, ws.equals,
        _formatVariable( encoding->owner, byteswap ), ws.separator,
        ws.memb_indent, "selection", memb_name_w, ws.equals,
        _formatVariable( encoding->selection, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::SelectionRequest >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::SelectionRequest;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SelectionRequest::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const SelectionRequest::Encoding* encoding {
        reinterpret_cast< const SelectionRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionRequest::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::SELECTIONREQUEST );
    assert( outputs.bytes_parsed == SelectionRequest::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "requestor" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time, byteswap,
                         { SelectionRequest::time_names } ), ws.separator,
        ws.memb_indent, "owner", memb_name_w, ws.equals,
        _formatVariable( encoding->owner, byteswap ), ws.separator,
        ws.memb_indent, "requestor", memb_name_w, ws.equals,
        _formatVariable( encoding->requestor, byteswap ), ws.separator,
        ws.memb_indent, "selection", memb_name_w, ws.equals,
        _formatVariable( encoding->selection, byteswap ), ws.separator,
        ws.memb_indent, "target", memb_name_w, ws.equals,
        _formatVariable( encoding->target, byteswap ), ws.separator,
        ws.memb_indent, "property", memb_name_w, ws.equals,
        _formatVariable( encoding->property, byteswap,
                         { SelectionRequest::property_names } ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::SelectionNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::SelectionNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SelectionNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const SelectionNotify::Encoding* encoding {
        reinterpret_cast< const SelectionNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::SELECTIONNOTIFY );
    assert( outputs.bytes_parsed == SelectionNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "requestor" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time, byteswap,
                         { SelectionNotify::time_names } ), ws.separator,
        ws.memb_indent, "requestor", memb_name_w, ws.equals,
        _formatVariable( encoding->requestor, byteswap ), ws.separator,
        ws.memb_indent, "selection", memb_name_w, ws.equals,
        _formatVariable( encoding->selection, byteswap ), ws.separator,
        ws.memb_indent, "target", memb_name_w, ws.equals,
        _formatVariable( encoding->target, byteswap ), ws.separator,
        ws.memb_indent, "property", memb_name_w, ws.equals,
        _formatVariable( encoding->property, byteswap,
                         { SelectionNotify::property_names } ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ColormapNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::ColormapNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ColormapNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const ColormapNotify::Encoding* encoding {
        reinterpret_cast< const ColormapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ColormapNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::COLORMAPNOTIFY );
    assert( outputs.bytes_parsed == ColormapNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "colormap" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "colormap", memb_name_w, ws.equals,
        _formatVariable( encoding->colormap, byteswap,
                         { ColormapNotify::colormap_names } ), ws.separator,
        ws.memb_indent, "new", memb_name_w, ws.equals,
        _formatVariable( encoding->new_, byteswap ), ws.separator,
        ws.memb_indent, "state", memb_name_w, ws.equals,
        _formatVariable( encoding->state, byteswap,
                         { ColormapNotify::state_names } ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ClientMessage >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::ClientMessage;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ClientMessage::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const ClientMessage::Encoding* encoding {
        reinterpret_cast< const ClientMessage::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ClientMessage::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::CLIENTMESSAGE );
    assert( sz >= ClientMessage::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "format" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        ws.memb_indent, "format", memb_name_w, ws.equals,
        _formatVariable( encoding->header.format, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "type", memb_name_w, ws.equals,
        _formatVariable( encoding->type, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MappingNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    using protocol::events::MappingNotify;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= MappingNotify::ENCODING_SZ );

    _ParsingOutputs outputs {};
    const bool byteswap { conn->byteswap };
    const MappingNotify::Encoding* encoding {
        reinterpret_cast< const MappingNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MappingNotify::Encoding );
    assert( ( _ordered( encoding->header.code, byteswap ) &
              protocol::requests::SendEvent::EVENT_CODE_MASK ) ==
            protocol::events::codes::MAPPINGNOTIFY );
    assert( outputs.bytes_parsed == MappingNotify::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "first-keycode" ) ) - 1 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "request", memb_name_w, ws.equals,
        _formatVariable( encoding->request, byteswap,
                         { MappingNotify::request_names } ), ws.separator,
        ws.memb_indent, "first-keycode", memb_name_w, ws.equals,
        _formatVariable( encoding->first_keycode, byteswap ), ws.separator,
        ws.memb_indent, "count", memb_name_w, ws.equals,
        _formatVariable( encoding->count, byteswap ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent(
    Connection* conn, const uint8_t* data, const size_t sz,
    const _Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::events::Event::ENCODING_SZ );

    const protocol::events::Event::Header* header {
        reinterpret_cast< const protocol::events::Event::Header* >( data ) };
    // event codes with msb toggled on are generated by request SendEvent
    const uint8_t code ( _ordered( header->code, conn->byteswap ) &
                         protocol::requests::SendEvent::EVENT_CODE_MASK );
    const _EventCodeTraits& code_traits { _event_codes.at( code ) };
    _ParsingOutputs event {
        // pointer-to-member access operator
        (this->*code_traits.parse_func)( conn, data, sz, ws ) };
    assert( event.bytes_parsed == protocol::events::Event::ENCODING_SZ );
    return event;
}
