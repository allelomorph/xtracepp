#include <string_view>
#include <tuple>
#include <unordered_map>

#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/requests.hpp"
#include "protocol/predefined_atoms.hpp"


template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetWindowAttributes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetWindowAttributes::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetWindowAttributes;
    const GetWindowAttributes::ReplyEncoding* encoding {
        reinterpret_cast< const GetWindowAttributes::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetWindowAttributes::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(GetWindowAttributes::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "do-not-propagate-mask" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "backing-store", name_width, _EQUALS,
        _formatInteger( encoding->backing_store, GetWindowAttributes::backing_store_names ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "visual", name_width, _EQUALS,
        _formatProtocolType( encoding->visual ), _SEPARATOR,
        _BASE_INDENTS.member, "class", name_width, _EQUALS,
        _formatInteger( encoding->class_,
                        GetWindowAttributes::class_names, _IndexRange{ 1, 2 } ), _SEPARATOR,
        _BASE_INDENTS.member, "bit-gravity", name_width, _EQUALS,
        _formatProtocolType( encoding->bit_gravity ), _SEPARATOR,
        _BASE_INDENTS.member, "win-gravity", name_width, _EQUALS,
        _formatProtocolType( encoding->win_gravity ), _SEPARATOR,
        _BASE_INDENTS.member, "backing-planes", name_width, _EQUALS,
        _formatInteger( encoding->backing_planes ), _SEPARATOR,
        _BASE_INDENTS.member, "backing-pixel", name_width, _EQUALS,
        _formatInteger( encoding->backing_pixel ), _SEPARATOR,
        _BASE_INDENTS.member, "save-under", name_width, _EQUALS,
        _formatProtocolType( encoding->save_under ), _SEPARATOR,
        _BASE_INDENTS.member, "map-is-installed", name_width, _EQUALS,
        _formatProtocolType( encoding->map_is_installed ), _SEPARATOR,
        _BASE_INDENTS.member, "map-state", name_width, _EQUALS,
        _formatInteger( encoding->map_state, GetWindowAttributes::map_state_names ), _SEPARATOR,
        _BASE_INDENTS.member, "override-redirect", name_width, _EQUALS,
        _formatProtocolType( encoding->override_redirect ), _SEPARATOR,
        _BASE_INDENTS.member, "all-event-masks", name_width, _EQUALS,
        _formatProtocolType( encoding->all_event_masks ), _SEPARATOR,
        _BASE_INDENTS.member, "your-event-mask", name_width, _EQUALS,
        _formatProtocolType( encoding->your_event_mask ), _SEPARATOR,
        _BASE_INDENTS.member, "do-not-propagate-mask", name_width, _EQUALS,
        _formatProtocolType( encoding->do_not_propagate_mask ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetGeometry >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetGeometry::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetGeometry;
    const GetGeometry::ReplyEncoding* encoding {
        reinterpret_cast< const GetGeometry::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetGeometry::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(GetGeometry::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "do-not-propagate-mask" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "depth", name_width, _EQUALS,
        _formatInteger( encoding->depth ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "root", name_width, _EQUALS,
        _formatProtocolType( encoding->root ), _SEPARATOR,
        _BASE_INDENTS.member, "x", name_width, _EQUALS,
        _formatInteger( encoding->x ), _SEPARATOR,
        _BASE_INDENTS.member, "y", name_width, _EQUALS,
        _formatInteger( encoding->y ), _SEPARATOR,
        _BASE_INDENTS.member, "width", name_width, _EQUALS,
        _formatInteger( encoding->width ), _SEPARATOR,
        _BASE_INDENTS.member, "height", name_width, _EQUALS,
        _formatInteger( encoding->height ), _SEPARATOR,
        _BASE_INDENTS.member, "border-width", name_width, _EQUALS,
        _formatInteger( encoding->border_width ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryTree >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryTree::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::QueryTree;
    const QueryTree::ReplyEncoding* encoding {
        reinterpret_cast< const QueryTree::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( QueryTree::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(QueryTree::ReplyEncoding) +
              ( sizeof(protocol::WINDOW) * encoding->n ) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs children {
        _parseLISTof< protocol::WINDOW >(
            data + bytes_parsed, sz - bytes_parsed, encoding->n,
            _BASE_INDENTS.nested() ) };
    assert( encoding->reply_length == children.bytes_parsed / _ALIGN );
    bytes_parsed += children.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "root", name_width, _EQUALS,
        _formatProtocolType( encoding->root ), _SEPARATOR,
        _BASE_INDENTS.member, "parent", name_width, _EQUALS,
        _formatProtocolType( encoding->parent, QueryTree::parent_names ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n children", name_width, _EQUALS,
            _formatInteger( encoding->n ), _SEPARATOR ),
        _BASE_INDENTS.member, "children", name_width, _EQUALS,
        children.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::InternAtom >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::InternAtom::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::InternAtom;
    const InternAtom::ReplyEncoding* encoding {
        reinterpret_cast< const InternAtom::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( InternAtom::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(InternAtom::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );
    // TBD make this assert for every Reply
    assert( conn->sequence == encoding->sequence_number );
    if ( encoding->atom.data != protocol::atoms::NONE )
        _internStashedAtom( { conn->id, conn->sequence }, encoding->atom );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "atom", name_width, _EQUALS,
        _formatProtocolType( encoding->atom, InternAtom::atom_names ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetAtomName >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetAtomName::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetAtomName;
    const GetAtomName::ReplyEncoding* encoding {
        reinterpret_cast< const GetAtomName::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetAtomName::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(GetAtomName::ReplyEncoding) +
              _pad( sizeof(char) * encoding->n ) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const std::string_view name {
        reinterpret_cast<const char*>( data + bytes_parsed ), encoding->n };
    bytes_parsed += _pad(encoding->n);

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}\"{}\"{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "name length", name_width, _EQUALS,
            _formatInteger( encoding->n ), _SEPARATOR ),
        _BASE_INDENTS.member, "name", name_width, _EQUALS,
        name, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetProperty::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetProperty;
    const GetProperty::ReplyEncoding* encoding {
        reinterpret_cast< const GetProperty::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetProperty::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    const uint32_t value_sz { [&encoding](){
        uint32_t bytes { encoding->fmt_unit_ct };
        switch ( encoding->format ) {
        case 0:  bytes *= 0; break;
        case 8:  bytes *= 1; break;
        case 16: bytes *= 2; break;
        case 32: bytes *= 4; break;
        default: break;
        }
        return bytes;
    }() };
    assert( encoding->reply_length ==
            ( sizeof(GetProperty::ReplyEncoding) + _pad( value_sz ) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs value {
        _parseLISTof< protocol::BYTE >(
            data + bytes_parsed, sz - bytes_parsed, value_sz,
            _BASE_INDENTS.nested( _Indentation::SINGLELINE ) ) };
    bytes_parsed += _pad( value.bytes_parsed );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "format", name_width, _EQUALS,
        _formatInteger( encoding->format ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "type", name_width, _EQUALS,
        _formatProtocolType( encoding->type, GetProperty::reply_type_names ), _SEPARATOR,
        _BASE_INDENTS.member, "bytes-after", name_width, _EQUALS,
        _formatInteger( encoding->bytes_after ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n format units", name_width, _EQUALS,
            _formatInteger( encoding->fmt_unit_ct ), _SEPARATOR ),
        _BASE_INDENTS.member, "value", name_width, _EQUALS,
        value.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::ListProperties >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::ListProperties::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::ListProperties;
    const ListProperties::ReplyEncoding* encoding {
        reinterpret_cast< const ListProperties::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( ListProperties::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length == encoding->n );
    assert( encoding->reply_length ==
            ( sizeof(ListProperties::ReplyEncoding) +
              _pad( sizeof(protocol::ATOM) * encoding->n ) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs atoms {
        _parseLISTof< protocol::ATOM >(
            data + bytes_parsed, sz - bytes_parsed, encoding->n,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += _pad( atoms.bytes_parsed );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n atoms", name_width, _EQUALS,
            _formatInteger( encoding->n ), _SEPARATOR ),
        _BASE_INDENTS.member, "atoms", name_width, _EQUALS,
        atoms.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetSelectionOwner >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetSelectionOwner::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetSelectionOwner;
    const GetSelectionOwner::ReplyEncoding* encoding {
        reinterpret_cast< const GetSelectionOwner::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetSelectionOwner::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(GetSelectionOwner::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "owner", name_width, _EQUALS,
        _formatProtocolType( encoding->owner, GetSelectionOwner::owner_names ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GrabPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GrabPointer::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GrabPointer;
    const GrabPointer::ReplyEncoding* encoding {
        reinterpret_cast< const GrabPointer::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GrabPointer::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(GrabPointer::ReplyEncoding)  -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "status", name_width, _EQUALS,
        _formatInteger( encoding->status, GrabPointer::status_names ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GrabKeyboard >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GrabKeyboard::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GrabKeyboard;
    const GrabKeyboard::ReplyEncoding* encoding {
        reinterpret_cast< const GrabKeyboard::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GrabKeyboard::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(GrabKeyboard::ReplyEncoding)  -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "status", name_width, _EQUALS,
        _formatInteger( encoding->status, GrabKeyboard::status_names ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryPointer::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::QueryPointer;
    const QueryPointer::ReplyEncoding* encoding {
        reinterpret_cast< const QueryPointer::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( QueryPointer::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(QueryPointer::ReplyEncoding)  -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "same-screen", name_width, _EQUALS,
        _formatProtocolType( encoding->same_screen ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "root", name_width, _EQUALS,
        _formatProtocolType( encoding->root ), _SEPARATOR,
        _BASE_INDENTS.member, "child", name_width, _EQUALS,
        _formatProtocolType( encoding->child, QueryPointer::child_names ), _SEPARATOR,
        _BASE_INDENTS.member, "root-x", name_width, _EQUALS,
        _formatInteger( encoding->root_x ), _SEPARATOR,
        _BASE_INDENTS.member, "root-y", name_width, _EQUALS,
        _formatInteger( encoding->root_y ), _SEPARATOR,
        _BASE_INDENTS.member, "win-x", name_width, _EQUALS,
        _formatInteger( encoding->win_x ), _SEPARATOR,
        _BASE_INDENTS.member, "win-y", name_width, _EQUALS,
        _formatInteger( encoding->win_y ), _SEPARATOR,
        _BASE_INDENTS.member, "mask", name_width, _EQUALS,
        _formatProtocolType( encoding->mask ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetMotionEvents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetMotionEvents::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetMotionEvents;
    const GetMotionEvents::ReplyEncoding* encoding {
        reinterpret_cast< const GetMotionEvents::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetMotionEvents::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(GetMotionEvents::ReplyEncoding) +
              ( sizeof(GetMotionEvents::TIMECOORD) * encoding->n ) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs events {
        _parseLISTof< GetMotionEvents::TIMECOORD >(
            data + bytes_parsed, sz - bytes_parsed, encoding->n,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += events.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n events", name_width, _EQUALS,
            _formatInteger( encoding->n ), _SEPARATOR ),
        _BASE_INDENTS.member, "events", name_width, _EQUALS,
        events.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::TranslateCoordinates >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::TranslateCoordinates::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::TranslateCoordinates;
    const TranslateCoordinates::ReplyEncoding* encoding {
        reinterpret_cast< const TranslateCoordinates::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( TranslateCoordinates::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length == 0 );
    assert( encoding->reply_length ==
            ( sizeof(TranslateCoordinates::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "same-screen", name_width, _EQUALS,
        _formatProtocolType( encoding->same_screen ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "child", name_width, _EQUALS,
        _formatProtocolType( encoding->child, TranslateCoordinates::child_names ), _SEPARATOR,
        _BASE_INDENTS.member, "dst-x", name_width, _EQUALS,
        _formatInteger( encoding->dst_x ), _SEPARATOR,
        _BASE_INDENTS.member, "dst-y", name_width, _EQUALS,
        _formatInteger( encoding->dst_y ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetInputFocus >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetInputFocus::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetInputFocus;
    const GetInputFocus::ReplyEncoding* encoding {
        reinterpret_cast< const GetInputFocus::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetInputFocus::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(GetInputFocus::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "revert-to", name_width, _EQUALS,
        _formatInteger( encoding->revert_to, GetInputFocus::revert_to_names ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "focus", name_width, _EQUALS,
        _formatProtocolType( encoding->focus, GetInputFocus::focus_names ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryKeymap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryKeymap::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::QueryKeymap;
    const QueryKeymap::ReplyEncoding* encoding {
        reinterpret_cast< const QueryKeymap::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( QueryKeymap::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(QueryKeymap::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs keys {
        _parseLISTof< protocol::CARD8 >(
            encoding->keys, sizeof(encoding->keys), sizeof(encoding->keys),
            _BASE_INDENTS.nested( _Indentation::SINGLELINE ) ) };
    // keys bytes already parsed as part of encoding

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "keys", name_width, _EQUALS,
        keys.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryFont::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::QueryFont;
    const QueryFont::ReplyEncoding* encoding {
        reinterpret_cast< const QueryFont::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( QueryFont::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof( QueryFont::ReplyEncoding ) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ +
              ( sizeof( QueryFont::FONTPROP ) * encoding->n ) +
              ( sizeof( QueryFont::CHARINFO ) * encoding->m ) ) / _ALIGN );

    const _ParsingOutputs properties {
        _parseLISTof< QueryFont::FONTPROP >(
            data + bytes_parsed, sz - bytes_parsed, encoding->n,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += properties.bytes_parsed;
    const _ParsingOutputs char_infos {
        _parseLISTof< QueryFont::CHARINFO >(
            data + bytes_parsed, sz - bytes_parsed, encoding->m,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += char_infos.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "max-char-or-byte2" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "min-bounds", name_width, _EQUALS,
        _formatProtocolType( encoding->min_bounds, _BASE_INDENTS.nested() ), _SEPARATOR,
        _BASE_INDENTS.member, "max-bounds", name_width, _EQUALS,
        _formatProtocolType( encoding->max_bounds, _BASE_INDENTS.nested() ), _SEPARATOR,
        _BASE_INDENTS.member, "min-char-or-byte2", name_width, _EQUALS,
        _formatInteger( encoding->min_char_or_byte2 ), _SEPARATOR,
        _BASE_INDENTS.member, "max-char-or-byte2", name_width, _EQUALS,
        _formatInteger( encoding->max_char_or_byte2 ), _SEPARATOR,
        _BASE_INDENTS.member, "default-char", name_width, _EQUALS,
        _formatInteger( encoding->default_char ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n properties", name_width, _EQUALS,
            _formatInteger( encoding->n ), _SEPARATOR ),
        _BASE_INDENTS.member, "draw-direction", name_width, _EQUALS,
        _formatInteger( encoding->draw_direction, QueryFont::draw_direction_names ), _SEPARATOR,
        _BASE_INDENTS.member, "min-byte1", name_width, _EQUALS,
        _formatInteger( encoding->min_byte1 ), _SEPARATOR,
        _BASE_INDENTS.member, "max-byte1", name_width, _EQUALS,
        _formatInteger( encoding->max_byte1 ), _SEPARATOR,
        _BASE_INDENTS.member, "all-chars-exist", name_width, _EQUALS,
        _formatProtocolType( encoding->all_chars_exist ), _SEPARATOR,
        _BASE_INDENTS.member, "font-ascent", name_width, _EQUALS,
        _formatInteger( encoding->font_ascent ), _SEPARATOR,
        _BASE_INDENTS.member, "font-descent", name_width, _EQUALS,
        _formatInteger( encoding->font_descent ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "m char-infos", name_width, _EQUALS,
            _formatInteger( encoding->m ), _SEPARATOR ),
        _BASE_INDENTS.member, "properties", name_width, _EQUALS,
        properties.str, _SEPARATOR,
        _BASE_INDENTS.member, "char-infos", name_width, _EQUALS,
        char_infos.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryTextExtents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryTextExtents::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::QueryTextExtents;
    const QueryTextExtents::ReplyEncoding* encoding {
        reinterpret_cast< const QueryTextExtents::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( QueryTextExtents::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof( QueryTextExtents::ReplyEncoding ) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "draw-direction", name_width, _EQUALS,
        _formatInteger( encoding->draw_direction, QueryTextExtents::draw_direction_names ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "font-ascent", name_width, _EQUALS,
        _formatInteger( encoding->font_ascent ), _SEPARATOR,
        _BASE_INDENTS.member, "font-descent", name_width, _EQUALS,
        _formatInteger( encoding->font_descent ), _SEPARATOR,
        _BASE_INDENTS.member, "overall-ascent", name_width, _EQUALS,
        _formatInteger( encoding->overall_ascent ), _SEPARATOR,
        _BASE_INDENTS.member, "overall-descent", name_width, _EQUALS,
        _formatInteger( encoding->overall_descent ), _SEPARATOR,
        _BASE_INDENTS.member, "overall-width", name_width, _EQUALS,
        _formatInteger( encoding->overall_width ), _SEPARATOR,
        _BASE_INDENTS.member, "overall-left", name_width, _EQUALS,
        _formatInteger( encoding->overall_left ), _SEPARATOR,
        _BASE_INDENTS.member, "overall-right", name_width, _EQUALS,
        _formatInteger( encoding->overall_right ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::ListFonts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::ListFonts::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::ListFonts;
    const ListFonts::ReplyEncoding* encoding {
        reinterpret_cast< const ListFonts::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( ListFonts::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );

    const _ParsingOutputs names {
        _parseLISTof< protocol::STR >(
            data + bytes_parsed, sz - bytes_parsed, encoding->name_ct,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += _pad(names.bytes_parsed);
    assert( encoding->reply_length ==
            ( sizeof(ListFonts::ReplyEncoding) +
              _pad(names.bytes_parsed) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n names", name_width, _EQUALS,
            _formatInteger( encoding->name_ct ), _SEPARATOR ),
        _BASE_INDENTS.member, "names", name_width, _EQUALS,
        names.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::ListFontsWithInfo >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::ListFontsWithInfo::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::ListFontsWithInfo;
    const ListFontsWithInfo::ReplyEncoding* encoding {
        reinterpret_cast< const ListFontsWithInfo::ReplyEncoding* >( data ) };
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );

    if ( encoding->n == ListFontsWithInfo::LAST_REPLY ) {
        const ListFontsWithInfo::FinalReplyEncoding* fr_encoding {
            reinterpret_cast< const ListFontsWithInfo::FinalReplyEncoding* >( data ) };
        bytes_parsed += sizeof( ListFontsWithInfo::FinalReplyEncoding );
        assert( encoding->reply_length ==
                ( sizeof(ListFontsWithInfo::FinalReplyEncoding) -
                  protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );
        const uint32_t name_width (
            settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
        fmt::println(
            "{{{}"
            "{}"
            "{}{: <{}}{}{}{}"
            "{}{}"
            "{}}}",
            _SEPARATOR,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _BASE_INDENTS.member, "reply", name_width, _EQUALS,
                _formatInteger( fr_encoding->reply ), _SEPARATOR ),
            _BASE_INDENTS.member, "last-reply", name_width, _EQUALS,
            _formatInteger( fr_encoding->last_reply ), _SEPARATOR,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
                _formatInteger( fr_encoding->sequence_number ), _SEPARATOR ),
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
                _formatInteger( fr_encoding->reply_length ), _SEPARATOR ),
            _BASE_INDENTS.enclosure
            );
    } else {  // encoding->n != ListFontsWithInfo::LAST_REPLY
        bytes_parsed += sizeof( ListFontsWithInfo::ReplyEncoding );

        const _ParsingOutputs properties {
            _parseLISTof< ListFontsWithInfo::FONTPROP >(
                data + bytes_parsed, sz - bytes_parsed, encoding->m,
                _BASE_INDENTS.nested() ) };
        bytes_parsed += properties.bytes_parsed;
        const std::string_view name {
            reinterpret_cast<const char*>( data + bytes_parsed ), encoding->n };
        bytes_parsed += _pad( encoding->n );

        assert( encoding->reply_length ==
                ( sizeof(ListFontsWithInfo::ReplyEncoding) +
                  _pad(encoding->n) +
                  ( sizeof(ListFontsWithInfo::FONTPROP) * encoding->m ) -
                  protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );
        const uint32_t name_width (
            settings.multiline ? sizeof( "max-char-or-byte2" ) - 1 : 0 );
        fmt::println(
            "{{{}"
            "{}{}{}{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}{: <{}}{}\"{}\"{}"
            "{}}}",
            _SEPARATOR,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _BASE_INDENTS.member, "reply", name_width, _EQUALS,
                _formatInteger( encoding->reply ), _SEPARATOR ),
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _BASE_INDENTS.member, "name len", name_width, _EQUALS,
                _formatInteger( encoding->n ), _SEPARATOR ),
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
                _formatInteger( encoding->sequence_number ), _SEPARATOR ),
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
                _formatInteger( encoding->reply_length ), _SEPARATOR ),
            _BASE_INDENTS.member, "min-bounds", name_width, _EQUALS,
            _formatProtocolType( encoding->min_bounds, _BASE_INDENTS.nested() ), _SEPARATOR,
            _BASE_INDENTS.member, "max-bounds", name_width, _EQUALS,
            _formatProtocolType( encoding->max_bounds, _BASE_INDENTS.nested() ), _SEPARATOR,
            _BASE_INDENTS.member, "min-char-or-byte2", name_width, _EQUALS,
            _formatInteger( encoding->min_char_or_byte2 ), _SEPARATOR,
            _BASE_INDENTS.member, "max-char-or-byte2", name_width, _EQUALS,
            _formatInteger( encoding->max_char_or_byte2 ), _SEPARATOR,
            _BASE_INDENTS.member, "default-char", name_width, _EQUALS,
            _formatInteger( encoding->default_char ), _SEPARATOR,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _BASE_INDENTS.member, "n properties", name_width, _EQUALS,
                _formatInteger( encoding->m ), _SEPARATOR ),
            _BASE_INDENTS.member, "draw-direction", name_width, _EQUALS,
            _formatInteger( encoding->draw_direction, ListFontsWithInfo::draw_direction_names ), _SEPARATOR,
            _BASE_INDENTS.member, "min-byte1", name_width, _EQUALS,
            _formatInteger( encoding->min_byte1 ), _SEPARATOR,
            _BASE_INDENTS.member, "max-byte1", name_width, _EQUALS,
            _formatInteger( encoding->max_byte1 ), _SEPARATOR,
            _BASE_INDENTS.member, "all-chars-exist", name_width, _EQUALS,
            _formatProtocolType( encoding->all_chars_exist ), _SEPARATOR,
            _BASE_INDENTS.member, "font-ascent", name_width, _EQUALS,
            _formatInteger( encoding->font_ascent ), _SEPARATOR,
            _BASE_INDENTS.member, "font-descent", name_width, _EQUALS,
            _formatInteger( encoding->font_descent ), _SEPARATOR,
            _BASE_INDENTS.member, "replies-hint", name_width, _EQUALS,
            _formatInteger( encoding->replies_hint ), _SEPARATOR,
            _BASE_INDENTS.member, "properties", name_width, _EQUALS,
            properties.str, _SEPARATOR,
            _BASE_INDENTS.member, "name", name_width, _EQUALS,
            name, _SEPARATOR,
            _BASE_INDENTS.enclosure
            );
    }
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetFontPath >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetFontPath::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetFontPath;
    const GetFontPath::ReplyEncoding* encoding {
        reinterpret_cast< const GetFontPath::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetFontPath::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );

    const _ParsingOutputs path {
        _parseLISTof< protocol::STR >(
            data + bytes_parsed, sz - bytes_parsed, encoding->str_ct,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += _pad(path.bytes_parsed);
    assert( encoding->reply_length ==
            ( sizeof(GetFontPath::ReplyEncoding) +
              _pad(path.bytes_parsed) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n path elems", name_width, _EQUALS,
            _formatInteger( encoding->str_ct ), _SEPARATOR ),
        _BASE_INDENTS.member, "path", name_width, _EQUALS,
        path.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetImage >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetImage::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetImage;
    const GetImage::ReplyEncoding* encoding {
        reinterpret_cast< const GetImage::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetImage::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );

    const _ParsingOutputs data_ {
        _parseLISTof< protocol::BYTE >(
            data + bytes_parsed, sz - bytes_parsed,
            encoding->reply_length * _ALIGN, _BASE_INDENTS.nested() ) };
    bytes_parsed += _pad(data_.bytes_parsed);
    // TBD no assert on reply_length as in this case it is the source of the encoding
    //   suffix length (no way to know value of p but n will always be padded)

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "depth", name_width, _EQUALS,
        _formatInteger( encoding->depth ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "visual", name_width, _EQUALS,
        _formatProtocolType( encoding->visual, GetImage::visual_names ), _SEPARATOR,
        _BASE_INDENTS.member, "data", name_width, _EQUALS,
        data_.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::ListInstalledColormaps >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::ListInstalledColormaps::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::ListInstalledColormaps;
    const ListInstalledColormaps::ReplyEncoding* encoding {
        reinterpret_cast< const ListInstalledColormaps::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( ListInstalledColormaps::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(ListInstalledColormaps::ReplyEncoding) +
              ( sizeof(protocol::COLORMAP) * encoding->n ) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs cmaps {
        _parseLISTof< protocol::COLORMAP >(
            data + bytes_parsed, sz- bytes_parsed, encoding->n,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += cmaps.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n cmaps", name_width, _EQUALS,
            _formatInteger( encoding->n ), _SEPARATOR ),
        _BASE_INDENTS.member, "cmaps", name_width, _EQUALS,
        cmaps.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::AllocColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::AllocColor::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::AllocColor;
    const AllocColor::ReplyEncoding* encoding {
        reinterpret_cast< const AllocColor::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( AllocColor::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(AllocColor::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "red", name_width, _EQUALS,
        _formatInteger( encoding->red ), _SEPARATOR,
        _BASE_INDENTS.member, "green", name_width, _EQUALS,
        _formatInteger( encoding->green ), _SEPARATOR,
        _BASE_INDENTS.member, "blue", name_width, _EQUALS,
        _formatInteger( encoding->blue ), _SEPARATOR,
        _BASE_INDENTS.member, "pixel", name_width, _EQUALS,
        _formatInteger( encoding->pixel ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::AllocNamedColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::AllocNamedColor::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::AllocNamedColor;
    const AllocNamedColor::ReplyEncoding* encoding {
        reinterpret_cast< const AllocNamedColor::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( AllocNamedColor::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(AllocNamedColor::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "pixel", name_width, _EQUALS,
        _formatInteger( encoding->pixel ), _SEPARATOR,
        _BASE_INDENTS.member, "exact-red", name_width, _EQUALS,
        _formatInteger( encoding->exact_red ), _SEPARATOR,
        _BASE_INDENTS.member, "exact-green", name_width, _EQUALS,
        _formatInteger( encoding->exact_green ), _SEPARATOR,
        _BASE_INDENTS.member, "exact-blue", name_width, _EQUALS,
        _formatInteger( encoding->exact_blue ), _SEPARATOR,
        _BASE_INDENTS.member, "visual-red", name_width, _EQUALS,
        _formatInteger( encoding->visual_red ), _SEPARATOR,
        _BASE_INDENTS.member, "visual-green", name_width, _EQUALS,
        _formatInteger( encoding->visual_green ), _SEPARATOR,
        _BASE_INDENTS.member, "visual-blue", name_width, _EQUALS,
        _formatInteger( encoding->visual_blue ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::AllocColorCells >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::AllocColorCells::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::AllocColorCells;
    const AllocColorCells::ReplyEncoding* encoding {
        reinterpret_cast< const AllocColorCells::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( AllocColorCells::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(AllocColorCells::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + bytes_parsed, sz - bytes_parsed, encoding->n,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += pixels.bytes_parsed;
    // TBD format as bitmasks
    const _ParsingOutputs masks {
        _parseLISTof< protocol::CARD32 >(
            data + bytes_parsed, sz - bytes_parsed, encoding->m,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += pixels.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n pixels", name_width, _EQUALS,
            _formatInteger( encoding->n ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n masks", name_width, _EQUALS,
            _formatInteger( encoding->m ), _SEPARATOR ),
        _BASE_INDENTS.member, "pixels", name_width, _EQUALS,
        pixels.str, _SEPARATOR,
        _BASE_INDENTS.member, "masks", name_width, _EQUALS,
        masks.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::AllocColorPlanes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::AllocColorPlanes::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::AllocColorPlanes;
    const AllocColorPlanes::ReplyEncoding* encoding {
        reinterpret_cast< const AllocColorPlanes::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( AllocColorPlanes::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(AllocColorPlanes::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + bytes_parsed, sz - bytes_parsed, encoding->n,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += pixels.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n pixels", name_width, _EQUALS,
            _formatInteger( encoding->n ), _SEPARATOR ),
        _BASE_INDENTS.member, "red-mask", name_width, _EQUALS,
        _formatBitmask( encoding->red_mask ), _SEPARATOR,
        _BASE_INDENTS.member, "green-mask", name_width, _EQUALS,
        _formatBitmask( encoding->green_mask ), _SEPARATOR,
        _BASE_INDENTS.member, "blue-mask", name_width, _EQUALS,
        _formatBitmask( encoding->blue_mask ), _SEPARATOR,
        _BASE_INDENTS.member, "pixels", name_width, _EQUALS,
        pixels.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryColors::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::QueryColors;
    const QueryColors::ReplyEncoding* encoding {
        reinterpret_cast< const QueryColors::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( QueryColors::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(QueryColors::ReplyEncoding) +
              ( sizeof(QueryColors::RGB) * encoding->n ) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs colors {
        _parseLISTof< QueryColors::RGB >(
            data + bytes_parsed, sz - bytes_parsed, encoding->n,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += colors.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n colors", name_width, _EQUALS,
            _formatInteger( encoding->n ), _SEPARATOR ),
        _BASE_INDENTS.member, "colors", name_width, _EQUALS,
        colors.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::LookupColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::LookupColor::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::LookupColor;
    const LookupColor::ReplyEncoding* encoding {
        reinterpret_cast< const LookupColor::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( LookupColor::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(LookupColor::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "exact-red", name_width, _EQUALS,
        _formatInteger( encoding->exact_red ), _SEPARATOR,
        _BASE_INDENTS.member, "exact-green", name_width, _EQUALS,
        _formatInteger( encoding->exact_green ), _SEPARATOR,
        _BASE_INDENTS.member, "exact-blue", name_width, _EQUALS,
        _formatInteger( encoding->exact_blue ), _SEPARATOR,
        _BASE_INDENTS.member, "visual-red", name_width, _EQUALS,
        _formatInteger( encoding->visual_red ), _SEPARATOR,
        _BASE_INDENTS.member, "visual-green", name_width, _EQUALS,
        _formatInteger( encoding->visual_green ), _SEPARATOR,
        _BASE_INDENTS.member, "visual-blue", name_width, _EQUALS,
        _formatInteger( encoding->visual_blue ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryBestSize >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryBestSize::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::QueryBestSize;
    const QueryBestSize::ReplyEncoding* encoding {
        reinterpret_cast< const QueryBestSize::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( QueryBestSize::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(QueryBestSize::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "width", name_width, _EQUALS,
        _formatInteger( encoding->width ), _SEPARATOR,
        _BASE_INDENTS.member, "height", name_width, _EQUALS,
        _formatInteger( encoding->height ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

// TBD need to implement encoding->present spoofing for --denyextensions
// TBD important for later implementation of extensions:
//   encoding->first_event: base event code
//   encoding->first_error: base error code
template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryExtension >(
        Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryExtension::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::QueryExtension;
    // not const to allow for .present spoofing
    QueryExtension::ReplyEncoding* encoding {
        reinterpret_cast< QueryExtension::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( QueryExtension::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(QueryExtension::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );
    if ( settings.denyallextensions )
        encoding->present.data = int(false);

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "present", name_width, _EQUALS,
        _formatProtocolType( encoding->present ), _SEPARATOR,
        _BASE_INDENTS.member, "major-opcode", name_width, _EQUALS,
        _formatInteger( encoding->major_opcode ), _SEPARATOR,
        _BASE_INDENTS.member, "first-event", name_width, _EQUALS,
        _formatInteger( encoding->first_event ), _SEPARATOR,
        _BASE_INDENTS.member, "first-error", name_width, _EQUALS,
        _formatInteger( encoding->first_error ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::ListExtensions >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::ListExtensions::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::ListExtensions;
    const ListExtensions::ReplyEncoding* encoding {
        reinterpret_cast< const ListExtensions::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( ListExtensions::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );

    const _ParsingOutputs names {
        _parseLISTof< protocol::STR >(
            data + bytes_parsed, sz - bytes_parsed, encoding->n,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += _pad(names.bytes_parsed);
    assert( encoding->reply_length ==
            ( sizeof(ListExtensions::ReplyEncoding) +
              _pad(names.bytes_parsed) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n names", name_width, _EQUALS,
            _formatInteger( encoding->n ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "names", name_width, _EQUALS,
        names.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetKeyboardMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetKeyboardMapping::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetKeyboardMapping;
    const GetKeyboardMapping::ReplyEncoding* encoding {
        reinterpret_cast< const GetKeyboardMapping::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetKeyboardMapping::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );

    // n_keycodes should equal `count` from the request
    const uint32_t n_keycodes { encoding->reply_length / encoding->keysyms_per_keycode };
    const _ParsingOutputs keysyms {
        _parseLISTof< protocol::KEYSYM >(
            data + bytes_parsed, sz - bytes_parsed,
            n_keycodes * encoding->keysyms_per_keycode,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += keysyms.bytes_parsed;
    assert( encoding->reply_length ==
            ( sizeof(GetKeyboardMapping::ReplyEncoding) +
              keysyms.bytes_parsed -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "keysyms-per-keycode" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "keysyms-per-keycode", name_width, _EQUALS,
            _formatInteger( encoding->keysyms_per_keycode ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "keysyms", name_width, _EQUALS,
        keysyms.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetKeyboardControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetKeyboardControl::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetKeyboardControl;
    const GetKeyboardControl::ReplyEncoding* encoding {
        reinterpret_cast< const GetKeyboardControl::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetKeyboardControl::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(GetKeyboardControl::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs auto_repeats {
        _parseLISTof< protocol::CARD8 >(
            data + bytes_parsed, sz - bytes_parsed,
            sizeof(GetKeyboardControl::ReplyEncoding::auto_repeats),
            _BASE_INDENTS.nested( _Indentation::SINGLELINE ) ) };
    // auto_repeats.bytes_parsed ignored, as auto-repeats is a fixed size array
    //   member of GetKeyboardControl::ReplyEncoding

    const uint32_t name_width (
        settings.multiline ? sizeof( "global-auto-repeat" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "global-auto-repeat", name_width, _EQUALS,
        _formatInteger( encoding->global_auto_repeat, GetKeyboardControl::global_auto_repeat_names ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "led-mask", name_width, _EQUALS,
        _formatInteger( encoding->led_mask ), _SEPARATOR,
        _BASE_INDENTS.member, "key-click-percent", name_width, _EQUALS,
        _formatInteger( encoding->key_click_percent ), _SEPARATOR,
        _BASE_INDENTS.member, "bell-percent", name_width, _EQUALS,
        _formatInteger( encoding->bell_percent ), _SEPARATOR,
        _BASE_INDENTS.member, "bell-pitch", name_width, _EQUALS,
        _formatInteger( encoding->bell_pitch ), _SEPARATOR,
        _BASE_INDENTS.member, "bell-duration", name_width, _EQUALS,
        _formatInteger( encoding->bell_duration ), _SEPARATOR,
        _BASE_INDENTS.member, "auto-repeats", name_width, _EQUALS,
        auto_repeats.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetPointerControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetPointerControl::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetPointerControl;
    const GetPointerControl::ReplyEncoding* encoding {
        reinterpret_cast< const GetPointerControl::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetPointerControl::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(GetPointerControl::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "acceleration-denominator" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "acceleration-numerator", name_width, _EQUALS,
        _formatInteger( encoding->acceleration_numerator ), _SEPARATOR,
        _BASE_INDENTS.member, "acceleration-denominator", name_width, _EQUALS,
        _formatInteger( encoding->acceleration_denominator ), _SEPARATOR,
        _BASE_INDENTS.member, "threshold", name_width, _EQUALS,
        _formatInteger( encoding->threshold ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetScreenSaver >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetScreenSaver::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetScreenSaver;
    const GetScreenSaver::ReplyEncoding* encoding {
        reinterpret_cast< const GetScreenSaver::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetScreenSaver::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(GetScreenSaver::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "timeout", name_width, _EQUALS,
        _formatInteger( encoding->timeout ), _SEPARATOR,
        _BASE_INDENTS.member, "interval", name_width, _EQUALS,
        _formatInteger( encoding->interval ), _SEPARATOR,
        _BASE_INDENTS.member, "prefer-blanking", name_width, _EQUALS,
        _formatInteger( encoding->prefer_blanking, GetScreenSaver::prefer_blanking_names ), _SEPARATOR,
        _BASE_INDENTS.member, "allow-exposures", name_width, _EQUALS,
        _formatInteger( encoding->allow_exposures, GetScreenSaver::allow_exposures_names ), _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::ListHosts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::ListHosts::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::ListHosts;
    const ListHosts::ReplyEncoding* encoding {
        reinterpret_cast< const ListHosts::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( ListHosts::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(ListHosts::ReplyEncoding) +
              ( sizeof(protocol::HOST) * encoding->n ) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs hosts {
        _parseLISTof< protocol::HOST >(
            data + bytes_parsed, sz - bytes_parsed, encoding->n,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += hosts.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "mode", name_width, _EQUALS,
        _formatInteger( encoding->mode, ListHosts::mode_names ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n hosts", name_width, _EQUALS,
            _formatInteger( encoding->n ), _SEPARATOR ),
        _BASE_INDENTS.member, "hosts", name_width, _EQUALS,
        hosts.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::SetPointerMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::SetPointerMapping::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::SetPointerMapping;
    const SetPointerMapping::ReplyEncoding* encoding {
        reinterpret_cast< const SetPointerMapping::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( SetPointerMapping::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(SetPointerMapping::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "status", name_width, _EQUALS,
        _formatInteger( encoding->status, SetPointerMapping::status_names ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetPointerMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetPointerMapping::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetPointerMapping;
    const GetPointerMapping::ReplyEncoding* encoding {
        reinterpret_cast< const GetPointerMapping::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetPointerMapping::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(GetPointerMapping::ReplyEncoding) +
              _pad(encoding->n) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs map {
        _parseLISTof< protocol::CARD8 >(
            data + bytes_parsed, sz - bytes_parsed, encoding->n,
            _BASE_INDENTS.nested( _Indentation::SINGLELINE ) ) };
    bytes_parsed += _pad(map.bytes_parsed);

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "n map bytes", name_width, _EQUALS,
            _formatInteger( encoding->n ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "map", name_width, _EQUALS,
        map.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::SetModifierMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::SetModifierMapping::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::SetModifierMapping;
    const SetModifierMapping::ReplyEncoding* encoding {
        reinterpret_cast< const SetModifierMapping::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( SetModifierMapping::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(SetModifierMapping::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "status", name_width, _EQUALS,
        _formatInteger( encoding->status, SetModifierMapping::status_names ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetModifierMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetModifierMapping::ReplyEncoding ) ); // TBD

    size_t bytes_parsed {};
    using protocol::requests::GetModifierMapping;
    const GetModifierMapping::ReplyEncoding* encoding {
        reinterpret_cast< const GetModifierMapping::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( GetModifierMapping::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    const uint32_t n_keycodes {
        GetModifierMapping::MODIFIER_CT * encoding->keycodes_per_modifier };
    assert( encoding->reply_length ==
            ( sizeof(GetModifierMapping::ReplyEncoding) +
              ( sizeof(protocol::KEYCODE) * n_keycodes ) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const _ParsingOutputs keycodes {
        _parseLISTof< protocol::KEYCODE >(
            data + bytes_parsed, sz - bytes_parsed, n_keycodes,
            _BASE_INDENTS.nested() ) };
    bytes_parsed += keycodes.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "keycodes-per-modifier" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply", name_width, _EQUALS,
            _formatInteger( encoding->reply ), _SEPARATOR ),
        _BASE_INDENTS.member, "keycodes-per-modifier", name_width, _EQUALS,
        _formatInteger( encoding->keycodes_per_modifier ), _SEPARATOR,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "sequence number", name_width, _EQUALS,
            _formatInteger( encoding->sequence_number ), _SEPARATOR ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _BASE_INDENTS.member, "reply length", name_width, _EQUALS,
            _formatInteger( encoding->reply_length ), _SEPARATOR ),
        _BASE_INDENTS.member, "keycodes", name_width, _EQUALS,
        keycodes.str, _SEPARATOR,
        _BASE_INDENTS.enclosure
        );
    return bytes_parsed;
}

// TBD default to const data, since mods are the exception
// TBD need data to be mutable for reply spoofing, eg QueryExtension
size_t X11ProtocolParser::_logServerReply(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::DEFAULT_REPLY_ENCODING_SZ );

    // get request opcode via sequence number
    using protocol::requests::ReplyHeader;
    const ReplyHeader* header {
        reinterpret_cast< const ReplyHeader* >( data ) };
    assert( header->reply == protocol::requests::REPLY_PREFIX );
    const uint8_t opcode { conn->lookupRequest( header->sequence_number ) };
    assert( opcode >= protocol::requests::opcodes::MIN &&
            opcode <= protocol::requests::opcodes::MAX );

    // static constexpr std::tuple<
    //     protocol::requests::GetWindowAttributes,     //  3
    //     protocol::requests::GetGeometry,             // 14
    //     protocol::requests::QueryTree,               // 15
    //     protocol::requests::InternAtom,              // 16
    //     protocol::requests::GetAtomName,             // 17
    //     protocol::requests::GetProperty,             // 20
    //     protocol::requests::ListProperties,          // 21
    //     protocol::requests::GetSelectionOwner,       // 23
    //     protocol::requests::GrabPointer,             // 26
    //     protocol::requests::GrabKeyboard,            // 31
    //     protocol::requests::QueryPointer,            // 38
    //     protocol::requests::GetMotionEvents,         // 39
    //     protocol::requests::TranslateCoordinates,    // 40
    //     protocol::requests::GetInputFocus,           // 43
    //     protocol::requests::QueryKeymap,             // 44
    //     protocol::requests::QueryFont,               // 47
    //     protocol::requests::QueryTextExtents,        // 48
    //     protocol::requests::ListFonts,               // 49
    //     protocol::requests::ListFontsWithInfo,       // 50
    //     protocol::requests::GetFontPath,             // 52
    //     protocol::requests::GetImage,                // 73
    //     protocol::requests::ListInstalledColormaps,  // 83
    //     protocol::requests::AllocColor,              // 84
    //     protocol::requests::AllocNamedColor,         // 85
    //     protocol::requests::AllocColorCells,         // 86
    //     protocol::requests::AllocColorPlanes,        // 87
    //     protocol::requests::QueryColors,             // 91
    //     protocol::requests::LookupColor,             // 92
    //     protocol::requests::QueryBestSize,           // 97
    //     protocol::requests::QueryExtension,          // 98
    //     protocol::requests::ListExtensions,          // 99
    //     protocol::requests::GetKeyboardMapping,      // 101
    //     protocol::requests::GetKeyboardControl,      // 103
    //     protocol::requests::GetPointerControl,       // 106
    //     protocol::requests::GetScreenSaver,          // 108
    //     protocol::requests::ListHosts,               // 110
    //     protocol::requests::SetPointerMapping,       // 116
    //     protocol::requests::GetPointerMapping,       // 117
    //     protocol::requests::SetModifierMapping,      // 118
    //     protocol::requests::GetModifierMapping       // 119
    //     > request_types {};

    size_t bytes_parsed {};
    fmt::print( "{:03d}:>: server reply to {}({}) ",
                conn->id, protocol::requests::names[ opcode ], opcode );
    switch ( opcode ) {
    case protocol::requests::opcodes::GETWINDOWATTRIBUTES:     //  3
        bytes_parsed = _logServerReply<
            protocol::requests::GetWindowAttributes >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETGEOMETRY:             // 14
        bytes_parsed = _logServerReply<
            protocol::requests::GetGeometry >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYTREE:               // 15
        bytes_parsed = _logServerReply<
            protocol::requests::QueryTree >( conn, data, sz );
        break;
    case protocol::requests::opcodes::INTERNATOM:              // 16
        bytes_parsed = _logServerReply<
            protocol::requests::InternAtom >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETATOMNAME:             // 17
        bytes_parsed = _logServerReply<
            protocol::requests::GetAtomName >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPROPERTY:             // 20
        bytes_parsed = _logServerReply<
            protocol::requests::GetProperty >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTPROPERTIES:          // 21
        bytes_parsed = _logServerReply<
            protocol::requests::ListProperties >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETSELECTIONOWNER:       // 23
        bytes_parsed = _logServerReply<
            protocol::requests::GetSelectionOwner >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABPOINTER:             // 26
        bytes_parsed = _logServerReply<
            protocol::requests::GrabPointer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABKEYBOARD:            // 31
        bytes_parsed = _logServerReply<
            protocol::requests::GrabKeyboard >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYPOINTER:            // 38
        bytes_parsed = _logServerReply<
            protocol::requests::QueryPointer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETMOTIONEVENTS:         // 39
        bytes_parsed = _logServerReply<
            protocol::requests::GetMotionEvents >( conn, data, sz );
        break;
    case protocol::requests::opcodes::TRANSLATECOORDINATES:    // 40
        bytes_parsed = _logServerReply<
            protocol::requests::TranslateCoordinates >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETINPUTFOCUS:           // 43
        bytes_parsed = _logServerReply<
            protocol::requests::GetInputFocus >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYKEYMAP:             // 44
        bytes_parsed = _logServerReply<
            protocol::requests::QueryKeymap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYFONT:               // 47
        bytes_parsed = _logServerReply<
            protocol::requests::QueryFont >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYTEXTEXTENTS:        // 48
        bytes_parsed = _logServerReply<
            protocol::requests::QueryTextExtents >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTFONTS:               // 49
        bytes_parsed = _logServerReply<
            protocol::requests::ListFonts >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTFONTSWITHINFO:       // 50
        bytes_parsed = _logServerReply<
            protocol::requests::ListFontsWithInfo >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETFONTPATH:             // 52
        bytes_parsed = _logServerReply<
            protocol::requests::GetFontPath >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETIMAGE:                // 73
        bytes_parsed = _logServerReply<
            protocol::requests::GetImage >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTINSTALLEDCOLORMAPS:  // 83
        bytes_parsed = _logServerReply<
            protocol::requests::ListInstalledColormaps >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLOR:              // 84
        bytes_parsed = _logServerReply<
            protocol::requests::AllocColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCNAMEDCOLOR:         // 85
        bytes_parsed = _logServerReply<
            protocol::requests::AllocNamedColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLORCELLS:         // 86
        bytes_parsed = _logServerReply<
            protocol::requests::AllocColorCells >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLORPLANES:        // 87
        bytes_parsed = _logServerReply<
            protocol::requests::AllocColorPlanes >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYCOLORS:             // 91
        bytes_parsed = _logServerReply<
            protocol::requests::QueryColors >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LOOKUPCOLOR:             // 92
        bytes_parsed = _logServerReply<
            protocol::requests::LookupColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYBESTSIZE:           // 97
        bytes_parsed = _logServerReply<
            protocol::requests::QueryBestSize >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYEXTENSION:          // 98
        bytes_parsed = _logServerReply<
            protocol::requests::QueryExtension >(
                conn, const_cast<uint8_t*>( data ), sz );
        break;
    case protocol::requests::opcodes::LISTEXTENSIONS:          // 99
        bytes_parsed = _logServerReply<
            protocol::requests::ListExtensions >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETKEYBOARDMAPPING:      // 101
        bytes_parsed = _logServerReply<
            protocol::requests::GetKeyboardMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETKEYBOARDCONTROL:      // 103
        bytes_parsed = _logServerReply<
            protocol::requests::GetKeyboardControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPOINTERCONTROL:       // 106
        bytes_parsed = _logServerReply<
            protocol::requests::GetPointerControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETSCREENSAVER:          // 108
        bytes_parsed = _logServerReply<
            protocol::requests::GetScreenSaver >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTHOSTS:               // 110
        bytes_parsed = _logServerReply<
            protocol::requests::ListHosts >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETPOINTERMAPPING:       // 116
        bytes_parsed = _logServerReply<
            protocol::requests::SetPointerMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPOINTERMAPPING:       // 117
        bytes_parsed = _logServerReply<
            protocol::requests::GetPointerMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETMODIFIERMAPPING:      // 118
        bytes_parsed = _logServerReply<
            protocol::requests::SetModifierMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETMODIFIERMAPPING:      // 119
        bytes_parsed = _logServerReply<
            protocol::requests::GetModifierMapping >( conn, data, sz );
        break;
    default:
        break;
    };
    assert( bytes_parsed >= protocol::requests::DEFAULT_REPLY_ENCODING_SZ );
    return bytes_parsed;
}
