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
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "detail", memb_name_w, ws.equals,
        _formatProtocolType( encoding->header.detail ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", memb_name_w, ws.equals,
        _formatProtocolType( encoding->child,
                             InputEvent::child_names ), ws.separator,
        ws.memb_indent, "root-x", memb_name_w, ws.equals,
        _formatInteger( encoding->root_x ), ws.separator,
        ws.memb_indent, "root-y", memb_name_w, ws.equals,
        _formatInteger( encoding->root_y ), ws.separator,
        ws.memb_indent, "event-x", memb_name_w, ws.equals,
        _formatInteger( encoding->event_x ), ws.separator,
        ws.memb_indent, "event-y", memb_name_w, ws.equals,
        _formatInteger( encoding->event_y ), ws.separator,
        ws.memb_indent, "state", memb_name_w, ws.equals,
        _formatProtocolType( encoding->state ), ws.separator,
        ws.memb_indent, "same-screen", memb_name_w, ws.equals,
        _formatProtocolType( encoding->same_screen ), ws.separator,
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
    assert( reinterpret_cast< const KeyPress::Header* >(
                data )->code & protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
    assert( reinterpret_cast< const KeyRelease::Header* >(
                data )->code & protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
    assert( reinterpret_cast< const ButtonPress::Header* >(
                data )->code & protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
    assert( reinterpret_cast< const ButtonRelease::Header* >(
                data )->code & protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
    const MotionNotify::Encoding* encoding {
        reinterpret_cast< const MotionNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MotionNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "detail", memb_name_w, ws.equals,
        _formatInteger( encoding->header.detail,
                        MotionNotify::detail_names ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", memb_name_w, ws.equals,
        _formatProtocolType( encoding->child,
                             MotionNotify::child_names ), ws.separator,
        ws.memb_indent, "root-x", memb_name_w, ws.equals,
        _formatInteger( encoding->root_x ), ws.separator,
        ws.memb_indent, "root-y", memb_name_w, ws.equals,
        _formatInteger( encoding->root_y ), ws.separator,
        ws.memb_indent, "event-x", memb_name_w, ws.equals,
        _formatInteger( encoding->event_x ), ws.separator,
        ws.memb_indent, "event-y", memb_name_w, ws.equals,
        _formatInteger( encoding->event_y ), ws.separator,
        ws.memb_indent, "state", memb_name_w, ws.equals,
        _formatProtocolType( encoding->state ), ws.separator,
        ws.memb_indent, "same-screen", memb_name_w, ws.equals,
        _formatProtocolType( encoding->same_screen ), ws.separator,
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
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "detail", memb_name_w, ws.equals,
        _formatInteger( encoding->header.detail, BoundaryEvent::detail_names,
                        _IndexRange{ 0, BoundaryEvent::MAX_DETAIL } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatProtocolType( encoding->root ), ws.separator,
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "child", memb_name_w, ws.equals,
        _formatProtocolType( encoding->child,
                             BoundaryEvent::child_names ), ws.separator,
        ws.memb_indent, "root-x", memb_name_w, ws.equals,
        _formatInteger( encoding->root_x ), ws.separator,
        ws.memb_indent, "root-y", memb_name_w, ws.equals,
        _formatInteger( encoding->root_y ), ws.separator,
        ws.memb_indent, "event-x", memb_name_w, ws.equals,
        _formatInteger( encoding->event_x ), ws.separator,
        ws.memb_indent, "event-y", memb_name_w, ws.equals,
        _formatInteger( encoding->event_y ), ws.separator,
        ws.memb_indent, "state", memb_name_w, ws.equals,
        _formatProtocolType( encoding->state ), ws.separator,
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatInteger( encoding->mode, BoundaryEvent::mode_names,
                        _IndexRange{ 0, BoundaryEvent::MAX_MODE } ), ws.separator,
        ws.memb_indent, "(same-screen/focus mask)", memb_name_w, ws.equals,
        _formatBitmask( encoding->focus_same_screen_mask,
                        BoundaryEvent::focus_same_screen_names ), ws.separator,
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
    assert( reinterpret_cast< const EnterNotify::Header* >(
                data )->code & protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
    assert( reinterpret_cast< const LeaveNotify::Header* >(
                data )->code & protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "detail", memb_name_w, ws.equals,
        _formatInteger( encoding->header.detail,
                        FocusEvent::detail_names ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatInteger( encoding->mode,
                        FocusEvent::mode_names ), ws.separator,
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
    assert( reinterpret_cast< const FocusIn::Header* >(
                data )->code & protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
    assert( reinterpret_cast< const FocusOut::Header* >(
                data )->code & protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
    const KeymapNotify::Encoding* encoding {
        reinterpret_cast< const KeymapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( KeymapNotify::Encoding );
    assert( outputs.bytes_parsed == KeymapNotify::ENCODING_SZ );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
            protocol::events::codes::KEYMAPNOTIFY );
    // TBD can we develop a way of printing the keyboard state instead of bytes?
    const _ParsingOutputs keys {
        _parseLISTof< protocol::CARD8 >(
            encoding->keys, sizeof( encoding->keys ), sizeof( encoding->keys ),
            ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };

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
            _formatInteger( encoding->header.code ), ws.separator ),
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
    const Expose::Encoding* encoding {
        reinterpret_cast< const Expose::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( Expose::Encoding );
    assert( outputs.bytes_parsed == Expose::ENCODING_SZ );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatInteger( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatInteger( encoding->height ), ws.separator,
        ws.memb_indent, "count", memb_name_w, ws.equals,
        _formatInteger( encoding->count ), ws.separator,
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
    const GraphicsExposure::Encoding* encoding {
        reinterpret_cast< const GraphicsExposure::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( GraphicsExposure::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
            protocol::events::codes::GRAPHICSEXPOSURE );
    assert( outputs.bytes_parsed == GraphicsExposure::ENCODING_SZ );
    // TBD lookup request opcodes and assert against encoding opcodes?

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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatProtocolType( encoding->drawable ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatInteger( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatInteger( encoding->height ), ws.separator,
        ws.memb_indent, "minor-opcode", memb_name_w, ws.equals,
        _formatInteger( encoding->minor_opcode ), ws.separator,
        ws.memb_indent, "count", memb_name_w, ws.equals,
        _formatInteger( encoding->count ), ws.separator,
        ws.memb_indent, "major-opcode", memb_name_w, ws.equals,
        _formatInteger( encoding->major_opcode ), ws.separator,
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
    const NoExposure::Encoding* encoding {
        reinterpret_cast< const NoExposure::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( NoExposure::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
            protocol::events::codes::NOEXPOSURE );
    assert( outputs.bytes_parsed == NoExposure::ENCODING_SZ );
    // TBD lookup request opcodes and assert against encoding opcodes?

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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatProtocolType( encoding->drawable ), ws.separator,
        ws.memb_indent, "minor-opcode", memb_name_w, ws.equals,
        _formatInteger( encoding->minor_opcode ), ws.separator,
        ws.memb_indent, "major-opcode", memb_name_w, ws.equals,
        _formatInteger( encoding->major_opcode ), ws.separator,
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
    const VisibilityNotify::Encoding* encoding {
        reinterpret_cast< const VisibilityNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( VisibilityNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "state", memb_name_w, ws.equals,
        _formatInteger( encoding->state,
                        VisibilityNotify::state_names ), ws.separator,
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
    const CreateNotify::Encoding* encoding {
        reinterpret_cast< const CreateNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CreateNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatProtocolType( encoding->parent ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatInteger( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatInteger( encoding->height ), ws.separator,
        ws.memb_indent, "border-width", memb_name_w, ws.equals,
        _formatInteger( encoding->border_width ), ws.separator,
        ws.memb_indent, "override-redirect", memb_name_w, ws.equals,
        _formatProtocolType( encoding->override_redirect ), ws.separator,
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
    const DestroyNotify::Encoding* encoding {
        reinterpret_cast< const DestroyNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( DestroyNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
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
    const UnmapNotify::Encoding* encoding {
        reinterpret_cast< const UnmapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( UnmapNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "from-configure", memb_name_w, ws.equals,
        _formatProtocolType( encoding->from_configure ), ws.separator,
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
    const MapNotify::Encoding* encoding {
        reinterpret_cast< const MapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MapNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "override-redirect", memb_name_w, ws.equals,
        _formatProtocolType( encoding->override_redirect ), ws.separator,
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
    const MapRequest::Encoding* encoding {
        reinterpret_cast< const MapRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MapRequest::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatProtocolType( encoding->parent ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
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
    const ReparentNotify::Encoding* encoding {
        reinterpret_cast< const ReparentNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ReparentNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatProtocolType( encoding->parent ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.memb_indent, "override-redirect", memb_name_w, ws.equals,
        _formatProtocolType( encoding->override_redirect ), ws.separator,
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
    const ConfigureNotify::Encoding* encoding {
        reinterpret_cast< const ConfigureNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ConfigureNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "above-sibling", memb_name_w, ws.equals,
        _formatProtocolType( encoding->above_sibling,
                             ConfigureNotify::above_sibling_names ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatInteger( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatInteger( encoding->height ), ws.separator,
        ws.memb_indent, "border-width", memb_name_w, ws.equals,
        _formatInteger( encoding->border_width ), ws.separator,
        ws.memb_indent, "override-redirect", memb_name_w, ws.equals,
        _formatProtocolType( encoding->override_redirect ), ws.separator,
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
    assert( sz >= sizeof( ConfigureRequest::Encoding ) );
    const ConfigureRequest::Encoding* encoding {
        reinterpret_cast< const ConfigureRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ConfigureRequest::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "stack-mode", memb_name_w, ws.equals,
        _formatInteger( encoding->header.stack_mode,
                        ConfigureRequest::stack_mode_names ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatProtocolType( encoding->parent ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "sibling", memb_name_w, ws.equals,
        _formatProtocolType( encoding->sibling,
                             ConfigureRequest::sibling_names ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatInteger( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatInteger( encoding->height ), ws.separator,
        ws.memb_indent, "border-width", memb_name_w, ws.equals,
        _formatInteger( encoding->border_width ), ws.separator,
        ws.memb_indent, "value-mask", memb_name_w, ws.equals,
        _formatBitmask( encoding->value_mask,
                        ConfigureRequest::value_names ), ws.separator,
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
    const GravityNotify::Encoding* encoding {
        reinterpret_cast< const GravityNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( GravityNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatInteger( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatInteger( encoding->y ), ws.separator,
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
    const ResizeRequest::Encoding* encoding {
        reinterpret_cast< const ResizeRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ResizeRequest::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatInteger( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatInteger( encoding->height ), ws.separator,
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
    const CirculateNotify::Encoding* encoding {
        reinterpret_cast< const CirculateNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CirculateNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "event", memb_name_w, ws.equals,
        _formatProtocolType( encoding->event ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "place", memb_name_w, ws.equals,
        _formatInteger( encoding->place, CirculateNotify::place_names ), ws.separator,
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
    const CirculateRequest::Encoding* encoding {
        reinterpret_cast< const CirculateRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( CirculateRequest::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatProtocolType( encoding->parent ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "place", memb_name_w, ws.equals,
        _formatInteger( encoding->place, CirculateRequest::place_names ), ws.separator,
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
    const PropertyNotify::Encoding* encoding {
        reinterpret_cast< const PropertyNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( PropertyNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "atom", memb_name_w, ws.equals,
        _formatProtocolType( encoding->atom ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "state", memb_name_w, ws.equals,
        _formatInteger( encoding->state,
                        PropertyNotify::state_names ), ws.separator,
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
    const SelectionClear::Encoding* encoding {
        reinterpret_cast< const SelectionClear::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionClear::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatProtocolType( encoding->time ), ws.separator,
        ws.memb_indent, "owner", memb_name_w, ws.equals,
        _formatProtocolType( encoding->owner ), ws.separator,
        ws.memb_indent, "selection", memb_name_w, ws.equals,
        _formatProtocolType( encoding->selection ), ws.separator,
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
    const SelectionRequest::Encoding* encoding {
        reinterpret_cast< const SelectionRequest::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionRequest::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatProtocolType( encoding->time,
                             SelectionRequest::time_names ), ws.separator,
        ws.memb_indent, "owner", memb_name_w, ws.equals,
        _formatProtocolType( encoding->owner ), ws.separator,
        ws.memb_indent, "requestor", memb_name_w, ws.equals,
        _formatProtocolType( encoding->requestor ), ws.separator,
        ws.memb_indent, "selection", memb_name_w, ws.equals,
        _formatProtocolType( encoding->selection ), ws.separator,
        ws.memb_indent, "target", memb_name_w, ws.equals,
        _formatProtocolType( encoding->target ), ws.separator,
        ws.memb_indent, "property", memb_name_w, ws.equals,
        _formatProtocolType( encoding->property,
                             SelectionRequest::property_names ), ws.separator,
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
    const SelectionNotify::Encoding* encoding {
        reinterpret_cast< const SelectionNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( SelectionNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatProtocolType( encoding->time,
                             SelectionNotify::time_names ), ws.separator,
        ws.memb_indent, "requestor", memb_name_w, ws.equals,
        _formatProtocolType( encoding->requestor ), ws.separator,
        ws.memb_indent, "selection", memb_name_w, ws.equals,
        _formatProtocolType( encoding->selection ), ws.separator,
        ws.memb_indent, "target", memb_name_w, ws.equals,
        _formatProtocolType( encoding->target ), ws.separator,
        ws.memb_indent, "property", memb_name_w, ws.equals,
        _formatProtocolType( encoding->property,
                             SelectionNotify::property_names ), ws.separator,
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
    const ColormapNotify::Encoding* encoding {
        reinterpret_cast< const ColormapNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ColormapNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "colormap", memb_name_w, ws.equals,
        _formatProtocolType( encoding->colormap,
                             ColormapNotify::colormap_names ), ws.separator,
        ws.memb_indent, "new", memb_name_w, ws.equals,
        _formatProtocolType( encoding->new_ ), ws.separator,
        ws.memb_indent, "state", memb_name_w, ws.equals,
        _formatInteger( encoding->state,
                        ColormapNotify::state_names ), ws.separator,
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
    const ClientMessage::Encoding* encoding {
        reinterpret_cast< const ClientMessage::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( ClientMessage::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        ws.memb_indent, "format", memb_name_w, ws.equals,
        _formatInteger( encoding->header.format ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatProtocolType( encoding->window ), ws.separator,
        ws.memb_indent, "type", memb_name_w, ws.equals,
        _formatProtocolType( encoding->type ), ws.separator,
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
    const MappingNotify::Encoding* encoding {
        reinterpret_cast< const MappingNotify::Encoding* >( data ) };
    outputs.bytes_parsed += sizeof( MappingNotify::Encoding );
    assert( encoding->header.code &
            protocol::requests::SendEvent::EVENT_CODE_MASK ==
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
            _formatInteger( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatInteger( encoding->header.sequence_num ), ws.separator ),
        ws.memb_indent, "request", memb_name_w, ws.equals,
        _formatInteger( encoding->request,
                        MappingNotify::request_names ), ws.separator,
        ws.memb_indent, "first-keycode", memb_name_w, ws.equals,
        _formatProtocolType( encoding->first_keycode ), ws.separator,
        ws.memb_indent, "count", memb_name_w, ws.equals,
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

    // event codes with msb toggled on are generated by request SendEvent
    const uint8_t code (
        reinterpret_cast< const protocol::events::Event::Header* >(
            data )->code & protocol::requests::SendEvent::EVENT_CODE_MASK );
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
    using protocol::requests::SendEvent;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::events::Event::ENCODING_SZ );

    const protocol::events::Event::Header* header {
        reinterpret_cast< const protocol::events::Event::Header* >( data ) };
    // event codes with msb turned on are generated by request SendEvent
    const uint8_t code ( header->code & SendEvent::EVENT_CODE_MASK );
    // KeymapNotify presents edge case, as it does not encode a sequence number
    const std::string sequence_num_str {
        ( header->code == protocol::events::codes::KEYMAPNOTIFY ) ? "?????" :
        fmt::format( "{:05d}", header->sequence_num ) };
    const _ParsingOutputs event {
        _parseEvent( conn, data, sz, _ROOT_WS ) };
    fmt::println( settings.log_fs,
                  "C{:03d}:{:04d}B:{}:S{}: Event {}({}){}: {}",
                  conn->id, event.bytes_parsed, _SERVER_TO_CLIENT,
                  sequence_num_str, protocol::events::names[ code ], code,
                  header->code & SendEvent::GENERATED_EVENT_FLAG ?
                  " (generated)" : "", event.str );
    assert( event.bytes_parsed == protocol::events::Event::ENCODING_SZ );
    return event.bytes_parsed;
}
