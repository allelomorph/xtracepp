#include <string_view>
#include <tuple>
#include <unordered_map>

#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/requests.hpp"
#include "protocol/atoms.hpp"


template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetWindowAttributes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetWindowAttributes::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetWindowAttributes;
    const GetWindowAttributes::ReplyEncoding* encoding {
        reinterpret_cast< const GetWindowAttributes::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetWindowAttributes::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetWindowAttributes::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "do-not-propagate-mask" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "backing-store", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->backing_store, GetWindowAttributes::backing_store_names ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "visual", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->visual ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "class", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->class_,
                        GetWindowAttributes::class_names, _IndexRange{ 1, 2 } ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "bit-gravity", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->bit_gravity ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "win-gravity", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->win_gravity ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "backing-planes", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->backing_planes ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "backing-pixel", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->backing_pixel ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "save-under", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->save_under ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "map-is-installed", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->map_is_installed ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "map-state", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->map_state, GetWindowAttributes::map_state_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "override-redirect", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->override_redirect ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "all-event-masks", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->all_event_masks ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "your-event-mask", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->your_event_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "do-not-propagate-mask", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->do_not_propagate_mask ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetGeometry >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetGeometry::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetGeometry;
    const GetGeometry::ReplyEncoding* encoding {
        reinterpret_cast< const GetGeometry::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetGeometry::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetGeometry::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "do-not-propagate-mask" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "depth", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->depth ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "root", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->root ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "width", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->width ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "height", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->height ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "border-width", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->border_width ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryTree >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryTree::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::QueryTree;
    const QueryTree::ReplyEncoding* encoding {
        reinterpret_cast< const QueryTree::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryTree::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(QueryTree::ReplyEncoding) +
                           ( sizeof(protocol::WINDOW) * encoding->n ) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const _ParsingOutputs children {
        _parseLISTof< protocol::WINDOW >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->n,
            _ROOT_WS.nested() ) };
    assert( encoding->reply_length == _alignedUnits( children.bytes_parsed ) );
    reply.bytes_parsed += children.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "root", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->root ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "parent", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->parent, QueryTree::parent_names ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n children", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "children", name_width, _ROOT_WS.equals,
        children.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::InternAtom >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::InternAtom::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::InternAtom;
    const InternAtom::ReplyEncoding* encoding {
        reinterpret_cast< const InternAtom::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( InternAtom::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(InternAtom::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );
    // TBD replies may be batched, so use encoding->sequence_number instead of conn->sequence
    if ( encoding->atom.data != protocol::atoms::NONE )
        _internStashedAtom( { conn->id, encoding->sequence_number }, encoding->atom );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "atom", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->atom, InternAtom::atom_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetAtomName >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetAtomName::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetAtomName;
    const GetAtomName::ReplyEncoding* encoding {
        reinterpret_cast< const GetAtomName::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetAtomName::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetAtomName::ReplyEncoding) +
                           _pad( sizeof(char) * encoding->n ) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const std::string_view name {
        reinterpret_cast< const char* >( data + reply.bytes_parsed ), encoding->n };
    reply.bytes_parsed += _pad(encoding->n);

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "name length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "name", name_width, _ROOT_WS.equals,
        name, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetProperty::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetProperty;
    const GetProperty::ReplyEncoding* encoding {
        reinterpret_cast< const GetProperty::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetProperty::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->format <= 32 && encoding->format % 8 == 0 );
    // followed by LISTofBYTE value
    const uint32_t value_sz { encoding->fmt_unit_ct * ( encoding->format / 8 ) };
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetProperty::ReplyEncoding) +
                           _pad( value_sz ) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );
    _ParsingOutputs value;
    if ( encoding->type.data == protocol::atoms::predefined::STRING ) {
        value.str = fmt::format(
            "{:?}", std::string_view{
                reinterpret_cast< const char* >( data + reply.bytes_parsed ), value_sz } );
        value.bytes_parsed = value_sz;
    } else {
        value = _parseLISTof< protocol::BYTE >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, value_sz,
            _ROOT_WS.nested( _Whitespace::SINGLELINE ) );
    }
    reply.bytes_parsed += _pad( value.bytes_parsed );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "format", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->format ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "type", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->type, GetProperty::reply_type_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "bytes-after", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->bytes_after ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n format units", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->fmt_unit_ct ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "value", name_width, _ROOT_WS.equals,
        value.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::ListProperties >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::ListProperties::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::ListProperties;
    const ListProperties::ReplyEncoding* encoding {
        reinterpret_cast< const ListProperties::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( ListProperties::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length == encoding->n );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(ListProperties::ReplyEncoding) +
                           _pad( sizeof(protocol::ATOM) * encoding->n ) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const _ParsingOutputs atoms {
        _parseLISTof< protocol::ATOM >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->n,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += _pad( atoms.bytes_parsed );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n atoms", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "atoms", name_width, _ROOT_WS.equals,
        atoms.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetSelectionOwner >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetSelectionOwner::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetSelectionOwner;
    const GetSelectionOwner::ReplyEncoding* encoding {
        reinterpret_cast< const GetSelectionOwner::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetSelectionOwner::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetSelectionOwner::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "owner", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->owner, GetSelectionOwner::owner_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GrabPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GrabPointer::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GrabPointer;
    const GrabPointer::ReplyEncoding* encoding {
        reinterpret_cast< const GrabPointer::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GrabPointer::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GrabPointer::ReplyEncoding)  -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "status", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->status, GrabPointer::status_names ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GrabKeyboard >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GrabKeyboard::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GrabKeyboard;
    const GrabKeyboard::ReplyEncoding* encoding {
        reinterpret_cast< const GrabKeyboard::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GrabKeyboard::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GrabKeyboard::ReplyEncoding)  -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "status", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->status, GrabKeyboard::status_names ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryPointer::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::QueryPointer;
    const QueryPointer::ReplyEncoding* encoding {
        reinterpret_cast< const QueryPointer::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryPointer::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(QueryPointer::ReplyEncoding)  -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "same-screen", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->same_screen ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "root", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->root ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "child", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->child, QueryPointer::child_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "root-x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->root_x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "root-y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->root_y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "win-x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->win_x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "win-y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->win_y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "mask", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->mask ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetMotionEvents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetMotionEvents::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetMotionEvents;
    const GetMotionEvents::ReplyEncoding* encoding {
        reinterpret_cast< const GetMotionEvents::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetMotionEvents::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetMotionEvents::ReplyEncoding) +
                           ( sizeof(GetMotionEvents::TIMECOORD) * encoding->n ) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const _ParsingOutputs events {
        _parseLISTof< GetMotionEvents::TIMECOORD >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->n,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += events.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n events", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "events", name_width, _ROOT_WS.equals,
        events.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::TranslateCoordinates >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::TranslateCoordinates::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::TranslateCoordinates;
    const TranslateCoordinates::ReplyEncoding* encoding {
        reinterpret_cast< const TranslateCoordinates::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( TranslateCoordinates::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length == 0 );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(TranslateCoordinates::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "same-screen", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->same_screen ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "child", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->child, TranslateCoordinates::child_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->dst_x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->dst_y ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetInputFocus >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetInputFocus::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetInputFocus;
    const GetInputFocus::ReplyEncoding* encoding {
        reinterpret_cast< const GetInputFocus::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetInputFocus::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetInputFocus::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "revert-to", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->revert_to, GetInputFocus::revert_to_names ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "focus", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->focus, GetInputFocus::focus_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryKeymap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryKeymap::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::QueryKeymap;
    const QueryKeymap::ReplyEncoding* encoding {
        reinterpret_cast< const QueryKeymap::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryKeymap::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(QueryKeymap::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const _ParsingOutputs keys {
        _parseLISTof< protocol::CARD8 >(
            encoding->keys, sizeof(encoding->keys), sizeof(encoding->keys),
            _ROOT_WS.nested( _Whitespace::SINGLELINE ) ) };
    // keys bytes already parsed as part of encoding

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "keys", name_width, _ROOT_WS.equals,
        keys.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryFont::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::QueryFont;
    const QueryFont::ReplyEncoding* encoding {
        reinterpret_cast< const QueryFont::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryFont::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof( QueryFont::ReplyEncoding ) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ +
                           ( sizeof( QueryFont::FONTPROP ) * encoding->n ) +
                           ( sizeof( QueryFont::CHARINFO ) * encoding->m ) ) );

    const _ParsingOutputs properties {
        _parseLISTof< QueryFont::FONTPROP >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->n,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += properties.bytes_parsed;
    const _ParsingOutputs char_infos {
        _parseLISTof< QueryFont::CHARINFO >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->m,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += char_infos.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "max-char-or-byte2" ) - 1 : 0 );
    reply.str = fmt::format(
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
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "min-bounds", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->min_bounds, _ROOT_WS.nested() ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "max-bounds", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->max_bounds, _ROOT_WS.nested() ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "min-char-or-byte2", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->min_char_or_byte2 ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "max-char-or-byte2", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->max_char_or_byte2 ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "default-char", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->default_char ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n properties", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "draw-direction", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->draw_direction, QueryFont::draw_direction_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "min-byte1", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->min_byte1 ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "max-byte1", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->max_byte1 ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "all-chars-exist", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->all_chars_exist ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "font-ascent", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->font_ascent ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "font-descent", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->font_descent ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "m char-infos", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->m ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "properties", name_width, _ROOT_WS.equals,
        properties.str, _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "char-infos", name_width, _ROOT_WS.equals,
        char_infos.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryTextExtents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    // TBD variable length
    assert( sz >= sizeof(
                protocol::requests::QueryTextExtents::ReplyEncoding ) );

    _ParsingOutputs reply;
    using protocol::requests::QueryTextExtents;
    const QueryTextExtents::ReplyEncoding* encoding {
        reinterpret_cast< const QueryTextExtents::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryTextExtents::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof( QueryTextExtents::ReplyEncoding ) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "draw-direction", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->draw_direction, QueryTextExtents::draw_direction_names ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "font-ascent", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->font_ascent ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "font-descent", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->font_descent ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "overall-ascent", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->overall_ascent ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "overall-descent", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->overall_descent ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "overall-width", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->overall_width ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "overall-left", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->overall_left ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "overall-right", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->overall_right ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::ListFonts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    // TBD variable length
    assert( sz >= sizeof(
                protocol::requests::ListFonts::ReplyEncoding ) );

    _ParsingOutputs reply;
    using protocol::requests::ListFonts;
    const ListFonts::ReplyEncoding* encoding {
        reinterpret_cast< const ListFonts::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( ListFonts::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );

    const _ParsingOutputs names {
        _parseLISTof< protocol::STR >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->name_ct,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += _pad(names.bytes_parsed);
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(ListFonts::ReplyEncoding) +
                           _pad(names.bytes_parsed) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n names", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->name_ct ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "names", name_width, _ROOT_WS.equals,
        names.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::ListFontsWithInfo >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::ListFontsWithInfo::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::ListFontsWithInfo;
    const ListFontsWithInfo::ReplyEncoding* encoding {
        reinterpret_cast< const ListFontsWithInfo::ReplyEncoding* >( data ) };
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );

    if ( encoding->n == ListFontsWithInfo::LAST_REPLY ) {
        const ListFontsWithInfo::FinalReplyEncoding* fr_encoding {
            reinterpret_cast< const ListFontsWithInfo::FinalReplyEncoding* >( data ) };
        reply.bytes_parsed += sizeof( ListFontsWithInfo::FinalReplyEncoding );
        assert( encoding->reply_length ==
                _alignedUnits( sizeof(ListFontsWithInfo::FinalReplyEncoding) -
                               protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );
        const uint32_t name_width (
            settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
        reply.str = fmt::format(
            "{{{}"
            "{}"
            "{}{: <{}}{}{}{}"
            "{}{}"
            "{}}}",
            _ROOT_WS.separator,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
                _formatInteger( fr_encoding->reply ), _ROOT_WS.separator ),
            _ROOT_WS.memb_indent, "last-reply", name_width, _ROOT_WS.equals,
            _formatInteger( fr_encoding->last_reply ), _ROOT_WS.separator,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
                _formatInteger( fr_encoding->sequence_number ), _ROOT_WS.separator ),
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
                _formatInteger( fr_encoding->reply_length ), _ROOT_WS.separator ),
            _ROOT_WS.encl_indent
            );
    } else {  // encoding->n != ListFontsWithInfo::LAST_REPLY
        reply.bytes_parsed += sizeof( ListFontsWithInfo::ReplyEncoding );

        const _ParsingOutputs properties {
            _parseLISTof< ListFontsWithInfo::FONTPROP >(
                data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->m,
                _ROOT_WS.nested() ) };
        reply.bytes_parsed += properties.bytes_parsed;
        const std::string_view name {
            reinterpret_cast< const char* >( data + reply.bytes_parsed ), encoding->n };
        reply.bytes_parsed += _pad( encoding->n );

        assert( encoding->reply_length ==
                _alignedUnits( sizeof(ListFontsWithInfo::ReplyEncoding) +
                               _pad(encoding->n) +
                               ( sizeof(ListFontsWithInfo::FONTPROP) * encoding->m ) -
                               protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );
        const uint32_t name_width (
            settings.multiline ? sizeof( "max-char-or-byte2" ) - 1 : 0 );
        reply.str = fmt::format(
            "{{{}"
            "{}{}{}{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
            "{}{: <{}}{}{:?}{}"
            "{}}}",
            _ROOT_WS.separator,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
                _formatInteger( encoding->reply ), _ROOT_WS.separator ),
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _ROOT_WS.memb_indent, "name len", name_width, _ROOT_WS.equals,
                _formatInteger( encoding->n ), _ROOT_WS.separator ),
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
                _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
                _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
            _ROOT_WS.memb_indent, "min-bounds", name_width, _ROOT_WS.equals,
            _formatProtocolType( encoding->min_bounds, _ROOT_WS.nested() ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "max-bounds", name_width, _ROOT_WS.equals,
            _formatProtocolType( encoding->max_bounds, _ROOT_WS.nested() ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "min-char-or-byte2", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->min_char_or_byte2 ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "max-char-or-byte2", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->max_char_or_byte2 ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "default-char", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->default_char ), _ROOT_WS.separator,
            !settings.verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                _ROOT_WS.memb_indent, "n properties", name_width, _ROOT_WS.equals,
                _formatInteger( encoding->m ), _ROOT_WS.separator ),
            _ROOT_WS.memb_indent, "draw-direction", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->draw_direction, ListFontsWithInfo::draw_direction_names ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "min-byte1", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->min_byte1 ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "max-byte1", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->max_byte1 ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "all-chars-exist", name_width, _ROOT_WS.equals,
            _formatProtocolType( encoding->all_chars_exist ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "font-ascent", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->font_ascent ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "font-descent", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->font_descent ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "replies-hint", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->replies_hint ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "properties", name_width, _ROOT_WS.equals,
            properties.str, _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "name", name_width, _ROOT_WS.equals,
            name, _ROOT_WS.separator,
            _ROOT_WS.encl_indent
            );
    }
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetFontPath >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetFontPath::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetFontPath;
    const GetFontPath::ReplyEncoding* encoding {
        reinterpret_cast< const GetFontPath::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetFontPath::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );

    const _ParsingOutputs path {
        _parseLISTof< protocol::STR >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->str_ct,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += _pad(path.bytes_parsed);
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetFontPath::ReplyEncoding) +
                           _pad(path.bytes_parsed) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n path elems", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->str_ct ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "path", name_width, _ROOT_WS.equals,
        path.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetImage >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetImage::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetImage;
    const GetImage::ReplyEncoding* encoding {
        reinterpret_cast< const GetImage::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetImage::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    // No use in an assert on reply_length, as here it is the only way to derive
    //   the length of LISTofBYTE `data`

    const size_t data_sz { encoding->reply_length * _ALIGN };
    const _ParsingOutputs data_ {
        _parseLISTof< protocol::BYTE >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            data_sz, _ROOT_WS.nested( _Whitespace::SINGLELINE ) ) };
    reply.bytes_parsed += _pad(data_.bytes_parsed);

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "depth", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->depth ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "visual", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->visual, GetImage::visual_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "data", name_width, _ROOT_WS.equals,
        data_.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::ListInstalledColormaps >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::ListInstalledColormaps::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::ListInstalledColormaps;
    const ListInstalledColormaps::ReplyEncoding* encoding {
        reinterpret_cast< const ListInstalledColormaps::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( ListInstalledColormaps::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(ListInstalledColormaps::ReplyEncoding) +
                           ( sizeof(protocol::COLORMAP) * encoding->n ) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const _ParsingOutputs cmaps {
        _parseLISTof< protocol::COLORMAP >(
            data + reply.bytes_parsed, sz- reply.bytes_parsed, encoding->n,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += cmaps.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n cmaps", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "cmaps", name_width, _ROOT_WS.equals,
        cmaps.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::AllocColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::AllocColor::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::AllocColor;
    const AllocColor::ReplyEncoding* encoding {
        reinterpret_cast< const AllocColor::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( AllocColor::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(AllocColor::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "red", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->red ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "green", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->green ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "blue", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->blue ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "pixel", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->pixel ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::AllocNamedColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::AllocNamedColor::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::AllocNamedColor;
    const AllocNamedColor::ReplyEncoding* encoding {
        reinterpret_cast< const AllocNamedColor::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( AllocNamedColor::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(AllocNamedColor::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "pixel", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->pixel ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "exact-red", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->exact_red ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "exact-green", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->exact_green ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "exact-blue", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->exact_blue ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "visual-red", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->visual_red ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "visual-green", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->visual_green ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "visual-blue", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->visual_blue ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::AllocColorCells >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::AllocColorCells::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::AllocColorCells;
    const AllocColorCells::ReplyEncoding* encoding {
        reinterpret_cast< const AllocColorCells::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( AllocColorCells::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(AllocColorCells::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->n,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += pixels.bytes_parsed;
    // TBD format as bitmasks
    const _ParsingOutputs masks {
        _parseLISTof< protocol::CARD32 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->m,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += pixels.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n pixels", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n masks", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->m ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "pixels", name_width, _ROOT_WS.equals,
        pixels.str, _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "masks", name_width, _ROOT_WS.equals,
        masks.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::AllocColorPlanes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::AllocColorPlanes::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::AllocColorPlanes;
    const AllocColorPlanes::ReplyEncoding* encoding {
        reinterpret_cast< const AllocColorPlanes::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( AllocColorPlanes::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(AllocColorPlanes::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->n,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += pixels.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n pixels", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "red-mask", name_width, _ROOT_WS.equals,
        _formatBitmask( encoding->red_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "green-mask", name_width, _ROOT_WS.equals,
        _formatBitmask( encoding->green_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "blue-mask", name_width, _ROOT_WS.equals,
        _formatBitmask( encoding->blue_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "pixels", name_width, _ROOT_WS.equals,
        pixels.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryColors::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::QueryColors;
    const QueryColors::ReplyEncoding* encoding {
        reinterpret_cast< const QueryColors::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryColors::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(QueryColors::ReplyEncoding) +
                           ( sizeof(QueryColors::RGB) * encoding->n ) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const _ParsingOutputs colors {
        _parseLISTof< QueryColors::RGB >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->n,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += colors.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n colors", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "colors", name_width, _ROOT_WS.equals,
        colors.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::LookupColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::LookupColor::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::LookupColor;
    const LookupColor::ReplyEncoding* encoding {
        reinterpret_cast< const LookupColor::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( LookupColor::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(LookupColor::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "exact-red", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->exact_red ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "exact-green", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->exact_green ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "exact-blue", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->exact_blue ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "visual-red", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->visual_red ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "visual-green", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->visual_green ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "visual-blue", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->visual_blue ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryBestSize >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryBestSize::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::QueryBestSize;
    const QueryBestSize::ReplyEncoding* encoding {
        reinterpret_cast< const QueryBestSize::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryBestSize::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(QueryBestSize::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "width", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->width ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "height", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->height ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

// TBD important for later implementation of extensions:
//   encoding->first_event: base event code
//   encoding->first_error: base error code
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryExtension >(
        Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::QueryExtension::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::QueryExtension;
    // not const to allow for .present spoofing
    QueryExtension::ReplyEncoding* encoding {
        reinterpret_cast< QueryExtension::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryExtension::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(QueryExtension::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );
    if ( settings.denyallextensions )
        encoding->present.data = int(false);

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "present", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->present ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "major-opcode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->major_opcode ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "first-event", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->first_event ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "first-error", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->first_error ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::ListExtensions >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::ListExtensions::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::ListExtensions;
    const ListExtensions::ReplyEncoding* encoding {
        reinterpret_cast< const ListExtensions::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( ListExtensions::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );

    const _ParsingOutputs names {
        _parseLISTof< protocol::STR >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->n,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += _pad(names.bytes_parsed);
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(ListExtensions::ReplyEncoding) +
                           _pad(names.bytes_parsed) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n names", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "names", name_width, _ROOT_WS.equals,
        names.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetKeyboardMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetKeyboardMapping::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetKeyboardMapping;
    const GetKeyboardMapping::ReplyEncoding* encoding {
        reinterpret_cast< const GetKeyboardMapping::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetKeyboardMapping::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );

    // n_keycodes should equal `count` from the request
    const uint32_t n_keycodes { encoding->reply_length / encoding->keysyms_per_keycode };
    const _ParsingOutputs keysyms {
        _parseLISTof< protocol::KEYSYM >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            n_keycodes * encoding->keysyms_per_keycode,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += keysyms.bytes_parsed;
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetKeyboardMapping::ReplyEncoding) +
                           keysyms.bytes_parsed -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "keysyms-per-keycode" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "keysyms-per-keycode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->keysyms_per_keycode ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "keysyms", name_width, _ROOT_WS.equals,
        keysyms.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetKeyboardControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetKeyboardControl::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetKeyboardControl;
    const GetKeyboardControl::ReplyEncoding* encoding {
        reinterpret_cast< const GetKeyboardControl::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetKeyboardControl::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetKeyboardControl::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const _ParsingOutputs auto_repeats {
        _parseLISTof< protocol::CARD8 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            sizeof(GetKeyboardControl::ReplyEncoding::auto_repeats),
            _ROOT_WS.nested( _Whitespace::SINGLELINE ) ) };
    // auto_repeats.bytes_parsed ignored, as auto-repeats is a fixed size array
    //   member of GetKeyboardControl::ReplyEncoding

    const uint32_t name_width (
        settings.multiline ? sizeof( "global-auto-repeat" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "global-auto-repeat", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->global_auto_repeat, GetKeyboardControl::global_auto_repeat_names ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "led-mask", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->led_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "key-click-percent", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->key_click_percent ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "bell-percent", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->bell_percent ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "bell-pitch", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->bell_pitch ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "bell-duration", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->bell_duration ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "auto-repeats", name_width, _ROOT_WS.equals,
        auto_repeats.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetPointerControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetPointerControl::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetPointerControl;
    const GetPointerControl::ReplyEncoding* encoding {
        reinterpret_cast< const GetPointerControl::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetPointerControl::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetPointerControl::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "acceleration-denominator" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "acceleration-numerator", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->acceleration_numerator ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "acceleration-denominator", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->acceleration_denominator ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "threshold", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->threshold ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetScreenSaver >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetScreenSaver::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetScreenSaver;
    const GetScreenSaver::ReplyEncoding* encoding {
        reinterpret_cast< const GetScreenSaver::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetScreenSaver::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetScreenSaver::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "timeout", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->timeout ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "interval", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->interval ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "prefer-blanking", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->prefer_blanking, GetScreenSaver::prefer_blanking_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "allow-exposures", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->allow_exposures, GetScreenSaver::allow_exposures_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::ListHosts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::ListHosts::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::ListHosts;
    const ListHosts::ReplyEncoding* encoding {
        reinterpret_cast< const ListHosts::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( ListHosts::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(ListHosts::ReplyEncoding) +
                           ( sizeof(protocol::HOST) * encoding->n ) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const _ParsingOutputs hosts {
        _parseLISTof< protocol::HOST >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->n,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += hosts.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->mode, ListHosts::mode_names ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n hosts", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "hosts", name_width, _ROOT_WS.equals,
        hosts.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::SetPointerMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::SetPointerMapping::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::SetPointerMapping;
    const SetPointerMapping::ReplyEncoding* encoding {
        reinterpret_cast< const SetPointerMapping::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( SetPointerMapping::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(SetPointerMapping::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "status", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->status, SetPointerMapping::status_names ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetPointerMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetPointerMapping::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetPointerMapping;
    const GetPointerMapping::ReplyEncoding* encoding {
        reinterpret_cast< const GetPointerMapping::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetPointerMapping::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetPointerMapping::ReplyEncoding) +
                           _pad(encoding->n) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const _ParsingOutputs map {
        _parseLISTof< protocol::CARD8 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->n,
            _ROOT_WS.nested( _Whitespace::SINGLELINE ) ) };
    reply.bytes_parsed += _pad(map.bytes_parsed);

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n map bytes", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "map", name_width, _ROOT_WS.equals,
        map.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::SetModifierMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::SetModifierMapping::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::SetModifierMapping;
    const SetModifierMapping::ReplyEncoding* encoding {
        reinterpret_cast< const SetModifierMapping::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( SetModifierMapping::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(SetModifierMapping::ReplyEncoding) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "sequence number" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "status", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->status, SetModifierMapping::status_names ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.encl_indent
        );
    return reply;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetModifierMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof(
                protocol::requests::GetModifierMapping::ReplyEncoding ) ); // TBD

    _ParsingOutputs reply;
    using protocol::requests::GetModifierMapping;
    const GetModifierMapping::ReplyEncoding* encoding {
        reinterpret_cast< const GetModifierMapping::ReplyEncoding* >( data ) };
    reply.bytes_parsed += sizeof( GetModifierMapping::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    const uint32_t n_keycodes {
        GetModifierMapping::MODIFIER_CT * encoding->keycodes_per_modifier };
    assert( encoding->reply_length ==
            _alignedUnits( sizeof(GetModifierMapping::ReplyEncoding) +
                           ( sizeof(protocol::KEYCODE) * n_keycodes ) -
                           protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) );

    const _ParsingOutputs keycodes {
        _parseLISTof< protocol::KEYCODE >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, n_keycodes,
            _ROOT_WS.nested() ) };
    reply.bytes_parsed += keycodes.bytes_parsed;

    const uint32_t name_width (
        settings.multiline ? sizeof( "keycodes-per-modifier" ) - 1 : 0 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "keycodes-per-modifier", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->keycodes_per_modifier ), _ROOT_WS.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "sequence number", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->sequence_number ), _ROOT_WS.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "reply length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->reply_length ), _ROOT_WS.separator ),
        _ROOT_WS.memb_indent, "keycodes", name_width, _ROOT_WS.equals,
        keycodes.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return reply;
}

// TBD default to const data, since mods are the exception
// TBD need data to be mutable for reply spoofing, eg QueryExtension
size_t X11ProtocolParser::_logReply(
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
    // TBD only before extension implementation
    assert( opcode >= protocol::requests::opcodes::MIN &&
            opcode <= protocol::requests::opcodes::MAX );

    _ParsingOutputs reply;
    switch ( opcode ) {
    case protocol::requests::opcodes::GETWINDOWATTRIBUTES:     //  3
        reply = _parseReply<
            protocol::requests::GetWindowAttributes >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETGEOMETRY:             // 14
        reply = _parseReply<
            protocol::requests::GetGeometry >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYTREE:               // 15
        reply = _parseReply<
            protocol::requests::QueryTree >( conn, data, sz );
        break;
    case protocol::requests::opcodes::INTERNATOM:              // 16
        reply = _parseReply<
            protocol::requests::InternAtom >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETATOMNAME:             // 17
        reply = _parseReply<
            protocol::requests::GetAtomName >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPROPERTY:             // 20
        reply = _parseReply<
            protocol::requests::GetProperty >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTPROPERTIES:          // 21
        reply = _parseReply<
            protocol::requests::ListProperties >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETSELECTIONOWNER:       // 23
        reply = _parseReply<
            protocol::requests::GetSelectionOwner >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABPOINTER:             // 26
        reply = _parseReply<
            protocol::requests::GrabPointer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABKEYBOARD:            // 31
        reply = _parseReply<
            protocol::requests::GrabKeyboard >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYPOINTER:            // 38
        reply = _parseReply<
            protocol::requests::QueryPointer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETMOTIONEVENTS:         // 39
        reply = _parseReply<
            protocol::requests::GetMotionEvents >( conn, data, sz );
        break;
    case protocol::requests::opcodes::TRANSLATECOORDINATES:    // 40
        reply = _parseReply<
            protocol::requests::TranslateCoordinates >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETINPUTFOCUS:           // 43
        reply = _parseReply<
            protocol::requests::GetInputFocus >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYKEYMAP:             // 44
        reply = _parseReply<
            protocol::requests::QueryKeymap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYFONT:               // 47
        reply = _parseReply<
            protocol::requests::QueryFont >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYTEXTEXTENTS:        // 48
        reply = _parseReply<
            protocol::requests::QueryTextExtents >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTFONTS:               // 49
        reply = _parseReply<
            protocol::requests::ListFonts >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTFONTSWITHINFO:       // 50
        reply = _parseReply<
            protocol::requests::ListFontsWithInfo >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETFONTPATH:             // 52
        reply = _parseReply<
            protocol::requests::GetFontPath >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETIMAGE:                // 73
        reply = _parseReply<
            protocol::requests::GetImage >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTINSTALLEDCOLORMAPS:  // 83
        reply = _parseReply<
            protocol::requests::ListInstalledColormaps >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLOR:              // 84
        reply = _parseReply<
            protocol::requests::AllocColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCNAMEDCOLOR:         // 85
        reply = _parseReply<
            protocol::requests::AllocNamedColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLORCELLS:         // 86
        reply = _parseReply<
            protocol::requests::AllocColorCells >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLORPLANES:        // 87
        reply = _parseReply<
            protocol::requests::AllocColorPlanes >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYCOLORS:             // 91
        reply = _parseReply<
            protocol::requests::QueryColors >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LOOKUPCOLOR:             // 92
        reply = _parseReply<
            protocol::requests::LookupColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYBESTSIZE:           // 97
        reply = _parseReply<
            protocol::requests::QueryBestSize >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYEXTENSION:          // 98
        reply = _parseReply<
            protocol::requests::QueryExtension >(
                conn, const_cast<uint8_t*>( data ), sz );
        break;
    case protocol::requests::opcodes::LISTEXTENSIONS:          // 99
        reply = _parseReply<
            protocol::requests::ListExtensions >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETKEYBOARDMAPPING:      // 101
        reply = _parseReply<
            protocol::requests::GetKeyboardMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETKEYBOARDCONTROL:      // 103
        reply = _parseReply<
            protocol::requests::GetKeyboardControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPOINTERCONTROL:       // 106
        reply = _parseReply<
            protocol::requests::GetPointerControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETSCREENSAVER:          // 108
        reply = _parseReply<
            protocol::requests::GetScreenSaver >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTHOSTS:               // 110
        reply = _parseReply<
            protocol::requests::ListHosts >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETPOINTERMAPPING:       // 116
        reply = _parseReply<
            protocol::requests::SetPointerMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPOINTERMAPPING:       // 117
        reply = _parseReply<
            protocol::requests::GetPointerMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETMODIFIERMAPPING:      // 118
        reply = _parseReply<
            protocol::requests::SetModifierMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETMODIFIERMAPPING:      // 119
        reply = _parseReply<
            protocol::requests::GetModifierMapping >( conn, data, sz );
        break;
    default:
        break;
    };
    fmt::println( "C{:03d}:{:04d}B:{}:S{:05d}: Reply to {}({}): {}",
                  conn->id, reply.bytes_parsed, _SERVER_TO_CLIENT,
                  header->sequence_number,
                  protocol::requests::names[ opcode ], opcode, reply.str );
    if ( opcode != protocol::requests::opcodes::LISTFONTSWITHINFO ||
         header->last_reply == protocol::requests::ListFontsWithInfo::LAST_REPLY ) {
        conn->unregisterRequest( header->sequence_number );
    }
    assert( reply.bytes_parsed >= protocol::requests::DEFAULT_REPLY_ENCODING_SZ );
    return reply.bytes_parsed;
}
