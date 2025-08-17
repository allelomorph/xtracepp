#include <string_view>
#include <tuple>
#include <unordered_map>

#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/requests.hpp"


// template <>
// X11ProtocolParser::_ParsingOutputs
// X11ProtocolParser::_parseEvent< protocol::events::SelectionRequest >(
//     Connection* conn, const uint8_t* data,
//     const X11ProtocolParser::_EventFormat format /*= _EventFormat::NORMAL*/ ) {
//     assert( conn != nullptr );
//     assert( data != nullptr );
// //    assert( sz >= sizeof( SelectionRequest::Encoding ) ); // TBD

//     _ParsingOutputs outputs {};
//     using protocol::events::SelectionRequest;
//     const SelectionRequest::Encoding* encoding {
//         reinterpret_cast< const SelectionRequest::Encoding* >( data ) };
//     outputs.bytes_parsed += sizeof( SelectionRequest::Encoding );
//     assert( encoding->code == protocol::events::codes::SELECTIONREQUEST );

//     const uint32_t tab_ct (
//         ( format == _EventFormat::SEND_EVENT ) ? 1 : 0 );
//     const std::string_view struct_indent {
//         _multiline ? _tabIndent( tab_ct ) : "" };
//     const std::string_view memb_indent {
//         _multiline ? _tabIndent( tab_ct + 1 ) : "" };
//     const uint32_t name_width (
//         _multiline ? sizeof( "sequence number" ) - 1 : 0 );
//     outputs.str = fmt::format(
//         "{}({}) {{{}"
//         "{}{}"
//         "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
//         "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
//         "{}}}",
//         protocol::events::names[encoding->code], encoding->code,
//         _separator,
//         !_verbose ? "" :
//         fmt::format(
//             "{}{: <{}}{}{}{}",
//             memb_indent, "code", name_width, _equals,
//             _formatInteger( encoding->code ), _separator ),
//         !_verbose ? "" :
//         fmt::format(
//             "{}{: <{}}{}{}{}",
//             memb_indent, "sequence number", name_width, _equals,
//             _formatInteger( encoding->sequence_number ), _separator ),
//         memb_indent, "time", name_width, _equals,
//         _formatCommonType( encoding->time, SelectionRequest::time_names ), _separator,
//         memb_indent, "owner", name_width, _equals,
//         _formatCommonType( encoding->owner ), _separator,
//         memb_indent, "requestor", name_width, _equals,
//         _formatCommonType( encoding->requestor ), _separator,
//         memb_indent, "selection", name_width, _equals,
//         _formatCommonType( encoding->selection ), _separator,
//         memb_indent, "target", name_width, _equals,
//         _formatCommonType( encoding->target ), _separator,
//         memb_indent, "property", name_width, _equals,
//         _formatCommonType( encoding->property, SelectionRequest::property_names ), _separator,
//         struct_indent
//         );
//     return outputs;
// }


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
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::InternAtom >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetAtomName >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetProperty >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::ListProperties >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetSelectionOwner >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GrabPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GrabKeyboard >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryPointer >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetMotionEvents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::TranslateCoordinates >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetInputFocus >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryKeymap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryFont >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryTextExtents >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::ListFonts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::ListFontsWithInfo >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetFontPath >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetImage >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::ListInstalledColormaps >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::AllocColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::AllocNamedColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::AllocColorCells >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::AllocColorPlanes >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryColors >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::LookupColor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryBestSize >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::QueryExtension >(
        Connection* conn, uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::ListExtensions >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetKeyboardMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetKeyboardControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetPointerControl >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetScreenSaver >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::ListHosts >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::SetPointerMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetPointerMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::SetModifierMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
}

template <>
size_t X11ProtocolParser::_logServerReply<
    protocol::requests::GetModifierMapping >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return {};
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
