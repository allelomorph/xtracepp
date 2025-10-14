#include <string_view>

#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/events.hpp"


template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::impl::InputEvent >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::impl::InputEvent;
    assert( sz >= sizeof( InputEvent::Encoding ) );
    const InputEvent::Encoding* encoding {
        reinterpret_cast< const InputEvent::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( InputEvent::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code >= protocol::events::codes::KEYPRESS &&
            code <= protocol::events::codes::BUTTONRELEASE );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
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
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatProtocolType( encoding->header.detail ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", name_width, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", name_width, ws.equals,
        _formatProtocolType( encoding->child, InputEvent::child_names ), ws.separator,
        ws.memb_indent, "root-x", name_width, ws.equals,
        _formatInteger( encoding->root_x ), ws.separator,
        ws.memb_indent, "root-y", name_width, ws.equals,
        _formatInteger( encoding->root_y ), ws.separator,
        ws.memb_indent, "event-x", name_width, ws.equals,
        _formatInteger( encoding->event_x ), ws.separator,
        ws.memb_indent, "event-y", name_width, ws.equals,
        _formatInteger( encoding->event_y ), ws.separator,
        ws.memb_indent, "state", name_width, ws.equals,
        _formatProtocolType( encoding->state ), ws.separator,
        ws.memb_indent, "same-screen", name_width, ws.equals,
        _formatProtocolType( encoding->same_screen ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::KeyPress >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    return _parseEvent< protocol::events::impl::InputEvent >(
        conn, data, sz, ws );
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::KeyRelease >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    return _parseEvent< protocol::events::impl::InputEvent >(
        conn, data, sz, ws );
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ButtonPress >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    return _parseEvent< protocol::events::impl::InputEvent >(
        conn, data, sz, ws );
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ButtonRelease >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    return _parseEvent< protocol::events::impl::InputEvent >(
        conn, data, sz, ws );
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MotionNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::MotionNotify;
    assert( sz >= sizeof( MotionNotify::Encoding ) );
    const MotionNotify::Encoding* encoding {
        reinterpret_cast< const MotionNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MotionNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::MOTIONNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
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
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatInteger( encoding->header.detail,
                        MotionNotify::detail_names ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", name_width, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", name_width, ws.equals,
        _formatProtocolType( encoding->child,
                             MotionNotify::child_names ), ws.separator,
        ws.memb_indent, "root-x", name_width, ws.equals,
        _formatInteger( encoding->root_x ), ws.separator,
        ws.memb_indent, "root-y", name_width, ws.equals,
        _formatInteger( encoding->root_y ), ws.separator,
        ws.memb_indent, "event-x", name_width, ws.equals,
        _formatInteger( encoding->event_x ), ws.separator,
        ws.memb_indent, "event-y", name_width, ws.equals,
        _formatInteger( encoding->event_y ), ws.separator,
        ws.memb_indent, "state", name_width, ws.equals,
        _formatProtocolType( encoding->state ), ws.separator,
        ws.memb_indent, "same-screen", name_width, ws.equals,
        _formatProtocolType( encoding->same_screen ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::impl::BoundaryEvent >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::impl::BoundaryEvent;
    assert( sz >= sizeof( BoundaryEvent::Encoding ) );
    const BoundaryEvent::Encoding* encoding {
        reinterpret_cast< const BoundaryEvent::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( BoundaryEvent::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::ENTERNOTIFY ||
            code == protocol::events::codes::LEAVENOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "same-screen/focus" ) - 1 : 0 );
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
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatInteger( encoding->header.detail, BoundaryEvent::detail_names,
                        _IndexRange{ 0, BoundaryEvent::MAX_DETAIL } ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", name_width, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", name_width, ws.equals,
        _formatProtocolType( encoding->child,
                             BoundaryEvent::child_names ), ws.separator,
        ws.memb_indent, "root-x", name_width, ws.equals,
        _formatInteger( encoding->root_x ), ws.separator,
        ws.memb_indent, "root-y", name_width, ws.equals,
        _formatInteger( encoding->root_y ), ws.separator,
        ws.memb_indent, "event-x", name_width, ws.equals,
        _formatInteger( encoding->event_x ), ws.separator,
        ws.memb_indent, "event-y", name_width, ws.equals,
        _formatInteger( encoding->event_y ), ws.separator,
        ws.memb_indent, "state", name_width, ws.equals,
        _formatProtocolType( encoding->state ), ws.separator,
        ws.memb_indent, "mode", name_width, ws.equals,
        _formatInteger( encoding->mode, BoundaryEvent::mode_names,
                        _IndexRange{ 0, BoundaryEvent::MAX_MODE } ), ws.separator,
        ws.memb_indent, "same-screen/focus", name_width, ws.equals,
        _formatBitmask( encoding->focus_same_screen_mask,
                        BoundaryEvent::focus_same_screen_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::EnterNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    return _parseEvent< protocol::events::impl::BoundaryEvent >(
        conn, data, sz, ws );
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::LeaveNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    return _parseEvent< protocol::events::impl::BoundaryEvent >(
        conn, data, sz, ws );
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::impl::FocusEvent >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::impl::FocusEvent;
    assert( sz >= sizeof( FocusEvent::Encoding ) );
    const FocusEvent::Encoding* encoding {
        reinterpret_cast< const FocusEvent::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( FocusEvent::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::FOCUSIN ||
            code == protocol::events::codes::FOCUSOUT );

    const uint32_t name_width (
        settings.multiline ? sizeof( "same-screen/focus" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatInteger( encoding->header.detail,
                        FocusEvent::detail_names ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "mode", name_width, ws.equals,
        _formatInteger( encoding->mode,
                        FocusEvent::mode_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::FocusIn >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    return _parseEvent< protocol::events::impl::FocusEvent >(
        conn, data, sz, ws );
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::FocusOut >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    return _parseEvent< protocol::events::impl::FocusEvent >(
        conn, data, sz, ws );
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::KeymapNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::KeymapNotify;
    assert( sz >= sizeof( KeymapNotify::Encoding ) );
    const KeymapNotify::Encoding* encoding {
        reinterpret_cast< const KeymapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( KeymapNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::KEYMAPNOTIFY );

    // TBD can we develop a way of printing the keyboard state instead of bytes?
    _ParsingOutputs keys {
        _parseLISTof< protocol::CARD8 >(
            encoding->keys, sizeof( encoding->keys ), sizeof( encoding->keys ),
            ws.nested( _Whitespace::SINGLELINE ) ) };

    const uint32_t name_width (
        settings.multiline ? sizeof( "keys(0-7 omitted)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "keys(0-7 omitted)", name_width, ws.equals,
        keys.str, ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::Expose >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::Expose;
    assert( sz >= sizeof( Expose::Encoding ) );
    const Expose::Encoding* encoding {
        reinterpret_cast< const Expose::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( Expose::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::EXPOSE );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "x", name_width, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", name_width, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.memb_indent, "width", name_width, ws.equals,
        _formatInteger( encoding->width ), ws.separator,
        ws.memb_indent, "height", name_width, ws.equals,
        _formatInteger( encoding->height ), ws.separator,
        ws.memb_indent, "count", name_width, ws.equals,
        _formatInteger( encoding->count ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::GraphicsExposure >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::GraphicsExposure;
    assert( sz >= sizeof( GraphicsExposure::Encoding ) );
    const GraphicsExposure::Encoding* encoding {
        reinterpret_cast< const GraphicsExposure::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( GraphicsExposure::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::GRAPHICSEXPOSURE );

    // TBD lookup request opcodes and assert against encoding opcodes?
    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "drawable", name_width, ws.equals,
        _formatProtocolType( encoding->drawable ), ws.separator,
        ws.memb_indent, "x", name_width, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", name_width, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.memb_indent, "width", name_width, ws.equals,
        _formatInteger( encoding->width ), ws.separator,
        ws.memb_indent, "height", name_width, ws.equals,
        _formatInteger( encoding->height ), ws.separator,
        ws.memb_indent, "minor-opcode", name_width, ws.equals,
        _formatInteger( encoding->minor_opcode ), ws.separator,
        ws.memb_indent, "count", name_width, ws.equals,
        _formatInteger( encoding->count ), ws.separator,
        ws.memb_indent, "major-opcode", name_width, ws.equals,
        _formatInteger( encoding->major_opcode ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::NoExposure >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::NoExposure;
    assert( sz >= sizeof( NoExposure::Encoding ) );
    const NoExposure::Encoding* encoding {
        reinterpret_cast< const NoExposure::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( NoExposure::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::NOEXPOSURE );

    // TBD lookup request opcodes and assert against encoding opcodes?
    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "drawable", name_width, ws.equals,
        _formatProtocolType( encoding->drawable ), ws.separator,
        ws.memb_indent, "minor-opcode", name_width, ws.equals,
        _formatInteger( encoding->minor_opcode ), ws.separator,
        ws.memb_indent, "major-opcode", name_width, ws.equals,
        _formatInteger( encoding->major_opcode ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::VisibilityNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::VisibilityNotify;
    assert( sz >= sizeof( VisibilityNotify::Encoding ) );
    const VisibilityNotify::Encoding* encoding {
        reinterpret_cast< const VisibilityNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( VisibilityNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::VISIBILITYNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "state", name_width, ws.equals,
        _formatInteger( encoding->state,
                        VisibilityNotify::state_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::CreateNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::CreateNotify;
    assert( sz >= sizeof( CreateNotify::Encoding ) );
    const CreateNotify::Encoding* encoding {
        reinterpret_cast< const CreateNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CreateNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::CREATENOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "override-redirect" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "parent", name_width, ws.equals,
        _formatProtocolType( encoding->parent ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "x", name_width, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", name_width, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.memb_indent, "width", name_width, ws.equals,
        _formatInteger( encoding->width ), ws.separator,
        ws.memb_indent, "height", name_width, ws.equals,
        _formatInteger( encoding->height ), ws.separator,
        ws.memb_indent, "border-width", name_width, ws.equals,
        _formatInteger( encoding->border_width ), ws.separator,
        ws.memb_indent, "override-redirect", name_width, ws.equals,
        _formatProtocolType( encoding->override_redirect ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::DestroyNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::DestroyNotify;
    assert( sz >= sizeof( DestroyNotify::Encoding ) );
    const DestroyNotify::Encoding* encoding {
        reinterpret_cast< const DestroyNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( DestroyNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::DESTROYNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::UnmapNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::UnmapNotify;
    assert( sz >= sizeof( UnmapNotify::Encoding ) );
    const UnmapNotify::Encoding* encoding {
        reinterpret_cast< const UnmapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( UnmapNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::UNMAPNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "from-configure", name_width, ws.equals,
        _formatProtocolType( encoding->from_configure ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MapNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::MapNotify;
    assert( sz >= sizeof( MapNotify::Encoding ) );
    const MapNotify::Encoding* encoding {
        reinterpret_cast< const MapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MapNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::MAPNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "override-redirect" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "override-redirect", name_width, ws.equals,
        _formatProtocolType( encoding->override_redirect ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MapRequest >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::MapRequest;
    assert( sz >= sizeof( MapRequest::Encoding ) );
    const MapRequest::Encoding* encoding {
        reinterpret_cast< const MapRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MapRequest::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::MAPREQUEST );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "parent", name_width, ws.equals,
        _formatProtocolType( encoding->parent ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ReparentNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::ReparentNotify;
    assert( sz >= sizeof( ReparentNotify::Encoding ) );
    const ReparentNotify::Encoding* encoding {
        reinterpret_cast< const ReparentNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ReparentNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( encoding->header.code == protocol::events::codes::REPARENTNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "override-redirect" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "parent", name_width, ws.equals,
        _formatProtocolType( encoding->parent ), ws.separator,
        ws.memb_indent, "x", name_width, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", name_width, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.memb_indent, "override-redirect", name_width, ws.equals,
        _formatProtocolType( encoding->override_redirect ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ConfigureNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::ConfigureNotify;
    assert( sz >= sizeof( ConfigureNotify::Encoding ) );
    const ConfigureNotify::Encoding* encoding {
        reinterpret_cast< const ConfigureNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ConfigureNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::CONFIGURENOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "override-redirect" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "above-sibling", name_width, ws.equals,
        _formatProtocolType( encoding->above_sibling,
                             ConfigureNotify::above_sibling_names ), ws.separator,
        ws.memb_indent, "x", name_width, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", name_width, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.memb_indent, "width", name_width, ws.equals,
        _formatInteger( encoding->width ), ws.separator,
        ws.memb_indent, "height", name_width, ws.equals,
        _formatInteger( encoding->height ), ws.separator,
        ws.memb_indent, "border-width", name_width, ws.equals,
        _formatInteger( encoding->border_width ), ws.separator,
        ws.memb_indent, "override-redirect", name_width, ws.equals,
        _formatProtocolType( encoding->override_redirect ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ConfigureRequest >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::ConfigureRequest;
    assert( sz >= sizeof( ConfigureRequest::Encoding ) );
    const ConfigureRequest::Encoding* encoding {
        reinterpret_cast< const ConfigureRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ConfigureRequest::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::CONFIGUREREQUEST );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
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
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "stack-mode", name_width, ws.equals,
        _formatInteger( encoding->header.stack_mode,
                        ConfigureRequest::stack_mode_names ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "parent", name_width, ws.equals,
        _formatProtocolType( encoding->parent ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "sibling", name_width, ws.equals,
        _formatProtocolType( encoding->sibling,
                             ConfigureRequest::sibling_names ), ws.separator,
        ws.memb_indent, "x", name_width, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", name_width, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.memb_indent, "width", name_width, ws.equals,
        _formatInteger( encoding->width ), ws.separator,
        ws.memb_indent, "height", name_width, ws.equals,
        _formatInteger( encoding->height ), ws.separator,
        ws.memb_indent, "border-width", name_width, ws.equals,
        _formatInteger( encoding->border_width ), ws.separator,
        ws.memb_indent, "value-mask", name_width, ws.equals,
        _formatBitmask( encoding->value_mask, ConfigureRequest::value_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::GravityNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::GravityNotify;
    assert( sz >= sizeof( GravityNotify::Encoding ) );
    const GravityNotify::Encoding* encoding {
        reinterpret_cast< const GravityNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( GravityNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::GRAVITYNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "x", name_width, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", name_width, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ResizeRequest >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::ResizeRequest;
    assert( sz >= sizeof( ResizeRequest::Encoding ) );
    const ResizeRequest::Encoding* encoding {
        reinterpret_cast< const ResizeRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ResizeRequest::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::RESIZEREQUEST );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "width", name_width, ws.equals,
        _formatInteger( encoding->width ), ws.separator,
        ws.memb_indent, "height", name_width, ws.equals,
        _formatInteger( encoding->height ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::CirculateNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::CirculateNotify;
    assert( sz >= sizeof( CirculateNotify::Encoding ) );
    const CirculateNotify::Encoding* encoding {
        reinterpret_cast< const CirculateNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CirculateNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::CIRCULATENOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "place", name_width, ws.equals,
        _formatInteger( encoding->place, CirculateNotify::place_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::CirculateRequest >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::CirculateRequest;
    assert( sz >= sizeof( CirculateRequest::Encoding ) );
    const CirculateRequest::Encoding* encoding {
        reinterpret_cast< const CirculateRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CirculateRequest::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::CIRCULATEREQUEST );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "parent", name_width, ws.equals,
        _formatProtocolType( encoding->parent ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "place", name_width, ws.equals,
        _formatInteger( encoding->place, CirculateRequest::place_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::PropertyNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::PropertyNotify;
    assert( sz >= sizeof( PropertyNotify::Encoding ) );
    const PropertyNotify::Encoding* encoding {
        reinterpret_cast< const PropertyNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( PropertyNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::PROPERTYNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "atom", name_width, ws.equals,
        _formatProtocolType( encoding->atom ), ws.separator,
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "state", name_width, ws.equals,
        _formatInteger( encoding->state, PropertyNotify::state_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::SelectionClear >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::SelectionClear;
    assert( sz >= sizeof( SelectionClear::Encoding ) );
    const SelectionClear::Encoding* encoding {
        reinterpret_cast< const SelectionClear::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionClear::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::SELECTIONCLEAR );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "owner", name_width, ws.equals,
        _formatProtocolType( encoding->owner ), ws.separator,
        ws.memb_indent, "selection", name_width, ws.equals,
        _formatProtocolType( encoding->selection ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::SelectionRequest >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::SelectionRequest;
    assert( sz >= sizeof( SelectionRequest::Encoding ) );
    const SelectionRequest::Encoding* encoding {
        reinterpret_cast< const SelectionRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionRequest::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::SELECTIONREQUEST );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time,
                             SelectionRequest::time_names ), ws.separator,
        ws.memb_indent, "owner", name_width, ws.equals,
        _formatProtocolType( encoding->owner ), ws.separator,
        ws.memb_indent, "requestor", name_width, ws.equals,
        _formatProtocolType( encoding->requestor ), ws.separator,
        ws.memb_indent, "selection", name_width, ws.equals,
        _formatProtocolType( encoding->selection ), ws.separator,
        ws.memb_indent, "target", name_width, ws.equals,
        _formatProtocolType( encoding->target ), ws.separator,
        ws.memb_indent, "property", name_width, ws.equals,
        _formatProtocolType( encoding->property,
                             SelectionRequest::property_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::SelectionNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::SelectionNotify;
    assert( sz >= sizeof( SelectionNotify::Encoding ) );
    const SelectionNotify::Encoding* encoding {
        reinterpret_cast< const SelectionNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::SELECTIONNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time,
                             SelectionNotify::time_names ), ws.separator,
        ws.memb_indent, "requestor", name_width, ws.equals,
        _formatProtocolType( encoding->requestor ), ws.separator,
        ws.memb_indent, "selection", name_width, ws.equals,
        _formatProtocolType( encoding->selection ), ws.separator,
        ws.memb_indent, "target", name_width, ws.equals,
        _formatProtocolType( encoding->target ), ws.separator,
        ws.memb_indent, "property", name_width, ws.equals,
        _formatProtocolType( encoding->property,
                             SelectionNotify::property_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ColormapNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::ColormapNotify;
    assert( sz >= sizeof( ColormapNotify::Encoding ) );
    const ColormapNotify::Encoding* encoding {
        reinterpret_cast< const ColormapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ColormapNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::COLORMAPNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "colormap", name_width, ws.equals,
        _formatProtocolType( encoding->colormap,
                             ColormapNotify::colormap_names ), ws.separator,
        ws.memb_indent, "new", name_width, ws.equals,
        _formatProtocolType( encoding->new_ ), ws.separator,
        ws.memb_indent, "state", name_width, ws.equals,
        _formatInteger( encoding->state,
                        ColormapNotify::state_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ClientMessage >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::ClientMessage;
    assert( sz >= sizeof( ClientMessage::Encoding ) );
    const ClientMessage::Encoding* encoding {
        reinterpret_cast< const ClientMessage::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ClientMessage::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::CLIENTMESSAGE );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "format", name_width, ws.equals,
        _formatInteger( encoding->header.format ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "type", name_width, ws.equals,
        _formatProtocolType( encoding->type ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MappingNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::MappingNotify;
    assert( sz >= sizeof( MappingNotify::Encoding ) );
    const MappingNotify::Encoding* encoding {
        reinterpret_cast< const MappingNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MappingNotify::Encoding );
    const uint8_t code ( encoding->header.code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::MAPPINGNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "(sequence number)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", name_width, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "request", name_width, ws.equals,
        _formatInteger( encoding->request,
                        MappingNotify::request_names ), ws.separator,
        ws.memb_indent, "first-keycode", name_width, ws.equals,
        _formatProtocolType( encoding->first_keycode ), ws.separator,
        ws.memb_indent, "count", name_width, ws.equals,
        _formatInteger( encoding->count ), ws.separator,
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

    // event codes with msb turned on are generated by request SendEvent
    const uint8_t code (
        reinterpret_cast< const protocol::events::Event::Header* >( data )->code &
        _EVENT_CODE_MASK );
    // TBD will change with extensions
    assert( code >= protocol::events::codes::MIN &&
            code <= protocol::events::codes::MAX );
    _ParsingOutputs event {};
    switch ( code ) {
    case protocol::events::codes::KEYPRESS:          //  2
        event = _parseEvent< protocol::events::KeyPress >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::KEYRELEASE:        //  3
        event = _parseEvent< protocol::events::KeyRelease >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::BUTTONPRESS:       //  4
        event = _parseEvent< protocol::events::ButtonPress >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::BUTTONRELEASE:     //  5
        event = _parseEvent< protocol::events::ButtonRelease >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::MOTIONNOTIFY:      //  6
        event = _parseEvent< protocol::events::MotionNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::ENTERNOTIFY:       //  7
        event = _parseEvent< protocol::events::EnterNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::LEAVENOTIFY:       //  8
        event = _parseEvent< protocol::events::LeaveNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::FOCUSIN:           //  9
        event = _parseEvent< protocol::events::FocusIn >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::FOCUSOUT:          // 10
        event = _parseEvent< protocol::events::FocusOut >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::KEYMAPNOTIFY:      // 11
        event = _parseEvent< protocol::events::KeymapNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::EXPOSE:            // 12
        event = _parseEvent< protocol::events::Expose >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::GRAPHICSEXPOSURE:  // 13
        event = _parseEvent< protocol::events::GraphicsExposure >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::NOEXPOSURE:        // 14
        event = _parseEvent< protocol::events::NoExposure >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::VISIBILITYNOTIFY:  // 15
        event = _parseEvent< protocol::events::VisibilityNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::CREATENOTIFY:      // 16
        event = _parseEvent< protocol::events::CreateNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::DESTROYNOTIFY:     // 17
        event = _parseEvent< protocol::events::DestroyNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::UNMAPNOTIFY:       // 18
        event = _parseEvent< protocol::events::UnmapNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::MAPNOTIFY:         // 19
        event = _parseEvent< protocol::events::MapNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::MAPREQUEST:        // 20
        event = _parseEvent< protocol::events::MapRequest >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::REPARENTNOTIFY:    // 21
        event = _parseEvent< protocol::events::ReparentNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::CONFIGURENOTIFY:   // 22
        event = _parseEvent< protocol::events::ConfigureNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::CONFIGUREREQUEST:  // 23
        event = _parseEvent< protocol::events::ConfigureRequest >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::GRAVITYNOTIFY:     // 24
        event = _parseEvent< protocol::events::GravityNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::RESIZEREQUEST:     // 25
        event = _parseEvent< protocol::events::ResizeRequest >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::CIRCULATENOTIFY:   // 26
        event = _parseEvent< protocol::events::CirculateNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::CIRCULATEREQUEST:  // 27
        event = _parseEvent< protocol::events::CirculateRequest >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::PROPERTYNOTIFY:    // 28
        event = _parseEvent< protocol::events::PropertyNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::SELECTIONCLEAR:    // 29
        event = _parseEvent< protocol::events::SelectionClear >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::SELECTIONREQUEST:  // 30
        event = _parseEvent< protocol::events::SelectionRequest >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::SELECTIONNOTIFY:   // 31
        event = _parseEvent< protocol::events::SelectionNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::COLORMAPNOTIFY:    // 32
        event = _parseEvent< protocol::events::ColormapNotify >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::CLIENTMESSAGE:     // 33
        event = _parseEvent< protocol::events::ClientMessage >(
            conn, data, sz, ws );
        break;
    case protocol::events::codes::MAPPINGNOTIFY:     // 34
        event = _parseEvent< protocol::events::MappingNotify >(
            conn, data, sz, ws );
        break;
    default:
        break;
    }
    assert( event.bytes_parsed == protocol::events::Event::ENCODING_SZ );
    return event;
}

// logging of events implemented as wrapper around _parseEvent to accommodate
//    logging of both normal events and those encoded by SendEvent
size_t X11ProtocolParser::_logEvent(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::events::Event::ENCODING_SZ );

    const protocol::events::Event::Header* header {
        reinterpret_cast< const protocol::events::Event::Header* >( data ) };
    // event codes with msb turned on are generated by request SendEvent
    const uint8_t code ( header->code & _EVENT_CODE_MASK );
    // KeymapNotify presents edge case, as it does not encode a sequence number
    const std::string sequence_num_str {
        ( header->code == protocol::events::codes::KEYMAPNOTIFY ) ? "?????" :
        fmt::format( "{:05d}", header->sequence_num ) };
    const _ParsingOutputs event {
        _parseEvent( conn, data, sz, _ROOT_WS ) };
    fmt::println( settings.log_fs,
                  "C{:03d}:{:04d}B:{}:S{}: Event {}({}){}: {}",
                  conn->id, event.bytes_parsed, _SERVER_TO_CLIENT,
                  sequence_num_str,
                  protocol::events::names[ code ], code,
                  header->code & _GENERATED_EVENT_FLAG ? " (generated)" : "",
                  event.str );
    assert( event.bytes_parsed == protocol::events::Event::ENCODING_SZ );
    return event.bytes_parsed;
}
