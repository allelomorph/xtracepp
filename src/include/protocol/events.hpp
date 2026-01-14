#ifndef PROTOCOL_EVENTS_HPP
#define PROTOCOL_EVENTS_HPP

/**
 * @file events.hpp
 * @see [X protocol events](https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Events)
 */

#include <cstdint>
#include <array>

#include "protocol/Response.hpp"
#include "protocol/common_types.hpp"


namespace protocol {

namespace events {

/**
 * @brief Interface class providing default [encoding] for protocol events.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct Event : public Response {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique event identifier; should always be a valid
         *    [core](#codes::Codes) or extension event code. */
        uint8_t code;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused;
    public:
        /** @brief [Serial number on current connection](#Connection::sequence)
         *   of relevant request. */
        CARD16  sequence_num;
    };
    /**
     * @brief Complete fixed encoding, including [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[28];
    };
    /** @brief Expected size in bytes of [Encoding](#Encoding). */
    static constexpr uint32_t ENCODING_SZ { 32 };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );

    virtual ~Event() = 0;
};

namespace codes {

/**
 * @brief Constants representing all core event [codes].
 * [codes]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
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
/** @brief Lowest core event [code](#Codes). */
inline constexpr
int MIN { KEYPRESS };
/** @brief Highest core event [code](#Codes). */
inline constexpr
int MAX { MAPPINGNOTIFY };

}  // namespace codes

/**
 * @brief Name strings for all core event [codes].
 * [codes]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
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

/**
 * @brief Represents shared [encoding] of input events (eg KeyPress, KeyRelease,
 *   ButtonPress, ButtonRelease).
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct InputEvent : public Event {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique event identifier; should always be a valid
         *    [core](#codes::Codes) or extension event code. */
        uint8_t         code;
        /** @brief Protocol name: detail. */
        KEYCODE         detail;
        /** @brief [Serial number on current connection](#Connection::sequence)
         *   of relevant request. */
        CARD16          sequence_num;
    };
    /**
     * @brief Complete fixed encoding, including [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header          header;
        /** @brief Protocol name: time. */
        TIMESTAMP       time;
        /** @brief Protocol name: root. */
        WINDOW          root;
        /** @brief Protocol name: event. */
        WINDOW          event;
        /** @brief Protocol name: child; uses enum: 0=None. */
        WINDOW          child;
        /** @brief Protocol name: root-x. */
        INT16           root_x;
        /** @brief Protocol name: root-y. */
        INT16           root_y;
        /** @brief Protocol name: event-x. */
        INT16           event_x;
        /** @brief Protocol name: event-y. */
        INT16           event_y;
        /** @brief Protocol name: state. */
        SETofKEYBUTMASK state;
        /** @brief Protocol name: same-screen. */
        BOOL            same_screen;
    private:
        /** @brief Ignored bytes. */
        uint8_t         _unused;
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [child](#Encoding::child) enum names. */
    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };

    virtual ~InputEvent() = 0;
};

/**
 * @brief Represents shared [encoding] of boundary events (eg EnterNotify,
 *   LeaveNotify).
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct BoundaryEvent : public Event {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique event identifier; should always be a valid
         *    [core](#codes::Codes) or extension event code. */
        uint8_t         code;
        /** @brief Protocol name: detail; uses enum: 0=Ancestor 1=Virtual
         *    2=Inferior 3=Nonlinear 4=NonlinearVirtual. */
        uint8_t         detail;
        /** @brief [Serial number on current connection](#Connection::sequence)
         *   of relevant request. */
        CARD16          sequence_num;
    };
    /**
     * @brief Complete fixed encoding, including [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header          header;
        /** @brief Protocol name: time. */
        TIMESTAMP       time;
        /** @brief Protocol name: root. */
        WINDOW          root;
        /** @brief Protocol name: event. */
        WINDOW          event;
        /** @brief Protocol name: child; uses enum: 0=None. */
        WINDOW          child;
        /** @brief Protocol name: root-x. */
        INT16           root_x;
        /** @brief Protocol name: root-y. */
        INT16           root_y;
        /** @brief Protocol name: event-x. */
        INT16           event_x;
        /** @brief Protocol name: event-y. */
        INT16           event_y;
        /** @brief Protocol name: state. */
        SETofKEYBUTMASK state;
        /** @brief Protocol name: node; uses enum: 0=Normal 1=Grab 2=Ungrab. */
        uint8_t         mode;
        /** @brief Protocol name: same-screen, focus; both use enum: 0=False
         *    1=True, and use bits: 0x01=focus 0x02=same-screen 0xFC=(unused). */
        uint8_t         focus_same_screen_mask;
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [detail](#Header::detail) enum names. */
    inline static const
    std::vector< std::string_view >& detail_names {
        protocol::enum_names::focus_detail };
    /** @brief Maximum [detail](#Header::detail) value that uses enum name. */
    static constexpr uint8_t DETAIL_ENUM_MAX { 4 };
    /** @brief [child](#Encoding::child) enum names. */
    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
    /** @brief [mode](#Encoding::mode) enum names. */
    inline static const
    std::vector< std::string_view >& mode_names {
        protocol::enum_names::focus_mode };
    /** @brief Maximum [mode](#Encoding::mode) value that uses enum name. */
    static constexpr uint8_t MODE_ENUM_MAX { 2 };
    /** @brief [focus/same-screen](#Encoding::focus_same_screen_mask) flag names. */
    inline static const
    std::vector< std::string_view >& focus_same_screen_names {
        protocol::enum_names::focus_same_screen_mask };
    /** @brief Bits unused by [focus/same-screen](#Encoding::focus_same_screen_mask). */
    static constexpr uint8_t FOCUS_SAME_SCREEN_ZERO_BITS { 0xFC };

    virtual ~BoundaryEvent() = 0;
};

/**
 * @brief Represents shared [encoding] of focus events (eg FocusIn, FocusOut).
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct FocusEvent : public Event {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique event identifier; should always be a valid
         *    [core](#codes::Codes) or extension event code. */
        uint8_t code;
        /** @brief Protocol name: detail; uses enum: 0=Ancestor 1=Virtual
         *    2=Inferior 3=Nonlinear 4=NonlinearVirtual 5=Pointer 6=PointerRoot
         *    7=None. */
        uint8_t detail;
        /** @brief [Serial number on current connection](#Connection::sequence)
         *   of relevant request. */
        CARD16  sequence_num;
    };
    /**
     * @brief Complete fixed encoding, including [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;
        /** @brief Protocol name: event. */
        WINDOW  event;
        /** @brief Protocol name: mode; uses enum: 0=Normal 1=Grab 2=Ungrab
         *    3=WhileGrabbed. */
        uint8_t mode;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[23];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [detail](#Header::detail) enum names. */
    inline static const
    std::vector< std::string_view >& detail_names {
        protocol::enum_names::focus_detail };
    /** @brief [mode](#Encoding::mode) enum names. */
    inline static const
    std::vector< std::string_view >& mode_names {
        protocol::enum_names::focus_mode };

    virtual ~FocusEvent() = 0;
};

}  // namespace impl

/**
 * @brief Represents X11 %KeyPress event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct KeyPress : public impl::InputEvent {
    // Header.code 2
};
/**
 * @brief Represents X11 %KeyRelease event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct KeyRelease : public impl::InputEvent {
    // Header.code 3
};
/**
 * @brief Represents X11 %ButtonPress event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct ButtonPress : public impl::InputEvent {
    // Header.code 4
};
/**
 * @brief Represents X11 %ButtonRelease event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct ButtonRelease : public impl::InputEvent {
    // Header.code 5
};
/**
 * @brief Represents X11 %MotionNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct MotionNotify : public Event {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique event identifier; should always be MOTIONNOTIFY(6). */
        uint8_t code;
        /** @brief Protocol name: detail; uses enum: 0=Normal 1=Hint. */
        uint8_t detail;
        /** @brief [Serial number on current connection](#Connection::sequence)
         *   of relevant request. */
        CARD16  sequence_num;
    };
    /**
     * @brief Complete fixed encoding, including [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header          header;
        /** @brief Protocol name: time. */
        TIMESTAMP       time;
        /** @brief Protocol name: root. */
        WINDOW          root;
        /** @brief Protocol name: event. */
        WINDOW          event;
        /** @brief Protocol name: child; uses enum 0=None. */
        WINDOW          child;
        /** @brief Protocol name: root-x. */
        INT16           root_x;
        /** @brief Protocol name: root-y. */
        INT16           root_y;
        /** @brief Protocol name: event-x. */
        INT16           event_x;
        /** @brief Protocol name: event-y. */
        INT16           event_y;
        /** @brief Protocol name: state. */
        SETofKEYBUTMASK state;
        /** @brief Protocol name: same-screen. */
        BOOL            same_screen;
    private:
        /** @brief Ignored bytes. */
        uint8_t         _unused;
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [detail](#Header::detail) enum names. */
    inline static const
    std::vector< std::string_view > detail_names {
        "Normal",  // 0
        "Hint"     // 1
    };
    /** @brief [child](#Encoding::child) enum names. */
    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
};
/**
 * @brief Represents X11 %EnterNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct EnterNotify : public impl::BoundaryEvent {
    // Header.code 7
};
/**
 * @brief Represents X11 %LeaveNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct LeaveNotify : public impl::BoundaryEvent {
    // Header.code 8
};
/**
 * @brief Represents X11 %FocusIn event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct FocusIn : public impl::FocusEvent {
    // Header.code 9
};
/**
 * @brief Represents X11 %FocusOut event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct FocusOut : public impl::FocusEvent {
    // Header.code 10
};
/**
 * @brief Represents X11 %KeymapNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct KeymapNotify : public Event {
    // TBD maybe we could glean sequence from last event? "This event is
    //   reported to clients selecting KeymapState on a window and is generated
    //   immediately after every EnterNotify and FocusIn."
    /**
     * @brief Fixed encoding prefix.
     * @warning %KeymapNotify is exceptional in that it does not encode a
     *   [sequence number](#Event::Header::sequence_num), and so needs special parsing.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique event identifier; should always be KEYMAPNOTIFY(11). */
        uint8_t code;
    };
    /**
     * @brief Complete fixed encoding, including [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header header;
        /** @brief Protocol name: keys; keycode - 8 is index in array
         *    (keycodes 0-7 omitted). */
        CARD8  keys[31];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %Expose event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct Expose : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;  // header.code 12
        /** @brief Protocol name: window. */
        WINDOW  window;
        /** @brief Protocol name: x. */
        CARD16  x;
        /** @brief Protocol name: y. */
        CARD16  y;
        /** @brief Protocol name: width. */
        CARD16  width;
        /** @brief Protocol name: height. */
        CARD16  height;
        /** @brief Protocol name: count. */
        CARD16  count;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[14];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %GraphicsExposure event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct GraphicsExposure : public Event {
    // TBD major/minor opcode fields allow skipping lookup by conn id/seq
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header   header;  // header.code 13
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: x. */
        CARD16   x;
        /** @brief Protocol name: y. */
        CARD16   y;
        /** @brief Protocol name: width. */
        CARD16   width;
        /** @brief Protocol name: height. */
        CARD16   height;
        /** @brief Protocol name: minor-opcode. */
        CARD16   minor_opcode;
        /** @brief Protocol name: count. */
        CARD16   count;
        /** @brief Protocol name: major-opcode. */
        CARD8    major_opcode;  // CopyArea, CopyPlane, or extension
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[11];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %NoExposure event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct NoExposure : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header   header;  // header.code 14
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: minor-opcode. */
        CARD16   minor_opcode;
        /** @brief Protocol name: major-opcode. */
        CARD8    major_opcode;  // CopyArea, CopyPlane, or extension
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[21];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %VisibilityNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct VisibilityNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;  // header.code 15
        /** @brief Protocol name: window. */
        WINDOW  window;
        /** @brief Protocol name: state; uses enum: 0=Unobscured
         *    1=PartiallyObscured 2=FullyObscured. */
        uint8_t state;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[23];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [state](#Encoding::state) enum names. */
    inline static const
    std::vector< std::string_view > state_names {
        "Unobscured",         // 0
        "PartiallyObscured",  // 1
        "FullyObscured"       // 2
    };
};
/**
 * @brief Represents X11 %CreateNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct CreateNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;  // header.code 16
        /** @brief Protocol name: parent. */
        WINDOW  parent;
        /** @brief Protocol name: window. */
        WINDOW  window;
        /** @brief Protocol name: x. */
        INT16   x;
        /** @brief Protocol name: y. */
        INT16   y;
        /** @brief Protocol name: width. */
        CARD16  width;
        /** @brief Protocol name: height. */
        CARD16  height;
        /** @brief Protocol name: border-width. */
        CARD16  border_width;
        /** @brief Protocol name: override-redirect. */
        BOOL    override_redirect;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[9];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %DestroyNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct DestroyNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;        // header.code 17
        /** @brief Protocol name: event. */
        WINDOW  event;
        /** @brief Protocol name: window. */
        WINDOW  window;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[20];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %UnmapNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct UnmapNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;          // header.code 18
        /** @brief Protocol name: event. */
        WINDOW  event;
        /** @brief Protocol name: window. */
        WINDOW  window;
        /** @brief Protocol name: from-configure. */
        BOOL    from_configure;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[19];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %MapNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct MapNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;             // header.code 19
        /** @brief Protocol name: event. */
        WINDOW  event;
        /** @brief Protocol name: window. */
        WINDOW  window;
        /** @brief Protocol name: override-redirect. */
        BOOL    override_redirect;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[19];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %MapRequest event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct MapRequest : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;  // header.code 20
        /** @brief Protocol name: parent. */
        WINDOW  parent;
        /** @brief Protocol name: window. */
        WINDOW  window;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[20];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %ReparentNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct ReparentNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;  // header.code 21
        /** @brief Protocol name: event. */
        WINDOW  event;
        /** @brief Protocol name: window. */
        WINDOW  window;
        /** @brief Protocol name: parent. */
        WINDOW  parent;
        /** @brief Protocol name: x. */
        INT16   x;
        /** @brief Protocol name: y. */
        INT16   y;
        /** @brief Protocol name: override-redirect. */
        BOOL    override_redirect;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[11];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %ConfigureNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct ConfigureNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;  // header.code 22
        /** @brief Protocol name: event. */
        WINDOW  event;
        /** @brief Protocol name: window. */
        WINDOW  window;
        /** @brief Protocol name: above-sibling; uses enum 0=None. */
        WINDOW  above_sibling;
        /** @brief Protocol name: x. */
        INT16   x;
        /** @brief Protocol name: y. */
        INT16   y;
        /** @brief Protocol name: width. */
        CARD16  width;
        /** @brief Protocol name: height. */
        CARD16  height;
        /** @brief Protocol name: border-width. */
        CARD16  border_width;
        /** @brief Protocol name: override-redirect. */
        BOOL    override_redirect;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[5];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [above-sibling](#Encoding::above_sibling) enum names. */
    inline static const
    std::vector< std::string_view >& above_sibling_names {
        protocol::enum_names::zero_none };
};
/**
 * @brief Represents X11 %ConfigureRequest event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct ConfigureRequest : public Event {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique event identifier; should always be CONFIGUREREQUEST(23). */
        uint8_t code;
        /** @brief Protocol name stack-mode; uses enum: 0=Above 1=Below
         *    2=TopIf 3=BottomIf 4=Opposite. */
        uint8_t stack_mode;
        /** @brief [Serial number on current connection](#Connection::sequence)
         *   of relevant request. */
        CARD16  sequence_num;
    };
    /**
     * @brief Complete fixed encoding, including [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header   header;
        /** @brief Protocol name: parent. */
        WINDOW   parent;
        /** @brief Protocol name: window. */
        WINDOW   window;
        /** @brief Protocol name: sibling; uses enum: 0=None. */
        WINDOW   sibling;
        /** @brief Protocol name: x. */
        INT16    x;
        /** @brief Protocol name: y. */
        INT16    y;
        /** @brief Protocol name: width. */
        CARD16   width;
        /** @brief Protocol name: height. */
        CARD16   height;
        /** @brief Protocol name: border-width. */
        CARD16   border_width;
        /** @brief Protocol name: value-mask.
         *  @note Value masks can be different sizes, 2B here. */
        uint16_t value_mask;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[4];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [stack-mode](#Header::stack_mode) enum names. */
    inline static const
    std::vector< std::string_view >& stack_mode_names {
        protocol::enum_names::window_value_stack_mode };
    /** @brief [sibling](#Encoding::sibling) enum names. */
    inline static const
    std::vector< std::string_view >& sibling_names {
        protocol::enum_names::zero_none };
    /** @brief [value-mask](#Encoding::value_mask) flag names. */
    inline static const
    std::vector< std::string_view >& value_names {
        protocol::enum_names::window_value_mask };
};
/**
 * @brief Represents X11 %GravityNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct GravityNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;  // header.code 24
        /** @brief Protocol name: event. */
        WINDOW  event;
        /** @brief Protocol name: window. */
        WINDOW  window;
        /** @brief Protocol name: x. */
        INT16   x;
        /** @brief Protocol name: y. */
        INT16   y;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[16];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %ResizeRequest event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct ResizeRequest : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;  // header.code 25
        /** @brief Protocol name: window. */
        WINDOW  window;
        /** @brief Protocol name: width. */
        CARD16  width;
        /** @brief Protocol name: height. */
        CARD16  height;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[20];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %CirculateNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct CirculateNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;  // header.code 26
        /** @brief Protocol name: event. */
        WINDOW  event;
        /** @brief Protocol name: window. */
        WINDOW  window;
    private:
        /** @brief Ignored bytes. */
        WINDOW  _unused1;
    public:
        /** @brief Protocol name: place; uses enum: 0=Top 1=Bottom. */
        uint8_t place;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused2[15];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [place](#Encoding::place) enum names. */
    inline static const
    std::vector< std::string_view >& place_names {
        protocol::enum_names::circulate_place };
};
/**
 * @brief Represents X11 %CirculateRequest event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct CirculateRequest : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header   header;  // header.code 27
        /** @brief Protocol name: parent. */
        WINDOW   parent;
        /** @brief Protocol name: window. */
        WINDOW   window;
    private:
        /** @brief Ignored bytes. */
        uint32_t _unused1;
    public:
        /** @brief Protocol name: place; uses enum: 0=Top 1=Bottom. */
        uint8_t  place;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused2[15];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [place](#Encoding::place) enum names. */
    inline static const
    std::vector< std::string_view >& place_names {
        protocol::enum_names::circulate_place };
};
/**
 * @brief Represents X11 %PropertyNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct PropertyNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header    header;  // header.code 28
        /** @brief Protocol name: window. */
        WINDOW    window;
        /** @brief Protocol name: atom. */
        ATOM      atom;
        /** @brief Protocol name: time. */
        TIMESTAMP time;
        /** @brief Protocol name: state; uses enum: 0=NewValue 1=Deleted. */
        uint8_t   state;
    private:
        /** @brief Ignored bytes. */
        uint8_t   _unused[15];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [state](#Encoding::state) enum names. */
    inline static const
    std::vector< std::string_view > state_names {
        "NewValue",  // 0
        "Deleted"    // 1
    };
};
/**
 * @brief Represents X11 %SelectionClear event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct SelectionClear : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header    header;  // header.code 29
        /** @brief Protocol name: time. */
        TIMESTAMP time;
        /** @brief Protocol name: owner. */
        WINDOW    owner;
        /** @brief Protocol name: selection. */
        ATOM      selection;
    private:
        /** @brief Ignored bytes. */
        uint8_t   _unused[16];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %SelectionRequest event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct SelectionRequest : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header    header;  // header.code 30
        /** @brief Protocol name: time; uses enum: 0=CurrentTime. */
        TIMESTAMP time;
        /** @brief Protocol name: owner. */
        WINDOW    owner;
        /** @brief Protocol name: requestor. */
        WINDOW    requestor;
        /** @brief Protocol name: selection. */
        ATOM      selection;
        /** @brief Protocol name: target. */
        ATOM      target;
        /** @brief Protocol name: property; uses enum: 0=None. */
        ATOM      property;
    private:
        /** @brief Ignored bytes. */
        uint8_t   _unused[4];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [time](#Encoding::time) enum names. */
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    /** @brief [property](#Encoding::property) enum names. */
    inline static const
    std::vector< std::string_view >& property_names {
        protocol::enum_names::zero_none };
};
/**
 * @brief Represents X11 %SelectionNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct SelectionNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header    header;  // header.code 31
        /** @brief Protocol name: time; uses enum: 0=CurrentTime. */
        TIMESTAMP time;
        /** @brief Protocol name: requestor. */
        WINDOW    requestor;
        /** @brief Protocol name: selection. */
        ATOM      selection;
        /** @brief Protocol name: target. */
        ATOM      target;
        /** @brief Protocol name: property; uses enum 0=None. */
        ATOM      property;
    private:
        /** @brief Ignored bytes. */
        uint8_t   _unused[8];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [time](#Encoding::time) enum names. */
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    /** @brief [property](#Encoding::property) enum names. */
    inline static const
    std::vector< std::string_view >& property_names {
        protocol::enum_names::zero_none };
};
/**
 * @brief Represents X11 %ColormapNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct ColormapNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header   header;  // header.code 32
        /** @brief Protocol name: window. */
        WINDOW   window;
        /** @brief Protocol name: colormap; uses enum: 0=None. */
        COLORMAP colormap;
        /** @brief Protocol name: new. */
        BOOL     new_;
        /** @brief Protocol name: state; uses enum 0=Uninstalled 1=Installed. */
        uint8_t  state;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[18];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [colormap](#Encoding::colormap) enum names. */
    inline static const
    std::vector< std::string_view >& colormap_names {
        protocol::enum_names::zero_none };
    /** @brief [state](#Encoding::state) enum names. */
    inline static const
    std::vector< std::string_view > state_names {
        "Uninstalled",  // 0
        "Installed"     // 1
    };
};
/**
 * @brief Represents X11 %ClientMessage event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct ClientMessage : public Event {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique event identifier; should always be CLIENTMESSAGE(33). */
        uint8_t code;
        /** @brief Protocol name format. */
        CARD8   format;
        /** @brief [Serial number on current connection](#Connection::sequence)
         *   of relevant request. */
        CARD16  sequence_num;
    };
    /**
     * @brief Complete fixed encoding, including [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;
        /** @brief Protocol name: window. */
        WINDOW  window;
        /** @brief Protocol name: type. */
        ATOM    type;
        /** @brief Protocol name: data. */
        uint8_t data[20];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %MappingNotify event [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Events
 */
struct MappingNotify : public Event {
    /**
     * @brief Complete fixed encoding, uses [Event::Header](#Event::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;  // header.code 34
        /** @brief Protocol name: request; uses enum: 0=Modifier 1=Keyboard
         *    2=Pointer. */
        uint8_t request;
        /** @brief Protocol name: first-keycode. */
        KEYCODE first_keycode;
        /** @brief Protocol name: count. */
        CARD8   count;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[25];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
    /** @brief [request](#Encoding::request) enum names. */
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
