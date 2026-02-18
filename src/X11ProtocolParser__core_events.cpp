#include <unordered_map>

#include <cstdint>

#include "X11ProtocolParser.hpp"

#include "protocol/events.hpp"


namespace ev = protocol::events;

const std::unordered_map< uint8_t, X11ProtocolParser::_EventCodeTraits >
X11ProtocolParser::_core_events {
    { ev::codes::KEYPRESS,          //  2
      _EventCodeTraits( ev::names.at( ev::codes::KEYPRESS ),
                        &X11ProtocolParser::_parseEvent< ev::KeyPress > ) },
    { ev::codes::KEYRELEASE,        //  3
      _EventCodeTraits( ev::names.at( ev::codes::KEYRELEASE ),
                        &X11ProtocolParser::_parseEvent< ev::KeyRelease > ) },
    { ev::codes::BUTTONPRESS,       //  4
      _EventCodeTraits( ev::names.at( ev::codes::BUTTONPRESS ),
                        &X11ProtocolParser::_parseEvent< ev::ButtonPress > ) },
    { ev::codes::BUTTONRELEASE,     //  5
      _EventCodeTraits( ev::names.at( ev::codes::BUTTONRELEASE ),
                        &X11ProtocolParser::_parseEvent< ev::ButtonRelease > ) },
    { ev::codes::MOTIONNOTIFY,      //  6
      _EventCodeTraits( ev::names.at( ev::codes::MOTIONNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::MotionNotify > ) },
    { ev::codes::ENTERNOTIFY,       //  7
      _EventCodeTraits( ev::names.at( ev::codes::ENTERNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::EnterNotify > ) },
    { ev::codes::LEAVENOTIFY,       //  8
      _EventCodeTraits( ev::names.at( ev::codes::LEAVENOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::LeaveNotify > ) },
    { ev::codes::FOCUSIN,           //  9
      _EventCodeTraits( ev::names.at( ev::codes::FOCUSIN ),
                        &X11ProtocolParser::_parseEvent< ev::FocusIn > ) },
    { ev::codes::FOCUSOUT,          // 10
      _EventCodeTraits( ev::names.at( ev::codes::FOCUSOUT ),
                        &X11ProtocolParser::_parseEvent< ev::FocusOut > ) },
    { ev::codes::KEYMAPNOTIFY,      // 11
      _EventCodeTraits( ev::names.at( ev::codes::KEYMAPNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::KeymapNotify > ) },
    { ev::codes::EXPOSE,            // 12
      _EventCodeTraits( ev::names.at( ev::codes::EXPOSE ),
                        &X11ProtocolParser::_parseEvent< ev::Expose > ) },
    { ev::codes::GRAPHICSEXPOSURE,  // 13
      _EventCodeTraits( ev::names.at( ev::codes::GRAPHICSEXPOSURE ),
                        &X11ProtocolParser::_parseEvent< ev::GraphicsExposure > ) },
    { ev::codes::NOEXPOSURE,        // 14
      _EventCodeTraits( ev::names.at( ev::codes::NOEXPOSURE ),
                        &X11ProtocolParser::_parseEvent< ev::NoExposure > ) },
    { ev::codes::VISIBILITYNOTIFY,  // 15
      _EventCodeTraits( ev::names.at( ev::codes::VISIBILITYNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::VisibilityNotify > ) },
    { ev::codes::CREATENOTIFY,      // 16
      _EventCodeTraits( ev::names.at( ev::codes::CREATENOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::CreateNotify > ) },
    { ev::codes::DESTROYNOTIFY,     // 17
      _EventCodeTraits( ev::names.at( ev::codes::DESTROYNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::DestroyNotify > ) },
    { ev::codes::UNMAPNOTIFY,       // 18
      _EventCodeTraits( ev::names.at( ev::codes::UNMAPNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::UnmapNotify > ) },
    { ev::codes::MAPNOTIFY,         // 19
      _EventCodeTraits( ev::names.at( ev::codes::MAPNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::MapNotify > ) },
    { ev::codes::MAPREQUEST,        // 20
      _EventCodeTraits( ev::names.at( ev::codes::MAPREQUEST ),
                        &X11ProtocolParser::_parseEvent< ev::MapRequest > ) },
    { ev::codes::REPARENTNOTIFY,    // 21
      _EventCodeTraits( ev::names.at( ev::codes::REPARENTNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::ReparentNotify > ) },
    { ev::codes::CONFIGURENOTIFY,   // 22
      _EventCodeTraits( ev::names.at( ev::codes::CONFIGURENOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::ConfigureNotify > ) },
    { ev::codes::CONFIGUREREQUEST,  // 23
      _EventCodeTraits( ev::names.at( ev::codes::CONFIGUREREQUEST ),
                        &X11ProtocolParser::_parseEvent< ev::ConfigureRequest > ) },
    { ev::codes::GRAVITYNOTIFY,     // 24
      _EventCodeTraits( ev::names.at( ev::codes::GRAVITYNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::GravityNotify > ) },
    { ev::codes::RESIZEREQUEST,     // 25
      _EventCodeTraits( ev::names.at( ev::codes::RESIZEREQUEST ),
                        &X11ProtocolParser::_parseEvent< ev::ResizeRequest > ) },
    { ev::codes::CIRCULATENOTIFY,   // 26
      _EventCodeTraits( ev::names.at( ev::codes::CIRCULATENOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::CirculateNotify > ) },
    { ev::codes::CIRCULATEREQUEST,  // 27
      _EventCodeTraits( ev::names.at( ev::codes::CIRCULATEREQUEST ),
                        &X11ProtocolParser::_parseEvent< ev::CirculateRequest > ) },
    { ev::codes::PROPERTYNOTIFY,    // 28
      _EventCodeTraits( ev::names.at( ev::codes::PROPERTYNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::PropertyNotify > ) },
    { ev::codes::SELECTIONCLEAR,    // 29
      _EventCodeTraits( ev::names.at( ev::codes::SELECTIONCLEAR ),
                        &X11ProtocolParser::_parseEvent< ev::SelectionClear > ) },
    { ev::codes::SELECTIONREQUEST,  // 30
      _EventCodeTraits( ev::names.at( ev::codes::SELECTIONREQUEST ),
                        &X11ProtocolParser::_parseEvent< ev::SelectionRequest > ) },
    { ev::codes::SELECTIONNOTIFY,   // 31
      _EventCodeTraits( ev::names.at( ev::codes::SELECTIONNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::SelectionNotify > ) },
    { ev::codes::COLORMAPNOTIFY,    // 32
      _EventCodeTraits( ev::names.at( ev::codes::COLORMAPNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::ColormapNotify > ) },
    { ev::codes::CLIENTMESSAGE,     // 33
      _EventCodeTraits( ev::names.at( ev::codes::CLIENTMESSAGE ),
                        &X11ProtocolParser::_parseEvent< ev::ClientMessage > ) },
    { ev::codes::MAPPINGNOTIFY,     // 34
      _EventCodeTraits( ev::names.at( ev::codes::MAPPINGNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ev::MappingNotify > ) },
};
