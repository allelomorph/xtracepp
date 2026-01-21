#include <unordered_map>
#include <cstdint>

#include "X11ProtocolParser.hpp"
#include "protocol/events.hpp"


using namespace protocol::events;

const std::unordered_map< uint8_t, X11ProtocolParser::_EventCodeTraits >
X11ProtocolParser::_core_events {
    { codes::KEYPRESS,          //  2
      _EventCodeTraits( names.at( codes::KEYPRESS ),
                        &X11ProtocolParser::_parseEvent< KeyPress > ) },
    { codes::KEYRELEASE,        //  3
      _EventCodeTraits( names.at( codes::KEYRELEASE ),
                        &X11ProtocolParser::_parseEvent< KeyRelease > ) },
    { codes::BUTTONPRESS,       //  4
      _EventCodeTraits( names.at( codes::BUTTONPRESS ),
                        &X11ProtocolParser::_parseEvent< ButtonPress > ) },
    { codes::BUTTONRELEASE,     //  5
      _EventCodeTraits( names.at( codes::BUTTONRELEASE ),
                        &X11ProtocolParser::_parseEvent< ButtonRelease > ) },
    { codes::MOTIONNOTIFY,      //  6
      _EventCodeTraits( names.at( codes::MOTIONNOTIFY ),
                        &X11ProtocolParser::_parseEvent< MotionNotify > ) },
    { codes::ENTERNOTIFY,       //  7
      _EventCodeTraits( names.at( codes::ENTERNOTIFY ),
                        &X11ProtocolParser::_parseEvent< EnterNotify > ) },
    { codes::LEAVENOTIFY,       //  8
      _EventCodeTraits( names.at( codes::LEAVENOTIFY ),
                        &X11ProtocolParser::_parseEvent< LeaveNotify > ) },
    { codes::FOCUSIN,           //  9
      _EventCodeTraits( names.at( codes::FOCUSIN ),
                        &X11ProtocolParser::_parseEvent< FocusIn > ) },
    { codes::FOCUSOUT,          // 10
      _EventCodeTraits( names.at( codes::FOCUSOUT ),
                        &X11ProtocolParser::_parseEvent< FocusOut > ) },
    { codes::KEYMAPNOTIFY,      // 11
      _EventCodeTraits( names.at( codes::KEYMAPNOTIFY ),
                        &X11ProtocolParser::_parseEvent< KeymapNotify > ) },
    { codes::EXPOSE,            // 12
      _EventCodeTraits( names.at( codes::EXPOSE ),
                        &X11ProtocolParser::_parseEvent< Expose > ) },
    { codes::GRAPHICSEXPOSURE,  // 13
      _EventCodeTraits( names.at( codes::GRAPHICSEXPOSURE ),
                        &X11ProtocolParser::_parseEvent< GraphicsExposure > ) },
    { codes::NOEXPOSURE,        // 14
      _EventCodeTraits( names.at( codes::NOEXPOSURE ),
                        &X11ProtocolParser::_parseEvent< NoExposure > ) },
    { codes::VISIBILITYNOTIFY,  // 15
      _EventCodeTraits( names.at( codes::VISIBILITYNOTIFY ),
                        &X11ProtocolParser::_parseEvent< VisibilityNotify > ) },
    { codes::CREATENOTIFY,      // 16
      _EventCodeTraits( names.at( codes::CREATENOTIFY ),
                        &X11ProtocolParser::_parseEvent< CreateNotify > ) },
    { codes::DESTROYNOTIFY,     // 17
      _EventCodeTraits( names.at( codes::DESTROYNOTIFY ),
                        &X11ProtocolParser::_parseEvent< DestroyNotify > ) },
    { codes::UNMAPNOTIFY,       // 18
      _EventCodeTraits( names.at( codes::UNMAPNOTIFY ),
                        &X11ProtocolParser::_parseEvent< UnmapNotify > ) },
    { codes::MAPNOTIFY,         // 19
      _EventCodeTraits( names.at( codes::MAPNOTIFY ),
                        &X11ProtocolParser::_parseEvent< MapNotify > ) },
    { codes::MAPREQUEST,        // 20
      _EventCodeTraits( names.at( codes::MAPREQUEST ),
                        &X11ProtocolParser::_parseEvent< MapRequest > ) },
    { codes::REPARENTNOTIFY,    // 21
      _EventCodeTraits( names.at( codes::REPARENTNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ReparentNotify > ) },
    { codes::CONFIGURENOTIFY,   // 22
      _EventCodeTraits( names.at( codes::CONFIGURENOTIFY ),
                        &X11ProtocolParser::_parseEvent< ConfigureNotify > ) },
    { codes::CONFIGUREREQUEST,  // 23
      _EventCodeTraits( names.at( codes::CONFIGUREREQUEST ),
                        &X11ProtocolParser::_parseEvent< ConfigureRequest > ) },
    { codes::GRAVITYNOTIFY,     // 24
      _EventCodeTraits( names.at( codes::GRAVITYNOTIFY ),
                        &X11ProtocolParser::_parseEvent< GravityNotify > ) },
    { codes::RESIZEREQUEST,     // 25
      _EventCodeTraits( names.at( codes::RESIZEREQUEST ),
                        &X11ProtocolParser::_parseEvent< ResizeRequest > ) },
    { codes::CIRCULATENOTIFY,   // 26
      _EventCodeTraits( names.at( codes::CIRCULATENOTIFY ),
                        &X11ProtocolParser::_parseEvent< CirculateNotify > ) },
    { codes::CIRCULATEREQUEST,  // 27
      _EventCodeTraits( names.at( codes::CIRCULATEREQUEST ),
                        &X11ProtocolParser::_parseEvent< CirculateRequest > ) },
    { codes::PROPERTYNOTIFY,    // 28
      _EventCodeTraits( names.at( codes::PROPERTYNOTIFY ),
                        &X11ProtocolParser::_parseEvent< PropertyNotify > ) },
    { codes::SELECTIONCLEAR,    // 29
      _EventCodeTraits( names.at( codes::SELECTIONCLEAR ),
                        &X11ProtocolParser::_parseEvent< SelectionClear > ) },
    { codes::SELECTIONREQUEST,  // 30
      _EventCodeTraits( names.at( codes::SELECTIONREQUEST ),
                        &X11ProtocolParser::_parseEvent< SelectionRequest > ) },
    { codes::SELECTIONNOTIFY,   // 31
      _EventCodeTraits( names.at( codes::SELECTIONNOTIFY ),
                        &X11ProtocolParser::_parseEvent< SelectionNotify > ) },
    { codes::COLORMAPNOTIFY,    // 32
      _EventCodeTraits( names.at( codes::COLORMAPNOTIFY ),
                        &X11ProtocolParser::_parseEvent< ColormapNotify > ) },
    { codes::CLIENTMESSAGE,     // 33
      _EventCodeTraits( names.at( codes::CLIENTMESSAGE ),
                        &X11ProtocolParser::_parseEvent< ClientMessage > ) },
    { codes::MAPPINGNOTIFY,     // 34
      _EventCodeTraits( names.at( codes::MAPPINGNOTIFY ),
                        &X11ProtocolParser::_parseEvent< MappingNotify > ) },
};
