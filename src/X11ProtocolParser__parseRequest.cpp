#include <string_view>
#include <unordered_set>
#include <type_traits>

#include <cctype>      // isprint

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/common_types.hpp"
#include "protocol/requests.hpp"
#include "protocol/events.hpp"
#include "protocol/atoms.hpp"
#include "protocol/extensions/big_requests.hpp"  // EXTENDED_LENGTH_FLAG


namespace ext = protocol::extensions;

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest< X11ProtocolParser::_UnknownRequest >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= _UnknownRequest::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< _UnknownRequest > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( fe.header_only );
    // potentially followed by unparsable suffix
    const auto tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t data_len { alignment.size( tl_aligned_units ) -
        ( fe.big_request ? _UnknownRequest::BASE_BIG_ENCODING_SZ :
                           _UnknownRequest::BASE_ENCODING_SZ ) };
    request.bytes_parsed += alignment.pad( data_len );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "(unparseable suffix)" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}({} bytes){}"
        "{}}}",
        ws.separator,
        ws.memb_indent, "major-opcode", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->major_opcode, byteswap ), ws.separator,
        ws.memb_indent, "minor-opcode", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->minor_opcode, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "(unparseable suffix)", memb_name_w, ws.equals,
        data_len, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::impl::SimpleRequest >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::impl::SimpleRequest;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SimpleRequest::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SimpleRequest > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( fe.header_only );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::impl::SimpleWindowRequest >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::impl::SimpleWindowRequest;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SimpleWindowRequest::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SimpleWindowRequest > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "window" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::impl::ListFontsRequest >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::impl::ListFontsRequest;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ListFontsRequest::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ListFontsRequest > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    // followed by STRING8 pattern
    const auto pattern_len { _ordered( fe.encoding->pattern_len, byteswap ) };
    const std::string_view pattern {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        pattern_len };
    request.bytes_parsed += alignment.pad( pattern_len );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "max-names" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "max-names", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->max_names, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(pattern length)", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->pattern_len, byteswap ), ws.separator ),
        ws.memb_indent, "pattern", memb_name_w, ws.equals,
        pattern, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::impl::PolyPointRequest >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::impl::PolyPointRequest;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= PolyPointRequest::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< PolyPointRequest > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    // followed by LISTofPOINT points
    const uint32_t tl_aligned_units {
        fe.big_request ?
        _ordered( fe.big_length->tl_aligned_units, byteswap ) :
        _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t points_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? PolyPointRequest::BASE_BIG_ENCODING_SZ :
                           PolyPointRequest::BASE_ENCODING_SZ ) };
    const size_t points_ct { points_sz / sizeof( protocol::POINT ) };
    const _ParsingOutputs points {
        _parseLISTof< protocol::POINT >(
            data + request.bytes_parsed, points_sz, points_ct,
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( points.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "coordinate-mode" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "coordinate-mode", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->coordinate_mode, byteswap,
                         { PolyPointRequest::coordinate_mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "points", memb_name_w, ws.equals,
        points.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::impl::SimpleCmapRequest >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::impl::SimpleCmapRequest;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SimpleCmapRequest::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SimpleCmapRequest > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "cmap" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cmap, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CreateWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::CreateWindow;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CreateWindow::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< CreateWindow > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CREATEWINDOW );
    // followed by LISTofVALUE value-list
    const std::vector< _ValueTraits > value_traits {
        /* background-pixmap     */ {
            _EnumNameRange{ CreateWindow::background_pixmap_names } },
        /* background-pixel      */ {},
        /* border-pixmap         */ {
            _EnumNameRange{ CreateWindow::border_pixmap_names } },
        /* border-pixel          */ {},
        /* bit-gravity           */ {},
        /* win-gravity           */ {},
        /* backing-store         */ {
            _EnumNameRange{ CreateWindow::backing_store_names } },
        /* backing-planes        */ {},
        /* backing-pixel         */ {},
        /* override-redirect     */ {},
        /* save-under            */ {},
        /* event-mask            */ {
            {}, _ValueTraits::BITMASK },
        /* do-not-propagate-mask */ {
            {}, _ValueTraits::BITMASK },
        /* colormap              */ {
            _EnumNameRange{ CreateWindow::colormap_names } },
        /* cursor                */ {
            _EnumNameRange{ CreateWindow::cursor_names } }
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        data + request.bytes_parsed, sz - request.bytes_parsed,
        _ordered( fe.encoding->value_mask, byteswap ),
        CreateWindow::value_types, CreateWindow::value_names, value_traits,
        byteswap, ws.nested() };
    _LISTofVALUEParsingOutputs value_list;
    _parseLISTofVALUE( value_list_inputs, &value_list );
    request.bytes_parsed += value_list.bytes_parsed;
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "border-width" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->depth, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "wid", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->wid, byteswap ), ws.separator,
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->parent, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->height, byteswap ), ws.separator,
        ws.memb_indent, "border-width", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->border_width, byteswap ), ws.separator,
        ws.memb_indent, "class", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->class_, byteswap,
                         { CreateWindow::class_names } ), ws.separator,
        ws.memb_indent, "visual", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->visual, byteswap,
                         { CreateWindow::visual_names } ),
        ws.separator,
        ws.memb_indent, "value-mask", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->value_mask, byteswap,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "value-list", memb_name_w, ws.equals,
        value_list.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeWindowAttributes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ChangeWindowAttributes;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ChangeWindowAttributes::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ChangeWindowAttributes > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CHANGEWINDOWATTRIBUTES );
    // followed by LISTofVALUE value-list
    const std::vector< _ValueTraits > value_traits {
        /* background-pixmap     */ {
            _EnumNameRange{ ChangeWindowAttributes::background_pixmap_names } },
        /* background-pixel      */ {},
        /* border-pixmap         */ {
            _EnumNameRange{ ChangeWindowAttributes::border_pixmap_names } },
        /* border-pixel          */ {},
        /* bit-gravity           */ {},
        /* win-gravity           */ {},
        /* backing-store         */ {
            _EnumNameRange{ ChangeWindowAttributes::backing_store_names } },
        /* backing-planes        */ {},
        /* backing-pixel         */ {},
        /* override-redirect     */ {},
        /* save-under            */ {},
        /* event-mask            */ {
            {}, _ValueTraits::BITMASK },
        /* do-not-propagate-mask */ {
            {}, _ValueTraits::BITMASK },
        /* colormap              */ {
            _EnumNameRange{ ChangeWindowAttributes::colormap_names } },
        /* cursor                */ {
            _EnumNameRange{ ChangeWindowAttributes::cursor_names } }
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        data + request.bytes_parsed, sz - request.bytes_parsed,
        _ordered( fe.encoding->value_mask, byteswap ),
        ChangeWindowAttributes::value_types, ChangeWindowAttributes::value_names,
        value_traits, byteswap, ws.nested() };
    _LISTofVALUEParsingOutputs value_list;
    _parseLISTofVALUE( value_list_inputs, &value_list );
    request.bytes_parsed += value_list.bytes_parsed;
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "value-mask" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "value-mask", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->value_mask, byteswap,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "value-list", memb_name_w, ws.equals,
        value_list.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetWindowAttributes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetWindowAttributes;
    using protocol::requests::impl::SimpleWindowRequest;
    static_assert( std::is_base_of_v< SimpleWindowRequest, GetWindowAttributes > );
    assert( data != nullptr );
    assert( sz >= sizeof( GetWindowAttributes::Prefix ) );
    assert( _ordered( reinterpret_cast< const GetWindowAttributes::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::GETWINDOWATTRIBUTES );
    return _parseRequest< SimpleWindowRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::DestroyWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::DestroyWindow;
    using protocol::requests::impl::SimpleWindowRequest;
    static_assert( std::is_base_of_v< SimpleWindowRequest, DestroyWindow > );
    assert( data != nullptr );
    assert( sz >= sizeof( DestroyWindow::Prefix ) );
    assert( _ordered( reinterpret_cast< const DestroyWindow::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::DESTROYWINDOW );
    return _parseRequest< SimpleWindowRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::DestroySubwindows >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::DestroySubwindows;
    using protocol::requests::impl::SimpleWindowRequest;
    static_assert( std::is_base_of_v< SimpleWindowRequest, DestroySubwindows > );
    assert( data != nullptr );
    assert( sz >= sizeof( DestroySubwindows::Prefix ) );
    assert( _ordered( reinterpret_cast< const DestroySubwindows::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::DESTROYSUBWINDOWS );
    return _parseRequest< SimpleWindowRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeSaveSet >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ChangeSaveSet;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ChangeSaveSet::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ChangeSaveSet > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CHANGESAVESET );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "window" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->mode, byteswap,
                         { ChangeSaveSet::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ReparentWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ReparentWindow;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ReparentWindow::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ReparentWindow > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::REPARENTWINDOW );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "window" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->y, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::MapWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::MapWindow;
    using protocol::requests::impl::SimpleWindowRequest;
    static_assert( std::is_base_of_v< SimpleWindowRequest, MapWindow > );
    assert( data != nullptr );
    assert( sz >= sizeof( MapWindow::Prefix ) );
    assert( _ordered( reinterpret_cast< const MapWindow::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::MAPWINDOW );
    return _parseRequest< SimpleWindowRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::MapSubwindows >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::MapSubwindows;
    using protocol::requests::impl::SimpleWindowRequest;
    static_assert( std::is_base_of_v< SimpleWindowRequest, MapSubwindows > );
    assert( data != nullptr );
    assert( sz >= sizeof( MapSubwindows::Prefix ) );
    assert( _ordered( reinterpret_cast< const MapSubwindows::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::MAPSUBWINDOWS );
    return _parseRequest< SimpleWindowRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UnmapWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::UnmapWindow;
    using protocol::requests::impl::SimpleWindowRequest;
    static_assert( std::is_base_of_v< SimpleWindowRequest, UnmapWindow > );
    assert( data != nullptr );
    assert( sz >= sizeof( UnmapWindow::Prefix ) );
    assert( _ordered( reinterpret_cast< const UnmapWindow::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::UNMAPWINDOW );
    return _parseRequest< SimpleWindowRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UnmapSubwindows >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::UnmapSubwindows;
    using protocol::requests::impl::SimpleWindowRequest;
    static_assert( std::is_base_of_v< SimpleWindowRequest, UnmapSubwindows > );
    assert( data != nullptr );
    assert( sz >= sizeof( UnmapSubwindows::Prefix ) );
    assert( _ordered( reinterpret_cast< const UnmapSubwindows::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::UNMAPSUBWINDOWS );
    return _parseRequest< SimpleWindowRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ConfigureWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ConfigureWindow;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ConfigureWindow::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ConfigureWindow > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CONFIGUREWINDOW );
    // followed by LISTofVALUE value-list
    const std::vector< _ValueTraits > value_traits {
        /* x            */ {},
        /* y            */ {},
        /* width        */ {},
        /* height       */ {},
        /* border-width */ {},
        /* sibling      */ {},
        /* stack-mode   */ {
            _EnumNameRange{ ConfigureWindow::stack_mode_names } }
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        data + request.bytes_parsed, sz - request.bytes_parsed,
        _ordered( fe.encoding->value_mask, byteswap ),
        ConfigureWindow::value_types, ConfigureWindow::value_names, value_traits,
        byteswap, ws.nested() };
    _LISTofVALUEParsingOutputs value_list;
    _parseLISTofVALUE( value_list_inputs, &value_list );
    request.bytes_parsed += value_list.bytes_parsed;
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "value-mask" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "value-mask", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->value_mask, byteswap,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "value-list", memb_name_w, ws.equals,
        value_list.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CirculateWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::CirculateWindow;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CirculateWindow::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< CirculateWindow > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CIRCULATEWINDOW );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "direction" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "direction", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->direction, byteswap,
                         { CirculateWindow::direction_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetGeometry >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetGeometry;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GetGeometry::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< GetGeometry > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::GETGEOMETRY );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryTree >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryTree;
    using protocol::requests::impl::SimpleWindowRequest;
    static_assert( std::is_base_of_v< SimpleWindowRequest, QueryTree > );
    assert( data != nullptr );
    assert( sz >= sizeof( QueryTree::Prefix ) );
    assert( _ordered( reinterpret_cast< const QueryTree::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::QUERYTREE );
    return _parseRequest< SimpleWindowRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::InternAtom >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::InternAtom;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= InternAtom::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< InternAtom > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::INTERNATOM );
    // followed by STRING8 name
    const auto name_len { _ordered( fe.encoding->name_len, byteswap ) };
    const std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        name_len };
    request.bytes_parsed += alignment.pad( name_len );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    // Stash copy of atom until reply comes in - at that time we will include it
    //   in our own internment if it isn't already
    _stashString( conn->id, conn->sequence, name );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "only-if-exists" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "only-if-exists", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->only_if_exists, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->name_len, byteswap ), ws.separator ),
        ws.memb_indent, "name", memb_name_w, ws.equals,
        name, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetAtomName >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetAtomName;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GetAtomName::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< GetAtomName > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::GETATOMNAME );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "atom" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "atom", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->atom, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ChangeProperty;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ChangeProperty::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ChangeProperty > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CHANGEPROPERTY );
    // followed by LISTofBYTE data
    const auto format { _ordered( fe.encoding->format, byteswap ) };
    assert( format <= 32 && format % 8 == 0 );
    const uint32_t data_len {
        _ordered( fe.encoding->data_fmt_unit_len, byteswap ) *
        ( format / 8 ) };
    _ParsingOutputs data_;
    if ( _ordered( fe.encoding->type.data, byteswap ) ==
         protocol::atoms::predefined::STRING ) {
        data_.str = fmt::format(
            "{:?}", std::string_view{
                reinterpret_cast< const char* >(
                    data + request.bytes_parsed ), data_len } );
        data_.bytes_parsed = data_len;
    } else {
        data_ = _parseLISTof< protocol::BYTE >(
            data + request.bytes_parsed, data_len, data_len,
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) );
    }
    request.bytes_parsed += alignment.pad( data_.bytes_parsed );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "property" ) - 1 :
                            sizeof( "(data length (format units))" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->mode, byteswap,
                         { ChangeProperty::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "property", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->property, byteswap ), ws.separator,
        ws.memb_indent, "type", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->type, byteswap ), ws.separator,
        ws.memb_indent, "format", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->format, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(data length (format units))", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->data_fmt_unit_len, byteswap ), ws.separator ),
        ws.memb_indent, "data", memb_name_w, ws.equals,
        data_.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::DeleteProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::DeleteProperty;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= DeleteProperty::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< DeleteProperty > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::DELETEPROPERTY );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "property" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "property", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->property, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetProperty;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GetProperty::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< GetProperty > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::GETPROPERTY );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "long-offset" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "delete", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->delete_, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "property", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->property, byteswap ), ws.separator,
        ws.memb_indent, "type", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->type, byteswap,
                         { GetProperty::type_names } ), ws.separator,
        ws.memb_indent, "long-offset", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->long_offset, byteswap ), ws.separator,
        ws.memb_indent, "long-length", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->long_length, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ListProperties >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ListProperties;
    using protocol::requests::impl::SimpleWindowRequest;
    static_assert( std::is_base_of_v< SimpleWindowRequest, ListProperties > );
    assert( data != nullptr );
    assert( sz >= sizeof( ListProperties::Prefix ) );
    assert( _ordered( reinterpret_cast< const ListProperties::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::LISTPROPERTIES );
    return _parseRequest< SimpleWindowRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetSelectionOwner >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SetSelectionOwner;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SetSelectionOwner::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SetSelectionOwner > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::SETSELECTIONOWNER );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "selection" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "owner", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->owner, byteswap,
                         { SetSelectionOwner::owner_names } ), ws.separator,
        ws.memb_indent, "selection", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->selection, byteswap ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->time, byteswap,
                         { SetSelectionOwner::time_names } ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetSelectionOwner >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetSelectionOwner;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GetSelectionOwner::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< GetSelectionOwner > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::GETSELECTIONOWNER );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "selection" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "selection", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->selection, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ConvertSelection >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ConvertSelection;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ConvertSelection::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ConvertSelection > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CONVERTSELECTION );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "requestor" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "requestor", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->requestor, byteswap ), ws.separator,
        ws.memb_indent, "selection", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->selection, byteswap ), ws.separator,
        ws.memb_indent, "target", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->target, byteswap ), ws.separator,
        ws.memb_indent, "property", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->property, byteswap,
                         { ConvertSelection::property_names } ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->time, byteswap,
                         { ConvertSelection::time_names } ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SendEvent >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SendEvent;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SendEvent::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SendEvent > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::SENDEVENT );
    // followed by Event
    const uint8_t evt_code {
        _ordered( reinterpret_cast< const protocol::events::Event::Header* >(
                      data + request.bytes_parsed )->code, byteswap ) };
    const _ParsingOutputs event { _parseEvent(
            conn, data + request.bytes_parsed,
            protocol::events::Event::ENCODING_SZ, ws.nested() ) };
    request.bytes_parsed += event.bytes_parsed;
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "destination" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}({}) {}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "propagate", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->propagate, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "destination", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->destination, byteswap,
                         { SendEvent::destination_names } ), ws.separator,
        ws.memb_indent, "event-mask", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->event_mask, byteswap ), ws.separator,
        ws.memb_indent, "event", memb_name_w, ws.equals,
        protocol::events::names[ evt_code ], evt_code,
        event.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GrabPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GrabPointer;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GrabPointer::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< GrabPointer > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::GRABPOINTER );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "keyboard-mode" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "owner-events", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->owner_events, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "grab-window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->grab_window, byteswap ), ws.separator,
        ws.memb_indent, "event-mask", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->event_mask, byteswap ), ws.separator,
        ws.memb_indent, "pointer-mode", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->pointer_mode, byteswap,
                         { GrabPointer::pointer_mode_names } ), ws.separator,
        ws.memb_indent, "keyboard-mode", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->keyboard_mode, byteswap,
                         { GrabPointer::keyboard_mode_names } ), ws.separator,
        ws.memb_indent, "confine-to", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->confine_to, byteswap,
                         { GrabPointer::confine_to_names } ), ws.separator,
        ws.memb_indent, "cursor", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cursor, byteswap,
                         { GrabPointer::cursor_names } ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->time, byteswap,
                         { GrabPointer::time_names } ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UngrabPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::UngrabPointer;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= UngrabPointer::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< UngrabPointer > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::UNGRABPOINTER );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "time" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->time, byteswap,
                         { UngrabPointer::time_names } ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GrabButton >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GrabButton;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GrabButton::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< GrabButton > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::GRABBUTTON );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "keyboard-mode" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "owner-events", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->owner_events, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "grab-window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->grab_window, byteswap ), ws.separator,
        ws.memb_indent, "event-mask", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->event_mask, byteswap ), ws.separator,
        ws.memb_indent, "pointer-mode", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->pointer_mode, byteswap,
                         { GrabButton::pointer_mode_names } ), ws.separator,
        ws.memb_indent, "keyboard-mode", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->keyboard_mode, byteswap,
                         { GrabButton::keyboard_mode_names } ), ws.separator,
        ws.memb_indent, "confine-to", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->confine_to, byteswap,
                         { GrabButton::confine_to_names } ), ws.separator,
        ws.memb_indent, "cursor", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cursor, byteswap,
                         { GrabButton::cursor_names } ), ws.separator,
        ws.memb_indent, "button", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->button, byteswap,
                         { GrabButton::button_names } ), ws.separator,
        ws.memb_indent, "modifiers", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->modifiers, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UngrabButton >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::UngrabButton;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= UngrabButton::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< UngrabButton > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::UNGRABBUTTON );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "grab-window" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "button", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->button, byteswap,
                         { UngrabButton::button_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "grab-window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->grab_window, byteswap ), ws.separator,
        ws.memb_indent, "modifiers", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->modifiers, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeActivePointerGrab >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ChangeActivePointerGrab;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ChangeActivePointerGrab::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ChangeActivePointerGrab > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CHANGEACTIVEPOINTERGRAB );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "event-mask" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cursor", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cursor, byteswap,
                         { ChangeActivePointerGrab::cursor_names } ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->time, byteswap,
                         { ChangeActivePointerGrab::time_names } ), ws.separator,
        ws.memb_indent, "event-mask", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->event_mask, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GrabKeyboard >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GrabKeyboard;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GrabKeyboard::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< GrabKeyboard > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::GRABKEYBOARD );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "keyboard-mode" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "owner-events", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->owner_events, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "grab-window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->grab_window, byteswap ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->time, byteswap,
                         { GrabKeyboard::time_names } ), ws.separator,
        ws.memb_indent, "pointer-mode", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->pointer_mode, byteswap,
                         { GrabKeyboard::pointer_mode_names } ), ws.separator,
        ws.memb_indent, "keyboard-mode", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->keyboard_mode, byteswap,
                         { GrabKeyboard::keyboard_mode_names } ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UngrabKeyboard >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::UngrabKeyboard;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= UngrabKeyboard::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< UngrabKeyboard > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::UNGRABKEYBOARD );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "time" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->time, byteswap,
                         { UngrabKeyboard::time_names } ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GrabKey >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GrabKey;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GrabKey::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< GrabKey > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::GRABKEY );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "keyboard-mode" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "owner-events", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->owner_events, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "modifiers", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->modifiers, byteswap ), ws.separator,
        ws.memb_indent, "key", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->key, byteswap,
                         { GrabKey::key_names } ), ws.separator,
        ws.memb_indent, "pointer-mode", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->pointer_mode, byteswap,
                         { GrabKey::pointer_mode_names } ), ws.separator,
        ws.memb_indent, "keyboard-mode", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->keyboard_mode, byteswap,
                         { GrabKey::keyboard_mode_names } ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UngrabKey >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::UngrabKey;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= UngrabKey::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< UngrabKey > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::UNGRABKEY );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "grab-window" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "key", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->key, byteswap,
                         UngrabKey::key_names ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "grab-window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->grab_window, byteswap ), ws.separator,
        ws.memb_indent, "modifiers", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->modifiers, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::AllowEvents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::AllowEvents;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= AllowEvents::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< AllowEvents > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::ALLOWEVENTS );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "mode" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->mode, byteswap,
                         { AllowEvents::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->time, byteswap,
                         { AllowEvents::time_names } ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GrabServer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GrabServer;
    using protocol::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, GrabServer > );
    assert( data != nullptr );
    assert( sz >= sizeof( GrabServer::Prefix ) );
    assert( _ordered( reinterpret_cast< const GrabServer::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::GRABSERVER );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UngrabServer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::UngrabServer;
    using protocol::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, UngrabServer > );
    assert( data != nullptr );
    assert( sz >= sizeof( UngrabServer::Prefix ) );
    assert( reinterpret_cast< const UngrabServer::Prefix* >(
                data )->opcode ==
            protocol::requests::opcodes::UNGRABSERVER );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryPointer;
    using protocol::requests::impl::SimpleWindowRequest;
    static_assert( std::is_base_of_v< SimpleWindowRequest, QueryPointer > );
    assert( data != nullptr );
    assert( sz >= sizeof( QueryPointer::Prefix ) );
    assert( _ordered( reinterpret_cast< const QueryPointer::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::QUERYPOINTER );
    return _parseRequest< SimpleWindowRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetMotionEvents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetMotionEvents;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GetMotionEvents::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< GetMotionEvents > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::GETMOTIONEVENTS );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "window" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "start", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->start, byteswap,
                         { GetMotionEvents::start_names } ), ws.separator,
        ws.memb_indent, "stop", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->stop, byteswap,
                         { GetMotionEvents::stop_names } ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::TranslateCoordinates >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::TranslateCoordinates;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= TranslateCoordinates::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< TranslateCoordinates > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::TRANSLATECOORDINATES );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "src-window" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "src-window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_window, byteswap ), ws.separator,
        ws.memb_indent, "dst-window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_window, byteswap ), ws.separator,
        ws.memb_indent, "src-x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_x, byteswap ), ws.separator,
        ws.memb_indent, "src-y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_y, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::WarpPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::WarpPointer;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= WarpPointer::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< WarpPointer > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::WARPPOINTER );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "src-window" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "src-window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_window, byteswap,
                         { WarpPointer::src_window_names } ), ws.separator,
        ws.memb_indent, "dst-window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_window, byteswap,
                         { WarpPointer::dst_window_names } ), ws.separator,
        ws.memb_indent, "src-x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_x, byteswap ), ws.separator,
        ws.memb_indent, "src-y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_y, byteswap ), ws.separator,
        ws.memb_indent, "src-width", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_width, byteswap ), ws.separator,
        ws.memb_indent, "src-height", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_height, byteswap ), ws.separator,
        ws.memb_indent, "dst-x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_x, byteswap ), ws.separator,
        ws.memb_indent, "dst-y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_y, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetInputFocus >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SetInputFocus;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SetInputFocus::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SetInputFocus > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::SETINPUTFOCUS );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "revert-to" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "revert-to", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->revert_to, byteswap,
                         { SetInputFocus::revert_to_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "focus", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->focus, byteswap,
                         { SetInputFocus::focus_names,
                           _EnumNameRange::Bound::MAX,
                           SetInputFocus::FOCUS_ENUM_MAX } ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->time, byteswap,
                         { SetInputFocus::time_names } ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetInputFocus >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetInputFocus;
    using protocol::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, GetInputFocus > );
    assert( data != nullptr );
    assert( sz >= sizeof( GetInputFocus::Prefix ) );
    assert( _ordered( reinterpret_cast< const GetInputFocus::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::GETINPUTFOCUS );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryKeymap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryKeymap;
    using protocol::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, QueryKeymap > );
    assert( data != nullptr );
    assert( sz >= sizeof( QueryKeymap::Prefix ) );
    assert( _ordered( reinterpret_cast< const QueryKeymap::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::QUERYKEYMAP );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::OpenFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::OpenFont;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= OpenFont::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< OpenFont > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::OPENFONT );
    // followed by STRING8 name
    const auto name_len { _ordered( fe.encoding->name_len, byteswap ) };
    const std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        name_len };
    request.bytes_parsed += alignment.pad( name_len );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "fid" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "fid", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->fid, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->name_len, byteswap ), ws.separator ),
        ws.memb_indent, "name", memb_name_w, ws.equals,
        name, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CloseFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::CloseFont;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CloseFont::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< CloseFont > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CLOSEFONT );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "font" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "font", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->font, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryFont;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= QueryFont::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< QueryFont > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::QUERYFONT );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "font" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "font", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->font, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryTextExtents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryTextExtents;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= QueryTextExtents::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< QueryTextExtents > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::QUERYTEXTEXTENTS );
    // followed by STRING16 string
    // - first calc padded string length due to ambiguity around odd-length
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t string_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? QueryTextExtents::BASE_BIG_ENCODING_SZ :
                           QueryTextExtents::BASE_ENCODING_SZ ) };
    const size_t string_len {
        ( string_sz / sizeof( protocol::CHAR2B/*char16_t*/ ) ) -
        ( _ordered( fe.prefix->odd_length.data, byteswap ) ? 1 : 0 ) };
    _ParsingOutputs string {
        _parseLISTof< protocol::CHAR2B >(
            data + request.bytes_parsed, string_sz, string_len,
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    // - bypass expected use of string.bytes_parsed due to odd-length
    request.bytes_parsed += string_sz;
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "string" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(odd length)", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->odd_length, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "font", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->font, byteswap ), ws.separator,
        ws.memb_indent, "string", memb_name_w, ws.equals,
        string.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ListFonts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ListFonts;
    using protocol::requests::impl::ListFontsRequest;
    static_assert( std::is_base_of_v< ListFontsRequest, ListFonts > );
    assert( data != nullptr );
    assert( sz >= sizeof( ListFonts::Prefix ) );
    assert( _ordered( reinterpret_cast< const ListFonts::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::LISTFONTS );
    return _parseRequest< ListFontsRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ListFontsWithInfo >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ListFontsWithInfo;
    using protocol::requests::impl::ListFontsRequest;
    static_assert( std::is_base_of_v< ListFontsRequest, ListFontsWithInfo > );
    assert( data != nullptr );
    assert( sz >= sizeof( ListFontsWithInfo::Prefix ) );
    assert( _ordered( reinterpret_cast< const ListFontsWithInfo::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::LISTFONTSWITHINFO );
    return _parseRequest< ListFontsRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetFontPath >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SetFontPath;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SetFontPath::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SetFontPath > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::SETFONTPATH );
    // followed by LISTofSTR path
    const _ParsingOutputs path {
        _parseLISTof< protocol::STR >(
            data + request.bytes_parsed, sz - request.bytes_parsed,
            _ordered( fe.encoding->path_ct, byteswap ),
            byteswap, ws.nested() ) };
    request.bytes_parsed += alignment.pad( path.bytes_parsed );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "path" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(STRs in path)", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->path_ct, byteswap ), ws.separator ),
        ws.memb_indent, "path", memb_name_w, ws.equals,
        path.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetFontPath >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetFontPath;
    using protocol::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, GetFontPath > );
    assert( data != nullptr );
    assert( sz >= sizeof( GetFontPath::Prefix ) );
    assert( _ordered( reinterpret_cast< const GetFontPath::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::GETFONTPATH );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CreatePixmap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::CreatePixmap;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CreatePixmap::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< CreatePixmap > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CREATEPIXMAP );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->depth, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "pid", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->pid, byteswap ), ws.separator,
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->height, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::FreePixmap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::FreePixmap;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= FreePixmap::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< FreePixmap > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::FREEPIXMAP );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "pixmap" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "pixmap", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->pixmap, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CreateGC >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::CreateGC;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CreateGC::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< CreateGC > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CREATEGC );
    // followed by LISTofVALUE value-list
    const std::vector< _ValueTraits > value_traits {
        /* function              */ {
            _EnumNameRange{ CreateGC::function_names } },
        /* plane-mask            */ {
            {}, _ValueTraits::BITMASK },
        /* foreground            */ {},
        /* background            */ {},
        /* line-width            */ {},
        /* line-style            */ {
            _EnumNameRange{ CreateGC::line_style_names } },
        /* cap-style             */ {
            _EnumNameRange{ CreateGC::cap_style_names } },
        /* join-style            */ {
            _EnumNameRange{ CreateGC::join_style_names } },
        /* fill-style            */ {
            _EnumNameRange{ CreateGC::fill_style_names } },
        /* fill-rule             */ {
            _EnumNameRange{ CreateGC::fill_rule_names } },
        /* tile                  */ {},
        /* stipple               */ {},
        /* tile-stipple-x-origin */ {},
        /* tile-stipple-y-origin */ {},
        /* font                  */ {},
        /* subwindow-mode        */ {
            _EnumNameRange{ CreateGC::subwindow_mode_names } },
        /* graphics-exposures    */ {},
        /* clip-x-origin         */ {},
        /* clip-y-origin         */ {},
        /* clip-mask             */ {
            _EnumNameRange{ CreateGC::clip_mask_names }, _ValueTraits::BITMASK },
        /* dash-offset           */ {},
        /* dashes                */ {},
        /* arc-mode              */ {
            _EnumNameRange{ CreateGC::arc_mode_names } }
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        data + request.bytes_parsed, sz - request.bytes_parsed,
        _ordered( fe.encoding->value_mask, byteswap ),
        CreateGC::value_types, CreateGC::value_names, value_traits,
        byteswap, ws.nested() };
    _LISTofVALUEParsingOutputs value_list;
    _parseLISTofVALUE( value_list_inputs, &value_list );
    request.bytes_parsed += value_list.bytes_parsed;
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "value-list" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cid", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cid, byteswap ), ws.separator,
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "value-mask", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->value_mask, byteswap,
                             { CreateGC::value_names },
                             _ValueTraits::BITMASK ), ws.separator ),
        ws.memb_indent, "value-list", memb_name_w, ws.equals,
        value_list.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeGC >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ChangeGC;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ChangeGC::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ChangeGC > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CHANGEGC );
    // followed by LISTofVALUE value-list
    const std::vector< _ValueTraits > value_traits {
        /* function              */ {
            _EnumNameRange{ ChangeGC::function_names } },
        /* plane-mask            */ {
            {}, _ValueTraits::BITMASK },
        /* foreground            */ {},
        /* background            */ {},
        /* line-width            */ {},
        /* line-style            */ {
            _EnumNameRange{ ChangeGC::line_style_names } },
        /* cap-style             */ {
            _EnumNameRange{ ChangeGC::cap_style_names } },
        /* join-style            */ {
            _EnumNameRange{ ChangeGC::join_style_names } },
        /* fill-style            */ {
            _EnumNameRange{ ChangeGC::fill_style_names } },
        /* fill-rule             */ {
            _EnumNameRange{ ChangeGC::fill_rule_names } },
        /* tile                  */ {},
        /* stipple               */ {},
        /* tile-stipple-x-origin */ {},
        /* tile-stipple-y-origin */ {},
        /* font                  */ {},
        /* subwindow-mode        */ {
            _EnumNameRange{ ChangeGC::subwindow_mode_names } },
        /* graphics-exposures    */ {},
        /* clip-x-origin         */ {},
        /* clip-y-origin         */ {},
        /* clip-mask             */ {
            _EnumNameRange { ChangeGC::clip_mask_names }, _ValueTraits::BITMASK },
        /* dash-offset           */ {},
        /* dashes                */ {},
        /* arc-mode              */ {
            _EnumNameRange{ ChangeGC::arc_mode_names } }
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        data + request.bytes_parsed, sz - request.bytes_parsed,
        _ordered( fe.encoding->value_mask, byteswap ),
        ChangeGC::value_types, ChangeGC::value_names, value_traits,
        byteswap, ws.nested() };
    _LISTofVALUEParsingOutputs value_list;
    _parseLISTofVALUE( value_list_inputs, &value_list );
    request.bytes_parsed += value_list.bytes_parsed;
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "value-list" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "value-mask", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->value_mask, byteswap,
                             { ChangeGC::value_names },
                             _ValueTraits::BITMASK ), ws.separator ),
        ws.memb_indent, "value-list", memb_name_w, ws.equals,
        value_list.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CopyGC >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::CopyGC;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CopyGC::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< CopyGC > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::COPYGC );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "value-mask" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "src-gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_gc, byteswap ), ws.separator,
        ws.memb_indent, "dst-gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_gc, byteswap ), ws.separator,
        ws.memb_indent, "value-mask", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->value_mask, byteswap,
                         { CopyGC::value_names },
                         _ValueTraits::BITMASK ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetDashes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SetDashes;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SetDashes::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SetDashes > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::SETDASHES );
    // followed by LISTofCARD8 dashes
    _ParsingOutputs dashes {
        _parseLISTof< protocol::CARD8 >(
            data + request.bytes_parsed, sz - request.bytes_parsed,
            _ordered( fe.encoding->dashes_len, byteswap ),
            byteswap, ws.nested() ) };
    request.bytes_parsed += alignment.pad( dashes.bytes_parsed );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "dash-offset" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "dash-offset", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dash_offset, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(dashes length)", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->dashes_len, byteswap ), ws.separator ),
        ws.memb_indent, "dashes", memb_name_w, ws.equals,
        dashes.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetClipRectangles >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SetClipRectangles;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SetClipRectangles::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SetClipRectangles > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::SETCLIPRECTANGLES );
    // followed by LISTofRECTANGLE rectangles
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t rectangles_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? SetClipRectangles::BASE_BIG_ENCODING_SZ :
                           SetClipRectangles::BASE_ENCODING_SZ ) };
    const size_t rectangles_ct { rectangles_sz / sizeof( protocol::RECTANGLE ) };
    _ParsingOutputs rectangles {
        _parseLISTof< protocol::RECTANGLE >(
            data + request.bytes_parsed, rectangles_sz, rectangles_ct,
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( rectangles.bytes_parsed );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "clip-x-origin" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "ordering", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->ordering, byteswap,
                         { SetClipRectangles::ordering_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "clip-x-origin", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->clip_x_origin, byteswap ), ws.separator,
        ws.memb_indent, "clip-y-origin", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->clip_y_origin, byteswap ), ws.separator,
        ws.memb_indent, "rectangles", memb_name_w, ws.equals,
        rectangles.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::FreeGC >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::FreeGC;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= FreeGC::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< FreeGC > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::FREEGC );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "gc" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ClearArea >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ClearArea;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ClearArea::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ClearArea > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CLEARAREA );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "exposures" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "exposures", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->exposures, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->height, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CopyArea >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::CopyArea;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CopyArea::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< CopyArea > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::COPYAREA );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "src-drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "src-drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_drawable, byteswap ), ws.separator,
        ws.memb_indent, "dst-drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "src-x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_x, byteswap ), ws.separator,
        ws.memb_indent, "src-y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_y, byteswap ), ws.separator,
        ws.memb_indent, "dst-x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_x, byteswap ), ws.separator,
        ws.memb_indent, "dst-y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_y, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->height, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CopyPlane >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::CopyPlane;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CopyPlane::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< CopyPlane > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::COPYPLANE );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "src-drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "src-drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_drawable, byteswap ), ws.separator,
        ws.memb_indent, "dst-drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "src-x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_x, byteswap ), ws.separator,
        ws.memb_indent, "src-y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_y, byteswap ), ws.separator,
        ws.memb_indent, "dst-x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_x, byteswap ), ws.separator,
        ws.memb_indent, "dst-y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_y, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->height, byteswap ), ws.separator,
        ws.memb_indent, "bit-plane", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->bit_plane, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyPoint >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::PolyPoint;
    using protocol::requests::impl::PolyPointRequest;
    static_assert( std::is_base_of_v< PolyPointRequest, PolyPoint > );
    assert( data != nullptr );
    assert( sz >= sizeof( PolyPoint::Prefix ) );
    assert( _ordered( reinterpret_cast< const PolyPoint::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::POLYPOINT );
    return _parseRequest< PolyPointRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyLine >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::PolyLine;
    using protocol::requests::impl::PolyPointRequest;
    static_assert( std::is_base_of_v< PolyPointRequest, PolyLine > );
    assert( data != nullptr );
    assert( sz >= sizeof( PolyLine::Prefix ) );
    assert( _ordered( reinterpret_cast< const PolyLine::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::POLYLINE );
    return _parseRequest< PolyPointRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolySegment >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::PolySegment;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= PolySegment::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< PolySegment > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::POLYSEGMENT );
    // followed by LISTofSEGMENT segments
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t segments_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? PolySegment::BASE_BIG_ENCODING_SZ :
                           PolySegment::BASE_ENCODING_SZ ) };
    const size_t segments_ct { segments_sz / sizeof( PolySegment::SEGMENT ) };
    const _ParsingOutputs segments {
        _parseLISTof< PolySegment::SEGMENT >(
            data + request.bytes_parsed, segments_sz, segments_ct,
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( segments.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "segments", memb_name_w, ws.equals,
        segments.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyRectangle >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::PolyRectangle;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= PolyRectangle::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< PolyRectangle > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::POLYRECTANGLE );
    // followed by LISTofRECTANGLE rectangles
    const auto tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t rectangles_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? PolyRectangle::BASE_BIG_ENCODING_SZ :
                           PolyRectangle::BASE_ENCODING_SZ ) };
    const size_t rectangles_ct { rectangles_sz / sizeof( protocol::RECTANGLE ) };
    _ParsingOutputs rectangles {
        _parseLISTof< protocol::RECTANGLE >(
            data + request.bytes_parsed, rectangles_sz, rectangles_ct,
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( rectangles.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "rectangles" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "rectangles", memb_name_w, ws.equals,
        rectangles.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyArc >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::PolyArc;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= PolyArc::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< PolyArc > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::POLYARC );
    // followed by LISTofARC arcs
    const auto tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t arcs_sz {
        tl_aligned_units - ( fe.big_request ? PolyArc::BASE_BIG_ENCODING_SZ :
                                              PolyArc::BASE_ENCODING_SZ ) };
    const size_t arcs_ct { arcs_sz / sizeof( protocol::ARC ) };
    const _ParsingOutputs arcs {
        _parseLISTof< protocol::ARC >(
            data + request.bytes_parsed, arcs_sz, arcs_ct,
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( arcs.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "arcs", memb_name_w, ws.equals,
        arcs.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::FillPoly >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::FillPoly;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= FillPoly::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< FillPoly > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::FILLPOLY );
    // followed by LISTofPOINT points
    const auto tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t points_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? FillPoly::BASE_BIG_ENCODING_SZ :
                           FillPoly::BASE_ENCODING_SZ ) };
    const size_t points_ct { points_sz / sizeof( protocol::POINT ) };
    const _ParsingOutputs points {
        _parseLISTof< protocol::POINT >(
            data + request.bytes_parsed, points_sz, points_ct,
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( points.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "coordinate-mode" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "shape", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->shape, byteswap,
                         { FillPoly::shape_names } ), ws.separator,
        ws.memb_indent, "coordinate-mode", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->coordinate_mode, byteswap,
                         { FillPoly::coordinate_mode_names } ), ws.separator,
        ws.memb_indent, "points", memb_name_w, ws.equals,
        points.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyFillRectangle >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::PolyFillRectangle;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= PolyFillRectangle::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< PolyFillRectangle > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::POLYFILLRECTANGLE );
    // followed by LISTofRECTANGLE rectangles
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t rectangles_sz {
        alignment.size( tl_aligned_units ) - sizeof( PolyFillRectangle::Encoding ) };
    const uint16_t rectangles_ct ( rectangles_sz / sizeof( protocol::RECTANGLE ) );
    const _ParsingOutputs rectangles {
        _parseLISTof< protocol::RECTANGLE >(
            data + request.bytes_parsed, rectangles_sz, rectangles_ct,
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( rectangles.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "rectangles" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "rectangles", memb_name_w, ws.equals,
        rectangles.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyFillArc >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::PolyFillArc;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= PolyFillArc::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< PolyFillArc > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::POLYFILLARC );
    // followed by LISTofARC arcs
    const auto tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t arcs_sz {
        alignment.size( tl_aligned_units ) - sizeof( PolyFillArc::Encoding ) };
    const uint16_t arcs_ct ( arcs_sz  / sizeof( protocol::ARC ) );
    const _ParsingOutputs arcs {
        _parseLISTof< protocol::ARC >(
            data + request.bytes_parsed, arcs_sz, arcs_ct,
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( arcs.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "arcs", memb_name_w, ws.equals,
        arcs.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PutImage >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::PutImage;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= PutImage::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< PutImage > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::PUTIMAGE );
    // followed by LISTofBYTE data
    const auto tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t data_len {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? PutImage::BASE_BIG_ENCODING_SZ :
                           PutImage::BASE_ENCODING_SZ ) };
    // - as in xtrace, we will only print the size in bytes of the image data
    request.bytes_parsed += alignment.pad( data_len );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}({} bytes){}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "format", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->format, byteswap,
                         { PutImage::format_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->height, byteswap ), ws.separator,
        ws.memb_indent, "dst-x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_x, byteswap ), ws.separator,
        ws.memb_indent, "dst-y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->dst_y, byteswap ), ws.separator,
        ws.memb_indent, "left-pad", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->left_pad, byteswap ), ws.separator,
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->depth, byteswap ), ws.separator,
        ws.memb_indent, "data", memb_name_w, ws.equals,
        data_len, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetImage >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetImage;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GetImage::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< GetImage > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::GETIMAGE );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "plane-mask" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
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
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "format", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->format, byteswap,
                         { GetImage::format_names, _EnumNameRange::Bound::MIN,
                           GetImage::FORMAT_ENUM_MIN } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->height, byteswap ), ws.separator,
        ws.memb_indent, "plane-mask", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->plane_mask, byteswap,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyText8 >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::PolyText8;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= PolyText8::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< PolyText8 > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::POLYTEXT8 );
    // followed by LISTofTEXTITEM8 items
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t items_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? PolyText8::BASE_BIG_ENCODING_SZ :
                           PolyText8::BASE_ENCODING_SZ ) };
    // TEXTITEM8 count undetermined due to their variable length
    const _ParsingOutputs items {
        _parseLISTof< PolyText8::TEXTITEM8 >(
            data + request.bytes_parsed, items_sz,
            byteswap, ws.nested() ) };
    request.bytes_parsed += alignment.pad( items.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "items", memb_name_w, ws.equals,
        items.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyText16 >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::PolyText16;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= PolyText16::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< PolyText16 > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::POLYTEXT16 );
    // followed by LISTofTEXTITEM16 items
    const auto tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t items_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? PolyText16::BASE_BIG_ENCODING_SZ :
                           PolyText16::BASE_ENCODING_SZ ) };
    // TEXTITEM8 count undetermined due to their variable length
    const _ParsingOutputs items {
        _parseLISTof< PolyText16::TEXTITEM16 >(
            data + request.bytes_parsed, items_sz,
            byteswap, ws.nested() ) };
    request.bytes_parsed += alignment.pad( items.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "items", memb_name_w, ws.equals,
        items.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ImageText8 >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ImageText8;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ImageText8::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ImageText8 > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::IMAGETEXT8 );
    // followed by STRING8 string
    const auto string_len { _ordered( fe.prefix->string_len, byteswap ) };
    const std::string_view string {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        string_len };
    request.bytes_parsed += alignment.pad( string_len );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(string length)", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->string_len, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "string", memb_name_w, ws.equals,
        string, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ImageText16 >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ImageText16;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ImageText16::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ImageText16 > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::IMAGETEXT16 );
    // followed by STRING16 string
    const auto string_2B_len {
        _ordered( fe.prefix->string_2B_len, byteswap) };
    const size_t string_sz { string_2B_len * sizeof( protocol::CHAR2B ) };
    const _ParsingOutputs string {
        _parseLISTof< protocol::CHAR2B >(
            data + request.bytes_parsed, string_sz, string_2B_len,
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    request.bytes_parsed += alignment.pad( string.bytes_parsed );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}[{}]{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(string len (CHAR2B))", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->string_2B_len, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->gc, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->y, byteswap ), ws.separator,
        ws.memb_indent, "string", memb_name_w, ws.equals,
        string.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CreateColormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::CreateColormap;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CreateColormap::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< CreateColormap > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CREATECOLORMAP );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "window" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "alloc", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->alloc, byteswap,
                         { CreateColormap::alloc_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "mid", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->mid, byteswap ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        ws.memb_indent, "visual", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->visual, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::FreeColormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::FreeColormap;
    using protocol::requests::impl::SimpleCmapRequest;
    static_assert( std::is_base_of_v< SimpleCmapRequest, FreeColormap > );
    assert( data != nullptr );
    assert( sz >= sizeof( FreeColormap::Prefix ) );
    assert( _ordered( reinterpret_cast< const FreeColormap::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::FREECOLORMAP );
    return _parseRequest< SimpleCmapRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CopyColormapAndFree >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::CopyColormapAndFree;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CopyColormapAndFree::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< CopyColormapAndFree > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::COPYCOLORMAPANDFREE );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "src-cmap" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "mid", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->mid, byteswap ), ws.separator,
        ws.memb_indent, "src-cmap", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->src_cmap, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::InstallColormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::InstallColormap;
    using protocol::requests::impl::SimpleCmapRequest;
    static_assert( std::is_base_of_v< SimpleCmapRequest, InstallColormap > );
    assert( data != nullptr );
    assert( sz >= sizeof( InstallColormap::Prefix ) );
    assert( _ordered( reinterpret_cast< const InstallColormap::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::INSTALLCOLORMAP );
    return _parseRequest< SimpleCmapRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UninstallColormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::UninstallColormap;
    using protocol::requests::impl::SimpleCmapRequest;
    static_assert( std::is_base_of_v< SimpleCmapRequest, UninstallColormap > );
    assert( data != nullptr );
    assert( sz >= sizeof( UninstallColormap::Prefix ) );
    assert( _ordered( reinterpret_cast< const UninstallColormap::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::UNINSTALLCOLORMAP );
    return _parseRequest< SimpleCmapRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ListInstalledColormaps >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ListInstalledColormaps;
    using protocol::requests::impl::SimpleWindowRequest;
    static_assert( std::is_base_of_v< SimpleWindowRequest, ListInstalledColormaps > );
    assert( data != nullptr );
    assert( sz >= sizeof( ListInstalledColormaps::Prefix ) );
    assert( _ordered( reinterpret_cast< const ListInstalledColormaps::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::LISTINSTALLEDCOLORMAPS );
    return _parseRequest< SimpleWindowRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::AllocColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::AllocColor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= AllocColor::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< AllocColor > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::ALLOCCOLOR );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "green" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cmap, byteswap ), ws.separator,
        ws.memb_indent, "red", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->red, byteswap ), ws.separator,
        ws.memb_indent, "green", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->green, byteswap ), ws.separator,
        ws.memb_indent, "blue", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->blue, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::AllocNamedColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::AllocNamedColor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= AllocNamedColor::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< AllocNamedColor > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::ALLOCNAMEDCOLOR );
    // followed by STRING8 name
    const auto name_len {
        _ordered( fe.encoding->name_len, byteswap ) };
    const std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        name_len };
    request.bytes_parsed += alignment.pad( name_len );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "cmap" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cmap, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->name_len, byteswap ), ws.separator ),
        ws.memb_indent, "name", memb_name_w, ws.equals,
        name, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::AllocColorCells >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::AllocColorCells;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= AllocColorCells::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< AllocColorCells > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::ALLOCCOLORCELLS );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "contiguous" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "contiguous", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->contiguous, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cmap, byteswap ), ws.separator,
        ws.memb_indent, "colors", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->colors, byteswap ), ws.separator,
        ws.memb_indent, "planes", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->planes, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::AllocColorPlanes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::AllocColorPlanes;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= AllocColorPlanes::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< AllocColorPlanes > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::ALLOCCOLORPLANES );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "contiguous" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "contiguous", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->contiguous, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cmap, byteswap ), ws.separator,
        ws.memb_indent, "colors", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->colors, byteswap ), ws.separator,
        ws.memb_indent, "reds", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->reds, byteswap ), ws.separator,
        ws.memb_indent, "greens", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->greens, byteswap ), ws.separator,
        ws.memb_indent, "blues", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->blues, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::FreeColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::FreeColors;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= FreeColors::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< FreeColors > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::FREECOLORS );
    // followed by LISTofCARD32 pixels
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t pixels_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? FreeColors::BASE_BIG_ENCODING_SZ :
                           FreeColors::BASE_ENCODING_SZ ) };
    const size_t pixels_ct { pixels_sz / sizeof( protocol::CARD32 ) };
    const _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + request.bytes_parsed, pixels_sz, pixels_ct,
            byteswap, ws.nested() ) };
    request.bytes_parsed += alignment.pad( pixels.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "plane-mask" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cmap, byteswap ), ws.separator,
        ws.memb_indent, "plane-mask", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->plane_mask, byteswap,
                         {}, _ValueTraits::BITMASK ), ws.separator,
        ws.memb_indent, "pixels", memb_name_w, ws.equals,
        pixels.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::StoreColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::StoreColors;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= StoreColors::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< StoreColors > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::STORECOLORS );
    // followed by LISTofCOLORITEM items
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t items_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? StoreColors::BASE_BIG_ENCODING_SZ :
                           StoreColors::BASE_ENCODING_SZ ) };
    const size_t items_ct { items_sz / sizeof( StoreColors::COLORITEM ) };
    const _ParsingOutputs items {
        _parseLISTof< StoreColors::COLORITEM >(
            data + request.bytes_parsed, items_sz, items_ct,
            byteswap, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( items.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "items" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cmap, byteswap ), ws.separator,
        ws.memb_indent, "items", memb_name_w, ws.equals,
        items.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::StoreNamedColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::StoreNamedColor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= StoreNamedColor::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< StoreNamedColor > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::STORENAMEDCOLOR );
    // followed by STRING8 name
    const auto name_len {
        _ordered( fe.encoding->name_len, byteswap ) };
    const std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        name_len };
    request.bytes_parsed += alignment.pad( name_len );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "(do rgb mask)" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "(do rgb mask)", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->do_rgb_mask, byteswap,
                         { StoreNamedColor::do_rgb_names },
                         _ValueTraits::BITMASK ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cmap, byteswap ), ws.separator,
        ws.memb_indent, "pixel", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->pixel, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->name_len, byteswap ), ws.separator ),
        ws.memb_indent, "name", memb_name_w, ws.equals,
        name, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryColors;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= QueryColors::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< QueryColors > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::QUERYCOLORS );
    // followed by LISTofCARD32 pixels
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t pixels_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? QueryColors::BASE_BIG_ENCODING_SZ :
                           QueryColors::BASE_ENCODING_SZ ) };
    const size_t pixels_ct { pixels_sz / sizeof( protocol::CARD32 ) };
    const _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + request.bytes_parsed, pixels_sz, pixels_ct,
            byteswap, ws.nested() ) };
    request.bytes_parsed += alignment.pad( pixels.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "pixels" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cmap, byteswap ), ws.separator,
        ws.memb_indent, "pixels", memb_name_w, ws.equals,
        pixels.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::LookupColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::LookupColor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= LookupColor::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< LookupColor > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::LOOKUPCOLOR );
    // followed by STRING8 name
    const auto name_len {
        _ordered( fe.encoding->name_len, byteswap ) };
    const std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        name_len };
    request.bytes_parsed += alignment.pad( name_len );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "cmap" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cmap, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->name_len, byteswap ), ws.separator ),
        ws.memb_indent, "name", memb_name_w, ws.equals,
        name, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CreateCursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::CreateCursor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CreateCursor::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< CreateCursor > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CREATECURSOR );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "fore-green" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cid", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cid, byteswap ), ws.separator,
        ws.memb_indent, "source", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->source, byteswap ), ws.separator,
        ws.memb_indent, "mask", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->mask, byteswap,
                         { CreateCursor::mask_names } ), ws.separator,
        ws.memb_indent, "fore-red", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->fore_red, byteswap ), ws.separator,
        ws.memb_indent, "fore-green", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->fore_green, byteswap ), ws.separator,
        ws.memb_indent, "fore-blue", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->fore_blue, byteswap ), ws.separator,
        ws.memb_indent, "back-red", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->back_red, byteswap ), ws.separator,
        ws.memb_indent, "back-green", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->back_green, byteswap ), ws.separator,
        ws.memb_indent, "back-blue", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->back_blue, byteswap ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->x, byteswap ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->y, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CreateGlyphCursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::CreateGlyphCursor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= CreateGlyphCursor::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< CreateGlyphCursor > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CREATEGLYPHCURSOR );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "source-font" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cid", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cid, byteswap ), ws.separator,
        ws.memb_indent, "source-font", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->source_font, byteswap ), ws.separator,
        ws.memb_indent, "mask-font", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->mask_font, byteswap,
                         { CreateGlyphCursor::mask_font_names } ), ws.separator,
        ws.memb_indent, "source-char", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->source_char, byteswap ), ws.separator,
        ws.memb_indent, "mask-char", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->mask_char, byteswap ), ws.separator,
        ws.memb_indent, "fore-red", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->fore_red, byteswap ), ws.separator,
        ws.memb_indent, "fore-green", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->fore_green, byteswap ), ws.separator,
        ws.memb_indent, "fore-blue", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->fore_blue, byteswap ), ws.separator,
        ws.memb_indent, "back-red", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->back_red, byteswap ), ws.separator,
        ws.memb_indent, "back-green", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->back_green, byteswap ), ws.separator,
        ws.memb_indent, "back-blue", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->back_blue, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::FreeCursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::FreeCursor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= FreeCursor::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< FreeCursor > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::FREECURSOR );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "cursor" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cursor", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cursor, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::RecolorCursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::RecolorCursor;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= RecolorCursor::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< RecolorCursor > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::RECOLORCURSOR );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "fore-green" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "cursor", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->cursor, byteswap ), ws.separator,
        ws.memb_indent, "fore-red", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->fore_red, byteswap ), ws.separator,
        ws.memb_indent, "fore-green", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->fore_green, byteswap ), ws.separator,
        ws.memb_indent, "fore-blue", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->fore_blue, byteswap ), ws.separator,
        ws.memb_indent, "back-red", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->back_red, byteswap ), ws.separator,
        ws.memb_indent, "back-green", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->back_green, byteswap ), ws.separator,
        ws.memb_indent, "back-blue", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->back_blue, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryBestSize >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryBestSize;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= QueryBestSize::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< QueryBestSize > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::QUERYBESTSIZE );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "drawable" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "class", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->class_, byteswap,
                         { QueryBestSize::class_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->drawable, byteswap ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->width, byteswap ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->height, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryExtension >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::QueryExtension;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= QueryExtension::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< QueryExtension > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::QUERYEXTENSION );
    // followed by STRING8 name
    const auto name_len { _ordered( fe.encoding->name_len, byteswap ) };
    const std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        name_len };
    request.bytes_parsed += alignment.pad( name_len );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    // Stash copy of extension name until reply comes in - at that time we will
    //   use it in the activation of the extension
    _stashString( conn->id, conn->sequence, name );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "name" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->name_len, byteswap ), ws.separator ),
        ws.memb_indent, "name", memb_name_w, ws.equals,
        name, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ListExtensions >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ListExtensions;
    using protocol::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, ListExtensions > );
    assert( data != nullptr );
    assert( sz >= sizeof( ListExtensions::Prefix ) );
    assert( _ordered( reinterpret_cast< const ListExtensions::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::LISTEXTENSIONS );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeKeyboardMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ChangeKeyboardMapping;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ChangeKeyboardMapping::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ChangeKeyboardMapping > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CHANGEKEYBOARDMAPPING );
    // followed by LISTofKEYSYM keysyms
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t keysyms_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? ChangeKeyboardMapping::BASE_BIG_ENCODING_SZ :
                           ChangeKeyboardMapping::BASE_ENCODING_SZ ) };
    const uint16_t keysyms_ct (
        fe.prefix->keycode_count * fe.encoding->keysyms_per_keycode );
    assert( keysyms_ct == keysyms_sz / sizeof( protocol::KEYSYM ) );
    const _ParsingOutputs keysyms {
        _parseLISTof< protocol::KEYSYM >(
            data + request.bytes_parsed, keysyms_sz, keysyms_ct,
            byteswap, ws.nested() ) };
    request.bytes_parsed += alignment.pad( keysyms.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "keysyms-per-keycode" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "keycode-count", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->keycode_count, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "first-keycode", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->first_keycode, byteswap ), ws.separator,
        ws.memb_indent, "keysyms-per-keycode", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->keysyms_per_keycode, byteswap ), ws.separator,
        ws.memb_indent, "keysyms", memb_name_w, ws.equals,
        keysyms.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetKeyboardMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetKeyboardMapping;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= GetKeyboardMapping::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< GetKeyboardMapping > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::GETKEYBOARDMAPPING );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "first-keycode" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "first-keycode", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->first_keycode, byteswap ), ws.separator,
        ws.memb_indent, "count", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->count, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeKeyboardControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ChangeKeyboardControl;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ChangeKeyboardControl::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ChangeKeyboardControl > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CHANGEKEYBOARDCONTROL );
    // followed by LISTofVALUE value-list
    const std::vector< _ValueTraits > value_traits {
        /* key-click-percent */ {},
        /* bell-percent      */ {},
        /* bell-pitch        */ {},
        /* bell-duration     */ {},
        /* led               */ {},
        /* led-mode          */ {
            _EnumNameRange{ ChangeKeyboardControl::led_mode_names,
                            _EnumNameRange::Bound::MAX,
                            ChangeKeyboardControl::LED_MODE_ENUM_MAX } },
        /* key               */ {},
        /* auto-repeat-mode  */ {
            _EnumNameRange{ ChangeKeyboardControl::auto_repeat_mode_names } }
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        data + request.bytes_parsed, sz - request.bytes_parsed,
        _ordered( fe.encoding->value_mask, byteswap ),
        ChangeKeyboardControl::value_types, ChangeKeyboardControl::value_names,
        value_traits, byteswap, ws.nested() };
    _LISTofVALUEParsingOutputs value_list;
    _parseLISTofVALUE( value_list_inputs, &value_list );
    request.bytes_parsed += value_list.bytes_parsed;
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "value-list" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "value-mask", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->value_mask, byteswap,
                             {}, _ValueTraits::BITMASK ), ws.separator ),
        ws.memb_indent, "value-list", memb_name_w, ws.equals,
        value_list.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetKeyboardControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetKeyboardControl;
    using protocol::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, GetKeyboardControl > );
    assert( data != nullptr );
    assert( sz >= sizeof( GetKeyboardControl::Prefix ) );
    assert( _ordered( reinterpret_cast< const GetKeyboardControl::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::GETKEYBOARDCONTROL );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::Bell >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::Bell;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= Bell::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< Bell > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::BELL );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "percent" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "percent", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->percent, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangePointerControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ChangePointerControl;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ChangePointerControl::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ChangePointerControl > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CHANGEPOINTERCONTROL );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "acceleration-denominator" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "acceleration-numerator", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->acceleration_numerator, byteswap ), ws.separator,
        ws.memb_indent, "acceleration-denominator", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->acceleration_denominator, byteswap ), ws.separator,
        ws.memb_indent, "threshold", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->threshold, byteswap ), ws.separator,
        ws.memb_indent, "do-acceleration", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->do_acceleration, byteswap ), ws.separator,
        ws.memb_indent, "do-threshold", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->do_threshold, byteswap ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetPointerControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetPointerControl;
    using protocol::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, GetPointerControl > );
    assert( data != nullptr );
    assert( sz >= sizeof( GetPointerControl::Prefix ) );
    assert( _ordered( reinterpret_cast< const GetPointerControl::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::GETPOINTERCONTROL );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetScreenSaver >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SetScreenSaver;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SetScreenSaver::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SetScreenSaver > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::SETSCREENSAVER );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "prefer-blanking" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "timeout", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->timeout, byteswap ), ws.separator,
        ws.memb_indent, "interval", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->interval, byteswap ), ws.separator,
        ws.memb_indent, "prefer-blanking", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->prefer_blanking, byteswap,
                         { SetScreenSaver::prefer_blanking_names } ), ws.separator,
        ws.memb_indent, "allow-exposures", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->allow_exposures, byteswap,
                         { SetScreenSaver::allow_exposures_names } ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetScreenSaver >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetScreenSaver;
    using protocol::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, GetScreenSaver > );
    assert( data != nullptr );
    assert( sz >= sizeof( GetScreenSaver::Prefix ) );
    assert( _ordered( reinterpret_cast< const GetScreenSaver::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::GETSCREENSAVER );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeHosts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ChangeHosts;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ChangeHosts::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ChangeHosts > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::CHANGEHOSTS );
    // followed by LISTofCARD8 address
    // - format as byte array, as we can't guarantee that the family is Internet
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t address_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? ChangeHosts::BASE_BIG_ENCODING_SZ :
                           ChangeHosts::BASE_ENCODING_SZ ) };
    const _ParsingOutputs address {
        _parseLISTof< protocol::CARD8 >(
            data + request.bytes_parsed, address_sz,
            _ordered( fe.encoding->address_len, byteswap ),
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    request.bytes_parsed += alignment.pad( address.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "address" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->mode, byteswap,
                         ChangeHosts::mode_names ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "family", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->family, byteswap,
                         { ChangeHosts::family_names, _EnumNameRange::Bound::MAX,
                           ChangeHosts::FAMILY_ENUM_MAX } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(address length)", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->address_len, byteswap ), ws.separator ),
        ws.memb_indent, "address", memb_name_w, ws.equals,
        address.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ListHosts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ListHosts;
    using protocol::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, ListHosts > );
    assert( data != nullptr );
    assert( sz >= sizeof( ListHosts::Prefix ) );
    assert( _ordered( reinterpret_cast< const ListHosts::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::LISTHOSTS );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetAccessControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SetAccessControl;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SetAccessControl::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SetAccessControl > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::SETACCESSCONTROL );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "mode" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->mode, byteswap,
                         { SetAccessControl::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetCloseDownMode >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SetCloseDownMode;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SetCloseDownMode::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SetCloseDownMode > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::SETCLOSEDOWNMODE );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "mode" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->mode, byteswap,
                         { SetCloseDownMode::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::KillClient >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::KillClient;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= KillClient::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< KillClient > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::KILLCLIENT );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "resource" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "resource", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->resource, byteswap,
                         { KillClient::resource_names } ), ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::RotateProperties >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::RotateProperties;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= RotateProperties::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< RotateProperties > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( !fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::ROTATEPROPERTIES );
    // followed by LISTofATOM properties
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t properties_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? RotateProperties::BASE_BIG_ENCODING_SZ :
                           RotateProperties::BASE_ENCODING_SZ ) };
    const _ParsingOutputs properties {
        _parseLISTof< protocol::ATOM >(
            data + request.bytes_parsed, properties_sz,
            _ordered( fe.encoding->properties_ct, byteswap ),
            byteswap, ws.nested() ) };
    request.bytes_parsed += alignment.pad( properties.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "properties" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->window, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(ATOMs in properties)", memb_name_w, ws.equals,
            _formatVariable( fe.encoding->properties_ct, byteswap ), ws.separator ),
        ws.memb_indent, "delta", memb_name_w, ws.equals,
        _formatVariable( fe.encoding->delta, byteswap ), ws.separator,
        ws.memb_indent, "properties", memb_name_w, ws.equals,
        properties.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ForceScreenSaver >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::ForceScreenSaver;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ForceScreenSaver::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< ForceScreenSaver > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::FORCESCREENSAVER );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "mode" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->mode, byteswap,
                         { ForceScreenSaver::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetPointerMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SetPointerMapping;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SetPointerMapping::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SetPointerMapping > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::SETPOINTERMAPPING );
    // followed by LISTofCARD8 map
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t map_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? SetPointerMapping::BASE_BIG_ENCODING_SZ :
                           SetPointerMapping::BASE_ENCODING_SZ ) };
    const _ParsingOutputs map {
        _parseLISTof< protocol::CARD8 >(
            data + request.bytes_parsed, map_sz,
            _ordered( fe.prefix->map_len, byteswap ),
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    request.bytes_parsed += alignment.pad( map.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        !settings.verbose ? sizeof( "map" ) - 1 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(map length)", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->map_len, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "map", memb_name_w, ws.equals,
        map.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetPointerMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetPointerMapping;
    using protocol::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, GetPointerMapping > );
    assert( data != nullptr );
    assert( sz >= sizeof( GetPointerMapping::Prefix ) );
    assert( _ordered( reinterpret_cast< const GetPointerMapping::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::GETPOINTERMAPPING );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetModifierMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::SetModifierMapping;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SetModifierMapping::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< SetModifierMapping > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::SETMODIFIERMAPPING );
    // followed by LISTofKEYCODE keycodes
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t keycodes_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? SetModifierMapping::BASE_BIG_ENCODING_SZ :
                           SetModifierMapping::BASE_ENCODING_SZ ) };
    const uint16_t keycodes_ct (
        SetModifierMapping::MODIFIER_CT * fe.prefix->keycodes_per_modifier );
    const _ParsingOutputs keycodes {
        _parseLISTof< protocol::KEYCODE >(
            data + request.bytes_parsed, keycodes_sz, keycodes_ct,
            byteswap, ws.nested() ) };
    request.bytes_parsed += alignment.pad( keycodes.bytes_parsed );
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline  ? 0 :
        fe.big_request ? sizeof( "(extended length flag)" ) - 1 :
                         sizeof( "keycodes-per-modifier" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        ws.memb_indent, "keycodes-per-modifier", memb_name_w, ws.equals,
        _formatVariable( fe.prefix->keycodes_per_modifier, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "keycodes", memb_name_w, ws.equals,
        keycodes.str, ws.separator,
        ws.encl_indent
        );
    return request;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetModifierMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::GetModifierMapping;
    using protocol::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, GetModifierMapping > );
    assert( data != nullptr );
    assert( sz >= sizeof( GetModifierMapping::Prefix ) );
    assert( _ordered( reinterpret_cast< const GetModifierMapping::Prefix* >(
                          data )->opcode, conn->byteswap ) ==
            protocol::requests::opcodes::GETMODIFIERMAPPING );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::NoOperation >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::requests::NoOperation;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= NoOperation::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const _RequestFixedEncoding< NoOperation > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( fe.header_only );
    assert( _ordered( fe.prefix->opcode, byteswap ) ==
            protocol::requests::opcodes::NOOPERATION );
    // protocol specifies that no-op may be followed by variable length dummy data
    const uint32_t tl_aligned_units {
        fe.big_request ? _ordered( fe.big_length->tl_aligned_units, byteswap ) :
                         _ordered( fe.length->tl_aligned_units, byteswap ) };
    const size_t dummy_sz {
        alignment.size( tl_aligned_units ) -
        ( fe.big_request ? NoOperation::BASE_BIG_ENCODING_SZ :
                           NoOperation::BASE_ENCODING_SZ ) };
    request.bytes_parsed += dummy_sz;
    assert( tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        ( !ws.multiline || !settings.verbose ) ? 0 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.encl_indent
        );
    return request;
}
