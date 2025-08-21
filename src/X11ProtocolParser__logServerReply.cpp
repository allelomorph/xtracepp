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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "do-not-propagate-mask" ) - 1 : 0 );
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
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "backing-store", name_width, _equals,
        _formatInteger( encoding->backing_store, GetWindowAttributes::backing_store_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "visual", name_width, _equals,
        _formatCommonType( encoding->visual ), _separator,
        memb_indent, "class", name_width, _equals,
        _formatInteger( encoding->class_, GetWindowAttributes::class_names, 2, 1 ), _separator,
        memb_indent, "bit-gravity", name_width, _equals,
        _formatCommonType( encoding->bit_gravity ), _separator,
        memb_indent, "win-gravity", name_width, _equals,
        _formatCommonType( encoding->win_gravity ), _separator,
        memb_indent, "backing-planes", name_width, _equals,
        _formatInteger( encoding->backing_planes ), _separator,
        memb_indent, "backing-pixel", name_width, _equals,
        _formatInteger( encoding->backing_pixel ), _separator,
        memb_indent, "save-under", name_width, _equals,
        _formatCommonType( encoding->save_under ), _separator,
        memb_indent, "map-is-installed", name_width, _equals,
        _formatCommonType( encoding->map_is_installed ), _separator,
        memb_indent, "map-state", name_width, _equals,
        _formatInteger( encoding->map_state, GetWindowAttributes::map_state_names ), _separator,
        memb_indent, "override-redirect", name_width, _equals,
        _formatCommonType( encoding->override_redirect ), _separator,
        memb_indent, "all-event-masks", name_width, _equals,
        _formatCommonType( encoding->all_event_masks ), _separator,
        memb_indent, "your-event-mask", name_width, _equals,
        _formatCommonType( encoding->your_event_mask ), _separator,
        memb_indent, "do-not-propagate-mask", name_width, _equals,
        _formatCommonType( encoding->do_not_propagate_mask ), _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "do-not-propagate-mask" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "depth", name_width, _equals,
        _formatInteger( encoding->depth ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "root", name_width, _equals,
        _formatCommonType( encoding->root ), _separator,
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
        struct_indent
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
        _parseLISTofWINDOW( data + bytes_parsed, encoding->n ) };
    assert( encoding->reply_length == children.bytes_parsed / _ALIGN );
    bytes_parsed += children.bytes_parsed;

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "root", name_width, _equals,
        _formatCommonType( encoding->root ), _separator,
        memb_indent, "parent", name_width, _equals,
        _formatCommonType( encoding->parent, QueryTree::parent_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n children", name_width, _equals,
            _formatInteger( encoding->n ), _separator ),
        memb_indent, "children", name_width, _equals,
        children.str, _separator,
        struct_indent
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

    if ( encoding->atom.data != protocol::atoms::NONE )
        conn->internStashedAtom( encoding->atom );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "atom", name_width, _equals,
        _formatCommonType( conn, encoding->atom, InternAtom::atom_names ), _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}\"{}\"{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "name length", name_width, _equals,
            _formatInteger( encoding->n ), _separator ),
        memb_indent, "name", name_width, _equals,
        name, _separator,
        struct_indent
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
        _parseLISTofBYTE( data + bytes_parsed, value_sz ) };
    bytes_parsed += _pad( value.bytes_parsed );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "format", name_width, _equals,
        _formatInteger( encoding->format ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "type", name_width, _equals,
        _formatCommonType( conn, encoding->type, GetProperty::reply_type_names ), _separator,
        memb_indent, "bytes-after", name_width, _equals,
        _formatInteger( encoding->bytes_after ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n format units", name_width, _equals,
            _formatInteger( encoding->fmt_unit_ct ), _separator ),
        memb_indent, "value", name_width, _equals,
        value.str, _separator,
        struct_indent
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
        _parseLISTofATOM( conn, data + bytes_parsed, encoding->n ) };
    bytes_parsed += _pad( atoms.bytes_parsed );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n atoms", name_width, _equals,
            _formatInteger( encoding->n ), _separator ),
        memb_indent, "atoms", name_width, _equals,
        atoms.str, _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "owner", name_width, _equals,
        _formatCommonType( encoding->owner, GetSelectionOwner::owner_names ), _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "status", name_width, _equals,
        _formatInteger( encoding->status, GrabPointer::status_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "status", name_width, _equals,
        _formatInteger( encoding->status, GrabKeyboard::status_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "same-screen", name_width, _equals,
        _formatCommonType( encoding->same_screen ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "root", name_width, _equals,
        _formatCommonType( encoding->root ), _separator,
        memb_indent, "child", name_width, _equals,
        _formatCommonType( encoding->child, QueryPointer::child_names ), _separator,
        memb_indent, "root-x", name_width, _equals,
        _formatInteger( encoding->root_x ), _separator,
        memb_indent, "root-y", name_width, _equals,
        _formatInteger( encoding->root_y ), _separator,
        memb_indent, "win-x", name_width, _equals,
        _formatInteger( encoding->win_x ), _separator,
        memb_indent, "win-y", name_width, _equals,
        _formatInteger( encoding->win_y ), _separator,
        memb_indent, "mask", name_width, _equals,
        _formatCommonType( encoding->mask ), _separator,
        struct_indent
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
        _parseLISTofTIMECOORD( data + bytes_parsed, encoding->n ) };
    bytes_parsed += events.bytes_parsed;

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n events", name_width, _equals,
            _formatInteger( encoding->n ), _separator ),
        memb_indent, "events", name_width, _equals,
        events.str, _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "same-screen", name_width, _equals,
        _formatCommonType( encoding->same_screen ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "child", name_width, _equals,
        _formatCommonType( encoding->child, TranslateCoordinates::child_names ), _separator,
        memb_indent, "dst-x", name_width, _equals,
        _formatInteger( encoding->dst_x ), _separator,
        memb_indent, "dst-y", name_width, _equals,
        _formatInteger( encoding->dst_y ), _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "revert-to", name_width, _equals,
        _formatInteger( encoding->revert_to, GetInputFocus::revert_to_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "focus", name_width, _equals,
        _formatCommonType( encoding->focus, GetInputFocus::focus_names ), _separator,
        struct_indent
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
        _parseLISTofCARD8( encoding->keys, sizeof(encoding->keys) ) };
    // keys bytes already parsed as part of encoding

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "keys", name_width, _equals,
        keys.str, _separator,
        struct_indent
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
        _parseLISTofFONTPROP( conn, data + bytes_parsed, encoding->n ) };
    bytes_parsed += properties.bytes_parsed;
    const _ParsingOutputs char_infos {
        _parseLISTofCHARINFO( data + bytes_parsed, encoding->m ) };
    bytes_parsed += char_infos.bytes_parsed;

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "max-char-or-byte2" ) - 1 : 0 );
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
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "min-bounds", name_width, _equals,
        _formatCommonType( encoding->min_bounds ), _separator,
        memb_indent, "max-bounds", name_width, _equals,
        _formatCommonType( encoding->max_bounds ), _separator,
        memb_indent, "min-char-or-byte2", name_width, _equals,
        _formatInteger( encoding->min_char_or_byte2 ), _separator,
        memb_indent, "max-char-or-byte2", name_width, _equals,
        _formatInteger( encoding->max_char_or_byte2 ), _separator,
        memb_indent, "default-char", name_width, _equals,
        _formatInteger( encoding->default_char ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n properties", name_width, _equals,
            _formatInteger( encoding->n ), _separator ),
        memb_indent, "draw-direction", name_width, _equals,
        _formatInteger( encoding->draw_direction, QueryFont::draw_direction_names ), _separator,
        memb_indent, "min-byte1", name_width, _equals,
        _formatInteger( encoding->min_byte1 ), _separator,
        memb_indent, "max-byte1", name_width, _equals,
        _formatInteger( encoding->max_byte1 ), _separator,
        memb_indent, "all-chars-exist", name_width, _equals,
        _formatCommonType( encoding->all_chars_exist ), _separator,
        memb_indent, "font-ascent", name_width, _equals,
        _formatInteger( encoding->font_ascent ), _separator,
        memb_indent, "font-descent", name_width, _equals,
        _formatInteger( encoding->font_descent ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "m char-infos", name_width, _equals,
            _formatInteger( encoding->m ), _separator ),
        memb_indent, "properties", name_width, _equals,
        properties.str, _separator,
        memb_indent, "char-infos", name_width, _equals,
        char_infos.str, _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "draw-direction", name_width, _equals,
        _formatInteger( encoding->draw_direction, QueryTextExtents::draw_direction_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "font-ascent", name_width, _equals,
        _formatInteger( encoding->font_ascent ), _separator,
        memb_indent, "font-descent", name_width, _equals,
        _formatInteger( encoding->font_descent ), _separator,
        memb_indent, "overall-ascent", name_width, _equals,
        _formatInteger( encoding->overall_ascent ), _separator,
        memb_indent, "overall-descent", name_width, _equals,
        _formatInteger( encoding->overall_descent ), _separator,
        memb_indent, "overall-width", name_width, _equals,
        _formatInteger( encoding->overall_width ), _separator,
        memb_indent, "overall-left", name_width, _equals,
        _formatInteger( encoding->overall_left ), _separator,
        memb_indent, "overall-right", name_width, _equals,
        _formatInteger( encoding->overall_right ), _separator,
        struct_indent
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
        _parseLISTofSTR( data + bytes_parsed, encoding->name_ct ) };
    bytes_parsed += _pad(names.bytes_parsed);
    assert( encoding->reply_length ==
            ( sizeof(ListFonts::ReplyEncoding) +
              _pad(names.bytes_parsed) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n names", name_width, _equals,
            _formatInteger( encoding->name_ct ), _separator ),
        memb_indent, "names", name_width, _equals,
        names.str, _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    if ( encoding->n == ListFontsWithInfo::LAST_REPLY ) {
        const ListFontsWithInfo::FinalReplyEncoding* fr_encoding {
            reinterpret_cast< const ListFontsWithInfo::FinalReplyEncoding* >( data ) };
        bytes_parsed += sizeof( ListFontsWithInfo::FinalReplyEncoding );
        assert( encoding->reply_length ==
                ( sizeof(ListFontsWithInfo::FinalReplyEncoding) -
                  protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );
        const uint32_t name_width (
            _multiline ? sizeof( "sequence number" ) - 1 : 0 );
        fmt::println(
            "{{{}"
            "{}"
            "{}{: <{}}{}{}{}"
            "{}{}"
            "{}}}",
            _separator,
            !_verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                memb_indent, "reply", name_width, _equals,
                _formatInteger( fr_encoding->reply ), _separator ),
            memb_indent, "last-reply", name_width, _equals,
            _formatInteger( fr_encoding->last_reply ), _separator,
            !_verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                memb_indent, "sequence number", name_width, _equals,
                _formatInteger( fr_encoding->sequence_number ), _separator ),
            !_verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                memb_indent, "reply length", name_width, _equals,
                _formatInteger( fr_encoding->reply_length ), _separator ),
            struct_indent
            );
    } else {  // encoding->n != ListFontsWithInfo::LAST_REPLY
        bytes_parsed += sizeof( ListFontsWithInfo::ReplyEncoding );

        const _ParsingOutputs properties {
            _parseLISTofFONTPROP( conn, data + bytes_parsed, encoding->m ) };
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
            _multiline ? sizeof( "max-char-or-byte2" ) - 1 : 0 );
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
            _separator,
            !_verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                memb_indent, "reply", name_width, _equals,
                _formatInteger( encoding->reply ), _separator ),
            !_verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                memb_indent, "name len", name_width, _equals,
                _formatInteger( encoding->n ), _separator ),
            !_verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                memb_indent, "sequence number", name_width, _equals,
                _formatInteger( encoding->sequence_number ), _separator ),
            !_verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                memb_indent, "reply length", name_width, _equals,
                _formatInteger( encoding->reply_length ), _separator ),
            memb_indent, "min-bounds", name_width, _equals,
            _formatCommonType( encoding->min_bounds ), _separator,
            memb_indent, "max-bounds", name_width, _equals,
            _formatCommonType( encoding->max_bounds ), _separator,
            memb_indent, "min-char-or-byte2", name_width, _equals,
            _formatInteger( encoding->min_char_or_byte2 ), _separator,
            memb_indent, "max-char-or-byte2", name_width, _equals,
            _formatInteger( encoding->max_char_or_byte2 ), _separator,
            memb_indent, "default-char", name_width, _equals,
            _formatInteger( encoding->default_char ), _separator,
            !_verbose ? "" :
            fmt::format(
                "{}{: <{}}{}{}{}",
                memb_indent, "n properties", name_width, _equals,
                _formatInteger( encoding->m ), _separator ),
            memb_indent, "draw-direction", name_width, _equals,
            _formatInteger( encoding->draw_direction, ListFontsWithInfo::draw_direction_names ), _separator,
            memb_indent, "min-byte1", name_width, _equals,
            _formatInteger( encoding->min_byte1 ), _separator,
            memb_indent, "max-byte1", name_width, _equals,
            _formatInteger( encoding->max_byte1 ), _separator,
            memb_indent, "all-chars-exist", name_width, _equals,
            _formatCommonType( encoding->all_chars_exist ), _separator,
            memb_indent, "font-ascent", name_width, _equals,
            _formatInteger( encoding->font_ascent ), _separator,
            memb_indent, "font-descent", name_width, _equals,
            _formatInteger( encoding->font_descent ), _separator,
            memb_indent, "replies-hint", name_width, _equals,
            _formatInteger( encoding->replies_hint ), _separator,
            memb_indent, "properties", name_width, _equals,
            properties.str, _separator,
            memb_indent, "name", name_width, _equals,
            name, _separator,
            struct_indent
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
        _parseLISTofSTR( data + bytes_parsed, encoding->str_ct ) };
    bytes_parsed += _pad(path.bytes_parsed);
    assert( encoding->reply_length ==
            ( sizeof(GetFontPath::ReplyEncoding) +
              _pad(path.bytes_parsed) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n path elems", name_width, _equals,
            _formatInteger( encoding->str_ct ), _separator ),
        memb_indent, "path", name_width, _equals,
        path.str, _separator,
        struct_indent
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
        _parseLISTofBYTE( data + bytes_parsed, encoding->reply_length * _ALIGN ) };
    bytes_parsed += _pad(data_.bytes_parsed);
    // TBD no assert on reply_length as in this case it is the source of the encoding
    //   suffix length (no way to know value of p but n will always be padded)

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "depth", name_width, _equals,
        _formatInteger( encoding->depth ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "visual", name_width, _equals,
        _formatCommonType( encoding->visual, GetImage::visual_names ), _separator,
        memb_indent, "data", name_width, _equals,
        data_.str, _separator,
        struct_indent
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
        _parseLISTofCOLORMAP( data + bytes_parsed, encoding->n ) };
    bytes_parsed += cmaps.bytes_parsed;

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n cmaps", name_width, _equals,
            _formatInteger( encoding->n ), _separator ),
        memb_indent, "cmaps", name_width, _equals,
        cmaps.str, _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "red", name_width, _equals,
        _formatInteger( encoding->red ), _separator,
        memb_indent, "green", name_width, _equals,
        _formatInteger( encoding->green ), _separator,
        memb_indent, "blue", name_width, _equals,
        _formatInteger( encoding->blue ), _separator,
        memb_indent, "pixel", name_width, _equals,
        _formatInteger( encoding->pixel ), _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "pixel", name_width, _equals,
        _formatInteger( encoding->pixel ), _separator,
        memb_indent, "exact-red", name_width, _equals,
        _formatInteger( encoding->exact_red ), _separator,
        memb_indent, "exact-green", name_width, _equals,
        _formatInteger( encoding->exact_green ), _separator,
        memb_indent, "exact-blue", name_width, _equals,
        _formatInteger( encoding->exact_blue ), _separator,
        memb_indent, "visual-red", name_width, _equals,
        _formatInteger( encoding->visual_red ), _separator,
        memb_indent, "visual-green", name_width, _equals,
        _formatInteger( encoding->visual_green ), _separator,
        memb_indent, "visual-blue", name_width, _equals,
        _formatInteger( encoding->visual_blue ), _separator,
        struct_indent
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
        _parseLISTofCARD32( data + bytes_parsed, encoding->n ) };
    bytes_parsed += pixels.bytes_parsed;
    // TBD format as bitmasks
    const _ParsingOutputs masks {
        _parseLISTofCARD32( data + bytes_parsed, encoding->m ) };
    bytes_parsed += pixels.bytes_parsed;

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n pixels", name_width, _equals,
            _formatInteger( encoding->n ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n masks", name_width, _equals,
            _formatInteger( encoding->m ), _separator ),
        memb_indent, "pixels", name_width, _equals,
        pixels.str, _separator,
        memb_indent, "masks", name_width, _equals,
        masks.str, _separator,
        struct_indent
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
        _parseLISTofCARD32( data + bytes_parsed, encoding->n ) };
    bytes_parsed += pixels.bytes_parsed;

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n pixels", name_width, _equals,
            _formatInteger( encoding->n ), _separator ),
        memb_indent, "red-mask", name_width, _equals,
        _formatBitmask( encoding->red_mask ), _separator,
        memb_indent, "green-mask", name_width, _equals,
        _formatBitmask( encoding->green_mask ), _separator,
        memb_indent, "blue-mask", name_width, _equals,
        _formatBitmask( encoding->blue_mask ), _separator,
        memb_indent, "pixels", name_width, _equals,
        pixels.str, _separator,
        struct_indent
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
        _parseLISTofRGB( data + bytes_parsed, encoding->n ) };
    bytes_parsed += colors.bytes_parsed;

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n colors", name_width, _equals,
            _formatInteger( encoding->n ), _separator ),
        memb_indent, "colors", name_width, _equals,
        colors.str, _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "exact-red", name_width, _equals,
        _formatInteger( encoding->exact_red ), _separator,
        memb_indent, "exact-green", name_width, _equals,
        _formatInteger( encoding->exact_green ), _separator,
        memb_indent, "exact-blue", name_width, _equals,
        _formatInteger( encoding->exact_blue ), _separator,
        memb_indent, "visual-red", name_width, _equals,
        _formatInteger( encoding->visual_red ), _separator,
        memb_indent, "visual-green", name_width, _equals,
        _formatInteger( encoding->visual_green ), _separator,
        memb_indent, "visual-blue", name_width, _equals,
        _formatInteger( encoding->visual_blue ), _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "width", name_width, _equals,
        _formatInteger( encoding->width ), _separator,
        memb_indent, "height", name_width, _equals,
        _formatInteger( encoding->height ), _separator,
        struct_indent
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
    const QueryExtension::ReplyEncoding* encoding {
        reinterpret_cast< const QueryExtension::ReplyEncoding* >( data ) };
    bytes_parsed += sizeof( QueryExtension::ReplyEncoding );
    assert( encoding->reply == protocol::requests::REPLY_PREFIX );
    assert( encoding->reply_length ==
            ( sizeof(QueryExtension::ReplyEncoding) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "present", name_width, _equals,
        _formatCommonType( encoding->present ), _separator,
        memb_indent, "major-opcode", name_width, _equals,
        _formatInteger( encoding->major_opcode ), _separator,
        memb_indent, "first-event", name_width, _equals,
        _formatInteger( encoding->first_event ), _separator,
        memb_indent, "first-error", name_width, _equals,
        _formatInteger( encoding->first_error ), _separator,
        struct_indent
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
        _parseLISTofSTR( data + bytes_parsed, encoding->n ) };
    bytes_parsed += _pad(names.bytes_parsed);
    assert( encoding->reply_length ==
            ( sizeof(ListExtensions::ReplyEncoding) +
              _pad(names.bytes_parsed) -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n names", name_width, _equals,
            _formatInteger( encoding->n ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "names", name_width, _equals,
        names.str, _separator,
        struct_indent
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
        _parseLISTofKEYSYM( data + bytes_parsed,
                            n_keycodes * encoding->keysyms_per_keycode ) };
    bytes_parsed += keysyms.bytes_parsed;
    assert( encoding->reply_length ==
            ( sizeof(GetKeyboardMapping::ReplyEncoding) +
              keysyms.bytes_parsed -
              protocol::requests::DEFAULT_REPLY_ENCODING_SZ ) / _ALIGN );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "keysyms-per-keycode" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "keysyms-per-keycode", name_width, _equals,
            _formatInteger( encoding->keysyms_per_keycode ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "keysyms", name_width, _equals,
        keysyms.str, _separator,
        struct_indent
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
        _parseLISTofCARD8( data + bytes_parsed,
                           sizeof(GetKeyboardControl::ReplyEncoding::auto_repeats) /
                           sizeof(protocol::CARD8) ) };
    // auto_repeats.bytes_parsed ignored, as auto-repeats is a fixed size array
    //   member of GetKeyboardControl::ReplyEncoding

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "global-auto-repeat" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "global-auto-repeat", name_width, _equals,
        _formatInteger( encoding->global_auto_repeat, GetKeyboardControl::global_auto_repeat_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "led-mask", name_width, _equals,
        _formatInteger( encoding->led_mask ), _separator,
        memb_indent, "key-click-percent", name_width, _equals,
        _formatInteger( encoding->key_click_percent ), _separator,
        memb_indent, "bell-percent", name_width, _equals,
        _formatInteger( encoding->bell_percent ), _separator,
        memb_indent, "bell-pitch", name_width, _equals,
        _formatInteger( encoding->bell_pitch ), _separator,
        memb_indent, "bell-duration", name_width, _equals,
        _formatInteger( encoding->bell_duration ), _separator,
        memb_indent, "auto-repeats", name_width, _equals,
        auto_repeats.str, _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "acceleration-denominator" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "acceleration-numerator", name_width, _equals,
        _formatInteger( encoding->acceleration_numerator ), _separator,
        memb_indent, "acceleration-denominator", name_width, _equals,
        _formatInteger( encoding->acceleration_denominator ), _separator,
        memb_indent, "threshold", name_width, _equals,
        _formatInteger( encoding->threshold ), _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "timeout", name_width, _equals,
        _formatInteger( encoding->timeout ), _separator,
        memb_indent, "interval", name_width, _equals,
        _formatInteger( encoding->interval ), _separator,
        memb_indent, "prefer-blanking", name_width, _equals,
        _formatInteger( encoding->prefer_blanking, GetScreenSaver::prefer_blanking_names ), _separator,
        memb_indent, "allow-exposures", name_width, _equals,
        _formatInteger( encoding->allow_exposures, GetScreenSaver::allow_exposures_names ), _separator,
        struct_indent
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
        _parseLISTofHOST( data + bytes_parsed, encoding->n ) };
    bytes_parsed += hosts.bytes_parsed;

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "mode", name_width, _equals,
        _formatInteger( encoding->mode, ListHosts::mode_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n hosts", name_width, _equals,
            _formatInteger( encoding->n ), _separator ),
        memb_indent, "hosts", name_width, _equals,
        hosts.str, _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "status", name_width, _equals,
        _formatInteger( encoding->status, SetPointerMapping::status_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        struct_indent
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
        _parseLISTofCARD8( data + bytes_parsed, encoding->n ) };
    bytes_parsed += _pad(map.bytes_parsed);

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n map bytes", name_width, _equals,
            _formatInteger( encoding->n ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "map", name_width, _equals,
        map.str, _separator,
        struct_indent
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

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "status", name_width, _equals,
        _formatInteger( encoding->status, SetModifierMapping::status_names ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        struct_indent
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
        _parseLISTofKEYCODE( data + bytes_parsed, n_keycodes ) };
    bytes_parsed += keycodes.bytes_parsed;

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "keycodes-per-modifier" ) - 1 : 0 );
    fmt::println(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply", name_width, _equals,
            _formatInteger( encoding->reply ), _separator ),
        memb_indent, "keycodes-per-modifier", name_width, _equals,
        _formatInteger( encoding->keycodes_per_modifier ), _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "reply length", name_width, _equals,
            _formatInteger( encoding->reply_length ), _separator ),
        memb_indent, "keycodes", name_width, _equals,
        keycodes.str, _separator,
        struct_indent
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
