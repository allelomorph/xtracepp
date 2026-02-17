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
X11ProtocolParser::_parseReply< X11ProtocolParser::_UnknownRequest >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _UnknownRequest::Reply::Encoding* encoding {
        reinterpret_cast< const _UnknownRequest::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( _UnknownRequest::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // potentially followed by unparsable suffix
    const uint32_t extra_aligned_units {
        _ordered( encoding->header.extra_aligned_units, byteswap ) };
    reply.bytes_parsed += alignment.size( extra_aligned_units );
    assert( extra_aligned_units ==
            alignment.units( reply.bytes_parsed -
                             protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 :
        !settings.verbose ? sizeof( "(unparseable suffix)" ) - 1 :
                            sizeof( "(extra aligned units)" ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}({} bytes){}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "(unparseable suffix)", memb_name_w, ws.equals,
        alignment.size( extra_aligned_units ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetWindowAttributes >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::GetWindowAttributes;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const GetWindowAttributes::Reply::Encoding* encoding {
        reinterpret_cast< const GetWindowAttributes::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetWindowAttributes::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "backing-store", memb_name_w, ws.equals,
        _formatVariable( encoding->header.backing_store, byteswap,
                         { GetWindowAttributes::Reply::backing_store_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "visual", memb_name_w, ws.equals,
        _formatVariable( encoding->visual, byteswap ), ws.separator,
        ws.memb_indent, "class", memb_name_w, ws.equals,
        _formatVariable( encoding->class_, byteswap,
                         { GetWindowAttributes::Reply::class_names,
                           _EnumNameRange::Bound::MIN,
                           GetWindowAttributes::Reply::CLASS_ENUM_MIN } ), ws.separator,
        ws.memb_indent, "bit-gravity", memb_name_w, ws.equals,
        _formatVariable( encoding->bit_gravity, byteswap ), ws.separator,
        ws.memb_indent, "win-gravity", memb_name_w, ws.equals,
        _formatVariable( encoding->win_gravity, byteswap ), ws.separator,
        ws.memb_indent, "backing-planes", memb_name_w, ws.equals,
        _formatVariable( encoding->backing_planes, byteswap ), ws.separator,
        ws.memb_indent, "backing-pixel", memb_name_w, ws.equals,
        _formatVariable( encoding->backing_pixel, byteswap ), ws.separator,
        ws.memb_indent, "save-under", memb_name_w, ws.equals,
        _formatVariable( encoding->save_under, byteswap ), ws.separator,
        ws.memb_indent, "map-is-installed", memb_name_w, ws.equals,
        _formatVariable( encoding->map_is_installed, byteswap ), ws.separator,
        ws.memb_indent, "map-state", memb_name_w, ws.equals,
        _formatVariable( encoding->map_state, byteswap,
                         { GetWindowAttributes::Reply::map_state_names } ), ws.separator,
        ws.memb_indent, "override-redirect", memb_name_w, ws.equals,
        _formatVariable( encoding->override_redirect, byteswap ), ws.separator,
        ws.memb_indent, "all-event-masks", memb_name_w, ws.equals,
        _formatVariable( encoding->all_event_masks, byteswap ), ws.separator,
        ws.memb_indent, "your-event-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->your_event_mask, byteswap ), ws.separator,
        ws.memb_indent, "do-not-propagate-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->do_not_propagate_mask, byteswap ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetGeometry >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::GetGeometry;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const GetGeometry::Reply::Encoding* encoding {
        reinterpret_cast< const GetGeometry::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetGeometry::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatVariable( encoding->header.depth, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatVariable( encoding->root, byteswap ), ws.separator,
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
    const bool byteswap { conn->byteswap };
    const QueryTree::Reply::Encoding* encoding {
        reinterpret_cast< const QueryTree::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryTree::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofWINDOW children
    const _ParsingOutputs children {
        _parseLISTof< protocol::WINDOW >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->children_ct, byteswap ),
            byteswap, ws.nested() ) };
    reply.bytes_parsed += children.bytes_parsed;
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatVariable( encoding->root, byteswap ), ws.separator,
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatVariable( encoding->parent, byteswap,
                         { QueryTree::Reply::parent_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(WINDOWs in children)", memb_name_w, ws.equals,
            _formatVariable( encoding->children_ct, byteswap ), ws.separator ),
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
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::InternAtom;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const InternAtom::Reply::Encoding* encoding {
        reinterpret_cast< const InternAtom::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( InternAtom::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
            alignment.units( reply.bytes_parsed -
                             protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );

    // Intern own copy of atom if not stored already, to reduce incidence of
    //   "(unknown atom)" in log
    if ( const auto atom { _ordered( encoding->atom.data, byteswap ) };
         atom != protocol::atoms::NONE ) {
        const std::string_view atom_str {
            _unstashString(
                conn->id, _ordered( encoding->header.sequence_num, byteswap ) ) };
        // not sure if server will reuse ATOMs, so we allow for it in our
        //   mirroring of internments
        _interned_atoms[ atom ] = atom_str;
    }

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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "atom", memb_name_w, ws.equals,
        _formatVariable( encoding->atom, byteswap,
                         { InternAtom::Reply::atom_names } ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetAtomName >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::GetAtomName;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const GetAtomName::Reply::Encoding* encoding {
        reinterpret_cast< const GetAtomName::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetAtomName::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by STRING8 name
    const auto name_len { _ordered( encoding->name_len, byteswap ) };
    const std::string_view name {
        reinterpret_cast< const char* >( data + reply.bytes_parsed ),
        name_len };
    reply.bytes_parsed += alignment.pad( name_len );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( encoding->name_len, byteswap ), ws.separator ),
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
    const bool byteswap { conn->byteswap };
    const GetProperty::Reply::Encoding* encoding {
        reinterpret_cast< const GetProperty::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetProperty::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    const auto format { _ordered( encoding->header.format, byteswap ) };
    assert( format <= GetProperty::Reply::MAX_FORMAT &&
            format % GetProperty::Reply::FORMAT_FACTOR == 0 );
    // followed by LISTofBYTE value
    _ParsingOutputs value;
    const uint32_t value_sz {
        _ordered( encoding->value_fmt_unit_len, byteswap ) *
        ( format / GetProperty::Reply::FORMAT_FACTOR ) };
    if ( _ordered( encoding->type.data, byteswap ) ==
         protocol::atoms::predefined::STRING ) {
        value.str = fmt::format( "{:?}", std::string_view{
                reinterpret_cast< const char* >( data + reply.bytes_parsed ),
                value_sz } );
        value.bytes_parsed = value_sz;
    } else {
        value = _parseLISTof< protocol::BYTE >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, value_sz,
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) );
    }
    reply.bytes_parsed += alignment.pad( value.bytes_parsed );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "format", memb_name_w, ws.equals,
        _formatVariable( encoding->header.format, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "type", memb_name_w, ws.equals,
        _formatVariable( encoding->type, byteswap,
                         { GetProperty::Reply::type_names } ), ws.separator,
        ws.memb_indent, "bytes-after", memb_name_w, ws.equals,
        _formatVariable( encoding->bytes_after, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(value length (format units))", memb_name_w, ws.equals,
            _formatVariable( encoding->value_fmt_unit_len, byteswap ), ws.separator ),
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
    const bool byteswap { conn->byteswap };
    const ListProperties::Reply::Encoding* encoding {
        reinterpret_cast< const ListProperties::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( ListProperties::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofATOM atoms
    const _ParsingOutputs atoms {
        _parseLISTof< protocol::ATOM >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->atoms_ct,
            byteswap, ws.nested() ) };
    reply.bytes_parsed += alignment.pad( atoms.bytes_parsed );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(ATOMs in atoms)", memb_name_w, ws.equals,
            _formatVariable( encoding->atoms_ct, byteswap ), ws.separator ),
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
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::GetSelectionOwner;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const GetSelectionOwner::Reply::Encoding* encoding {
        reinterpret_cast< const GetSelectionOwner::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetSelectionOwner::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "owner", memb_name_w, ws.equals,
        _formatVariable( encoding->owner, byteswap,
                         { GetSelectionOwner::Reply::owner_names } ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GrabPointer >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::GrabPointer;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const GrabPointer::Reply::Encoding* encoding {
        reinterpret_cast< const GrabPointer::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GrabPointer::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "status", memb_name_w, ws.equals,
        _formatVariable( encoding->header.status, byteswap,
                         { GrabPointer::Reply::status_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GrabKeyboard >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::GrabKeyboard;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const GrabKeyboard::Reply::Encoding* encoding {
        reinterpret_cast< const GrabKeyboard::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GrabKeyboard::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "status", memb_name_w, ws.equals,
        _formatVariable( encoding->header.status, byteswap,
                         { GrabKeyboard::Reply::status_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryPointer >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::QueryPointer;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const QueryPointer::Reply::Encoding* encoding {
        reinterpret_cast< const QueryPointer::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryPointer::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "same-screen", memb_name_w, ws.equals,
        _formatVariable( encoding->header.same_screen, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "root", memb_name_w, ws.equals,
        _formatVariable( encoding->root, byteswap ), ws.separator,
        ws.memb_indent, "child", memb_name_w, ws.equals,
        _formatVariable( encoding->child, byteswap,
                         { QueryPointer::Reply::child_names } ), ws.separator,
        ws.memb_indent, "root-x", memb_name_w, ws.equals,
        _formatVariable( encoding->root_x, byteswap ), ws.separator,
        ws.memb_indent, "root-y", memb_name_w, ws.equals,
        _formatVariable( encoding->root_y, byteswap ), ws.separator,
        ws.memb_indent, "win-x", memb_name_w, ws.equals,
        _formatVariable( encoding->win_x, byteswap ), ws.separator,
        ws.memb_indent, "win-y", memb_name_w, ws.equals,
        _formatVariable( encoding->win_y, byteswap ), ws.separator,
        ws.memb_indent, "mask", memb_name_w, ws.equals,
        _formatVariable( encoding->mask, byteswap ), ws.separator,
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
    const bool byteswap { conn->byteswap };
    const GetMotionEvents::Reply::Encoding* encoding {
        reinterpret_cast< const GetMotionEvents::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetMotionEvents::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofTIMECOORD events
    const _ParsingOutputs events {
        _parseLISTof< GetMotionEvents::Reply::TIMECOORD >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, encoding->events_ct,
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += events.bytes_parsed;
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(TIMECOORDs in events)", memb_name_w, ws.equals,
            _formatVariable( encoding->events_ct, byteswap ), ws.separator ),
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
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::TranslateCoordinates;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const TranslateCoordinates::Reply::Encoding* encoding {
        reinterpret_cast< const TranslateCoordinates::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( TranslateCoordinates::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "same-screen", memb_name_w, ws.equals,
        _formatVariable( encoding->header.same_screen, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "child", memb_name_w, ws.equals,
        _formatVariable( encoding->child, byteswap,
                         { TranslateCoordinates::Reply::child_names } ), ws.separator,
        ws.memb_indent, "dst-x", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_x, byteswap ), ws.separator,
        ws.memb_indent, "dst-y", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_y, byteswap ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetInputFocus >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::GetInputFocus;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const GetInputFocus::Reply::Encoding* encoding {
        reinterpret_cast< const GetInputFocus::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetInputFocus::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "revert-to", memb_name_w, ws.equals,
        _formatVariable( encoding->header.revert_to, byteswap,
                         { GetInputFocus::Reply::revert_to_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "focus", memb_name_w, ws.equals,
        _formatVariable( encoding->focus, byteswap,
                         { GetInputFocus::Reply::focus_names,
                           _EnumNameRange::Bound::MAX,
                           GetInputFocus::Reply::FOCUS_ENUM_MAX } ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryKeymap >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::QueryKeymap;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const QueryKeymap::Reply::Encoding* encoding {
        reinterpret_cast< const QueryKeymap::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryKeymap::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
            alignment.units( reply.bytes_parsed -
                             protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );
    // Encoding includes fixed size LISTofCARD8 keys
    const _ParsingOutputs keys {
        _parseLISTof< protocol::CARD8 >(
            encoding->keys, sizeof( encoding->keys ), sizeof( encoding->keys ),
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };

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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
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
    const bool byteswap { conn->byteswap };
    const QueryFont::Reply::Encoding* encoding {
        reinterpret_cast< const QueryFont::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryFont::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofFONTPROP properties
    const _ParsingOutputs properties {
        _parseLISTof< QueryFont::Reply::FONTPROP >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->properties_ct, byteswap ),
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += properties.bytes_parsed;
    // followed by LISTofCHARINFO char-infos
    const _ParsingOutputs char_infos {
        _parseLISTof< QueryFont::Reply::CHARINFO >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->char_infos_ct, byteswap ),
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += char_infos.bytes_parsed;
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "min-bounds", memb_name_w, ws.equals,
        _formatVariable( encoding->min_bounds, byteswap, ws.nested() ), ws.separator,
        ws.memb_indent, "max-bounds", memb_name_w, ws.equals,
        _formatVariable( encoding->max_bounds, byteswap, ws.nested() ), ws.separator,
        ws.memb_indent, "min-char-or-byte2", memb_name_w, ws.equals,
        _formatVariable( encoding->min_char_or_byte2, byteswap ), ws.separator,
        ws.memb_indent, "max-char-or-byte2", memb_name_w, ws.equals,
        _formatVariable( encoding->max_char_or_byte2, byteswap ), ws.separator,
        ws.memb_indent, "default-char", memb_name_w, ws.equals,
        _formatVariable( encoding->default_char, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(FONTPROPs in properties)", memb_name_w, ws.equals,
            _formatVariable( encoding->properties_ct, byteswap ), ws.separator ),
        ws.memb_indent, "draw-direction", memb_name_w, ws.equals,
        _formatVariable( encoding->draw_direction, byteswap,
                         { QueryFont::Reply::draw_direction_names } ), ws.separator,
        ws.memb_indent, "min-byte1", memb_name_w, ws.equals,
        _formatVariable( encoding->min_byte1, byteswap ), ws.separator,
        ws.memb_indent, "max-byte1", memb_name_w, ws.equals,
        _formatVariable( encoding->max_byte1, byteswap ), ws.separator,
        ws.memb_indent, "all-chars-exist", memb_name_w, ws.equals,
        _formatVariable( encoding->all_chars_exist, byteswap ), ws.separator,
        ws.memb_indent, "font-ascent", memb_name_w, ws.equals,
        _formatVariable( encoding->font_ascent, byteswap ), ws.separator,
        ws.memb_indent, "font-descent", memb_name_w, ws.equals,
        _formatVariable( encoding->font_descent, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(CHARINFOs in char-infos)", memb_name_w, ws.equals,
            _formatVariable( encoding->char_infos_ct, byteswap ), ws.separator ),
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
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::QueryTextExtents;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const QueryTextExtents::Reply::Encoding* encoding {
        reinterpret_cast< const QueryTextExtents::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryTextExtents::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "draw-direction", memb_name_w, ws.equals,
        _formatVariable( encoding->header.draw_direction, byteswap,
                         { QueryTextExtents::Reply::draw_direction_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "font-ascent", memb_name_w, ws.equals,
        _formatVariable( encoding->font_ascent, byteswap ), ws.separator,
        ws.memb_indent, "font-descent", memb_name_w, ws.equals,
        _formatVariable( encoding->font_descent, byteswap ), ws.separator,
        ws.memb_indent, "overall-ascent", memb_name_w, ws.equals,
        _formatVariable( encoding->overall_ascent, byteswap ), ws.separator,
        ws.memb_indent, "overall-descent", memb_name_w, ws.equals,
        _formatVariable( encoding->overall_descent, byteswap ), ws.separator,
        ws.memb_indent, "overall-width", memb_name_w, ws.equals,
        _formatVariable( encoding->overall_width, byteswap ), ws.separator,
        ws.memb_indent, "overall-left", memb_name_w, ws.equals,
        _formatVariable( encoding->overall_left, byteswap ), ws.separator,
        ws.memb_indent, "overall-right", memb_name_w, ws.equals,
        _formatVariable( encoding->overall_right, byteswap ), ws.separator,
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
    const bool byteswap { conn->byteswap };
    const ListFonts::Reply::Encoding* encoding {
        reinterpret_cast< const ListFonts::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( ListFonts::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofSTR names
    const _ParsingOutputs names {
        _parseLISTof< protocol::STR >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->names_ct, byteswap ),
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += alignment.pad( names.bytes_parsed );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(STRs in names)", memb_name_w, ws.equals,
            _formatVariable( encoding->names_ct, byteswap ), ws.separator ),
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
    const bool byteswap { conn->byteswap };
    const ListFontsWithInfo::Reply::Encoding* encoding {
        reinterpret_cast< const ListFontsWithInfo::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( ListFontsWithInfo::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );

    if ( _ordered( encoding->header.last_reply, byteswap ) ==
         ListFontsWithInfo::Reply::LAST_REPLY ) {
        assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
                _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
            ws.memb_indent, "last-reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.last_reply, byteswap ), ws.separator,
            !settings.verbose ? "" : fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
                _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
            !settings.verbose ? "" : fmt::format(
                "{}{: <{}}{}{}{}",
                ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
                _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
            ws.encl_indent
            );
        return reply;
    }

    // followed by LISTofFONTPROP properties
    const _ParsingOutputs properties {
        _parseLISTof< ListFontsWithInfo::Reply::FONTPROP >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->properties_ct, byteswap ),
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += properties.bytes_parsed;
    // followed by STRING8 name
    const std::string_view name {
        reinterpret_cast< const char* >( data + reply.bytes_parsed ),
        encoding->header.name_len };
    reply.bytes_parsed += alignment.pad( encoding->header.name_len );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.name_len, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "min-bounds", memb_name_w, ws.equals,
        _formatVariable( encoding->min_bounds, byteswap, ws.nested() ), ws.separator,
        ws.memb_indent, "max-bounds", memb_name_w, ws.equals,
        _formatVariable( encoding->max_bounds, byteswap, ws.nested() ), ws.separator,
        ws.memb_indent, "min-char-or-byte2", memb_name_w, ws.equals,
        _formatVariable( encoding->min_char_or_byte2, byteswap ), ws.separator,
        ws.memb_indent, "max-char-or-byte2", memb_name_w, ws.equals,
        _formatVariable( encoding->max_char_or_byte2, byteswap ), ws.separator,
        ws.memb_indent, "default-char", memb_name_w, ws.equals,
        _formatVariable( encoding->default_char, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(FONTPROPs in properties)", memb_name_w, ws.equals,
            _formatVariable( encoding->properties_ct, byteswap ), ws.separator ),
        ws.memb_indent, "draw-direction", memb_name_w, ws.equals,
        _formatVariable( encoding->draw_direction, byteswap,
                         { ListFontsWithInfo::Reply::draw_direction_names } ), ws.separator,
        ws.memb_indent, "min-byte1", memb_name_w, ws.equals,
        _formatVariable( encoding->min_byte1, byteswap ), ws.separator,
        ws.memb_indent, "max-byte1", memb_name_w, ws.equals,
        _formatVariable( encoding->max_byte1, byteswap ), ws.separator,
        ws.memb_indent, "all-chars-exist", memb_name_w, ws.equals,
        _formatVariable( encoding->all_chars_exist, byteswap ), ws.separator,
        ws.memb_indent, "font-ascent", memb_name_w, ws.equals,
        _formatVariable( encoding->font_ascent, byteswap ), ws.separator,
        ws.memb_indent, "font-descent", memb_name_w, ws.equals,
        _formatVariable( encoding->font_descent, byteswap ), ws.separator,
        ws.memb_indent, "replies-hint", memb_name_w, ws.equals,
        _formatVariable( encoding->replies_hint, byteswap ), ws.separator,
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
    const bool byteswap { conn->byteswap };
    const GetFontPath::Reply::Encoding* encoding {
        reinterpret_cast< const GetFontPath::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetFontPath::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofSTR path
    const _ParsingOutputs path {
        _parseLISTof< protocol::STR >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->path_ct, byteswap ),
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += alignment.pad( path.bytes_parsed );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(STRs in path)", memb_name_w, ws.equals,
            _formatVariable( encoding->path_ct, byteswap ), ws.separator ),
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
    const bool byteswap { conn->byteswap };
    const GetImage::Reply::Encoding* encoding {
        reinterpret_cast< const GetImage::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetImage::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofBYTE data
    const auto extra_aligned_units {
        _ordered( encoding->header.extra_aligned_units, byteswap ) };
    const _ParsingOutputs data_ {
        _parseLISTof< protocol::BYTE >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            // (no data length in encoding, must be inferred from eau)
            alignment.size( extra_aligned_units ),
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    reply.bytes_parsed += alignment.pad( data_.bytes_parsed );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatVariable( encoding->header.depth, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "visual", memb_name_w, ws.equals,
        _formatVariable( encoding->visual, byteswap,
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
    const bool byteswap { conn->byteswap };
    const ListInstalledColormaps::Reply::Encoding* encoding {
        reinterpret_cast< const ListInstalledColormaps::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( ListInstalledColormaps::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofCOLORMAP cmaps
    const _ParsingOutputs cmaps {
        _parseLISTof< protocol::COLORMAP >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->cmaps_ct, byteswap ),
            byteswap, ws.nested() ) };
    reply.bytes_parsed += cmaps.bytes_parsed;
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(COLORMAPs in cmaps)", memb_name_w, ws.equals,
            _formatVariable( encoding->cmaps_ct, byteswap ), ws.separator ),
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
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::AllocColor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const AllocColor::Reply::Encoding* encoding {
        reinterpret_cast< const AllocColor::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( AllocColor::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "red", memb_name_w, ws.equals,
        _formatVariable( encoding->red, byteswap ), ws.separator,
        ws.memb_indent, "green", memb_name_w, ws.equals,
        _formatVariable( encoding->green, byteswap ), ws.separator,
        ws.memb_indent, "blue", memb_name_w, ws.equals,
        _formatVariable( encoding->blue, byteswap ), ws.separator,
        ws.memb_indent, "pixel", memb_name_w, ws.equals,
        _formatVariable( encoding->pixel, byteswap ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::AllocNamedColor >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::AllocNamedColor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const AllocNamedColor::Reply::Encoding* encoding {
        reinterpret_cast< const AllocNamedColor::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( AllocNamedColor::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "pixel", memb_name_w, ws.equals,
        _formatVariable( encoding->pixel, byteswap ), ws.separator,
        ws.memb_indent, "exact-red", memb_name_w, ws.equals,
        _formatVariable( encoding->exact_red, byteswap ), ws.separator,
        ws.memb_indent, "exact-green", memb_name_w, ws.equals,
        _formatVariable( encoding->exact_green, byteswap ), ws.separator,
        ws.memb_indent, "exact-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->exact_blue, byteswap ), ws.separator,
        ws.memb_indent, "visual-red", memb_name_w, ws.equals,
        _formatVariable( encoding->visual_red, byteswap ), ws.separator,
        ws.memb_indent, "visual-green", memb_name_w, ws.equals,
        _formatVariable( encoding->visual_green, byteswap ), ws.separator,
        ws.memb_indent, "visual-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->visual_blue, byteswap ), ws.separator,
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
    const bool byteswap { conn->byteswap };
    const AllocColorCells::Reply::Encoding* encoding {
        reinterpret_cast< const AllocColorCells::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( AllocColorCells::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofCARD32 pixels
    const _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->pixels_ct, byteswap ),
            byteswap, ws.nested() ) };
    reply.bytes_parsed += pixels.bytes_parsed;
    // followed by LISTofCARD32 masks
    const _ParsingOutputs masks {
        _parseLISTof< protocol::CARD32 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->masks_ct, byteswap ),
            byteswap, ws.nested(),
            _ValueTraits{ {}, _ValueTraits::BITMASK } ) };  // force hex
    reply.bytes_parsed += masks.bytes_parsed;
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(CARD32 in pixels)", memb_name_w, ws.equals,
            _formatVariable( encoding->pixels_ct, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(CARD32 in masks)", memb_name_w, ws.equals,
            _formatVariable( encoding->masks_ct, byteswap ), ws.separator ),
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
    const bool byteswap { conn->byteswap };
    const AllocColorPlanes::Reply::Encoding* encoding {
        reinterpret_cast< const AllocColorPlanes::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( AllocColorPlanes::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofCARD32 pixels
    const _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->pixels_ct, byteswap ),
            byteswap, ws.nested() ) };
    reply.bytes_parsed += pixels.bytes_parsed;
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(CARD32 in pixels)", memb_name_w, ws.equals,
            _formatVariable( encoding->pixels_ct, byteswap ), ws.separator ),
        ws.memb_indent, "red-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->red_mask, byteswap,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "green-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->green_mask, byteswap,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "blue-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->blue_mask, byteswap,
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
    const bool byteswap { conn->byteswap };
    const QueryColors::Reply::Encoding* encoding {
        reinterpret_cast< const QueryColors::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryColors::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofRGB colors
    const _ParsingOutputs colors {
        _parseLISTof< QueryColors::Reply::RGB >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->colors_ct, byteswap ),
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += colors.bytes_parsed;
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(RGBs in colors)", memb_name_w, ws.equals,
            _formatVariable( encoding->colors_ct, byteswap ), ws.separator ),
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
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::LookupColor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const LookupColor::Reply::Encoding* encoding {
        reinterpret_cast< const LookupColor::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( LookupColor::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "exact-red", memb_name_w, ws.equals,
        _formatVariable( encoding->exact_red, byteswap ), ws.separator,
        ws.memb_indent, "exact-green", memb_name_w, ws.equals,
        _formatVariable( encoding->exact_green, byteswap ), ws.separator,
        ws.memb_indent, "exact-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->exact_blue, byteswap ), ws.separator,
        ws.memb_indent, "visual-red", memb_name_w, ws.equals,
        _formatVariable( encoding->visual_red, byteswap ), ws.separator,
        ws.memb_indent, "visual-green", memb_name_w, ws.equals,
        _formatVariable( encoding->visual_green, byteswap ), ws.separator,
        ws.memb_indent, "visual-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->visual_blue, byteswap ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryBestSize >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::QueryBestSize;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const QueryBestSize::Reply::Encoding* encoding {
        reinterpret_cast< const QueryBestSize::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryBestSize::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height, byteswap ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::QueryExtension >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::QueryExtension;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const QueryExtension::Reply::Encoding* encoding {
        reinterpret_cast< const QueryExtension::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( QueryExtension::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
            alignment.units( reply.bytes_parsed -
                             protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );
    const std::string_view ext_name {
        _unstashString(
            conn->id, _ordered( encoding->header.sequence_num, byteswap ) ) };
    // control use of extensions by spoofing `present` in reply
    if ( settings.extensionDenied( ext_name ) ) {
        // setting to false/0 should not need byte ordering
        const_cast< QueryExtension::Reply::Encoding* >(
            reinterpret_cast< const QueryExtension::Reply::Encoding* >(
                data ) )->present.data = false;
    }
    if ( encoding->present.data ) {
        _enableExtensionParsing( ext_name,
                                 _ordered( encoding->major_opcode, byteswap ),
                                 _ordered( encoding->first_event, byteswap ),
                                 _ordered( encoding->first_error, byteswap ) );
        // BIG-REQUESTS is a special case in that it is not considered activated
        //   until the server replies to request BigReqEnable
        if ( ext_name != protocol::extensions::big_requests::name ) {
            assert( !conn->extensions.active( ext_name ) );
            conn->extensions.activate( ext_name );
        }
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "present", memb_name_w, ws.equals,
        _formatVariable( encoding->present, byteswap ), ws.separator,
        ws.memb_indent, "major-opcode", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode, byteswap ), ws.separator,
        ws.memb_indent, "first-event", memb_name_w, ws.equals,
        _formatVariable( encoding->first_event, byteswap ), ws.separator,
        ws.memb_indent, "first-error", memb_name_w, ws.equals,
        _formatVariable( encoding->first_error, byteswap ), ws.separator,
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
    const bool byteswap { conn->byteswap };
    const ListExtensions::Reply::Encoding* encoding {
        reinterpret_cast< const ListExtensions::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( ListExtensions::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofSTR names
    const _ParsingOutputs names {
        _parseLISTof< protocol::STR >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->header.names_ct, byteswap ),
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    reply.bytes_parsed += alignment.pad( names.bytes_parsed );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(STRs in names)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.names_ct, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
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
    const bool byteswap { conn->byteswap };
    const GetKeyboardMapping::Reply::Encoding* encoding {
        reinterpret_cast< const GetKeyboardMapping::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetKeyboardMapping::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofKEYSYM keysyms
    // (keycode count is field in request, but here must be inferred)
    const auto extra_aligned_units {
        _ordered( encoding->header.extra_aligned_units, byteswap ) };
    const uint32_t keysym_ct (
        alignment.size( extra_aligned_units ) / sizeof( protocol::KEYSYM ) );
    const _ParsingOutputs keysyms {
        _parseLISTof< protocol::KEYSYM >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, keysym_ct,
            byteswap, ws.nested() ) };
    reply.bytes_parsed += keysyms.bytes_parsed;
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "keysyms-per-keycode", memb_name_w, ws.equals,
            _formatVariable( encoding->header.keysyms_per_keycode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
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
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::GetKeyboardControl;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const GetKeyboardControl::Reply::Encoding* encoding {
        reinterpret_cast< const GetKeyboardControl::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetKeyboardControl::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
            alignment.units( reply.bytes_parsed -
                             protocol::requests::Reply::DEFAULT_ENCODING_SZ ) );
    // Encoding includes fixed size LISTofCARD8 auto-repeats
    const _ParsingOutputs auto_repeats {
        _parseLISTof< protocol::CARD8 >(
            encoding->auto_repeats,
            sizeof( encoding->auto_repeats ), sizeof( encoding->auto_repeats ),
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };

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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "global-auto-repeat", memb_name_w, ws.equals,
        _formatVariable(
            encoding->header.global_auto_repeat, byteswap,
            { GetKeyboardControl::Reply::global_auto_repeat_names,
              _EnumNameRange::Bound::MAX,
              GetKeyboardControl::Reply::GLOBAL_AUTO_REPEAT_ENUM_MAX } ),
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "led-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->led_mask, byteswap,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "key-click-percent", memb_name_w, ws.equals,
        _formatVariable( encoding->key_click_percent, byteswap ), ws.separator,
        ws.memb_indent, "bell-percent", memb_name_w, ws.equals,
        _formatVariable( encoding->bell_percent, byteswap ), ws.separator,
        ws.memb_indent, "bell-pitch", memb_name_w, ws.equals,
        _formatVariable( encoding->bell_pitch, byteswap ), ws.separator,
        ws.memb_indent, "bell-duration", memb_name_w, ws.equals,
        _formatVariable( encoding->bell_duration, byteswap ), ws.separator,
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
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::GetPointerControl;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const GetPointerControl::Reply::Encoding* encoding {
        reinterpret_cast< const GetPointerControl::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetPointerControl::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "acceleration-numerator", memb_name_w, ws.equals,
        _formatVariable( encoding->acceleration_numerator, byteswap ), ws.separator,
        ws.memb_indent, "acceleration-denominator", memb_name_w, ws.equals,
        _formatVariable( encoding->acceleration_denominator, byteswap ), ws.separator,
        ws.memb_indent, "threshold", memb_name_w, ws.equals,
        _formatVariable( encoding->threshold, byteswap ), ws.separator,
        ws.encl_indent
        );
    return reply;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    protocol::requests::GetScreenSaver >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::GetScreenSaver;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const GetScreenSaver::Reply::Encoding* encoding {
        reinterpret_cast< const GetScreenSaver::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetScreenSaver::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "timeout", memb_name_w, ws.equals,
        _formatVariable( encoding->timeout, byteswap ), ws.separator,
        ws.memb_indent, "interval", memb_name_w, ws.equals,
        _formatVariable( encoding->interval, byteswap ), ws.separator,
        ws.memb_indent, "prefer-blanking", memb_name_w, ws.equals,
        _formatVariable( encoding->prefer_blanking, byteswap,
                         { GetScreenSaver::Reply::prefer_blanking_names,
                           _EnumNameRange::Bound::MAX,
                           GetScreenSaver::Reply::PREFER_BLANKING_ENUM_MAX } ),
        ws.separator,
        ws.memb_indent, "allow-exposures", memb_name_w, ws.equals,
        _formatVariable( encoding->allow_exposures, byteswap,
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
    const bool byteswap { conn->byteswap };
    const ListHosts::Reply::Encoding* encoding {
        reinterpret_cast< const ListHosts::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( ListHosts::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofHOST hosts
    const _ParsingOutputs hosts {
        _parseLISTof< protocol::HOST >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->hosts_ct, byteswap ),
            byteswap, ws.nested() ) };
    reply.bytes_parsed += hosts.bytes_parsed;
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( encoding->header.mode, byteswap,
                         { ListHosts::Reply::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(HOSTs in hosts)", memb_name_w, ws.equals,
            _formatVariable( encoding->hosts_ct, byteswap ), ws.separator ),
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
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::SetPointerMapping;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const SetPointerMapping::Reply::Encoding* encoding {
        reinterpret_cast< const SetPointerMapping::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( SetPointerMapping::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "status", memb_name_w, ws.equals,
        _formatVariable( encoding->header.status, byteswap,
                         { SetPointerMapping::Reply::status_names,
                           _EnumNameRange::Bound::MAX,
                           SetPointerMapping::Reply::STATUS_ENUM_MAX } ),
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
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
    const bool byteswap { conn->byteswap };
    const GetPointerMapping::Reply::Encoding* encoding {
        reinterpret_cast< const GetPointerMapping::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetPointerMapping::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofCARD8 map
    const _ParsingOutputs map {
        _parseLISTof< protocol::CARD8 >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed,
            _ordered( encoding->header.map_len, byteswap ),
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(map length)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.map_len, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
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
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::requests::SetModifierMapping;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const SetModifierMapping::Reply::Encoding* encoding {
        reinterpret_cast< const SetModifierMapping::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( SetModifierMapping::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "status", memb_name_w, ws.equals,
        _formatVariable( encoding->header.status, byteswap,
                         { SetModifierMapping::Reply::status_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
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
    const bool byteswap { conn->byteswap };
    const GetModifierMapping::Reply::Encoding* encoding {
        reinterpret_cast< const GetModifierMapping::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( GetModifierMapping::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            protocol::requests::Reply::REPLY );
    // followed by LISTofKEYCODE keycodes
    const uint32_t keycode_ct {
        GetModifierMapping::Reply::MODIFIER_CT *
        _ordered( encoding->header.keycodes_per_modifier, byteswap ) };
    const _ParsingOutputs keycodes {
        _parseLISTof< protocol::KEYCODE >(
            data + reply.bytes_parsed, sz - reply.bytes_parsed, keycode_ct,
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    reply.bytes_parsed += keycodes.bytes_parsed;
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
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
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        ws.memb_indent, "keycodes-per-modifier", memb_name_w, ws.equals,
        _formatVariable( encoding->header.keycodes_per_modifier, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "keycodes", memb_name_w, ws.equals,
        keycodes.str, ws.separator,
        ws.encl_indent
        );
    return reply;
}
