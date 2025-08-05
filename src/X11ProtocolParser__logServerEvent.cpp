#include <string_view>
//#include <unordered_set>

//#include <cctype>      // isprint
#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
//#include "Settings.hpp"
//#include "protocol/common_types.hpp"
//#include "protocol/requests.hpp"
#include "protocol/events.hpp"


size_t X11ProtocolParser::_logKeyPress(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logKeyRelease(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logButtonPress(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logButtonRelease(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logMotionNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logEnterNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logLeaveNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logFocusIn(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logFocusOut(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logKeymapNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logExpose(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logGraphicsExposure(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logNoExposure(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logVisibilityNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logCreateNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logDestroyNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logUnmapNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logMapNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logMapRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logReparentNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logConfigureNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logConfigureRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logGravityNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logResizeRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logCirculateNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logCirculateRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logPropertyNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logSelectionClear(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logSelectionRequest(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logSelectionNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logColormapNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logClientMessage(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logMappingNotify(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    return {};
}

size_t X11ProtocolParser::_logServerEvent(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    //assert( sz >= protocol::events::ENCODING_SZ ); // TBD

    const uint8_t code { *data };
    assert( code >= protocol::events::codes::MIN &&
            code <= protocol::events::codes::MAX );
    size_t bytes_parsed {};
    fmt::print( _log_fs, "{:03d}:<:server event {:>3d}: {} ",
                conn->id, code, protocol::events::names[ code ] );
    switch ( code ) {
    case protocol::events::codes::KEYPRESS:          //  2
        bytes_parsed = _logKeyPress( conn, data, sz );
        break;
    case protocol::events::codes::KEYRELEASE:        //  3
        bytes_parsed = _logKeyRelease( conn, data, sz );
        break;
    case protocol::events::codes::BUTTONPRESS:       //  4
        bytes_parsed = _logButtonPress( conn, data, sz );
        break;
    case protocol::events::codes::BUTTONRELEASE:     //  5
        bytes_parsed = _logButtonRelease( conn, data, sz );
        break;
    case protocol::events::codes::MOTIONNOTIFY:      //  6
        bytes_parsed = _logMotionNotify( conn, data, sz );
        break;
    case protocol::events::codes::ENTERNOTIFY:       //  7
        bytes_parsed = _logEnterNotify( conn, data, sz );
        break;
    case protocol::events::codes::LEAVENOTIFY:       //  8
        bytes_parsed = _logLeaveNotify( conn, data, sz );
        break;
    case protocol::events::codes::FOCUSIN:           //  9
        bytes_parsed = _logFocusIn( conn, data, sz );
        break;
    case protocol::events::codes::FOCUSOUT:          // 10
        bytes_parsed = _logFocusOut( conn, data, sz );
        break;
    case protocol::events::codes::KEYMAPNOTIFY:      // 11
        bytes_parsed = _logKeymapNotify( conn, data, sz );
        break;
    case protocol::events::codes::EXPOSE:            // 12
        bytes_parsed = _logExpose( conn, data, sz );
        break;
    case protocol::events::codes::GRAPHICSEXPOSURE:  // 13
        bytes_parsed = _logGraphicsExposure( conn, data, sz );
        break;
    case protocol::events::codes::NOEXPOSURE:        // 14
        bytes_parsed = _logNoExposure( conn, data, sz );
        break;
    case protocol::events::codes::VISIBILITYNOTIFY:  // 15
        bytes_parsed = _logVisibilityNotify( conn, data, sz );
        break;
    case protocol::events::codes::CREATENOTIFY:      // 16
        bytes_parsed = _logCreateNotify( conn, data, sz );
        break;
    case protocol::events::codes::DESTROYNOTIFY:     // 17
        bytes_parsed = _logDestroyNotify( conn, data, sz );
        break;
    case protocol::events::codes::UNMAPNOTIFY:       // 18
        bytes_parsed = _logUnmapNotify( conn, data, sz );
        break;
    case protocol::events::codes::MAPNOTIFY:         // 19
        bytes_parsed = _logMapNotify( conn, data, sz );
        break;
    case protocol::events::codes::MAPREQUEST:        // 20
        bytes_parsed = _logMapRequest( conn, data, sz );
        break;
    case protocol::events::codes::REPARENTNOTIFY:    // 21
        bytes_parsed = _logReparentNotify( conn, data, sz );
        break;
    case protocol::events::codes::CONFIGURENOTIFY:   // 22
        bytes_parsed = _logConfigureNotify( conn, data, sz );
        break;
    case protocol::events::codes::CONFIGUREREQUEST:  // 23
        bytes_parsed = _logConfigureRequest( conn, data, sz );
        break;
    case protocol::events::codes::GRAVITYNOTIFY:     // 24
        bytes_parsed = _logGravityNotify( conn, data, sz );
        break;
    case protocol::events::codes::RESIZEREQUEST:     // 25
        bytes_parsed = _logResizeRequest( conn, data, sz );
        break;
    case protocol::events::codes::CIRCULATENOTIFY:   // 26
        bytes_parsed = _logCirculateNotify( conn, data, sz );
        break;
    case protocol::events::codes::CIRCULATEREQUEST:  // 27
        bytes_parsed = _logCirculateRequest( conn, data, sz );
        break;
    case protocol::events::codes::PROPERTYNOTIFY:    // 28
        bytes_parsed = _logPropertyNotify( conn, data, sz );
        break;
    case protocol::events::codes::SELECTIONCLEAR:    // 29
        bytes_parsed = _logSelectionClear( conn, data, sz );
        break;
    case protocol::events::codes::SELECTIONREQUEST:  // 30
        bytes_parsed = _logSelectionRequest( conn, data, sz );
        break;
    case protocol::events::codes::SELECTIONNOTIFY:   // 31
        bytes_parsed = _logSelectionNotify( conn, data, sz );
        break;
    case protocol::events::codes::COLORMAPNOTIFY:    // 32
        bytes_parsed = _logColormapNotify( conn, data, sz );
        break;
    case protocol::events::codes::CLIENTMESSAGE:     // 33
        bytes_parsed = _logClientMessage( conn, data, sz );
        break;
    case protocol::events::codes::MAPPINGNOTIFY:     // 34
        bytes_parsed = _logMappingNotify( conn, data, sz );
        break;
    default:
        break;
    }
    assert( bytes_parsed == protocol::events::ENCODING_SZ );
    return bytes_parsed;
}
