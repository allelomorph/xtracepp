#include <string_view>
#include <unordered_set>

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
    using protocol::requests::impl::SimpleReqEncoding;
    const SimpleReqEncoding* encoding {
        reinterpret_cast< const SimpleReqEncoding* >( data ) };
    bytes_parsed += sizeof( SimpleReqEncoding );
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logSimpleWindowRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::impl::SimpleWindowReqEncoding;
    const SimpleWindowReqEncoding* encoding {
        reinterpret_cast< const SimpleWindowReqEncoding* >( data ) };
    bytes_parsed += sizeof( SimpleWindowReqEncoding );
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
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
        reinterpret_cast<const char*>( data + bytes_parsed ), encoding->n };
    bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}\"{}\"{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "max-names", name_width, _equals,
        _formatInteger( encoding->max_names ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n", name_width, _equals,
            _formatInteger( encoding->n ), _separator ) : "",
        memb_indent, "pattern", name_width, _equals,
        pattern, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logCreateWindow(
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

    static const uint32_t tab_ct { 0 };
    // TBD lifetime seems too short (causes read-after-free segfaults) if initialized
    //   nested in value_list_inputs initializer
    const std::vector< _EnumTraits > value_traits {
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
        CreateWindow::value_types,
        CreateWindow::value_names,
        value_traits,
        tab_ct + 2
    };
    _LISTParsingOutputs value_list_outputs;
    _parseLISTofVALUE(
        encoding->value_mask, value_list_inputs,
        data + bytes_parsed, &value_list_outputs );
    bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "depth", name_width, _equals,
        _formatInteger( encoding->depth ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "wid", name_width, _equals,
        _formatCommonType( encoding->wid ), _separator,
        memb_indent, "parent", name_width, _equals,
        _formatCommonType( encoding->parent ), _separator,
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
        memb_indent, "class", name_width, _equals,
        _formatInteger( encoding->class_,
                        protocol::enum_names::window_class ), _separator,
        memb_indent, "visual", name_width, _equals,
        _formatCommonType( encoding->visual,
                           protocol::enum_names::zero_copy_from_parent ), _separator,
        memb_indent, "value-mask", name_width, _equals,
        _formatBitmask( encoding->value_mask ), _separator,
        memb_indent, "value-list", name_width, _equals,
        value_list_outputs.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logChangeWindowAttributes(
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

    static const uint32_t tab_ct { 0 };
    // TBD lifetime seems too short (causes read-after-free segfaults) if initialized
    //   nested in value_list_inputs initializer
    // VALUE list encoding same as CreateWindow
    const std::vector< _EnumTraits > value_traits {
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
        ChangeWindowAttributes::value_types,
        ChangeWindowAttributes::value_names,
        value_traits,
        tab_ct + 2
    };
    _LISTParsingOutputs value_list_outputs;
    _parseLISTofVALUE(
        encoding->value_mask, value_list_inputs,
        data + bytes_parsed, &value_list_outputs );
    bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "value-mask", name_width, _equals,
        _formatBitmask( encoding->value_mask ), _separator,
        memb_indent, "value-list", name_width, _equals,
        value_list_outputs.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logChangeSaveSet(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "mode", name_width, _equals,
        _formatInteger( encoding->mode, ChangeSaveSet::mode_names ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logReparentWindow(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "parent", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "x", name_width, _equals,
        _formatInteger( encoding->x ), _separator,
        memb_indent, "y", name_width, _equals,
        _formatInteger( encoding->y ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logConfigureWindow(
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

    static const uint32_t tab_ct { 0 };
    // TBD lifetime seems too short (causes read-after-free segfaults) if initialized
    //   nested in value_list_inputs initializer
    // VALUE list encoding same as CreateWindow
    const std::vector< _EnumTraits > value_traits {
        /* x            */ {},
        /* y            */ {},
        /* width        */ {},
        /* height       */ {},
        /* border-width */ {},
        /* sibling      */ {},
        /* stack-mode   */ { ConfigureWindow::stack_mode_names },
    };
    const _LISTofVALUEParsingInputs value_list_inputs {
        ConfigureWindow::value_types,
        ConfigureWindow::value_names,
        value_traits,
        tab_ct + 2
    };
    _LISTParsingOutputs value_list_outputs;
    _parseLISTofVALUE(
        encoding->value_mask, value_list_inputs,
        data + bytes_parsed, &value_list_outputs );
    bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "value-mask", name_width, _equals,
        _formatBitmask( encoding->value_mask ), _separator,
        memb_indent, "value-list", name_width, _equals,
        value_list_outputs.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logCirculateWindow(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "direction", name_width, _equals,
        _formatInteger( encoding->direction, CirculateWindow::direction_names ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logGetGeometry(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logInternAtom(
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
    // followed by pad(n) STRING8 name
    std::string_view name {
        reinterpret_cast<const char*>( data + bytes_parsed ), encoding->n };
    bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}{}"
        "{}{: <{}}{}\"{}\"{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "only-if-exists", name_width, _equals,
        _formatCommonType( encoding->only_if_exists ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n", name_width, _equals,
            _formatInteger( encoding->n ), _separator ) : "",
        memb_indent, "name", name_width, _equals,
        name, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logGetAtomName(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "atom", name_width, _equals,
        _formatCommonType( encoding->atom ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logChangeProperty(
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
    assert( encoding->format == 8 || encoding->format == 16 ||
        encoding->format == 32 );
    std::string data_hex;
    const uint32_t data_sz {
        encoding->format == 32 ? encoding->fmt_unit_ct / 4 :
        encoding->format == 16 ? encoding->fmt_unit_ct / 2 :
        /* encoding->format == 8 */ encoding->fmt_unit_ct };
    for ( uint32_t i {}; i < data_sz; ++i ) {
        data_hex += fmt::format(
            "{}{:02X}", data_hex.empty() ? "" : " ",
            *(data + bytes_parsed + i) );
    }
    bytes_parsed += data_sz;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );

    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}[{}]{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "mode", name_width, _equals,
        _formatInteger( encoding->mode, ChangeProperty::mode_names ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "property", name_width, _equals,
        _formatCommonType( encoding->property ), _separator,
        memb_indent, "type", name_width, _equals,
        _formatCommonType( encoding->type ), _separator,
        memb_indent, "format", name_width, _equals,
        _formatInteger( encoding->format ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "format units", name_width, _equals,
            _formatInteger( encoding->fmt_unit_ct ), _separator ) : "",
        memb_indent, "data", name_width, _equals,
        data_hex, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logDeleteProperty(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "property", name_width, _equals,
        _formatCommonType( encoding->property ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logGetProperty(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "delete", name_width, _equals,
        _formatCommonType( encoding->delete_ ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "property", name_width, _equals,
        _formatCommonType( encoding->property ), _separator,
        memb_indent, "type", name_width, _equals,
        _formatCommonType( encoding->type, GetProperty::request_type_names ), _separator,
        memb_indent, "long-offset", name_width, _equals,
        _formatInteger( encoding->long_offset ), _separator,
        memb_indent, "long-length", name_width, _equals,
        _formatInteger( encoding->long_length ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logSetSelectionOwner(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "owner", name_width, _equals,
        _formatCommonType( encoding->owner, SetSelectionOwner::owner_names ), _separator,
        memb_indent, "selection", name_width, _equals,
        _formatCommonType( encoding->selection ), _separator,
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time, SetSelectionOwner::time_names ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logGetSelectionOwner(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "selection", name_width, _equals,
        _formatCommonType( encoding->selection ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logConvertSelection(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "requestor", name_width, _equals,
        _formatCommonType( encoding->requestor ), _separator,
        memb_indent, "selection", name_width, _equals,
        _formatCommonType( encoding->selection ), _separator,
        memb_indent, "target", name_width, _equals,
        _formatCommonType( encoding->target ), _separator,
        memb_indent, "property", name_width, _equals,
        _formatCommonType( encoding->property, ConvertSelection::property_names ), _separator,
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time, ConvertSelection::time_names ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logSendEvent(
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
    // size_t _logServerEvent()
    bytes_parsed += protocol::events::ENCODING_SZ;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    /*
      000:<:0001: 44: Request(25): SendEvent propagate=false(0x00) destination=PointerWindow(0x00000000) event-mask=StructureNotify ConfigureNotify(22) event=0x00000000 window=0x00000000 above-sibling=None(0x00000000) x=0 y=0 width=0 height=0 border-width=0 override-redirect=false(0x00)
     */
    // TBD parse event ahead of time
    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "propagate", name_width, _equals,
        _formatCommonType( encoding->propagate ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "destination", name_width, _equals,
        _formatCommonType( encoding->destination, SendEvent::destination_names ), _separator,
        memb_indent, "event-mask", name_width, _equals,
        _formatCommonType( encoding->event_mask ), _separator,
        // TBD update after event formatting is complete
        memb_indent, "event", name_width, _equals,
        "(formatted event TBD)", _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logGrabPointer(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "owner-events", name_width, _equals,
        _formatCommonType( encoding->owner_events ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "grab-window", name_width, _equals,
        _formatCommonType( encoding->grab_window ), _separator,
        memb_indent, "event-mask", name_width, _equals,
        _formatCommonType( encoding->event_mask ), _separator,
        memb_indent, "pointer-mode", name_width, _equals,
        _formatInteger( encoding->pointer_mode, GrabPointer::pointer_mode_names ), _separator,
        memb_indent, "keyboard-mode", name_width, _equals,
        _formatInteger( encoding->keyboard_mode, GrabPointer::keyboard_mode_names ), _separator,
        memb_indent, "confine-to", name_width, _equals,
        _formatCommonType( encoding->confine_to, GrabPointer::confine_to_names ), _separator,
        memb_indent, "cursor", name_width, _equals,
        _formatCommonType( encoding->cursor, GrabPointer::cursor_names ), _separator,
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time, GrabPointer::time_names ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logUngrabPointer(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time, UngrabPointer::time_names ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logGrabButton(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "owner-events", name_width, _equals,
        _formatCommonType( encoding->owner_events ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "grab-window", name_width, _equals,
        _formatCommonType( encoding->grab_window ), _separator,
        memb_indent, "event-mask", name_width, _equals,
        _formatCommonType( encoding->event_mask ), _separator,
        memb_indent, "pointer-mode", name_width, _equals,
        _formatInteger( encoding->pointer_mode, GrabButton::pointer_mode_names ), _separator,
        memb_indent, "keyboard-mode", name_width, _equals,
        _formatInteger( encoding->keyboard_mode, GrabButton::keyboard_mode_names ), _separator,
        memb_indent, "confine-to", name_width, _equals,
        _formatCommonType( encoding->confine_to, GrabButton::confine_to_names ), _separator,
        memb_indent, "cursor", name_width, _equals,
        _formatCommonType( encoding->cursor, GrabButton::cursor_names ), _separator,
        memb_indent, "button", name_width, _equals,
        _formatCommonType( encoding->button, GrabButton::button_names ), _separator,
        memb_indent, "modifiers", name_width, _equals,
        _formatCommonType( encoding->modifiers ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logUngrabButton(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "button", name_width, _equals,
        _formatCommonType( encoding->button, UngrabButton::button_names ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "grab-window", name_width, _equals,
        _formatCommonType( encoding->grab_window ), _separator,
        memb_indent, "modifiers", name_width, _equals,
        _formatCommonType( encoding->modifiers ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logChangeActivePointerGrab(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "cursor", name_width, _equals,
        _formatCommonType( encoding->cursor, ChangeActivePointerGrab::cursor_names ), _separator,
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time, ChangeActivePointerGrab::time_names ), _separator,
        memb_indent, "event-mask", name_width, _equals,
        _formatCommonType( encoding->event_mask ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logGrabKeyboard(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "owner-events", name_width, _equals,
        _formatCommonType( encoding->owner_events ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "grab-window", name_width, _equals,
        _formatCommonType( encoding->grab_window ), _separator,
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time, GrabKeyboard::time_names ), _separator,
        memb_indent, "pointer-mode", name_width, _equals,
        _formatInteger( encoding->pointer_mode, GrabKeyboard::pointer_mode_names ), _separator,
        memb_indent, "keyboard-mode", name_width, _equals,
        _formatInteger( encoding->keyboard_mode, GrabKeyboard::keyboard_mode_names ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logUngrabKeyboard(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time, UngrabKeyboard::time_names ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logGrabKey(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "owner-events", name_width, _equals,
        _formatCommonType( encoding->owner_events ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "modifiers", name_width, _equals,
        _formatCommonType( encoding->modifiers ), _separator,
        memb_indent, "key", name_width, _equals,
        _formatCommonType( encoding->key, GrabKey::key_names ), _separator,
        memb_indent, "pointer-mode", name_width, _equals,
        _formatInteger( encoding->pointer_mode, GrabKey::pointer_mode_names ), _separator,
        memb_indent, "keyboard-mode", name_width, _equals,
        _formatInteger( encoding->keyboard_mode, GrabKey::keyboard_mode_names ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logUngrabKey(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "key", name_width, _equals,
        _formatCommonType( encoding->key, UngrabKey::key_names ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "grab-window", name_width, _equals,
        _formatCommonType( encoding->grab_window ), _separator,
        memb_indent, "modifiers", name_width, _equals,
        _formatCommonType( encoding->modifiers ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logAllowEvents(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "mode", name_width, _equals,
        _formatInteger( encoding->mode, AllowEvents::mode_names ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time, AllowEvents::time_names ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logGetMotionEvents(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "start", name_width, _equals,
        _formatCommonType( encoding->start, GetMotionEvents::start_names ), _separator,
        memb_indent, "stop", name_width, _equals,
        _formatCommonType( encoding->stop, GetMotionEvents::stop_names ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logTranslateCoordinates(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "src-window", name_width, _equals,
        _formatCommonType( encoding->src_window ), _separator,
        memb_indent, "dst-window", name_width, _equals,
        _formatCommonType( encoding->dst_window ), _separator,
        memb_indent, "src-x", name_width, _equals,
        _formatInteger( encoding->src_x ), _separator,
        memb_indent, "src-y", name_width, _equals,
        _formatInteger( encoding->src_y ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logWarpPointer(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "src-window", name_width, _equals,
        _formatCommonType( encoding->src_window, WarpPointer::src_window_names ), _separator,
        memb_indent, "dst-window", name_width, _equals,
        _formatCommonType( encoding->dst_window, WarpPointer::dst_window_names ), _separator,
        memb_indent, "src-x", name_width, _equals,
        _formatInteger( encoding->src_x ), _separator,
        memb_indent, "src-y", name_width, _equals,
        _formatInteger( encoding->src_y ), _separator,
        memb_indent, "src-width", name_width, _equals,
        _formatInteger( encoding->src_width ), _separator,
        memb_indent, "src-height", name_width, _equals,
        _formatInteger( encoding->src_height ), _separator,
        memb_indent, "dst-x", name_width, _equals,
        _formatInteger( encoding->dst_x ), _separator,
        memb_indent, "dst-y", name_width, _equals,
        _formatInteger( encoding->dst_y ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logSetInputFocus(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "revert-to", name_width, _equals,
        _formatInteger( encoding->revert_to, SetInputFocus::revert_to_names ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "focus", name_width, _equals,
        _formatCommonType( encoding->focus, SetInputFocus::focus_names ), _separator,
        memb_indent, "time", name_width, _equals,
        _formatCommonType( encoding->time, SetInputFocus::time_names ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logOpenFont(
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
        reinterpret_cast<const char*>( data + bytes_parsed ), encoding->n };
    bytes_parsed += _pad( encoding->n );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}\"{}\"{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "fid", name_width, _equals,
        _formatCommonType( encoding->fid ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n", name_width, _equals,
            _formatInteger( encoding->n ), _separator ) : "",
        memb_indent, "name", name_width, _equals,
        name, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logCloseFont(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "font", name_width, _equals,
        _formatCommonType( encoding->font ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logQueryFont(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "font", name_width, _equals,
        // TBD is it necessary to resolve FONT or GCONTEXT from FONTABLE?
        _formatCommonType( encoding->font.font ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logQueryTextExtents(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}{: <{}}{}[{}]{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "odd length", name_width, _equals,
            _formatCommonType( encoding->odd_length ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "font", name_width, _equals,
        // TBD is it necessary to resolve FONT or GCONTEXT from FONTABLE?
        _formatCommonType( encoding->font.font ), _separator,
        memb_indent, "string", name_width, _equals,
        string_as_hex, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logSetFontPath(
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

    _LISTParsingOutputs path {
        _parseLISTofSTR( data + bytes_parsed, encoding->str_ct ) };
    bytes_parsed += _pad( path.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "STRs in path", name_width, _equals,
            _formatInteger( encoding->str_ct ), _separator ) : "",
        memb_indent, "path", name_width, _equals,
        path.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logCreatePixmap(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "depth", name_width, _equals,
        _formatInteger( encoding->depth ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "pid", name_width, _equals,
        _formatCommonType( encoding->pid ), _separator,
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        memb_indent, "width", name_width, _equals,
        _formatInteger( encoding->width ), _separator,
        memb_indent, "height", name_width, _equals,
        _formatInteger( encoding->height ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logFreePixmap(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "pixmap", name_width, _equals,
        _formatCommonType( encoding->pixmap ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logCreateGC(
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

    static const uint32_t tab_ct { 0 };
    // TBD lifetime seems too short (causes read-after-free segfaults) if initialized
    //   nested in value_list_inputs initializer
    const std::vector< _EnumTraits > value_traits {
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
        CreateGC::value_types,
        CreateGC::value_names,
        value_traits,
        tab_ct + 2
    };
    _LISTParsingOutputs value_list_outputs;
    _parseLISTofVALUE(
        encoding->value_mask, value_list_inputs,
        data + bytes_parsed, &value_list_outputs );
    bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "cid", name_width, _equals,
        _formatCommonType( encoding->cid ), _separator,
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        // TBD only print value-mask in verbose mode? print all flags even with value-list?
        memb_indent, "value-mask", name_width, _equals,
        _formatBitmask( encoding->value_mask ), _separator,
        memb_indent, "value-list", name_width, _equals,
        value_list_outputs.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logChangeGC(
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

    static const uint32_t tab_ct { 0 };
    // TBD lifetime seems too short (causes read-after-free segfaults) if initialized
    //   nested in value_list_inputs initializer
    const std::vector< _EnumTraits > value_traits {
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
        ChangeGC::value_types,
        ChangeGC::value_names,
        value_traits,
        tab_ct + 2
    };
    _LISTParsingOutputs value_list_outputs;
    _parseLISTofVALUE(
        encoding->value_mask, value_list_inputs,
        data + bytes_parsed, &value_list_outputs );
    bytes_parsed += value_list_outputs.bytes_parsed;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        // TBD only print value-mask in verbose mode? print all flags even with value-list?
        memb_indent, "value-mask", name_width, _equals,
        _formatBitmask( encoding->value_mask ), _separator,
        memb_indent, "value-list", name_width, _equals,
        value_list_outputs.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logCopyGC(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "src-gc", name_width, _equals,
        _formatCommonType( encoding->src_gc ), _separator,
        memb_indent, "dst-gc", name_width, _equals,
        _formatCommonType( encoding->dst_gc ), _separator,
        memb_indent, "value-mask", name_width, _equals,
        _formatBitmask( encoding->value_mask, CopyGC::value_names ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logSetDashes(
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

    _LISTParsingOutputs dashes {
        _parseLISTofCARD8( data + bytes_parsed, encoding->n ) };
    bytes_parsed += _pad( dashes.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        memb_indent, "dash-offset", name_width, _equals,
        _formatInteger( encoding->dash_offset ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "n", name_width, _equals,
            _formatInteger( encoding->n ), _separator ) : "",
        memb_indent, "dashes", name_width, _equals,
        dashes.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logSetClipRectangles(
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
    _LISTParsingOutputs rectangles {
        _parseLISTofRECTANGLE( data + bytes_parsed, n_rectangles ) };
    bytes_parsed += _pad( rectangles.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "ordering", name_width, _equals,
        _formatInteger( encoding->ordering, SetClipRectangles::ordering_names  ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        memb_indent, "clip-x-origin", name_width, _equals,
        _formatInteger( encoding->clip_x_origin ), _separator,
        memb_indent, "clip-y-origin", name_width, _equals,
        _formatInteger( encoding->clip_y_origin ), _separator,
        memb_indent, "rectangles", name_width, _equals,
        rectangles.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logFreeGC(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logClearArea(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "exposures", name_width, _equals,
        _formatCommonType( encoding->exposures ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "window", name_width, _equals,
        _formatCommonType( encoding->window ), _separator,
        memb_indent, "x", name_width, _equals,
        _formatInteger( encoding->x ), _separator,
        memb_indent, "y", name_width, _equals,
        _formatInteger( encoding->y ), _separator,
        memb_indent, "width", name_width, _equals,
        _formatInteger( encoding->width ), _separator,
        memb_indent, "height", name_width, _equals,
        _formatInteger( encoding->height ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logCopyArea(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "src-drawable", name_width, _equals,
        _formatCommonType( encoding->src_drawable ), _separator,
        memb_indent, "dst-drawable", name_width, _equals,
        _formatCommonType( encoding->dst_drawable ), _separator,
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        memb_indent, "src-x", name_width, _equals,
        _formatInteger( encoding->src_x ), _separator,
        memb_indent, "src-y", name_width, _equals,
        _formatInteger( encoding->src_y ), _separator,
        memb_indent, "dst-x", name_width, _equals,
        _formatInteger( encoding->dst_x ), _separator,
        memb_indent, "dst-y", name_width, _equals,
        _formatInteger( encoding->dst_y ), _separator,
        memb_indent, "width", name_width, _equals,
        _formatInteger( encoding->width ), _separator,
        memb_indent, "height", name_width, _equals,
        _formatInteger( encoding->height ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logCopyPlane(
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

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "src-drawable", name_width, _equals,
        _formatCommonType( encoding->src_drawable ), _separator,
        memb_indent, "dst-drawable", name_width, _equals,
        _formatCommonType( encoding->dst_drawable ), _separator,
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        memb_indent, "src-x", name_width, _equals,
        _formatInteger( encoding->src_x ), _separator,
        memb_indent, "src-y", name_width, _equals,
        _formatInteger( encoding->src_y ), _separator,
        memb_indent, "dst-x", name_width, _equals,
        _formatInteger( encoding->dst_x ), _separator,
        memb_indent, "dst-y", name_width, _equals,
        _formatInteger( encoding->dst_y ), _separator,
        memb_indent, "width", name_width, _equals,
        _formatInteger( encoding->width ), _separator,
        memb_indent, "height", name_width, _equals,
        _formatInteger( encoding->height ), _separator,
        memb_indent, "bit-plane", name_width, _equals,
        _formatInteger( encoding->bit_plane ), _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logPolyPoint(
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
    _LISTParsingOutputs points {
        _parseLISTofPOINT( data + bytes_parsed, n_points ) };
    bytes_parsed += _pad( points.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "coordinate-mode" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "coordinate-mode", name_width, _equals,
        _formatInteger( encoding->coordinate_mode, PolyPoint::coordinate_mode_names ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        memb_indent, "points", name_width, _equals,
        points.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

// TBD same encoding as PolyPoint; generalize?
size_t X11ProtocolParser::_logPolyLine(
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
    _LISTParsingOutputs points {
        _parseLISTofPOINT( data + bytes_parsed, n_points ) };
    bytes_parsed += _pad( points.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "coordinate-mode" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        memb_indent, "coordinate-mode", name_width, _equals,
        _formatInteger( encoding->coordinate_mode, PolyLine::coordinate_mode_names ), _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        memb_indent, "points", name_width, _equals,
        points.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logPolySegment(
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
    _LISTParsingOutputs segments {
        _parseLISTofSEGMENT( data + bytes_parsed, n_segments ) };
    bytes_parsed += _pad( segments.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        memb_indent, "segments", name_width, _equals,
        segments.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logPolyRectangle(
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
    _LISTParsingOutputs rectangles {
        _parseLISTofRECTANGLE( data + bytes_parsed, n_rectangles ) };
    bytes_parsed += _pad( rectangles.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        memb_indent, "rectangles", name_width, _equals,
        rectangles.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logPolyArc(
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
    _LISTParsingOutputs arcs {
        _parseLISTofARC( data + bytes_parsed, n_arcs ) };
    bytes_parsed += _pad( arcs.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        memb_indent, "arcs", name_width, _equals,
        arcs.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logFillPoly(
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
    _LISTParsingOutputs points {
        _parseLISTofPOINT( data + bytes_parsed, n_points ) };
    bytes_parsed += _pad( points.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "coordinate-mode" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        memb_indent, "shape", name_width, _equals,
        _formatInteger( encoding->shape, FillPoly::shape_names ), _separator,
        memb_indent, "coordinate-mode", name_width, _equals,
        _formatInteger( encoding->coordinate_mode, FillPoly::coordinate_mode_names ), _separator,
        memb_indent, "points", name_width, _equals,
        points.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logPolyFillRectangle(
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
    _LISTParsingOutputs rectangles {
        _parseLISTofRECTANGLE( data + bytes_parsed, n_rectangles ) };
    bytes_parsed += _pad( rectangles.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "coordinate-mode" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        memb_indent, "rectangles", name_width, _equals,
        rectangles.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logPolyFillArc(
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
    _LISTParsingOutputs arcs {
        _parseLISTofARC( data + bytes_parsed, n_arcs ) };
    bytes_parsed += _pad( arcs.bytes_parsed );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "coordinate-mode" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        memb_indent, "drawable", name_width, _equals,
        _formatCommonType( encoding->drawable ), _separator,
        memb_indent, "gc", name_width, _equals,
        _formatCommonType( encoding->gc ), _separator,
        memb_indent, "arcs", name_width, _equals,
        arcs.str, _separator,
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}

size_t X11ProtocolParser::_logPutImage(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGetImage(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logPolyText8(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logPolyText16(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logImageText8(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logImageText16(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logCreateColormap(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logFreeColormap(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logCopyColormapAndFree(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logInstallColormap(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logUninstallColormap(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logAllocColor(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logAllocNamedColor(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logAllocColorCells(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logAllocColorPlanes(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logFreeColors(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logStoreColors(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logStoreNamedColor(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logQueryColors(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logLookupColor(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logCreateCursor(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logCreateGlyphCursor(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logFreeCursor(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logRecolorCursor(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logQueryBestSize(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logQueryExtension(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logChangeKeyboardMapping(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGetKeyboardMapping(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logChangeKeyboardControl(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logBell(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logChangePointerControl(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSetScreenSaver(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logChangeHosts(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSetAccessControl(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSetCloseDownMode(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logKillClient(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logRotateProperties(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logForceScreenSaver(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSetPointerMapping(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSetModifierMapping(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

// TBD not using _logSimpleRequest due to potential unused suffix data
size_t X11ProtocolParser::_logNoOperation(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};
    using protocol::requests::impl::SimpleReqEncoding;
    const SimpleReqEncoding* encoding {
        reinterpret_cast< const SimpleReqEncoding* >( data ) };
    bytes_parsed += sizeof( SimpleReqEncoding );
    assert( encoding->opcode == protocol::requests::opcodes::NOOPERATION );
    // protocol specifies that no-op may be followed by variable length dummy data
    bytes_parsed += ( encoding->request_length - 1 ) * _ALIGN;
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );
    fmt::println(
        _log_fs,
        "{{{}"
        "{}{}"
        "{}}}",
        _separator,
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "opcode", name_width, _equals,
            _formatInteger( encoding->opcode ), _separator ) : "",
        _verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "request length", name_width, _equals,
            _formatInteger( encoding->request_length ), _separator ) : "",
        struct_indent
        );
    assert( bytes_parsed == sz );
    return bytes_parsed;
}


size_t X11ProtocolParser::_logClientRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= 4 ); // TBD

    const uint8_t opcode { *data };
    assert( ( opcode >= 1 && opcode <= 119 ) || opcode == 127 );  // TBD core opcodes only to start

    // map opcode to sequence number to aid in parsing request errors and replies
    conn->registerRequest( opcode );
    size_t bytes_parsed {};

    fmt::print( _log_fs, "{:03d}:<:client request {:>3d}: {} ",
                conn->id, opcode, protocol::requests::names[ opcode ] );
    switch ( opcode ) {
    case protocol::requests::opcodes::CREATEWINDOW:
        bytes_parsed = _logCreateWindow( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEWINDOWATTRIBUTES:
        bytes_parsed = _logChangeWindowAttributes( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETWINDOWATTRIBUTES:
        bytes_parsed = _logSimpleWindowRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::DESTROYWINDOW:
        bytes_parsed = _logSimpleWindowRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::DESTROYSUBWINDOWS:
        bytes_parsed = _logSimpleWindowRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGESAVESET:
        bytes_parsed = _logChangeSaveSet( conn, data, sz );
        break;
    case protocol::requests::opcodes::REPARENTWINDOW:
        bytes_parsed = _logReparentWindow( conn, data, sz );
        break;
    case protocol::requests::opcodes::MAPWINDOW:
        bytes_parsed = _logSimpleWindowRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::MAPSUBWINDOWS:
        bytes_parsed = _logSimpleWindowRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNMAPWINDOW:
        bytes_parsed = _logSimpleWindowRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNMAPSUBWINDOWS:
        bytes_parsed = _logSimpleWindowRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::CONFIGUREWINDOW:
        bytes_parsed = _logConfigureWindow( conn, data, sz );
        break;
    case protocol::requests::opcodes::CIRCULATEWINDOW:
        bytes_parsed = _logCirculateWindow( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETGEOMETRY:
        bytes_parsed = _logGetGeometry( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYTREE:
        bytes_parsed = _logSimpleWindowRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::INTERNATOM:
        bytes_parsed = _logInternAtom( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETATOMNAME:
        bytes_parsed = _logGetAtomName( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEPROPERTY:
        bytes_parsed = _logChangeProperty( conn, data, sz );
        break;
    case protocol::requests::opcodes::DELETEPROPERTY:
        bytes_parsed = _logDeleteProperty( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPROPERTY:
        bytes_parsed = _logGetProperty( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTPROPERTIES:
        bytes_parsed = _logSimpleWindowRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETSELECTIONOWNER:
        bytes_parsed = _logSetSelectionOwner( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETSELECTIONOWNER:
        bytes_parsed = _logGetSelectionOwner( conn, data, sz );
        break;
    case protocol::requests::opcodes::CONVERTSELECTION:
        bytes_parsed = _logConvertSelection( conn, data, sz );
        break;
    case protocol::requests::opcodes::SENDEVENT:
        bytes_parsed = _logSendEvent( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABPOINTER:
        bytes_parsed = _logGrabPointer( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABPOINTER:
        bytes_parsed = _logUngrabPointer( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABBUTTON:
        bytes_parsed = _logGrabButton( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABBUTTON:
        bytes_parsed = _logUngrabButton( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEACTIVEPOINTERGRAB:
        bytes_parsed = _logChangeActivePointerGrab( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABKEYBOARD:
        bytes_parsed = _logGrabKeyboard( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABKEYBOARD:
        bytes_parsed = _logUngrabKeyboard( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABKEY:
        bytes_parsed = _logGrabKey( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABKEY:
        bytes_parsed = _logUngrabKey( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOWEVENTS:
        bytes_parsed = _logAllowEvents( conn, data, sz );
        break;
    case protocol::requests::opcodes::GRABSERVER:
        bytes_parsed = _logSimpleRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNGRABSERVER:
        bytes_parsed = _logSimpleRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYPOINTER:
        bytes_parsed = _logSimpleWindowRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETMOTIONEVENTS:
        bytes_parsed = _logGetMotionEvents( conn, data, sz );
        break;
    case protocol::requests::opcodes::TRANSLATECOORDINATES:
        bytes_parsed = _logTranslateCoordinates( conn, data, sz );
        break;
    case protocol::requests::opcodes::WARPPOINTER:
        bytes_parsed = _logWarpPointer( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETINPUTFOCUS:
        bytes_parsed = _logSetInputFocus( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETINPUTFOCUS:
        bytes_parsed = _logSimpleRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYKEYMAP:
        bytes_parsed = _logSimpleRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::OPENFONT:
        bytes_parsed = _logOpenFont( conn, data, sz );
        break;
    case protocol::requests::opcodes::CLOSEFONT:
        bytes_parsed = _logCloseFont( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYFONT:
        bytes_parsed = _logQueryFont( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYTEXTEXTENTS:
        bytes_parsed = _logQueryTextExtents( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTFONTS:
        bytes_parsed = _logListFontsRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTFONTSWITHINFO:
        bytes_parsed = _logListFontsRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETFONTPATH:
        bytes_parsed = _logSetFontPath( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETFONTPATH:
        bytes_parsed = _logSimpleRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATEPIXMAP:
        bytes_parsed = _logCreatePixmap( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREEPIXMAP:
        bytes_parsed = _logFreePixmap( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATEGC:
        bytes_parsed = _logCreateGC( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEGC:
        bytes_parsed = _logChangeGC( conn, data, sz );
        break;
    case protocol::requests::opcodes::COPYGC:
        bytes_parsed = _logCopyGC( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETDASHES:
        bytes_parsed = _logSetDashes( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETCLIPRECTANGLES:
        bytes_parsed = _logSetClipRectangles( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREEGC:
        bytes_parsed = _logFreeGC( conn, data, sz );
        break;
    case protocol::requests::opcodes::CLEARAREA:
        bytes_parsed = _logClearArea( conn, data, sz );
        break;
    case protocol::requests::opcodes::COPYAREA:
        bytes_parsed = _logCopyArea( conn, data, sz );
        break;
    case protocol::requests::opcodes::COPYPLANE:
        bytes_parsed = _logCopyPlane( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYPOINT:
        bytes_parsed = _logPolyPoint( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYLINE:
        bytes_parsed = _logPolyLine( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYSEGMENT:
        bytes_parsed = _logPolySegment( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYRECTANGLE:
        bytes_parsed = _logPolyRectangle( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYARC:
        bytes_parsed = _logPolyArc( conn, data, sz );
        break;
    case protocol::requests::opcodes::FILLPOLY:
        bytes_parsed = _logFillPoly( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYFILLRECTANGLE:
        bytes_parsed = _logPolyFillRectangle( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYFILLARC:
        bytes_parsed = _logPolyFillArc( conn, data, sz );
        break;
    case protocol::requests::opcodes::PUTIMAGE:
        bytes_parsed = _logPutImage( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETIMAGE:
        bytes_parsed = _logGetImage( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYTEXT8:
        bytes_parsed = _logPolyText8( conn, data, sz );
        break;
    case protocol::requests::opcodes::POLYTEXT16:
        bytes_parsed = _logPolyText16( conn, data, sz );
        break;
    case protocol::requests::opcodes::IMAGETEXT8:
        bytes_parsed = _logImageText8( conn, data, sz );
        break;
    case protocol::requests::opcodes::IMAGETEXT16:
        bytes_parsed = _logImageText16( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATECOLORMAP:
        bytes_parsed = _logCreateColormap( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREECOLORMAP:
        bytes_parsed = _logFreeColormap( conn, data, sz );
        break;
    case protocol::requests::opcodes::COPYCOLORMAPANDFREE:
        bytes_parsed = _logCopyColormapAndFree( conn, data, sz );
        break;
    case protocol::requests::opcodes::INSTALLCOLORMAP:
        bytes_parsed = _logInstallColormap( conn, data, sz );
        break;
    case protocol::requests::opcodes::UNINSTALLCOLORMAP:
        bytes_parsed = _logUninstallColormap( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTINSTALLEDCOLORMAPS:
        bytes_parsed = _logSimpleWindowRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLOR:
        bytes_parsed = _logAllocColor( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCNAMEDCOLOR:
        bytes_parsed = _logAllocNamedColor( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLORCELLS:
        bytes_parsed = _logAllocColorCells( conn, data, sz );
        break;
    case protocol::requests::opcodes::ALLOCCOLORPLANES:
        bytes_parsed = _logAllocColorPlanes( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREECOLORS:
        bytes_parsed = _logFreeColors( conn, data, sz );
        break;
    case protocol::requests::opcodes::STORECOLORS:
        bytes_parsed = _logStoreColors( conn, data, sz );
        break;
    case protocol::requests::opcodes::STORENAMEDCOLOR:
        bytes_parsed = _logStoreNamedColor( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYCOLORS:
        bytes_parsed = _logQueryColors( conn, data, sz );
        break;
    case protocol::requests::opcodes::LOOKUPCOLOR:
        bytes_parsed = _logLookupColor( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATECURSOR:
        bytes_parsed = _logCreateCursor( conn, data, sz );
        break;
    case protocol::requests::opcodes::CREATEGLYPHCURSOR:
        bytes_parsed = _logCreateGlyphCursor( conn, data, sz );
        break;
    case protocol::requests::opcodes::FREECURSOR:
        bytes_parsed = _logFreeCursor( conn, data, sz );
        break;
    case protocol::requests::opcodes::RECOLORCURSOR:
        bytes_parsed = _logRecolorCursor( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYBESTSIZE:
        bytes_parsed = _logQueryBestSize( conn, data, sz );
        break;
    case protocol::requests::opcodes::QUERYEXTENSION:
        bytes_parsed = _logQueryExtension( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTEXTENSIONS:
        bytes_parsed = _logSimpleRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEKEYBOARDMAPPING:
        bytes_parsed = _logChangeKeyboardMapping( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETKEYBOARDMAPPING:
        bytes_parsed = _logGetKeyboardMapping( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEKEYBOARDCONTROL:
        bytes_parsed = _logChangeKeyboardControl( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETKEYBOARDCONTROL:
        bytes_parsed = _logSimpleRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::BELL:
        bytes_parsed = _logBell( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEPOINTERCONTROL:
        bytes_parsed = _logChangePointerControl( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPOINTERCONTROL:
        bytes_parsed = _logSimpleRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETSCREENSAVER:
        bytes_parsed = _logSetScreenSaver( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETSCREENSAVER:
        bytes_parsed = _logSimpleRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::CHANGEHOSTS:
        bytes_parsed = _logChangeHosts( conn, data, sz );
        break;
    case protocol::requests::opcodes::LISTHOSTS:
        bytes_parsed = _logSimpleRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETACCESSCONTROL:
        bytes_parsed = _logSetAccessControl( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETCLOSEDOWNMODE:
        bytes_parsed = _logSetCloseDownMode( conn, data, sz );
        break;
    case protocol::requests::opcodes::KILLCLIENT:
        bytes_parsed = _logKillClient( conn, data, sz );
        break;
    case protocol::requests::opcodes::ROTATEPROPERTIES:
        bytes_parsed = _logRotateProperties( conn, data, sz );
        break;
    case protocol::requests::opcodes::FORCESCREENSAVER:
        bytes_parsed = _logForceScreenSaver( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETPOINTERMAPPING:
        bytes_parsed = _logSetPointerMapping( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETPOINTERMAPPING:
        bytes_parsed = _logSimpleRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::SETMODIFIERMAPPING:
        bytes_parsed = _logSetModifierMapping( conn, data, sz );
        break;
    case protocol::requests::opcodes::GETMODIFIERMAPPING:
        bytes_parsed = _logSimpleRequest( conn, data, sz );
        break;
    case protocol::requests::opcodes::NOOPERATION:
        bytes_parsed = _logNoOperation( conn, data, sz );
        break;
    default:
        break;
    };
    return bytes_parsed;
}
