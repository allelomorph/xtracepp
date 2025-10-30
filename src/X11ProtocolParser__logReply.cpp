#include <string_view>
#include <tuple>
#include <unordered_map>

#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/requests.hpp"
#include "protocol/atoms.hpp"


template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetWindowAttributes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetWindowAttributes;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetWindowAttributes::Reply::Encoding* encoding {
        reinterpret_cast< const GetWindowAttributes::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetWindowAttributes::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "do-not-propagate-mask" ) ) - 1 );
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
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "backing-store", memb_name_w, ws.equals,
        _formatVariable( encoding->header.backing_store,
                         { GetWindowAttributes::Reply::backing_store_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "visual", memb_name_w, ws.equals,
        _formatVariable( encoding->visual ), ws.separator,
        ws.memb_indent, "class", memb_name_w, ws.equals,
        _formatVariable(
            encoding->class_, { GetWindowAttributes::Reply::class_names,
                                _EnumNameRange::Bound::MIN,
                                GetWindowAttributes::Reply::CLASS_ENUM_MIN } ), ws.separator,
        ws.memb_indent, "bit-gravity", memb_name_w, ws.equals,
        _formatVariable( encoding->bit_gravity ), ws.separator,
        ws.memb_indent, "win-gravity", memb_name_w, ws.equals,
        _formatVariable( encoding->win_gravity ), ws.separator,
        ws.memb_indent, "backing-planes", memb_name_w, ws.equals,
        _formatVariable( encoding->backing_planes ), ws.separator,
        ws.memb_indent, "backing-pixel", memb_name_w, ws.equals,
        _formatVariable( encoding->backing_pixel ), ws.separator,
        ws.memb_indent, "save-under", memb_name_w, ws.equals,
        _formatVariable( encoding->save_under ), ws.separator,
        ws.memb_indent, "map-is-installed", memb_name_w, ws.equals,
        _formatVariable( encoding->map_is_installed ), ws.separator,
        ws.memb_indent, "map-state", memb_name_w, ws.equals,
        _formatVariable( encoding->map_state,
                         { GetWindowAttributes::Reply::map_state_names } ), ws.separator,
        ws.memb_indent, "override-redirect", memb_name_w, ws.equals,
        _formatVariable( encoding->override_redirect ), ws.separator,
        ws.memb_indent, "all-event-masks", memb_name_w, ws.equals,
        _formatVariable( encoding->all_event_masks ), ws.separator,
        ws.memb_indent, "your-event-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->your_event_mask ), ws.separator,
        ws.memb_indent, "do-not-propagate-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->do_not_propagate_mask ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetGeometry >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetGeometry;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetGeometry::Reply::Encoding* encoding {
        reinterpret_cast< const GetGeometry::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetGeometry::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "border-width" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatVariable( encoding->header.depth ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatVariable( encoding->root ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height ), ws.separator,
        ws.memb_indent, "border-width", memb_name_w, ws.equals,
        _formatVariable( encoding->border_width ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryTree >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryTree;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const QueryTree::Reply::Encoding* encoding {
        reinterpret_cast< const QueryTree::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryTree::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofWINDOW children
    const _ParsingOutputs children {
        _parseLISTof< protocol::WINDOW >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->children_ct, ws.nested() ) };
    reply.bytes_parsed += children.bytes_parsed;
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "children" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatVariable( encoding->root ), ws.separator,
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatVariable( encoding->parent,
                         { QueryTree::Reply::parent_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(WINDOWs in children)", memb_name_w, ws.equals,
            _formatVariable( encoding->children_ct ), ws.separator ),
        ws.memb_indent, "children", memb_name_w, ws.equals,
        children.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::InternAtom >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::InternAtom;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const InternAtom::Reply::Encoding* encoding {
        reinterpret_cast< const InternAtom::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( InternAtom::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    // Intern own copy of atom if not stored already, to reduce incidence of
    //   "(unknown atom)" in log
    if ( encoding->atom.data != protocol::atoms::NONE )
        _internStashedAtom( { conn->id, encoding->header.sequence_num },
                            encoding->atom );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "atom" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "atom", memb_name_w, ws.equals,
        _formatVariable( encoding->atom,
                         { InternAtom::Reply::atom_names } ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetAtomName >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetAtomName;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetAtomName::Reply::Encoding* encoding {
        reinterpret_cast< const GetAtomName::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetAtomName::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by STRING8 name
    const std::string_view name {
        reinterpret_cast< const char* >( data + reply.bytes_parsed ),
        encoding->name_len };
    reply.bytes_parsed += alignment.pad( encoding->name_len );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "name" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( encoding->name_len ), ws.separator ),
        ws.memb_indent, "name", memb_name_w, ws.equals,
        name, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetProperty;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetProperty::Reply::Encoding* encoding {
        reinterpret_cast< const GetProperty::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetProperty::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.format <= GetProperty::Reply::MAX_FORMAT &&
              encoding->header.format % GetProperty::Reply::FORMAT_FACTOR == 0 );
    // followed by LISTofBYTE value
    _ParsingOutputs value;
    const uint32_t value_sz {
        encoding->value_fmt_unit_len *
        ( encoding->header.format / GetProperty::Reply::FORMAT_FACTOR ) };
    if ( encoding->type.data == protocol::atoms::predefined::STRING ) {
        value.str = fmt::format( "{:?}", std::string_view{
                reinterpret_cast< const char* >( data + reply.bytes_parsed ),
                value_sz } );
        value.bytes_parsed = value_sz;
    } else {
        value = _parseLISTof< protocol::BYTE >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, value_sz,
            ws.nested( _Whitespace::FORCE_SINGLELINE ) );
    }
    reply.bytes_parsed += alignment.pad( value.bytes_parsed );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(value length (format units))" ) :
                              sizeof( "bytes-after" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "format", memb_name_w, ws.equals,
        _formatVariable( encoding->header.format ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "type", memb_name_w, ws.equals,
        _formatVariable( encoding->type,
                         { GetProperty::Reply::type_names } ), ws.separator,
        ws.memb_indent, "bytes-after", memb_name_w, ws.equals,
        _formatVariable( encoding->bytes_after ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(value length (format units))", memb_name_w, ws.equals,
            _formatVariable( encoding->value_fmt_unit_len ), ws.separator ),
        ws.memb_indent, "value", memb_name_w, ws.equals,
        value.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::ListProperties >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ListProperties;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const ListProperties::Reply::Encoding* encoding {
        reinterpret_cast< const ListProperties::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( ListProperties::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofATOM atoms
    const _ParsingOutputs atoms {
        _parseLISTof< protocol::ATOM >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->atoms_ct, ws.nested() ) };
    reply.bytes_parsed += alignment.pad( atoms.bytes_parsed );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "atoms" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(ATOMs in atoms)", memb_name_w, ws.equals,
            _formatVariable( encoding->atoms_ct ), ws.separator ),
        ws.memb_indent, "atoms", memb_name_w, ws.equals,
        atoms.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetSelectionOwner >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetSelectionOwner;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetSelectionOwner::Reply::Encoding* encoding {
        reinterpret_cast< const GetSelectionOwner::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetSelectionOwner::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "owner" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "owner", memb_name_w, ws.equals,
        _formatVariable( encoding->owner,
                         { GetSelectionOwner::Reply::owner_names } ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GrabPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GrabPointer;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GrabPointer::Reply::Encoding* encoding {
        reinterpret_cast< const GrabPointer::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GrabPointer::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "status" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "status", memb_name_w, ws.equals,
        _formatVariable( encoding->header.status,
                         { GrabPointer::Reply::status_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GrabKeyboard >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GrabKeyboard;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GrabKeyboard::Reply::Encoding* encoding {
        reinterpret_cast< const GrabKeyboard::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GrabKeyboard::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "status" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "status", memb_name_w, ws.equals,
        _formatVariable( encoding->header.status,
                         { GrabKeyboard::Reply::status_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryPointer;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const QueryPointer::Reply::Encoding* encoding {
        reinterpret_cast< const QueryPointer::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryPointer::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "same-screen" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "same-screen", memb_name_w, ws.equals,
        _formatVariable( encoding->header.same_screen ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatVariable( encoding->root ), ws.separator,
        ws.memb_indent, "child", memb_name_w, ws.equals,
        _formatVariable( encoding->child,
                         { QueryPointer::Reply::child_names } ), ws.separator,
        ws.memb_indent, "root-x", memb_name_w, ws.equals,
        _formatVariable( encoding->root_x ), ws.separator,
        ws.memb_indent, "root-y", memb_name_w, ws.equals,
        _formatVariable( encoding->root_y ), ws.separator,
        ws.memb_indent, "win-x", memb_name_w, ws.equals,
        _formatVariable( encoding->win_x ), ws.separator,
        ws.memb_indent, "win-y", memb_name_w, ws.equals,
        _formatVariable( encoding->win_y ), ws.separator,
        ws.memb_indent, "mask", memb_name_w, ws.equals,
        _formatVariable( encoding->mask ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetMotionEvents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetMotionEvents;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetMotionEvents::Reply::Encoding* encoding {
        reinterpret_cast< const GetMotionEvents::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetMotionEvents::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofTIMECOORD events
    const _ParsingOutputs events {
        _parseLISTof< GetMotionEvents::Reply::TIMECOORD >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->events_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += events.bytes_parsed;
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(TIMECOORDs in events)" ) :
                              sizeof( "events" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(TIMECOORDs in events)", memb_name_w, ws.equals,
            _formatVariable( encoding->events_ct ), ws.separator ),
        ws.memb_indent, "events", memb_name_w, ws.equals,
        events.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::TranslateCoordinates >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::TranslateCoordinates;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const TranslateCoordinates::Reply::Encoding* encoding {
        reinterpret_cast< const TranslateCoordinates::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( TranslateCoordinates::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "same-screen" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "same-screen", memb_name_w, ws.equals,
        _formatVariable( encoding->header.same_screen ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "child", memb_name_w, ws.equals,
        _formatVariable( encoding->child,
                         { TranslateCoordinates::Reply::child_names } ), ws.separator,
        ws.memb_indent, "dst-x", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_x ), ws.separator,
        ws.memb_indent, "dst-y", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_y ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetInputFocus >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetInputFocus;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetInputFocus::Reply::Encoding* encoding {
        reinterpret_cast< const GetInputFocus::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetInputFocus::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "revert-to" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "revert-to", memb_name_w, ws.equals,
        _formatVariable( encoding->header.revert_to,
                         { GetInputFocus::Reply::revert_to_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "focus", memb_name_w, ws.equals,
        _formatVariable( encoding->focus, { GetInputFocus::Reply::focus_names,
                                            _EnumNameRange::Bound::MAX,
                                            GetInputFocus::Reply::FOCUS_ENUM_MAX } ),
        ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryKeymap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryKeymap;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const QueryKeymap::Reply::Encoding* encoding {
        reinterpret_cast< const QueryKeymap::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryKeymap::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );
    // keys is included in QueryKeymap::Reply::Encoding
    const _ParsingOutputs keys {
        _parseLISTof< protocol::CARD8 >(
            encoding->keys, sizeof( encoding->keys ), sizeof( encoding->keys ),
            ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "keys" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "keys", memb_name_w, ws.equals,
        keys.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryFont;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const QueryFont::Reply::Encoding* encoding {
        reinterpret_cast< const QueryFont::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryFont::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofFONTPROP properties
    const _ParsingOutputs properties {
        _parseLISTof< QueryFont::Reply::FONTPROP >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->properties_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += properties.bytes_parsed;
    // followed by LISTofCHARINFO char-infos
    const _ParsingOutputs char_infos {
        _parseLISTof< QueryFont::Reply::CHARINFO >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->char_infos_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += char_infos.bytes_parsed;
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(FONTPROPs in properties)" ) :
                              sizeof( "min-char-or-byte2" ) ) - 1 );
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
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "min-bounds", memb_name_w, ws.equals,
        _formatVariable( encoding->min_bounds, ws.nested() ), ws.separator,
        ws.memb_indent, "max-bounds", memb_name_w, ws.equals,
        _formatVariable( encoding->max_bounds, ws.nested() ), ws.separator,
        ws.memb_indent, "min-char-or-byte2", memb_name_w, ws.equals,
        _formatVariable( encoding->min_char_or_byte2 ), ws.separator,
        ws.memb_indent, "max-char-or-byte2", memb_name_w, ws.equals,
        _formatVariable( encoding->max_char_or_byte2 ), ws.separator,
        ws.memb_indent, "default-char", memb_name_w, ws.equals,
        _formatVariable( encoding->default_char ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(FONTPROPs in properties)", memb_name_w, ws.equals,
            _formatVariable( encoding->properties_ct ), ws.separator ),
        ws.memb_indent, "draw-direction", memb_name_w, ws.equals,
        _formatVariable( encoding->draw_direction,
                         { QueryFont::Reply::draw_direction_names } ), ws.separator,
        ws.memb_indent, "min-byte1", memb_name_w, ws.equals,
        _formatVariable( encoding->min_byte1 ), ws.separator,
        ws.memb_indent, "max-byte1", memb_name_w, ws.equals,
        _formatVariable( encoding->max_byte1 ), ws.separator,
        ws.memb_indent, "all-chars-exist", memb_name_w, ws.equals,
        _formatVariable( encoding->all_chars_exist ), ws.separator,
        ws.memb_indent, "font-ascent", memb_name_w, ws.equals,
        _formatVariable( encoding->font_ascent ), ws.separator,
        ws.memb_indent, "font-descent", memb_name_w, ws.equals,
        _formatVariable( encoding->font_descent ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(CHARINFOs in char-infos)", memb_name_w, ws.equals,
            _formatVariable( encoding->char_infos_ct ), ws.separator ),
        ws.memb_indent, "properties", memb_name_w, ws.equals,
        properties.str, ws.separator,
        ws.memb_indent, "char-infos", memb_name_w, ws.equals,
        char_infos.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryTextExtents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryTextExtents;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const QueryTextExtents::Reply::Encoding* encoding {
        reinterpret_cast< const QueryTextExtents::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryTextExtents::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "overall-descent" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "draw-direction", memb_name_w, ws.equals,
        _formatVariable( encoding->header.draw_direction,
                         { QueryTextExtents::Reply::draw_direction_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "font-ascent", memb_name_w, ws.equals,
        _formatVariable( encoding->font_ascent ), ws.separator,
        ws.memb_indent, "font-descent", memb_name_w, ws.equals,
        _formatVariable( encoding->font_descent ), ws.separator,
        ws.memb_indent, "overall-ascent", memb_name_w, ws.equals,
        _formatVariable( encoding->overall_ascent ), ws.separator,
        ws.memb_indent, "overall-descent", memb_name_w, ws.equals,
        _formatVariable( encoding->overall_descent ), ws.separator,
        ws.memb_indent, "overall-width", memb_name_w, ws.equals,
        _formatVariable( encoding->overall_width ), ws.separator,
        ws.memb_indent, "overall-left", memb_name_w, ws.equals,
        _formatVariable( encoding->overall_left ), ws.separator,
        ws.memb_indent, "overall-right", memb_name_w, ws.equals,
        _formatVariable( encoding->overall_right ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::ListFonts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ListFonts;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const ListFonts::Reply::Encoding* encoding {
        reinterpret_cast< const ListFonts::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( ListFonts::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofSTR names
    const _ParsingOutputs names {
        _parseLISTof< protocol::STR >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->names_ct, ws.nested() ) };
    reply.bytes_parsed += alignment.pad( names.bytes_parsed );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "names" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(STRs in names)", memb_name_w, ws.equals,
            _formatVariable( encoding->names_ct ), ws.separator ),
        ws.memb_indent, "names", memb_name_w, ws.equals,
        names.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::ListFontsWithInfo >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ListFontsWithInfo;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const ListFontsWithInfo::Reply::Encoding* encoding {
        reinterpret_cast< const ListFontsWithInfo::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( ListFontsWithInfo::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );

    if ( encoding->header.last_reply == ListFontsWithInfo::Reply::LAST_REPLY ) {
        assert( encoding->header.extra_aligned_units ==
                  alignment.units( reply.bytes_parsed -
                                   protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

        const uint32_t memb_name_w (
            !ws.multiline ? 0 : ( settings.verbose ?
                                  sizeof( "(extra aligned units)" ) :
                                  sizeof( "last-reply" ) ) - 1 );
        reply.str = fmt::format(
            "{{{}"
            "{}"
            "{}{: <{}}{}{}{}"
            "{}{}"
            "{}}}",
            ws.separator,
            !settings.verbose ? "" : fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "reply", memb_name_w, ws.equals,
                _formatVariable( encoding->header.reply ), ws.separator ),
            ws.memb_indent, "last-reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.last_reply ), ws.separator,
            !settings.verbose ? "" : fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
                _formatVariable( encoding->header.sequence_num ), ws.separator ),
            !settings.verbose ? "" : fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
                _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
            ws.encl_indent
            );
        return reply;
    }

    // followed by LISTofFONTPROP properties
    const _ParsingOutputs properties {
        _parseLISTof< ListFontsWithInfo::Reply::FONTPROP >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->properties_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += properties.bytes_parsed;
    // followed by STRING8 name
    const std::string_view name {
        reinterpret_cast< const char* >( data + reply.bytes_parsed ),
        encoding->header.name_len };
    reply.bytes_parsed += alignment.pad( encoding->header.name_len );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(FONTPROPs in properties)" ) :
                              sizeof( "min-char-or-byte2" ) ) - 1 );
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
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.name_len ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "min-bounds", memb_name_w, ws.equals,
        _formatVariable( encoding->min_bounds, ws.nested() ), ws.separator,
        ws.memb_indent, "max-bounds", memb_name_w, ws.equals,
        _formatVariable( encoding->max_bounds, ws.nested() ), ws.separator,
        ws.memb_indent, "min-char-or-byte2", memb_name_w, ws.equals,
        _formatVariable( encoding->min_char_or_byte2 ), ws.separator,
        ws.memb_indent, "max-char-or-byte2", memb_name_w, ws.equals,
        _formatVariable( encoding->max_char_or_byte2 ), ws.separator,
        ws.memb_indent, "default-char", memb_name_w, ws.equals,
        _formatVariable( encoding->default_char ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(FONTPROPs in properties)", memb_name_w, ws.equals,
            _formatVariable( encoding->properties_ct ), ws.separator ),
        ws.memb_indent, "draw-direction", memb_name_w, ws.equals,
        _formatVariable( encoding->draw_direction,
                         { ListFontsWithInfo::Reply::draw_direction_names } ), ws.separator,
        ws.memb_indent, "min-byte1", memb_name_w, ws.equals,
        _formatVariable( encoding->min_byte1 ), ws.separator,
        ws.memb_indent, "max-byte1", memb_name_w, ws.equals,
        _formatVariable( encoding->max_byte1 ), ws.separator,
        ws.memb_indent, "all-chars-exist", memb_name_w, ws.equals,
        _formatVariable( encoding->all_chars_exist ), ws.separator,
        ws.memb_indent, "font-ascent", memb_name_w, ws.equals,
        _formatVariable( encoding->font_ascent ), ws.separator,
        ws.memb_indent, "font-descent", memb_name_w, ws.equals,
        _formatVariable( encoding->font_descent ), ws.separator,
        ws.memb_indent, "replies-hint", memb_name_w, ws.equals,
        _formatVariable( encoding->replies_hint ), ws.separator,
        ws.memb_indent, "properties", memb_name_w, ws.equals,
        properties.str, ws.separator,
        ws.memb_indent, "name", memb_name_w, ws.equals,
        name, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetFontPath >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetFontPath;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetFontPath::Reply::Encoding* encoding {
        reinterpret_cast< const GetFontPath::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetFontPath::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofSTR path
    const _ParsingOutputs path {
        _parseLISTof< protocol::STR >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->path_ct, ws.nested() ) };
    reply.bytes_parsed += alignment.pad( path.bytes_parsed );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "path" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(STRs in path)", memb_name_w, ws.equals,
            _formatVariable( encoding->path_ct ), ws.separator ),
        ws.memb_indent, "path", memb_name_w, ws.equals,
        path.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetImage >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetImage;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetImage::Reply::Encoding* encoding {
        reinterpret_cast< const GetImage::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetImage::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofBYTE data
    const _ParsingOutputs data_ {
        _parseLISTof< protocol::BYTE >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            // (no data length in encoding, must be inferred from eau)
            alignment.size( encoding->header.extra_aligned_units ),
            ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    reply.bytes_parsed += alignment.pad( data_.bytes_parsed );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "visual" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatVariable( encoding->header.depth ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "visual", memb_name_w, ws.equals,
        _formatVariable( encoding->visual,
                         { GetImage::Reply::visual_names } ), ws.separator,
        ws.memb_indent, "data", memb_name_w, ws.equals,
        data_.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::ListInstalledColormaps >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ListInstalledColormaps;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const ListInstalledColormaps::Reply::Encoding* encoding {
        reinterpret_cast< const ListInstalledColormaps::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( ListInstalledColormaps::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofCOLORMAP cmaps
    const _ParsingOutputs cmaps {
        _parseLISTof< protocol::COLORMAP >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->cmaps_ct, ws.nested() ) };
    reply.bytes_parsed += cmaps.bytes_parsed;
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "cmaps" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(COLORMAPs in cmaps)", memb_name_w, ws.equals,
            _formatVariable( encoding->cmaps_ct ), ws.separator ),
        ws.memb_indent, "cmaps", memb_name_w, ws.equals,
        cmaps.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::AllocColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::AllocColor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const AllocColor::Reply::Encoding* encoding {
        reinterpret_cast< const AllocColor::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( AllocColor::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "green" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "red", memb_name_w, ws.equals,
        _formatVariable( encoding->red ), ws.separator,
        ws.memb_indent, "green", memb_name_w, ws.equals,
        _formatVariable( encoding->green ), ws.separator,
        ws.memb_indent, "blue", memb_name_w, ws.equals,
        _formatVariable( encoding->blue ), ws.separator,
        ws.memb_indent, "pixel", memb_name_w, ws.equals,
        _formatVariable( encoding->pixel ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::AllocNamedColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::AllocNamedColor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const AllocNamedColor::Reply::Encoding* encoding {
        reinterpret_cast< const AllocNamedColor::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( AllocNamedColor::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "visual-green" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "pixel", memb_name_w, ws.equals,
        _formatVariable( encoding->pixel ), ws.separator,
        ws.memb_indent, "exact-red", memb_name_w, ws.equals,
        _formatVariable( encoding->exact_red ), ws.separator,
        ws.memb_indent, "exact-green", memb_name_w, ws.equals,
        _formatVariable( encoding->exact_green ), ws.separator,
        ws.memb_indent, "exact-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->exact_blue ), ws.separator,
        ws.memb_indent, "visual-red", memb_name_w, ws.equals,
        _formatVariable( encoding->visual_red ), ws.separator,
        ws.memb_indent, "visual-green", memb_name_w, ws.equals,
        _formatVariable( encoding->visual_green ), ws.separator,
        ws.memb_indent, "visual-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->visual_blue ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::AllocColorCells >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::AllocColorCells;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const AllocColorCells::Reply::Encoding* encoding {
        reinterpret_cast< const AllocColorCells::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( AllocColorCells::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofCARD32 pixels
    const _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->pixels_ct, ws.nested() ) };
    reply.bytes_parsed += pixels.bytes_parsed;
    // followed by LISTofCARD32 masks
    const _ParsingOutputs masks {
        _parseLISTof< protocol::CARD32 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->masks_ct, ws.nested(),
            _ValueTraits{ {}, _ValueTraits::BITMASK } ) };  // force hex
    reply.bytes_parsed += pixels.bytes_parsed;
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "pixels" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(CARD32 in pixels)", memb_name_w, ws.equals,
            _formatVariable( encoding->pixels_ct ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(CARD32 in masks)", memb_name_w, ws.equals,
            _formatVariable( encoding->masks_ct ), ws.separator ),
        ws.memb_indent, "pixels", memb_name_w, ws.equals,
        pixels.str, ws.separator,
        ws.memb_indent, "masks", memb_name_w, ws.equals,
        masks.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::AllocColorPlanes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::AllocColorPlanes;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const AllocColorPlanes::Reply::Encoding* encoding {
        reinterpret_cast< const AllocColorPlanes::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( AllocColorPlanes::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofCARD32 pixels
    const _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->pixels_ct, ws.nested() ) };
    reply.bytes_parsed += pixels.bytes_parsed;
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "green-mask" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(CARD32 in pixels)", memb_name_w, ws.equals,
            _formatVariable( encoding->pixels_ct ), ws.separator ),
        ws.memb_indent, "red-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->red_mask,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "green-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->green_mask,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "blue-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->blue_mask,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "pixels", memb_name_w, ws.equals,
        pixels.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryColors;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const QueryColors::Reply::Encoding* encoding {
        reinterpret_cast< const QueryColors::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryColors::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofRGB colors
    const _ParsingOutputs colors {
        _parseLISTof< QueryColors::Reply::RGB >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->colors_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += colors.bytes_parsed;
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "colors" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(RGBs in colors)", memb_name_w, ws.equals,
            _formatVariable( encoding->colors_ct ), ws.separator ),
        ws.memb_indent, "colors", memb_name_w, ws.equals,
        colors.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::LookupColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::LookupColor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const LookupColor::Reply::Encoding* encoding {
        reinterpret_cast< const LookupColor::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( LookupColor::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "visual-green" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "exact-red", memb_name_w, ws.equals,
        _formatVariable( encoding->exact_red ), ws.separator,
        ws.memb_indent, "exact-green", memb_name_w, ws.equals,
        _formatVariable( encoding->exact_green ), ws.separator,
        ws.memb_indent, "exact-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->exact_blue ), ws.separator,
        ws.memb_indent, "visual-red", memb_name_w, ws.equals,
        _formatVariable( encoding->visual_red ), ws.separator,
        ws.memb_indent, "visual-green", memb_name_w, ws.equals,
        _formatVariable( encoding->visual_green ), ws.separator,
        ws.memb_indent, "visual-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->visual_blue ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryBestSize >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryBestSize;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const QueryBestSize::Reply::Encoding* encoding {
        reinterpret_cast< const QueryBestSize::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryBestSize::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "height" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

// TBD important for later implementation of extensions:
//   encoding->first_event: base event code
//   encoding->first_error: base error code
template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryExtension >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryExtension;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const QueryExtension::Reply::Encoding* encoding {
        reinterpret_cast< const QueryExtension::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryExtension::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    if ( settings.denyallextensions ) {
        const_cast< QueryExtension::Reply::Encoding* >(
            reinterpret_cast< const QueryExtension::Reply::Encoding* >(
                data ) )->present.data = false;
    }

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "major-opcode" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "present", memb_name_w, ws.equals,
        _formatVariable( encoding->present ), ws.separator,
        ws.memb_indent, "major-opcode", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode ), ws.separator,
        ws.memb_indent, "first-event", memb_name_w, ws.equals,
        _formatVariable( encoding->first_event ), ws.separator,
        ws.memb_indent, "first-error", memb_name_w, ws.equals,
        _formatVariable( encoding->first_error ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::ListExtensions >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ListExtensions;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const ListExtensions::Reply::Encoding* encoding {
        reinterpret_cast< const ListExtensions::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( ListExtensions::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofSTR names
    const _ParsingOutputs names {
        _parseLISTof< protocol::STR >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->header.names_ct, ws.nested() ) };
    reply.bytes_parsed += alignment.pad( names.bytes_parsed );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "names" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(STRs in names)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.names_ct ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "names", memb_name_w, ws.equals,
        names.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetKeyboardMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetKeyboardMapping;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetKeyboardMapping::Reply::Encoding* encoding {
        reinterpret_cast< const GetKeyboardMapping::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetKeyboardMapping::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofKEYSYM keysyms
    // (keycode count is field in request, but here must be inferred)
    const uint32_t keysym_ct (
        alignment.size( encoding->header.extra_aligned_units ) /
        sizeof( protocol::KEYSYM ) );
    const _ParsingOutputs keysyms {
        _parseLISTof< protocol::KEYSYM >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            keysym_ct, ws.nested() ) };
    reply.bytes_parsed += keysyms.bytes_parsed;
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "keysyms" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "keysyms-per-keycode", memb_name_w, ws.equals,
            _formatVariable( encoding->header.keysyms_per_keycode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "keysyms", memb_name_w, ws.equals,
        keysyms.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetKeyboardControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetKeyboardControl;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetKeyboardControl::Reply::Encoding* encoding {
        reinterpret_cast< const GetKeyboardControl::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetKeyboardControl::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // Encoding includes fixed size LISTofCARD8 auto-repeats
    const _ParsingOutputs auto_repeats {
        _parseLISTof< protocol::CARD8 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            GetKeyboardControl::Reply::AUTO_REPEATS_SZ,
            ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "global-auto-repeat" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "global-auto-repeat", memb_name_w, ws.equals,
        _formatVariable(
            encoding->header.global_auto_repeat,
            { GetKeyboardControl::Reply::global_auto_repeat_names,
              _EnumNameRange::Bound::MAX,
              GetKeyboardControl::Reply::GLOBAL_AUTO_REPREAT_ENUM_MAX } ),
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "led-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->led_mask,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "key-click-percent", memb_name_w, ws.equals,
        _formatVariable( encoding->key_click_percent ), ws.separator,
        ws.memb_indent, "bell-percent", memb_name_w, ws.equals,
        _formatVariable( encoding->bell_percent ), ws.separator,
        ws.memb_indent, "bell-pitch", memb_name_w, ws.equals,
        _formatVariable( encoding->bell_pitch ), ws.separator,
        ws.memb_indent, "bell-duration", memb_name_w, ws.equals,
        _formatVariable( encoding->bell_duration ), ws.separator,
        ws.memb_indent, "auto-repeats", memb_name_w, ws.equals,
        auto_repeats.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetPointerControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetPointerControl;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetPointerControl::Reply::Encoding* encoding {
        reinterpret_cast< const GetPointerControl::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetPointerControl::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "acceleration-denominator" ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "acceleration-numerator", memb_name_w, ws.equals,
        _formatVariable( encoding->acceleration_numerator ), ws.separator,
        ws.memb_indent, "acceleration-denominator", memb_name_w, ws.equals,
        _formatVariable( encoding->acceleration_denominator ), ws.separator,
        ws.memb_indent, "threshold", memb_name_w, ws.equals,
        _formatVariable( encoding->threshold ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetScreenSaver >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetScreenSaver;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetScreenSaver::Reply::Encoding* encoding {
        reinterpret_cast< const GetScreenSaver::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetScreenSaver::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "prefer-blanking" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "timeout", memb_name_w, ws.equals,
        _formatVariable( encoding->timeout ), ws.separator,
        ws.memb_indent, "interval", memb_name_w, ws.equals,
        _formatVariable( encoding->interval ), ws.separator,
        ws.memb_indent, "prefer-blanking", memb_name_w, ws.equals,
        _formatVariable( encoding->prefer_blanking,
                         { GetScreenSaver::Reply::prefer_blanking_names,
                           _EnumNameRange::Bound::MAX,
                           GetScreenSaver::Reply::PREFER_BLANKING_ENUM_MAX } ),
        ws.separator,
        ws.memb_indent, "allow-exposures", memb_name_w, ws.equals,
        _formatVariable( encoding->allow_exposures,
                         { GetScreenSaver::Reply::allow_exposures_names,
                           _EnumNameRange::Bound::MAX,
                           GetScreenSaver::Reply::ALLOW_EXPOSURES_ENUM_MAX } ),
        ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::ListHosts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ListHosts;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const ListHosts::Reply::Encoding* encoding {
        reinterpret_cast< const ListHosts::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( ListHosts::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofHOST hosts
    const _ParsingOutputs hosts {
        _parseLISTof< protocol::HOST >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->hosts_ct, ws.nested() ) };
    reply.bytes_parsed += hosts.bytes_parsed;
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "hosts" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( encoding->header.mode,
                         { ListHosts::Reply::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(HOSTs in hosts)", memb_name_w, ws.equals,
            _formatVariable( encoding->hosts_ct ), ws.separator ),
        ws.memb_indent, "hosts", memb_name_w, ws.equals,
        hosts.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::SetPointerMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SetPointerMapping;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const SetPointerMapping::Reply::Encoding* encoding {
        reinterpret_cast< const SetPointerMapping::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( SetPointerMapping::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "status" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "status", memb_name_w, ws.equals,
        _formatVariable( encoding->header.status,
                         { SetPointerMapping::Reply::status_names,
                           _EnumNameRange::Bound::MAX,
                           SetPointerMapping::Reply::STATUS_ENUM_MAX } ),
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetPointerMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetPointerMapping;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetPointerMapping::Reply::Encoding* encoding {
        reinterpret_cast< const GetPointerMapping::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetPointerMapping::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofCARD8 map
    const _ParsingOutputs map {
        _parseLISTof< protocol::CARD8 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            encoding->header.map_len, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    reply.bytes_parsed += alignment.pad( map.bytes_parsed );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "map" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(map length)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.map_len ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "map", memb_name_w, ws.equals,
        map.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::SetModifierMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SetModifierMapping;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const SetModifierMapping::Reply::Encoding* encoding {
        reinterpret_cast< const SetModifierMapping::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( SetModifierMapping::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "status" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "status", memb_name_w, ws.equals,
        _formatVariable( encoding->header.status,
                         { SetModifierMapping::Reply::status_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetModifierMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetModifierMapping;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const GetModifierMapping::Reply::Encoding* encoding {
        reinterpret_cast< const GetModifierMapping::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetModifierMapping::Reply::Encoding );
    assert( encoding->header.reply == protocol::requests::Reply::REPLY );
    // followed by LISTofKEYCODE keycodes
    const uint32_t keycode_ct {
        GetModifierMapping::Reply::MODIFIER_CT *
        encoding->header.keycodes_per_modifier };
    const _ParsingOutputs keycodes {
        _parseLISTof< protocol::KEYCODE >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            keycode_ct, ws.nested() ) };
    reply.bytes_parsed += keycodes.bytes_parsed;
    assert( encoding->header.extra_aligned_units ==
              alignment.units( reply.bytes_parsed -
                               protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(extra aligned units)" ) :
                              sizeof( "keycodes-per-modifier" ) ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply ), ws.separator ),
        ws.memb_indent, "keycodes-per-modifier", memb_name_w, ws.equals,
        _formatVariable( encoding->header.keycodes_per_modifier ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units ), ws.separator ),
        ws.memb_indent, "keycodes", memb_name_w, ws.equals,
        keycodes.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}

size_t X11ProtocolParser::_logReply(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    // get request opcode via sequence number
    using protocol::requests::Reply;
    const Reply::Header* header {
        reinterpret_cast< const Reply::Header* >( data ) };
    assert( header->reply == protocol::requests::Reply::REPLY );
    const uint8_t opcode { conn->lookupRequest( header->sequence_num ) };
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
            protocol::requests::QueryExtension >( conn, data, sz );
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
    fmt::println( settings.log_fs,
                  "C{:03d}:{:04d}B:{}:S{:05d}: Reply to {}({}): {}",
                  conn->id, reply.bytes_parsed, SERVER_TO_CLIENT,
                  header->sequence_num,
                  protocol::requests::names[ opcode ], opcode, reply.str );
    // ListFontsWithInfo presents edge case as it issues a series of replies
    using protocol::requests::ListFontsWithInfo;
    if ( opcode != protocol::requests::opcodes::LISTFONTSWITHINFO ||
         reinterpret_cast< const ListFontsWithInfo::Reply::Header* >(
             data )->last_reply == ListFontsWithInfo::Reply::LAST_REPLY ) {
        conn->unregisterRequest( header->sequence_num );
    }
    assert( reply.bytes_parsed >=
              protocol::requests::Reply::DEFAULT_ENCODING_SZ );
    return reply.bytes_parsed;
}
