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


size_t X11ProtocolParser::_logSimpleRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::impl::SimpleRequest;
    const SimpleRequest::Encoding* encoding {
        reinterpret_cast< const SimpleRequest::Encoding* >( data ) };
    bytes_parsed += sizeof( SimpleRequest::Encoding );
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
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logSimpleWindowRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::impl::SimpleWindowRequest;
    const SimpleWindowRequest::Encoding* encoding {
        reinterpret_cast< const SimpleWindowRequest::Encoding* >( data ) };
    bytes_parsed += sizeof( SimpleWindowRequest::Encoding );
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
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    return bytes_parsed;
}

size_t X11ProtocolParser::_logListFontsRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ListFonts;
    const ListFonts::Encoding* encoding {
        reinterpret_cast< const ListFonts::Encoding* >( data ) };
    bytes_parsed += sizeof( ListFonts::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::LISTFONTS ||
            encoding->opcode == protocol::requests::opcodes::LISTFONTSWITHINFO );

    std::string_view pattern {
        reinterpret_cast< const char* >( data + bytes_parsed ), encoding->n };
    bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::CreateWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::CreateWindow;
    const CreateWindow::Encoding* encoding {
        reinterpret_cast< const CreateWindow::Encoding* >( data ) };
    bytes_parsed += sizeof( CreateWindow::Encoding );
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
        value_list_inputs, data + bytes_parsed, &value_list_outputs );
    bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ChangeWindowAttributes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ChangeWindowAttributes;
    const ChangeWindowAttributes::Encoding* encoding {
        reinterpret_cast< const ChangeWindowAttributes::Encoding* >( data ) };
    bytes_parsed += sizeof( ChangeWindowAttributes::Encoding );
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
    _parseLISTofVALUE( value_list_inputs, data + bytes_parsed, &value_list_outputs );
    bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ChangeSaveSet >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ChangeSaveSet;
    const ChangeSaveSet::Encoding* encoding {
        reinterpret_cast< const ChangeSaveSet::Encoding* >( data ) };
    bytes_parsed += sizeof( ChangeSaveSet::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGESAVESET );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ReparentWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ReparentWindow;
    const ReparentWindow::Encoding* encoding {
        reinterpret_cast< const ReparentWindow::Encoding* >( data ) };
    bytes_parsed += sizeof( ReparentWindow::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::REPARENTWINDOW );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ConfigureWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ConfigureWindow;
    const ConfigureWindow::Encoding* encoding {
        reinterpret_cast< const ConfigureWindow::Encoding* >( data ) };
    bytes_parsed += sizeof( ConfigureWindow::Encoding );
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
    _parseLISTofVALUE( value_list_inputs, data + bytes_parsed, &value_list_outputs );
    bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::CirculateWindow >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::CirculateWindow;
    const CirculateWindow::Encoding* encoding {
        reinterpret_cast< const CirculateWindow::Encoding* >( data ) };
    bytes_parsed += sizeof( CirculateWindow::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CIRCULATEWINDOW );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::GetGeometry >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::GetGeometry;
    const GetGeometry::Encoding* encoding {
        reinterpret_cast< const GetGeometry::Encoding* >( data ) };
    bytes_parsed += sizeof( GetGeometry::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETGEOMETRY );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::InternAtom >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::InternAtom;
    const InternAtom::Encoding* encoding {
        reinterpret_cast< const InternAtom::Encoding* >( data ) };
    bytes_parsed += sizeof( InternAtom::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::INTERNATOM );
    assert( encoding->n > 0 );
    // followed by pad(n) STRING8 name
    std::string_view name {
        reinterpret_cast< const char* >( data + bytes_parsed ), encoding->n };
    bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == bytes_parsed / _ALIGN );
    _stashAtom( { conn->id, conn->sequence }, name );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::GetAtomName >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::GetAtomName;
    const GetAtomName::Encoding* encoding {
        reinterpret_cast< const GetAtomName::Encoding* >( data ) };
    bytes_parsed += sizeof( GetAtomName::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETATOMNAME );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ChangeProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ChangeProperty;
    const ChangeProperty::Encoding* encoding {
        reinterpret_cast< const ChangeProperty::Encoding* >( data ) };
    bytes_parsed += sizeof( ChangeProperty::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEPROPERTY );
    assert( encoding->format <= 32 && encoding->format % 8 == 0 );
    const uint32_t data_sz { encoding->fmt_unit_ct * ( encoding->format / 8 ) };
    // TBD can we modify printing based on type, or at least print as string when "STRING"?
    const _ParsingOutputs data_ {
        _parseLISTof< protocol::BYTE >(
            data + bytes_parsed, sz - bytes_parsed, data_sz,
            _ROOT_WS.nested( _Whitespace::SINGLELINE ) ) };
    bytes_parsed += _pad(data_.bytes_parsed);
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );

    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::DeleteProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::DeleteProperty;
    const DeleteProperty::Encoding* encoding {
        reinterpret_cast< const DeleteProperty::Encoding* >( data ) };
    bytes_parsed += sizeof( DeleteProperty::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::DELETEPROPERTY );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::GetProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::GetProperty;
    const GetProperty::Encoding* encoding {
        reinterpret_cast< const GetProperty::Encoding* >( data ) };
    bytes_parsed += sizeof( GetProperty::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETPROPERTY );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
        _formatProtocolType( encoding->type, GetProperty::request_type_names ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "long-offset", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->long_offset ), _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "long-length", name_width, _ROOT_WS.equals,
        _formatInteger( encoding->long_length ), _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::SetSelectionOwner >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::SetSelectionOwner;
    const SetSelectionOwner::Encoding* encoding {
        reinterpret_cast< const SetSelectionOwner::Encoding* >( data ) };
    bytes_parsed += sizeof( SetSelectionOwner::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETSELECTIONOWNER );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::GetSelectionOwner >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::GetSelectionOwner;
    const GetSelectionOwner::Encoding* encoding {
        reinterpret_cast< const GetSelectionOwner::Encoding* >( data ) };
    bytes_parsed += sizeof( GetSelectionOwner::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETSELECTIONOWNER );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ConvertSelection >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ConvertSelection;
    const ConvertSelection::Encoding* encoding {
        reinterpret_cast< const ConvertSelection::Encoding* >( data ) };
    bytes_parsed += sizeof( ConvertSelection::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CONVERTSELECTION );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::SendEvent >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::SendEvent;
    const SendEvent::Encoding* encoding {
        reinterpret_cast< const SendEvent::Encoding* >( data ) };
    bytes_parsed += sizeof( SendEvent::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SENDEVENT );
    // TBD parse event ahead of time, can get struct type from first byte
    // maybe events will be unlike reuests, errors, etc in that there is
    //   both a log func and a format func?
    _ParsingOutputs event { _parseEvent(
            conn, data + bytes_parsed, protocol::events::ENCODING_SZ,
            _ROOT_WS.nested() ) };
    bytes_parsed += event.bytes_parsed;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
        event.str, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::GrabPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::GrabPointer;
    const GrabPointer::Encoding* encoding {
        reinterpret_cast< const GrabPointer::Encoding* >( data ) };
    bytes_parsed += sizeof( GrabPointer::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GRABPOINTER );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::UngrabPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::UngrabPointer;
    const UngrabPointer::Encoding* encoding {
        reinterpret_cast< const UngrabPointer::Encoding* >( data ) };
    bytes_parsed += sizeof( UngrabPointer::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::UNGRABPOINTER );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::GrabButton >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::GrabButton;
    const GrabButton::Encoding* encoding {
        reinterpret_cast< const GrabButton::Encoding* >( data ) };
    bytes_parsed += sizeof( GrabButton::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GRABBUTTON );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::UngrabButton >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::UngrabButton;
    const UngrabButton::Encoding* encoding {
        reinterpret_cast< const UngrabButton::Encoding* >( data ) };
    bytes_parsed += sizeof( UngrabButton::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::UNGRABBUTTON );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ChangeActivePointerGrab >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ChangeActivePointerGrab;
    const ChangeActivePointerGrab::Encoding* encoding {
        reinterpret_cast< const ChangeActivePointerGrab::Encoding* >( data ) };
    bytes_parsed += sizeof( ChangeActivePointerGrab::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEACTIVEPOINTERGRAB );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::GrabKeyboard >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::GrabKeyboard;
    const GrabKeyboard::Encoding* encoding {
        reinterpret_cast< const GrabKeyboard::Encoding* >( data ) };
    bytes_parsed += sizeof( GrabKeyboard::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GRABKEYBOARD );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::UngrabKeyboard >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::UngrabKeyboard;
    const UngrabKeyboard::Encoding* encoding {
        reinterpret_cast< const UngrabKeyboard::Encoding* >( data ) };
    bytes_parsed += sizeof( UngrabKeyboard::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::UNGRABKEYBOARD );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::GrabKey >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::GrabKey;
    const GrabKey::Encoding* encoding {
        reinterpret_cast< const GrabKey::Encoding* >( data ) };
    bytes_parsed += sizeof( GrabKey::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GRABKEY );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::UngrabKey >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::UngrabKey;
    const UngrabKey::Encoding* encoding {
        reinterpret_cast< const UngrabKey::Encoding* >( data ) };
    bytes_parsed += sizeof( UngrabKey::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::UNGRABKEY );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::AllowEvents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::AllowEvents;
    const AllowEvents::Encoding* encoding {
        reinterpret_cast< const AllowEvents::Encoding* >( data ) };
    bytes_parsed += sizeof( AllowEvents::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::ALLOWEVENTS );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::GetMotionEvents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::GetMotionEvents;
    const GetMotionEvents::Encoding* encoding {
        reinterpret_cast< const GetMotionEvents::Encoding* >( data ) };
    bytes_parsed += sizeof( GetMotionEvents::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETMOTIONEVENTS );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::TranslateCoordinates >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::TranslateCoordinates;
    const TranslateCoordinates::Encoding* encoding {
        reinterpret_cast< const TranslateCoordinates::Encoding* >( data ) };
    bytes_parsed += sizeof( TranslateCoordinates::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::TRANSLATECOORDINATES );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::WarpPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::WarpPointer;
    const WarpPointer::Encoding* encoding {
        reinterpret_cast< const WarpPointer::Encoding* >( data ) };
    bytes_parsed += sizeof( WarpPointer::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::WARPPOINTER );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::SetInputFocus >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::SetInputFocus;
    const SetInputFocus::Encoding* encoding {
        reinterpret_cast< const SetInputFocus::Encoding* >( data ) };
    bytes_parsed += sizeof( SetInputFocus::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETINPUTFOCUS );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::OpenFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::OpenFont;
    const OpenFont::Encoding* encoding {
        reinterpret_cast< const OpenFont::Encoding* >( data ) };
    bytes_parsed += sizeof( OpenFont::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::OPENFONT );

    std::string_view name {
        reinterpret_cast< const char* >( data + bytes_parsed ), encoding->n };
    bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::CloseFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::CloseFont;
    const CloseFont::Encoding* encoding {
        reinterpret_cast< const CloseFont::Encoding* >( data ) };
    bytes_parsed += sizeof( CloseFont::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CLOSEFONT );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::QueryFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::QueryFont;
    const QueryFont::Encoding* encoding {
        reinterpret_cast< const QueryFont::Encoding* >( data ) };
    bytes_parsed += sizeof( QueryFont::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::QUERYFONT );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::QueryTextExtents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::QueryTextExtents;
    const QueryTextExtents::Encoding* encoding {
        reinterpret_cast< const QueryTextExtents::Encoding* >( data ) };
    bytes_parsed += sizeof( QueryTextExtents::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::QUERYTEXTEXTENTS );

    const auto padded_string_sz {
        ( encoding->request_length * _ALIGN ) - sizeof( QueryTextExtents::Encoding ) };
    std::u16string_view u16string {
        reinterpret_cast<const char16_t*>( data + bytes_parsed ),
        ( padded_string_sz / sizeof( char16_t ) ) - ( encoding->odd_length.data ? 1 : 0 ) };
    // printing char16_t vals as hex, same as xtrace
    // TBD is there a way to convert to printable UTF-8 (with c16rtomb, for example)?
    // TBD STRING16 encoding seems quite tangled, from standard:
    //     "The primary interpretation of large characters in a STRING16 is that they are
    // composed of two bytes used to index a two-dimensional matrix, hence, the use of
    // CHAR2B rather than CARD16. This corresponds to the JIS/ISO method of indexing
    // 2-byte characters. It is expected that most large fonts will be defined with 2-byte
    // matrix indexing. For large fonts constructed with linear indexing, a CHAR2B can
    // be interpreted as a 16-bit number by treating byte1 as the most significant byte.
    // This means that clients should always transmit such 16-bit character values most
    // significant byte first, as the server will never byte-swap CHAR2B quantities."
    std::string string_as_hex;
    // fmt counts "0x" as part of width when using '#'
    const size_t c16_hex_width { ( sizeof( char16_t ) * 2 ) + 2 };
    for ( const char16_t c16 : u16string ) {
        string_as_hex += fmt::format(
            "{}{:#0{}x}", string_as_hex.empty() ? "" : " ",
            uint16_t( c16 ), c16_hex_width );
    }
    bytes_parsed += padded_string_sz;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
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
        string_as_hex, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::SetFontPath >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::SetFontPath;
    const SetFontPath::Encoding* encoding {
        reinterpret_cast< const SetFontPath::Encoding* >( data ) };
    bytes_parsed += sizeof( SetFontPath::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETFONTPATH );

    _ParsingOutputs path {
        _parseLISTof< protocol::STR >(
            data + bytes_parsed, sz - bytes_parsed, encoding->str_ct,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( path.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::CreatePixmap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::CreatePixmap;
    const CreatePixmap::Encoding* encoding {
        reinterpret_cast< const CreatePixmap::Encoding* >( data ) };
    bytes_parsed += sizeof( CreatePixmap::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CREATEPIXMAP );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::FreePixmap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::FreePixmap;
    const FreePixmap::Encoding* encoding {
        reinterpret_cast< const FreePixmap::Encoding* >( data ) };
    bytes_parsed += sizeof( FreePixmap::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FREEPIXMAP );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::CreateGC >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::CreateGC;
    const CreateGC::Encoding* encoding {
        reinterpret_cast< const CreateGC::Encoding* >( data ) };
    bytes_parsed += sizeof( CreateGC::Encoding );
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
    _parseLISTofVALUE( value_list_inputs, data + bytes_parsed, &value_list_outputs );
    bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ChangeGC >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ChangeGC;
    const ChangeGC::Encoding* encoding {
        reinterpret_cast< const ChangeGC::Encoding* >( data ) };
    bytes_parsed += sizeof( ChangeGC::Encoding );
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
    _parseLISTofVALUE( value_list_inputs, data + bytes_parsed, &value_list_outputs );
    bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::CopyGC >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::CopyGC;
    const CopyGC::Encoding* encoding {
        reinterpret_cast< const CopyGC::Encoding* >( data ) };
    bytes_parsed += sizeof( CopyGC::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::COPYGC );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::SetDashes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::SetDashes;
    const SetDashes::Encoding* encoding {
        reinterpret_cast< const SetDashes::Encoding* >( data ) };
    bytes_parsed += sizeof( SetDashes::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETDASHES );

    _ParsingOutputs dashes {
        _parseLISTof< protocol::CARD8 >(
            data + bytes_parsed, sz - bytes_parsed, encoding->n,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( dashes.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::SetClipRectangles >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::SetClipRectangles;
    const SetClipRectangles::Encoding* encoding {
        reinterpret_cast< const SetClipRectangles::Encoding* >( data ) };
    bytes_parsed += sizeof( SetClipRectangles::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETCLIPRECTANGLES );

    const size_t n_rectangles {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( SetClipRectangles::Encoding ) ) /
        sizeof( protocol::RECTANGLE ) };
    _ParsingOutputs rectangles {
        _parseLISTof< protocol::RECTANGLE >(
            data + bytes_parsed, sz - bytes_parsed, n_rectangles,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( rectangles.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::FreeGC >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::FreeGC;
    const FreeGC::Encoding* encoding {
        reinterpret_cast< const FreeGC::Encoding* >( data ) };
    bytes_parsed += sizeof( FreeGC::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FREEGC );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ClearArea >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ClearArea;
    const ClearArea::Encoding* encoding {
        reinterpret_cast< const ClearArea::Encoding* >( data ) };
    bytes_parsed += sizeof( ClearArea::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CLEARAREA );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::CopyArea >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::CopyArea;
    const CopyArea::Encoding* encoding {
        reinterpret_cast< const CopyArea::Encoding* >( data ) };
    bytes_parsed += sizeof( CopyArea::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::COPYAREA );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::CopyPlane >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::CopyPlane;
    const CopyPlane::Encoding* encoding {
        reinterpret_cast< const CopyPlane::Encoding* >( data ) };
    bytes_parsed += sizeof( CopyPlane::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::COPYPLANE );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::PolyPoint >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::PolyPoint;
    const PolyPoint::Encoding* encoding {
        reinterpret_cast< const PolyPoint::Encoding* >( data ) };
    bytes_parsed += sizeof( PolyPoint::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYPOINT );

    const size_t n_points {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolyPoint::Encoding ) ) / sizeof( protocol::POINT ) };
    _ParsingOutputs points {
        _parseLISTof< protocol::POINT >(
            data + bytes_parsed, sz - bytes_parsed, n_points,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( points.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "coordinate-mode" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

// TBD same encoding as PolyPoint; generalize?
template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::PolyLine >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::PolyLine;
    const PolyLine::Encoding* encoding {
        reinterpret_cast< const PolyLine::Encoding* >( data ) };
    bytes_parsed += sizeof( PolyLine::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYLINE );

    const size_t n_points {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolyLine::Encoding ) ) / sizeof( protocol::POINT ) };
    _ParsingOutputs points {
        _parseLISTof< protocol::POINT >(
            data + bytes_parsed, sz - bytes_parsed, n_points,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( points.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "coordinate-mode" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::PolySegment >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::PolySegment;
    const PolySegment::Encoding* encoding {
        reinterpret_cast< const PolySegment::Encoding* >( data ) };
    bytes_parsed += sizeof( PolySegment::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYSEGMENT );

    const size_t n_segments {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolySegment::Encoding ) ) / sizeof( PolySegment::SEGMENT ) };
    _ParsingOutputs segments {
        _parseLISTof< PolySegment::SEGMENT >(
            data + bytes_parsed, sz - bytes_parsed, n_segments,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( segments.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::PolyRectangle >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::PolyRectangle;
    const PolyRectangle::Encoding* encoding {
        reinterpret_cast< const PolyRectangle::Encoding* >( data ) };
    bytes_parsed += sizeof( PolyRectangle::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYRECTANGLE );

    const size_t n_rectangles {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolyRectangle::Encoding ) ) / sizeof( protocol::RECTANGLE ) };
    _ParsingOutputs rectangles {
        _parseLISTof< protocol::RECTANGLE >(
            data + bytes_parsed, sz - bytes_parsed, n_rectangles,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( rectangles.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::PolyArc >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::PolyArc;
    const PolyArc::Encoding* encoding {
        reinterpret_cast< const PolyArc::Encoding* >( data ) };
    bytes_parsed += sizeof( PolyArc::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYARC );

    const size_t n_arcs {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolyArc::Encoding ) ) / sizeof( protocol::ARC ) };
    _ParsingOutputs arcs {
        _parseLISTof< protocol::ARC >(
            data + bytes_parsed, sz - bytes_parsed, n_arcs,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( arcs.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::FillPoly >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::FillPoly;
    const FillPoly::Encoding* encoding {
        reinterpret_cast< const FillPoly::Encoding* >( data ) };
    bytes_parsed += sizeof( FillPoly::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FILLPOLY );

    const size_t n_points {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( FillPoly::Encoding ) ) / sizeof( protocol::POINT ) };
    _ParsingOutputs points {
        _parseLISTof< protocol::POINT >(
            data + bytes_parsed, sz - bytes_parsed, n_points,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( points.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "coordinate-mode" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::PolyFillRectangle >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::PolyFillRectangle;
    const PolyFillRectangle::Encoding* encoding {
        reinterpret_cast< const PolyFillRectangle::Encoding* >( data ) };
    bytes_parsed += sizeof( PolyFillRectangle::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYFILLRECTANGLE );

    const size_t n_rectangles {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolyFillRectangle::Encoding ) ) / sizeof( protocol::RECTANGLE ) };
    _ParsingOutputs rectangles {
        _parseLISTof< protocol::RECTANGLE >(
            data + bytes_parsed, sz - bytes_parsed, n_rectangles,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( rectangles.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::PolyFillArc >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::PolyFillArc;
    const PolyFillArc::Encoding* encoding {
        reinterpret_cast< const PolyFillArc::Encoding* >( data ) };
    bytes_parsed += sizeof( PolyFillArc::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYFILLARC );

    const size_t n_arcs {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( PolyFillArc::Encoding ) ) / sizeof( protocol::ARC ) };
    _ParsingOutputs arcs {
        _parseLISTof< protocol::ARC >(
            data + bytes_parsed, sz - bytes_parsed, n_arcs,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( arcs.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::PutImage >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::PutImage;
    const PutImage::Encoding* encoding {
        reinterpret_cast< const PutImage::Encoding* >( data ) };
    bytes_parsed += sizeof( PutImage::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::PUTIMAGE );

    const size_t image_data_sz {
        ( encoding->request_length * _ALIGN ) - sizeof( PutImage::Encoding ) };
    // TBD not parsing iamge data as it is not done in xtrace
    bytes_parsed += image_data_sz;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::GetImage >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::GetImage;
    const GetImage::Encoding* encoding {
        reinterpret_cast< const GetImage::Encoding* >( data ) };
    bytes_parsed += sizeof( GetImage::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETIMAGE );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::PolyText8 >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::PolyText8;
    const PolyText8::Encoding* encoding {
        reinterpret_cast< const PolyText8::Encoding* >( data ) };
    bytes_parsed += sizeof( PolyText8::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYTEXT8 );

    const size_t text_item_list_sz {
        ( encoding->request_length * _ALIGN ) - sizeof( PolyText8::Encoding ) };
    _ParsingOutputs text_items {
        _parseLISTof< PolyText8::TEXTITEM8 >(
            data + bytes_parsed, text_item_list_sz,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( text_items.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::PolyText16 >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::PolyText16;
    const PolyText16::Encoding* encoding {
        reinterpret_cast< const PolyText16::Encoding* >( data ) };
    bytes_parsed += sizeof( PolyText16::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::POLYTEXT16 );

    const size_t text_item_list_sz {
        ( encoding->request_length * _ALIGN ) - sizeof( PolyText16::Encoding ) };
    _ParsingOutputs text_items {
        _parseLISTof< PolyText16::TEXTITEM16 >(
            data + bytes_parsed, text_item_list_sz,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( text_items.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ImageText8 >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ImageText8;
    const ImageText8::Encoding* encoding {
        reinterpret_cast< const ImageText8::Encoding* >( data ) };
    bytes_parsed += sizeof( ImageText8::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::IMAGETEXT8 );

    std::string_view string {
        reinterpret_cast< const char* >( data + bytes_parsed ), encoding->n };
    bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ImageText16 >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ImageText16;
    const ImageText16::Encoding* encoding {
        reinterpret_cast< const ImageText16::Encoding* >( data ) };
    bytes_parsed += sizeof( ImageText16::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::IMAGETEXT16 );

    std::u16string_view u16string {
        reinterpret_cast<const char16_t*>( data + bytes_parsed ), encoding->n };
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
    bytes_parsed += _pad( encoding->n * sizeof( protocol::CHAR2B ) );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::CreateColormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::CreateColormap;
    const CreateColormap::Encoding* encoding {
        reinterpret_cast< const CreateColormap::Encoding* >( data ) };
    bytes_parsed += sizeof( CreateColormap::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CREATECOLORMAP );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::FreeColormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::FreeColormap;
    const FreeColormap::Encoding* encoding {
        reinterpret_cast< const FreeColormap::Encoding* >( data ) };
    bytes_parsed += sizeof( FreeColormap::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FREECOLORMAP );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::CopyColormapAndFree >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::CopyColormapAndFree;
    const CopyColormapAndFree::Encoding* encoding {
        reinterpret_cast< const CopyColormapAndFree::Encoding* >( data ) };
    bytes_parsed += sizeof( CopyColormapAndFree::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::COPYCOLORMAPANDFREE );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

// TBD generalize shared encoding for InstallColomap/UninstallColomap
template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::InstallColormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::InstallColormap;
    const InstallColormap::Encoding* encoding {
        reinterpret_cast< const InstallColormap::Encoding* >( data ) };
    bytes_parsed += sizeof( InstallColormap::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::INSTALLCOLORMAP );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::UninstallColormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::UninstallColormap;
    const UninstallColormap::Encoding* encoding {
        reinterpret_cast< const UninstallColormap::Encoding* >( data ) };
    bytes_parsed += sizeof( UninstallColormap::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::UNINSTALLCOLORMAP );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::AllocColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::AllocColor;
    const AllocColor::Encoding* encoding {
        reinterpret_cast< const AllocColor::Encoding* >( data ) };
    bytes_parsed += sizeof( AllocColor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::ALLOCCOLOR );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::AllocNamedColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::AllocNamedColor;
    const AllocNamedColor::Encoding* encoding {
        reinterpret_cast< const AllocNamedColor::Encoding* >( data ) };
    bytes_parsed += sizeof( AllocNamedColor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::ALLOCNAMEDCOLOR );

    std::string_view name {
        reinterpret_cast< const char* >( data + bytes_parsed ), encoding->n };
    bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::AllocColorCells >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::AllocColorCells;
    const AllocColorCells::Encoding* encoding {
        reinterpret_cast< const AllocColorCells::Encoding* >( data ) };
    bytes_parsed += sizeof( AllocColorCells::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::ALLOCCOLORCELLS );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::AllocColorPlanes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::AllocColorPlanes;
    const AllocColorPlanes::Encoding* encoding {
        reinterpret_cast< const AllocColorPlanes::Encoding* >( data ) };
    bytes_parsed += sizeof( AllocColorPlanes::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::ALLOCCOLORPLANES );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::FreeColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::FreeColors;
    const FreeColors::Encoding* encoding {
        reinterpret_cast< const FreeColors::Encoding* >( data ) };
    bytes_parsed += sizeof( FreeColors::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FREECOLORS );

    const size_t n_pixels {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( FreeColors::Encoding ) ) / sizeof( protocol::CARD32 ) };
    _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + bytes_parsed, sz - bytes_parsed, n_pixels,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( pixels.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::StoreColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::StoreColors;
    const StoreColors::Encoding* encoding {
        reinterpret_cast< const StoreColors::Encoding* >( data ) };
    bytes_parsed += sizeof( StoreColors::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::STORECOLORS );

    const size_t n_items {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( StoreColors::Encoding ) ) / sizeof( StoreColors::COLORITEM ) };
    _ParsingOutputs items {
        _parseLISTof< StoreColors::COLORITEM >(
            data + bytes_parsed, sz - bytes_parsed, n_items,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( items.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::StoreNamedColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::StoreNamedColor;
    const StoreNamedColor::Encoding* encoding {
        reinterpret_cast< const StoreNamedColor::Encoding* >( data ) };
    bytes_parsed += sizeof( StoreNamedColor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::STORENAMEDCOLOR );

    std::string_view name {
        reinterpret_cast< const char* >( data + bytes_parsed ), encoding->n };
    bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::QueryColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::QueryColors;
    const QueryColors::Encoding* encoding {
        reinterpret_cast< const QueryColors::Encoding* >( data ) };
    bytes_parsed += sizeof( QueryColors::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::QUERYCOLORS );

    const size_t n_pixels {
        ( ( encoding->request_length * _ALIGN ) -
          sizeof( QueryColors::Encoding ) ) / sizeof( protocol::CARD32 ) };
    _ParsingOutputs pixels {
        _parseLISTof< protocol::CARD32 >(
            data + bytes_parsed, sz - bytes_parsed, n_pixels,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( pixels.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::LookupColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::LookupColor;
    const LookupColor::Encoding* encoding {
        reinterpret_cast< const LookupColor::Encoding* >( data ) };
    bytes_parsed += sizeof( LookupColor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::LOOKUPCOLOR );

    std::string_view name {
        reinterpret_cast< const char* >( data + bytes_parsed ), encoding->n };
    bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::CreateCursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::CreateCursor;
    const CreateCursor::Encoding* encoding {
        reinterpret_cast< const CreateCursor::Encoding* >( data ) };
    bytes_parsed += sizeof( CreateCursor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CREATECURSOR );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::CreateGlyphCursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::CreateGlyphCursor;
    const CreateGlyphCursor::Encoding* encoding {
        reinterpret_cast< const CreateGlyphCursor::Encoding* >( data ) };
    bytes_parsed += sizeof( CreateGlyphCursor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CREATEGLYPHCURSOR );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::FreeCursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::FreeCursor;
    const FreeCursor::Encoding* encoding {
        reinterpret_cast< const FreeCursor::Encoding* >( data ) };
    bytes_parsed += sizeof( FreeCursor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FREECURSOR );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::RecolorCursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::RecolorCursor;
    const RecolorCursor::Encoding* encoding {
        reinterpret_cast< const RecolorCursor::Encoding* >( data ) };
    bytes_parsed += sizeof( RecolorCursor::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::RECOLORCURSOR );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::QueryBestSize >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::QueryBestSize;
    const QueryBestSize::Encoding* encoding {
        reinterpret_cast< const QueryBestSize::Encoding* >( data ) };
    bytes_parsed += sizeof( QueryBestSize::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::QUERYBESTSIZE );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::QueryExtension >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::QueryExtension;
    const QueryExtension::Encoding* encoding {
        reinterpret_cast< const QueryExtension::Encoding* >( data ) };
    bytes_parsed += sizeof( QueryExtension::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::QUERYEXTENSION );

    std::string_view name {
        reinterpret_cast< const char* >( data + bytes_parsed ), encoding->n };
    bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ChangeKeyboardMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ChangeKeyboardMapping;
    const ChangeKeyboardMapping::Encoding* encoding {
        reinterpret_cast< const ChangeKeyboardMapping::Encoding* >( data ) };
    bytes_parsed += sizeof( ChangeKeyboardMapping::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEKEYBOARDMAPPING );

    _ParsingOutputs keysyms {
        _parseLISTof< protocol::KEYSYM >(
            data + bytes_parsed, sz - bytes_parsed,
            encoding->keycode_count * encoding->keysyms_per_keycode,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( keysyms.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "keysyms-per-keycode" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::GetKeyboardMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::GetKeyboardMapping;
    const GetKeyboardMapping::Encoding* encoding {
        reinterpret_cast< const GetKeyboardMapping::Encoding* >( data ) };
    bytes_parsed += sizeof( GetKeyboardMapping::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::GETKEYBOARDMAPPING );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "keysyms-per-keycode" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ChangeKeyboardControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ChangeKeyboardControl;
    const ChangeKeyboardControl::Encoding* encoding {
        reinterpret_cast< const ChangeKeyboardControl::Encoding* >( data ) };
    bytes_parsed += sizeof( ChangeKeyboardControl::Encoding );
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
    _parseLISTofVALUE( value_list_inputs, data + bytes_parsed, &value_list_outputs );
    bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::Bell >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::Bell;
    const Bell::Encoding* encoding {
        reinterpret_cast< const Bell::Encoding* >( data ) };
    bytes_parsed += sizeof( Bell::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::BELL );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ChangePointerControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ChangePointerControl;
    const ChangePointerControl::Encoding* encoding {
        reinterpret_cast< const ChangePointerControl::Encoding* >( data ) };
    bytes_parsed += sizeof( ChangePointerControl::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEPOINTERCONTROL );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "acceleration-denominator" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::SetScreenSaver >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::SetScreenSaver;
    const SetScreenSaver::Encoding* encoding {
        reinterpret_cast< const SetScreenSaver::Encoding* >( data ) };
    bytes_parsed += sizeof( SetScreenSaver::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETSCREENSAVER );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "prefer-blanking" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ChangeHosts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ChangeHosts;
    const ChangeHosts::Encoding* encoding {
        reinterpret_cast< const ChangeHosts::Encoding* >( data ) };
    bytes_parsed += sizeof( ChangeHosts::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::CHANGEHOSTS );

    // TBD format into IPv4/6 instead of byte array? (depends in family)
    //   (xtrace prints as bytes)
    // TBD in the standard, the full description of ChangeHosts anticipates up
    //   to 5 family values, but in the encoding section, family is limited to
    //   Internet, DECnet, or Chaos
    _ParsingOutputs address {
        _parseLISTof< protocol::CARD8 >(
            data + bytes_parsed, sz - bytes_parsed,
            encoding->length_of_address,
            _ROOT_WS.nested( _Whitespace::SINGLELINE ) ) };
    bytes_parsed += _pad( address.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "length of address" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::SetAccessControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::SetAccessControl;
    const SetAccessControl::Encoding* encoding {
        reinterpret_cast< const SetAccessControl::Encoding* >( data ) };
    bytes_parsed += sizeof( SetAccessControl::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETACCESSCONTROL );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::SetCloseDownMode >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::SetCloseDownMode;
    const SetCloseDownMode::Encoding* encoding {
        reinterpret_cast< const SetCloseDownMode::Encoding* >( data ) };
    bytes_parsed += sizeof( SetCloseDownMode::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETCLOSEDOWNMODE );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::KillClient >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::KillClient;
    const KillClient::Encoding* encoding {
        reinterpret_cast< const KillClient::Encoding* >( data ) };
    bytes_parsed += sizeof( KillClient::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::KILLCLIENT );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::RotateProperties >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::RotateProperties;
    const RotateProperties::Encoding* encoding {
        reinterpret_cast< const RotateProperties::Encoding* >( data ) };
    bytes_parsed += sizeof( RotateProperties::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::ROTATEPROPERTIES );

    _ParsingOutputs properties {
        _parseLISTof< protocol::ATOM >(
            data + bytes_parsed, sz - bytes_parsed,
            encoding->number_of_properties,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( properties.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "number of properties" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::ForceScreenSaver >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::ForceScreenSaver;
    const ForceScreenSaver::Encoding* encoding {
        reinterpret_cast< const ForceScreenSaver::Encoding* >( data ) };
    bytes_parsed += sizeof( ForceScreenSaver::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::FORCESCREENSAVER );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::SetPointerMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::SetPointerMapping;
    const SetPointerMapping::Encoding* encoding {
        reinterpret_cast< const SetPointerMapping::Encoding* >( data ) };
    bytes_parsed += sizeof( SetPointerMapping::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETPOINTERMAPPING );

    _ParsingOutputs map {
        _parseLISTof< protocol::CARD8 >(
            data + bytes_parsed, sz - bytes_parsed,
            encoding->length_of_map,
            _ROOT_WS.nested( _Whitespace::SINGLELINE ) ) };
    bytes_parsed += _pad( map.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::SetModifierMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::SetModifierMapping;
    const SetModifierMapping::Encoding* encoding {
        reinterpret_cast< const SetModifierMapping::Encoding* >( data ) };
    bytes_parsed += sizeof( SetModifierMapping::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::SETMODIFIERMAPPING );

    // TBD "The number of keycodes in the list must be 8*keycodes-per-modifier
    //   (or a Length error results)."
    // TBD may need constant to avoid magic value 8
    _ParsingOutputs keycodes {
        _parseLISTof< protocol::KEYCODE >(
            data + bytes_parsed, sz - bytes_parsed,
            8 * encoding->keycodes_per_modifier,
            _ROOT_WS.nested() ) };
    bytes_parsed += _pad( keycodes.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "keycodes-per-modifier" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

template <>
size_t X11ProtocolParser::_logRequest<
    protocol::requests::NoOperation >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::NoOperation;
    const NoOperation::Encoding* encoding {
        reinterpret_cast< const NoOperation::Encoding* >( data ) };
    bytes_parsed += sizeof( NoOperation::Encoding );
    assert( encoding->opcode == protocol::requests::opcodes::NOOPERATION );
    // protocol specifies that no-op may be followed by variable length dummy data
    bytes_parsed += ( encoding->request_length - 1 ) * _ALIGN;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const uint32_t name_width (
        settings.multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
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
    // assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= 4 ); // TBD

    const uint8_t opcode { *data };
    assert( ( opcode >= 1 && opcode <= 119 ) || opcode == 127 );  // TBD core opcodes only to start
    // map opcode to sequence number to aid in parsing request errors and replies
    conn->registerRequest( opcode );

    size_t bytes_parsed {};
    fmt::print( settings.log_fs, "{:03d}:<:client request {:>3d}: {} ",
                conn->id, opcode, protocol::requests::names[ opcode ] );
    switch ( opcode ) {
    case protocol::requests::opcodes::CREATEWINDOW:
        bytes_parsed = _logRequest<
            protocol::requests::CreateWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEWINDOWATTRIBUTES:
        bytes_parsed = _logRequest<
            protocol::requests::ChangeWindowAttributes >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETWINDOWATTRIBUTES:
        bytes_parsed = _logRequest<
            protocol::requests::GetWindowAttributes >( conn, data, sz );
        break;
    case protocol::requests::opcodes::DESTROYWINDOW:
        bytes_parsed = _logRequest<
            protocol::requests::DestroyWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::DESTROYSUBWINDOWS:
        bytes_parsed = _logRequest<
            protocol::requests::DestroySubwindows >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGESAVESET:
        bytes_parsed = _logRequest<
            protocol::requests::ChangeSaveSet >( conn, data, sz );
        break;
    case protocol::requests::opcodes::REPARENTWINDOW:
        bytes_parsed = _logRequest<
            protocol::requests::ReparentWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::MAPWINDOW:
        bytes_parsed = _logRequest<
            protocol::requests::MapWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::MAPSUBWINDOWS:
        bytes_parsed = _logRequest<
            protocol::requests::MapSubwindows >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNMAPWINDOW:
        bytes_parsed = _logRequest<
            protocol::requests::UnmapWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNMAPSUBWINDOWS:
        bytes_parsed = _logRequest<
            protocol::requests::UnmapSubwindows >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CONFIGUREWINDOW:
        bytes_parsed = _logRequest<
            protocol::requests::ConfigureWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CIRCULATEWINDOW:
        bytes_parsed = _logRequest<
            protocol::requests::CirculateWindow >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETGEOMETRY:
        bytes_parsed = _logRequest<
            protocol::requests::GetGeometry >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYTREE:
        bytes_parsed = _logRequest<
            protocol::requests::QueryTree >( conn, data, sz );
        break;
    case protocol::requests::opcodes::INTERNATOM:
        bytes_parsed = _logRequest<
            protocol::requests::InternAtom >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETATOMNAME:
        bytes_parsed = _logRequest<
            protocol::requests::GetAtomName >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEPROPERTY:
        bytes_parsed = _logRequest<
            protocol::requests::ChangeProperty >( conn, data, sz );
        break;
    case protocol::requests::opcodes::DELETEPROPERTY:
        bytes_parsed = _logRequest<
            protocol::requests::DeleteProperty >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPROPERTY:
        bytes_parsed = _logRequest<
            protocol::requests::GetProperty >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTPROPERTIES:
        bytes_parsed = _logRequest<
            protocol::requests::ListProperties >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETSELECTIONOWNER:
        bytes_parsed = _logRequest<
            protocol::requests::SetSelectionOwner >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETSELECTIONOWNER:
        bytes_parsed = _logRequest<
            protocol::requests::GetSelectionOwner >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CONVERTSELECTION:
        bytes_parsed = _logRequest<
            protocol::requests::ConvertSelection >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SENDEVENT:
        bytes_parsed = _logRequest<
            protocol::requests::SendEvent >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABPOINTER:
        bytes_parsed = _logRequest<
            protocol::requests::GrabPointer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABPOINTER:
        bytes_parsed = _logRequest<
            protocol::requests::UngrabPointer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABBUTTON:
        bytes_parsed = _logRequest<
            protocol::requests::GrabButton >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABBUTTON:
        bytes_parsed = _logRequest<
            protocol::requests::UngrabButton >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEACTIVEPOINTERGRAB:
        bytes_parsed = _logRequest<
            protocol::requests::ChangeActivePointerGrab >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABKEYBOARD:
        bytes_parsed = _logRequest<
            protocol::requests::GrabKeyboard >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABKEYBOARD:
        bytes_parsed = _logRequest<
            protocol::requests::UngrabKeyboard >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABKEY:
        bytes_parsed = _logRequest<
            protocol::requests::GrabKey >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABKEY:
        bytes_parsed = _logRequest<
            protocol::requests::UngrabKey >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOWEVENTS:
        bytes_parsed = _logRequest<
            protocol::requests::AllowEvents >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABSERVER:
        bytes_parsed = _logRequest<
            protocol::requests::GrabServer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABSERVER:
        bytes_parsed = _logRequest<
            protocol::requests::UngrabServer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYPOINTER:
        bytes_parsed = _logRequest<
            protocol::requests::QueryPointer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETMOTIONEVENTS:
        bytes_parsed = _logRequest<
            protocol::requests::GetMotionEvents >( conn, data, sz );
        break;
    case protocol::requests::opcodes::TRANSLATECOORDINATES:
        bytes_parsed = _logRequest<
            protocol::requests::TranslateCoordinates >( conn, data, sz );
        break;
    case protocol::requests::opcodes::WARPPOINTER:
        bytes_parsed = _logRequest<
            protocol::requests::WarpPointer >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETINPUTFOCUS:
        bytes_parsed = _logRequest<
            protocol::requests::SetInputFocus >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETINPUTFOCUS:
        bytes_parsed = _logRequest<
            protocol::requests::GetInputFocus >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYKEYMAP:
        bytes_parsed = _logRequest<
            protocol::requests::QueryKeymap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::OPENFONT:
        bytes_parsed = _logRequest<
            protocol::requests::OpenFont >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CLOSEFONT:
        bytes_parsed = _logRequest<
            protocol::requests::CloseFont >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYFONT:
        bytes_parsed = _logRequest<
            protocol::requests::QueryFont >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYTEXTEXTENTS:
        bytes_parsed = _logRequest<
            protocol::requests::QueryTextExtents >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTFONTS:
        bytes_parsed = _logRequest<
            protocol::requests::ListFonts >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTFONTSWITHINFO:
        bytes_parsed = _logRequest<
            protocol::requests::ListFontsWithInfo >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETFONTPATH:
        bytes_parsed = _logRequest<
            protocol::requests::SetFontPath >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETFONTPATH:
        bytes_parsed = _logRequest<
            protocol::requests::GetFontPath >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATEPIXMAP:
        bytes_parsed = _logRequest<
            protocol::requests::CreatePixmap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREEPIXMAP:
        bytes_parsed = _logRequest<
            protocol::requests::FreePixmap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATEGC:
        bytes_parsed = _logRequest<
            protocol::requests::CreateGC >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEGC:
        bytes_parsed = _logRequest<
            protocol::requests::ChangeGC >( conn, data, sz );
        break;
    case protocol::requests::opcodes::COPYGC:
        bytes_parsed = _logRequest<
            protocol::requests::CopyGC >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETDASHES:
        bytes_parsed = _logRequest<
            protocol::requests::SetDashes >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETCLIPRECTANGLES:
        bytes_parsed = _logRequest<
            protocol::requests::SetClipRectangles >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREEGC:
        bytes_parsed = _logRequest<
            protocol::requests::FreeGC >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CLEARAREA:
        bytes_parsed = _logRequest<
            protocol::requests::ClearArea >( conn, data, sz );
        break;
    case protocol::requests::opcodes::COPYAREA:
        bytes_parsed = _logRequest<
            protocol::requests::CopyArea >( conn, data, sz );
        break;
    case protocol::requests::opcodes::COPYPLANE:
        bytes_parsed = _logRequest<
            protocol::requests::CopyPlane >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYPOINT:
        bytes_parsed = _logRequest<
            protocol::requests::PolyPoint >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYLINE:
        bytes_parsed = _logRequest<
            protocol::requests::PolyLine >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYSEGMENT:
        bytes_parsed = _logRequest<
            protocol::requests::PolySegment >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYRECTANGLE:
        bytes_parsed = _logRequest<
            protocol::requests::PolyRectangle >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYARC:
        bytes_parsed = _logRequest<
            protocol::requests::PolyArc >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FILLPOLY:
        bytes_parsed = _logRequest<
            protocol::requests::FillPoly >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYFILLRECTANGLE:
        bytes_parsed = _logRequest<
            protocol::requests::PolyFillRectangle >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYFILLARC:
        bytes_parsed = _logRequest<
            protocol::requests::PolyFillArc >( conn, data, sz );
        break;
    case protocol::requests::opcodes::PUTIMAGE:
        bytes_parsed = _logRequest<
            protocol::requests::PutImage >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETIMAGE:
        bytes_parsed = _logRequest<
            protocol::requests::GetImage >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYTEXT8:
        bytes_parsed = _logRequest<
            protocol::requests::PolyText8 >( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYTEXT16:
        bytes_parsed = _logRequest<
            protocol::requests::PolyText16 >( conn, data, sz );
        break;
    case protocol::requests::opcodes::IMAGETEXT8:
        bytes_parsed = _logRequest<
            protocol::requests::ImageText8 >( conn, data, sz );
        break;
    case protocol::requests::opcodes::IMAGETEXT16:
        bytes_parsed = _logRequest<
            protocol::requests::ImageText16 >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATECOLORMAP:
        bytes_parsed = _logRequest<
            protocol::requests::CreateColormap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREECOLORMAP:
        bytes_parsed = _logRequest<
            protocol::requests::FreeColormap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::COPYCOLORMAPANDFREE:
        bytes_parsed = _logRequest<
            protocol::requests::CopyColormapAndFree >( conn, data, sz );
        break;
    case protocol::requests::opcodes::INSTALLCOLORMAP:
        bytes_parsed = _logRequest<
            protocol::requests::InstallColormap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNINSTALLCOLORMAP:
        bytes_parsed = _logRequest<
            protocol::requests::UninstallColormap >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTINSTALLEDCOLORMAPS:
        bytes_parsed = _logRequest<
            protocol::requests::ListInstalledColormaps >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLOR:
        bytes_parsed = _logRequest<
            protocol::requests::AllocColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCNAMEDCOLOR:
        bytes_parsed = _logRequest<
            protocol::requests::AllocNamedColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLORCELLS:
        bytes_parsed = _logRequest<
            protocol::requests::AllocColorCells >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLORPLANES:
        bytes_parsed = _logRequest<
            protocol::requests::AllocColorPlanes >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREECOLORS:
        bytes_parsed = _logRequest<
            protocol::requests::FreeColors >( conn, data, sz );
        break;
    case protocol::requests::opcodes::STORECOLORS:
        bytes_parsed = _logRequest<
            protocol::requests::StoreColors >( conn, data, sz );
        break;
    case protocol::requests::opcodes::STORENAMEDCOLOR:
        bytes_parsed = _logRequest<
            protocol::requests::StoreNamedColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYCOLORS:
        bytes_parsed = _logRequest<
            protocol::requests::QueryColors >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LOOKUPCOLOR:
        bytes_parsed = _logRequest<
            protocol::requests::LookupColor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATECURSOR:
        bytes_parsed = _logRequest<
            protocol::requests::CreateCursor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATEGLYPHCURSOR:
        bytes_parsed = _logRequest<
            protocol::requests::CreateGlyphCursor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREECURSOR:
        bytes_parsed = _logRequest<
            protocol::requests::FreeCursor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::RECOLORCURSOR:
        bytes_parsed = _logRequest<
            protocol::requests::RecolorCursor >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYBESTSIZE:
        bytes_parsed = _logRequest<
            protocol::requests::QueryBestSize >( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYEXTENSION:
        bytes_parsed = _logRequest<
            protocol::requests::QueryExtension >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTEXTENSIONS:
        bytes_parsed = _logRequest<
            protocol::requests::ListExtensions >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEKEYBOARDMAPPING:
        bytes_parsed = _logRequest<
            protocol::requests::ChangeKeyboardMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETKEYBOARDMAPPING:
        bytes_parsed = _logRequest<
            protocol::requests::GetKeyboardMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEKEYBOARDCONTROL:
        bytes_parsed = _logRequest<
            protocol::requests::ChangeKeyboardControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETKEYBOARDCONTROL:
        bytes_parsed = _logRequest<
            protocol::requests::GetKeyboardControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::BELL:
        bytes_parsed = _logRequest<
            protocol::requests::Bell >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEPOINTERCONTROL:
        bytes_parsed = _logRequest<
            protocol::requests::ChangePointerControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPOINTERCONTROL:
        bytes_parsed = _logRequest<
            protocol::requests::GetPointerControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETSCREENSAVER:
        bytes_parsed = _logRequest<
            protocol::requests::SetScreenSaver >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETSCREENSAVER:
        bytes_parsed = _logRequest<
            protocol::requests::GetScreenSaver >( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEHOSTS:
        bytes_parsed = _logRequest<
            protocol::requests::ChangeHosts >( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTHOSTS:
        bytes_parsed = _logRequest<
            protocol::requests::ListHosts >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETACCESSCONTROL:
        bytes_parsed = _logRequest<
            protocol::requests::SetAccessControl >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETCLOSEDOWNMODE:
        bytes_parsed = _logRequest<
            protocol::requests::SetCloseDownMode >( conn, data, sz );
        break;
    case protocol::requests::opcodes::KILLCLIENT:
        bytes_parsed = _logRequest<
            protocol::requests::KillClient >( conn, data, sz );
        break;
    case protocol::requests::opcodes::ROTATEPROPERTIES:
        bytes_parsed = _logRequest<
            protocol::requests::RotateProperties >( conn, data, sz );
        break;
    case protocol::requests::opcodes::FORCESCREENSAVER:
        bytes_parsed = _logRequest<
            protocol::requests::ForceScreenSaver >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETPOINTERMAPPING:
        bytes_parsed = _logRequest<
            protocol::requests::SetPointerMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPOINTERMAPPING:
        bytes_parsed = _logRequest<
            protocol::requests::GetPointerMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETMODIFIERMAPPING:
        bytes_parsed = _logRequest<
            protocol::requests::SetModifierMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETMODIFIERMAPPING:
        bytes_parsed = _logRequest<
            protocol::requests::GetModifierMapping >( conn, data, sz );
        break;
    case protocol::requests::opcodes::NOOPERATION:
        bytes_parsed = _logRequest<
            protocol::requests::NoOperation >( conn, data, sz );
        break;
    default:
        break;
    };
    return bytes_parsed;
}
