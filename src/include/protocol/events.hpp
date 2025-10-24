#ifndef PROTOCOL_EVENTS_HPP
#define PROTOCOL_EVENTS_HPP


#include <cstdint>
#include <array>

#include "protocol/Response.hpp"
#include "protocol/common_types.hpp"

// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#event_format
// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Events
// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events

namespace protocol {

namespace events {

struct Event : public Response {
    struct [[gnu::packed]] Header {
        uint8_t code;
    private:
        uint8_t _unused;
    public:
        CARD16  sequence_num;
    };
    struct [[gnu::packed]] Encoding {
        Header  header;
    private:
        uint8_t _unused[28];
    };

    static constexpr uint32_t ENCODING_SZ { 32 };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    virtual ~Event() = 0;
};

namespace codes {

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

namespace impl {

struct InputEvent : public Event {
    struct [[gnu::packed]] Header {
        uint8_t         code;
        KEYCODE         detail;
        CARD16          sequence_num;  // sequence number
    };
    struct [[gnu::packed]] Encoding {
        Header          header;
        TIMESTAMP       time;
        WINDOW          root;
        WINDOW          event;
        WINDOW          child;         // 0 None
        INT16           root_x;        // root-x
        INT16           root_y;        // root-y
        INT16           event_x;       // event-x
        INT16           event_y;       // event-y
        SETofKEYBUTMASK state;
        BOOL            same_screen;
    private:
        uint8_t         _unused;
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };

    virtual ~InputEvent() = 0;
};

struct BoundaryEvent : public Event {
    struct [[gnu::packed]] Header {
        uint8_t         code;
        uint8_t         detail;        // 0 Ancestor 1 Virtual 2 Inferior 3 Nonlinear 4 NonlinearVirtual
        CARD16          sequence_num;  // sequence number
    };
    struct [[gnu::packed]] Encoding {
        Header          header;
        TIMESTAMP       time;
        WINDOW          root;
        WINDOW          event;
        WINDOW          child;         // 0 None
        INT16           root_x;        // root-x
        INT16           root_y;        // root-y
        INT16           event_x;       // event-x
        INT16           event_y;       // event-y
        SETofKEYBUTMASK state;
        uint8_t         mode;          // 0 Normal 1 Grab 2 Ungrab
        uint8_t         focus_same_screen_mask;
        //   0x01: focus        // 0 False 1 True
        //   0x02: same-screen  // 0 False 1 True
        //   0xFC: unused
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view >& detail_names {
        protocol::enum_names::focus_detail };
    static constexpr uint8_t DETAIL_ENUM_MAX { 4 };
    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& mode_names {
        protocol::enum_names::focus_mode };
    static constexpr uint8_t MODE_ENUM_MAX { 2 };
    inline static const
    std::vector< std::string_view >& focus_same_screen_names {
        protocol::enum_names::focus_same_screen_mask };
    static constexpr uint8_t FOCUS_SAME_SCREEN_ZERO_BITS { 0xFC };

    virtual ~BoundaryEvent() = 0;
};

struct FocusEvent : public Event {
    struct [[gnu::packed]] Header {
        uint8_t code;
        uint8_t detail;        // 0 Ancestor 1 Virtual 2 Inferior 3 Nonlinear 4 NonlinearVirtual 5 Pointer 6 PointerRoot 7 None
        CARD16  sequence_num;  // sequence number
    };
    struct [[gnu::packed]] Encoding {
        Header  header;
        WINDOW  event;
        uint8_t mode;  // 0 Normal 1 Grab 2 Ungrab 3 WhileGrabbed
    private:
        uint8_t _unused[23];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view >& detail_names {
        protocol::enum_names::focus_detail };
    inline static const
    std::vector< std::string_view >& mode_names {
        protocol::enum_names::focus_mode };

    virtual ~FocusEvent() = 0;
};

}  // namespace impl

struct KeyPress : public impl::InputEvent {
    // Header.code 2
};

struct KeyRelease : public impl::InputEvent {
    // Header.code 3
};

struct ButtonPress : public impl::InputEvent {
    // Header.code 4
};

struct ButtonRelease : public impl::InputEvent {
    // Header.code 5
};

struct MotionNotify : public Event {
    struct [[gnu::packed]] Header {
        uint8_t code;          // 6
        uint8_t detail;        // 0 Normal 1 Hint
        CARD16  sequence_num;  // sequence number
    };
    struct [[gnu::packed]] Encoding {
        Header          header;
        TIMESTAMP       time;
        WINDOW          root;
        WINDOW          event;
        WINDOW          child;         // 0 None
        INT16           root_x;        // root-x
        INT16           root_y;        // root-y
        INT16           event_x;       // event-x
        INT16           event_y;       // event-y
        SETofKEYBUTMASK state;
        BOOL            same_screen;
    private:
        uint8_t         _unused;
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view > detail_names {
        "Normal",  // 0
        "Hint"     // 1
    };
    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
};

struct EnterNotify : public impl::BoundaryEvent {
    // Header.code 7
};

struct LeaveNotify : public impl::BoundaryEvent {
    // Header.code 8
};

struct FocusIn : public impl::FocusEvent {
    // Header.code 9
};

struct FocusOut : public impl::FocusEvent {
    // Header.code 10
};

struct KeymapNotify : public Event {
    // TBD note lack of sequence number, needs exceptional parsing
    //   - maybe we could glean sequence from last event? "This event is
    //   reported to clients selecting KeymapState on a window and is generated
    //   immediately after every EnterNotify and FocusIn."
    struct [[gnu::packed]] Header {
        uint8_t code;     // 11
    };
    struct [[gnu::packed]] Encoding {
        Header header;
        CARD8  keys[31];  // LISTofCARD8 (byte for keycodes 0-7 is omitted)
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct Expose : public Event {
    struct [[gnu::packed]] Encoding {
        Header  header;   // header.code 12
        WINDOW  window;
        CARD16  x;
        CARD16  y;
        CARD16  width;
        CARD16  height;
        CARD16  count;
    private:
        uint8_t _unused[14];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct GraphicsExposure : public Event {
    // TBD major/minor opcode fields allow skipping lookup by conn id/seq
    struct [[gnu::packed]] Encoding {
        Header   header;        // header.code 13
        DRAWABLE drawable;
        CARD16   x;
        CARD16   y;
        CARD16   width;
        CARD16   height;
        CARD16   minor_opcode;  // minor-opcode
        CARD16   count;
        CARD8    major_opcode;  // major-opcode  // CopyArea, CopyPlane, or extension
    private:
        uint8_t  _unused[11];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct NoExposure : public Event {
    struct [[gnu::packed]] Encoding {
        Header   header;        // header.code 14
        DRAWABLE drawable;
        CARD16   minor_opcode;  // minor-opcode
        CARD8    major_opcode;  // major-opcode  // CopyArea, CopyPlane, or extension
    private:
        uint8_t  _unused[21];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct VisibilityNotify : public Event {
    // Header.code 15
    struct [[gnu::packed]] Encoding {
        Header  header;
        WINDOW  window;
        uint8_t state;  // 0 Unobscured 1 PartiallyObscured 2 FullyObscured
    private:
        uint8_t _unused[23];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view > state_names {
        "Unobscured",         // 0
        "PartiallyObscured",  // 1
        "FullyObscured"       // 2
    };
};

struct CreateNotify : public Event {
    struct [[gnu::packed]] Encoding {
        Header  header;             // header.code 16
        WINDOW  parent;
        WINDOW  window;
        INT16   x;
        INT16   y;
        CARD16  width;
        CARD16  height;
        CARD16  border_width;       // border-width
        BOOL    override_redirect;  // override-redirect
    private:
        uint8_t _unused[9];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct DestroyNotify : public Event {
    struct [[gnu::packed]] Encoding {
        Header  header;        // header.code 17
        WINDOW  event;
        WINDOW  window;
    private:
        uint8_t _unused[20];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct UnmapNotify : public Event {
    struct [[gnu::packed]] Encoding {
        Header  header;          // header.code 18
        WINDOW  event;
        WINDOW  window;
        BOOL    from_configure;  // from-configure
    private:
        uint8_t _unused[19];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct MapNotify : public Event {
    struct [[gnu::packed]] Encoding {
        Header  header;             // header.code 19
        WINDOW  event;
        WINDOW  window;
        BOOL    override_redirect;  // override-redirect
    private:
        uint8_t _unused[19];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct MapRequest : public Event {
    struct [[gnu::packed]] Encoding {
        Header  header;        // header.code 20
        WINDOW  parent;
        WINDOW  window;
    private:
        uint8_t _unused[20];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct ReparentNotify : public Event {
    struct [[gnu::packed]] Encoding {
        Header  header;             // header.code 21
        WINDOW  event;
        WINDOW  window;
        WINDOW  parent;
        INT16   x;
        INT16   y;
        BOOL    override_redirect;  // override-redirect
    private:
        uint8_t _unused[11];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct ConfigureNotify : public Event {
    struct [[gnu::packed]] Encoding {
        Header  header;             // header.code 22
        WINDOW  event;
        WINDOW  window;
        WINDOW  above_sibling;      // above-sibling // 0 None
        INT16   x;
        INT16   y;
        CARD16  width;
        CARD16  height;
        CARD16  border_width;       // border-width
        BOOL    override_redirect;  // override-redirect
    private:
        uint8_t _unused[5];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view >& above_sibling_names {
        protocol::enum_names::zero_none };
};

struct ConfigureRequest : public Event {
    struct [[gnu::packed]] Header {
        uint8_t code;          // 23
        uint8_t stack_mode;    // stack-mode // 0 Above 1 Below 2 TopIf 3 BottomIf 4 Opposite
        CARD16  sequence_num;  // sequence number
    };
    struct [[gnu::packed]] Encoding {
        Header   header;
        WINDOW   parent;
        WINDOW   window;
        WINDOW   sibling;       // 0 None
        INT16    x;
        INT16    y;
        CARD16   width;
        CARD16   height;
        CARD16   border_width;  // border-width
        uint16_t value_mask;    // 2B value-mask
        //   0x0001: x
        //   0x0002: y
        //   0x0004: width
        //   0x0008: height
        //   0x0010: border-width
        //   0x0020: sibling
        //   0x0040: stack-mode
    private:
        uint8_t  _unused[4];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

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

struct GravityNotify : public Event {
    struct [[gnu::packed]] Encoding {
        Header  header;        // header.code 24
        WINDOW  event;
        WINDOW  window;
        INT16   x;
        INT16   y;
    private:
        uint8_t _unused[16];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct ResizeRequest : public Event {
    struct [[gnu::packed]] Encoding {
        Header  header;        // header.code 25
        WINDOW  window;
        CARD16  width;
        CARD16  height;
    private:
        uint8_t _unused[20];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct CirculateNotify : public Event {
    struct [[gnu::packed]] Encoding {
        Header  header;        // header.code 26
        WINDOW  event;
        WINDOW  window;
    private:
        WINDOW  _unused1;
    public:
        uint8_t place;         // 0 Top 1 Bottom
    private:
        uint8_t _unused2[15];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view >& place_names {
        protocol::enum_names::circulate_place };
};

struct CirculateRequest : public Event {
    struct [[gnu::packed]] Encoding {
        Header   header;        // header.code 27
        WINDOW   parent;
        WINDOW   window;
    private:
        uint32_t _unused1;
    public:
        uint8_t  place;         // 0 Top 1 Bottom
    private:
        uint8_t  _unused2[15];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view >& place_names {
        protocol::enum_names::circulate_place };
};

struct PropertyNotify : public Event {
    struct [[gnu::packed]] Encoding {
        Header    header;        // header.code 28
        WINDOW    window;
        ATOM      atom;
        TIMESTAMP time;
        uint8_t   state;         // 0 NewValue 1 Deleted
    private:
        uint8_t   _unused[15];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view > state_names {
        "NewValue",  // 0
        "Deleted"    // 1
    };
};

struct SelectionClear : public Event {
    struct [[gnu::packed]] Encoding {
        Header    header;        // header.code 29
        TIMESTAMP time;
        WINDOW    owner;
        ATOM      selection;
    private:
        uint8_t   _unused[16];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct SelectionRequest : public Event {
    struct [[gnu::packed]] Encoding {
        Header    header;        // header.code 30
        TIMESTAMP time;          // 0 CurrentTime
        WINDOW    owner;
        WINDOW    requestor;
        ATOM      selection;
        ATOM      target;
        ATOM      property;      // 0 None
    private:
        uint8_t   _unused[4];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    inline static const
    std::vector< std::string_view >& property_names {
        protocol::enum_names::zero_none };
};

struct SelectionNotify : public Event {
    struct [[gnu::packed]] Encoding {
        Header    header;        // header.code 31
        TIMESTAMP time;          // 0 CurrentTime
        WINDOW    requestor;
        ATOM      selection;
        ATOM      target;
        ATOM      property;      // 0 None
    private:
        uint8_t   _unused[8];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    inline static const
    std::vector< std::string_view >& property_names {
        protocol::enum_names::zero_none };
};

struct ColormapNotify : public Event {
    struct [[gnu::packed]] Encoding {
        Header   header;        // header.code 32
        WINDOW   window;
        COLORMAP colormap;      // 0 None
        BOOL     new_;          // new
        uint8_t  state;         // 0 Uninstalled 1 Installed
    private:
        uint8_t  _unused[18];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view >& colormap_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view > state_names {
        "Uninstalled",  // 0
        "Installed"     // 1
    };
};

struct ClientMessage : public Event {
    struct [[gnu::packed]] Header {
        uint8_t code;          // 33
        CARD8   format;
        CARD16  sequence_num;  // sequence number
    };
    struct [[gnu::packed]] Encoding {
        Header  header;
        WINDOW  window;
        ATOM    type;
        uint8_t data[20];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct MappingNotify : public Event {
    struct [[gnu::packed]] Encoding {
        Header  header;         // header.code 34
        uint8_t request;        // 0 Modifier 1 Keyboard 2 Pointer
        KEYCODE first_keycode;  // first-keycode
        CARD8   count;
    private:
        uint8_t _unused[25];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    inline static const
    std::vector< std::string_view > request_names {
        "Modifier",  // 0
        "Keyboard",  // 1
        "Pointer"    // 2
    };
};

}  // namespace events

}  // namespace protocol


#endif  // PROTOCOL_EVENTS_HPP
