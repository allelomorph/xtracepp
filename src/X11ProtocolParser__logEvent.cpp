#include <string_view>

#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/events.hpp"


template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::KeyPress >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::KeyPress;
    assert( sz >= sizeof( KeyPress::Encoding ) ); // TBD
    const KeyPress::Encoding* encoding {
        reinterpret_cast< const KeyPress::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( KeyPress::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::KEYPRESS );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatProtocolType( encoding->detail ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", name_width, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", name_width, ws.equals,
        _formatProtocolType( encoding->child, KeyPress::child_names ), ws.separator,
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

// TBD can we have a shared specialization given that the encoding is the same as
//   KeyPress?
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::KeyRelease >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::KeyRelease;
    assert( sz >= sizeof( KeyRelease::Encoding ) ); // TBD
    const KeyRelease::Encoding* encoding {
        reinterpret_cast< const KeyRelease::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( KeyRelease::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::KEYRELEASE );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatProtocolType( encoding->detail ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", name_width, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", name_width, ws.equals,
        _formatProtocolType( encoding->child, KeyRelease::child_names ), ws.separator,
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

// TBD only differs from KeyPress/KeyRelease encoding by type of `detail`
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ButtonPress >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::ButtonPress;
    assert( sz >= sizeof( ButtonPress::Encoding ) ); // TBD
    const ButtonPress::Encoding* encoding {
        reinterpret_cast< const ButtonPress::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ButtonPress::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::BUTTONPRESS );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatProtocolType( encoding->detail ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", name_width, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", name_width, ws.equals,
        _formatProtocolType( encoding->child, ButtonPress::child_names ), ws.separator,
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

// TBD can we have a shared specialization given that the encoding is the same as
//   ButtonPress?
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ButtonRelease >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::ButtonRelease;
    assert( sz >= sizeof( ButtonRelease::Encoding ) ); // TBD
    const ButtonRelease::Encoding* encoding {
        reinterpret_cast< const ButtonRelease::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ButtonRelease::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::BUTTONRELEASE );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatProtocolType( encoding->detail ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", name_width, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", name_width, ws.equals,
        _formatProtocolType( encoding->child, ButtonRelease::child_names ), ws.separator,
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

// TBD only differs from KeyPress/KeyRelease/ButtonPress/ButtonRelease encoding by type of `detail`
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MotionNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::MotionNotify;
    assert( sz >= sizeof( MotionNotify::Encoding ) ); // TBD
    const MotionNotify::Encoding* encoding {
        reinterpret_cast< const MotionNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MotionNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::MOTIONNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatInteger( encoding->detail, MotionNotify::detail_names ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", name_width, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", name_width, ws.equals,
        _formatProtocolType( encoding->child, MotionNotify::child_names ), ws.separator,
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
X11ProtocolParser::_parseEvent< protocol::events::EnterNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::EnterNotify;
    assert( sz >= sizeof( EnterNotify::Encoding ) ); // TBD
    const EnterNotify::Encoding* encoding {
        reinterpret_cast< const EnterNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( EnterNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( encoding->code == protocol::events::codes::ENTERNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "same-screen/focus" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        // TBD how to indicate max enum without magic number
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatInteger( encoding->detail,
                        EnterNotify::detail_names, _IndexRange{ 0, 4 } ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", name_width, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", name_width, ws.equals,
        _formatProtocolType( encoding->child, EnterNotify::child_names ), ws.separator,
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
        // TBD how to indicate max enum without magic number
        ws.memb_indent, "mode", name_width, ws.equals,
        _formatInteger( encoding->mode,
                        EnterNotify::mode_names, _IndexRange{ 0, 2 } ), ws.separator,
        ws.memb_indent, "same-screen/focus", name_width, ws.equals,
        _formatBitmask( encoding->focus_same_screen, EnterNotify::focus_same_screen_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

// TBD shared encoding with EnterNotify
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::LeaveNotify >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::LeaveNotify;
    assert( sz >= sizeof( LeaveNotify::Encoding ) ); // TBD
    const LeaveNotify::Encoding* encoding {
        reinterpret_cast< const LeaveNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( LeaveNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::LEAVENOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "same-screen/focus" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        // TBD how to indicate max enum without magic number
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatInteger( encoding->detail,
                        LeaveNotify::detail_names, _IndexRange{ 0, 4 } ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", name_width, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", name_width, ws.equals,
        _formatProtocolType( encoding->child, LeaveNotify::child_names ), ws.separator,
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
        // TBD how to indicate max enum without magic number
        ws.memb_indent, "mode", name_width, ws.equals,
        _formatInteger( encoding->mode,
                        LeaveNotify::mode_names, _IndexRange{ 0, 2 } ), ws.separator,
        ws.memb_indent, "same-screen/focus", name_width, ws.equals,
        _formatBitmask( encoding->focus_same_screen, LeaveNotify::focus_same_screen_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::FocusIn >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::FocusIn;
    assert( sz >= sizeof( FocusIn::Encoding ) ); // TBD
    const FocusIn::Encoding* encoding {
        reinterpret_cast< const FocusIn::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( FocusIn::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::FOCUSIN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "same-screen/focus" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        // TBD how to indicate max enum without magic number
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatInteger( encoding->detail, FocusIn::detail_names ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "mode", name_width, ws.equals,
        _formatInteger( encoding->mode, FocusIn::mode_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
}

// TBD shared encoding with FocusIn
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::FocusOut >(
    Connection* conn, const uint8_t* data, const size_t sz,
    const X11ProtocolParser::_Whitespace& ws ) {
    assert( conn != nullptr );
    assert( data != nullptr );

    _ParsingOutputs outputs {};
    using protocol::events::FocusOut;
    assert( sz >= sizeof( FocusOut::Encoding ) ); // TBD
    const FocusOut::Encoding* encoding {
        reinterpret_cast< const FocusOut::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( FocusOut::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::FOCUSOUT );

    const uint32_t name_width (
        settings.multiline ? sizeof( "same-screen/focus" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        // TBD how to indicate max enum without magic number
        ws.memb_indent, "detail", name_width, ws.equals,
        _formatInteger( encoding->detail, FocusOut::detail_names ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "mode", name_width, ws.equals,
        _formatInteger( encoding->mode, FocusOut::mode_names ), ws.separator,
        ws.encl_indent
        );
    return outputs;
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
    assert( sz >= sizeof( KeymapNotify::Encoding ) ); // TBD
    const KeymapNotify::Encoding* encoding {
        reinterpret_cast< const KeymapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( KeymapNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::KEYMAPNOTIFY );

    // TBD can we develop a way of printing the keyboard state instead of bytes?
    _ParsingOutputs keys {
        _parseLISTof< protocol::CARD8 >(
            encoding->keys, sizeof( encoding->keys ), sizeof( encoding->keys ),
            ws.nested( _Whitespace::SINGLELINE ) ) };

    const uint32_t name_width (
        settings.multiline ? sizeof( "keys(0-7 omitted)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        // TBD can we make a way to log the keyboard state from this map?
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
    assert( sz >= sizeof( Expose::Encoding ) ); // TBD
    const Expose::Encoding* encoding {
        reinterpret_cast< const Expose::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( Expose::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::EXPOSE );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( GraphicsExposure::Encoding ) ); // TBD
    const GraphicsExposure::Encoding* encoding {
        reinterpret_cast< const GraphicsExposure::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( GraphicsExposure::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::GRAPHICSEXPOSURE );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
        // TBD breakout reuqest printing by opcode "RequestName(opcode)" in separate func
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
    assert( sz >= sizeof( NoExposure::Encoding ) ); // TBD
    const NoExposure::Encoding* encoding {
        reinterpret_cast< const NoExposure::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( NoExposure::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::NOEXPOSURE );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "drawable", name_width, ws.equals,
        _formatProtocolType( encoding->drawable ), ws.separator,
        ws.memb_indent, "minor-opcode", name_width, ws.equals,
        _formatInteger( encoding->minor_opcode ), ws.separator,
        // TBD breakout reuqest printing by opcode "RequestName(opcode)" in separate func
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
    assert( sz >= sizeof( VisibilityNotify::Encoding ) ); // TBD
    const VisibilityNotify::Encoding* encoding {
        reinterpret_cast< const VisibilityNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( VisibilityNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::VISIBILITYNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "state", name_width, ws.equals,
        _formatInteger( encoding->state, VisibilityNotify::state_names ), ws.separator,
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
    assert( sz >= sizeof( CreateNotify::Encoding ) ); // TBD
    const CreateNotify::Encoding* encoding {
        reinterpret_cast< const CreateNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CreateNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::CREATENOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "override-redirect" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( DestroyNotify::Encoding ) ); // TBD
    const DestroyNotify::Encoding* encoding {
        reinterpret_cast< const DestroyNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( DestroyNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::DESTROYNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( UnmapNotify::Encoding ) ); // TBD
    const UnmapNotify::Encoding* encoding {
        reinterpret_cast< const UnmapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( UnmapNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::UNMAPNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( MapNotify::Encoding ) ); // TBD
    const MapNotify::Encoding* encoding {
        reinterpret_cast< const MapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MapNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::MAPNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "override-redirect" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( MapRequest::Encoding ) ); // TBD
    const MapRequest::Encoding* encoding {
        reinterpret_cast< const MapRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MapRequest::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::MAPREQUEST );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( ReparentNotify::Encoding ) ); // TBD
    const ReparentNotify::Encoding* encoding {
        reinterpret_cast< const ReparentNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ReparentNotify::Encoding );
    assert( encoding->code == protocol::events::codes::REPARENTNOTIFY ||
            encoding->code == protocol::events::codes::REPARENTNOTIFY | 0x80 );

    const uint32_t name_width (
        settings.multiline ? sizeof( "override-redirect" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( ConfigureNotify::Encoding ) ); // TBD
    const ConfigureNotify::Encoding* encoding {
        reinterpret_cast< const ConfigureNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ConfigureNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::CONFIGURENOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "override-redirect" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "event", name_width, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "above-sibling", name_width, ws.equals,
        _formatProtocolType( encoding->above_sibling, ConfigureNotify::above_sibling_names ), ws.separator,
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
    assert( sz >= sizeof( ConfigureRequest::Encoding ) ); // TBD
    const ConfigureRequest::Encoding* encoding {
        reinterpret_cast< const ConfigureRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ConfigureRequest::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::CONFIGUREREQUEST );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        ws.memb_indent, "stack-mode", name_width, ws.equals,
        _formatInteger( encoding->stack_mode, ConfigureRequest::stack_mode_names ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "parent", name_width, ws.equals,
        _formatProtocolType( encoding->parent ), ws.separator,
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "sibling", name_width, ws.equals,
        _formatProtocolType( encoding->sibling, ConfigureRequest::sibling_names ), ws.separator,
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
    assert( sz >= sizeof( GravityNotify::Encoding ) ); // TBD
    const GravityNotify::Encoding* encoding {
        reinterpret_cast< const GravityNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( GravityNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::GRAVITYNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( ResizeRequest::Encoding ) ); // TBD
    const ResizeRequest::Encoding* encoding {
        reinterpret_cast< const ResizeRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ResizeRequest::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::RESIZEREQUEST );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( CirculateNotify::Encoding ) ); // TBD
    const CirculateNotify::Encoding* encoding {
        reinterpret_cast< const CirculateNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CirculateNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::CIRCULATENOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( CirculateRequest::Encoding ) ); // TBD
    const CirculateRequest::Encoding* encoding {
        reinterpret_cast< const CirculateRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CirculateRequest::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::CIRCULATEREQUEST );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( PropertyNotify::Encoding ) ); // TBD
    const PropertyNotify::Encoding* encoding {
        reinterpret_cast< const PropertyNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( PropertyNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::PROPERTYNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( SelectionClear::Encoding ) ); // TBD
    const SelectionClear::Encoding* encoding {
        reinterpret_cast< const SelectionClear::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionClear::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::SELECTIONCLEAR );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( SelectionRequest::Encoding ) ); // TBD
    const SelectionRequest::Encoding* encoding {
        reinterpret_cast< const SelectionRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionRequest::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::SELECTIONREQUEST );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time, SelectionRequest::time_names ), ws.separator,
        ws.memb_indent, "owner", name_width, ws.equals,
        _formatProtocolType( encoding->owner ), ws.separator,
        ws.memb_indent, "requestor", name_width, ws.equals,
        _formatProtocolType( encoding->requestor ), ws.separator,
        ws.memb_indent, "selection", name_width, ws.equals,
        _formatProtocolType( encoding->selection ), ws.separator,
        ws.memb_indent, "target", name_width, ws.equals,
        _formatProtocolType( encoding->target ), ws.separator,
        ws.memb_indent, "property", name_width, ws.equals,
        _formatProtocolType( encoding->property, SelectionRequest::property_names ), ws.separator,
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
    assert( sz >= sizeof( SelectionNotify::Encoding ) ); // TBD
    const SelectionNotify::Encoding* encoding {
        reinterpret_cast< const SelectionNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::SELECTIONNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "time", name_width, ws.equals,
        _formatProtocolType( encoding->time, SelectionNotify::time_names ), ws.separator,
        ws.memb_indent, "requestor", name_width, ws.equals,
        _formatProtocolType( encoding->requestor ), ws.separator,
        ws.memb_indent, "selection", name_width, ws.equals,
        _formatProtocolType( encoding->selection ), ws.separator,
        ws.memb_indent, "target", name_width, ws.equals,
        _formatProtocolType( encoding->target ), ws.separator,
        ws.memb_indent, "property", name_width, ws.equals,
        _formatProtocolType( encoding->property, SelectionNotify::property_names ), ws.separator,
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
    assert( sz >= sizeof( ColormapNotify::Encoding ) ); // TBD
    const ColormapNotify::Encoding* encoding {
        reinterpret_cast< const ColormapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ColormapNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::COLORMAPNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "window", name_width, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "colormap", name_width, ws.equals,
        _formatProtocolType( encoding->colormap, ColormapNotify::colormap_names ), ws.separator,
        ws.memb_indent, "new", name_width, ws.equals,
        _formatProtocolType( encoding->new_ ), ws.separator,
        ws.memb_indent, "state", name_width, ws.equals,
        _formatInteger( encoding->state, ColormapNotify::state_names ), ws.separator,
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
    assert( sz >= sizeof( ClientMessage::Encoding ) ); // TBD
    const ClientMessage::Encoding* encoding {
        reinterpret_cast< const ClientMessage::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ClientMessage::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::CLIENTMESSAGE );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        ws.memb_indent, "format", name_width, ws.equals,
        _formatInteger( encoding->format ), ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
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
    assert( sz >= sizeof( MappingNotify::Encoding ) ); // TBD
    const MappingNotify::Encoding* encoding {
        reinterpret_cast< const MappingNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MappingNotify::Encoding );
    const uint8_t code ( encoding->code & _EVENT_CODE_MASK );
    assert( code == protocol::events::codes::MAPPINGNOTIFY );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {}{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[code], code,
        encoding->code & _GENERATED_EVENT_FLAG ? "(generated) " : "",
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", name_width, ws.equals,
            _formatInteger( encoding->code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "sequence number", name_width, ws.equals,
            _formatInteger( encoding->sequence_number ), ws.separator ),
        ws.memb_indent, "request", name_width, ws.equals,
        _formatInteger( encoding->request, MappingNotify::request_names ), ws.separator,
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
    assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    // event codes with msb turned on are generated by request SendEvent
    const uint8_t code ( *data & _EVENT_CODE_MASK );
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
    assert( event.bytes_parsed == protocol::events::ENCODING_SZ );
    return event;
}

// logging of events implemented as wrapper around _parseEvent to accommodate
//    logging of both normal events and those encoded by SendEvent
size_t X11ProtocolParser::_logEvent(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    _ParsingOutputs event {
        _parseEvent( conn, data, sz, _ROOT_WS ) };
    const protocol::events::Header* header {
        reinterpret_cast< const protocol::events::Header* >( data ) };
    fmt::println( settings.log_fs,
                  "C{:03d}:{:04d}B:{}:S{:05d}: Event {}",
                  conn->id, event.bytes_parsed, _SERVER_TO_CLIENT,
                  header->sequence_number, event.str );
    assert( event.bytes_parsed == protocol::events::ENCODING_SZ );
    return event.bytes_parsed;
}
