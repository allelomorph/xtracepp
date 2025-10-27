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
    const SimpleRequest::Header* header {
        reinterpret_cast< const SimpleRequest::Header* >( data ) };
    request.bytes_parsed += sizeof( SimpleRequest::Header );
    // SimpleRequest is header-only
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
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
    const SimpleWindowRequest::Header* header {
        reinterpret_cast< const SimpleWindowRequest::Header* >( data ) };
    request.bytes_parsed += sizeof( SimpleWindowRequest::Header );
    const SimpleWindowRequest::Encoding* encoding {
        reinterpret_cast< const SimpleWindowRequest::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( SimpleWindowRequest::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "window" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
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
    const ListFontsRequest::Header* header {
        reinterpret_cast< const ListFontsRequest::Header* >( data ) };
    request.bytes_parsed += sizeof( ListFontsRequest::Header );
    const ListFontsRequest::Encoding* encoding {
        reinterpret_cast< const ListFontsRequest::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ListFontsRequest::Encoding );
    // followed by STRING8 pattern
    std::string_view pattern {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        encoding->pattern_len };
    request.bytes_parsed += alignment.pad( encoding->pattern_len );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "max-names" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "max-names", memb_name_w, ws.equals,
        _formatVariable( encoding->max_names ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(pattern length)", memb_name_w, ws.equals,
            _formatVariable( encoding->pattern_len ), ws.separator ),
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
    const PolyPointRequest::Header* header {
        reinterpret_cast< const PolyPointRequest::Header* >( data ) };
    request.bytes_parsed += sizeof( PolyPointRequest::Header );
    const PolyPointRequest::Encoding* encoding {
        reinterpret_cast< const PolyPointRequest::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( PolyPointRequest::Encoding );
    // followed by LISTofPOINT points
    const size_t points_sz {
        alignment.size( header->tl_aligned_units ) -
        PolyPointRequest::BASE_ENCODING_SZ };
    const size_t points_ct { points_sz / sizeof( protocol::POINT ) };
    const _ParsingOutputs points {
        _parseLISTof< protocol::POINT >(
            data + request.bytes_parsed, points_sz, points_ct,
            ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( points.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "coordinate-mode" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "coordinate-mode", memb_name_w, ws.equals,
        _formatVariable( header->coordinate_mode,
                         { PolyPointRequest::coordinate_mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
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
    const SimpleCmapRequest::Header* header {
        reinterpret_cast< const SimpleCmapRequest::Header* >( data ) };
    request.bytes_parsed += sizeof( SimpleCmapRequest::Header );
    assert( header->opcode == protocol::requests::opcodes::FREECOLORMAP );
    const SimpleCmapRequest::Encoding* encoding {
        reinterpret_cast< const SimpleCmapRequest::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( SimpleCmapRequest::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "cmap" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( encoding->cmap ), ws.separator,
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
    const CreateWindow::Header* header {
        reinterpret_cast< const CreateWindow::Header* >( data ) };
    request.bytes_parsed += sizeof( CreateWindow::Header );
    assert( header->opcode == protocol::requests::opcodes::CREATEWINDOW );
    const CreateWindow::Encoding* encoding {
        reinterpret_cast< const CreateWindow::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( CreateWindow::Encoding );
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
        encoding->value_mask, CreateWindow::value_types,
        CreateWindow::value_names, value_traits, ws.nested() };
    _LISTofVALUEParsingOutputs value_list;
    _parseLISTofVALUE( value_list_inputs, &value_list );
    request.bytes_parsed += value_list.bytes_parsed;
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "border-width" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatVariable( header->depth ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "wid", memb_name_w, ws.equals,
        _formatVariable( encoding->wid ), ws.separator,
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatVariable( encoding->parent ), ws.separator,
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
        ws.memb_indent, "class", memb_name_w, ws.equals,
        _formatVariable( encoding->class_,
                         { CreateWindow::class_names } ), ws.separator,
        ws.memb_indent, "visual", memb_name_w, ws.equals,
        _formatVariable( encoding->visual,
                         { CreateWindow::visual_names } ),
        ws.separator,
        ws.memb_indent, "value-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->value_mask,
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
    const ChangeWindowAttributes::Header* header {
        reinterpret_cast< const ChangeWindowAttributes::Header* >( data ) };
    request.bytes_parsed += sizeof( ChangeWindowAttributes::Header );
    assert( header->opcode == protocol::requests::opcodes::CHANGEWINDOWATTRIBUTES );
    const ChangeWindowAttributes::Encoding* encoding {
        reinterpret_cast< const ChangeWindowAttributes::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ChangeWindowAttributes::Encoding );
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
        encoding->value_mask, ChangeWindowAttributes::value_types,
        ChangeWindowAttributes::value_names, value_traits, ws.nested() };
    _LISTofVALUEParsingOutputs value_list;
    _parseLISTofVALUE( value_list_inputs, &value_list );
    request.bytes_parsed += value_list.bytes_parsed;
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "value-mask" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
        ws.memb_indent, "value-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->value_mask,
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
    assert( sz >= sizeof( GetWindowAttributes::Header ) );
    assert( reinterpret_cast< const GetWindowAttributes::Header* >(
                data )->opcode ==
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
    assert( sz >= sizeof( DestroyWindow::Header ) );
    assert( reinterpret_cast< const DestroyWindow::Header* >(
                data )->opcode ==
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
    assert( sz >= sizeof( DestroySubwindows::Header ) );
    assert( reinterpret_cast< const DestroySubwindows::Header* >(
                data )->opcode ==
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
    const ChangeSaveSet::Header* header {
        reinterpret_cast< const ChangeSaveSet::Header* >( data ) };
    request.bytes_parsed += sizeof( ChangeSaveSet::Header );
    assert( header->opcode == protocol::requests::opcodes::CHANGESAVESET );
    const ChangeSaveSet::Encoding* encoding {
        reinterpret_cast< const ChangeSaveSet::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ChangeSaveSet::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "window" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( header->mode,
                         { ChangeSaveSet::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
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
    const ReparentWindow::Header* header {
        reinterpret_cast< const ReparentWindow::Header* >( data ) };
    request.bytes_parsed += sizeof( ReparentWindow::Header );
    assert( header->opcode == protocol::requests::opcodes::REPARENTWINDOW );
    const ReparentWindow::Encoding* encoding {
        reinterpret_cast< const ReparentWindow::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ReparentWindow::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "window" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
        ws.memb_indent, "parent", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y ), ws.separator,
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
    assert( sz >= sizeof( MapWindow::Header ) );
    assert( reinterpret_cast< const MapWindow::Header* >(
                data )->opcode ==
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
    assert( sz >= sizeof( MapSubwindows::Header ) );
    assert( reinterpret_cast< const MapSubwindows::Header* >(
                data )->opcode ==
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
    assert( sz >= sizeof( UnmapWindow::Header ) );
    assert( reinterpret_cast< const UnmapWindow::Header* >(
                data )->opcode ==
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
    assert( sz >= sizeof( UnmapSubwindows::Header ) );
    assert( reinterpret_cast< const UnmapSubwindows::Header* >(
                data )->opcode ==
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
    const ConfigureWindow::Header* header {
        reinterpret_cast< const ConfigureWindow::Header* >( data ) };
    request.bytes_parsed += sizeof( ConfigureWindow::Header );
    assert( header->opcode == protocol::requests::opcodes::CONFIGUREWINDOW );
    const ConfigureWindow::Encoding* encoding {
        reinterpret_cast< const ConfigureWindow::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ConfigureWindow::Encoding );
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
        encoding->value_mask, ConfigureWindow::value_types,
        ConfigureWindow::value_names, value_traits, ws.nested() };
    _LISTofVALUEParsingOutputs value_list;
    _parseLISTofVALUE( value_list_inputs, &value_list );
    request.bytes_parsed += value_list.bytes_parsed;
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "value-mask" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
        ws.memb_indent, "value-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->value_mask,
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
    const CirculateWindow::Header* header {
        reinterpret_cast< const CirculateWindow::Header* >( data ) };
    request.bytes_parsed += sizeof( CirculateWindow::Header );
    assert( header->opcode == protocol::requests::opcodes::CIRCULATEWINDOW );
    const CirculateWindow::Encoding* encoding {
        reinterpret_cast< const CirculateWindow::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( CirculateWindow::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "direction" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "direction", memb_name_w, ws.equals,
        _formatVariable( header->direction,
                         { CirculateWindow::direction_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
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
    const GetGeometry::Header* header {
        reinterpret_cast< const GetGeometry::Header* >( data ) };
    request.bytes_parsed += sizeof( GetGeometry::Header );
    assert( header->opcode == protocol::requests::opcodes::GETGEOMETRY );
    const GetGeometry::Encoding* encoding {
        reinterpret_cast< const GetGeometry::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( GetGeometry::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
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
    assert( sz >= sizeof( QueryTree::Header ) );
    assert( reinterpret_cast< const QueryTree::Header* >(
                data )->opcode ==
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
    const InternAtom::Header* header {
        reinterpret_cast< const InternAtom::Header* >( data ) };
    request.bytes_parsed += sizeof( InternAtom::Header );
    assert( header->opcode == protocol::requests::opcodes::INTERNATOM );
    const InternAtom::Encoding* encoding {
        reinterpret_cast< const InternAtom::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( InternAtom::Encoding );
    // followed by STRING8 name
    const std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        encoding->name_len };
    request.bytes_parsed += alignment.pad( encoding->name_len );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    // Stash copy of atom until reply comes in - at that time we will include it
    //   in our own internment if it isn't already
    _stashString( { conn->id, conn->sequence }, name );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "only-if-exists" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "only-if-exists", memb_name_w, ws.equals,
        _formatVariable( header->only_if_exists ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( encoding->name_len ), ws.separator ),
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
    const GetAtomName::Header* header {
        reinterpret_cast< const GetAtomName::Header* >( data ) };
    request.bytes_parsed += sizeof( GetAtomName::Header );
    assert( header->opcode == protocol::requests::opcodes::GETATOMNAME );
    const GetAtomName::Encoding* encoding {
        reinterpret_cast< const GetAtomName::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( GetAtomName::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "atom" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "atom", memb_name_w, ws.equals,
        _formatVariable( encoding->atom ), ws.separator,
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
    const ChangeProperty::Header* header {
        reinterpret_cast< const ChangeProperty::Header* >( data ) };
    request.bytes_parsed += sizeof( ChangeProperty::Header );
    assert( header->opcode == protocol::requests::opcodes::CHANGEPROPERTY );
    const ChangeProperty::Encoding* encoding {
        reinterpret_cast< const ChangeProperty::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ChangeProperty::Encoding );
    assert( encoding->format <= 32 && encoding->format % 8 == 0 );
    // followed by LISTofBYTE data
    const uint32_t data_len { encoding->data_fmt_unit_len *
                              ( encoding->format / 8 ) };
    _ParsingOutputs data_;
    if ( encoding->type.data == protocol::atoms::predefined::STRING ) {
        data_.str = fmt::format(
            "{:?}", std::string_view{
                reinterpret_cast< const char* >(
                    data + request.bytes_parsed ), data_len } );
        data_.bytes_parsed = data_len;
    } else {
        data_ = _parseLISTof< protocol::BYTE >(
            data + request.bytes_parsed, data_len,
            data_len, ws.nested( _Whitespace::FORCE_SINGLELINE ) );
    }
    request.bytes_parsed += alignment.pad( data_.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(data length (format units))" ) :
                              sizeof( "property" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( header->mode,
                         { ChangeProperty::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
        ws.memb_indent, "property", memb_name_w, ws.equals,
        _formatVariable( encoding->property ), ws.separator,
        ws.memb_indent, "type", memb_name_w, ws.equals,
        _formatVariable( encoding->type ), ws.separator,
        ws.memb_indent, "format", memb_name_w, ws.equals,
        _formatVariable( encoding->format ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(data length (format units))", memb_name_w, ws.equals,
            _formatVariable( encoding->data_fmt_unit_len ), ws.separator ),
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
    const DeleteProperty::Header* header {
        reinterpret_cast< const DeleteProperty::Header* >( data ) };
    request.bytes_parsed += sizeof( DeleteProperty::Header );
    assert( header->opcode == protocol::requests::opcodes::DELETEPROPERTY );
    const DeleteProperty::Encoding* encoding {
        reinterpret_cast< const DeleteProperty::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( DeleteProperty::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "property" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
        ws.memb_indent, "property", memb_name_w, ws.equals,
        _formatVariable( encoding->property ), ws.separator,
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
    const GetProperty::Header* header {
        reinterpret_cast< const GetProperty::Header* >( data ) };
    request.bytes_parsed += sizeof( GetProperty::Header );
    assert( header->opcode == protocol::requests::opcodes::GETPROPERTY );
    const GetProperty::Encoding* encoding {
        reinterpret_cast< const GetProperty::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( GetProperty::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "long-offset" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "delete", memb_name_w, ws.equals,
        _formatVariable( header->delete_ ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
        ws.memb_indent, "property", memb_name_w, ws.equals,
        _formatVariable( encoding->property ), ws.separator,
        ws.memb_indent, "type", memb_name_w, ws.equals,
        _formatVariable( encoding->type,
                         { GetProperty::type_names } ), ws.separator,
        ws.memb_indent, "long-offset", memb_name_w, ws.equals,
        _formatVariable( encoding->long_offset ), ws.separator,
        ws.memb_indent, "long-length", memb_name_w, ws.equals,
        _formatVariable( encoding->long_length ), ws.separator,
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
    assert( sz >= sizeof( ListProperties::Header ) );
    assert( reinterpret_cast< const ListProperties::Header* >(
                data )->opcode ==
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
    const SetSelectionOwner::Header* header {
        reinterpret_cast< const SetSelectionOwner::Header* >( data ) };
    request.bytes_parsed += sizeof( SetSelectionOwner::Header );
    assert( header->opcode == protocol::requests::opcodes::SETSELECTIONOWNER );
    const SetSelectionOwner::Encoding* encoding {
        reinterpret_cast< const SetSelectionOwner::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( SetSelectionOwner::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "selection" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "owner", memb_name_w, ws.equals,
        _formatVariable( encoding->owner,
                         { SetSelectionOwner::owner_names } ), ws.separator,
        ws.memb_indent, "selection", memb_name_w, ws.equals,
        _formatVariable( encoding->selection ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time,
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
    const GetSelectionOwner::Header* header {
        reinterpret_cast< const GetSelectionOwner::Header* >( data ) };
    request.bytes_parsed += sizeof( GetSelectionOwner::Header );
    assert( header->opcode == protocol::requests::opcodes::GETSELECTIONOWNER );
    const GetSelectionOwner::Encoding* encoding {
        reinterpret_cast< const GetSelectionOwner::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( GetSelectionOwner::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "selection" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "selection", memb_name_w, ws.equals,
        _formatVariable( encoding->selection ), ws.separator,
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
    const ConvertSelection::Header* header {
        reinterpret_cast< const ConvertSelection::Header* >( data ) };
    request.bytes_parsed += sizeof( ConvertSelection::Header );
    assert( header->opcode == protocol::requests::opcodes::CONVERTSELECTION );
    const ConvertSelection::Encoding* encoding {
        reinterpret_cast< const ConvertSelection::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ConvertSelection::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "requestor" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "requestor", memb_name_w, ws.equals,
        _formatVariable( encoding->requestor ), ws.separator,
        ws.memb_indent, "selection", memb_name_w, ws.equals,
        _formatVariable( encoding->selection ), ws.separator,
        ws.memb_indent, "target", memb_name_w, ws.equals,
        _formatVariable( encoding->target ), ws.separator,
        ws.memb_indent, "property", memb_name_w, ws.equals,
        _formatVariable( encoding->property,
                         { ConvertSelection::property_names } ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time,
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
    const SendEvent::Header* header {
        reinterpret_cast< const SendEvent::Header* >( data ) };
    request.bytes_parsed += sizeof( SendEvent::Header );
    assert( header->opcode == protocol::requests::opcodes::SENDEVENT );
    const SendEvent::Encoding* encoding {
        reinterpret_cast< const SendEvent::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( SendEvent::Encoding );
    // followed by Event
    const uint8_t evt_code {
        reinterpret_cast< const protocol::events::Event::Header* >(
            data + request.bytes_parsed )->code };
    const _ParsingOutputs event { _parseEvent(
            conn, data + request.bytes_parsed,
            protocol::events::Event::ENCODING_SZ, ws.nested() ) };
    request.bytes_parsed += event.bytes_parsed;
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "destination" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}({}) {}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "propagate", memb_name_w, ws.equals,
        _formatVariable( header->propagate ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "destination", memb_name_w, ws.equals,
        _formatVariable( encoding->destination,
                         { SendEvent::destination_names } ), ws.separator,
        ws.memb_indent, "event-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->event_mask ), ws.separator,
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
    const GrabPointer::Header* header {
        reinterpret_cast< const GrabPointer::Header* >( data ) };
    request.bytes_parsed += sizeof( GrabPointer::Header );
    assert( header->opcode == protocol::requests::opcodes::GRABPOINTER );
    const GrabPointer::Encoding* encoding {
        reinterpret_cast< const GrabPointer::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( GrabPointer::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "keyboard-mode" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "owner-events", memb_name_w, ws.equals,
        _formatVariable( header->owner_events ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "grab-window", memb_name_w, ws.equals,
        _formatVariable( encoding->grab_window ), ws.separator,
        ws.memb_indent, "event-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->event_mask ), ws.separator,
        ws.memb_indent, "pointer-mode", memb_name_w, ws.equals,
        _formatVariable( encoding->pointer_mode,
                         { GrabPointer::pointer_mode_names } ), ws.separator,
        ws.memb_indent, "keyboard-mode", memb_name_w, ws.equals,
        _formatVariable( encoding->keyboard_mode,
                         { GrabPointer::keyboard_mode_names } ), ws.separator,
        ws.memb_indent, "confine-to", memb_name_w, ws.equals,
        _formatVariable( encoding->confine_to,
                         { GrabPointer::confine_to_names } ), ws.separator,
        ws.memb_indent, "cursor", memb_name_w, ws.equals,
        _formatVariable( encoding->cursor,
                         { GrabPointer::cursor_names } ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time,
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
    const UngrabPointer::Header* header {
        reinterpret_cast< const UngrabPointer::Header* >( data ) };
    request.bytes_parsed += sizeof( UngrabPointer::Header );
    assert( header->opcode == protocol::requests::opcodes::UNGRABPOINTER );
    const UngrabPointer::Encoding* encoding {
        reinterpret_cast< const UngrabPointer::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( UngrabPointer::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "time" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time,
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
    const GrabButton::Header* header {
        reinterpret_cast< const GrabButton::Header* >( data ) };
    request.bytes_parsed += sizeof( GrabButton::Header );
    assert( header->opcode == protocol::requests::opcodes::GRABBUTTON );
    const GrabButton::Encoding* encoding {
        reinterpret_cast< const GrabButton::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( GrabButton::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "keyboard-mode" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "owner-events", memb_name_w, ws.equals,
        _formatVariable( header->owner_events ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "grab-window", memb_name_w, ws.equals,
        _formatVariable( encoding->grab_window ), ws.separator,
        ws.memb_indent, "event-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->event_mask ), ws.separator,
        ws.memb_indent, "pointer-mode", memb_name_w, ws.equals,
        _formatVariable( encoding->pointer_mode,
                         { GrabButton::pointer_mode_names } ), ws.separator,
        ws.memb_indent, "keyboard-mode", memb_name_w, ws.equals,
        _formatVariable( encoding->keyboard_mode,
                         { GrabButton::keyboard_mode_names } ), ws.separator,
        ws.memb_indent, "confine-to", memb_name_w, ws.equals,
        _formatVariable( encoding->confine_to,
                         { GrabButton::confine_to_names } ), ws.separator,
        ws.memb_indent, "cursor", memb_name_w, ws.equals,
        _formatVariable( encoding->cursor,
                         { GrabButton::cursor_names } ), ws.separator,
        ws.memb_indent, "button", memb_name_w, ws.equals,
        _formatVariable( encoding->button,
                         { GrabButton::button_names } ), ws.separator,
        ws.memb_indent, "modifiers", memb_name_w, ws.equals,
        _formatVariable( encoding->modifiers ), ws.separator,
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
    const UngrabButton::Header* header {
        reinterpret_cast< const UngrabButton::Header* >( data ) };
    request.bytes_parsed += sizeof( UngrabButton::Header );
    assert( header->opcode == protocol::requests::opcodes::UNGRABBUTTON );
    const UngrabButton::Encoding* encoding {
        reinterpret_cast< const UngrabButton::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( UngrabButton::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "grab-window" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "button", memb_name_w, ws.equals,
        _formatVariable( header->button,
                         { UngrabButton::button_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "grab-window", memb_name_w, ws.equals,
        _formatVariable( encoding->grab_window ), ws.separator,
        ws.memb_indent, "modifiers", memb_name_w, ws.equals,
        _formatVariable( encoding->modifiers ), ws.separator,
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
    const ChangeActivePointerGrab::Header* header {
        reinterpret_cast< const ChangeActivePointerGrab::Header* >( data ) };
    request.bytes_parsed += sizeof( ChangeActivePointerGrab::Header );
    assert( header->opcode == protocol::requests::opcodes::CHANGEACTIVEPOINTERGRAB );
    const ChangeActivePointerGrab::Encoding* encoding {
        reinterpret_cast< const ChangeActivePointerGrab::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ChangeActivePointerGrab::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "event-mask" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cursor", memb_name_w, ws.equals,
        _formatVariable( encoding->cursor,
                         { ChangeActivePointerGrab::cursor_names } ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time,
                         { ChangeActivePointerGrab::time_names } ), ws.separator,
        ws.memb_indent, "event-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->event_mask ), ws.separator,
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
    const GrabKeyboard::Header* header {
        reinterpret_cast< const GrabKeyboard::Header* >( data ) };
    request.bytes_parsed += sizeof( GrabKeyboard::Header );
    assert( header->opcode == protocol::requests::opcodes::GRABKEYBOARD );
    const GrabKeyboard::Encoding* encoding {
        reinterpret_cast< const GrabKeyboard::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( GrabKeyboard::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "keyboard-mode" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "owner-events", memb_name_w, ws.equals,
        _formatVariable( header->owner_events ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "grab-window", memb_name_w, ws.equals,
        _formatVariable( encoding->grab_window ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time,
                         { GrabKeyboard::time_names } ), ws.separator,
        ws.memb_indent, "pointer-mode", memb_name_w, ws.equals,
        _formatVariable( encoding->pointer_mode,
                         { GrabKeyboard::pointer_mode_names } ), ws.separator,
        ws.memb_indent, "keyboard-mode", memb_name_w, ws.equals,
        _formatVariable( encoding->keyboard_mode,
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
    const UngrabKeyboard::Header* header {
        reinterpret_cast< const UngrabKeyboard::Header* >( data ) };
    request.bytes_parsed += sizeof( UngrabKeyboard::Header );
    assert( header->opcode == protocol::requests::opcodes::UNGRABKEYBOARD );
    const UngrabKeyboard::Encoding* encoding {
        reinterpret_cast< const UngrabKeyboard::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( UngrabKeyboard::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "time" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time,
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
    const GrabKey::Header* header {
        reinterpret_cast< const GrabKey::Header* >( data ) };
    request.bytes_parsed += sizeof( GrabKey::Header );
    assert( header->opcode == protocol::requests::opcodes::GRABKEY );
    const GrabKey::Encoding* encoding {
        reinterpret_cast< const GrabKey::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( GrabKey::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        settings.multiline ? sizeof( "(total aligned units)" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "owner-events", memb_name_w, ws.equals,
        _formatVariable( header->owner_events ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "modifiers", memb_name_w, ws.equals,
        _formatVariable( encoding->modifiers ), ws.separator,
        ws.memb_indent, "key", memb_name_w, ws.equals,
        _formatVariable( encoding->key,
                         { GrabKey::key_names } ), ws.separator,
        ws.memb_indent, "pointer-mode", memb_name_w, ws.equals,
        _formatVariable( encoding->pointer_mode,
                         { GrabKey::pointer_mode_names } ), ws.separator,
        ws.memb_indent, "keyboard-mode", memb_name_w, ws.equals,
        _formatVariable( encoding->keyboard_mode,
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
    const UngrabKey::Header* header {
        reinterpret_cast< const UngrabKey::Header* >( data ) };
    request.bytes_parsed += sizeof( UngrabKey::Header );
    assert( header->opcode == protocol::requests::opcodes::UNGRABKEY );
    const UngrabKey::Encoding* encoding {
        reinterpret_cast< const UngrabKey::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( UngrabKey::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "grab-window" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "key", memb_name_w, ws.equals,
        _formatVariable( header->key,
                         UngrabKey::key_names ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "grab-window", memb_name_w, ws.equals,
        _formatVariable( encoding->grab_window ), ws.separator,
        ws.memb_indent, "modifiers", memb_name_w, ws.equals,
        _formatVariable( encoding->modifiers ), ws.separator,
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
    const AllowEvents::Header* header {
        reinterpret_cast< const AllowEvents::Header* >( data ) };
    request.bytes_parsed += sizeof( AllowEvents::Header );
    assert( header->opcode == protocol::requests::opcodes::ALLOWEVENTS );
    const AllowEvents::Encoding* encoding {
        reinterpret_cast< const AllowEvents::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( AllowEvents::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "mode" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( header->mode,
                         { AllowEvents::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time,
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
    assert( sz >= sizeof( GrabServer::Header ) );
    assert( reinterpret_cast< const GrabServer::Header* >(
                data )->opcode ==
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
    assert( sz >= sizeof( UngrabServer::Header ) );
    assert( reinterpret_cast< const UngrabServer::Header* >(
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
    assert( sz >= sizeof( QueryPointer::Header ) );
    assert( reinterpret_cast< const QueryPointer::Header* >(
                data )->opcode ==
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
    const GetMotionEvents::Header* header {
        reinterpret_cast< const GetMotionEvents::Header* >( data ) };
    request.bytes_parsed += sizeof( GetMotionEvents::Header );
    assert( header->opcode == protocol::requests::opcodes::GETMOTIONEVENTS );
    const GetMotionEvents::Encoding* encoding {
        reinterpret_cast< const GetMotionEvents::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( GetMotionEvents::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "window" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
        ws.memb_indent, "start", memb_name_w, ws.equals,
        _formatVariable( encoding->start,
                         { GetMotionEvents::start_names } ), ws.separator,
        ws.memb_indent, "stop", memb_name_w, ws.equals,
        _formatVariable( encoding->stop,
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
    const TranslateCoordinates::Header* header {
        reinterpret_cast< const TranslateCoordinates::Header* >( data ) };
    request.bytes_parsed += sizeof( TranslateCoordinates::Header );
    assert( header->opcode == protocol::requests::opcodes::TRANSLATECOORDINATES );
    const TranslateCoordinates::Encoding* encoding {
        reinterpret_cast< const TranslateCoordinates::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( TranslateCoordinates::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "src-window" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "src-window", memb_name_w, ws.equals,
        _formatVariable( encoding->src_window ), ws.separator,
        ws.memb_indent, "dst-window", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_window ), ws.separator,
        ws.memb_indent, "src-x", memb_name_w, ws.equals,
        _formatVariable( encoding->src_x ), ws.separator,
        ws.memb_indent, "src-y", memb_name_w, ws.equals,
        _formatVariable( encoding->src_y ), ws.separator,
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
    const WarpPointer::Header* header {
        reinterpret_cast< const WarpPointer::Header* >( data ) };
    request.bytes_parsed += sizeof( WarpPointer::Header );
    assert( header->opcode == protocol::requests::opcodes::WARPPOINTER );
    const WarpPointer::Encoding* encoding {
        reinterpret_cast< const WarpPointer::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( WarpPointer::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "src-window" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "src-window", memb_name_w, ws.equals,
        _formatVariable( encoding->src_window,
                         { WarpPointer::src_window_names } ), ws.separator,
        ws.memb_indent, "dst-window", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_window,
                         { WarpPointer::dst_window_names } ), ws.separator,
        ws.memb_indent, "src-x", memb_name_w, ws.equals,
        _formatVariable( encoding->src_x ), ws.separator,
        ws.memb_indent, "src-y", memb_name_w, ws.equals,
        _formatVariable( encoding->src_y ), ws.separator,
        ws.memb_indent, "src-width", memb_name_w, ws.equals,
        _formatVariable( encoding->src_width ), ws.separator,
        ws.memb_indent, "src-height", memb_name_w, ws.equals,
        _formatVariable( encoding->src_height ), ws.separator,
        ws.memb_indent, "dst-x", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_x ), ws.separator,
        ws.memb_indent, "dst-y", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_y ), ws.separator,
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
    const SetInputFocus::Header* header {
        reinterpret_cast< const SetInputFocus::Header* >( data ) };
    request.bytes_parsed += sizeof( SetInputFocus::Header );
    assert( header->opcode == protocol::requests::opcodes::SETINPUTFOCUS );
    const SetInputFocus::Encoding* encoding {
        reinterpret_cast< const SetInputFocus::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( SetInputFocus::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "revert-to" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "revert-to", memb_name_w, ws.equals,
        _formatVariable( header->revert_to,
                         { SetInputFocus::revert_to_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "focus", memb_name_w, ws.equals,
        _formatVariable( encoding->focus,
                         { SetInputFocus::focus_names,
                           _EnumNameRange::Bound::MAX,
                           SetInputFocus::FOCUS_ENUM_MAX } ), ws.separator,
        ws.memb_indent, "time", memb_name_w, ws.equals,
        _formatVariable( encoding->time,
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
    assert( sz >= sizeof( GetInputFocus::Header ) );
    assert( reinterpret_cast< const GetInputFocus::Header* >(
                data )->opcode ==
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
    assert( sz >= sizeof( QueryKeymap::Header ) );
    assert( reinterpret_cast< const QueryKeymap::Header* >(
                data )->opcode ==
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
    const OpenFont::Header* header {
        reinterpret_cast< const OpenFont::Header* >( data ) };
    request.bytes_parsed += sizeof( OpenFont::Header );
    assert( header->opcode == protocol::requests::opcodes::OPENFONT );
    const OpenFont::Encoding* encoding {
        reinterpret_cast< const OpenFont::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( OpenFont::Encoding );
    // followed by STRING8 name
    const std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        encoding->name_len };
    request.bytes_parsed += alignment.pad( encoding->name_len );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "fid" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "fid", memb_name_w, ws.equals,
        _formatVariable( encoding->fid ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( encoding->name_len ), ws.separator ),
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
    const CloseFont::Header* header {
        reinterpret_cast< const CloseFont::Header* >( data ) };
    request.bytes_parsed += sizeof( CloseFont::Header );
    assert( header->opcode == protocol::requests::opcodes::CLOSEFONT );
    const CloseFont::Encoding* encoding {
        reinterpret_cast< const CloseFont::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( CloseFont::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "font" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "font", memb_name_w, ws.equals,
        _formatVariable( encoding->font ), ws.separator,
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
    const QueryFont::Header* header {
        reinterpret_cast< const QueryFont::Header* >( data ) };
    request.bytes_parsed += sizeof( QueryFont::Header );
    assert( header->opcode == protocol::requests::opcodes::QUERYFONT );
    const QueryFont::Encoding* encoding {
        reinterpret_cast< const QueryFont::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( QueryFont::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "font" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "font", memb_name_w, ws.equals,
        _formatVariable( encoding->font ), ws.separator,
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
    const QueryTextExtents::Header* header {
        reinterpret_cast< const QueryTextExtents::Header* >( data ) };
    request.bytes_parsed += sizeof( QueryTextExtents::Header );
    assert( header->opcode == protocol::requests::opcodes::QUERYTEXTEXTENTS );
    const QueryTextExtents::Encoding* encoding {
        reinterpret_cast< const QueryTextExtents::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( QueryTextExtents::Encoding );
    // followed by STRING16 string
    //   first calc padded string length due to ambiguity around odd-length
    const size_t string_sz {
        alignment.size( header->tl_aligned_units ) -
        QueryTextExtents::BASE_ENCODING_SZ };
    const size_t string_len {
        ( string_sz / sizeof( protocol::CHAR2B/*char16_t*/ ) ) -
        ( header->odd_length.data ? 1 : 0 ) };
    _ParsingOutputs string {
        _parseLISTof< protocol::CHAR2B >(
            data + request.bytes_parsed, string_sz,
            string_len, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    //   bypass expected use of string.bytes_parsed due to odd-length
    request.bytes_parsed += string_sz;
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "string" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(odd length)", memb_name_w, ws.equals,
            _formatVariable( header->odd_length ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "font", memb_name_w, ws.equals,
        _formatVariable( encoding->font ), ws.separator,
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
    assert( sz >= sizeof( ListFonts::Header ) );
    assert( reinterpret_cast< const ListFonts::Header* >(
                data )->opcode ==
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
    assert( sz >= sizeof( ListFontsWithInfo::Header ) );
    assert( reinterpret_cast< const ListFontsWithInfo::Header* >(
                data )->opcode ==
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
    const SetFontPath::Header* header {
        reinterpret_cast< const SetFontPath::Header* >( data ) };
    request.bytes_parsed += sizeof( SetFontPath::Header );
    assert( header->opcode == protocol::requests::opcodes::SETFONTPATH );
    const SetFontPath::Encoding* encoding {
        reinterpret_cast< const SetFontPath::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( SetFontPath::Encoding );
    // followed by LISTofSTR path
    const _ParsingOutputs path {
        _parseLISTof< protocol::STR >(
            data + request.bytes_parsed, sz - request.bytes_parsed,
            encoding->path_ct, ws.nested() ) };
    request.bytes_parsed += alignment.pad( path.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "path" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(STRs in path)", memb_name_w, ws.equals,
            _formatVariable( encoding->path_ct ), ws.separator ),
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
    assert( sz >= sizeof( GetFontPath::Header ) );
    assert( reinterpret_cast< const GetFontPath::Header* >(
                data )->opcode ==
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
    const CreatePixmap::Header* header {
        reinterpret_cast< const CreatePixmap::Header* >( data ) };
    request.bytes_parsed += sizeof( CreatePixmap::Header );
    assert( header->opcode == protocol::requests::opcodes::CREATEPIXMAP );
    const CreatePixmap::Encoding* encoding {
        reinterpret_cast< const CreatePixmap::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( CreatePixmap::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatVariable( header->depth ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "pid", memb_name_w, ws.equals,
        _formatVariable( encoding->pid ), ws.separator,
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height ), ws.separator,
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
    const FreePixmap::Header* header {
        reinterpret_cast< const FreePixmap::Header* >( data ) };
    request.bytes_parsed += sizeof( FreePixmap::Header );
    assert( header->opcode == protocol::requests::opcodes::FREEPIXMAP );
    const FreePixmap::Encoding* encoding {
        reinterpret_cast< const FreePixmap::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( FreePixmap::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "pixmap" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "pixmap", memb_name_w, ws.equals,
        _formatVariable( encoding->pixmap ), ws.separator,
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
    const CreateGC::Header* header {
        reinterpret_cast< const CreateGC::Header* >( data ) };
    request.bytes_parsed += sizeof( CreateGC::Header );
    assert( header->opcode == protocol::requests::opcodes::CREATEGC );
    const CreateGC::Encoding* encoding {
        reinterpret_cast< const CreateGC::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( CreateGC::Encoding );
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
        encoding->value_mask, CreateGC::value_types, CreateGC::value_names,
        value_traits, ws.nested() };
    _LISTofVALUEParsingOutputs value_list;
    _parseLISTofVALUE( value_list_inputs, &value_list );
    request.bytes_parsed += value_list.bytes_parsed;
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "value-list" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cid", memb_name_w, ws.equals,
        _formatVariable( encoding->cid ), ws.separator,
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "value-mask", memb_name_w, ws.equals,
            _formatVariable( encoding->value_mask, { CreateGC::value_names },
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
    const ChangeGC::Header* header {
        reinterpret_cast< const ChangeGC::Header* >( data ) };
    request.bytes_parsed += sizeof( ChangeGC::Header );
    assert( header->opcode == protocol::requests::opcodes::CHANGEGC );
    const ChangeGC::Encoding* encoding {
        reinterpret_cast< const ChangeGC::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ChangeGC::Encoding );
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
        encoding->value_mask, ChangeGC::value_types, ChangeGC::value_names,
        value_traits, ws.nested() };
    _LISTofVALUEParsingOutputs value_list;
    _parseLISTofVALUE( value_list_inputs, &value_list );
    request.bytes_parsed += value_list.bytes_parsed;
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "value-list" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "value-mask", memb_name_w, ws.equals,
            _formatVariable( encoding->value_mask, { ChangeGC::value_names },
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
    const CopyGC::Header* header {
        reinterpret_cast< const CopyGC::Header* >( data ) };
    request.bytes_parsed += sizeof( CopyGC::Header );
    assert( header->opcode == protocol::requests::opcodes::COPYGC );
    const CopyGC::Encoding* encoding {
        reinterpret_cast< const CopyGC::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( CopyGC::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "value-mask" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "src-gc", memb_name_w, ws.equals,
        _formatVariable( encoding->src_gc ), ws.separator,
        ws.memb_indent, "dst-gc", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_gc ), ws.separator,
        ws.memb_indent, "value-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->value_mask, { CopyGC::value_names },
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
    const SetDashes::Header* header {
        reinterpret_cast< const SetDashes::Header* >( data ) };
    request.bytes_parsed += sizeof( SetDashes::Header );
    assert( header->opcode == protocol::requests::opcodes::SETDASHES );
    const SetDashes::Encoding* encoding {
        reinterpret_cast< const SetDashes::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( SetDashes::Encoding );
    // followed by LISTofCARD8 dashes
    _ParsingOutputs dashes {
        _parseLISTof< protocol::CARD8 >(
            data + request.bytes_parsed, sz - request.bytes_parsed,
            encoding->dashes_len, ws.nested() ) };
    request.bytes_parsed += alignment.pad( dashes.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "dash-offset" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
        ws.memb_indent, "dash-offset", memb_name_w, ws.equals,
        _formatVariable( encoding->dash_offset ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(dashes length)", memb_name_w, ws.equals,
            _formatVariable( encoding->dashes_len ), ws.separator ),
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
    const SetClipRectangles::Header* header {
        reinterpret_cast< const SetClipRectangles::Header* >( data ) };
    request.bytes_parsed += sizeof( SetClipRectangles::Header );
    assert( header->opcode == protocol::requests::opcodes::SETCLIPRECTANGLES );
    const SetClipRectangles::Encoding* encoding {
        reinterpret_cast< const SetClipRectangles::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( SetClipRectangles::Encoding );
    // followed by LISTofRECTANGLE rectangles
    const size_t rectangles_sz {
        alignment.size( header->tl_aligned_units ) -
        SetClipRectangles::BASE_ENCODING_SZ };
    const size_t rectangles_ct { rectangles_sz / sizeof( protocol::RECTANGLE ) };
    _ParsingOutputs rectangles {
        _parseLISTof< protocol::RECTANGLE >(
            data + request.bytes_parsed, rectangles_sz,
            rectangles_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( rectangles.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "clip-x-origin" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "ordering", memb_name_w, ws.equals,
        _formatVariable( header->ordering,
                         { SetClipRectangles::ordering_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
        ws.memb_indent, "clip-x-origin", memb_name_w, ws.equals,
        _formatVariable( encoding->clip_x_origin ), ws.separator,
        ws.memb_indent, "clip-y-origin", memb_name_w, ws.equals,
        _formatVariable( encoding->clip_y_origin ), ws.separator,
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
    const FreeGC::Header* header {
        reinterpret_cast< const FreeGC::Header* >( data ) };
    request.bytes_parsed += sizeof( FreeGC::Header );
    assert( header->opcode == protocol::requests::opcodes::FREEGC );
    const FreeGC::Encoding* encoding {
        reinterpret_cast< const FreeGC::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( FreeGC::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        settings.multiline ? sizeof( "(total aligned units)" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
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
    const ClearArea::Header* header {
        reinterpret_cast< const ClearArea::Header* >( data ) };
    request.bytes_parsed += sizeof( ClearArea::Header );
    assert( header->opcode == protocol::requests::opcodes::CLEARAREA );
    const ClearArea::Encoding* encoding {
        reinterpret_cast< const ClearArea::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ClearArea::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "exposures" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "exposures", memb_name_w, ws.equals,
        _formatVariable( header->exposures ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height ), ws.separator,
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
    const CopyArea::Header* header {
        reinterpret_cast< const CopyArea::Header* >( data ) };
    request.bytes_parsed += sizeof( CopyArea::Header );
    assert( header->opcode == protocol::requests::opcodes::COPYAREA );
    const CopyArea::Encoding* encoding {
        reinterpret_cast< const CopyArea::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( CopyArea::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "src-drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "src-drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->src_drawable ), ws.separator,
        ws.memb_indent, "dst-drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
        ws.memb_indent, "src-x", memb_name_w, ws.equals,
        _formatVariable( encoding->src_x ), ws.separator,
        ws.memb_indent, "src-y", memb_name_w, ws.equals,
        _formatVariable( encoding->src_y ), ws.separator,
        ws.memb_indent, "dst-x", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_x ), ws.separator,
        ws.memb_indent, "dst-y", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height ), ws.separator,
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
    const CopyPlane::Header* header {
        reinterpret_cast< const CopyPlane::Header* >( data ) };
    request.bytes_parsed += sizeof( CopyPlane::Header );
    assert( header->opcode == protocol::requests::opcodes::COPYPLANE );
    const CopyPlane::Encoding* encoding {
        reinterpret_cast< const CopyPlane::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( CopyPlane::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "src-drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "src-drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->src_drawable ), ws.separator,
        ws.memb_indent, "dst-drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
        ws.memb_indent, "src-x", memb_name_w, ws.equals,
        _formatVariable( encoding->src_x ), ws.separator,
        ws.memb_indent, "src-y", memb_name_w, ws.equals,
        _formatVariable( encoding->src_y ), ws.separator,
        ws.memb_indent, "dst-x", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_x ), ws.separator,
        ws.memb_indent, "dst-y", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height ), ws.separator,
        ws.memb_indent, "bit-plane", memb_name_w, ws.equals,
        _formatVariable( encoding->bit_plane ), ws.separator,
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
    assert( sz >= sizeof( PolyPoint::Header ) );
    assert( reinterpret_cast< const PolyPoint::Header* >(
                data )->opcode ==
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
    assert( sz >= sizeof( PolyLine::Header ) );
    assert( reinterpret_cast< const PolyLine::Header* >(
                data )->opcode ==
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
    const PolySegment::Header* header {
        reinterpret_cast< const PolySegment::Header* >( data ) };
    request.bytes_parsed += sizeof( PolySegment::Header );
    assert( header->opcode == protocol::requests::opcodes::POLYSEGMENT );
    const PolySegment::Encoding* encoding {
        reinterpret_cast< const PolySegment::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( PolySegment::Encoding );
    // followed by LISTofSEGMENT segments
    const size_t segments_sz {
        alignment.size( header->tl_aligned_units ) -
        PolySegment::BASE_ENCODING_SZ };
    const size_t segments_ct { segments_sz / sizeof( PolySegment::SEGMENT ) };
    const _ParsingOutputs segments {
        _parseLISTof< PolySegment::SEGMENT >(
            data + request.bytes_parsed, segments_sz,
            segments_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( segments.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
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
    const PolyRectangle::Header* header {
        reinterpret_cast< const PolyRectangle::Header* >( data ) };
    request.bytes_parsed += sizeof( PolyRectangle::Header );
    assert( header->opcode == protocol::requests::opcodes::POLYRECTANGLE );
    const PolyRectangle::Encoding* encoding {
        reinterpret_cast< const PolyRectangle::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( PolyRectangle::Encoding );
    // followed by LISTofRECTANGLE rectangles
    const size_t rectangles_sz {
        alignment.size( header->tl_aligned_units ) -
        PolyRectangle::BASE_ENCODING_SZ };
    const size_t rectangles_ct { rectangles_sz / sizeof( protocol::RECTANGLE ) };
    _ParsingOutputs rectangles {
        _parseLISTof< protocol::RECTANGLE >(
            data + request.bytes_parsed, rectangles_sz,
            rectangles_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( rectangles.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "rectangles" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
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
    const PolyArc::Header* header {
        reinterpret_cast< const PolyArc::Header* >( data ) };
    request.bytes_parsed += sizeof( PolyArc::Header );
    assert( header->opcode == protocol::requests::opcodes::POLYARC );
    const PolyArc::Encoding* encoding {
        reinterpret_cast< const PolyArc::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( PolyArc::Encoding );
    // followed by LISTofARC arcs
    const size_t arcs_sz {
        alignment.size( header->tl_aligned_units ) -
        PolyArc::BASE_ENCODING_SZ };
    const size_t arcs_ct { arcs_sz / sizeof( protocol::ARC ) };
    const _ParsingOutputs arcs {
        _parseLISTof< protocol::ARC >(
            data + request.bytes_parsed, arcs_sz,
            arcs_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( arcs.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
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
    const FillPoly::Header* header {
        reinterpret_cast< const FillPoly::Header* >( data ) };
    request.bytes_parsed += sizeof( FillPoly::Header );
    assert( header->opcode == protocol::requests::opcodes::FILLPOLY );
    const FillPoly::Encoding* encoding {
        reinterpret_cast< const FillPoly::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( FillPoly::Encoding );
    // followed by LISTofPOINT points
    const size_t points_sz {
        alignment.size( header->tl_aligned_units ) -
        FillPoly::BASE_ENCODING_SZ };
    const size_t points_ct { points_sz / sizeof( protocol::POINT ) };
    const _ParsingOutputs points {
        _parseLISTof< protocol::POINT >(
            data + request.bytes_parsed, points_sz, points_ct,
            ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( points.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "coordinate-mode" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
        ws.memb_indent, "shape", memb_name_w, ws.equals,
        _formatVariable( encoding->shape,
                         { FillPoly::shape_names } ), ws.separator,
        ws.memb_indent, "coordinate-mode", memb_name_w, ws.equals,
        _formatVariable( encoding->coordinate_mode,
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
    const PolyFillRectangle::Header* header {
        reinterpret_cast< const PolyFillRectangle::Header* >( data ) };
    request.bytes_parsed += sizeof( PolyFillRectangle::Header );
    assert( header->opcode == protocol::requests::opcodes::POLYFILLRECTANGLE );
    const PolyFillRectangle::Encoding* encoding {
        reinterpret_cast< const PolyFillRectangle::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( PolyFillRectangle::Encoding );
    // followed by LISTofRECTANGLE rectangles
    const size_t rectangles_sz {
        alignment.size( header->tl_aligned_units ) -
        sizeof( PolyFillRectangle::Encoding ) };
    const uint16_t rectangles_ct ( rectangles_sz / sizeof( protocol::RECTANGLE ) );
    const _ParsingOutputs rectangles {
        _parseLISTof< protocol::RECTANGLE >(
            data + request.bytes_parsed, rectangles_sz,
            rectangles_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( rectangles.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "rectangles" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
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
    const PolyFillArc::Header* header {
        reinterpret_cast< const PolyFillArc::Header* >( data ) };
    request.bytes_parsed += sizeof( PolyFillArc::Header );
    assert( header->opcode == protocol::requests::opcodes::POLYFILLARC );
    const PolyFillArc::Encoding* encoding {
        reinterpret_cast< const PolyFillArc::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( PolyFillArc::Encoding );
    // followed by LISTofARC arcs
    const size_t arcs_sz {
        alignment.size( header->tl_aligned_units ) -
        sizeof( PolyFillArc::Encoding ) };
    const uint16_t arcs_ct ( arcs_sz  / sizeof( protocol::ARC ) );
    const _ParsingOutputs arcs {
        _parseLISTof< protocol::ARC >(
            data + request.bytes_parsed, arcs_sz,
            arcs_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( arcs.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
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
    const PutImage::Header* header {
        reinterpret_cast< const PutImage::Header* >( data ) };
    request.bytes_parsed += sizeof( PutImage::Header );
    assert( header->opcode == protocol::requests::opcodes::PUTIMAGE );
    const PutImage::Encoding* encoding {
        reinterpret_cast< const PutImage::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( PutImage::Encoding );
    // followed by LISTofBYTE data
    const size_t data_len {
        alignment.size( header->tl_aligned_units ) -
        PutImage::BASE_ENCODING_SZ };
    //   as in xtrace, we will only print the size in bytes of the image data
    request.bytes_parsed += alignment.pad( data_len );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}({} bytes){}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "format", memb_name_w, ws.equals,
        _formatVariable( header->format,
                         { PutImage::format_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height ), ws.separator,
        ws.memb_indent, "dst-x", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_x ), ws.separator,
        ws.memb_indent, "dst-y", memb_name_w, ws.equals,
        _formatVariable( encoding->dst_y ), ws.separator,
        ws.memb_indent, "left-pad", memb_name_w, ws.equals,
        _formatVariable( encoding->left_pad ), ws.separator,
        ws.memb_indent, "depth", memb_name_w, ws.equals,
        _formatVariable( encoding->depth ), ws.separator,
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
    const GetImage::Header* header {
        reinterpret_cast< const GetImage::Header* >( data ) };
    request.bytes_parsed += sizeof( GetImage::Header );
    assert( header->opcode == protocol::requests::opcodes::GETIMAGE );
    const GetImage::Encoding* encoding {
        reinterpret_cast< const GetImage::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( GetImage::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "plane-mask" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "format", memb_name_w, ws.equals,
        _formatVariable( header->format, { GetImage::format_names,
                                           _EnumNameRange::Bound::MIN,
                                           GetImage::FORMAT_ENUM_MIN } ),
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height ), ws.separator,
        ws.memb_indent, "plane-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->plane_mask,
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
    const PolyText8::Header* header {
        reinterpret_cast< const PolyText8::Header* >( data ) };
    request.bytes_parsed += sizeof( PolyText8::Header );
    assert( header->opcode == protocol::requests::opcodes::POLYTEXT8 );
    const PolyText8::Encoding* encoding {
        reinterpret_cast< const PolyText8::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( PolyText8::Encoding );
    // followed by LISTofTEXTITEM8 items
    const size_t items_sz {
        alignment.size( header->tl_aligned_units ) -
        PolyText8::BASE_ENCODING_SZ };
    // TEXTITEM8 count undetermined due to their variable length
    const _ParsingOutputs items {
        _parseLISTof< PolyText8::TEXTITEM8 >(
            data + request.bytes_parsed, items_sz, ws.nested() ) };
    request.bytes_parsed += alignment.pad( items.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y ), ws.separator,
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
    const PolyText16::Header* header {
        reinterpret_cast< const PolyText16::Header* >( data ) };
    request.bytes_parsed += sizeof( PolyText16::Header );
    assert( header->opcode == protocol::requests::opcodes::POLYTEXT16 );
    const PolyText16::Encoding* encoding {
        reinterpret_cast< const PolyText16::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( PolyText16::Encoding );
    // followed by LISTofTEXTITEM16 items
    const size_t items_sz {
        alignment.size( header->tl_aligned_units ) -
        PolyText16::BASE_ENCODING_SZ };
    // TEXTITEM8 count undetermined due to their variable length
    const _ParsingOutputs items {
        _parseLISTof< PolyText16::TEXTITEM16 >(
            data + request.bytes_parsed, items_sz, ws.nested() ) };
    request.bytes_parsed += alignment.pad( items.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y ), ws.separator,
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
    const ImageText8::Header* header {
        reinterpret_cast< const ImageText8::Header* >( data ) };
    request.bytes_parsed += sizeof( ImageText8::Header );
    assert( header->opcode == protocol::requests::opcodes::IMAGETEXT8 );
    const ImageText8::Encoding* encoding {
        reinterpret_cast< const ImageText8::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ImageText8::Encoding );
    // followed by STRING8 string
    const std::string_view string {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        header->string_len };
    request.bytes_parsed += alignment.pad( header->string_len );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(string length)", memb_name_w, ws.equals,
            _formatVariable( header->string_len ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y ), ws.separator,
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
    const ImageText16::Header* header {
        reinterpret_cast< const ImageText16::Header* >( data ) };
    request.bytes_parsed += sizeof( ImageText16::Header );
    assert( header->opcode == protocol::requests::opcodes::IMAGETEXT16 );
    const ImageText16::Encoding* encoding {
        reinterpret_cast< const ImageText16::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ImageText16::Encoding );
    // followed by STRING16 string
    const size_t string_sz {
        header->string_2B_len * sizeof( protocol::CHAR2B ) };
    const _ParsingOutputs string {
        _parseLISTof< protocol::CHAR2B >(
            data + request.bytes_parsed, string_sz,
            header->string_2B_len, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    request.bytes_parsed += alignment.pad( string.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}[{}]{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(string len (CHAR2B))", memb_name_w, ws.equals,
            _formatVariable( header->string_2B_len ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "gc", memb_name_w, ws.equals,
        _formatVariable( encoding->gc ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y ), ws.separator,
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
    const CreateColormap::Header* header {
        reinterpret_cast< const CreateColormap::Header* >( data ) };
    request.bytes_parsed += sizeof( CreateColormap::Header );
    assert( header->opcode == protocol::requests::opcodes::CREATECOLORMAP );
    const CreateColormap::Encoding* encoding {
        reinterpret_cast< const CreateColormap::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( CreateColormap::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "window" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "alloc", memb_name_w, ws.equals,
        _formatVariable( header->alloc,
                         { CreateColormap::alloc_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "mid", memb_name_w, ws.equals,
        _formatVariable( encoding->mid ), ws.separator,
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
        ws.memb_indent, "visual", memb_name_w, ws.equals,
        _formatVariable( encoding->visual ), ws.separator,
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
    assert( sz >= sizeof( FreeColormap::Header ) );
    assert( reinterpret_cast< const FreeColormap::Header* >(
                data )->opcode ==
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
    const CopyColormapAndFree::Header* header {
        reinterpret_cast< const CopyColormapAndFree::Header* >( data ) };
    request.bytes_parsed += sizeof( CopyColormapAndFree::Header );
    assert( header->opcode == protocol::requests::opcodes::COPYCOLORMAPANDFREE );
    const CopyColormapAndFree::Encoding* encoding {
        reinterpret_cast< const CopyColormapAndFree::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( CopyColormapAndFree::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "src-cmap" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "mid", memb_name_w, ws.equals,
        _formatVariable( encoding->mid ), ws.separator,
        ws.memb_indent, "src-cmap", memb_name_w, ws.equals,
        _formatVariable( encoding->src_cmap ), ws.separator,
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
    assert( sz >= sizeof( InstallColormap::Header ) );
    assert( reinterpret_cast< const InstallColormap::Header* >(
                data )->opcode ==
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
    assert( sz >= sizeof( UninstallColormap::Header ) );
    assert( reinterpret_cast< const UninstallColormap::Header* >(
                data )->opcode ==
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
    assert( sz >= sizeof( ListInstalledColormaps::Header ) );
    assert( reinterpret_cast< const ListInstalledColormaps::Header* >(
                data )->opcode ==
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
    const AllocColor::Header* header {
        reinterpret_cast< const AllocColor::Header* >( data ) };
    request.bytes_parsed += sizeof( AllocColor::Header );
    assert( header->opcode == protocol::requests::opcodes::ALLOCCOLOR );
    const AllocColor::Encoding* encoding {
        reinterpret_cast< const AllocColor::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( AllocColor::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "green" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( encoding->cmap ), ws.separator,
        ws.memb_indent, "red", memb_name_w, ws.equals,
        _formatVariable( encoding->red ), ws.separator,
        ws.memb_indent, "green", memb_name_w, ws.equals,
        _formatVariable( encoding->green ), ws.separator,
        ws.memb_indent, "blue", memb_name_w, ws.equals,
        _formatVariable( encoding->blue ), ws.separator,
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
    const AllocNamedColor::Header* header {
        reinterpret_cast< const AllocNamedColor::Header* >( data ) };
    request.bytes_parsed += sizeof( AllocNamedColor::Header );
    assert( header->opcode == protocol::requests::opcodes::ALLOCNAMEDCOLOR );
    const AllocNamedColor::Encoding* encoding {
        reinterpret_cast< const AllocNamedColor::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( AllocNamedColor::Encoding );
    // followed by STRING8 name
    const std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        encoding->name_len };
    request.bytes_parsed += alignment.pad( encoding->name_len );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "cmap" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( encoding->cmap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( encoding->name_len ), ws.separator ),
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
    const AllocColorCells::Header* header {
        reinterpret_cast< const AllocColorCells::Header* >( data ) };
    request.bytes_parsed += sizeof( AllocColorCells::Header );
    assert( header->opcode == protocol::requests::opcodes::ALLOCCOLORCELLS );
    const AllocColorCells::Encoding* encoding {
        reinterpret_cast< const AllocColorCells::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( AllocColorCells::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "contiguous" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "contiguous", memb_name_w, ws.equals,
        _formatVariable( header->contiguous ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( encoding->cmap ), ws.separator,
        ws.memb_indent, "colors", memb_name_w, ws.equals,
        _formatVariable( encoding->colors ), ws.separator,
        ws.memb_indent, "planes", memb_name_w, ws.equals,
        _formatVariable( encoding->planes ), ws.separator,
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
    const AllocColorPlanes::Header* header {
        reinterpret_cast< const AllocColorPlanes::Header* >( data ) };
    request.bytes_parsed += sizeof( AllocColorPlanes::Header );
    assert( header->opcode == protocol::requests::opcodes::ALLOCCOLORPLANES );
    const AllocColorPlanes::Encoding* encoding {
        reinterpret_cast< const AllocColorPlanes::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( AllocColorPlanes::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "contiguous" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "contiguous", memb_name_w, ws.equals,
        _formatVariable( header->contiguous ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( encoding->cmap ), ws.separator,
        ws.memb_indent, "colors", memb_name_w, ws.equals,
        _formatVariable( encoding->colors ), ws.separator,
        ws.memb_indent, "reds", memb_name_w, ws.equals,
        _formatVariable( encoding->reds ), ws.separator,
        ws.memb_indent, "greens", memb_name_w, ws.equals,
        _formatVariable( encoding->greens ), ws.separator,
        ws.memb_indent, "blues", memb_name_w, ws.equals,
        _formatVariable( encoding->blues ), ws.separator,
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
    const FreeColors::Header* header {
        reinterpret_cast< const FreeColors::Header* >( data ) };
    request.bytes_parsed += sizeof( FreeColors::Header );
    assert( header->opcode == protocol::requests::opcodes::FREECOLORS );
    const FreeColors::Encoding* encoding {
        reinterpret_cast< const FreeColors::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( FreeColors::Encoding );
    // followed by LISTofCARD32 pixels
    const size_t pixels_sz {
        alignment.size( header->tl_aligned_units ) -
        FreeColors::BASE_ENCODING_SZ  };
    const size_t pixels_ct { pixels_sz / sizeof( protocol::CARD32 ) };
    const _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >( data + request.bytes_parsed,
                                          pixels_sz, pixels_ct, ws.nested() ) };
    request.bytes_parsed += alignment.pad( pixels.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "plane-mask" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( encoding->cmap ), ws.separator,
        ws.memb_indent, "plane-mask", memb_name_w, ws.equals,
        _formatVariable( encoding->plane_mask,
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
    const StoreColors::Header* header {
        reinterpret_cast< const StoreColors::Header* >( data ) };
    request.bytes_parsed += sizeof( StoreColors::Header );
    assert( header->opcode == protocol::requests::opcodes::STORECOLORS );
    const StoreColors::Encoding* encoding {
        reinterpret_cast< const StoreColors::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( StoreColors::Encoding );
    // followed by LISTofCOLORITEM items
    const size_t items_sz {
        alignment.size( header->tl_aligned_units ) -
        StoreColors::BASE_ENCODING_SZ };
    const size_t items_ct { items_sz / sizeof( StoreColors::COLORITEM ) };
    const _ParsingOutputs items {
        _parseLISTof< StoreColors::COLORITEM >(
            data + request.bytes_parsed, items_sz,
            items_ct, ws.nested(), _Whitespace::FORCE_SINGLELINE ) };
    request.bytes_parsed += alignment.pad( items.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "items" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( encoding->cmap ), ws.separator,
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
    const StoreNamedColor::Header* header {
        reinterpret_cast< const StoreNamedColor::Header* >( data ) };
    request.bytes_parsed += sizeof( StoreNamedColor::Header );
    assert( header->opcode == protocol::requests::opcodes::STORENAMEDCOLOR );
    const StoreNamedColor::Encoding* encoding {
        reinterpret_cast< const StoreNamedColor::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( StoreNamedColor::Encoding );
    // followed by STRING8 name
    const std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        encoding->name_len };
    request.bytes_parsed += alignment.pad( encoding->name_len );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "(do rgb mask)" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "(do rgb mask)", memb_name_w, ws.equals,
        _formatVariable( header->do_rgb_mask, { StoreNamedColor::do_rgb_names },
                         _ValueTraits::BITMASK ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( encoding->cmap ), ws.separator,
        ws.memb_indent, "pixel", memb_name_w, ws.equals,
        _formatVariable( encoding->pixel ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( encoding->name_len ), ws.separator ),
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
    const QueryColors::Header* header {
        reinterpret_cast< const QueryColors::Header* >( data ) };
    request.bytes_parsed += sizeof( QueryColors::Header );
    assert( header->opcode == protocol::requests::opcodes::QUERYCOLORS );
    const QueryColors::Encoding* encoding {
        reinterpret_cast< const QueryColors::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( QueryColors::Encoding );
    // followed by LISTofCARD32 pixels
    const size_t pixels_sz {
        alignment.size( header->tl_aligned_units ) -
        QueryColors::BASE_ENCODING_SZ  };
    const size_t pixels_ct { pixels_sz / sizeof( protocol::CARD32 ) };
    const _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >( data + request.bytes_parsed,
                                          pixels_sz, pixels_ct, ws.nested() ) };
    request.bytes_parsed += alignment.pad( pixels.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "pixels" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( encoding->cmap ), ws.separator,
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
    const LookupColor::Header* header {
        reinterpret_cast< const LookupColor::Header* >( data ) };
    request.bytes_parsed += sizeof( LookupColor::Header );
    assert( header->opcode == protocol::requests::opcodes::LOOKUPCOLOR );
    const LookupColor::Encoding* encoding {
        reinterpret_cast< const LookupColor::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( LookupColor::Encoding );
    // followed by STRING8 name
    const std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        encoding->name_len };
    request.bytes_parsed += alignment.pad( encoding->name_len );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "cmap" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cmap", memb_name_w, ws.equals,
        _formatVariable( encoding->cmap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( encoding->name_len ), ws.separator ),
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
    const CreateCursor::Header* header {
        reinterpret_cast< const CreateCursor::Header* >( data ) };
    request.bytes_parsed += sizeof( CreateCursor::Header );
    assert( header->opcode == protocol::requests::opcodes::CREATECURSOR );
    const CreateCursor::Encoding* encoding {
        reinterpret_cast< const CreateCursor::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( CreateCursor::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "fore-green" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cid", memb_name_w, ws.equals,
        _formatVariable( encoding->cid ), ws.separator,
        ws.memb_indent, "source", memb_name_w, ws.equals,
        _formatVariable( encoding->source ), ws.separator,
        ws.memb_indent, "mask", memb_name_w, ws.equals,
        _formatVariable( encoding->mask,
                         { CreateCursor::mask_names } ), ws.separator,
        ws.memb_indent, "fore-red", memb_name_w, ws.equals,
        _formatVariable( encoding->fore_red ), ws.separator,
        ws.memb_indent, "fore-green", memb_name_w, ws.equals,
        _formatVariable( encoding->fore_green ), ws.separator,
        ws.memb_indent, "fore-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->fore_blue ), ws.separator,
        ws.memb_indent, "back-red", memb_name_w, ws.equals,
        _formatVariable( encoding->back_red ), ws.separator,
        ws.memb_indent, "back-green", memb_name_w, ws.equals,
        _formatVariable( encoding->back_green ), ws.separator,
        ws.memb_indent, "back-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->back_blue ), ws.separator,
        ws.memb_indent, "x", memb_name_w, ws.equals,
        _formatVariable( encoding->x ), ws.separator,
        ws.memb_indent, "y", memb_name_w, ws.equals,
        _formatVariable( encoding->y ), ws.separator,
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
    const CreateGlyphCursor::Header* header {
        reinterpret_cast< const CreateGlyphCursor::Header* >( data ) };
    request.bytes_parsed += sizeof( CreateGlyphCursor::Header );
    assert( header->opcode == protocol::requests::opcodes::CREATEGLYPHCURSOR );
    const CreateGlyphCursor::Encoding* encoding {
        reinterpret_cast< const CreateGlyphCursor::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( CreateGlyphCursor::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "source-font" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cid", memb_name_w, ws.equals,
        _formatVariable( encoding->cid ), ws.separator,
        ws.memb_indent, "source-font", memb_name_w, ws.equals,
        _formatVariable( encoding->source_font ), ws.separator,
        ws.memb_indent, "mask-font", memb_name_w, ws.equals,
        _formatVariable( encoding->mask_font,
                         { CreateGlyphCursor::mask_font_names } ), ws.separator,
        ws.memb_indent, "source-char", memb_name_w, ws.equals,
        _formatVariable( encoding->source_char ), ws.separator,
        ws.memb_indent, "mask-char", memb_name_w, ws.equals,
        _formatVariable( encoding->mask_char ), ws.separator,
        ws.memb_indent, "fore-red", memb_name_w, ws.equals,
        _formatVariable( encoding->fore_red ), ws.separator,
        ws.memb_indent, "fore-green", memb_name_w, ws.equals,
        _formatVariable( encoding->fore_green ), ws.separator,
        ws.memb_indent, "fore-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->fore_blue ), ws.separator,
        ws.memb_indent, "back-red", memb_name_w, ws.equals,
        _formatVariable( encoding->back_red ), ws.separator,
        ws.memb_indent, "back-green", memb_name_w, ws.equals,
        _formatVariable( encoding->back_green ), ws.separator,
        ws.memb_indent, "back-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->back_blue ), ws.separator,
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
    const FreeCursor::Header* header {
        reinterpret_cast< const FreeCursor::Header* >( data ) };
    request.bytes_parsed += sizeof( FreeCursor::Header );
    assert( header->opcode == protocol::requests::opcodes::FREECURSOR );
    const FreeCursor::Encoding* encoding {
        reinterpret_cast< const FreeCursor::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( FreeCursor::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "cursor" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cursor", memb_name_w, ws.equals,
        _formatVariable( encoding->cursor ), ws.separator,
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
    const RecolorCursor::Header* header {
        reinterpret_cast< const RecolorCursor::Header* >( data ) };
    request.bytes_parsed += sizeof( RecolorCursor::Header );
    assert( header->opcode == protocol::requests::opcodes::RECOLORCURSOR );
    const RecolorCursor::Encoding* encoding {
        reinterpret_cast< const RecolorCursor::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( RecolorCursor::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "fore-green" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "cursor", memb_name_w, ws.equals,
        _formatVariable( encoding->cursor ), ws.separator,
        ws.memb_indent, "fore-red", memb_name_w, ws.equals,
        _formatVariable( encoding->fore_red ), ws.separator,
        ws.memb_indent, "fore-green", memb_name_w, ws.equals,
        _formatVariable( encoding->fore_green ), ws.separator,
        ws.memb_indent, "fore-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->fore_blue ), ws.separator,
        ws.memb_indent, "back-red", memb_name_w, ws.equals,
        _formatVariable( encoding->back_red ), ws.separator,
        ws.memb_indent, "back-green", memb_name_w, ws.equals,
        _formatVariable( encoding->back_green ), ws.separator,
        ws.memb_indent, "back-blue", memb_name_w, ws.equals,
        _formatVariable( encoding->back_blue ), ws.separator,
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
    const QueryBestSize::Header* header {
        reinterpret_cast< const QueryBestSize::Header* >( data ) };
    request.bytes_parsed += sizeof( QueryBestSize::Header );
    assert( header->opcode == protocol::requests::opcodes::QUERYBESTSIZE );
    const QueryBestSize::Encoding* encoding {
        reinterpret_cast< const QueryBestSize::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( QueryBestSize::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "drawable" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "class", memb_name_w, ws.equals,
        _formatVariable( header->class_,
                         { QueryBestSize::class_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "drawable", memb_name_w, ws.equals,
        _formatVariable( encoding->drawable ), ws.separator,
        ws.memb_indent, "width", memb_name_w, ws.equals,
        _formatVariable( encoding->width ), ws.separator,
        ws.memb_indent, "height", memb_name_w, ws.equals,
        _formatVariable( encoding->height ), ws.separator,
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
    const QueryExtension::Header* header {
        reinterpret_cast< const QueryExtension::Header* >( data ) };
    request.bytes_parsed += sizeof( QueryExtension::Header );
    assert( header->opcode == protocol::requests::opcodes::QUERYEXTENSION );
    const QueryExtension::Encoding* encoding {
        reinterpret_cast< const QueryExtension::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( QueryExtension::Encoding );
    // followed by STRING8 name
    const std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        encoding->name_len };
    request.bytes_parsed += alignment.pad( encoding->name_len );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "name" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(name length)", memb_name_w, ws.equals,
            _formatVariable( encoding->name_len ), ws.separator ),
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
    assert( sz >= sizeof( ListExtensions::Header ) );
    assert( reinterpret_cast< const ListExtensions::Header* >(
                data )->opcode ==
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
    const ChangeKeyboardMapping::Header* header {
        reinterpret_cast< const ChangeKeyboardMapping::Header* >( data ) };
    request.bytes_parsed += sizeof( ChangeKeyboardMapping::Header );
    assert( header->opcode == protocol::requests::opcodes::CHANGEKEYBOARDMAPPING );
    const ChangeKeyboardMapping::Encoding* encoding {
        reinterpret_cast< const ChangeKeyboardMapping::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ChangeKeyboardMapping::Encoding );
    // followed by LISTofKEYSYM keysyms
    const size_t keysyms_sz {
        alignment.size( header->tl_aligned_units ) -
        ChangeKeyboardMapping::BASE_ENCODING_SZ };
    const uint16_t keysyms_ct (
        header->keycode_count * encoding->keysyms_per_keycode );
    assert( keysyms_ct == keysyms_sz / sizeof( protocol::KEYSYM ) );
    const _ParsingOutputs keysyms {
        _parseLISTof< protocol::KEYSYM >( data + request.bytes_parsed, keysyms_sz,
                                          keysyms_ct, ws.nested() ) };
    request.bytes_parsed += alignment.pad( keysyms.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "keysyms-per-keycode" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "keycode-count", memb_name_w, ws.equals,
        _formatVariable( header->keycode_count ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "first-keycode", memb_name_w, ws.equals,
        _formatVariable( encoding->first_keycode ), ws.separator,
        ws.memb_indent, "keysyms-per-keycode", memb_name_w, ws.equals,
        _formatVariable( encoding->keysyms_per_keycode ), ws.separator,
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
    const GetKeyboardMapping::Header* header {
        reinterpret_cast< const GetKeyboardMapping::Header* >( data ) };
    request.bytes_parsed += sizeof( GetKeyboardMapping::Header );
    assert( header->opcode == protocol::requests::opcodes::GETKEYBOARDMAPPING );
    const GetKeyboardMapping::Encoding* encoding {
        reinterpret_cast< const GetKeyboardMapping::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( GetKeyboardMapping::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "first-keycode" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "first-keycode", memb_name_w, ws.equals,
        _formatVariable( encoding->first_keycode ), ws.separator,
        ws.memb_indent, "count", memb_name_w, ws.equals,
        _formatVariable( encoding->count ), ws.separator,
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
    const ChangeKeyboardControl::Header* header {
        reinterpret_cast< const ChangeKeyboardControl::Header* >( data ) };
    request.bytes_parsed += sizeof( ChangeKeyboardControl::Header );
    assert( header->opcode == protocol::requests::opcodes::CHANGEKEYBOARDCONTROL );
    const ChangeKeyboardControl::Encoding* encoding {
        reinterpret_cast< const ChangeKeyboardControl::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ChangeKeyboardControl::Encoding );
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
        encoding->value_mask, ChangeKeyboardControl::value_types,
        ChangeKeyboardControl::value_names, value_traits, ws.nested() };
    _LISTofVALUEParsingOutputs value_list;
    _parseLISTofVALUE( value_list_inputs, &value_list );
    request.bytes_parsed += value_list.bytes_parsed;
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "value-list" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "value-mask", memb_name_w, ws.equals,
            _formatVariable( encoding->value_mask,
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
    assert( sz >= sizeof( GetKeyboardControl::Header ) );
    assert( reinterpret_cast< const GetKeyboardControl::Header* >(
                data )->opcode ==
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
    const Bell::Header* header {
        reinterpret_cast< const Bell::Header* >( data ) };
    request.bytes_parsed += sizeof( Bell::Header );
    assert( header->opcode == protocol::requests::opcodes::BELL );
    // Bell is header-only
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "percent" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "percent", memb_name_w, ws.equals,
        _formatVariable( header->percent ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
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
    const ChangePointerControl::Header* header {
        reinterpret_cast< const ChangePointerControl::Header* >( data ) };
    request.bytes_parsed += sizeof( ChangePointerControl::Header );
    assert( header->opcode == protocol::requests::opcodes::CHANGEPOINTERCONTROL );
    const ChangePointerControl::Encoding* encoding {
        reinterpret_cast< const ChangePointerControl::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ChangePointerControl::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "acceleration-denominator" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "acceleration-numerator", memb_name_w, ws.equals,
        _formatVariable( encoding->acceleration_numerator ), ws.separator,
        ws.memb_indent, "acceleration-denominator", memb_name_w, ws.equals,
        _formatVariable( encoding->acceleration_denominator ), ws.separator,
        ws.memb_indent, "threshold", memb_name_w, ws.equals,
        _formatVariable( encoding->threshold ), ws.separator,
        ws.memb_indent, "do-acceleration", memb_name_w, ws.equals,
        _formatVariable( encoding->do_acceleration ), ws.separator,
        ws.memb_indent, "do-threshold", memb_name_w, ws.equals,
        _formatVariable( encoding->do_threshold ), ws.separator,
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
    assert( sz >= sizeof( GetPointerControl::Header ) );
    assert( reinterpret_cast< const GetPointerControl::Header* >(
                data )->opcode ==
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
    const SetScreenSaver::Header* header {
        reinterpret_cast< const SetScreenSaver::Header* >( data ) };
    request.bytes_parsed += sizeof( SetScreenSaver::Header );
    assert( header->opcode == protocol::requests::opcodes::SETSCREENSAVER );
    const SetScreenSaver::Encoding* encoding {
        reinterpret_cast< const SetScreenSaver::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( SetScreenSaver::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "prefer-blanking" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "timeout", memb_name_w, ws.equals,
        _formatVariable( encoding->timeout ), ws.separator,
        ws.memb_indent, "interval", memb_name_w, ws.equals,
        _formatVariable( encoding->interval ), ws.separator,
        ws.memb_indent, "prefer-blanking", memb_name_w, ws.equals,
        _formatVariable( encoding->prefer_blanking,
                         { SetScreenSaver::prefer_blanking_names } ), ws.separator,
        ws.memb_indent, "allow-exposures", memb_name_w, ws.equals,
        _formatVariable( encoding->allow_exposures,
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
    assert( sz >= sizeof( GetScreenSaver::Header ) );
    assert( reinterpret_cast< const GetScreenSaver::Header* >(
                data )->opcode ==
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
    const ChangeHosts::Header* header {
        reinterpret_cast< const ChangeHosts::Header* >( data ) };
    request.bytes_parsed += sizeof( ChangeHosts::Header );
    assert( header->opcode == protocol::requests::opcodes::CHANGEHOSTS );
    const ChangeHosts::Encoding* encoding {
        reinterpret_cast< const ChangeHosts::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( ChangeHosts::Encoding );
    // followed by LISTofCARD8 address
    //   format as byte array, as we can't guarantee that the family is Internet
    const size_t address_sz {
        alignment.size( header->tl_aligned_units ) -
        ChangeHosts::BASE_ENCODING_SZ };
    const _ParsingOutputs address {
        _parseLISTof< protocol::CARD8 >(
            data + request.bytes_parsed, address_sz, encoding->address_len,
            ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    request.bytes_parsed += alignment.pad( address.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "address" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( header->mode,
                         ChangeHosts::mode_names ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "family", memb_name_w, ws.equals,
        _formatVariable( encoding->family,
                         { ChangeHosts::family_names,
                           _EnumNameRange::Bound::MAX,
                           ChangeHosts::FAMILY_ENUM_MAX } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(address length)", memb_name_w, ws.equals,
            _formatVariable( encoding->address_len ), ws.separator ),
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
    assert( sz >= sizeof( ListHosts::Header ) );
    assert( reinterpret_cast< const ListHosts::Header* >(
                data )->opcode ==
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
    const SetAccessControl::Header* header {
        reinterpret_cast< const SetAccessControl::Header* >( data ) };
    request.bytes_parsed += sizeof( SetAccessControl::Header );
    assert( header->opcode == protocol::requests::opcodes::SETACCESSCONTROL );
    // SetAccessControl is header-only
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "mode" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( header->mode,
                         { SetAccessControl::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
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
    const SetCloseDownMode::Header* header {
        reinterpret_cast< const SetCloseDownMode::Header* >( data ) };
    request.bytes_parsed += sizeof( SetCloseDownMode::Header );
    assert( header->opcode == protocol::requests::opcodes::SETCLOSEDOWNMODE );
    // SetCloseDownMode is header-only
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "mode" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( header->mode,
                         { SetCloseDownMode::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
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
    const KillClient::Header* header {
        reinterpret_cast< const KillClient::Header* >( data ) };
    request.bytes_parsed += sizeof( KillClient::Header );
    assert( header->opcode == protocol::requests::opcodes::KILLCLIENT );
    const KillClient::Encoding* encoding {
        reinterpret_cast< const KillClient::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( KillClient::Encoding );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "resource" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "resource", memb_name_w, ws.equals,
        _formatVariable( encoding->resource,
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
    const RotateProperties::Header* header {
        reinterpret_cast< const RotateProperties::Header* >( data ) };
    request.bytes_parsed += sizeof( RotateProperties::Header );
    assert( header->opcode == protocol::requests::opcodes::ROTATEPROPERTIES );
    const RotateProperties::Encoding* encoding {
        reinterpret_cast< const RotateProperties::Encoding* >(
            data + request.bytes_parsed ) };
    request.bytes_parsed += sizeof( RotateProperties::Encoding );
    // followed by LISTofATOM properties
    const size_t properties_sz {
        alignment.size( header->tl_aligned_units ) -
        RotateProperties::BASE_ENCODING_SZ };
    const _ParsingOutputs properties {
        _parseLISTof< protocol::ATOM >(
            data + request.bytes_parsed, properties_sz,
            encoding->properties_ct, ws.nested() ) };
    request.bytes_parsed += alignment.pad( properties.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "properties" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.memb_indent, "window", memb_name_w, ws.equals,
        _formatVariable( encoding->window ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(ATOMs in properties)", memb_name_w, ws.equals,
            _formatVariable( encoding->properties_ct ), ws.separator ),
        ws.memb_indent, "delta", memb_name_w, ws.equals,
        _formatVariable( encoding->delta ), ws.separator,
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
    const ForceScreenSaver::Header* header {
        reinterpret_cast< const ForceScreenSaver::Header* >( data ) };
    request.bytes_parsed += sizeof( ForceScreenSaver::Header );
    assert( header->opcode == protocol::requests::opcodes::FORCESCREENSAVER );
    // ForceScreenSaver is header-only
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "mode" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "mode", memb_name_w, ws.equals,
        _formatVariable( header->mode,
                         { ForceScreenSaver::mode_names } ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
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
    const SetPointerMapping::Header* header {
        reinterpret_cast< const SetPointerMapping::Header* >( data ) };
    request.bytes_parsed += sizeof( SetPointerMapping::Header );
    assert( header->opcode == protocol::requests::opcodes::SETPOINTERMAPPING );
    // SetPointerMapping is header-only
    // followed by LISTofCARD8 map
    const size_t map_sz {
        alignment.size( header->tl_aligned_units ) -
        SetPointerMapping::BASE_ENCODING_SZ };
    const _ParsingOutputs map {
        _parseLISTof< protocol::CARD8 >(
            data + request.bytes_parsed, map_sz,
            header->map_len, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    request.bytes_parsed += alignment.pad( map.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(total aligned units)" ) :
                              sizeof( "map" ) ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(map length)", memb_name_w, ws.equals,
            _formatVariable( header->map_len ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
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
    assert( sz >= sizeof( GetPointerMapping::Header ) );
    assert( reinterpret_cast< const GetPointerMapping::Header* >(
                data )->opcode ==
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
    const SetModifierMapping::Header* header {
        reinterpret_cast< const SetModifierMapping::Header* >( data ) };
    request.bytes_parsed += sizeof( SetModifierMapping::Header );
    assert( header->opcode == protocol::requests::opcodes::SETMODIFIERMAPPING );
    // SetModifierMapping is header-only
    // followed by LISTofKEYCODE keycodes
    const size_t keycodes_sz {
        alignment.size( header->tl_aligned_units ) -
        SetModifierMapping::BASE_ENCODING_SZ };
    const uint16_t keycodes_ct (
        SetModifierMapping::MODIFIER_CT * header->keycodes_per_modifier );
    const _ParsingOutputs keycodes {
        _parseLISTof< protocol::KEYCODE >(
            data + request.bytes_parsed, keycodes_sz, keycodes_ct, ws.nested() ) };
    request.bytes_parsed += alignment.pad( keycodes.bytes_parsed );
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "keycodes-per-modifier" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        ws.memb_indent, "keycodes-per-modifier", memb_name_w, ws.equals,
        _formatVariable( header->keycodes_per_modifier ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
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
    assert( sz >= sizeof( GetModifierMapping::Header ) );
    assert( reinterpret_cast< const GetModifierMapping::Header* >(
                data )->opcode ==
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
    const _Whitespace& ws { ws };
    const NoOperation::Header* header {
        reinterpret_cast< const NoOperation::Header* >( data ) };
    request.bytes_parsed += sizeof( NoOperation::Header );
    assert( header->opcode == protocol::requests::opcodes::NOOPERATION );
    // NoOperation is header-only
    // protocol specifies that no-op may be followed by variable length dummy data
    const size_t dummy_sz {
        alignment.size( header->tl_aligned_units ) -
        NoOperation::BASE_ENCODING_SZ };
    request.bytes_parsed += dummy_sz;
    assert( header->tl_aligned_units == alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        ( !ws.multiline || !settings.verbose ) ? 0 :
        sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "opcode", memb_name_w, ws.equals,
            _formatVariable( header->opcode ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( header->tl_aligned_units ), ws.separator ),
        ws.encl_indent
        );
    return request;
}

size_t
X11ProtocolParser::_logRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( protocol::requests::Request::Header ) );

    const uint8_t opcode {
        reinterpret_cast< const protocol::requests::Request::Header* >(
            data )->opcode };
    // TBD before implemementing extensions
    assert( ( opcode >= 1 && opcode <= 119 ) || opcode == 127 );
    // map opcode to sequence number to aid in parsing request errors and replies
    const uint16_t sequence { conn->registerRequest( opcode ) };
    _ParsingOutputs request {};
    switch ( opcode ) {
    case protocol::requests::opcodes::CREATEWINDOW:
        request = _parseRequest<
            protocol::requests::CreateWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEWINDOWATTRIBUTES:
        request = _parseRequest<
            protocol::requests::ChangeWindowAttributes >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETWINDOWATTRIBUTES:
        request = _parseRequest<
            protocol::requests::GetWindowAttributes >( conn, data, sz );
        break;
    case protocol::requests::opcodes::DESTROYWINDOW:
        request = _parseRequest<
            protocol::requests::DestroyWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::DESTROYSUBWINDOWS:
        request = _parseRequest<
            protocol::requests::DestroySubwindows >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGESAVESET:
        request = _parseRequest<
            protocol::requests::ChangeSaveSet >( conn, data, sz );
        break;
    case protocol::requests::opcodes::REPARENTWINDOW:
        request = _parseRequest<
            protocol::requests::ReparentWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::MAPWINDOW:
        request = _parseRequest<
            protocol::requests::MapWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::MAPSUBWINDOWS:
        request = _parseRequest<
            protocol::requests::MapSubwindows >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNMAPWINDOW:
        request = _parseRequest<
            protocol::requests::UnmapWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNMAPSUBWINDOWS:
        request = _parseRequest<
            protocol::requests::UnmapSubwindows >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CONFIGUREWINDOW:
        request = _parseRequest<
            protocol::requests::ConfigureWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CIRCULATEWINDOW:
        request = _parseRequest<
            protocol::requests::CirculateWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETGEOMETRY:
        request = _parseRequest<
            protocol::requests::GetGeometry >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYTREE:
        request = _parseRequest<
            protocol::requests::QueryTree >( conn, data, sz );
        break;
    case protocol::requests::opcodes::INTERNATOM:
        request = _parseRequest<
            protocol::requests::InternAtom >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETATOMNAME:
        request = _parseRequest<
            protocol::requests::GetAtomName >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEPROPERTY:
        request = _parseRequest<
            protocol::requests::ChangeProperty >( conn, data, sz );
        break;
    case protocol::requests::opcodes::DELETEPROPERTY:
        request = _parseRequest<
            protocol::requests::DeleteProperty >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPROPERTY:
        request = _parseRequest<
            protocol::requests::GetProperty >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTPROPERTIES:
        request = _parseRequest<
            protocol::requests::ListProperties >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETSELECTIONOWNER:
        request = _parseRequest<
            protocol::requests::SetSelectionOwner >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETSELECTIONOWNER:
        request = _parseRequest<
            protocol::requests::GetSelectionOwner >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CONVERTSELECTION:
        request = _parseRequest<
            protocol::requests::ConvertSelection >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SENDEVENT:
        request = _parseRequest<
            protocol::requests::SendEvent >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABPOINTER:
        request = _parseRequest<
            protocol::requests::GrabPointer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABPOINTER:
        request = _parseRequest<
            protocol::requests::UngrabPointer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABBUTTON:
        request = _parseRequest<
            protocol::requests::GrabButton >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABBUTTON:
        request = _parseRequest<
            protocol::requests::UngrabButton >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEACTIVEPOINTERGRAB:
        request = _parseRequest<
            protocol::requests::ChangeActivePointerGrab >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABKEYBOARD:
        request = _parseRequest<
            protocol::requests::GrabKeyboard >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABKEYBOARD:
        request = _parseRequest<
            protocol::requests::UngrabKeyboard >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABKEY:
        request = _parseRequest<
            protocol::requests::GrabKey >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABKEY:
        request = _parseRequest<
            protocol::requests::UngrabKey >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOWEVENTS:
        request = _parseRequest<
            protocol::requests::AllowEvents >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABSERVER:
        request = _parseRequest<
            protocol::requests::GrabServer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABSERVER:
        request = _parseRequest<
            protocol::requests::UngrabServer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYPOINTER:
        request = _parseRequest<
            protocol::requests::QueryPointer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETMOTIONEVENTS:
        request = _parseRequest<
            protocol::requests::GetMotionEvents >( conn, data, sz );
        break;
    case protocol::requests::opcodes::TRANSLATECOORDINATES:
        request = _parseRequest<
            protocol::requests::TranslateCoordinates >( conn, data, sz );
        break;
    case protocol::requests::opcodes::WARPPOINTER:
        request = _parseRequest<
            protocol::requests::WarpPointer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETINPUTFOCUS:
        request = _parseRequest<
            protocol::requests::SetInputFocus >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETINPUTFOCUS:
        request = _parseRequest<
            protocol::requests::GetInputFocus >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYKEYMAP:
        request = _parseRequest<
            protocol::requests::QueryKeymap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::OPENFONT:
        request = _parseRequest<
            protocol::requests::OpenFont >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CLOSEFONT:
        request = _parseRequest<
            protocol::requests::CloseFont >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYFONT:
        request = _parseRequest<
            protocol::requests::QueryFont >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYTEXTEXTENTS:
        request = _parseRequest<
            protocol::requests::QueryTextExtents >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTFONTS:
        request = _parseRequest<
            protocol::requests::ListFonts >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTFONTSWITHINFO:
        request = _parseRequest<
            protocol::requests::ListFontsWithInfo >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETFONTPATH:
        request = _parseRequest<
            protocol::requests::SetFontPath >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETFONTPATH:
        request = _parseRequest<
            protocol::requests::GetFontPath >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATEPIXMAP:
        request = _parseRequest<
            protocol::requests::CreatePixmap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREEPIXMAP:
        request = _parseRequest<
            protocol::requests::FreePixmap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATEGC:
        request = _parseRequest<
            protocol::requests::CreateGC >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEGC:
        request = _parseRequest<
            protocol::requests::ChangeGC >( conn, data, sz );
        break;
    case protocol::requests::opcodes::COPYGC:
        request = _parseRequest<
            protocol::requests::CopyGC >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETDASHES:
        request = _parseRequest<
            protocol::requests::SetDashes >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETCLIPRECTANGLES:
        request = _parseRequest<
            protocol::requests::SetClipRectangles >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREEGC:
        request = _parseRequest<
            protocol::requests::FreeGC >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CLEARAREA:
        request = _parseRequest<
            protocol::requests::ClearArea >( conn, data, sz );
        break;
    case protocol::requests::opcodes::COPYAREA:
        request = _parseRequest<
            protocol::requests::CopyArea >( conn, data, sz );
        break;
    case protocol::requests::opcodes::COPYPLANE:
        request = _parseRequest<
            protocol::requests::CopyPlane >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYPOINT:
        request = _parseRequest<
            protocol::requests::PolyPoint >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYLINE:
        request = _parseRequest<
            protocol::requests::PolyLine >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYSEGMENT:
        request = _parseRequest<
            protocol::requests::PolySegment >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYRECTANGLE:
        request = _parseRequest<
            protocol::requests::PolyRectangle >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYARC:
        request = _parseRequest<
            protocol::requests::PolyArc >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FILLPOLY:
        request = _parseRequest<
            protocol::requests::FillPoly >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYFILLRECTANGLE:
        request = _parseRequest<
            protocol::requests::PolyFillRectangle >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYFILLARC:
        request = _parseRequest<
            protocol::requests::PolyFillArc >( conn, data, sz );
        break;
    case protocol::requests::opcodes::PUTIMAGE:
        request = _parseRequest<
            protocol::requests::PutImage >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETIMAGE:
        request = _parseRequest<
            protocol::requests::GetImage >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYTEXT8:
        request = _parseRequest<
            protocol::requests::PolyText8 >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYTEXT16:
        request = _parseRequest<
            protocol::requests::PolyText16 >( conn, data, sz );
        break;
    case protocol::requests::opcodes::IMAGETEXT8:
        request = _parseRequest<
            protocol::requests::ImageText8 >( conn, data, sz );
        break;
    case protocol::requests::opcodes::IMAGETEXT16:
        request = _parseRequest<
            protocol::requests::ImageText16 >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATECOLORMAP:
        request = _parseRequest<
            protocol::requests::CreateColormap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREECOLORMAP:
        request = _parseRequest<
            protocol::requests::FreeColormap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::COPYCOLORMAPANDFREE:
        request = _parseRequest<
            protocol::requests::CopyColormapAndFree >( conn, data, sz );
        break;
    case protocol::requests::opcodes::INSTALLCOLORMAP:
        request = _parseRequest<
            protocol::requests::InstallColormap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNINSTALLCOLORMAP:
        request = _parseRequest<
            protocol::requests::UninstallColormap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTINSTALLEDCOLORMAPS:
        request = _parseRequest<
            protocol::requests::ListInstalledColormaps >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLOR:
        request = _parseRequest<
            protocol::requests::AllocColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCNAMEDCOLOR:
        request = _parseRequest<
            protocol::requests::AllocNamedColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLORCELLS:
        request = _parseRequest<
            protocol::requests::AllocColorCells >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLORPLANES:
        request = _parseRequest<
            protocol::requests::AllocColorPlanes >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREECOLORS:
        request = _parseRequest<
            protocol::requests::FreeColors >( conn, data, sz );
        break;
    case protocol::requests::opcodes::STORECOLORS:
        request = _parseRequest<
            protocol::requests::StoreColors >( conn, data, sz );
        break;
    case protocol::requests::opcodes::STORENAMEDCOLOR:
        request = _parseRequest<
            protocol::requests::StoreNamedColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYCOLORS:
        request = _parseRequest<
            protocol::requests::QueryColors >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LOOKUPCOLOR:
        request = _parseRequest<
            protocol::requests::LookupColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATECURSOR:
        request = _parseRequest<
            protocol::requests::CreateCursor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATEGLYPHCURSOR:
        request = _parseRequest<
            protocol::requests::CreateGlyphCursor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREECURSOR:
        request = _parseRequest<
            protocol::requests::FreeCursor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::RECOLORCURSOR:
        request = _parseRequest<
            protocol::requests::RecolorCursor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYBESTSIZE:
        request = _parseRequest<
            protocol::requests::QueryBestSize >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYEXTENSION:
        request = _parseRequest<
            protocol::requests::QueryExtension >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTEXTENSIONS:
        request = _parseRequest<
            protocol::requests::ListExtensions >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEKEYBOARDMAPPING:
        request = _parseRequest<
            protocol::requests::ChangeKeyboardMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETKEYBOARDMAPPING:
        request = _parseRequest<
            protocol::requests::GetKeyboardMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEKEYBOARDCONTROL:
        request = _parseRequest<
            protocol::requests::ChangeKeyboardControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETKEYBOARDCONTROL:
        request = _parseRequest<
            protocol::requests::GetKeyboardControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::BELL:
        request = _parseRequest<
            protocol::requests::Bell >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEPOINTERCONTROL:
        request = _parseRequest<
            protocol::requests::ChangePointerControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPOINTERCONTROL:
        request = _parseRequest<
            protocol::requests::GetPointerControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETSCREENSAVER:
        request = _parseRequest<
            protocol::requests::SetScreenSaver >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETSCREENSAVER:
        request = _parseRequest<
            protocol::requests::GetScreenSaver >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEHOSTS:
        request = _parseRequest<
            protocol::requests::ChangeHosts >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTHOSTS:
        request = _parseRequest<
            protocol::requests::ListHosts >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETACCESSCONTROL:
        request = _parseRequest<
            protocol::requests::SetAccessControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETCLOSEDOWNMODE:
        request = _parseRequest<
            protocol::requests::SetCloseDownMode >( conn, data, sz );
        break;
    case protocol::requests::opcodes::KILLCLIENT:
        request = _parseRequest<
            protocol::requests::KillClient >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ROTATEPROPERTIES:
        request = _parseRequest<
            protocol::requests::RotateProperties >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FORCESCREENSAVER:
        request = _parseRequest<
            protocol::requests::ForceScreenSaver >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETPOINTERMAPPING:
        request = _parseRequest<
            protocol::requests::SetPointerMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPOINTERMAPPING:
        request = _parseRequest<
            protocol::requests::GetPointerMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETMODIFIERMAPPING:
        request = _parseRequest<
            protocol::requests::SetModifierMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETMODIFIERMAPPING:
        request = _parseRequest<
            protocol::requests::GetModifierMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::NOOPERATION:
        request = _parseRequest<
            protocol::requests::NoOperation >( conn, data, sz );
        break;
    default:
        // TBD extension logging function dispatch
        break;
    };
    fmt::println( settings.log_fs,
                  "C{:03d}:{:04d}B:{}:S{:05d}: Request {}({}): {}",
                  conn->id, request.bytes_parsed, CLIENT_TO_SERVER, sequence,
                  protocol::requests::names[ opcode ], opcode, request.str );
    return request.bytes_parsed;
}
