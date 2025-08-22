#include <string_view>

#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/events.hpp"


template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::KeyPress >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( KeyPress::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::KeyPress;
    const KeyPress::Encoding* encoding {
        reinterpret_cast< const KeyPress::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( KeyPress::Encoding );
    assert( encoding->code == protocol::events::codes::KEYPRESS );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        memb_indent, "detail", name_width, _equals,
        _formatCommonType( encoding->detail ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time ), _separator,
        memb_indent, "root", name_width, _equals,
        _formatCommonType( encoding->root ), _separator,
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "child", name_width, _equals,
        _formatCommonType( encoding->child, KeyPress::child_names ), _separator,
        memb_indent, "root-x", name_width, _equals,
        _formatInteger( encoding->root_x ), _separator,
        memb_indent, "root-y", name_width, _equals,
        _formatInteger( encoding->root_y ), _separator,
        memb_indent, "event-x", name_width, _equals,
        _formatInteger( encoding->event_x ), _separator,
        memb_indent, "event-y", name_width, _equals,
        _formatInteger( encoding->event_y ), _separator,
        memb_indent, "state", name_width, _equals,
        _formatCommonType( encoding->state ), _separator,
        memb_indent, "same-screen", name_width, _equals,
        _formatCommonType( encoding->same_screen ), _separator,
        struct_indent
        );
    return outputs;
}

// TBD can we have a shared specialization given that the encoding is the same as
//   KeyPress?
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::KeyRelease >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( KeyRelease::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::KeyRelease;
    const KeyRelease::Encoding* encoding {
        reinterpret_cast< const KeyRelease::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( KeyRelease::Encoding );
    assert( encoding->code == protocol::events::codes::KEYRELEASE );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        memb_indent, "detail", name_width, _equals,
        _formatCommonType( encoding->detail ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time ), _separator,
        memb_indent, "root", name_width, _equals,
        _formatCommonType( encoding->root ), _separator,
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "child", name_width, _equals,
        _formatCommonType( encoding->child, KeyRelease::child_names ), _separator,
        memb_indent, "root-x", name_width, _equals,
        _formatInteger( encoding->root_x ), _separator,
        memb_indent, "root-y", name_width, _equals,
        _formatInteger( encoding->root_y ), _separator,
        memb_indent, "event-x", name_width, _equals,
        _formatInteger( encoding->event_x ), _separator,
        memb_indent, "event-y", name_width, _equals,
        _formatInteger( encoding->event_y ), _separator,
        memb_indent, "state", name_width, _equals,
        _formatCommonType( encoding->state ), _separator,
        memb_indent, "same-screen", name_width, _equals,
        _formatCommonType( encoding->same_screen ), _separator,
        struct_indent
        );
    return outputs;
}

// TBD only differs from KeyPress/KeyRelease encoding by type of `detail`
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ButtonPress >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( ButtonPress::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::ButtonPress;
    const ButtonPress::Encoding* encoding {
        reinterpret_cast< const ButtonPress::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ButtonPress::Encoding );
    assert( encoding->code == protocol::events::codes::BUTTONPRESS );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        memb_indent, "detail", name_width, _equals,
        _formatCommonType( encoding->detail ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time ), _separator,
        memb_indent, "root", name_width, _equals,
        _formatCommonType( encoding->root ), _separator,
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "child", name_width, _equals,
        _formatCommonType( encoding->child, ButtonPress::child_names ), _separator,
        memb_indent, "root-x", name_width, _equals,
        _formatInteger( encoding->root_x ), _separator,
        memb_indent, "root-y", name_width, _equals,
        _formatInteger( encoding->root_y ), _separator,
        memb_indent, "event-x", name_width, _equals,
        _formatInteger( encoding->event_x ), _separator,
        memb_indent, "event-y", name_width, _equals,
        _formatInteger( encoding->event_y ), _separator,
        memb_indent, "state", name_width, _equals,
        _formatCommonType( encoding->state ), _separator,
        memb_indent, "same-screen", name_width, _equals,
        _formatCommonType( encoding->same_screen ), _separator,
        struct_indent
        );
    return outputs;
}

// TBD can we have a shared specialization given that the encoding is the same as
//   ButtonPress?
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ButtonRelease >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( ButtonRelease::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::ButtonRelease;
    const ButtonRelease::Encoding* encoding {
        reinterpret_cast< const ButtonRelease::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ButtonRelease::Encoding );
    assert( encoding->code == protocol::events::codes::BUTTONRELEASE );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        memb_indent, "detail", name_width, _equals,
        _formatCommonType( encoding->detail ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time ), _separator,
        memb_indent, "root", name_width, _equals,
        _formatCommonType( encoding->root ), _separator,
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "child", name_width, _equals,
        _formatCommonType( encoding->child, ButtonRelease::child_names ), _separator,
        memb_indent, "root-x", name_width, _equals,
        _formatInteger( encoding->root_x ), _separator,
        memb_indent, "root-y", name_width, _equals,
        _formatInteger( encoding->root_y ), _separator,
        memb_indent, "event-x", name_width, _equals,
        _formatInteger( encoding->event_x ), _separator,
        memb_indent, "event-y", name_width, _equals,
        _formatInteger( encoding->event_y ), _separator,
        memb_indent, "state", name_width, _equals,
        _formatCommonType( encoding->state ), _separator,
        memb_indent, "same-screen", name_width, _equals,
        _formatCommonType( encoding->same_screen ), _separator,
        struct_indent
        );
    return outputs;
}

// TBD only differs from KeyPress/KeyRelease/ButtonPress/ButtonRelease encoding by type of `detail`
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MotionNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( MotionNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::MotionNotify;
    const MotionNotify::Encoding* encoding {
        reinterpret_cast< const MotionNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MotionNotify::Encoding );
    assert( encoding->code == protocol::events::codes::MOTIONNOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        memb_indent, "detail", name_width, _equals,
        _formatInteger( encoding->detail, MotionNotify::detail_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time ), _separator,
        memb_indent, "root", name_width, _equals,
        _formatCommonType( encoding->root ), _separator,
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "child", name_width, _equals,
        _formatCommonType( encoding->child, MotionNotify::child_names ), _separator,
        memb_indent, "root-x", name_width, _equals,
        _formatInteger( encoding->root_x ), _separator,
        memb_indent, "root-y", name_width, _equals,
        _formatInteger( encoding->root_y ), _separator,
        memb_indent, "event-x", name_width, _equals,
        _formatInteger( encoding->event_x ), _separator,
        memb_indent, "event-y", name_width, _equals,
        _formatInteger( encoding->event_y ), _separator,
        memb_indent, "state", name_width, _equals,
        _formatCommonType( encoding->state ), _separator,
        memb_indent, "same-screen", name_width, _equals,
        _formatCommonType( encoding->same_screen ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::EnterNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( EnterNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::EnterNotify;
    const EnterNotify::Encoding* encoding {
        reinterpret_cast< const EnterNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( EnterNotify::Encoding );
    assert( encoding->code == protocol::events::codes::ENTERNOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "same-screen/focus" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        // TBD how to indicate max enum without magic number
        memb_indent, "detail", name_width, _equals,
        _formatInteger( encoding->detail, EnterNotify::detail_names, 4 ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time ), _separator,
        memb_indent, "root", name_width, _equals,
        _formatCommonType( encoding->root ), _separator,
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "child", name_width, _equals,
        _formatCommonType( encoding->child, EnterNotify::child_names ), _separator,
        memb_indent, "root-x", name_width, _equals,
        _formatInteger( encoding->root_x ), _separator,
        memb_indent, "root-y", name_width, _equals,
        _formatInteger( encoding->root_y ), _separator,
        memb_indent, "event-x", name_width, _equals,
        _formatInteger( encoding->event_x ), _separator,
        memb_indent, "event-y", name_width, _equals,
        _formatInteger( encoding->event_y ), _separator,
        memb_indent, "state", name_width, _equals,
        _formatCommonType( encoding->state ), _separator,
        // TBD how to indicate max enum without magic number
        memb_indent, "mode", name_width, _equals,
        _formatInteger( encoding->mode, EnterNotify::mode_names, 2 ), _separator,
        memb_indent, "same-screen/focus", name_width, _equals,
        _formatBitmask( encoding->focus_same_screen, EnterNotify::focus_same_screen_names ), _separator,
        struct_indent
        );
    return outputs;
}

// TBD shared encoding with EnterNotify
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::LeaveNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( LeaveNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::LeaveNotify;
    const LeaveNotify::Encoding* encoding {
        reinterpret_cast< const LeaveNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( LeaveNotify::Encoding );
    assert( encoding->code == protocol::events::codes::LEAVENOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "same-screen/focus" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        // TBD how to indicate max enum without magic number
        memb_indent, "detail", name_width, _equals,
        _formatInteger( encoding->detail, LeaveNotify::detail_names, 4 ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time ), _separator,
        memb_indent, "root", name_width, _equals,
        _formatCommonType( encoding->root ), _separator,
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "child", name_width, _equals,
        _formatCommonType( encoding->child, LeaveNotify::child_names ), _separator,
        memb_indent, "root-x", name_width, _equals,
        _formatInteger( encoding->root_x ), _separator,
        memb_indent, "root-y", name_width, _equals,
        _formatInteger( encoding->root_y ), _separator,
        memb_indent, "event-x", name_width, _equals,
        _formatInteger( encoding->event_x ), _separator,
        memb_indent, "event-y", name_width, _equals,
        _formatInteger( encoding->event_y ), _separator,
        memb_indent, "state", name_width, _equals,
        _formatCommonType( encoding->state ), _separator,
        // TBD how to indicate max enum without magic number
        memb_indent, "mode", name_width, _equals,
        _formatInteger( encoding->mode, LeaveNotify::mode_names, 2 ), _separator,
        memb_indent, "same-screen/focus", name_width, _equals,
        _formatBitmask( encoding->focus_same_screen, LeaveNotify::focus_same_screen_names ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::FocusIn >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( LeaveNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::FocusIn;
    const FocusIn::Encoding* encoding {
        reinterpret_cast< const FocusIn::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( FocusIn::Encoding );
    assert( encoding->code == protocol::events::codes::FOCUSIN );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "same-screen/focus" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        // TBD how to indicate max enum without magic number
        memb_indent, "detail", name_width, _equals,
        _formatInteger( encoding->detail, FocusIn::detail_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "mode", name_width, _equals,
        _formatInteger( encoding->mode, FocusIn::mode_names ), _separator,
        struct_indent
        );
    return outputs;
}

// TBD shared encoding with FocusIn
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::FocusOut >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( LeaveNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::FocusOut;
    const FocusOut::Encoding* encoding {
        reinterpret_cast< const FocusOut::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( FocusOut::Encoding );
    assert( encoding->code == protocol::events::codes::FOCUSOUT );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "same-screen/focus" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        // TBD how to indicate max enum without magic number
        memb_indent, "detail", name_width, _equals,
        _formatInteger( encoding->detail, FocusOut::detail_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "mode", name_width, _equals,
        _formatInteger( encoding->mode, FocusOut::mode_names ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::KeymapNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( LeaveNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::KeymapNotify;
    const KeymapNotify::Encoding* encoding {
        reinterpret_cast< const KeymapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( KeymapNotify::Encoding );
    assert( encoding->code == protocol::events::codes::KEYMAPNOTIFY );

    // TBD can we develop a way of printing the keyboard state instead of bytes?
    _ParsingOutputs keys { _parseLISTofCARD8(
            encoding->keys, sizeof( encoding->keys ) ) };

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "keys(0-7 omitted)" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        // TBD can we make a way to log the keyboard state from this map?
        memb_indent, "keys(0-7 omitted)", name_width, _equals,
        keys.str, _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::Expose >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( Expose::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::Expose;
    const Expose::Encoding* encoding {
        reinterpret_cast< const Expose::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( Expose::Encoding );
    assert( encoding->code == protocol::events::codes::EXPOSE );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "x", name_width, _equals,
        _formatInteger( encoding->x ), _separator,
        memb_indent, "y", name_width, _equals,
        _formatInteger( encoding->y ), _separator,
        memb_indent, "width", name_width, _equals,
        _formatInteger( encoding->width ), _separator,
        memb_indent, "height", name_width, _equals,
        _formatInteger( encoding->height ), _separator,
        memb_indent, "count", name_width, _equals,
        _formatInteger( encoding->count ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::GraphicsExposure >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( GraphicsExposure::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::GraphicsExposure;
    const GraphicsExposure::Encoding* encoding {
        reinterpret_cast< const GraphicsExposure::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( GraphicsExposure::Encoding );
    assert( encoding->code == protocol::events::codes::GRAPHICSEXPOSURE );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        memb_indent, "x", name_width, _equals,
        _formatInteger( encoding->x ), _separator,
        memb_indent, "y", name_width, _equals,
        _formatInteger( encoding->y ), _separator,
        memb_indent, "width", name_width, _equals,
        _formatInteger( encoding->width ), _separator,
        memb_indent, "height", name_width, _equals,
        _formatInteger( encoding->height ), _separator,
        memb_indent, "minor-opcode", name_width, _equals,
        _formatInteger( encoding->minor_opcode ), _separator,
        memb_indent, "count", name_width, _equals,
        _formatInteger( encoding->count ), _separator,
        // TBD breakout reuqest printing by opcode "RequestName(opcode)" in separate func
        memb_indent, "major-opcode", name_width, _equals,
        _formatInteger( encoding->major_opcode ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::NoExposure >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( NoExposure::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::NoExposure;
    const NoExposure::Encoding* encoding {
        reinterpret_cast< const NoExposure::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( NoExposure::Encoding );
    assert( encoding->code == protocol::events::codes::NOEXPOSURE );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        memb_indent, "minor-opcode", name_width, _equals,
        _formatInteger( encoding->minor_opcode ), _separator,
        // TBD breakout reuqest printing by opcode "RequestName(opcode)" in separate func
        memb_indent, "major-opcode", name_width, _equals,
        _formatInteger( encoding->major_opcode ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::VisibilityNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( VisibilityNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::VisibilityNotify;
    const VisibilityNotify::Encoding* encoding {
        reinterpret_cast< const VisibilityNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( VisibilityNotify::Encoding );
    assert( encoding->code == protocol::events::codes::VISIBILITYNOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "state", name_width, _equals,
        _formatInteger( encoding->state, VisibilityNotify::state_names ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::CreateNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( CreateNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::CreateNotify;
    const CreateNotify::Encoding* encoding {
        reinterpret_cast< const CreateNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CreateNotify::Encoding );
    assert( encoding->code == protocol::events::codes::CREATENOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "override-redirect" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "parent", name_width, _equals,
        _formatCommonType( encoding->parent ), _separator,
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "x", name_width, _equals,
        _formatInteger( encoding->x ), _separator,
        memb_indent, "y", name_width, _equals,
        _formatInteger( encoding->y ), _separator,
        memb_indent, "width", name_width, _equals,
        _formatInteger( encoding->width ), _separator,
        memb_indent, "height", name_width, _equals,
        _formatInteger( encoding->height ), _separator,
        memb_indent, "border-width", name_width, _equals,
        _formatInteger( encoding->border_width ), _separator,
        memb_indent, "override-redirect", name_width, _equals,
        _formatCommonType( encoding->override_redirect ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::DestroyNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( DestroyNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::DestroyNotify;
    const DestroyNotify::Encoding* encoding {
        reinterpret_cast< const DestroyNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( DestroyNotify::Encoding );
    assert( encoding->code == protocol::events::codes::DESTROYNOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::UnmapNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( UnmapNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::UnmapNotify;
    const UnmapNotify::Encoding* encoding {
        reinterpret_cast< const UnmapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( UnmapNotify::Encoding );
    assert( encoding->code == protocol::events::codes::UNMAPNOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "from-configure", name_width, _equals,
        _formatCommonType( encoding->from_configure ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MapNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( MapNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::MapNotify;
    const MapNotify::Encoding* encoding {
        reinterpret_cast< const MapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MapNotify::Encoding );
    assert( encoding->code == protocol::events::codes::MAPNOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "override-redirect" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "override-redirect", name_width, _equals,
        _formatCommonType( encoding->override_redirect ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MapRequest >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( MapRequest::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::MapRequest;
    const MapRequest::Encoding* encoding {
        reinterpret_cast< const MapRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MapRequest::Encoding );
    assert( encoding->code == protocol::events::codes::MAPREQUEST );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "parent", name_width, _equals,
        _formatCommonType( encoding->parent ), _separator,
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ReparentNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( ReparentNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::ReparentNotify;
    const ReparentNotify::Encoding* encoding {
        reinterpret_cast< const ReparentNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ReparentNotify::Encoding );
    assert( encoding->code == protocol::events::codes::REPARENTNOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "override-redirect" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "parent", name_width, _equals,
        _formatCommonType( encoding->parent ), _separator,
        memb_indent, "x", name_width, _equals,
        _formatInteger( encoding->x ), _separator,
        memb_indent, "y", name_width, _equals,
        _formatInteger( encoding->y ), _separator,
        memb_indent, "override-redirect", name_width, _equals,
        _formatCommonType( encoding->override_redirect ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ConfigureNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( ConfigureNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::ConfigureNotify;
    const ConfigureNotify::Encoding* encoding {
        reinterpret_cast< const ConfigureNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ConfigureNotify::Encoding );
    assert( encoding->code == protocol::events::codes::CONFIGURENOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "override-redirect" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "above-sibling", name_width, _equals,
        _formatCommonType( encoding->above_sibling, ConfigureNotify::above_sibling_names ), _separator,
        memb_indent, "x", name_width, _equals,
        _formatInteger( encoding->x ), _separator,
        memb_indent, "y", name_width, _equals,
        _formatInteger( encoding->y ), _separator,
        memb_indent, "width", name_width, _equals,
        _formatInteger( encoding->width ), _separator,
        memb_indent, "height", name_width, _equals,
        _formatInteger( encoding->height ), _separator,
        memb_indent, "border-width", name_width, _equals,
        _formatInteger( encoding->border_width ), _separator,
        memb_indent, "override-redirect", name_width, _equals,
        _formatCommonType( encoding->override_redirect ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ConfigureRequest >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( ConfigureRequest::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::ConfigureRequest;
    const ConfigureRequest::Encoding* encoding {
        reinterpret_cast< const ConfigureRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ConfigureRequest::Encoding );
    assert( encoding->code == protocol::events::codes::CONFIGUREREQUEST );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        memb_indent, "stack-mode", name_width, _equals,
        _formatInteger( encoding->stack_mode, ConfigureRequest::stack_mode_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "parent", name_width, _equals,
        _formatCommonType( encoding->parent ), _separator,
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "sibling", name_width, _equals,
        _formatCommonType( encoding->sibling, ConfigureRequest::sibling_names ), _separator,
        memb_indent, "x", name_width, _equals,
        _formatInteger( encoding->x ), _separator,
        memb_indent, "y", name_width, _equals,
        _formatInteger( encoding->y ), _separator,
        memb_indent, "width", name_width, _equals,
        _formatInteger( encoding->width ), _separator,
        memb_indent, "height", name_width, _equals,
        _formatInteger( encoding->height ), _separator,
        memb_indent, "border-width", name_width, _equals,
        _formatInteger( encoding->border_width ), _separator,
        memb_indent, "value-mask", name_width, _equals,
        _formatBitmask( encoding->value_mask, ConfigureRequest::value_names ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::GravityNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( GravityNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::GravityNotify;
    const GravityNotify::Encoding* encoding {
        reinterpret_cast< const GravityNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( GravityNotify::Encoding );
    assert( encoding->code == protocol::events::codes::GRAVITYNOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "x", name_width, _equals,
        _formatInteger( encoding->x ), _separator,
        memb_indent, "y", name_width, _equals,
        _formatInteger( encoding->y ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ResizeRequest >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( ResizeRequest::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::ResizeRequest;
    const ResizeRequest::Encoding* encoding {
        reinterpret_cast< const ResizeRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ResizeRequest::Encoding );
    assert( encoding->code == protocol::events::codes::RESIZEREQUEST );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "width", name_width, _equals,
        _formatInteger( encoding->width ), _separator,
        memb_indent, "height", name_width, _equals,
        _formatInteger( encoding->height ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::CirculateNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( CirculateNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::CirculateNotify;
    const CirculateNotify::Encoding* encoding {
        reinterpret_cast< const CirculateNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CirculateNotify::Encoding );
    assert( encoding->code == protocol::events::codes::CIRCULATENOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "event", name_width, _equals,
        _formatCommonType( encoding->event ), _separator,
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "place", name_width, _equals,
        _formatInteger( encoding->place, CirculateNotify::place_names ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::CirculateRequest >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( CirculateRequest::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::CirculateRequest;
    const CirculateRequest::Encoding* encoding {
        reinterpret_cast< const CirculateRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CirculateRequest::Encoding );
    assert( encoding->code == protocol::events::codes::CIRCULATEREQUEST );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "parent", name_width, _equals,
        _formatCommonType( encoding->parent ), _separator,
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "place", name_width, _equals,
        _formatInteger( encoding->place, CirculateRequest::place_names ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::PropertyNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( PropertyNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::PropertyNotify;
    const PropertyNotify::Encoding* encoding {
        reinterpret_cast< const PropertyNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( PropertyNotify::Encoding );
    assert( encoding->code == protocol::events::codes::PROPERTYNOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "atom", name_width, _equals,
        _formatCommonType( encoding->atom ), _separator,
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time ), _separator,
        memb_indent, "state", name_width, _equals,
        _formatInteger( encoding->state, PropertyNotify::state_names ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::SelectionClear >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( SelectionClear::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::SelectionClear;
    const SelectionClear::Encoding* encoding {
        reinterpret_cast< const SelectionClear::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionClear::Encoding );
    assert( encoding->code == protocol::events::codes::SELECTIONCLEAR );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time ), _separator,
        memb_indent, "owner", name_width, _equals,
        _formatCommonType( encoding->owner ), _separator,
        memb_indent, "selection", name_width, _equals,
        _formatCommonType( encoding->selection ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::SelectionRequest >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( SelectionRequest::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::SelectionRequest;
    const SelectionRequest::Encoding* encoding {
        reinterpret_cast< const SelectionRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionRequest::Encoding );
    assert( encoding->code == protocol::events::codes::SELECTIONREQUEST );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time, SelectionRequest::time_names ), _separator,
        memb_indent, "owner", name_width, _equals,
        _formatCommonType( encoding->owner ), _separator,
        memb_indent, "requestor", name_width, _equals,
        _formatCommonType( encoding->requestor ), _separator,
        memb_indent, "selection", name_width, _equals,
        _formatCommonType( encoding->selection ), _separator,
        memb_indent, "target", name_width, _equals,
        _formatCommonType( encoding->target ), _separator,
        memb_indent, "property", name_width, _equals,
        _formatCommonType( encoding->property, SelectionRequest::property_names ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::SelectionNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( SelectionNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::SelectionNotify;
    const SelectionNotify::Encoding* encoding {
        reinterpret_cast< const SelectionNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionNotify::Encoding );
    assert( encoding->code == protocol::events::codes::SELECTIONNOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time, SelectionNotify::time_names ), _separator,
        memb_indent, "requestor", name_width, _equals,
        _formatCommonType( encoding->requestor ), _separator,
        memb_indent, "selection", name_width, _equals,
        _formatCommonType( encoding->selection ), _separator,
        memb_indent, "target", name_width, _equals,
        _formatCommonType( encoding->target ), _separator,
        memb_indent, "property", name_width, _equals,
        _formatCommonType( encoding->property, SelectionNotify::property_names ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ColormapNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( ColormapNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::ColormapNotify;
    const ColormapNotify::Encoding* encoding {
        reinterpret_cast< const ColormapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ColormapNotify::Encoding );
    assert( encoding->code == protocol::events::codes::COLORMAPNOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "colormap", name_width, _equals,
        _formatCommonType( encoding->colormap, ColormapNotify::colormap_names ), _separator,
        memb_indent, "new", name_width, _equals,
        _formatCommonType( encoding->new_ ), _separator,
        memb_indent, "state", name_width, _equals,
        _formatInteger( encoding->state, ColormapNotify::state_names ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::ClientMessage >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( ClientMessage::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::ClientMessage;
    const ClientMessage::Encoding* encoding {
        reinterpret_cast< const ClientMessage::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ClientMessage::Encoding );
    assert( encoding->code == protocol::events::codes::CLIENTMESSAGE );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        memb_indent, "format", name_width, _equals,
        _formatInteger( encoding->format ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "type", name_width, _equals,
        _formatCommonType( encoding->type ), _separator,
        struct_indent
        );
    return outputs;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent< protocol::events::MappingNotify >(
    Connection* conn, const uint8_t* data,
    const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
//    assert( sz >= sizeof( MappingNotify::Encoding ) ); // TBD

    _ParsingOutputs outputs {};
    using protocol::events::MappingNotify;
    const MappingNotify::Encoding* encoding {
        reinterpret_cast< const MappingNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MappingNotify::Encoding );
    assert( encoding->code == protocol::events::codes::MAPPINGNOTIFY );

    const uint32_t tab_ct (
        ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    outputs.str = fmt::format(
        "{}({}) {{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        protocol::events::names[encoding->code], encoding->code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        memb_indent, "request", name_width, _equals,
        _formatInteger( encoding->request, MappingNotify::request_names ), _separator,
        memb_indent, "first-keycode", name_width, _equals,
        _formatCommonType( encoding->first_keycode ), _separator,
        memb_indent, "count", name_width, _equals,
        _formatInteger( encoding->count ), _separator,
        struct_indent
        );
    return outputs;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseEvent(
    Connection* conn, const uint8_t* data, const size_t sz,
    const _EventFormat format /*= _EventFormat::NORMAL*/ ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    const uint8_t code { *data };
    assert( code >= protocol::events::codes::MIN &&
            code <= protocol::events::codes::MAX );
    _ParsingOutputs event {};
    switch ( code ) {
    case protocol::events::codes::KEYPRESS:          //  2
        event = _parseEvent< protocol::events::KeyPress >(
            conn, data, format );
        break;
    case protocol::events::codes::KEYRELEASE:        //  3
        event = _parseEvent< protocol::events::KeyRelease >(
            conn, data, format );
        break;
    case protocol::events::codes::BUTTONPRESS:       //  4
        event = _parseEvent< protocol::events::ButtonPress >(
            conn, data, format );
        break;
    case protocol::events::codes::BUTTONRELEASE:     //  5
        event = _parseEvent< protocol::events::ButtonRelease >(
            conn, data, format );
        break;
    case protocol::events::codes::MOTIONNOTIFY:      //  6
        event = _parseEvent< protocol::events::MotionNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::ENTERNOTIFY:       //  7
        event = _parseEvent< protocol::events::EnterNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::LEAVENOTIFY:       //  8
        event = _parseEvent< protocol::events::LeaveNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::FOCUSIN:           //  9
        event = _parseEvent< protocol::events::FocusIn >(
            conn, data, format );
        break;
    case protocol::events::codes::FOCUSOUT:          // 10
        event = _parseEvent< protocol::events::FocusOut >(
            conn, data, format );
        break;
    case protocol::events::codes::KEYMAPNOTIFY:      // 11
        event = _parseEvent< protocol::events::KeymapNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::EXPOSE:            // 12
        event = _parseEvent< protocol::events::Expose >(
            conn, data, format );
        break;
    case protocol::events::codes::GRAPHICSEXPOSURE:  // 13
        event = _parseEvent< protocol::events::GraphicsExposure >(
            conn, data, format );
        break;
    case protocol::events::codes::NOEXPOSURE:        // 14
        event = _parseEvent< protocol::events::NoExposure >(
            conn, data, format );
        break;
    case protocol::events::codes::VISIBILITYNOTIFY:  // 15
        event = _parseEvent< protocol::events::VisibilityNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::CREATENOTIFY:      // 16
        event = _parseEvent< protocol::events::CreateNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::DESTROYNOTIFY:     // 17
        event = _parseEvent< protocol::events::DestroyNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::UNMAPNOTIFY:       // 18
        event = _parseEvent< protocol::events::UnmapNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::MAPNOTIFY:         // 19
        event = _parseEvent< protocol::events::MapNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::MAPREQUEST:        // 20
        event = _parseEvent< protocol::events::MapRequest >(
            conn, data, format );
        break;
    case protocol::events::codes::REPARENTNOTIFY:    // 21
        event = _parseEvent< protocol::events::ReparentNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::CONFIGURENOTIFY:   // 22
        event = _parseEvent< protocol::events::ConfigureNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::CONFIGUREREQUEST:  // 23
        event = _parseEvent< protocol::events::ConfigureRequest >(
            conn, data, format );
        break;
    case protocol::events::codes::GRAVITYNOTIFY:     // 24
        event = _parseEvent< protocol::events::GravityNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::RESIZEREQUEST:     // 25
        event = _parseEvent< protocol::events::ResizeRequest >(
            conn, data, format );
        break;
    case protocol::events::codes::CIRCULATENOTIFY:   // 26
        event = _parseEvent< protocol::events::CirculateNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::CIRCULATEREQUEST:  // 27
        event = _parseEvent< protocol::events::CirculateRequest >(
            conn, data, format );
        break;
    case protocol::events::codes::PROPERTYNOTIFY:    // 28
        event = _parseEvent< protocol::events::PropertyNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::SELECTIONCLEAR:    // 29
        event = _parseEvent< protocol::events::SelectionClear >(
            conn, data, format );
        break;
    case protocol::events::codes::SELECTIONREQUEST:  // 30
        event = _parseEvent< protocol::events::SelectionRequest >(
            conn, data, format );
        break;
    case protocol::events::codes::SELECTIONNOTIFY:   // 31
        event = _parseEvent< protocol::events::SelectionNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::COLORMAPNOTIFY:    // 32
        event = _parseEvent< protocol::events::ColormapNotify >(
            conn, data, format );
        break;
    case protocol::events::codes::CLIENTMESSAGE:     // 33
        event = _parseEvent< protocol::events::ClientMessage >(
            conn, data, format );
        break;
    case protocol::events::codes::MAPPINGNOTIFY:     // 34
        event = _parseEvent< protocol::events::MappingNotify >(
            conn, data, format );
        break;
    default:
        break;
    }
    assert( event.bytes_parsed == protocol::events::ENCODING_SZ );
    return event;
}

// logging of events implemented as wrapper around _parseEvent to accommodate
//    logging of both normal events and those encoded by SendEvent
size_t X11ProtocolParser::_logServerEvent(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    _ParsingOutputs event {
        _parseEvent( conn, data, sz ) };
    fmt::println( _log_fs, "{:03d}:<:server event {}",
                  conn->id, event.str );
    assert( event.bytes_parsed == protocol::events::ENCODING_SZ );
    return event.bytes_parsed;
}
