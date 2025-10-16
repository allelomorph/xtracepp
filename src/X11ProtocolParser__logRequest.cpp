#include <string_view>
#include <unordered_set>
#include <type_traits>

#include <cctype>      // isprint

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
//#include "Settings.hpp"
#include "protocol/common_types.hpp"
#include "protocol/requests.hpp"
#include "protocol/events.hpp"
#include "protocol/atoms.hpp"


X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseSimpleRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::impl::SimpleRequest;
    const SimpleRequest::Encoding* encoding {
        reinterpret_cast< const SimpleRequest::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SimpleRequest::Encoding );
    static const std::unordered_set<uint8_t> supported_opcodes {
        protocol::requests::opcodes::GRABSERVER,
        protocol::requests::opcodes::UNGRABSERVER,
        protocol::requests::opcodes::GETINPUTFOCUS,
        protocol::requests::opcodes::QUERYKEYMAP,
        protocol::requests::opcodes::GETFONTPATH,
        protocol::requests::opcodes::LISTEXTENSIONS,
        protocol::requests::opcodes::GETKEYBOARDCONTROL,
        protocol::requests::opcodes::GETPOINTERCONTROL,
        protocol::requests::opcodes::GETSCREENSAVER,
        protocol::requests::opcodes::LISTHOSTS,
        protocol::requests::opcodes::GETPOINTERMAPPING,
        protocol::requests::opcodes::GETMODIFIERMAPPING
    };
    assert( supported_opcodes.count( encoding->opcode ) != 0 );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseSimpleWindowRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::impl::SimpleWindowRequest;
    const SimpleWindowRequest::Encoding* encoding {
        reinterpret_cast< const SimpleWindowRequest::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SimpleWindowRequest::Encoding );
    static const std::unordered_set<uint8_t> supported_opcodes {
        protocol::requests::opcodes::GETWINDOWATTRIBUTES,
        protocol::requests::opcodes::DESTROYWINDOW,
        protocol::requests::opcodes::DESTROYSUBWINDOWS,
        protocol::requests::opcodes::MAPWINDOW,
        protocol::requests::opcodes::MAPSUBWINDOWS,
        protocol::requests::opcodes::UNMAPWINDOW,
        protocol::requests::opcodes::UNMAPSUBWINDOWS,
        protocol::requests::opcodes::QUERYTREE,
        protocol::requests::opcodes::LISTPROPERTIES,
        protocol::requests::opcodes::QUERYPOINTER,
        protocol::requests::opcodes::LISTINSTALLEDCOLORMAPS
    };
    assert( supported_opcodes.count( encoding->opcode ) != 0 );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( bytes_parsed == sz );
    return request;
}

X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseListFontsRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ListFonts;
    const ListFonts::Encoding* encoding {
        reinterpret_cast< const ListFonts::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ListFonts::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::LISTFONTS ||
            encoding->opcode == protocol::requests::opcodes::LISTFONTSWITHINFO );

    std::string_view pattern {
        reinterpret_cast< const char* >( data + request.bytes_parsed ),
        encoding->n };
    request.bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "max-names", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->max_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "pattern", name_width, _ROOT_WS.equals,
        pattern, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CreateWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::CreateWindow;
    const CreateWindow::Encoding* encoding {
        reinterpret_cast< const CreateWindow::Encoding* >( data ) };
    request.bytes_parsed += sizeof( CreateWindow::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CREATEWINDOW );

    // TBD lifetime seems too short (causes read-after-free segfaults) if initialized
    //   nested in value_list_inputs initializer
    const std::vector< _VALUETraits > value_traits {
        /* background-pixmap     */ { CreateWindow::background_pixmap_names },
        /* background-pixel      */ {},
        /* border-pixmap         */ { CreateWindow::border_pixmap_names },
        /* border-pixel          */ {},
        /* bit-gravity           */ {},
        /* win-gravity           */ {},
        /* backing-store         */ { CreateWindow::backing_store_names },
        /* backing-planes        */ {},
        /* backing-pixel         */ {},
        /* override-redirect     */ {},
        /* save-under            */ {},
        /* event-mask            */ {},
        /* do-not-propagate-mask */ {},
        /* colormap              */ { CreateWindow::colormap_names },
        /* cursor                */ { CreateWindow::cursor_names }
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        encoding->value_mask,
        CreateWindow::value_types,
        CreateWindow::value_names,
        value_traits,
        _ROOT_WS.nested()
    };
    _ParsingOutputs value_list_outputs;
    _parseLISTofVALUE(
        value_list_inputs, data + request.bytes_parsed, &value_list_outputs );
    request.bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "depth", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->depth ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "wid", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->wid ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "parent", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->parent ), _ROOT_WS.separator,
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
        _ROOT_WS.memb_indent, "class", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->class_,
                        protocol::enum_names::window_class ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "visual", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->visual,
                           protocol::enum_names::zero_copy_from_parent ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "value-mask", name_width, _ROOT_WS.equals,
        _formatBitmask( encoding->value_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "value-list", name_width, _ROOT_WS.equals,
        value_list_outputs.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeWindowAttributes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ChangeWindowAttributes;
    const ChangeWindowAttributes::Encoding* encoding {
        reinterpret_cast< const ChangeWindowAttributes::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ChangeWindowAttributes::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEWINDOWATTRIBUTES );

    // TBD lifetime seems too short (causes read-after-free segfaults) if initialized
    //   nested in value_list_inputs initializer
    // VALUE list encoding same as CreateWindow
    const std::vector< _VALUETraits > value_traits {
        /* background-pixmap     */ { ChangeWindowAttributes::background_pixmap_names },
        /* background-pixel      */ {},
        /* border-pixmap         */ { ChangeWindowAttributes::border_pixmap_names },
        /* border-pixel          */ {},
        /* bit-gravity           */ {},
        /* win-gravity           */ {},
        /* backing-store         */ { ChangeWindowAttributes::backing_store_names },
        /* backing-planes        */ {},
        /* backing-pixel         */ {},
        /* override-redirect     */ {},
        /* save-under            */ {},
        /* event-mask            */ {},
        /* do-not-propagate-mask */ {},
        /* colormap              */ { ChangeWindowAttributes::colormap_names },
        /* cursor                */ { ChangeWindowAttributes::cursor_names }
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        encoding->value_mask,
        ChangeWindowAttributes::value_types,
        ChangeWindowAttributes::value_names,
        value_traits,
        _ROOT_WS.nested()
    };
    _ParsingOutputs value_list_outputs;
    _parseLISTofVALUE( value_list_inputs, data + request.bytes_parsed, &value_list_outputs );
    request.bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "value-mask", name_width, _ROOT_WS.equals,
        _formatBitmask( encoding->value_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "value-list", name_width, _ROOT_WS.equals,
        value_list_outputs.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeSaveSet >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ChangeSaveSet;
    const ChangeSaveSet::Encoding* encoding {
        reinterpret_cast< const ChangeSaveSet::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ChangeSaveSet::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGESAVESET );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->mode, ChangeSaveSet::mode_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ReparentWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ReparentWindow;
    const ReparentWindow::Encoding* encoding {
        reinterpret_cast< const ReparentWindow::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ReparentWindow::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::REPARENTWINDOW );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "parent", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->y ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ConfigureWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ConfigureWindow;
    const ConfigureWindow::Encoding* encoding {
        reinterpret_cast< const ConfigureWindow::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ConfigureWindow::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CONFIGUREWINDOW );

    // TBD lifetime seems too short (causes read-after-free segfaults) if initialized
    //   nested in value_list_inputs initializer
    // VALUE list encoding same as CreateWindow
    const std::vector< _VALUETraits > value_traits {
        /* x            */ {},
        /* y            */ {},
        /* width        */ {},
        /* height       */ {},
        /* border-width */ {},
        /* sibling      */ {},
        /* stack-mode   */ { ConfigureWindow::stack_mode_names },
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        encoding->value_mask,
        ConfigureWindow::value_types,
        ConfigureWindow::value_names,
        value_traits,
        _ROOT_WS.nested()
    };
    _ParsingOutputs value_list_outputs;
    _parseLISTofVALUE( value_list_inputs, data + request.bytes_parsed, &value_list_outputs );
    request.bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "value-mask", name_width, _ROOT_WS.equals,
        _formatBitmask( encoding->value_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "value-list", name_width, _ROOT_WS.equals,
        value_list_outputs.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CirculateWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::CirculateWindow;
    const CirculateWindow::Encoding* encoding {
        reinterpret_cast< const CirculateWindow::Encoding* >( data ) };
    request.bytes_parsed += sizeof( CirculateWindow::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CIRCULATEWINDOW );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "direction", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->direction, CirculateWindow::direction_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetGeometry >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::GetGeometry;
    const GetGeometry::Encoding* encoding {
        reinterpret_cast< const GetGeometry::Encoding* >( data ) };
    request.bytes_parsed += sizeof( GetGeometry::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETGEOMETRY );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::InternAtom >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::InternAtom;
    const InternAtom::Encoding* encoding {
        reinterpret_cast< const InternAtom::Encoding* >( data ) };
    request.bytes_parsed += sizeof( InternAtom::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::INTERNATOM );
    assert( encoding->n > 0 );
    // followed by pad(n) STRING8 name
    std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ), encoding->n };
    request.bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    // Stash copy of atom until reply comes in - at that time we will include it
    //   in our own internment if it isn't already
    _stashAtom( { conn->id, conn->sequence }, name );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "only-if-exists", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->only_if_exists ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "name", name_width, _ROOT_WS.equals,
        name, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetAtomName >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::GetAtomName;
    const GetAtomName::Encoding* encoding {
        reinterpret_cast< const GetAtomName::Encoding* >( data ) };
    request.bytes_parsed += sizeof( GetAtomName::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETATOMNAME );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "atom", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->atom ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ChangeProperty;
    const ChangeProperty::Encoding* encoding {
        reinterpret_cast< const ChangeProperty::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ChangeProperty::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEPROPERTY );
    assert( encoding->format <= 32 && encoding->format % 8 == 0 );
    // followed by LISTofBYTE data
    const uint32_t data_sz { encoding->fmt_unit_ct * ( encoding->format / 8 ) };
    _ParsingOutputs data_;
    if ( encoding->type.data == protocol::atoms::predefined::STRING ) {
        data_.str = fmt::format(
            "{:?}", std::string_view{
                reinterpret_cast< const char* >( data + request.bytes_parsed ), data_sz } );
        data_.bytes_parsed = data_sz;
    } else {
        data_ = _parseLISTof< protocol::BYTE >(
            data + request.bytes_parsed, sz - request.bytes_parsed, data_sz,
            _ROOT_WS.nested( _Whitespace::SINGLELINE ) );
    }
    request.bytes_parsed += _pad(data_.bytes_parsed);
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );

    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->mode, ChangeProperty::mode_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "property", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->property ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "type", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->type ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "format", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->format ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "format units", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->fmt_unit_ct ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "data", name_width, _ROOT_WS.equals,
        data_.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::DeleteProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::DeleteProperty;
    const DeleteProperty::Encoding* encoding {
        reinterpret_cast< const DeleteProperty::Encoding* >( data ) };
    request.bytes_parsed += sizeof( DeleteProperty::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::DELETEPROPERTY );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "property", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->property ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::GetProperty;
    const GetProperty::Encoding* encoding {
        reinterpret_cast< const GetProperty::Encoding* >( data ) };
    request.bytes_parsed += sizeof( GetProperty::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETPROPERTY );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "delete", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->delete_ ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "property", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->property ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "type", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->type, GetProperty::type_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "long-offset", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->long_offset ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "long-length", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->long_length ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetSelectionOwner >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::SetSelectionOwner;
    const SetSelectionOwner::Encoding* encoding {
        reinterpret_cast< const SetSelectionOwner::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SetSelectionOwner::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETSELECTIONOWNER );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "owner", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->owner, SetSelectionOwner::owner_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "selection", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->selection ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "time", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->time, SetSelectionOwner::time_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetSelectionOwner >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::GetSelectionOwner;
    const GetSelectionOwner::Encoding* encoding {
        reinterpret_cast< const GetSelectionOwner::Encoding* >( data ) };
    request.bytes_parsed += sizeof( GetSelectionOwner::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETSELECTIONOWNER );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "selection", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->selection ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ConvertSelection >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ConvertSelection;
    const ConvertSelection::Encoding* encoding {
        reinterpret_cast< const ConvertSelection::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ConvertSelection::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CONVERTSELECTION );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "requestor", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->requestor ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "selection", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->selection ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "target", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->target ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "property", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->property, ConvertSelection::property_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "time", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->time, ConvertSelection::time_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SendEvent >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::SendEvent;
    const SendEvent::Encoding* encoding {
        reinterpret_cast< const SendEvent::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SendEvent::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SENDEVENT );

    const uint8_t evt_code {
        reinterpret_cast< const protocol::events::Event::Header* >(
            data + request.bytes_parsed )->code };
    const _ParsingOutputs event { _parseEvent(
            conn, data + request.bytes_parsed, protocol::events::Event::ENCODING_SZ,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += event.bytes_parsed;
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}({}) {}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "propagate", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->propagate ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "destination", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->destination, SendEvent::destination_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "event-mask", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->event_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "event", name_width, _ROOT_WS.equals,
        protocol::events::names[ evt_code ], evt_code,
        event.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GrabPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::GrabPointer;
    const GrabPointer::Encoding* encoding {
        reinterpret_cast< const GrabPointer::Encoding* >( data ) };
    request.bytes_parsed += sizeof( GrabPointer::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GRABPOINTER );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "owner-events", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->owner_events ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "grab-window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->grab_window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "event-mask", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->event_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "pointer-mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->pointer_mode, GrabPointer::pointer_mode_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "keyboard-mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->keyboard_mode, GrabPointer::keyboard_mode_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "confine-to", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->confine_to, GrabPointer::confine_to_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "cursor", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cursor, GrabPointer::cursor_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "time", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->time, GrabPointer::time_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UngrabPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::UngrabPointer;
    const UngrabPointer::Encoding* encoding {
        reinterpret_cast< const UngrabPointer::Encoding* >( data ) };
    request.bytes_parsed += sizeof( UngrabPointer::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::UNGRABPOINTER );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "time", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->time, UngrabPointer::time_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GrabButton >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::GrabButton;
    const GrabButton::Encoding* encoding {
        reinterpret_cast< const GrabButton::Encoding* >( data ) };
    request.bytes_parsed += sizeof( GrabButton::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GRABBUTTON );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "owner-events", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->owner_events ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "grab-window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->grab_window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "event-mask", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->event_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "pointer-mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->pointer_mode, GrabButton::pointer_mode_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "keyboard-mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->keyboard_mode, GrabButton::keyboard_mode_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "confine-to", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->confine_to, GrabButton::confine_to_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "cursor", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cursor, GrabButton::cursor_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "button", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->button, GrabButton::button_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "modifiers", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->modifiers ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UngrabButton >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::UngrabButton;
    const UngrabButton::Encoding* encoding {
        reinterpret_cast< const UngrabButton::Encoding* >( data ) };
    request.bytes_parsed += sizeof( UngrabButton::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::UNGRABBUTTON );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "button", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->button, UngrabButton::button_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "grab-window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->grab_window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "modifiers", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->modifiers ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeActivePointerGrab >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ChangeActivePointerGrab;
    const ChangeActivePointerGrab::Encoding* encoding {
        reinterpret_cast< const ChangeActivePointerGrab::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ChangeActivePointerGrab::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEACTIVEPOINTERGRAB );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cursor", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cursor, ChangeActivePointerGrab::cursor_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "time", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->time, ChangeActivePointerGrab::time_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "event-mask", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->event_mask ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GrabKeyboard >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::GrabKeyboard;
    const GrabKeyboard::Encoding* encoding {
        reinterpret_cast< const GrabKeyboard::Encoding* >( data ) };
    request.bytes_parsed += sizeof( GrabKeyboard::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GRABKEYBOARD );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "owner-events", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->owner_events ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "grab-window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->grab_window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "time", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->time, GrabKeyboard::time_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "pointer-mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->pointer_mode, GrabKeyboard::pointer_mode_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "keyboard-mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->keyboard_mode, GrabKeyboard::keyboard_mode_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UngrabKeyboard >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::UngrabKeyboard;
    const UngrabKeyboard::Encoding* encoding {
        reinterpret_cast< const UngrabKeyboard::Encoding* >( data ) };
    request.bytes_parsed += sizeof( UngrabKeyboard::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::UNGRABKEYBOARD );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "time", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->time, UngrabKeyboard::time_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GrabKey >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::GrabKey;
    const GrabKey::Encoding* encoding {
        reinterpret_cast< const GrabKey::Encoding* >( data ) };
    request.bytes_parsed += sizeof( GrabKey::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GRABKEY );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "owner-events", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->owner_events ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "modifiers", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->modifiers ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "key", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->key, GrabKey::key_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "pointer-mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->pointer_mode, GrabKey::pointer_mode_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "keyboard-mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->keyboard_mode, GrabKey::keyboard_mode_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UngrabKey >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::UngrabKey;
    const UngrabKey::Encoding* encoding {
        reinterpret_cast< const UngrabKey::Encoding* >( data ) };
    request.bytes_parsed += sizeof( UngrabKey::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::UNGRABKEY );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "key", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->key, UngrabKey::key_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "grab-window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->grab_window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "modifiers", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->modifiers ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::AllowEvents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::AllowEvents;
    const AllowEvents::Encoding* encoding {
        reinterpret_cast< const AllowEvents::Encoding* >( data ) };
    request.bytes_parsed += sizeof( AllowEvents::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::ALLOWEVENTS );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->mode, AllowEvents::mode_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "time", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->time, AllowEvents::time_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetMotionEvents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::GetMotionEvents;
    const GetMotionEvents::Encoding* encoding {
        reinterpret_cast< const GetMotionEvents::Encoding* >( data ) };
    request.bytes_parsed += sizeof( GetMotionEvents::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETMOTIONEVENTS );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "start", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->start, GetMotionEvents::start_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "stop", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->stop, GetMotionEvents::stop_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::TranslateCoordinates >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::TranslateCoordinates;
    const TranslateCoordinates::Encoding* encoding {
        reinterpret_cast< const TranslateCoordinates::Encoding* >( data ) };
    request.bytes_parsed += sizeof( TranslateCoordinates::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::TRANSLATECOORDINATES );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "src-window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->src_window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->dst_window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "src-x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->src_x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "src-y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->src_y ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::WarpPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::WarpPointer;
    const WarpPointer::Encoding* encoding {
        reinterpret_cast< const WarpPointer::Encoding* >( data ) };
    request.bytes_parsed += sizeof( WarpPointer::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::WARPPOINTER );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "src-window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->src_window, WarpPointer::src_window_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->dst_window, WarpPointer::dst_window_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "src-x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->src_x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "src-y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->src_y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "src-width", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->src_width ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "src-height", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->src_height ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->dst_x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->dst_y ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetInputFocus >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::SetInputFocus;
    const SetInputFocus::Encoding* encoding {
        reinterpret_cast< const SetInputFocus::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SetInputFocus::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETINPUTFOCUS );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "revert-to", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->revert_to, SetInputFocus::revert_to_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "focus", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->focus, SetInputFocus::focus_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "time", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->time, SetInputFocus::time_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::OpenFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::OpenFont;
    const OpenFont::Encoding* encoding {
        reinterpret_cast< const OpenFont::Encoding* >( data ) };
    request.bytes_parsed += sizeof( OpenFont::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::OPENFONT );

    std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ), encoding->n };
    request.bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "fid", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->fid ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "name", name_width, _ROOT_WS.equals,
        name, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CloseFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::CloseFont;
    const CloseFont::Encoding* encoding {
        reinterpret_cast< const CloseFont::Encoding* >( data ) };
    request.bytes_parsed += sizeof( CloseFont::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CLOSEFONT );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "font", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->font ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::QueryFont;
    const QueryFont::Encoding* encoding {
        reinterpret_cast< const QueryFont::Encoding* >( data ) };
    request.bytes_parsed += sizeof( QueryFont::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::QUERYFONT );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "font", name_width, _ROOT_WS.equals,
        // TBD is it necessary to resolve FONT or GCONTEXT from FONTABLE?
        _formatProtocolType( encoding->font.font ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryTextExtents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::QueryTextExtents;
    const QueryTextExtents::Encoding* encoding {
        reinterpret_cast< const QueryTextExtents::Encoding* >( data ) };
    request.bytes_parsed += sizeof( QueryTextExtents::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::QUERYTEXTEXTENTS );

    // first calc padded string length due to ambiguity around odd-length
    const size_t string_sz {
        ( encoding->request_length * _ALIGN ) - sizeof( QueryTextExtents::Encoding ) };
    const size_t n_CHAR2B {
        ( string_sz / sizeof( protocol::CHAR2B/*char16_t*/ ) ) -
        ( encoding->odd_length.data ? 1 : 0 ) };
    _ParsingOutputs string {
        _parseLISTof< protocol::CHAR2B >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_CHAR2B,
            _ROOT_WS.nested( _Whitespace::SINGLELINE ) ) };
    // bypass normal use of _Parsingoutputs.bytes_parsed due to odd-length
    request.bytes_parsed += string_sz;
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "odd length", name_width, _ROOT_WS.equals,
            _formatProtocolType( encoding->odd_length ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "font", name_width, _ROOT_WS.equals,
        // TBD is it necessary to resolve FONT or GCONTEXT from FONTABLE?
        _formatProtocolType( encoding->font.font ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "string", name_width, _ROOT_WS.equals,
        string.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetFontPath >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::SetFontPath;
    const SetFontPath::Encoding* encoding {
        reinterpret_cast< const SetFontPath::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SetFontPath::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETFONTPATH );

    _ParsingOutputs path {
        _parseLISTof< protocol::STR >(
            data + request.bytes_parsed, sz - request.bytes_parsed, encoding->str_ct,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( path.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "STRs in path", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->str_ct ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "path", name_width, _ROOT_WS.equals,
        path.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CreatePixmap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::CreatePixmap;
    const CreatePixmap::Encoding* encoding {
        reinterpret_cast< const CreatePixmap::Encoding* >( data ) };
    request.bytes_parsed += sizeof( CreatePixmap::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CREATEPIXMAP );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "depth", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->depth ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "pid", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->pid ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "width", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->width ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "height", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->height ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::FreePixmap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::FreePixmap;
    const FreePixmap::Encoding* encoding {
        reinterpret_cast< const FreePixmap::Encoding* >( data ) };
    request.bytes_parsed += sizeof( FreePixmap::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FREEPIXMAP );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "pixmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->pixmap ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CreateGC >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::CreateGC;
    const CreateGC::Encoding* encoding {
        reinterpret_cast< const CreateGC::Encoding* >( data ) };
    request.bytes_parsed += sizeof( CreateGC::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CREATEGC );

    // TBD lifetime seems too short (causes read-after-free segfaults) if initialized
    //   nested in value_list_inputs initializer
    const std::vector< _VALUETraits > value_traits {
        /* function              */ { CreateGC::function_names },
        /* plane-mask            */ {},
        /* foreground            */ {},
        /* background            */ {},
        /* line-width            */ {},
        /* line-style            */ { CreateGC::line_style_names },
        /* cap-style             */ { CreateGC::cap_style_names },
        /* join-style            */ { CreateGC::join_style_names },
        /* fill-style            */ { CreateGC::fill_style_names },
        /* fill-rule             */ { CreateGC::fill_rule_names },
        /* tile                  */ {},
        /* stipple               */ {},
        /* tile-stipple-x-origin */ {},
        /* tile-stipple-y-origin */ {},
        /* font                  */ {},
        /* subwindow-mode        */ { CreateGC::subwindow_mode_names },
        /* graphics-exposures    */ {},
        /* clip-x-origin         */ {},
        /* clip-y-origin         */ {},
        /* clip-mask             */ { CreateGC::clip_mask_names },
        /* dash-offset           */ {},
        /* dashes                */ {},
        /* arc-mode              */ { CreateGC::arc_mode_names }
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        encoding->value_mask,
        CreateGC::value_types,
        CreateGC::value_names,
        value_traits,
        _ROOT_WS.nested()
    };
    _ParsingOutputs value_list_outputs;
    _parseLISTofVALUE( value_list_inputs, data + request.bytes_parsed, &value_list_outputs );
    request.bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cid", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cid ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        // TBD only print value-mask in verbose mode? print all flags even with value-list?
        _ROOT_WS.memb_indent, "value-mask", name_width, _ROOT_WS.equals,
        _formatBitmask( encoding->value_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "value-list", name_width, _ROOT_WS.equals,
        value_list_outputs.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeGC >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ChangeGC;
    const ChangeGC::Encoding* encoding {
        reinterpret_cast< const ChangeGC::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ChangeGC::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEGC );

    // TBD lifetime seems too short (causes read-after-free segfaults) if initialized
    //   nested in value_list_inputs initializer
    const std::vector< _VALUETraits > value_traits {
        /* function              */ { ChangeGC::function_names },
        /* plane-mask            */ {},
        /* foreground            */ {},
        /* background            */ {},
        /* line-width            */ {},
        /* line-style            */ { ChangeGC::line_style_names },
        /* cap-style             */ { ChangeGC::cap_style_names },
        /* join-style            */ { ChangeGC::join_style_names },
        /* fill-style            */ { ChangeGC::fill_style_names },
        /* fill-rule             */ { ChangeGC::fill_rule_names },
        /* tile                  */ {},
        /* stipple               */ {},
        /* tile-stipple-x-origin */ {},
        /* tile-stipple-y-origin */ {},
        /* font                  */ {},
        /* subwindow-mode        */ { ChangeGC::subwindow_mode_names },
        /* graphics-exposures    */ {},
        /* clip-x-origin         */ {},
        /* clip-y-origin         */ {},
        /* clip-mask             */ { ChangeGC::clip_mask_names },
        /* dash-offset           */ {},
        /* dashes                */ {},
        /* arc-mode              */ { ChangeGC::arc_mode_names }
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        encoding->value_mask,
        ChangeGC::value_types,
        ChangeGC::value_names,
        value_traits,
        _ROOT_WS.nested()
    };
    _ParsingOutputs value_list_outputs;
    _parseLISTofVALUE( value_list_inputs, data + request.bytes_parsed, &value_list_outputs );
    request.bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        // TBD only print value-mask in verbose mode? print all flags even with value-list?
        _ROOT_WS.memb_indent, "value-mask", name_width, _ROOT_WS.equals,
        _formatBitmask( encoding->value_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "value-list", name_width, _ROOT_WS.equals,
        value_list_outputs.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CopyGC >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::CopyGC;
    const CopyGC::Encoding* encoding {
        reinterpret_cast< const CopyGC::Encoding* >( data ) };
    request.bytes_parsed += sizeof( CopyGC::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::COPYGC );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "src-gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->src_gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->dst_gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "value-mask", name_width, _ROOT_WS.equals,
        _formatBitmask( encoding->value_mask, CopyGC::value_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetDashes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::SetDashes;
    const SetDashes::Encoding* encoding {
        reinterpret_cast< const SetDashes::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SetDashes::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETDASHES );

    _ParsingOutputs dashes {
        _parseLISTof< protocol::CARD8 >(
            data + request.bytes_parsed, sz - request.bytes_parsed, encoding->n,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( dashes.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dash-offset", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->dash_offset ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "dashes", name_width, _ROOT_WS.equals,
        dashes.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetClipRectangles >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::SetClipRectangles;
    const SetClipRectangles::Encoding* encoding {
        reinterpret_cast< const SetClipRectangles::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SetClipRectangles::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETCLIPRECTANGLES );

    const size_t n_rectangles {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( SetClipRectangles::Encoding ) ) /
        sizeof( protocol::RECTANGLE ) };
    _ParsingOutputs rectangles {
        _parseLISTof< protocol::RECTANGLE >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_rectangles,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( rectangles.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "ordering", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->ordering, SetClipRectangles::ordering_names  ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "clip-x-origin", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->clip_x_origin ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "clip-y-origin", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->clip_y_origin ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "rectangles", name_width, _ROOT_WS.equals,
        rectangles.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::FreeGC >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::FreeGC;
    const FreeGC::Encoding* encoding {
        reinterpret_cast< const FreeGC::Encoding* >( data ) };
    request.bytes_parsed += sizeof( FreeGC::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FREEGC );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ClearArea >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ClearArea;
    const ClearArea::Encoding* encoding {
        reinterpret_cast< const ClearArea::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ClearArea::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CLEARAREA );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "exposures", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->exposures ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "width", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->width ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "height", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->height ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CopyArea >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::CopyArea;
    const CopyArea::Encoding* encoding {
        reinterpret_cast< const CopyArea::Encoding* >( data ) };
    request.bytes_parsed += sizeof( CopyArea::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::COPYAREA );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "src-drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->src_drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->dst_drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "src-x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->src_x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "src-y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->src_y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->dst_x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->dst_y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "width", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->width ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "height", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->height ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CopyPlane >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::CopyPlane;
    const CopyPlane::Encoding* encoding {
        reinterpret_cast< const CopyPlane::Encoding* >( data ) };
    request.bytes_parsed += sizeof( CopyPlane::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::COPYPLANE );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "src-drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->src_drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->dst_drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "src-x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->src_x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "src-y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->src_y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->dst_x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->dst_y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "width", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->width ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "height", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->height ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "bit-plane", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->bit_plane ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyPoint >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::PolyPoint;
    const PolyPoint::Encoding* encoding {
        reinterpret_cast< const PolyPoint::Encoding* >( data ) };
    request.bytes_parsed += sizeof( PolyPoint::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYPOINT );

    const size_t n_points {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolyPoint::Encoding ) ) / sizeof( protocol::POINT ) };
    _ParsingOutputs points {
        _parseLISTof< protocol::POINT >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_points,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( points.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "coordinate-mode" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "coordinate-mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->coordinate_mode, PolyPoint::coordinate_mode_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "points", name_width, _ROOT_WS.equals,
        points.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

// TBD same encoding as PolyPoint; generalize?
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyLine >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::PolyLine;
    const PolyLine::Encoding* encoding {
        reinterpret_cast< const PolyLine::Encoding* >( data ) };
    request.bytes_parsed += sizeof( PolyLine::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYLINE );

    const size_t n_points {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolyLine::Encoding ) ) / sizeof( protocol::POINT ) };
    _ParsingOutputs points {
        _parseLISTof< protocol::POINT >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_points,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( points.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "coordinate-mode" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "coordinate-mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->coordinate_mode, PolyLine::coordinate_mode_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "points", name_width, _ROOT_WS.equals,
        points.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolySegment >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::PolySegment;
    const PolySegment::Encoding* encoding {
        reinterpret_cast< const PolySegment::Encoding* >( data ) };
    request.bytes_parsed += sizeof( PolySegment::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYSEGMENT );

    const size_t n_segments {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolySegment::Encoding ) ) / sizeof( PolySegment::SEGMENT ) };
    _ParsingOutputs segments {
        _parseLISTof< PolySegment::SEGMENT >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_segments,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( segments.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "segments", name_width, _ROOT_WS.equals,
        segments.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyRectangle >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::PolyRectangle;
    const PolyRectangle::Encoding* encoding {
        reinterpret_cast< const PolyRectangle::Encoding* >( data ) };
    request.bytes_parsed += sizeof( PolyRectangle::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYRECTANGLE );

    const size_t n_rectangles {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolyRectangle::Encoding ) ) / sizeof( protocol::RECTANGLE ) };
    _ParsingOutputs rectangles {
        _parseLISTof< protocol::RECTANGLE >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_rectangles,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( rectangles.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "rectangles", name_width, _ROOT_WS.equals,
        rectangles.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyArc >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::PolyArc;
    const PolyArc::Encoding* encoding {
        reinterpret_cast< const PolyArc::Encoding* >( data ) };
    request.bytes_parsed += sizeof( PolyArc::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYARC );

    const size_t n_arcs {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolyArc::Encoding ) ) / sizeof( protocol::ARC ) };
    _ParsingOutputs arcs {
        _parseLISTof< protocol::ARC >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_arcs,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( arcs.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "arcs", name_width, _ROOT_WS.equals,
        arcs.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::FillPoly >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::FillPoly;
    const FillPoly::Encoding* encoding {
        reinterpret_cast< const FillPoly::Encoding* >( data ) };
    request.bytes_parsed += sizeof( FillPoly::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FILLPOLY );

    const size_t n_points {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( FillPoly::Encoding ) ) / sizeof( protocol::POINT ) };
    _ParsingOutputs points {
        _parseLISTof< protocol::POINT >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_points,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( points.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "coordinate-mode" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "shape", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->shape, FillPoly::shape_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "coordinate-mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->coordinate_mode, FillPoly::coordinate_mode_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "points", name_width, _ROOT_WS.equals,
        points.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyFillRectangle >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::PolyFillRectangle;
    const PolyFillRectangle::Encoding* encoding {
        reinterpret_cast< const PolyFillRectangle::Encoding* >( data ) };
    request.bytes_parsed += sizeof( PolyFillRectangle::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYFILLRECTANGLE );

    const size_t n_rectangles {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolyFillRectangle::Encoding ) ) / sizeof( protocol::RECTANGLE ) };
    _ParsingOutputs rectangles {
        _parseLISTof< protocol::RECTANGLE >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_rectangles,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( rectangles.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "rectangles", name_width, _ROOT_WS.equals,
        rectangles.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyFillArc >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::PolyFillArc;
    const PolyFillArc::Encoding* encoding {
        reinterpret_cast< const PolyFillArc::Encoding* >( data ) };
    request.bytes_parsed += sizeof( PolyFillArc::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYFILLARC );

    const size_t n_arcs {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolyFillArc::Encoding ) ) / sizeof( protocol::ARC ) };
    _ParsingOutputs arcs {
        _parseLISTof< protocol::ARC >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_arcs,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( arcs.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "arcs", name_width, _ROOT_WS.equals,
        arcs.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PutImage >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::PutImage;
    const PutImage::Encoding* encoding {
        reinterpret_cast< const PutImage::Encoding* >( data ) };
    request.bytes_parsed += sizeof( PutImage::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::PUTIMAGE );

    const size_t image_data_sz {
        ( encoding->request_length * _ALIGN ) - sizeof( PutImage::Encoding ) };
    // TBD not parsing iamge data as it is not done in xtrace
    request.bytes_parsed += image_data_sz;
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}({} bytes){}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "format", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->format, PutImage::format_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "width", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->width ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "height", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->height ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->dst_x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "dst-y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->dst_y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "left-pad", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->left_pad ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "depth", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->depth ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "data", name_width, _ROOT_WS.equals,
        image_data_sz, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetImage >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::GetImage;
    const GetImage::Encoding* encoding {
        reinterpret_cast< const GetImage::Encoding* >( data ) };
    request.bytes_parsed += sizeof( GetImage::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETIMAGE );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "format", name_width, _ROOT_WS.equals,
        // TBD how can we avoid magic numbers here (or anywhere) for max and min enum?
        _formatInteger( encoding->format,
                        GetImage::format_names, _IndexRange{ 1, 2 } ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "width", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->width ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "height", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->height ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "plane-mask", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->plane_mask ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyText8 >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::PolyText8;
    const PolyText8::Encoding* encoding {
        reinterpret_cast< const PolyText8::Encoding* >( data ) };
    request.bytes_parsed += sizeof( PolyText8::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYTEXT8 );

    const size_t text_item_list_sz {
        ( encoding->request_length * _ALIGN ) - sizeof( PolyText8::Encoding ) };
    _ParsingOutputs text_items {
        _parseLISTof< PolyText8::TEXTITEM8 >(
            data + request.bytes_parsed, text_item_list_sz,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( text_items.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "items", name_width, _ROOT_WS.equals,
        text_items.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::PolyText16 >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::PolyText16;
    const PolyText16::Encoding* encoding {
        reinterpret_cast< const PolyText16::Encoding* >( data ) };
    request.bytes_parsed += sizeof( PolyText16::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYTEXT16 );

    const size_t text_item_list_sz {
        ( encoding->request_length * _ALIGN ) - sizeof( PolyText16::Encoding ) };
    _ParsingOutputs text_items {
        _parseLISTof< PolyText16::TEXTITEM16 >(
            data + request.bytes_parsed, text_item_list_sz,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( text_items.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "items", name_width, _ROOT_WS.equals,
        text_items.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ImageText8 >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ImageText8;
    const ImageText8::Encoding* encoding {
        reinterpret_cast< const ImageText8::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ImageText8::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::IMAGETEXT8 );

    std::string_view string {
        reinterpret_cast< const char* >( data + request.bytes_parsed ), encoding->n };
    request.bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "string", name_width, _ROOT_WS.equals,
        string, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ImageText16 >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ImageText16;
    const ImageText16::Encoding* encoding {
        reinterpret_cast< const ImageText16::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ImageText16::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::IMAGETEXT16 );

    std::u16string_view u16string {
        reinterpret_cast<const char16_t*>( data + request.bytes_parsed ), encoding->n };
    // printing char16_t vals as hex, same as xtrace
    // TBD "This means that clients should always transmit such 16-bit character values most
    // significant byte first, as the server will never byte-swap CHAR2B quantities."
    std::string string_as_hex;
    // fmt counts "0x" as part of width when using '#'
    const size_t c16_hex_width { ( sizeof( char16_t ) * 2 ) + 2 };
    for ( const char16_t c16 : u16string ) {
        string_as_hex += fmt::format(
            "{}{:#0{}x}", string_as_hex.empty() ? "" : " ",
            uint16_t( c16 ), c16_hex_width );
    }
    request.bytes_parsed += _pad( encoding->n * sizeof( protocol::CHAR2B ) );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}[{}]{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "gc", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->gc ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "x", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->x ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "y", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->y ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "string", name_width, _ROOT_WS.equals,
        string_as_hex, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CreateColormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::CreateColormap;
    const CreateColormap::Encoding* encoding {
        reinterpret_cast< const CreateColormap::Encoding* >( data ) };
    request.bytes_parsed += sizeof( CreateColormap::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CREATECOLORMAP );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "alloc", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->alloc, CreateColormap::alloc_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "mid", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->mid ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "visual", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->visual ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::FreeColormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::FreeColormap;
    const FreeColormap::Encoding* encoding {
        reinterpret_cast< const FreeColormap::Encoding* >( data ) };
    request.bytes_parsed += sizeof( FreeColormap::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FREECOLORMAP );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cmap ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CopyColormapAndFree >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::CopyColormapAndFree;
    const CopyColormapAndFree::Encoding* encoding {
        reinterpret_cast< const CopyColormapAndFree::Encoding* >( data ) };
    request.bytes_parsed += sizeof( CopyColormapAndFree::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::COPYCOLORMAPANDFREE );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "mid", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->mid ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "src-cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->src_cmap ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

// TBD generalize shared encoding for InstallColomap/UninstallColomap
template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::InstallColormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::InstallColormap;
    const InstallColormap::Encoding* encoding {
        reinterpret_cast< const InstallColormap::Encoding* >( data ) };
    request.bytes_parsed += sizeof( InstallColormap::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::INSTALLCOLORMAP );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cmap ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::UninstallColormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::UninstallColormap;
    const UninstallColormap::Encoding* encoding {
        reinterpret_cast< const UninstallColormap::Encoding* >( data ) };
    request.bytes_parsed += sizeof( UninstallColormap::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::UNINSTALLCOLORMAP );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cmap ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::AllocColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::AllocColor;
    const AllocColor::Encoding* encoding {
        reinterpret_cast< const AllocColor::Encoding* >( data ) };
    request.bytes_parsed += sizeof( AllocColor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::ALLOCCOLOR );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cmap ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "red", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->red ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "green", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->green ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "blue", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->blue ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::AllocNamedColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::AllocNamedColor;
    const AllocNamedColor::Encoding* encoding {
        reinterpret_cast< const AllocNamedColor::Encoding* >( data ) };
    request.bytes_parsed += sizeof( AllocNamedColor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::ALLOCNAMEDCOLOR );

    std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ), encoding->n };
    request.bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cmap ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "name", name_width, _ROOT_WS.equals,
        name, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::AllocColorCells >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::AllocColorCells;
    const AllocColorCells::Encoding* encoding {
        reinterpret_cast< const AllocColorCells::Encoding* >( data ) };
    request.bytes_parsed += sizeof( AllocColorCells::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::ALLOCCOLORCELLS );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "contiguous", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->contiguous ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cmap ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "colors", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->colors ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "planes", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->planes ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::AllocColorPlanes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::AllocColorPlanes;
    const AllocColorPlanes::Encoding* encoding {
        reinterpret_cast< const AllocColorPlanes::Encoding* >( data ) };
    request.bytes_parsed += sizeof( AllocColorPlanes::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::ALLOCCOLORPLANES );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "contiguous", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->contiguous ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cmap ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "colors", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->colors ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "reds", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->reds ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "greens", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->greens ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "blues", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->blues ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::FreeColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::FreeColors;
    const FreeColors::Encoding* encoding {
        reinterpret_cast< const FreeColors::Encoding* >( data ) };
    request.bytes_parsed += sizeof( FreeColors::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FREECOLORS );

    const size_t n_pixels {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( FreeColors::Encoding ) ) / sizeof( protocol::CARD32 ) };
    _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_pixels,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( pixels.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cmap ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "plane-mask", name_width, _ROOT_WS.equals,
        _formatBitmask( encoding->plane_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "pixels", name_width, _ROOT_WS.equals,
        pixels.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::StoreColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::StoreColors;
    const StoreColors::Encoding* encoding {
        reinterpret_cast< const StoreColors::Encoding* >( data ) };
    request.bytes_parsed += sizeof( StoreColors::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::STORECOLORS );

    const size_t n_items {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( StoreColors::Encoding ) ) / sizeof( StoreColors::COLORITEM ) };
    _ParsingOutputs items {
        _parseLISTof< StoreColors::COLORITEM >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_items,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( items.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cmap ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "items", name_width, _ROOT_WS.equals,
        items.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::StoreNamedColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::StoreNamedColor;
    const StoreNamedColor::Encoding* encoding {
        reinterpret_cast< const StoreNamedColor::Encoding* >( data ) };
    request.bytes_parsed += sizeof( StoreNamedColor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::STORENAMEDCOLOR );

    std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ), encoding->n };
    request.bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "do rgb", name_width, _ROOT_WS.equals,
        _formatBitmask( encoding->do_rgb_mask, StoreNamedColor::do_rgb_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cmap ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "pixel", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->pixel ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "name", name_width, _ROOT_WS.equals,
        name, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::QueryColors;
    const QueryColors::Encoding* encoding {
        reinterpret_cast< const QueryColors::Encoding* >( data ) };
    request.bytes_parsed += sizeof( QueryColors::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::QUERYCOLORS );

    const size_t n_pixels {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( QueryColors::Encoding ) ) / sizeof( protocol::CARD32 ) };
    _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + request.bytes_parsed, sz - request.bytes_parsed, n_pixels,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( pixels.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cmap ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "pixels", name_width, _ROOT_WS.equals,
        pixels.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::LookupColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::LookupColor;
    const LookupColor::Encoding* encoding {
        reinterpret_cast< const LookupColor::Encoding* >( data ) };
    request.bytes_parsed += sizeof( LookupColor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::LOOKUPCOLOR );

    std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ), encoding->n };
    request.bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cmap", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cmap ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "name", name_width, _ROOT_WS.equals,
        name, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CreateCursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::CreateCursor;
    const CreateCursor::Encoding* encoding {
        reinterpret_cast< const CreateCursor::Encoding* >( data ) };
    request.bytes_parsed += sizeof( CreateCursor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CREATECURSOR );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cid", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cid ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "source", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->source ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "mask", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->mask, CreateCursor::mask_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "fore-red", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->fore_red ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "fore-green", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->fore_green ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "fore-blue", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->fore_blue ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "back-red", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->back_red ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "back-green", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->back_green ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "back-blue", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->back_blue ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::CreateGlyphCursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::CreateGlyphCursor;
    const CreateGlyphCursor::Encoding* encoding {
        reinterpret_cast< const CreateGlyphCursor::Encoding* >( data ) };
    request.bytes_parsed += sizeof( CreateGlyphCursor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CREATEGLYPHCURSOR );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cid", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cid ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "source-font", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->source_font ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "mask-font", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->mask_font, CreateGlyphCursor::mask_font_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "source-char", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->source_char ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "mask-char", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->mask_char ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "fore-red", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->fore_red ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "fore-green", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->fore_green ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "fore-blue", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->fore_blue ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "back-red", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->back_red ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "back-green", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->back_green ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "back-blue", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->back_blue ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::FreeCursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::FreeCursor;
    const FreeCursor::Encoding* encoding {
        reinterpret_cast< const FreeCursor::Encoding* >( data ) };
    request.bytes_parsed += sizeof( FreeCursor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FREECURSOR );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cursor", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cursor ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::RecolorCursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::RecolorCursor;
    const RecolorCursor::Encoding* encoding {
        reinterpret_cast< const RecolorCursor::Encoding* >( data ) };
    request.bytes_parsed += sizeof( RecolorCursor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::RECOLORCURSOR );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "cursor", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->cursor ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "fore-red", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->fore_red ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "fore-green", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->fore_green ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "fore-blue", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->fore_blue ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "back-red", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->back_red ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "back-green", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->back_green ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "back-blue", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->back_blue ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryBestSize >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::QueryBestSize;
    const QueryBestSize::Encoding* encoding {
        reinterpret_cast< const QueryBestSize::Encoding* >( data ) };
    request.bytes_parsed += sizeof( QueryBestSize::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::QUERYBESTSIZE );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "class", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->class_, QueryBestSize::class_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "drawable", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->drawable ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "width", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->width ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "height", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->height ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::QueryExtension >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::QueryExtension;
    const QueryExtension::Encoding* encoding {
        reinterpret_cast< const QueryExtension::Encoding* >( data ) };
    request.bytes_parsed += sizeof( QueryExtension::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::QUERYEXTENSION );

    std::string_view name {
        reinterpret_cast< const char* >( data + request.bytes_parsed ), encoding->n };
    request.bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{:?}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->n ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "name", name_width, _ROOT_WS.equals,
        name, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeKeyboardMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ChangeKeyboardMapping;
    const ChangeKeyboardMapping::Encoding* encoding {
        reinterpret_cast< const ChangeKeyboardMapping::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ChangeKeyboardMapping::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEKEYBOARDMAPPING );

    _ParsingOutputs keysyms {
        _parseLISTof< protocol::KEYSYM >(
            data + request.bytes_parsed, sz - request.bytes_parsed,
            encoding->keycode_count * encoding->keysyms_per_keycode,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( keysyms.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "keysyms-per-keycode" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "keycode-count", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->keycode_count ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "first-keycode", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->first_keycode ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "keysyms-per-keycode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->keysyms_per_keycode ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "keysyms", name_width, _ROOT_WS.equals,
        keysyms.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::GetKeyboardMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::GetKeyboardMapping;
    const GetKeyboardMapping::Encoding* encoding {
        reinterpret_cast< const GetKeyboardMapping::Encoding* >( data ) };
    request.bytes_parsed += sizeof( GetKeyboardMapping::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETKEYBOARDMAPPING );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "keysyms-per-keycode" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "first-keycode", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->first_keycode ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "count", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->count ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeKeyboardControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ChangeKeyboardControl;
    const ChangeKeyboardControl::Encoding* encoding {
        reinterpret_cast< const ChangeKeyboardControl::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ChangeKeyboardControl::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEKEYBOARDCONTROL );

    // TBD lifetime seems too short (causes read-after-free segfaults) if initialized
    //   nested in value_list_inputs initializer
    const std::vector< _VALUETraits > value_traits {
        /* key-click-percent */ {},
        /* bell-percent      */ {},
        /* bell-pitch        */ {},
        /* bell-duration     */ {},
        /* led               */ {},
        // TBD how to limit to first two without magic numbers?
        /* led-mode          */ { ChangeKeyboardControl::led_mode_names,
                                  _IndexRange{ 0, 1 } },
        /* key               */ {},
        /* auto-repeat-mode  */ { ChangeKeyboardControl::led_mode_names }
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        encoding->value_mask,
        ChangeKeyboardControl::value_types,
        ChangeKeyboardControl::value_names,
        value_traits,
        _ROOT_WS.nested()
    };
    _ParsingOutputs value_list_outputs;
    _parseLISTofVALUE( value_list_inputs, data + request.bytes_parsed, &value_list_outputs );
    request.bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "value-mask", name_width, _ROOT_WS.equals,
        _formatBitmask( encoding->value_mask ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "value-list", name_width, _ROOT_WS.equals,
        value_list_outputs.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::Bell >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::Bell;
    const Bell::Encoding* encoding {
        reinterpret_cast< const Bell::Encoding* >( data ) };
    request.bytes_parsed += sizeof( Bell::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::BELL );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "percent", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->percent ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangePointerControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ChangePointerControl;
    const ChangePointerControl::Encoding* encoding {
        reinterpret_cast< const ChangePointerControl::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ChangePointerControl::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEPOINTERCONTROL );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "acceleration-denominator" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "acceleration-numerator", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->acceleration_numerator ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "acceleration-denominator", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->acceleration_denominator ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "threshold", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->threshold ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "do-acceleration", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->do_acceleration ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "do-threshold", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->do_threshold ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetScreenSaver >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::SetScreenSaver;
    const SetScreenSaver::Encoding* encoding {
        reinterpret_cast< const SetScreenSaver::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SetScreenSaver::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETSCREENSAVER );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "prefer-blanking" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "timeout", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->timeout ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "interval", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->interval ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "prefer-blanking", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->prefer_blanking, SetScreenSaver::prefer_blanking_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "allow-exposures", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->allow_exposures, SetScreenSaver::allow_exposures_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ChangeHosts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ChangeHosts;
    const ChangeHosts::Encoding* encoding {
        reinterpret_cast< const ChangeHosts::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ChangeHosts::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEHOSTS );

    // TBD format into IPv4/6 instead of byte array? (depends in family)
    //   (xtrace prints as bytes)
    // TBD in the standard, the full description of ChangeHosts anticipates up
    //   to 5 family values, but in the encoding section, family is limited to
    //   Internet, DECnet, or Chaos
    _ParsingOutputs address {
        _parseLISTof< protocol::CARD8 >(
            data + request.bytes_parsed, sz - request.bytes_parsed,
            encoding->length_of_address,
            _ROOT_WS.nested( _Whitespace::SINGLELINE ) ) };
    request.bytes_parsed += _pad( address.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "length of address" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->mode, ChangeHosts::mode_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        // TBD need way to limit enum without magic numbers
        // TBD if using min/max, maybe make _formatInteger param order consistent
        //   with _EnumTraits member order
        _ROOT_WS.memb_indent, "family", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->family,
                        ChangeHosts::family_names, _IndexRange{ 0, 2 } ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "length of address", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->length_of_address ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "address", name_width, _ROOT_WS.equals,
        address.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetAccessControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::SetAccessControl;
    const SetAccessControl::Encoding* encoding {
        reinterpret_cast< const SetAccessControl::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SetAccessControl::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETACCESSCONTROL );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->mode, SetAccessControl::mode_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetCloseDownMode >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::SetCloseDownMode;
    const SetCloseDownMode::Encoding* encoding {
        reinterpret_cast< const SetCloseDownMode::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SetCloseDownMode::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETCLOSEDOWNMODE );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->mode, SetCloseDownMode::mode_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::KillClient >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::KillClient;
    const KillClient::Encoding* encoding {
        reinterpret_cast< const KillClient::Encoding* >( data ) };
    request.bytes_parsed += sizeof( KillClient::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::KILLCLIENT );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "resource", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->resource, KillClient::resource_names ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::RotateProperties >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::RotateProperties;
    const RotateProperties::Encoding* encoding {
        reinterpret_cast< const RotateProperties::Encoding* >( data ) };
    request.bytes_parsed += sizeof( RotateProperties::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::ROTATEPROPERTIES );

    _ParsingOutputs properties {
        _parseLISTof< protocol::ATOM >(
            data + request.bytes_parsed, sz - request.bytes_parsed,
            encoding->number_of_properties,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( properties.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "number of properties" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "window", name_width, _ROOT_WS.equals,
        _formatProtocolType( encoding->window ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "number of properties", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->number_of_properties ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "delta", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->delta ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "properties", name_width, _ROOT_WS.equals,
        properties.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::ForceScreenSaver >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::ForceScreenSaver;
    const ForceScreenSaver::Encoding* encoding {
        reinterpret_cast< const ForceScreenSaver::Encoding* >( data ) };
    request.bytes_parsed += sizeof( ForceScreenSaver::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FORCESCREENSAVER );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "mode", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->mode, ForceScreenSaver::mode_names ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetPointerMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::SetPointerMapping;
    const SetPointerMapping::Encoding* encoding {
        reinterpret_cast< const SetPointerMapping::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SetPointerMapping::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETPOINTERMAPPING );

    _ParsingOutputs map {
        _parseLISTof< protocol::CARD8 >(
            data + request.bytes_parsed, sz - request.bytes_parsed,
            encoding->length_of_map,
            _ROOT_WS.nested( _Whitespace::SINGLELINE ) ) };
    request.bytes_parsed += _pad( map.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "length_of_map", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->length_of_map ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "map", name_width, _ROOT_WS.equals,
        map.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::SetModifierMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::SetModifierMapping;
    const SetModifierMapping::Encoding* encoding {
        reinterpret_cast< const SetModifierMapping::Encoding* >( data ) };
    request.bytes_parsed += sizeof( SetModifierMapping::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETMODIFIERMAPPING );

    // TBD "The number of keycodes in the list must be 8*keycodes-per-modifier
    //   (or a Length error results)."
    // TBD may need constant to avoid magic value 8
    _ParsingOutputs keycodes {
        _parseLISTof< protocol::KEYCODE >(
            data + request.bytes_parsed, sz - request.bytes_parsed,
            8 * encoding->keycodes_per_modifier,
            _ROOT_WS.nested() ) };
    request.bytes_parsed += _pad( keycodes.bytes_parsed );
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "keycodes-per-modifier" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "keycodes-per-modifier", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->keycodes_per_modifier ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "keycodes", name_width, _ROOT_WS.equals,
        keycodes.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

template <>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    protocol::requests::NoOperation >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    _ParsingOutputs request {};
    using protocol::requests::NoOperation;
    const NoOperation::Encoding* encoding {
        reinterpret_cast< const NoOperation::Encoding* >( data ) };
    request.bytes_parsed += sizeof( NoOperation::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::NOOPERATION );
    // protocol specifies that no-op may be followed by variable length dummy data
    request.bytes_parsed += ( encoding->request_length - 1 ) * _ALIGN;
    assert( encoding->request_length == _alignedUnits( request.bytes_parsed ) );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    request.str = fmt::format(
        "{{{}"
        "{}{}"
        "{}}}",
        _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "opcode", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->opcode ), _ROOT_WS.separator ) : "",
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "request length", name_width, _ROOT_WS.equals,
            _formatInteger( encoding->request_length ), _ROOT_WS.separator ) : "",
        _ROOT_WS.encl_indent
        );
    // assert( request.bytes_parsed == sz );
    return request;
}

size_t
X11ProtocolParser::_logRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= 4 ); // TBD

    const uint8_t opcode { *data };
    // TBD core opcodes only to start
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
        break;
    };
    fmt::println( settings.log_fs,
                  "C{:03d}:{:04d}B:{}:S{:05d}: Request {}({}): {}",
                  conn->id, request.bytes_parsed, _CLIENT_TO_SERVER, sequence,
                  protocol::requests::names[ opcode ], opcode, request.str );
    return request.bytes_parsed;
}
