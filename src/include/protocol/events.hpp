#ifndef PROTOCOL_EVENTS_HPP
#define PROTOCOL_EVENTS_HPP


#include <cstdint>
#include <array>

#include "protocol/common_types.hpp"

// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#event_format
// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Events
// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events

// server: single byte begins all messages: errors preceeded by 0, replies by 1, events by 2+
// TBD seems like errors and events are all 32 bytes, and since deciding code is
//   always first byte (derefable without cast,) we could better represent the standard by
//   including the codes in the structs
// !!! some replies exceed 32 bytes, as defined by request in protocol

namespace protocol {

namespace events {

namespace codes {

//TBD need to reconcile differences between code number enum and SETof(POINTER|DEVICE)EVENT flag offsets

enum Codes {
    KEYPRESS            =  2,
    KEYRELEASE,        //  3
    BUTTONPRESS,       //  4
    BUTTONRELEASE,     //  5
    MOTIONNOTIFY,      //  6
    ENTERNOTIFY,       //  7
    LEAVENOTIFY,       //  8
    FOCUSIN,           //  9
    FOCUSOUT,          // 10
    KEYMAPNOTIFY,      // 11
    EXPOSE,            // 12
    GRAPHICSEXPOSURE,  // 13
    NOEXPOSURE,        // 14
    VISIBILITYNOTIFY,  // 15
    CREATENOTIFY,      // 16
    DESTROYNOTIFY,     // 17
    UNMAPNOTIFY,       // 18
    MAPNOTIFY,         // 19
    MAPREQUEST,        // 20
    REPARENTNOTIFY,    // 21
    CONFIGURENOTIFY,   // 22
    CONFIGUREREQUEST,  // 23
    GRAVITYNOTIFY,     // 24
    RESIZEREQUEST,     // 25
    CIRCULATENOTIFY,   // 26
    CIRCULATEREQUEST,  // 27
    PROPERTYNOTIFY,    // 28
    SELECTIONCLEAR,    // 29
    SELECTIONREQUEST,  // 30
    SELECTIONNOTIFY,   // 31
    COLORMAPNOTIFY,    // 32
    CLIENTMESSAGE,     // 33
    MAPPINGNOTIFY,     // 34
};

inline constexpr
int MIN { KEYPRESS };

inline constexpr
int MAX { MAPPINGNOTIFY };

}  // namespace codes

inline constexpr
std::array< std::string_view, codes::MAX + 1 > names {
    "",                  //  0 (reserved for errors)
    "",                  //  1 (reserved for replies)
    "KeyPress",          //  2
    "KeyRelease",        //  3
    "ButtonPress",       //  4
    "ButtonRelease",     //  5
    "MotionNotify",      //  6
    "EnterNotify",       //  7
    "LeaveNotify",       //  8
    "FocusIn",           //  9
    "FocusOut",          // 10
    "KeymapNotify",      // 11
    "Expose",            // 12
    "GraphicsExposure",  // 13
    "NoExposure",        // 14
    "VisibilityNotify",  // 15
    "CreateNotify",      // 16
    "DestroyNotify",     // 17
    "UnmapNotify",       // 18
    "MapNotify",         // 19
    "MapRequest",        // 20
    "ReparentNotify",    // 21
    "ConfigureNotify",   // 22
    "ConfigureRequest",  // 23
    "GravityNotify",     // 24
    "ResizeRequest",     // 25
    "CirculateNotify",   // 26
    "CirculateRequest",  // 27
    "PropertyNotify",    // 28
    "SelectionClear",    // 29
    "SelectionRequest",  // 30
    "SelectionNotify",   // 31
    "ColormapNotify",    // 32
    "ClientMessage",     // 33
    "MappingNotify",     // 34
};

// TBD use shared type for KeyPress KeyRelease ButtonPress ButtonRelease, as all fields are same
struct KeyPress {
    struct [[gnu::packed]] Encoding {
        uint8_t         code;  // 2
        KEYCODE         detail;
        CARD16          sequence_number;  // sequence number
        TIMESTAMP       time;
        WINDOW          root;
        WINDOW          event;
        WINDOW          child;  // 0 None
        INT16           root_x;  // root-x
        INT16           root_y;  // root-y
        INT16           event_x;  // event-x
        INT16           event_y;  // event-y
        SETofKEYBUTMASK state;
        BOOL            same_screen;
    private:
        uint8_t         _unused;
    };

    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
};

struct KeyRelease {
    struct [[gnu::packed]] Encoding {
        uint8_t         code;  // 3
        KEYCODE         detail;
        CARD16          sequence_number;  // sequence number
        TIMESTAMP       time;
        WINDOW          root;
        WINDOW          event;
        WINDOW          child;  // 0 None
        INT16           root_x;  // root-x
        INT16           root_y;  // root-y
        INT16           event_x;  // event-x
        INT16           event_y;  // event-y
        SETofKEYBUTMASK state;
        BOOL            same_screen; // same-screen
    private:
        uint8_t         _unused;
    };

    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
};

struct ButtonPress {
    struct [[gnu::packed]] Encoding {
        uint8_t         code;  // 4
        KEYCODE         detail;
        CARD16          sequence_number;  // sequence number
        TIMESTAMP       time;
        WINDOW          root;
        WINDOW          event;
        WINDOW          child;  // 0 None
        INT16           root_x;  // root-x
        INT16           root_y;  // root-y
        INT16           event_x;  // event-x
        INT16           event_y;  // event-y
        SETofKEYBUTMASK state;
        BOOL            same_screen; // same-screen
    private:
        uint8_t         _unused;
    };

    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
};

struct ButtonRelease {
    struct [[gnu::packed]] Encoding {
        uint8_t         code;  // 5
        KEYCODE         detail;
        CARD16          sequence_number;  // sequence number
        TIMESTAMP       time;
        WINDOW          root;
        WINDOW          event;
        WINDOW          child;  // 0 None
        INT16           root_x;  // root-x
        INT16           root_y;  // root-y
        INT16           event_x;  // event-x
        INT16           event_y;  // event-y
        SETofKEYBUTMASK state;
        BOOL            same_screen; // same-screen
    private:
        uint8_t         _unused;
    };

    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
};

// TBD same as events 2-5 but different use of first byte `detail`
struct MotionNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t         code;  // 6
        uint8_t         detail;  // 0 Normal 1 Hint
        CARD16          sequence_number;  // sequence number
        TIMESTAMP       time;
        WINDOW          root;
        WINDOW          event;
        WINDOW          child;  // 0 None
        INT16           root_x;  // root-x
        INT16           root_y;  // root-y
        INT16           event_x;  // event-x
        INT16           event_y;  // event-y
        SETofKEYBUTMASK state;
        BOOL            same_screen; // same-screen
    private:
        uint8_t         _unused;
    };

    inline static const
    std::vector< std::string_view >& detail_names {
        protocol::enum_names::motion_hint };
    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
};
// TBD EnterNotify and LeaveNotify share same encoding
struct EnterNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t         code;  // 7
        uint8_t         detail;  // 0 Ancestor 1 Virtual 2 Inferior 3 Nonlinear 4 NonlinearVirtual
        CARD16          sequence_number;  // sequence number
        TIMESTAMP       time;
        WINDOW          root;
        WINDOW          event;
        WINDOW          child;  // 0 None
        INT16           root_x;  // root-x
        INT16           root_y;  // root-y
        INT16           event_x;  // event-x
        INT16           event_y;  // event-y
        SETofKEYBUTMASK state;
        uint8_t         mode;  // 0 Normal 1 Grab 2 Ungrab
        uint8_t         focus_same_screen; // same-screen, focus
        // #x01     focus (1 is True, 0 is False)
        // #x02     same-screen (1 is True, 0 is False)
        // #xFC     unused
    };

    inline static const
    std::vector< std::string_view >& detail_names {
        protocol::enum_names::focus_detail };  // TBD up to 4
    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& mode_names {
        protocol::enum_names::focus_mode };  // TBD up to 2
    inline static const
    std::vector< std::string_view >& focus_same_screen_names {
        protocol::enum_names::focus_same_screen_mask };
};

struct LeaveNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t         code;  // 8
        uint8_t         detail;  // 0 Ancestor 1 Virtual 2 Inferior 3 Nonlinear 4 NonlinearVirtual
        CARD16          sequence_number;  // sequence number
        TIMESTAMP       time;
        WINDOW          root;
        WINDOW          event;
        WINDOW          child;  // 0 None
        INT16           root_x;  // root-x
        INT16           root_y;  // root-y
        INT16           event_x;  // event-x
        INT16           event_y;  // event-y
        SETofKEYBUTMASK state;
        uint8_t         mode;  // 0 Normal 1 Grab 2 Ungrab
        // TBD make this a union? not clear on how it is used
        uint8_t         focus_same_screen; // same-screen, focus
        // #x01     focus (1 is True, 0 is False)
        // #x02     same-screen (1 is True, 0 is False)
        // #xFC     unused
    };

    inline static const
    std::vector< std::string_view >& detail_names {
        protocol::enum_names::focus_detail };  // TBD up to 4
    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& mode_names {
        protocol::enum_names::focus_mode };  // TBD up to 2
    inline static const
    std::vector< std::string_view >& focus_same_screen_names {
        protocol::enum_names::focus_same_screen_mask };
};

// TBD FocusIn/Out share encoding
struct FocusIn {
    struct [[gnu::packed]] Encoding {
        uint8_t     code;  // 9
        uint8_t     detail;  // 0 Ancestor 1 Virtual 2 Inferior 3 Nonlinear 4 NonlinearVirtual 5 Pointer 6 PointerRoot 7 None
        CARD16      sequence_number;  // sequence number
        WINDOW      event;
        uint8_t     mode;  // 0 Normal 1 Grab 2 Ungrab 3 WhileGrabbed
    private:
        uint8_t     _unused[23];
    };

    inline static const
    std::vector< std::string_view >& detail_names {
        protocol::enum_names::focus_detail };
    inline static const
    std::vector< std::string_view >& mode_names {
        protocol::enum_names::focus_mode };
};

struct FocusOut {
    struct [[gnu::packed]] Encoding {
        uint8_t     code;  // 10
        uint8_t     detail;  // 0 Ancestor 1 Virtual 2 Inferior 3 Nonlinear 4 NonlinearVirtual 5 Pointer 6 PointerRoot 7 None
        CARD16      sequence_number;  // sequence number
        WINDOW      event;
        uint8_t     mode;  // 0 Normal 1 Grab 2 Ungrab 3 WhileGrabbed
    private:
        uint8_t     _unused[23];
    };

    inline static const
    std::vector< std::string_view >& detail_names {
        protocol::enum_names::focus_detail };
    inline static const
    std::vector< std::string_view >& mode_names {
        protocol::enum_names::focus_mode };
};

struct KeymapNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 11
        CARD8      keys[31];  // LISTofCARD8 (byte for keycodes 0-7 is omitted)
    };
};

// TBD notice that x and y are unsigned
struct Expose {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 12
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     window;
        CARD16     x;
        CARD16     y;
        CARD16     width;
        CARD16     height;
        CARD16     count;
    private:
        uint8_t    _unused2[14];
    };
};

// TBD notice that x and y are unsigned
struct GraphicsExposure {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 13
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        DRAWABLE   drawable;
        CARD16     x;
        CARD16     y;
        CARD16     width;
        CARD16     height;
        CARD16     minor_opcode;  // minor-opcode
        CARD16     count;
        CARD8      major_opcode;  // major-opcode
    private:
        uint8_t    _unused2[11];
    };
};

struct NoExposure {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 14
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        DRAWABLE   drawable;
        CARD16     minor_opcode;  // minor-opcode
        CARD8      major_opcode;  // major-opcode
    private:
        uint8_t    _unused2[21];
    };
};

struct VisibilityNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 15
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     window;
        uint8_t    state;  // 0 Unobscured 1 PartiallyObscured 2 FullyObscured
    private:
        uint8_t    _unused2[23];
    };

    inline static const
    std::vector< std::string_view >& state_names {
        protocol::enum_names::visibility_state };
};

struct CreateNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 16
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     parent;
        WINDOW     window;
        INT16      x;
        INT16      y;
        CARD16     width;
        CARD16     height;
        CARD16     border_width;  // border-width
        BOOL       override_redirect;  // override-redirect
    private:
        uint8_t    _unused2[9];
    };
};

struct DestroyNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 17
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     event;
        WINDOW     window;
    private:
        uint8_t    _unused2[20];
    };
};

// TBD UnmapNotify and MapNotify are same aside from name of BOOL
struct UnmapNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 18
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     event;
        WINDOW     window;
        BOOL       from_configure;  // from-configure
    private:
        uint8_t    _unused2[19];
    };
};

struct MapNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 19
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     event;
        WINDOW     window;
        BOOL       override_redirect;  // override-redirect
    private:
        uint8_t    _unused2[19];
    };
};

struct MapRequest {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 20
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     parent;
        WINDOW     window;
    private:
        uint8_t    _unused2[20];
    };
};

struct ReparentNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 21
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     event;
        WINDOW     window;
        WINDOW     parent;
        INT16      x;
        INT16      y;
        BOOL       override_redirect;  // override-redirect
    private:
        uint8_t    _unused2[11];
    };
};

struct ConfigureNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 22
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     event;
        WINDOW     window;
        WINDOW     above_sibling;  // above-sibling // 0 None
        INT16      x;
        INT16      y;
        CARD16     width;
        CARD16     height;
        CARD16     border_width;  // border-width
        BOOL       override_redirect;  // override-redirect
    private:
        uint8_t    _unused2[5];
    };

    inline static const
    std::vector< std::string_view >& above_sibling_names {
        protocol::enum_names::zero_none };
};

struct ConfigureRequest {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 23
        uint8_t    stack_mode;  // stack-mode // 0 Above 1 Below 2 TopIf 3 BottomIf 4 Opposite
        CARD16     sequence_number;  // sequence number
        WINDOW     parent;
        WINDOW     window;
        WINDOW     sibling;  // 0 None
        INT16      x;
        INT16      y;
        CARD16     width;
        CARD16     height;
        CARD16     border_width;  // border-width
        // TBD what is this referring to if not request members?
        uint16_t   value_mask;  // 2B value-mask
        /*
          1 << 0  #x0001     x
          1 << 1  #x0002     y
          1 << 2  #x0004     width
          1 << 3  #x0008     height
          1 << 4  #x0010     border-width
          1 << 5  #x0020     sibling
          1 << 6  #x0040     stack-mode
        */
    private:
        uint8_t    _unused2[4];
    };

    inline static const
    std::vector< std::string_view >& stack_mode_names {
        protocol::enum_names::window_value_stack_mode };
    inline static const
    std::vector< std::string_view >& sibling_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& value_names {
        protocol::enum_names::window_value_mask };
};

struct GravityNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 24
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     event;
        WINDOW     window;
        INT16      x;
        INT16      y;
    private:
        uint8_t    _unused2[16];
    };
};

struct ResizeRequest {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 25
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     window;
        CARD16     width;
        CARD16     height;
    private:
        uint8_t    _unused2[20];
    };
};

// TBD functionally same as CirculateRequest
struct CirculateNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 26
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     event;
        WINDOW     window;
    private:
        WINDOW     _unused2;
    public:
        uint8_t    place;  // 0 Top 1 Bottom
    private:
        uint8_t    _unused3[15];
    };

    inline static const
    std::vector< std::string_view >& place_names {
        protocol::enum_names::circulate_place };
};

struct CirculateRequest {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 27
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     parent;
        WINDOW     window;
    private:
        uint32_t   _unused2;
    public:
        uint8_t    place;  // 0 Top 1 Bottom
    private:
        uint8_t    _unused3[15];
    };

    inline static const
    std::vector< std::string_view >& place_names {
        protocol::enum_names::circulate_place };
};

struct PropertyNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 28
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     window;
        ATOM       atom;
        TIMESTAMP  time;
        uint8_t    state;  // 0 NewValue 1 Deleted
    private:
        uint8_t    _unused2[15];
    };

    inline static const
    std::vector< std::string_view >& state_names {
        protocol::enum_names::property_state };
};

struct SelectionClear {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 29
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        TIMESTAMP  time;
        WINDOW     owner;
        ATOM       selection;
    private:
        uint8_t    _unused2[16];
    };
};

struct SelectionRequest {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 30
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        TIMESTAMP  time;  // 0 CurrentTime
        WINDOW     owner;
        WINDOW     requestor;
        ATOM       selection;
        ATOM       target;
        ATOM       property;  // 0 None
    private:
        uint8_t    _unused2[4];
    };

    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    inline static const
    std::vector< std::string_view >& property_names {
        protocol::enum_names::zero_none };
};

struct SelectionNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 31
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        TIMESTAMP  time;  // 0 CurrentTime
        WINDOW     requestor;
        ATOM       selection;
        ATOM       target;
        ATOM       property;  // 0 None
    private:
        uint8_t    _unused2[8];
    };

    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    inline static const
    std::vector< std::string_view >& property_names {
        protocol::enum_names::zero_none };
};

struct ColormapNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 32
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        WINDOW     window;
        COLORMAP   colormap;  // 0 None
        BOOL       new_;  // new
        uint8_t    state;  // 0 Uninstalled 1 Installed
    private:
        uint8_t    _unused2[18];
    };

    inline static const
    std::vector< std::string_view >& colormap_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& state_names {
        protocol::enum_names::colormap_state };
};

struct ClientMessage {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 33
        CARD8      format;
        CARD16     sequence_number;  // sequence number
        WINDOW     window;
        ATOM       type;
        uint8_t    data[20];
    };
};

struct MappingNotify {
    struct [[gnu::packed]] Encoding {
        uint8_t    code;  // 34
    private:
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        uint8_t    request;  // 0 Modifier 1 Keyboard 2 Pointer
        KEYCODE    first_keycode;  // first-keycode
        CARD8      count;
    private:
        uint8_t    _unused2[25];
    };

    inline static const
    std::vector< std::string_view >& request_names {
        protocol::enum_names::mapping_state };
};

static constexpr uint32_t ENCODING_SZ { 32 };
static_assert( sizeof( KeyPress::Encoding )         == ENCODING_SZ );
static_assert( sizeof( KeyRelease::Encoding )       == ENCODING_SZ );
static_assert( sizeof( ButtonPress::Encoding )      == ENCODING_SZ );
static_assert( sizeof( ButtonRelease::Encoding )    == ENCODING_SZ );
static_assert( sizeof( MotionNotify::Encoding )     == ENCODING_SZ );
static_assert( sizeof( EnterNotify::Encoding )      == ENCODING_SZ );
static_assert( sizeof( LeaveNotify::Encoding )      == ENCODING_SZ );
static_assert( sizeof( FocusIn::Encoding )          == ENCODING_SZ );
static_assert( sizeof( FocusOut::Encoding )         == ENCODING_SZ );
static_assert( sizeof( KeymapNotify::Encoding )     == ENCODING_SZ );
static_assert( sizeof( Expose::Encoding )           == ENCODING_SZ );
static_assert( sizeof( GraphicsExposure::Encoding ) == ENCODING_SZ );
static_assert( sizeof( NoExposure::Encoding )       == ENCODING_SZ );
static_assert( sizeof( VisibilityNotify::Encoding ) == ENCODING_SZ );
static_assert( sizeof( CreateNotify::Encoding )     == ENCODING_SZ );
static_assert( sizeof( DestroyNotify::Encoding )    == ENCODING_SZ );
static_assert( sizeof( UnmapNotify::Encoding )      == ENCODING_SZ );
static_assert( sizeof( MapNotify::Encoding )        == ENCODING_SZ );
static_assert( sizeof( MapRequest::Encoding )       == ENCODING_SZ );
static_assert( sizeof( ReparentNotify::Encoding )   == ENCODING_SZ );
static_assert( sizeof( ConfigureNotify::Encoding )  == ENCODING_SZ );
static_assert( sizeof( ConfigureRequest::Encoding ) == ENCODING_SZ );
static_assert( sizeof( GravityNotify::Encoding )    == ENCODING_SZ );
static_assert( sizeof( ResizeRequest::Encoding )    == ENCODING_SZ );
static_assert( sizeof( CirculateNotify::Encoding )  == ENCODING_SZ );
static_assert( sizeof( CirculateRequest::Encoding ) == ENCODING_SZ );
static_assert( sizeof( PropertyNotify::Encoding )   == ENCODING_SZ );
static_assert( sizeof( SelectionClear::Encoding )   == ENCODING_SZ );
static_assert( sizeof( SelectionRequest::Encoding ) == ENCODING_SZ );
static_assert( sizeof( SelectionNotify::Encoding )  == ENCODING_SZ );
static_assert( sizeof( ColormapNotify::Encoding )   == ENCODING_SZ );
static_assert( sizeof( ClientMessage::Encoding )    == ENCODING_SZ );
static_assert( sizeof( MappingNotify::Encoding )    == ENCODING_SZ );

}  // namespace events

}  // namespace protocol


#endif  // PROTOCOL_EVENTS_HPP
