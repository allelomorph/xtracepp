#include <string_view>

#include <cctype>      // isprint

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
//#include "Settings.hpp"
#include "protocol/common_types.hpp"
#include "protocol/requests.hpp"


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

    // TBD make intro line into distinct func?
    fmt::print( _log_fs, "{:03d}:<:client request {:>3d}: {} ",
                conn->id, encoding->opcode,
                protocol::requests::names[ encoding->opcode ] );
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

    // TBD make intro line into distinct func?
    fmt::print( _log_fs, "{:03d}:<:client request {:>3d}: {} ",
                conn->id, encoding->opcode,
                protocol::requests::names[ encoding->opcode ] );
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

size_t X11ProtocolParser::_logGetWindowAttributes(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );  // TBD check min size

    size_t bytes_parsed {};

    using protocol::requests::GetWindowAttributes;
    const GetWindowAttributes::Encoding* encoding {
        reinterpret_cast< const GetWindowAttributes::Encoding* >( data ) };
    bytes_parsed += sizeof( GetWindowAttributes::Encoding );
    assert( encoding->request_length == bytes_parsed / _ALIGN );

    static const uint32_t tab_ct { 0 };
    const std::string_view struct_indent {
        _multiline ? _tabIndent( tab_ct ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( tab_ct + 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "request length" ) - 1 : 0 );

    // TBD make intro line into distinct func?
    fmt::print( _log_fs, "{:03d}:<:client request {:>3d}: {} ",
                conn->id, encoding->opcode,
                protocol::requests::names[ encoding->opcode ] );
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

size_t X11ProtocolParser::_logDestroyWindow(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logDestroySubwindows(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logChangeSaveSet(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logReparentWindow(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logMapWindow(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logMapSubwindows(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logUnmapWindow(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logUnmapSubwindows(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logConfigureWindow(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logCirculateWindow(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGetGeometry(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logQueryTree(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logInternAtom(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGetAtomName(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logChangeProperty(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logDeleteProperty(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGetProperty(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logListProperties(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSetSelectionOwner(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGetSelectionOwner(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logConvertSelection(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSendEvent(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGrabPointer(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logUngrabPointer(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGrabButton(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logUngrabButton(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logChangeActivePointerGrab(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGrabKeyboard(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logUngrabKeyboard(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGrabKey(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logUngrabKey(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logAllowEvents(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGrabServer(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logUngrabServer(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logQueryPointer(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGetMotionEvents(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logTranslateCoordinates(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logWarpPointer(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSetInputFocus(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGetInputFocus(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logQueryKeymap(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logOpenFont(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logCloseFont(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logQueryFont(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logQueryTextExtents(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logListFonts(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logListFontsWithInfo(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSetFontPath(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGetFontPath(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logCreatePixmap(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logFreePixmap(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logCreateGC(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logChangeGC(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logCopyGC(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSetDashes(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSetClipRectangles(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logFreeGC(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logClearArea(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logCopyArea(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logCopyPlane(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logPolyPoint(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logPolyLine(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logPolySegment(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logPolyRectangle(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logPolyArc(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logFillPoly(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logPolyFillRectangle(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logPolyFillArc(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
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

size_t X11ProtocolParser::_logListInstalledColormaps(
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

size_t X11ProtocolParser::_logListExtensions(
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

size_t X11ProtocolParser::_logGetKeyboardControl(
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

size_t X11ProtocolParser::_logGetPointerControl(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSetScreenSaver(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGetScreenSaver(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logChangeHosts(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logListHosts(
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

size_t X11ProtocolParser::_logGetPointerMapping(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logSetModifierMapping(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logGetModifierMapping(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logNoOperation(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

size_t X11ProtocolParser::_logClientRequest(
    Connection* conn, const uint8_t* data, const size_t sz, const uint8_t opcode ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= 4 ); // TBD
    // TBD core opcodes only to start
    assert( ( opcode >= 1 && opcode <= 119 ) || opcode == 127 );
    // map opcode to sequence number to aid in parsing request errors and replies
    conn->registerRequest( opcode );
    size_t bytes_parsed {};
    using namespace protocol::requests;
    switch ( opcode ) {
    case opcodes::CREATEWINDOW:
        bytes_parsed = _logCreateWindow( conn, data, sz );
        break;
    case opcodes::CHANGEWINDOWATTRIBUTES:
        bytes_parsed = _logChangeWindowAttributes( conn, data, sz );
        break;
    case opcodes::GETWINDOWATTRIBUTES:
        bytes_parsed = _logGetWindowAttributes( conn, data, sz );
        break;
    case opcodes::DESTROYWINDOW:
        bytes_parsed = _logDestroyWindow( conn, data, sz );
        break;
    case opcodes::DESTROYSUBWINDOWS:
        bytes_parsed = _logDestroySubwindows( conn, data, sz );
        break;
    case opcodes::CHANGESAVESET:
        bytes_parsed = _logChangeSaveSet( conn, data, sz );
        break;
    case opcodes::REPARENTWINDOW:
        bytes_parsed = _logReparentWindow( conn, data, sz );
        break;
    case opcodes::MAPWINDOW:
        bytes_parsed = _logMapWindow( conn, data, sz );
        break;
    case opcodes::MAPSUBWINDOWS:
        bytes_parsed = _logMapSubwindows( conn, data, sz );
        break;
    case opcodes::UNMAPWINDOW:
        bytes_parsed = _logUnmapWindow( conn, data, sz );
        break;
    case opcodes::UNMAPSUBWINDOWS:
        bytes_parsed = _logUnmapSubwindows( conn, data, sz );
        break;
    case opcodes::CONFIGUREWINDOW:
        bytes_parsed = _logConfigureWindow( conn, data, sz );
        break;
    case opcodes::CIRCULATEWINDOW:
        bytes_parsed = _logCirculateWindow( conn, data, sz );
        break;
    case opcodes::GETGEOMETRY:
        bytes_parsed = _logGetGeometry( conn, data, sz );
        break;
    case opcodes::QUERYTREE:
        bytes_parsed = _logQueryTree( conn, data, sz );
        break;
    case opcodes::INTERNATOM:
        bytes_parsed = _logInternAtom( conn, data, sz );
        break;
    case opcodes::GETATOMNAME:
        bytes_parsed = _logGetAtomName( conn, data, sz );
        break;
    case opcodes::CHANGEPROPERTY:
        bytes_parsed = _logChangeProperty( conn, data, sz );
        break;
    case opcodes::DELETEPROPERTY:
        bytes_parsed = _logDeleteProperty( conn, data, sz );
        break;
    case opcodes::GETPROPERTY:
        bytes_parsed = _logGetProperty( conn, data, sz );
        break;
    case opcodes::LISTPROPERTIES:
        bytes_parsed = _logListProperties( conn, data, sz );
        break;
    case opcodes::SETSELECTIONOWNER:
        bytes_parsed = _logSetSelectionOwner( conn, data, sz );
        break;
    case opcodes::GETSELECTIONOWNER:
        bytes_parsed = _logGetSelectionOwner( conn, data, sz );
        break;
    case opcodes::CONVERTSELECTION:
        bytes_parsed = _logConvertSelection( conn, data, sz );
        break;
    case opcodes::SENDEVENT:
        bytes_parsed = _logSendEvent( conn, data, sz );
        break;
    case opcodes::GRABPOINTER:
        bytes_parsed = _logGrabPointer( conn, data, sz );
        break;
    case opcodes::UNGRABPOINTER:
        bytes_parsed = _logUngrabPointer( conn, data, sz );
        break;
    case opcodes::GRABBUTTON:
        bytes_parsed = _logGrabButton( conn, data, sz );
        break;
    case opcodes::UNGRABBUTTON:
        bytes_parsed = _logUngrabButton( conn, data, sz );
        break;
    case opcodes::CHANGEACTIVEPOINTERGRAB:
        bytes_parsed = _logChangeActivePointerGrab( conn, data, sz );
        break;
    case opcodes::GRABKEYBOARD:
        bytes_parsed = _logGrabKeyboard( conn, data, sz );
        break;
    case opcodes::UNGRABKEYBOARD:
        bytes_parsed = _logUngrabKeyboard( conn, data, sz );
        break;
    case opcodes::GRABKEY:
        bytes_parsed = _logGrabKey( conn, data, sz );
        break;
    case opcodes::UNGRABKEY:
        bytes_parsed = _logUngrabKey( conn, data, sz );
        break;
    case opcodes::ALLOWEVENTS:
        bytes_parsed = _logAllowEvents( conn, data, sz );
        break;
    case opcodes::GRABSERVER:
        bytes_parsed = _logGrabServer( conn, data, sz );
        break;
    case opcodes::UNGRABSERVER:
        bytes_parsed = _logUngrabServer( conn, data, sz );
        break;
    case opcodes::QUERYPOINTER:
        bytes_parsed = _logQueryPointer( conn, data, sz );
        break;
    case opcodes::GETMOTIONEVENTS:
        bytes_parsed = _logGetMotionEvents( conn, data, sz );
        break;
    case opcodes::TRANSLATECOORDINATES:
        bytes_parsed = _logTranslateCoordinates( conn, data, sz );
        break;
    case opcodes::WARPPOINTER:
        bytes_parsed = _logWarpPointer( conn, data, sz );
        break;
    case opcodes::SETINPUTFOCUS:
        bytes_parsed = _logSetInputFocus( conn, data, sz );
        break;
    case opcodes::GETINPUTFOCUS:
        bytes_parsed = _logGetInputFocus( conn, data, sz );
        break;
    case opcodes::QUERYKEYMAP:
        bytes_parsed = _logQueryKeymap( conn, data, sz );
        break;
    case opcodes::OPENFONT:
        bytes_parsed = _logOpenFont( conn, data, sz );
        break;
    case opcodes::CLOSEFONT:
        bytes_parsed = _logCloseFont( conn, data, sz );
        break;
    case opcodes::QUERYFONT:
        bytes_parsed = _logQueryFont( conn, data, sz );
        break;
    case opcodes::QUERYTEXTEXTENTS:
        bytes_parsed = _logQueryTextExtents( conn, data, sz );
        break;
    case opcodes::LISTFONTS:
        bytes_parsed = _logListFonts( conn, data, sz );
        break;
    case opcodes::LISTFONTSWITHINFO:
        bytes_parsed = _logListFontsWithInfo( conn, data, sz );
        break;
    case opcodes::SETFONTPATH:
        bytes_parsed = _logSetFontPath( conn, data, sz );
        break;
    case opcodes::GETFONTPATH:
        bytes_parsed = _logGetFontPath( conn, data, sz );
        break;
    case opcodes::CREATEPIXMAP:
        bytes_parsed = _logCreatePixmap( conn, data, sz );
        break;
    case opcodes::FREEPIXMAP:
        bytes_parsed = _logFreePixmap( conn, data, sz );
        break;
    case opcodes::CREATEGC:
        bytes_parsed = _logCreateGC( conn, data, sz );
        break;
    case opcodes::CHANGEGC:
        bytes_parsed = _logChangeGC( conn, data, sz );
        break;
    case opcodes::COPYGC:
        bytes_parsed = _logCopyGC( conn, data, sz );
        break;
    case opcodes::SETDASHES:
        bytes_parsed = _logSetDashes( conn, data, sz );
        break;
    case opcodes::SETCLIPRECTANGLES:
        bytes_parsed = _logSetClipRectangles( conn, data, sz );
        break;
    case opcodes::FREEGC:
        bytes_parsed = _logFreeGC( conn, data, sz );
        break;
    case opcodes::CLEARAREA:
        bytes_parsed = _logClearArea( conn, data, sz );
        break;
    case opcodes::COPYAREA:
        bytes_parsed = _logCopyArea( conn, data, sz );
        break;
    case opcodes::COPYPLANE:
        bytes_parsed = _logCopyPlane( conn, data, sz );
        break;
    case opcodes::POLYPOINT:
        bytes_parsed = _logPolyPoint( conn, data, sz );
        break;
    case opcodes::POLYLINE:
        bytes_parsed = _logPolyLine( conn, data, sz );
        break;
    case opcodes::POLYSEGMENT:
        bytes_parsed = _logPolySegment( conn, data, sz );
        break;
    case opcodes::POLYRECTANGLE:
        bytes_parsed = _logPolyRectangle( conn, data, sz );
        break;
    case opcodes::POLYARC:
        bytes_parsed = _logPolyArc( conn, data, sz );
        break;
    case opcodes::FILLPOLY:
        bytes_parsed = _logFillPoly( conn, data, sz );
        break;
    case opcodes::POLYFILLRECTANGLE:
        bytes_parsed = _logPolyFillRectangle( conn, data, sz );
        break;
    case opcodes::POLYFILLARC:
        bytes_parsed = _logPolyFillArc( conn, data, sz );
        break;
    case opcodes::PUTIMAGE:
        bytes_parsed = _logPutImage( conn, data, sz );
        break;
    case opcodes::GETIMAGE:
        bytes_parsed = _logGetImage( conn, data, sz );
        break;
    case opcodes::POLYTEXT8:
        bytes_parsed = _logPolyText8( conn, data, sz );
        break;
    case opcodes::POLYTEXT16:
        bytes_parsed = _logPolyText16( conn, data, sz );
        break;
    case opcodes::IMAGETEXT8:
        bytes_parsed = _logImageText8( conn, data, sz );
        break;
    case opcodes::IMAGETEXT16:
        bytes_parsed = _logImageText16( conn, data, sz );
        break;
    case opcodes::CREATECOLORMAP:
        bytes_parsed = _logCreateColormap( conn, data, sz );
        break;
    case opcodes::FREECOLORMAP:
        bytes_parsed = _logFreeColormap( conn, data, sz );
        break;
    case opcodes::COPYCOLORMAPANDFREE:
        bytes_parsed = _logCopyColormapAndFree( conn, data, sz );
        break;
    case opcodes::INSTALLCOLORMAP:
        bytes_parsed = _logInstallColormap( conn, data, sz );
        break;
    case opcodes::UNINSTALLCOLORMAP:
        bytes_parsed = _logUninstallColormap( conn, data, sz );
        break;
    case opcodes::LISTINSTALLEDCOLORMAPS:
        bytes_parsed = _logListInstalledColormaps( conn, data, sz );
        break;
    case opcodes::ALLOCCOLOR:
        bytes_parsed = _logAllocColor( conn, data, sz );
        break;
    case opcodes::ALLOCNAMEDCOLOR:
        bytes_parsed = _logAllocNamedColor( conn, data, sz );
        break;
    case opcodes::ALLOCCOLORCELLS:
        bytes_parsed = _logAllocColorCells( conn, data, sz );
        break;
    case opcodes::ALLOCCOLORPLANES:
        bytes_parsed = _logAllocColorPlanes( conn, data, sz );
        break;
    case opcodes::FREECOLORS:
        bytes_parsed = _logFreeColors( conn, data, sz );
        break;
    case opcodes::STORECOLORS:
        bytes_parsed = _logStoreColors( conn, data, sz );
        break;
    case opcodes::STORENAMEDCOLOR:
        bytes_parsed = _logStoreNamedColor( conn, data, sz );
        break;
    case opcodes::QUERYCOLORS:
        bytes_parsed = _logQueryColors( conn, data, sz );
        break;
    case opcodes::LOOKUPCOLOR:
        bytes_parsed = _logLookupColor( conn, data, sz );
        break;
    case opcodes::CREATECURSOR:
        bytes_parsed = _logCreateCursor( conn, data, sz );
        break;
    case opcodes::CREATEGLYPHCURSOR:
        bytes_parsed = _logCreateGlyphCursor( conn, data, sz );
        break;
    case opcodes::FREECURSOR:
        bytes_parsed = _logFreeCursor( conn, data, sz );
        break;
    case opcodes::RECOLORCURSOR:
        bytes_parsed = _logRecolorCursor( conn, data, sz );
        break;
    case opcodes::QUERYBESTSIZE:
        bytes_parsed = _logQueryBestSize( conn, data, sz );
        break;
    case opcodes::QUERYEXTENSION:
        bytes_parsed = _logQueryExtension( conn, data, sz );
        break;
    case opcodes::LISTEXTENSIONS:
        bytes_parsed = _logListExtensions( conn, data, sz );
        break;
    case opcodes::CHANGEKEYBOARDMAPPING:
        bytes_parsed = _logChangeKeyboardMapping( conn, data, sz );
        break;
    case opcodes::GETKEYBOARDMAPPING:
        bytes_parsed = _logGetKeyboardMapping( conn, data, sz );
        break;
    case opcodes::CHANGEKEYBOARDCONTROL:
        bytes_parsed = _logChangeKeyboardControl( conn, data, sz );
        break;
    case opcodes::GETKEYBOARDCONTROL:
        bytes_parsed = _logGetKeyboardControl( conn, data, sz );
        break;
    case opcodes::BELL:
        bytes_parsed = _logBell( conn, data, sz );
        break;
    case opcodes::CHANGEPOINTERCONTROL:
        bytes_parsed = _logChangePointerControl( conn, data, sz );
        break;
    case opcodes::GETPOINTERCONTROL:
        bytes_parsed = _logGetPointerControl( conn, data, sz );
        break;
    case opcodes::SETSCREENSAVER:
        bytes_parsed = _logSetScreenSaver( conn, data, sz );
        break;
    case opcodes::GETSCREENSAVER:
        bytes_parsed = _logGetScreenSaver( conn, data, sz );
        break;
    case opcodes::CHANGEHOSTS:
        bytes_parsed = _logChangeHosts( conn, data, sz );
        break;
    case opcodes::LISTHOSTS:
        bytes_parsed = _logListHosts( conn, data, sz );
        break;
    case opcodes::SETACCESSCONTROL:
        bytes_parsed = _logSetAccessControl( conn, data, sz );
        break;
    case opcodes::SETCLOSEDOWNMODE:
        bytes_parsed = _logSetCloseDownMode( conn, data, sz );
        break;
    case opcodes::KILLCLIENT:
        bytes_parsed = _logKillClient( conn, data, sz );
        break;
    case opcodes::ROTATEPROPERTIES:
        bytes_parsed = _logRotateProperties( conn, data, sz );
        break;
    case opcodes::FORCESCREENSAVER:
        bytes_parsed = _logForceScreenSaver( conn, data, sz );
        break;
    case opcodes::SETPOINTERMAPPING:
        bytes_parsed = _logSetPointerMapping( conn, data, sz );
        break;
    case opcodes::GETPOINTERMAPPING:
        bytes_parsed = _logGetPointerMapping( conn, data, sz );
        break;
    case opcodes::SETMODIFIERMAPPING:
        bytes_parsed = _logSetModifierMapping( conn, data, sz );
        break;
    case opcodes::GETMODIFIERMAPPING:
        bytes_parsed = _logGetModifierMapping( conn, data, sz );
        break;
    case opcodes::NOOPERATION:
        bytes_parsed = _logNoOperation( conn, data, sz );
        break;
    default:
        break;
    };
    // return bytes_parsed;
    return sz;  // TBD testing only
}
