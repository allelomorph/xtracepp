#ifndef PROTOCOL_REQUESTS_HPP
#define PROTOCOL_REQUESTS_HPP

/**
 * @file requests.hpp
 * @see [X protocol requests](https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Requests)
 */

#include <cstdint>
#include <array>
#include <tuple>

#include "protocol/Message.hpp"
#include "protocol/Response.hpp"
#include "protocol/common_types.hpp"
#include "protocol/enum_names.hpp"


namespace protocol {

namespace requests {

namespace opcodes {

/**
 * @brief Constants representing all core request [opcodes].
 * [opcodes]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
enum Opcodes {
    CREATEWINDOW              =    1,
    CHANGEWINDOWATTRIBUTES,   //   2
    GETWINDOWATTRIBUTES,      //   3
    DESTROYWINDOW,            //   4
    DESTROYSUBWINDOWS,        //   5
    CHANGESAVESET,            //   6
    REPARENTWINDOW,           //   7
    MAPWINDOW,                //   8
    MAPSUBWINDOWS,            //   9
    UNMAPWINDOW,              //  10
    UNMAPSUBWINDOWS,          //  11
    CONFIGUREWINDOW,          //  12
    CIRCULATEWINDOW,          //  13
    GETGEOMETRY,              //  14
    QUERYTREE,                //  15
    INTERNATOM,               //  16
    GETATOMNAME,              //  17
    CHANGEPROPERTY,           //  18
    DELETEPROPERTY,           //  19
    GETPROPERTY,              //  20
    LISTPROPERTIES,           //  21
    SETSELECTIONOWNER,        //  22
    GETSELECTIONOWNER,        //  23
    CONVERTSELECTION,         //  24
    SENDEVENT,                //  25
    GRABPOINTER,              //  26
    UNGRABPOINTER,            //  27
    GRABBUTTON,               //  28
    UNGRABBUTTON,             //  29
    CHANGEACTIVEPOINTERGRAB,  //  30
    GRABKEYBOARD,             //  31
    UNGRABKEYBOARD,           //  32
    GRABKEY,                  //  33
    UNGRABKEY,                //  34
    ALLOWEVENTS,              //  35
    GRABSERVER,               //  36
    UNGRABSERVER,             //  37
    QUERYPOINTER,             //  38
    GETMOTIONEVENTS,          //  39
    TRANSLATECOORDINATES,     //  40
    WARPPOINTER,              //  41
    SETINPUTFOCUS,            //  42
    GETINPUTFOCUS,            //  43
    QUERYKEYMAP,              //  44
    OPENFONT,                 //  45
    CLOSEFONT,                //  46
    QUERYFONT,                //  47
    QUERYTEXTEXTENTS,         //  48
    LISTFONTS,                //  49
    LISTFONTSWITHINFO,        //  50
    SETFONTPATH,              //  51
    GETFONTPATH,              //  52
    CREATEPIXMAP,             //  53
    FREEPIXMAP,               //  54
    CREATEGC,                 //  55
    CHANGEGC,                 //  56
    COPYGC,                   //  57
    SETDASHES,                //  58
    SETCLIPRECTANGLES,        //  59
    FREEGC,                   //  60
    CLEARAREA,                //  61
    COPYAREA,                 //  62
    COPYPLANE,                //  63
    POLYPOINT,                //  64
    POLYLINE,                 //  65
    POLYSEGMENT,              //  66
    POLYRECTANGLE,            //  67
    POLYARC,                  //  68
    FILLPOLY,                 //  69
    POLYFILLRECTANGLE,        //  70
    POLYFILLARC,              //  71
    PUTIMAGE,                 //  72
    GETIMAGE,                 //  73
    POLYTEXT8,                //  74
    POLYTEXT16,               //  75
    IMAGETEXT8,               //  76
    IMAGETEXT16,              //  77
    CREATECOLORMAP,           //  78
    FREECOLORMAP,             //  79
    COPYCOLORMAPANDFREE,      //  80
    INSTALLCOLORMAP,          //  81
    UNINSTALLCOLORMAP,        //  82
    LISTINSTALLEDCOLORMAPS,   //  83
    ALLOCCOLOR,               //  84
    ALLOCNAMEDCOLOR,          //  85
    ALLOCCOLORCELLS,          //  86
    ALLOCCOLORPLANES,         //  87
    FREECOLORS,               //  88
    STORECOLORS,              //  89
    STORENAMEDCOLOR,          //  90
    QUERYCOLORS,              //  91
    LOOKUPCOLOR,              //  92
    CREATECURSOR,             //  93
    CREATEGLYPHCURSOR,        //  94
    FREECURSOR,               //  95
    RECOLORCURSOR,            //  96
    QUERYBESTSIZE,            //  97
    QUERYEXTENSION,           //  98
    LISTEXTENSIONS,           //  99
    CHANGEKEYBOARDMAPPING,    // 100
    GETKEYBOARDMAPPING,       // 101
    CHANGEKEYBOARDCONTROL,    // 102
    GETKEYBOARDCONTROL,       // 103
    BELL,                     // 104
    CHANGEPOINTERCONTROL,     // 105
    GETPOINTERCONTROL,        // 106
    SETSCREENSAVER,           // 107
    GETSCREENSAVER,           // 108
    CHANGEHOSTS,              // 109
    LISTHOSTS,                // 110
    SETACCESSCONTROL,         // 111
    SETCLOSEDOWNMODE,         // 112
    KILLCLIENT,               // 113
    ROTATEPROPERTIES,         // 114
    FORCESCREENSAVER,         // 115
    SETPOINTERMAPPING,        // 116
    GETPOINTERMAPPING,        // 117
    SETMODIFIERMAPPING,       // 118
    GETMODIFIERMAPPING,       // 119
    NOOPERATION               =  127
};
// TBD will change with extensions
/** @brief Lowest core request [opcode](#Opcodes). */
inline constexpr int MIN { CREATEWINDOW };
/** @brief Highest core request [opcode](#Opcodes). */
inline constexpr int MAX { NOOPERATION };

}  // namespace opcodes

/**
 * @brief Interface class providing default [encoding] for protocol requests.
 * @note Unlike with [Response](#protocol::Response)-derived classes like Reply,
 *   a Request-derived class will not have the Header included in the Encoding.
 *   This allows for implementation of the [BIG-REQUESTS] extension.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#request_format
 * [BIG-REQUESTS]: https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html
 */
struct Request : public Message {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        union [[gnu::packed]] {
            /** @brief Core opcode. */
            uint8_t opcode;
            /** @brief Extension major-opcode. */
            uint8_t major_opcode;
        };
        union [[gnu::packed]] {
            uint8_t _unused;
            /** @brief Extension minor-opcode. */
            uint8_t minor_opcode;
        };
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;
    };
    virtual ~Request() = 0;
};
/**
 * @brief Interface class providing default [encoding] for protocol request replies.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#reply_format
 */
struct Reply : public Response {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Identifies message as reply; should always equal [REPLY](#REPLY). */
        uint8_t  reply;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused;
    public:
        /** @brief [Serial number on current connection](#Connection::sequence)
         *   of relevant request. */
        CARD16   sequence_num;
        /** @brief Length of encoding in excess of default, in 4B units. */
        uint32_t extra_aligned_units;
    };
    /**
     * @brief Fixed encoding, including [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header  header;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[24];
    };
    /** @brief Identifies message as reply when found in
     *   [Header::reply](#Header::reply). */
    static constexpr uint8_t REPLY { Response::REPLY_PREFIX };
    /** @brief Default size in bytes of [Encoding](#Encoding). */
    static constexpr uint32_t DEFAULT_ENCODING_SZ { 32 };
    static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );

    virtual ~Reply() = 0;
};
/** @brief Sentinel value for name of unused core opcodes in [names](#names). */
inline constexpr
std::string_view UNUSED_OPCODE_STRING { "(unused core opcode)" };
/** @brief Core request names by opcode. */
inline constexpr
std::array< std::string_view, opcodes::MAX + 1 > names {
    UNUSED_OPCODE_STRING,       //   0
    "CreateWindow",             //   1
    "ChangeWindowAttributes",   //   2
    "GetWindowAttributes",      //   3
    "DestroyWindow",            //   4
    "DestroySubwindows",        //   5
    "ChangeSaveSet",            //   6
    "ReparentWindow",           //   7
    "MapWindow",                //   8
    "MapSubwindows",            //   9
    "UnmapWindow",              //  10
    "UnmapSubwindows",          //  11
    "ConfigureWindow",          //  12
    "CirculateWindow",          //  13
    "GetGeometry",              //  14
    "QueryTree",                //  15
    "InternAtom",               //  16
    "GetAtomName",              //  17
    "ChangeProperty",           //  18
    "DeleteProperty",           //  19
    "GetProperty",              //  20
    "ListProperties",           //  21
    "SetSelectionOwner",        //  22
    "GetSelectionOwner",        //  23
    "ConvertSelection",         //  24
    "SendEvent",                //  25
    "GrabPointer",              //  26
    "UngrabPointer",            //  27
    "GrabButton",               //  28
    "UngrabButton",             //  29
    "ChangeActivePointerGrab",  //  30
    "GrabKeyboard",             //  31
    "UngrabKeyboard",           //  32
    "GrabKey",                  //  33
    "UngrabKey",                //  34
    "AllowEvents",              //  35
    "GrabServer",               //  36
    "UngrabServer",             //  37
    "QueryPointer",             //  38
    "GetMotionEvents",          //  39
    "TranslateCoordinates",     //  40
    "WarpPointer",              //  41
    "SetInputFocus",            //  42
    "GetInputFocus",            //  43
    "QueryKeymap",              //  44
    "OpenFont",                 //  45
    "CloseFont",                //  46
    "QueryFont",                //  47
    "QueryTextExtents",         //  48
    "ListFonts",                //  49
    "ListFontsWithInfo",        //  50
    "SetFontPath",              //  51
    "GetFontPath",              //  52
    "CreatePixmap",             //  53
    "FreePixmap",               //  54
    "CreateGC",                 //  55
    "ChangeGC",                 //  56
    "CopyGC",                   //  57
    "SetDashes",                //  58
    "SetClipRectangles",        //  59
    "FreeGC",                   //  60
    "ClearArea",                //  61
    "CopyArea",                 //  62
    "CopyPlane",                //  63
    "PolyPoint",                //  64
    "PolyLine",                 //  65
    "PolySegment",              //  66
    "PolyRectangle",            //  67
    "PolyArc",                  //  68
    "FillPoly",                 //  69
    "PolyFillRectangle",        //  70
    "PolyFillArc",              //  71
    "PutImage",                 //  72
    "GetImage",                 //  73
    "PolyText8",                //  74
    "PolyText16",               //  75
    "ImageText8",               //  76
    "ImageText16",              //  77
    "CreateColormap",           //  78
    "FreeColormap",             //  79
    "CopyColormapAndFree",      //  80
    "InstallColormap",          //  81
    "UninstallColormap",        //  82
    "ListInstalledColormaps",   //  83
    "AllocColor",               //  84
    "AllocNamedColor",          //  85
    "AllocColorCells",          //  86
    "AllocColorPlanes",         //  87
    "FreeColors",               //  88
    "StoreColors",              //  89
    "StoreNamedColor",          //  90
    "QueryColors",              //  91
    "LookupColor",              //  92
    "CreateCursor",             //  93
    "CreateGlyphCursor",        //  94
    "FreeCursor",               //  95
    "RecolorCursor",            //  96
    "QueryBestSize",            //  97
    "QueryExtension",           //  98
    "ListExtensions",           //  99
    "ChangeKeyboardMapping",    // 100
    "GetKeyboardMapping",       // 101
    "ChangeKeyboardControl",    // 102
    "GetKeyboardControl",       // 103
    "Bell",                     // 104
    "ChangePointerControl",     // 105
    "GetPointerControl",        // 106
    "SetScreenSaver",           // 107
    "GetScreenSaver",           // 108
    "ChangeHosts",              // 109
    "ListHosts",                // 110
    "SetAccessControl",         // 111
    "SetCloseDownMode",         // 112
    "KillClient",               // 113
    "RotateProperties",         // 114
    "ForceScreenSaver",         // 115
    "SetPointerMapping",        // 116
    "GetPointerMapping",        // 117
    "SetModifierMapping",       // 118
    "GetModifierMapping",       // 119
    UNUSED_OPCODE_STRING,       // 120 (unused) // TBD consider map instead?
    UNUSED_OPCODE_STRING,       // 121 (unused)
    UNUSED_OPCODE_STRING,       // 122 (unused)
    UNUSED_OPCODE_STRING,       // 123 (unused)
    UNUSED_OPCODE_STRING,       // 124 (unused)
    UNUSED_OPCODE_STRING,       // 125 (unused)
    UNUSED_OPCODE_STRING,       // 126 (unused)
    "NoOperation"               // 127
};

namespace impl {

/**
 * @brief Represents shared [encoding] of WINDOW requests that use a value-list,
 *   (eg CreateWindow, ChangeWindowAttributes).
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct WindowValueListRequest : public Request {
    /** @brief List of all parseable types potentially stored as VALUEs in
     *    `value-list`.
     *  - `PIXMAP           background-pixmap` 0=None 1=ParentRelative
     *  - `CARD32           background-pixel`
     *  - `PIXMAP           border-pixmap` 0=CopyFromParent
     *  - `CARD32           border-pixel`
     *  - `BITGRAVITY       bit-gravity`
     *  - `WINGRAVITY       win-gravity`
     *  - `uint8_t          backing-store` 0=NotUseful 1=WhenMapped 2=Always
     *  - `CARD32           backing-planes`
     *  - `CARD32           backing-pixel`
     *  - `BOOL             override-redirect`
     *  - `BOOL             save-under`
     *  - `SETofEVENT       event-mask`
     *  - `SETofDEVICEEVENT do-not-propagate-mask`
     *  - `COLORMAP         colormap` 0=CopyFromParent
     *  - `CURSOR           cursor` 0=None */
    inline static constexpr
    std::tuple<
        PIXMAP, CARD32, PIXMAP, CARD32, BITGRAVITY, WINGRAVITY, uint8_t, CARD32,
        CARD32, BOOL, BOOL, SETofEVENT, SETofDEVICEEVENT, COLORMAP, CURSOR
        > value_types {};
    /** @brief `value-mask` flag names/`value-list` member names. */
    inline static const
    std::vector< std::string_view >& value_names {
        protocol::enum_names::window_attribute_value_mask };
    /** @brief VALUE `background-pixmap` enum names. */
    inline static const
    std::vector< std::string_view >& background_pixmap_names {
        protocol::enum_names::window_attribute_background_pixmap };
    /** @brief VALUE `border-pixmap` enum names. */
    inline static const
    std::vector< std::string_view >& border_pixmap_names {
        protocol::enum_names::zero_copy_from_parent };
    /** @brief VALUE `backing-store` enum names. */
    inline static const
    std::vector< std::string_view >& backing_store_names {
        protocol::enum_names::window_attribute_backing_store };
    /** @brief VALUE `colormap` enum names. */
    inline static const
    std::vector< std::string_view >& colormap_names {
        protocol::enum_names::zero_copy_from_parent };
    /** @brief VALUE `cursor` enum names. */
    inline static const
    std::vector< std::string_view >& cursor_names {
        protocol::enum_names::zero_none };
    virtual ~WindowValueListRequest() = 0;
};

}  // namespace impl

/**
 * @brief Represents X11 %CreateWindow request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct CreateWindow : public impl::WindowValueListRequest {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal CREATEWINDOW(1). */
        uint8_t  opcode;
        /** @brief Protocol name: depth. */
        CARD8    depth;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // (8+n)
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofVALUE value-list` of 4nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: wid. */
        WINDOW   wid;
        /** @brief Protocol name: parent. */
        WINDOW   parent;
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
        /** @brief Protocol name: class; uses enum: 0=CopyFromParent
         *    1=InputOutput 2=InputOnly. */
        uint16_t class_;
        /** @brief Protocol name: visual; uses enum: 0=CopyFromParent. */
        VISUALID visual;
        /** @brief Protocol name: value-mask (4B BITMASK, n bits set to 1). */
        uint32_t value_mask;
    };
    /** @brief [class](#Encoding::class_) enum names. */
    inline static const
    std::vector< std::string_view >& class_names {
        protocol::enum_names::window_class };
    /** @brief [visual](#Encoding::visual) enum names. */
    inline static const
    std::vector< std::string_view >& visual_names {
        protocol::enum_names::zero_copy_from_parent };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %ChangeWindowAttributes request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   CHANGEWINDOWATTRIBUTES(1) and `tl_aligned_units` of 3+n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ChangeWindowAttributes : public impl::WindowValueListRequest {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofVALUE value-list` of 4nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: window. */
        WINDOW   window;
        /** @brief Protocol name: value-mask (4B BITMASK, n bits set to 1). */
        uint32_t value_mask;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

namespace impl {

/**
 * @brief Represents shared [encoding] of minimal WINDOW requests (eg DestroyWindow).
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SimpleWindowRequest : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: window. */
        WINDOW window;
    };
    /** @brief Total encoding size in bytes (before suffixes). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    virtual ~SimpleWindowRequest() = 0;
};

}  // namespace impl

/**
 * @brief Represents X11 %GetWindowAttributes request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETWINDOWATTRIBUTES(3) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetWindowAttributes : public impl::SimpleWindowRequest {
    /**
     * @brief Represents X11 %GetWindowAttributes reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: backing-store; uses enum: 0=NotUseful
             *    1=WhenMapped 2=Always. */
            uint8_t  backing_store;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // 3
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header           header;
            /** @brief Protocol name: visual. */
            VISUALID         visual;
            /** @brief Protocol name: class; uses enum: 1=InputOutput
             *    2=InputOnly. */
            uint16_t         class_;
            /** @brief Protocol name: bit-gravity. */
            BITGRAVITY       bit_gravity;
            /** @brief Protocol name: win-gravity. */
            WINGRAVITY       win_gravity;
            /** @brief Protocol name: backing-planes. */
            CARD32           backing_planes;
            /** @brief Protocol name: backing-pixel. */
            CARD32           backing_pixel;
            /** @brief Protocol name: save-under. */
            BOOL             save_under;
            /** @brief Protocol name: map-is-installed. */
            BOOL             map_is_installed;
            /** @brief Protocol name: map-state; uses enum: 0=Unmapped
             *    1=Unviewable 2=Viewable. */
            uint8_t          map_state;
            /** @brief Protocol name: override-redirect. */
            BOOL             override_redirect;
            /** @brief Protocol name: colormap; uses enum: 0=None. */
            COLORMAP         colormap;
            /** @brief Protocol name: all-event-masks. */
            SETofEVENT       all_event_masks;
            /** @brief Protocol name: your-event-mask. */
            SETofEVENT       your_event_mask;
            /** @brief Protocol name: do-not-propagate-mask. */
            SETofDEVICEEVENT do_not_propagate_mask;
        private:
            /** @brief Ignored bytes. */
            uint8_t          _unused[2];
        };
        static_assert( sizeof( Encoding ) ==
                       DEFAULT_ENCODING_SZ + ( 3 * ALIGN ) );
        /** @brief [backing-store](#Header::backing_store) enum names. */
        inline static const
        std::vector< std::string_view >& backing_store_names {
            protocol::enum_names::window_attribute_backing_store };
        /** @brief [class](#Encoding::class_) enum names. */
        inline static const
        std::vector< std::string_view >& class_names {
            protocol::enum_names::window_class };
        /** @brief [class](#Encoding::class_) minimum value to use enum. */
        static constexpr uint16_t CLASS_ENUM_MIN { 1 };
        /** @brief [map-state](#Encoding::map_state) enum names. */
        inline static const
        std::vector< std::string_view > map_state_names {
            "Unmapped",    // 0
            "Unviewable",  // 1
            "Viewable"     // 2
        };
        /** @brief [colormap](#Encoding::colormap) enum names. */
        inline static const
        std::vector< std::string_view >& colormap_names {
            protocol::enum_names::zero_none };
    };
};
/**
 * @brief Represents X11 %DestroyWindow request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   DESTROYWINDOW(4) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct DestroyWindow : public impl::SimpleWindowRequest {
};
/**
 * @brief Represents X11 %DestroySubwindows request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   DESTROYSUBWINDOWS(5) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct DestroySubwindows : public impl::SimpleWindowRequest {
};
/**
 * @brief Represents X11 %ChangeSaveSet request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ChangeSaveSet : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal CHANGESAVESET(6). */
        uint8_t  opcode;
        /** @brief Protocol name: mode; uses enum: 0=Insert 1=Delete. */
        uint8_t  mode;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 2
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: window. */
        WINDOW window;
    };
    /** @brief [mode](#Header::mode) enum names. */
    inline static const
    std::vector< std::string_view >& mode_names {
        protocol::enum_names::change_mode };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %ReparentWindow request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   REPARENTWINDOW(7) and `tl_aligned_units` of 4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ReparentWindow : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: window. */
        WINDOW window;
        /** @brief Protocol name: parent. */
        WINDOW parent;
        /** @brief Protocol name: x. */
        INT16  x;
        /** @brief Protocol name: y. */
        INT16  y;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %MapWindow request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   MAPWINDOW(8) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct MapWindow : public impl::SimpleWindowRequest {
};
/**
 * @brief Represents X11 %MapSubwindows request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   MAPSUBWINDOWS(9) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct MapSubwindows : public impl::SimpleWindowRequest {
};
/**
 * @brief Represents X11 %UnmapWindow request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   UNMAPWINDOW(10) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct UnmapWindow : public impl::SimpleWindowRequest {
};
/**
 * @brief Represents X11 %UnmapSubwindows request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   UNMAPSUBWINDOWS(11) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct UnmapSubwindows : public impl::SimpleWindowRequest {
};
/**
 * @brief Represents X11 %ConfigureWindow request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   CONFIGUREWINDOW(12) and `tl_aligned_units` of 3+n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ConfigureWindow : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofVALUE value-list` of (4n)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: window. */
        WINDOW   window;
        /** @brief Protocol name: value-mask (2B BITMASK, n bits set to 1). */
        uint16_t value_mask;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[2];
    };
    /** @brief List of all parseable types potentially stored as VALUEs in
     *    `value-list`.
     *  - `INT16   x`
     *  - `INT16   y`
     *  - `CARD16  width`
     *  - `CARD16  height`
     *  - `CARD16  border-width`
     *  - `WINDOW  sibling`
     *  - `uint8_t stack-mode` 0=Above 1=Below 2=TopIf 3=BottomIf 4=Opposite */
    inline static constexpr
    std::tuple<
        INT16, INT16, CARD16, CARD16, CARD16, WINDOW, uint8_t
        > value_types {};
    /** @brief `value-mask` flag names/`value-list` member names. */
    inline static const
    std::vector< std::string_view >& value_names {
        protocol::enum_names::window_value_mask };
    /** @brief VALUE `stack-mode` enum names. */
    inline static const
    std::vector< std::string_view >& stack_mode_names {
        protocol::enum_names::window_value_stack_mode };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %CirculateWindow request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct CirculateWindow : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal CIRCULATEWINDOW(13). */
        uint8_t   opcode;
        /** @brief Protocol name: direction; uses enum:
         *    0=RaiseLowest 1=LowerHighest. */
        uint8_t   direction;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t  tl_aligned_units;  // 2
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: window. */
        WINDOW    window;
    };
    /** @brief [direction](#Header::direction) enum names. */
    inline static const
    std::vector< std::string_view > direction_names {
        "RaiseLowest",  // 0
        "LowerHighest"  // 1
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GetGeometry request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETGEOMETRY(14) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetGeometry : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE   drawable;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %GetGeometry reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: depth. */
            CARD8    depth;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // 0
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;
            /** @brief Protocol name: root. */
            WINDOW   root;
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
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused[10];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %QueryTree request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   QUERYTREE(15) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct QueryTree : public impl::SimpleWindowRequest {
    /**
     * @brief Represents X11 %QueryTree reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         * @note Followed by suffix:
         *   - `LISTofWINDOW children` of (4 *
         *     [children_ct](#Encoding::children_ct))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;  // `extra_aligned_units` of children_ct
            /** @brief Protocol name: root. */
            WINDOW   root;
            /** @brief Protocol name: parent; uses enum: 0=None. */
            WINDOW   parent;
            /** @brief `children` length in [WINDOW](#protocol::WINDOW)s. */
            uint16_t children_ct;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused[14];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
        /** @brief [parent](#Encoding::parent) enum names. */
        inline static const
        std::vector< std::string_view >& parent_names {
            protocol::enum_names::zero_none };
    };
};
/**
 * @brief Represents X11 %InternAtom request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct InternAtom : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should always equal INTERNATOM(16). */
        uint8_t   opcode;
        /** @brief Protocol name: only-if-exists. */
        BOOL      only_if_exists;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t  tl_aligned_units;  // 2 + pad(name_len)/4
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffix:
     *   - `STRING8 name` of pad(n)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Length of `name` in bytes. */
        uint16_t  name_len;
    private:
        /** @brief Ignored bytes. */
        uint8_t   _unused[2];
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %InternAtom reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Reply::Header](#requests::Reply::Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;  // `extra_aligned_units` of 0
            /** @brief Protocol name: atom; uses enum: 0=None. */
            ATOM    atom;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused2[20];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
        /** @brief [atom](#Encoding::atom) enum names. */
        inline static const
        std::vector< std::string_view >& atom_names {
            protocol::enum_names::zero_none };
    };
};
/**
 * @brief Represents X11 %GetAtomName request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETATOMNAME(17) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetAtomName : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: atom. */
        ATOM atom;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %GetAtomName reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Reply::Header](#requests::Reply::Header).
         * @note Followed by suffix:
         *   - `STRING8 name` of
         *   [pad](#X11ProtocolParser::_Alignment::pad)([name_len](#name_len))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;  // `extra_aligned_units` of pad(name_len)/4
            /** @brief Length of `name` in bytes. */
            uint16_t name_len;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[22];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %ChangeProperty request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ChangeProperty : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal CHANGEPROPERTY(18). */
        uint8_t  opcode;
        /** @brief Protocol name: mode; uses enum: 0=Replace 1=Prepend 2=Append. */
        uint8_t  mode;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 6+pad(n)/4
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofBYTE data` of pad(([format](#Encoding::format)
     *      / 8) * [data_fmt_unit_len](#Encoding::data_fmt_unit_len))B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: window. */
        WINDOW  window;
        /** @brief Protocol name: property. */
        ATOM    property;
        /** @brief Protocol name: type. */
        ATOM    type;
        /** @brief Protocol name: format; size of format units in bits (0,8,16,32). */
        CARD8   format;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[3];
    public:
        /** @brief Length of `data` in format units. */
        CARD32  data_fmt_unit_len;
    };
    /** @brief [mode](#Header::mode) enum names. */
    inline static const
    std::vector< std::string_view > mode_names {
        "Replace",  // 0
        "Prepend",  // 1
        "Append"    // 2
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %DeleteProperty request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   DELETEPROPERTY(19) and `tl_aligned_units` of 3.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct DeleteProperty : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: window. */
        WINDOW    window;
        /** @brief Protocol name: property. */
        ATOM      property;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GetProperty request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetProperty : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal GETPROPERTY(20). */
        uint8_t  opcode;
        /** @brief Protocol name: delete. */
        BOOL     delete_;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 6
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: window. */
        WINDOW window;
        /** @brief Protocol name: property. */
        ATOM   property;
        /** @brief Protocol name: type; uses enum: 0=AnyPropertyType. */
        ATOM   type;
        /** @brief Protocol name: long-offset. */
        CARD32 long_offset;
        /** @brief Protocol name: long-length. */
        CARD32 long_length;
    };
    /** @brief [type](#Encoding::type) enum names. */
    inline static const
    std::vector< std::string_view > type_names {
        "AnyPropertyType"  // 0
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %GetProperty reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Identifies message as reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: format; size of format units in bits (0,8,16,32). */
            CARD8    format;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // pad(value_fmt_unit_len * format) / 4 B
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         * @note Followed by suffix:
         *   - `LISTofBYTE value` of
         *   [pad](#X11ProtocolParser::_Alignment::pad)(
         *   [value_fmt_unit_len](#Encoding::value_fmt_unit_len) *
         *   [format](#Header::format))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;
            /** @brief Protocol name: type; uses enum: 0=None. */
            ATOM     type;
            /** @brief Protocol name: bytes-after. */
            CARD32   bytes_after;
            /** @brief Length of `value` in format units. */
            CARD32   value_fmt_unit_len;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused[12];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
        /** @brief [type](#Encoding::type) enum names. */
        inline static const
        std::vector< std::string_view >& type_names {
            protocol::enum_names::zero_none };
        /** @brief Constant for maximum [format](#Header::format) value. */
        static constexpr CARD8 MAX_FORMAT    { 32 };
        /** @brief Constant for [format](#Header::format) divisor to get byte count. */
        static constexpr CARD8 FORMAT_FACTOR { 8 };
    };
};
/**
 * @brief Represents X11 %ListProperties request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   LISTPROPERTIES(21) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ListProperties : public impl::SimpleWindowRequest {
    /**
     * @brief Represents X11 %ListProperties reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header)).
         * @note Followed by suffix:
         *   - `LISTofATOM atoms` of (sizeof(ATOM) *
         *   [atoms_ct](#Encoding::atoms_ct))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;  // `extra_aligned_units` of atoms_ct
            /** @brief Length of `atoms` in ATOMs. */
            uint16_t atoms_ct;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused[22];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %SetSelectionOwner request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   SETSELECTIONOWNER(22) and `tl_aligned_units` of 4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SetSelectionOwner : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: owner; uses enum: 0=None. */
        WINDOW    owner;
        /** @brief Protocol name: selection. */
        ATOM      selection;
        /** @brief Protocol name: time; uses enum: 0=CurrentTime. */
        TIMESTAMP time;
    };
    /** @brief [owner](#Encoding::owner) enum names. */
    inline static const
    std::vector< std::string_view >& owner_names {
        protocol::enum_names::zero_none };
    /** @brief [time](#Encoding::time) enum names. */
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GetSelectionOwner request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETSELECTIONOWNER(23) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetSelectionOwner : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: selection. */
        ATOM selection;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %GetSelectionOwner reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding after [Header](#requests::Reply::Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;  // `extra_aligned_units` of 0
            /** @brief Protocol name: owner; uses enum: 0=None. */
            WINDOW  owner;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[20];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
        /** @brief [owner](#Encoding::owner) enum names. */
        inline static const
        std::vector< std::string_view >& owner_names {
            protocol::enum_names::zero_none };
    };
};
/**
 * @brief Represents X11 %ConvertSelection request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   CONVERTSELECTION(24) and `tl_aligned_units` of 6.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ConvertSelection : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: requestor. */
        WINDOW    requestor;
        /** @brief Protocol name: selection. */
        ATOM      selection;
        /** @brief Protocol name: target. */
        ATOM      target;
        /** @brief Protocol name: property; uses enum: 0=None. */
        ATOM      property;
        /** @brief Protocol name: time; uses enum: 0=CurrentTime. */
        TIMESTAMP time;
    };
    /** @brief [property](#Encoding::property) enum names. */
    inline static const
    std::vector< std::string_view >& property_names {
        protocol::enum_names::zero_none };
    /** @brief [time](#Encoding::time) enum names. */
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %SendEvent request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SendEvent : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal SENDEVENT(25). */
        uint8_t  opcode;
        /** @brief Protocol name: propagate. */
        BOOL     propagate;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 11
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffix:
     *   - [Event](#protocol::events::Event) of 32B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: destination; uses enum: 0=PointerWindow
         *    1=InputFocus. */
        WINDOW     destination;
        /** @brief Protocol name: event-mask. */
        SETofEVENT event_mask;
    };
    /** @brief [destination](#Encoding::destination) enum names. */
    inline static const
    std::vector< std::string_view > destination_names {
        "PointerWindow",  // 0
        "InputFocus"      // 1
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /** @brief Generated events will have codes with msb toggled on. */
    static constexpr uint8_t GENERATED_EVENT_FLAG { 0x80 };
    /** @brief Masks out generated event flag. */
    static constexpr uint8_t EVENT_CODE_MASK { uint8_t( ~GENERATED_EVENT_FLAG ) };
};
/**
 * @brief Represents X11 %GrabPointer request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GrabPointer : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal GRABPOINTER(26). */
        uint8_t  opcode;
        /** @brief Protocol name: owner-events. */
        BOOL     owner_events;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 6
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: grab-window. */
        WINDOW            grab_window;
        /** @brief Protocol name: event-mask. */
        SETofPOINTEREVENT event_mask;
        /** @brief Protocol name: pointer-mode; uses enum:
         *   0=Synchronous 1=Asynchronous. */
        uint8_t           pointer_mode;
        /** @brief Protocol name: keyboard-mode; uses enum:
         *   0=Synchronous 1=Asynchronous. */
        uint8_t           keyboard_mode;
        /** @brief Protocol name: confine-to; uses enum: 0=None. */
        WINDOW            confine_to;
        /** @brief Protocol name: cursor; uses enum: 0=None. */
        CURSOR            cursor;
        /** @brief Protocol name: time; uses enum: 0=CurrentTime. */
        TIMESTAMP         time;
    };
    /** @brief [pointer-mode](#Encoding::pointer_mode) enum names. */
    inline static const
    std::vector< std::string_view >& pointer_mode_names {
        protocol::enum_names::input_mode };
    /** @brief [keyboard-mode](#Encoding::keyboard_mode) enum names. */
    inline static const
    std::vector< std::string_view >& keyboard_mode_names {
        protocol::enum_names::input_mode };
    /** @brief [confine-to](#Encoding::confine_to) enum names. */
    inline static const
    std::vector< std::string_view >& confine_to_names {
        protocol::enum_names::zero_none };
    /** @brief [cursor](#Encoding::cursor) enum names. */
    inline static const
    std::vector< std::string_view >& cursor_names {
        protocol::enum_names::zero_none };
    /** @brief [time](#Encoding::time) enum names. */
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %GrabPointer reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: status; uses enum:
             *   0=Success 1=AlreadyGrabbed 2=InvalidTime 3=NotViewable 4=Frozen. */
            uint8_t  status;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // 0
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        /** @brief [status](#Header::status) enum names. */
        inline static const
        std::vector< std::string_view >& status_names {
            protocol::enum_names::grab_status };
    };
};
/**
 * @brief Represents X11 %UngrabPointer request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   UNGRABPOINTER(27) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct UngrabPointer : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: time; uses enum: 0=CurrentTime. */
        TIMESTAMP time;
    };
    /** @brief [time](#Encoding::time) enum names. */
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GrabButton request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GrabButton : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal GRABBUTTON(28). */
        uint8_t  opcode;
        /** @brief Protocol name: owner-events. */
        BOOL     owner_events;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 6
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: grab-window. */
        WINDOW            grab_window;
        /** @brief Protocol name: event-mask. */
        SETofPOINTEREVENT event_mask;
        /** @brief Protocol name: pointer-mode; uses enum:
         *   0=Synchronous 1=Asynchronous. */
        uint8_t           pointer_mode;
        /** @brief Protocol name: keyboard-mode; uses enum:
         *   0=Synchronous 1=Asynchronous. */
        uint8_t           keyboard_mode;
        /** @brief Protocol name: confine-to; uses enum: 0=None. */
        WINDOW            confine_to;
        /** @brief Protocol name: cursor; uses enum: 0=None. */
        CURSOR            cursor;
        /** @brief Protocol name: button; uses enum: 0=AnyButton. */
        BUTTON            button;
    private:
        /** @brief Ignored bytes. */
        uint8_t           unused;
    public:
        /** @brief Protocol name: modifiers; may use special flag
         *   [AnyModifier](#SETofKEYMASK::ANYMODIFIER). */
        SETofKEYMASK      modifiers;
    };
    /** @brief [pointer-mode](#Encoding::pointer_mode) enum names. */
    inline static const
    std::vector< std::string_view >& pointer_mode_names {
        protocol::enum_names::input_mode };
    /** @brief [keyboard-mode](#Encoding::keyboard_mode) enum names. */
    inline static const
    std::vector< std::string_view >& keyboard_mode_names {
        protocol::enum_names::input_mode };
    /** @brief [confine-to](#Encoding::confine_to) enum names. */
    inline static const
    std::vector< std::string_view >& confine_to_names {
        protocol::enum_names::zero_none };
    /** @brief [cursor](#Encoding::cursor) enum names. */
    inline static const
    std::vector< std::string_view >& cursor_names {
        protocol::enum_names::zero_none };
    /** @brief [button](#Encoding::button) enum names. */
    inline static const
    std::vector< std::string_view >& button_names {
        protocol::enum_names::button };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %UngrabButton request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct UngrabButton : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal UNGRABBUTTON(29). */
        uint8_t  opcode;
        /** @brief Protocol name: button; uses enum: 0=AnyButton. */
        BUTTON   button;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 3
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: grab-window. */
        WINDOW       grab_window;
        /** @brief Protocol name: modifiers; may use special flag
         *   [AnyModifier](#SETofKEYMASK::ANYMODIFIER). */
        SETofKEYMASK modifiers;
    private:
        /** @brief Ignored bytes. */
        uint8_t      _unused[2];
    };
    /** @brief [button](#Header::button) enum names. */
    inline static const
    std::vector< std::string_view >& button_names {
        protocol::enum_names::button };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %ChangeActivePointerGrab request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   CHANGEACTIVEPOINTERGRAB(30) and `tl_aligned_units` of 4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ChangeActivePointerGrab : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cursor; uses enum: 0=None. */
        CURSOR            cursor;
        /** @brief Protocol name: time; uses enum: 0=CurrentTime. */
        TIMESTAMP         time;
        /** @brief Protocol name: event-mask. */
        SETofPOINTEREVENT event_mask;
    private:
        /** @brief Ignored bytes. */
        uint8_t           _unused[2];
    };
    /** @brief [cursor](#Encoding::cursor) enum names. */
    inline static const
    std::vector< std::string_view >& cursor_names {
        protocol::enum_names::zero_none };
    /** @brief [time](#Encoding::time) enum names. */
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GrabKeyboard request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GrabKeyboard : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal GRABKEYBOARD(31). */
        uint8_t  opcode;
        /** @brief Protocol name: owner-events. */
        BOOL     owner_events;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 4
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: grab-window. */
        WINDOW    grab_window;
        /** @brief Protocol name: grab-window; uses enum: 0=CurrentTime. */
        TIMESTAMP time;
        /** @brief Protocol name: pointer-mode; uses enum:
         *   0=Synchronous 1=Asynchronous. */
        uint8_t   pointer_mode;
        /** @brief Protocol name: keyboard-mode; uses enum:
         *   0=Synchronous 1=Asynchronous. */
        uint8_t   keyboard_mode;
    private:
        /** @brief Ignored bytes. */
        uint8_t   _unused[2];
    };
    /** @brief [time](#Encoding::time) enum names. */
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    /** @brief [pointer-mode](#Encoding::pointer_mode) enum names. */
    inline static const
    std::vector< std::string_view >& pointer_mode_names {
        protocol::enum_names::input_mode };
    /** @brief [keyboard-mode](#Encoding::keyboard_mode) enum names. */
    inline static const
    std::vector< std::string_view >& keyboard_mode_names {
        protocol::enum_names::input_mode };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %GrabKeyboard reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: status; uses enum:
             *   0=Success 1=AlreadyGrabbed 2=InvalidTime 3=NotViewable 4=Frozen. */
            uint8_t  status;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // 0
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        /** @brief [status](#Header::status) enum names. */
        inline static const
        std::vector< std::string_view >& status_names {
            protocol::enum_names::grab_status };
    };
};
/**
 * @brief Represents X11 %UngrabKeyboard request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   UNGRABKEYBOARD(32) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct UngrabKeyboard : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: time; uses enum: 0=CurrentTime. */
        TIMESTAMP time;
    };
    /** @brief [time](#Encoding::time) enum names. */
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GrabKey request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GrabKey : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal GRABKEY(33). */
        uint8_t  opcode;
        /** @brief Protocol name: owner-events. */
        BOOL     owner_events;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 4
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: grab-window. */
        WINDOW       grab_window;
        /** @brief Protocol name: modifiers; may use special flag
         *   [AnyModifier](#SETofKEYMASK::ANYMODIFIER). */
        SETofKEYMASK modifiers;
        /** @brief Protocol name: key; uses enum: 0=AnyKey. */
        KEYCODE      key;
        /** @brief Protocol name: pointer-mode; uses enum:
         *   0=Synchronous 1=Asynchronous. */
        uint8_t      pointer_mode;
        /** @brief Protocol name: keyboard-mode; uses enum:
         *   0=Synchronous 1=Asynchronous. */
        uint8_t      keyboard_mode;
    private:
        /** @brief Ignored bytes. */
        uint8_t      _unused[3];
    };
    /** @brief [key](#Encoding::key) enum names. */
    inline static const
    std::vector< std::string_view >& key_names {
        protocol::enum_names::key };
    /** @brief [pointer-mode](#Encoding::pointer_mode) enum names. */
    inline static const
    std::vector< std::string_view >& pointer_mode_names {
        protocol::enum_names::input_mode };
    /** @brief [keyboard-mode](#Encoding::keyboard_mode) enum names. */
    inline static const
    std::vector< std::string_view >& keyboard_mode_names {
        protocol::enum_names::input_mode };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %UngrabKey request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct UngrabKey : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal UNGRABKEY(34). */
        uint8_t  opcode;
        /** @brief Protocol name: key; uses enum: 0=AnyKey. */
        KEYCODE  key;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 3
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: grab-window. */
        WINDOW       grab_window;
        /** @brief Protocol name: modifiers; may use special flag
         *   [AnyModifier](#SETofKEYMASK::ANYMODIFIER). */
        SETofKEYMASK modifiers;
    private:
        /** @brief Ignored bytes. */
        uint8_t      _unused[2];
    };
    /** @brief [key](#Header::key) enum names. */
    inline static const
    std::vector< std::string_view >& key_names {
        protocol::enum_names::key };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %AllowEvents request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct AllowEvents : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal ALLOWEVENTS(35). */
        uint8_t  opcode;
        /** @brief Protocol name: mode; uses enum:
         *   0=AsyncPointer 1=SyncPointer 2=ReplayPointer 3=AsyncKeyboard
         *   4=SyncKeyboard 5=ReplayKeyboard 6=AsyncBoth 7=SyncBoth. */
        uint8_t  mode;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 2
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: time; uses enum: 0=CurrentTime. */
        TIMESTAMP time;
    };
    /** @brief [mode](#Header::mode) enum names. */
    inline static const
    std::vector< std::string_view > mode_names {
        "AsyncPointer",    // 0
        "SyncPointer",     // 1
        "ReplayPointer",   // 2
        "AsyncKeyboard",   // 3
        "SyncKeyboard",    // 4
        "ReplayKeyboard",  // 5
        "AsyncBoth",       // 6
        "SyncBoth"         // 7
    };
    /** @brief [time](#Encoding::time) enum names. */
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

namespace impl {

/**
 * @brief Interface class representing minimal [encoding] for X11 requests (eg
 *   GrabServer).
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#request_format
 */
struct SimpleRequest : public Request {
    /** @brief Total encoding size in bytes (before suffixes). */
    static constexpr size_t BASE_ENCODING_SZ { sizeof( Header ) };
    virtual ~SimpleRequest() = 0;
};

}  // namespace impl

/**
 * @brief Represents X11 %GrabServer request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GRABSERVER(36) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GrabServer : public impl::SimpleRequest {
};
/**
 * @brief Represents X11 %UngrabServer request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   UNGRABSERVER(37) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct UngrabServer : public impl::SimpleRequest {
};
/**
 * @brief Represents X11 %QueryPointer request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   QUERYPOINTER(38) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct QueryPointer : public impl::SimpleWindowRequest {
    /**
     * @brief Represents X11 %QueryPointer reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: same-screen. */
            BOOL     same_screen;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // 0
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header          header;
            /** @brief Protocol name: root. */
            WINDOW          root;
            /** @brief Protocol name: child; uses enum: 0=None. */
            WINDOW          child;
            /** @brief Protocol name: root-x. */
            INT16           root_x;
            /** @brief Protocol name: root-y. */
            INT16           root_y;
            /** @brief Protocol name: win-x. */
            INT16           win_x;
            /** @brief Protocol name: win-y. */
            INT16           win_y;
            /** @brief Protocol name: mask. */
            SETofKEYBUTMASK mask;
        private:
            /** @brief Ignored bytes. */
            uint8_t         _unused[6];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
        /** @brief [child](#Encoding::child) enum names. */
        inline static const
        std::vector< std::string_view >& child_names {
            protocol::enum_names::zero_none };
    };
};
/**
 * @brief Represents X11 %GetMotionEvents request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETMOTIONEVENTS(39) and `tl_aligned_units` of 4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetMotionEvents : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: grab-window. */
        WINDOW    window;
        /** @brief Protocol name: start; uses enum: 0=CurrentTime. */
        TIMESTAMP start;
        /** @brief Protocol name: stop; uses enum: 0=CurrentTime. */
        TIMESTAMP stop;
    };
    /** @brief [start](#Encoding::start) enum names. */
    inline static const
    std::vector< std::string_view >& start_names {
        protocol::enum_names::time };
    /** @brief [stop](#Encoding::stop) enum names. */
    inline static const
    std::vector< std::string_view >& stop_names {
        protocol::enum_names::time };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %GetMotionEvents reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Reply::Header](#requests::Reply::Header).
         * @note Followed by suffix:
         *   - `LISTofTIMECOORD events` of (sizeof(TIMECOORD) *
         *   [events_ct](#Encoding::events_ct))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;  // `extra_aligned_units` of 2 * events_ct
            /** @brief `events` suffix length in [TIMECOORD](#TIMECOORD)s. */
            uint32_t events_ct;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused[20];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
        /**
         * @brief Time coordinate record in suffix `events`.
         */
        struct [[gnu::packed]] TIMECOORD : public protocol::impl::Struct {
            /** @brief Protocol name: time. */
            TIMESTAMP time;
            /** @brief Protocol name: x. */
            INT16     x;
            /** @brief Protocol name: y. */
            INT16     y;
        };
    };
};
/**
 * @brief Represents X11 %TranslateCoordinates request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   TRANSLATECOORDINATES(40) and `tl_aligned_units` of 4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct TranslateCoordinates : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: src-window. */
        WINDOW src_window;
        /** @brief Protocol name: dst-window. */
        WINDOW dst_window;
        /** @brief Protocol name: src-x. */
        INT16  src_x;
        /** @brief Protocol name: src-y. */
        INT16  src_y;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %TranslateCoordinates reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: same-screen. */
            BOOL     same_screen;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // 0
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
            /** @brief Protocol name: child; uses enum: 0=None. */
            WINDOW  child;
            /** @brief Protocol name: dst-x. */
            INT16   dst_x;
            /** @brief Protocol name: dst-y. */
            INT16   dst_y;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[16];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
        /** @brief [child](#Encoding::child) enum names. */
        inline static const
        std::vector< std::string_view >& child_names {
            protocol::enum_names::zero_none };
    };
};
/**
 * @brief Represents X11 %WarpPointer request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   WARPPOINTER(41) and `tl_aligned_units` of 6.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct WarpPointer : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: src-window; uses enum: 0=None. */
        WINDOW src_window;
        /** @brief Protocol name: dst-window; uses enum: 0=None. */
        WINDOW dst_window;
        /** @brief Protocol name: src-x. */
        INT16  src_x;
        /** @brief Protocol name: src-y. */
        INT16  src_y;
        /** @brief Protocol name: src-width. */
        CARD16 src_width;
        /** @brief Protocol name: src-height. */
        CARD16 src_height;
        /** @brief Protocol name: dst-x. */
        INT16  dst_x;
        /** @brief Protocol name: dst-y. */
        INT16  dst_y;
    };
    /** @brief [src-window](#Encoding::src_window) enum names. */
    inline static const
    std::vector< std::string_view >& src_window_names {
        protocol::enum_names::zero_none };
    /** @brief [dst-window](#Encoding::dst_window) enum names. */
    inline static const
    std::vector< std::string_view >& dst_window_names {
        protocol::enum_names::zero_none };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %SetInputFocus request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SetInputFocus : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal SETINPUTFOCUS(42). */
        uint8_t  opcode;
        /** @brief Protocol name: revert-to; uses enum:
         *   0=None 1=PointerRoot 2=Parent. */
        uint8_t  revert_to;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 3
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: focus; uses enum: 0=None 1=PointerRoot. */
        WINDOW    focus;
        /** @brief Protocol name: focus; uses enum: 0=CurrentTime. */
        TIMESTAMP time;
    };
    /** @brief [revert-to](#Header::revert_to) enum names. */
    inline static const
    std::vector< std::string_view >& revert_to_names {
        protocol::enum_names::input_focus };
    /** @brief [focus](#Encoding::focus) enum names. */
    inline static const
    std::vector< std::string_view >& focus_names {
        protocol::enum_names::input_focus };
    /** @brief [focus](#Encoding::focus) maximum value to use enum. */
    static constexpr uint8_t FOCUS_ENUM_MAX { 1 };
    /** @brief [time](#Encoding::time) enum names. */
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GetInputFocus request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETINPUTFOCUS(43) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetInputFocus : public impl::SimpleRequest {
    /**
     * @brief Represents X11 %GetInputFocus reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: revert-to; uses enum:
             *   0=None 1=PointerRoot 2=Parent. */
            uint8_t  revert_to;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // 0
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
            /** @brief Protocol name: focus; uses enum: 0=None 1=PointerRoot. */
            WINDOW  focus;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[20];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        /** @brief [revert-to](#Header::revert_to) enum names. */
        inline static const
        std::vector< std::string_view >& revert_to_names {
            protocol::enum_names::input_focus };
        /** @brief [focus](#Encoding::focus) enum names. */
        inline static const
        std::vector< std::string_view >& focus_names {
            protocol::enum_names::input_focus };
        /** @brief [focus](#Encoding::focus) maximum value to use enum. */
        static constexpr int FOCUS_ENUM_MAX { 1 };
    };
};
/**
 * @brief Represents X11 %QueryKeymap request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   QUERYKEYMAP(44) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct QueryKeymap : public impl::SimpleRequest {
    /**
     * @brief Represents X11 %QueryKeymap reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header header;  // `extra_aligned_units` of 2
            /** @brief Protocol name: keys; LISTofCARD8 32B bit-vector. */
            CARD8  keys[32];
        };
        static_assert( sizeof( Encoding ) ==
                       DEFAULT_ENCODING_SZ + ( 2 * ALIGN ) );
    };
};
/**
 * @brief Represents X11 %OpenFont request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   OPENFONT(45) and `tl_aligned_units` of 3+pad(name_len)/4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct OpenFont : public Request {
    /**
     * @brief Fixed encoding betweem [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `STRING8 name` of pad(n)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: fid. */
        FONT     fid;
        /** @brief Length of suffix `name` in bytes. */
        uint16_t name_len;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[2];
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %CloseFont request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   CLOSEFONT(46) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct CloseFont : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: font. */
        FONT font;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

namespace impl {

// TBD wrap in interface class for QueryFont, ListFontsWithInfo?
/**
 * @brief Represents encoding of type used in replies to core font requests (eg
 *   QueryFont, ListFontsWithInfo).
 */
struct [[gnu::packed]] FONTPROP : public protocol::impl::Struct {
    /** @brief Protocol name: name. */
    ATOM     name;
    /** @brief Protocol name: value. */
    uint32_t value;
};
/**
 * @brief Represents encoding of type used in replies to core font requests (eg
 *   QueryFont, ListFontsWithInfo).
 */
struct [[gnu::packed]] CHARINFO : public protocol::impl::Struct {
    /** @brief Protocol name: left-side-bearing. */
    INT16  left_side_bearing;
    /** @brief Protocol name: right-side-bearing. */
    INT16  right_side_bearing;
    /** @brief Protocol name: character-width. */
    INT16  character_width;
    /** @brief Protocol name: ascent. */
    INT16  ascent;
    /** @brief Protocol name: descent. */
    INT16  descent;
    /** @brief Protocol name: attributes. */
    CARD16 attributes;
};

}  // namespace impl

/**
 * @brief Represents X11 %QueryFont request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   QUERYFONT(47) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct QueryFont : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: font. */
        FONTABLE font;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %QueryFont reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        using FONTPROP = impl::FONTPROP;
        using CHARINFO = impl::CHARINFO;
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         * @note Followed by suffixes:
         *   - `LISTofFONTPROP properties` of
         *   (8 * [properties_ct](#Encoding::properties_ct))B
         *   - `LISTofCHARINFO char-infos` of
         *   (12 * [char_infos_ct](#Encoding::char_infos_ct))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;  // `extra_aligned_units` of 7 +
                              //   (2 * properties_ct) + (3 * char_infos_ct)
            /** @brief Protocol name: min-bounds. */
            CHARINFO min_bounds;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused1[4];
        public:
            /** @brief Protocol name: max-bounds. */
            CHARINFO max_bounds;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused2[4];
        public:
            /** @brief Protocol name: min-char-or-byte2. */
            CARD16   min_char_or_byte2;
            /** @brief Protocol name: max-char-or-byte2. */
            CARD16   max_char_or_byte2;
            /** @brief Protocol name: default-char. */
            CARD16   default_char;
            /** @brief Length of suffix `properties` in [FONTPROP](#FONTPROP)s. */
            uint16_t properties_ct;
            /** @brief Protocol name: draw-direction; uses enum:
             *   0=LeftToRight 1=RightToLeft. */
            uint8_t  draw_direction;
            /** @brief Protocol name: min-byte1. */
            CARD8    min_byte1;
            /** @brief Protocol name: max-byte1. */
            CARD8    max_byte1;
            /** @brief Protocol name: all-chars-exist. */
            BOOL     all_chars_exist;
            /** @brief Protocol name: font-ascent. */
            INT16    font_ascent;
            /** @brief Protocol name: font-descent. */
            INT16    font_descent;
            /** @brief Length of suffix `char-infos` in [CHARINFO](#CHARINFO)s. */
            uint32_t char_infos_ct;
        };
        static_assert( sizeof( Encoding ) ==
                       DEFAULT_ENCODING_SZ + ( 7 * ALIGN ) );
        /** @brief [draw-direction](#Encoding::draw_direction) enum names. */
        inline static const
        std::vector< std::string_view >& draw_direction_names {
            protocol::enum_names::draw_direction };
    };
};
/**
 * @brief Represents X11 %QueryTextExtents request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct QueryTextExtents : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal QUERYTEXTEXTENTS(48). */
        uint8_t  opcode;
        /** @brief True if `string` padded by 2. */
        BOOL     odd_length;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 2+pad(2n)/4
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffix:
     *   - `STRING16 string` of pad(2n)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: font. */
        FONTABLE   font;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %QueryTextExtents reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: draw-direction; uses enum:
             *   0=LeftToRight 1=RightToLeft. */
            uint8_t  draw_direction;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // 0
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
            /** @brief Protocol name: font-ascent. */
            INT16   font_ascent;
            /** @brief Protocol name: font-descent. */
            INT16   font_descent;
            /** @brief Protocol name: overall-ascent. */
            INT16   overall_ascent;
            /** @brief Protocol name: overall-descent. */
            INT16   overall_descent;
            /** @brief Protocol name: overall-width. */
            INT32   overall_width;
            /** @brief Protocol name: overall-left. */
            INT32   overall_left;
            /** @brief Protocol name: overall-right. */
            INT32   overall_right;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[4];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        /** @brief [draw-direction](#Header::draw_direction) enum names. */
        inline static const
        std::vector< std::string_view >& draw_direction_names {
            protocol::enum_names::draw_direction };
    };
};

namespace impl {

/**
 * @brief Represents shared [encoding] of list fonts requests (eg ListFonts,
 *   ListFontsWithInfo).
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ListFontsRequest : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `STRING8 pattern` of pad(pattern_len)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: max-names. */
        CARD16   max_names;
        /** @brief Length of suffix `pattern` in bytes. */
        uint16_t pattern_len;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    virtual ~ListFontsRequest() = 0;
};

}  // namespace impl

/**
 * @brief Represents X11 %ListFonts request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   LISTFONTS(49) and `tl_aligned_units` of 2+pad(Encoding::pattern_len)/4.
 *   [Encoding](#impl::ListFontsRequest::Encoding) followed by `STRING8 pattern` of
 *   pad(Encoding::pattern_len)B.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ListFonts : public impl::ListFontsRequest {
    /**
     * @brief Represents X11 %ListFonts reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         * @note Followed by suffix:
         *   - `LISTofSTR names` of pad(n)B (LISTofSTRING8 in [description])
         * [description]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#requests:ListFonts
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;  // `extra_aligned_units` of pad(n)/4.
            /** @brief Length of suffix `names` in [STR](#protocol::STR)s. */
            CARD16  names_ct;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused2[22];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %ListFontsWithInfo request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   LISTFONTSWITHINFO(50) and `tl_aligned_units` of 2+pad(Encoding::pattern_len)/4.
 *   [Encoding](#impl::ListFontsRequest::Encoding) followed by `STRING8 pattern` of
 *   pad(Encoding::pattern_len)B.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ListFontsWithInfo : public impl::ListFontsRequest {
    /**
     * @brief Represents X11 %ListFontsWithInfo reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        using FONTPROP = impl::FONTPROP;
        using CHARINFO = impl::CHARINFO;
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            union [[gnu::packed]] {
                /** @brief Length of Encoding suffix `name` in bytes (never 0). */
                uint8_t name_len;
                /** @brief Protocol name: last-reply (0). */
                uint8_t last_reply;
            };
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // 7 + (2 * properties_ct) + pad(name_len) / 4
        };
        /** @brief [name_len](#Header::name_len) sentinel value to indicate last-reply. */
        static constexpr uint8_t LAST_REPLY {};
        /**
         * @brief Fixed encoding, including [Header](#Header).
         * @note Followed by suffixes:
         *   - `LISTofFONTPROP properties` of
         *   (8 * [properties_ct](#Encoding::properties_ct))B
         *   - `STRING8 name` of
         *   [pad](#X11ProtocolParser::_Alignment::pad)([name_len](#Header::name_len))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;
            /** @brief Protocol name: min-bounds. */
            CHARINFO min_bounds;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused1[4];
        public:
            /** @brief Protocol name: max-bounds. */
            CHARINFO max_bounds;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused2[4];
        public:
            /** @brief Protocol name: min-char-or-byte2. */
            CARD16   min_char_or_byte2;
            /** @brief Protocol name: max-char-or-byte2. */
            CARD16   max_char_or_byte2;
            /** @brief Protocol name: default-char. */
            CARD16   default_char;
            /** @brief Length of suffix `properties` in [FONTPROP](#FONTPROP)s. */
            uint16_t properties_ct;
            /** @brief Protocol name: draw-direction; uses enum:
             *   0=LeftToRight 1=RightToLeft. */
            uint8_t  draw_direction;
            /** @brief Protocol name: min-byte1. */
            CARD8    min_byte1;
            /** @brief Protocol name: max-byte1. */
            CARD8    max_byte1;
            /** @brief Protocol name: all-chars-exist. */
            BOOL     all_chars_exist;
            /** @brief Protocol name: font-ascent. */
            INT16    font_ascent;
            /** @brief Protocol name: font-descent. */
            INT16    font_descent;
            /** @brief Protocol name: replies-hint. */
            CARD32   replies_hint;
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ + ( 7 * ALIGN ) );
        /** @brief [draw-direction](#Encoding::draw_direction) enum names. */
        inline static const
        std::vector< std::string_view >& draw_direction_names {
            protocol::enum_names::draw_direction };
    };
};
/**
 * @brief Represents X11 %SetFontPath request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   SETFONTPATH(51) and `tl_aligned_units` of 2+pad(n)/4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SetFontPath : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofSTR path` of pad(n)B (`LISTofSTRING8` in [description])
     * [description]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#requests:SetFontPath
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Length of suffix `path` in [STR](#protocol::STR)s. */
        CARD16  path_ct;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[2];
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GetFontPath request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETFONTPATH(52) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetFontPath : public impl::SimpleRequest {
    /**
     * @brief Represents X11 %GetFontPath reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         * @note Followed by suffix:
         *   - `LISTofSTR path` of [pad](#X11ProtocolParser::_Alignment::pad)(n)B
         *   (`LISTofSTRING8` in [description])
         * [description]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#requests:GetFontPath
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;  // `extra_aligned_units` of pad(n)/4.
            /** @brief Length of suffix `path` in [STR](#protocol::STR)s. */
            CARD16  path_ct;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[22];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %CreatePixmap request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct CreatePixmap : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal CREATEPIXMAP(53). */
        uint8_t  opcode;
        /** @brief Protocol name: depth. */
        CARD8    depth;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 2
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: pid. */
        PIXMAP   pid;
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: width. */
        CARD16   width;
        /** @brief Protocol name: height. */
        CARD16   height;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %FreePixmap request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   FREEPIXMAP(54) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct FreePixmap : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: pixmap. */
        PIXMAP     pixmap;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

namespace impl {

/**
 * @brief Represents shared [encoding] of GCONTEXT requests that use a value-list,
 *   (eg CreateGC, ChangeGC).
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GCRequest : public Request {
    /** @brief List of all parseable types potentially stored as VALUEs in
     *    `value-list`.
     *  - `uint8_t function` 0=Clear 1=And 2=AndReverse 3=Copy 4=AndInverted
     *                       5=NoOp 6=Xor 7=Or 8=Nor 9=Equiv 10=Invert 11=OrReverse
     *                       12=CopyInverted 13=OrInverted 14=Nand 15=Set
     *  - `CARD32  plane-mask`
     *  - `CARD32  foreground`
     *  - `CARD32  background`
     *  - `CARD16  line-width`
     *  - `uint8_t line-style` 0=Solid 1=OnOffDash 2=DoubleDash
     *  - `uint8_t cap-style` 0=NotLast 1=Butt 2=Round 3=Projecting
     *  - `uint8_t join-style` 0=Miter 1=Round 2=Bevel
     *  - `uint8_t fill-style` 0=Solid 1=Tiled 2=Stippled 3=OpaqueStippled
     *  - `uint8_t fill-rule` 0=EvenOdd 1=Winding
     *  - `PIXMAP  tile`
     *  - `PIXMAP  stipple`
     *  - `INT16   tile-stipple-x-origin`
     *  - `INT16   tile-stipple-y-origin`
     *  - `FONT    font`
     *  - `uint8_t subwindow-mode` 0=ClipByChildren 1=IncludeInferiors
     *  - `BOOL    graphics-exposures`
     *  - `INT16   clip-x-origin`
     *  - `INT16   clip-y-origin`
     *  - `PIXMAP  clip-mask` 0=None
     *  - `CARD16  dash-offset`
     *  - `CARD8   dashes`
     *  - `uint8_t arc-mode` 0=Chord 1=PieSlice */
    inline static constexpr
    std::tuple<
        uint8_t, CARD32, CARD32, CARD32, CARD16, uint8_t, uint8_t, uint8_t,
        uint8_t, uint8_t, PIXMAP, PIXMAP, INT16, INT16, FONT, uint8_t, BOOL,
        INT16, INT16, PIXMAP, CARD16, CARD8, uint8_t
        > value_types {};
    /** @brief `value-mask` flag names/`value-list` member names. */
    inline static const
    std::vector< std::string_view >& value_names {
        protocol::enum_names::gc_value_mask };
    /** @brief VALUE `function` enum names. */
    inline static const
    std::vector< std::string_view >& function_names {
        protocol::enum_names::gc_value_function };
    /** @brief VALUE `line-style` enum names. */
    inline static const
    std::vector< std::string_view >& line_style_names {
        protocol::enum_names::gc_value_line_style };
    /** @brief VALUE `cap-style` enum names. */
    inline static const
    std::vector< std::string_view >& cap_style_names {
        protocol::enum_names::gc_value_cap_style };
    /** @brief VALUE `join-style` enum names. */
    inline static const
    std::vector< std::string_view >& join_style_names {
        protocol::enum_names::gc_value_join_style };
    /** @brief VALUE `fill-style` enum names. */
    inline static const
    std::vector< std::string_view >& fill_style_names {
        protocol::enum_names::gc_value_fill_style };
    /** @brief VALUE `fill-rule` enum names. */
    inline static const
    std::vector< std::string_view >& fill_rule_names {
        protocol::enum_names::gc_value_fill_rule };
    /** @brief VALUE `subwindow-mode` enum names. */
    inline static const
    std::vector< std::string_view >& subwindow_mode_names {
        protocol::enum_names::gc_value_subwindow_mode };
    /** @brief VALUE `clip-mask` enum names. */
    inline static const
    std::vector< std::string_view >& clip_mask_names {
        protocol::enum_names::zero_none };
    /** @brief VALUE `arc-mode` enum names. */
    inline static const
    std::vector< std::string_view >& arc_mode_names {
        protocol::enum_names::gc_value_arc_mode };
    virtual ~GCRequest() = 0;
};

}  // namespace impl

/**
 * @brief Represents X11 %CreateGC request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   CREATEGC(55) and `tl_aligned_units` of 4+n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct CreateGC : public impl::GCRequest {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofVALUE value-list` of 4nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cid. */
        GCONTEXT cid;
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: value-mask (4B BITMASK, n bits set to 1). */
        uint32_t value_mask;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %ChangeGC request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   CHANGEGC(56) and `tl_aligned_units` of 3+n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ChangeGC : public impl::GCRequest {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofVALUE value-list` of 4nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
        /** @brief Protocol name: value-mask (4B BITMASK, n bits set to 1). */
        uint32_t value_mask;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %CopyGC request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   COPYGC(57) and `tl_aligned_units` of 4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct CopyGC : public impl::GCRequest {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: src-gc. */
        GCONTEXT src_gc;
        /** @brief Protocol name: dst-gc. */
        GCONTEXT dst_gc;
        /** @brief Protocol name: value-mask (4B BITMASK, n bits set to 1). */
        uint32_t value_mask;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %SetDashes request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   SETDASHES(58) and `tl_aligned_units` of 3+pad(Encoding::dashes_len)/4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SetDashes : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofCARD8 dashes` of pad(dashes_len)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
        /** @brief Protocol name: dash-offset. */
        CARD16   dash_offset;
        /** @brief Length of suffix `dashes` in bytes. */
        uint16_t dashes_len;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %SetClipRectangles request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SetClipRectangles : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal SETCLIPRECTANGLES(59). */
        uint8_t  opcode;
        /** @brief Protocol name: ordering; uses enum:
         *   0=UnSorted 1=YSorted 2=YXSorted 3=YXBanded. */
        uint8_t  ordering;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 3+2n
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofRECTANGLE rectangles` of 8nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
        /** @brief Protocol name: clip-x-origin. */
        INT16    clip_x_origin;
        /** @brief Protocol name: clip-y-origin. */
        INT16    clip_y_origin;
    };
    /** @brief [ordering](#Header::ordering) enum names. */
    inline static const
    std::vector< std::string_view > ordering_names {
        "UnSorted",  // 0
        "YSorted",   // 1
        "YXSorted",  // 2
        "YXBanded"   // 3
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %FreeGC request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   FREEGC(60) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct FreeGC : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %ClearArea request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ClearArea : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal CLEARAREA(61). */
        uint8_t  opcode;
        /** @brief Protocol name: exposures. */
        BOOL     exposures;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 4
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: window. */
        WINDOW window;
        /** @brief Protocol name: x. */
        INT16  x;
        /** @brief Protocol name: y. */
        INT16  y;
        /** @brief Protocol name: width. */
        CARD16 width;
        /** @brief Protocol name: height. */
        CARD16 height;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %CopyArea request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   COPYAREA(62) and `tl_aligned_units` of 7.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct CopyArea : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: src-drawable. */
        DRAWABLE src_drawable;
        /** @brief Protocol name: dst-drawable. */
        DRAWABLE dst_drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
        /** @brief Protocol name: src-x. */
        INT16    src_x;
        /** @brief Protocol name: src-y. */
        INT16    src_y;
        /** @brief Protocol name: dst-x. */
        INT16    dst_x;
        /** @brief Protocol name: dst-y. */
        INT16    dst_y;
        /** @brief Protocol name: width. */
        CARD16   width;
        /** @brief Protocol name: height. */
        CARD16   height;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %CopyPlane request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   COPYPLANE(63) and `tl_aligned_units` of 8.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct CopyPlane : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: src-drawable. */
        DRAWABLE src_drawable;
        /** @brief Protocol name: dst-drawable. */
        DRAWABLE dst_drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
        /** @brief Protocol name: src-x. */
        INT16    src_x;
        /** @brief Protocol name: src-y. */
        INT16    src_y;
        /** @brief Protocol name: dst-x. */
        INT16    dst_x;
        /** @brief Protocol name: dst-y. */
        INT16    dst_y;
        /** @brief Protocol name: width. */
        CARD16   width;
        /** @brief Protocol name: width. */
        CARD16   height;
        /** @brief Protocol name: bit-plane. */
        CARD32   bit_plane;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

namespace impl {

/**
 * @brief Represents shared [encoding] of simple Poly* requests (eg PolyPoint,
 *   PolyLine).
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct PolyPointRequest : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier. */
        uint8_t  opcode;
        /** @brief Protocol name: coordinate-mode; uses enum 0=Origin 1=Previous. */
        uint8_t  coordinate_mode;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 3+n
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofPOINT points` of 4nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
    };
    /** @brief [coordinate-mode](#Header::coordinate_mode) enum names. */
    inline static const
    std::vector< std::string_view >& coordinate_mode_names {
        protocol::enum_names::poly_coordinate_mode };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    virtual ~PolyPointRequest() = 0;
};

}  // namespace impl

/**
 * @brief Represents X11 %PolyPoint request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   POLYPOINT(64) and `tl_aligned_units` of 3+n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct PolyPoint : public impl::PolyPointRequest {
};
/**
 * @brief Represents X11 %PolyLine request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   POLYLINE(65) and `tl_aligned_units` of 3+n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct PolyLine : public impl::PolyPointRequest {
};
// TBD PolySegment, PolyRectangle, PolyArc, PolyFillRectangle, PolyFillArc parent?
/**
 * @brief Represents X11 %PolySegment request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   POLYSEGMENT(66) and `tl_aligned_units` of 3+2n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct PolySegment : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofSEGMENT segments` of 8nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
    };
    /**
     * @brief Segment coordinates in suffix `segments`.
     */
    struct [[gnu::packed]] SEGMENT : public protocol::impl::Struct {
        /** @brief Protocol name: x1. */
        INT16 x1;
        /** @brief Protocol name: y1. */
        INT16 y1;
        /** @brief Protocol name: x2. */
        INT16 x2;
        /** @brief Protocol name: y2. */
        INT16 y2;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %PolyRectangle request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   POLYRECTANGLE(67) and `tl_aligned_units` of 3+2n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct PolyRectangle : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffixes.
     * @note Followed by suffix:
     *   - `LISTofRECTANGLE rectangles` of 8nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %PolyArc request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   POLYARC(68) and `tl_aligned_units` of 3+3n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct PolyArc : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofARC arcs` of 12nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %FillPoly request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   FILLPOLY(69) and `tl_aligned_units` of 4+n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct FillPoly : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofPOINT points` of 4nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
        /** @brief Protocol name: shape; uses enum:
         *   0=Complex 1=Nonconvex 2=Convex. */
        uint8_t  shape;
        /** @brief Protocol name: coordinate-mode; uses enum: 0=Origin 1=Previous. */
        uint8_t  coordinate_mode;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[2];
    };
    /** @brief [shape](#Encoding::shape) enum names. */
    inline static const
    std::vector< std::string_view > shape_names {
        "Complex",    // 0
        "Nonconvex",  // 1
        "Convex"      // 2
    };
    /** @brief [coordinate-mode](#Encoding::coordinate_mode) enum names. */
    inline static const
    std::vector< std::string_view >& coordinate_mode_names {
        protocol::enum_names::poly_coordinate_mode };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %PolyFillRectangle request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   POLYFILLRECTANGLE(70) and `tl_aligned_units` of 3+2n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct PolyFillRectangle : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofRECTANGLE rectangles` of 8nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %PolyFillArc request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   POLYFILLARC(71) and `tl_aligned_units` of 3+3n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct PolyFillArc : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofARC arcs` of 12nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %PutImage request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct PutImage : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal PUTIMAGE(72). */
        uint8_t  opcode;
        /** @brief Protocol name: format; uses enum: 0=Bitmap 1=XYPixmap 2=ZPixmap. */
        uint8_t  format;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 6+pad(n)/4
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofBYTE data` of pad(n)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
        /** @brief Protocol name: width. */
        CARD16   width;
        /** @brief Protocol name: height. */
        CARD16   height;
        /** @brief Protocol name: dst-x. */
        INT16    dst_x;
        /** @brief Protocol name: dst-y. */
        INT16    dst_y;
        /** @brief Protocol name: left-pad. */
        CARD8    left_pad;
        /** @brief Protocol name: depth. */
        CARD8    depth;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[2];
    };
    /** @brief [format](#Header::format) enum names. */
    inline static const
    std::vector< std::string_view >& format_names {
        protocol::enum_names::image_format };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GetImage request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetImage : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal GETIMAGE(73). */
        uint8_t  opcode;
        /** @brief Protocol name: format; uses enum: 1=XYPixmap 2=ZPixmap. */
        uint8_t  format;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 5
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: x. */
        INT16    x;
        /** @brief Protocol name: y. */
        INT16    y;
        /** @brief Protocol name: width. */
        CARD16   width;
        /** @brief Protocol name: height. */
        CARD16   height;
        /** @brief Protocol name: plane-mask. */
        CARD32   plane_mask;
    };
    /** @brief [format](#Header::format) enum names. */
    inline static const
    std::vector< std::string_view >& format_names {
        protocol::enum_names::image_format };
    /** @brief [format](#Header::format) minimum value to use enum. */
    static constexpr uint8_t FORMAT_ENUM_MIN { 1 };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %GetImage reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: depth. */
            CARD8    depth;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // pad(n) / 4
        };
        /**
         * @brief Fixed encoding between [Header](#Header) and suffix.
         * @note Followed by suffix:
         *   - `LISTofBYTE data` of pad(n)B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;
            /** @brief Protocol name: visual; uses enum: 0=None. */
            VISUALID visual;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused[20];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        /** @brief [visual](#Encoding::visual) enum names. */
        inline static const
        std::vector< std::string_view >& visual_names {
            protocol::enum_names::zero_none };
    };
};

namespace impl {

/**
 * @brief Represents shared [encoding] of PolyText requests (eg PolyText8,
 *   PolyText16).
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct PolyTextRequest : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofTEXTITEM8 items` of pad(n)B
     *     or
     *   - `LISTofTEXTITEM16 items` of pad(n)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
        /** @brief Protocol name: x. */
        INT16    x;
        /** @brief Protocol name: y. */
        INT16    y;
    };
    /**
     * @brief Stores X11 font code in network byte order; used in
     *   [TEXTITEM8](#PolyText8::TEXTITEM8) and [TEXTITEM16](#PolyText16::TEXTITEM16)
     *   unions.
     * @see [X11 fonts](https://www.x.org/wiki/guide/fonts/)
     */
    struct [[gnu::packed]] FONT : public protocol::impl::Struct {
        /** @brief Sentinel value to differentiate by first byte from
         *   alternative TEXTELT union members. */
        static constexpr uint8_t FONT_SHIFT { 255 };
        /** @brief Protocol name: font-shift (must equal #FONT_SHIFT). */
        uint8_t font_shift;
        /** @brief Font code convertible to [protocol::FONT](#protocol::FONT),
         *   from most- to least-significant byte (network byte order). */
        uint8_t font_bytes[4];
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    virtual ~PolyTextRequest() = 0;
};

}  // namespace impl

/**
 * @brief Represents X11 %PolyText8 request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   POLYTEXT8(74) and `tl_aligned_units` of 4+pad(n)/4.
 *   [Encoding](#PolyTextRequest::Encoding) followed by suffix:
 *   - `LISTofTEXTITEM8 items` of pad(n)B
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct PolyText8 : public impl::PolyTextRequest {
    /**
     * @brief [Encoding](#PolyTextRequest::Encoding) suffix list members.
     */
    union TEXTITEM8 {
        impl::PolyTextRequest::FONT font;
        /**
         * @brief Header for variable length single-byte char string.
         * @note Followed by suffix:
         *   - `STRING8 string` of string_lenB
         */
        struct [[gnu::packed]] TEXTELT8 : public protocol::impl::StructWithSuffixes {
            /** @brief Length of suffix `string` in bytes (cannot equal
             *    [FONT_SHIFT](#PolyTextRequest::FONT::FONT_SHIFT)). */
            uint8_t string_len;
            /** @brief Protocol name: delta. */
            INT8    delta;
        } text_element;
    };
};
/**
 * @brief Represents X11 %PolyText16 request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   POLYTEXT16(75) and `tl_aligned_units` of 4+pad(n)/4.
 *   [Encoding](#PolyTextRequest::Encoding) followed by suffix:
 *   - `LISTofTEXTITEM16 items` of pad(n)B
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct PolyText16 : public impl::PolyTextRequest {
    /**
     * @brief [Encoding](#PolyTextRequest::Encoding) suffix list members.
     */
    union TEXTITEM16 {
        impl::PolyTextRequest::FONT font;
        /**
         * @brief Header for variable length 2-byte char string.
         * @note Followed by suffix:
         *   - `STRING16 string` of 2string_lenB
         */
        struct [[gnu::packed]] TEXTELT16 : public protocol::impl::StructWithSuffixes {
            /** @brief Length of suffix `string` in [CHAR2B](#protocol::CHAR2B)
             *    (cannot equal [FONT_SHIFT](#PolyTextRequest::FONT::FONT_SHIFT)). */
            uint8_t string_2B_len;
            /** @brief Protocol name: delta. */
            INT8    delta;
        } text_element;
    };
};
/**
 * @brief Represents X11 %ImageText8 request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ImageText8 : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal IMAGETEXT8(76). */
        uint8_t  opcode;
        /** @brief Length of suffix `string` in bytes. */
        uint8_t  string_len;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 4+(n+p)/4
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffix:
     *   - `STRING8 string` of pad(n)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
        /** @brief Protocol name: x. */
        INT16    x;
        /** @brief Protocol name: y. */
        INT16    y;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %ImageText16 request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ImageText16 : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal IMAGETEXT16(77). */
        uint8_t  opcode;
        /** @brief Length of suffix `string` in [CHAR2B](#protocol::CHAR2B)s. */
        uint8_t  string_2B_len;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 4+(2n+p)/4
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffix:
     *   - `STRING16 string` of pad(2n)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: gc. */
        GCONTEXT gc;
        /** @brief Protocol name: x. */
        INT16    x;
        /** @brief Protocol name: y. */
        INT16    y;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %CreateColormap request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct CreateColormap : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal CREATECOLORMAP(78). */
        uint8_t  opcode;
        /** @brief Protocol name: alloc; uses enum: 0=None 1=All. */
        uint8_t  alloc;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 4
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: mid. */
        COLORMAP mid;
        /** @brief Protocol name: window. */
        WINDOW   window;
        /** @brief Protocol name: visual. */
        VISUALID visual;
    };
    /** @brief [alloc](#Header::alloc) enum names. */
    inline static const
    std::vector< std::string_view > alloc_names {
        "None",  // 0
        "All"    // 1
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

namespace impl {

// TBD StoreColors, QueryColors?
/**
 * @brief Represents shared [encoding] of simple COLORMAP requests (eg
 *   FreeColormap).
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SimpleCmapRequest : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cmap. */
        COLORMAP cmap;
    };
    /** @brief Total encoding size in bytes (before suffixes). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    virtual ~SimpleCmapRequest() = 0;
};

}  // namespace impl

/**
 * @brief Represents X11 %FreeColormap request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   FREECOLORMAP(79) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct FreeColormap : public impl::SimpleCmapRequest {
};
/**
 * @brief Represents X11 %CopyColormapAndFree request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   COPYCOLORMAPANDFREE(80) and `tl_aligned_units` of 3.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct CopyColormapAndFree : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: mid. */
        COLORMAP mid;
        /** @brief Protocol name: src-cmap. */
        COLORMAP src_cmap;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %InstallColormap request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   INSTALLCOLORMAP(81) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct InstallColormap : public impl::SimpleCmapRequest {
};
/**
 * @brief Represents X11 %UninstallColormap request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   UNINSTALLCOLORMAP(82) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct UninstallColormap : public impl::SimpleCmapRequest {
};
/**
 * @brief Represents X11 %ListInstalledColormaps request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   LISTINSTALLEDCOLORMAPS(83) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ListInstalledColormaps : public impl::SimpleWindowRequest {
    /**
     * @brief Represents X11 %ListInstalledColormaps reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: depth. */
            CARD8    depth;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // cmaps_ct
        };
        /**
         * @brief Fixed encoding between [Header](#Header) and suffix.
         * @note Followed by suffix:
         *   - `LISTofCOLORMAP cmaps` of (4 * cmaps_ct)B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;
            /** @brief Length of suffix `cmaps` in [COLORMAP](#protocol::COLORMAP)s. */
            uint16_t cmaps_ct;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused[22];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %AllocColor request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   ALLOCCOLOR(84) and `tl_aligned_units` of 4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct AllocColor : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cmap. */
        COLORMAP cmap;
        /** @brief Protocol name: red. */
        CARD16   red;
        /** @brief Protocol name: green. */
        CARD16   green;
        /** @brief Protocol name: blue. */
        CARD16   blue;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[2];
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %AllocColor reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;  // `extra_aligned_units` of 0
            /** @brief Protocol name: red. */
            CARD16  red;
            /** @brief Protocol name: green. */
            CARD16  green;
            /** @brief Protocol name: blue. */
            CARD16  blue;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused1[2];
        public:
            /** @brief Protocol name: pixel. */
            CARD32  pixel;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused2[12];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %AllocNamedColor request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   ALLOCNAMEDCOLOR(85) and `tl_aligned_units` of 3+pad(Encoding::name_len)/4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct AllocNamedColor : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `STRING8 name` of pad(name_len)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cmap. */
        COLORMAP cmap;
        /** @brief Length of suffix `name` in bytes. */
        uint16_t name_len;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[2];
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %AllocNamedColor reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;  // `extra_aligned_units` of 0
            /** @brief Protocol name: pixel. */
            CARD32  pixel;
            /** @brief Protocol name: exact-red. */
            CARD16  exact_red;
            /** @brief Protocol name: exact-green. */
            CARD16  exact_green;
            /** @brief Protocol name: exact-blue. */
            CARD16  exact_blue;
            /** @brief Protocol name: visual-red. */
            CARD16  visual_red;
            /** @brief Protocol name: visual-green. */
            CARD16  visual_green;
            /** @brief Protocol name: visual-blue. */
            CARD16  visual_blue;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[8];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %AllocColorCells request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct AllocColorCells : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal ALLOCCOLORCELLS(86). */
        uint8_t  opcode;
        /** @brief Protocol name: contiguous. */
        BOOL     contiguous;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 3
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cmap. */
        COLORMAP cmap;
        /** @brief Protocol name: colors. */
        CARD16   colors;
        /** @brief Protocol name: planes. */
        CARD16   planes;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %AllocColorCells reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         * @note Followed by suffixes:
         *   - `LISTofCARD32 pixels` of (4 * [pixels_ct](#pixels_ct))B
         *   - `LISTofCARD32 masks` of (4 * [masks_ct](#masks_ct))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;  // `extra_aligned_units` of pixels_ct + masks_ct
            /** @brief Length of suffix `pixels` in CARD32s. */
            uint16_t pixels_ct;
            /** @brief Length of suffix `masks` in CARD32s. */
            uint16_t masks_ct;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused[20];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %AllocColorPlanes request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct AllocColorPlanes : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal ALLOCCOLORPLANES(87). */
        uint8_t  opcode;
        /** @brief Protocol name: contiguous. */
        BOOL     contiguous;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 4
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cmap. */
        COLORMAP cmap;
        /** @brief Protocol name: colors. */
        CARD16   colors;
        /** @brief Protocol name: reds. */
        CARD16   reds;
        /** @brief Protocol name: greens. */
        CARD16   greens;
        /** @brief Protocol name: blues. */
        CARD16   blues;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %AllocColorPlanes reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         * @note Followed by suffix:
         *   - `LISTofCARD32 pixels` of (4 * [pixels_ct](#pixels_ct))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;  // `extra_aligned_units` of pixels_ct
            /** @brief Length of suffix `pixels` in CARD32s. */
            uint16_t pixels_ct;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused1[2];
        public:
            /** @brief Protocol name: red-mask. */
            CARD32   red_mask;
            /** @brief Protocol name: green-mask. */
            CARD32   green_mask;
            /** @brief Protocol name: blue-mask. */
            CARD32   blue_mask;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused2[8];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %FreeColors request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   FREECOLORS(88) and `tl_aligned_units` of 3+n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct FreeColors : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofCARD32 pixels` of 4nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cmap. */
        COLORMAP cmap;
        /** @brief Protocol name: plane-mask. */
        CARD32   plane_mask;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %StoreColors request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   STORECOLORS(89) and `tl_aligned_units` of 2+3n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct StoreColors : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofCOLORITEM items` of 12nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cmap. */
        COLORMAP cmap;
    };
    /**
     * @brief `items` list member color data.
     */
    struct [[gnu::packed]] COLORITEM : public protocol::impl::Struct {
        /** @brief Protocol name: pixel. */
        CARD32  pixel;
        /** @brief Protocol name: red. */
        CARD16  red;
        /** @brief Protocol name: green. */
        CARD16  green;
        /** @brief Protocol name: blue. */
        CARD16  blue;
        /** @brief BITMASK of following flags: 0x01=do-red 0x02=do-green
         *    0x04=do-blue (0xF8 bits unused). */
        uint8_t do_rgb_mask;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused;
    public:
        /** @brief [do_rgb_mask](#do_rgb_mask) flag names. */
        inline static const
        std::vector< std::string_view >& do_rgb_names {
            protocol::enum_names::do_rgb_mask };
        /** @brief [do_rgb_mask](#do_rgb_mask) bits which should always be 0. */
        static constexpr uint8_t DO_RGB_ZERO_BITS { 0xF8 };
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %StoreNamedColor request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct StoreNamedColor : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal STORENAMEDCOLOR(90). */
        uint8_t  opcode;
        /** @brief BITMASK of following flags: 0x01=do-red 0x02=do-green
         *    0x04=do-blue (0xF8 bits unused). */
        uint8_t  do_rgb_mask;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 4+pad(name_len)/4
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffixes:
     *   - `STRING8 name` of pad(name_len)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cmap. */
        COLORMAP cmap;
        /** @brief Protocol name: pixel. */
        CARD32   pixel;
        /** @brief Length of suffix `name` in bytes. */
        uint16_t name_len;
    private:
        /** @brief Ignored bytes. */
        uint16_t _unused;
    };
    /** @brief [do_rgb_mask](#Header::do_rgb_mask) flag names. */
    inline static const
    std::vector< std::string_view >& do_rgb_names {
        protocol::enum_names::do_rgb_mask };
    /** @brief [do_rgb_mask](#Header::do_rgb_mask) bits which should always be 0. */
    static constexpr uint8_t DO_RGB_ZERO_BITS { 0xF8 };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %QueryColors request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   QUERYCOLORS(91) and `tl_aligned_units` of 2+n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct QueryColors : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffixes:
     *   - `LISTofCARD32 pixels` of 4nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cmap. */
        COLORMAP   cmap;
    };
    /**
     * @brief Represents X11 %QueryColors reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         * @note Followed by suffixes:
         *   - `LISTofRGB colors` of (8 * [colors_ct](#Encoding::colors_ct))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;  // `extra_aligned_units` of 2 * colors_ct
            /** @brief Length of suffix `colors` in [RGB](#RGB)s. */
            uint16_t colors_ct;
        private:
            /** @brief Ignored bytes. */
            uint8_t  _unused[22];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        /**
         * @brief Color data member of `colors`.
         */
        struct [[gnu::packed]] RGB : public protocol::impl::Struct {
            /** @brief Protocol name: red. */
            CARD16  red;
            /** @brief Protocol name: green. */
            CARD16  green;
            /** @brief Protocol name: blue. */
            CARD16  blue;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[2];
        };
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %LookupColor request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   LOOKUPCOLOR(92) and `tl_aligned_units` of 3+pad(Encoding::name_len)/4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct LookupColor : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `STRING8 name` of pad(name_len)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cmap. */
        COLORMAP cmap;
        /** @brief Length of suffix `name` in bytes. */
        uint16_t name_len;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[2];
    };
    /**
     * @brief Represents X11 %LookupColor reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;  // `extra_aligned_units` of 0.
            /** @brief Protocol name: exact-red. */
            CARD16  exact_red;
            /** @brief Protocol name: exact-green. */
            CARD16  exact_green;
            /** @brief Protocol name: exact-blue. */
            CARD16  exact_blue;
            /** @brief Protocol name: visual-red. */
            CARD16  visual_red;
            /** @brief Protocol name: visual-green. */
            CARD16  visual_green;
            /** @brief Protocol name: visual-blue. */
            CARD16  visual_blue;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[12];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %CreateCursor request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   CREATECURSOR(93) and `tl_aligned_units` of 8.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct CreateCursor : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cid. */
        CURSOR cid;
        /** @brief Protocol name: source. */
        PIXMAP source;
        /** @brief Protocol name: mask; uses enum: 0=None. */
        PIXMAP mask;
        /** @brief Protocol name: fore-red. */
        CARD16 fore_red;
        /** @brief Protocol name: fore-green. */
        CARD16 fore_green;
        /** @brief Protocol name: fore-blue. */
        CARD16 fore_blue;
        /** @brief Protocol name: back-red. */
        CARD16 back_red;
        /** @brief Protocol name: back-green. */
        CARD16 back_green;
        /** @brief Protocol name: back-blue. */
        CARD16 back_blue;
        /** @brief Protocol name: x. */
        CARD16 x;
        /** @brief Protocol name: y. */
        CARD16 y;
    };
    /** @brief [mask](#Encoding::mask) enum names. */
    inline static const
    std::vector< std::string_view >& mask_names {
        protocol::enum_names::zero_none };
    /** @brief Total encoding size in bytes (before suffixes). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %CreateGlyphCursor request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   CREATEGLYPHCURSOR(94) and `tl_aligned_units` of 8.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct CreateGlyphCursor : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cid. */
        CURSOR cid;
        /** @brief Protocol name: source-font. */
        FONT   source_font;
        /** @brief Protocol name: mask-font; uses enum: 0=None. */
        FONT   mask_font;
        /** @brief Protocol name: source-char. */
        CARD16 source_char;
        /** @brief Protocol name: mask-char. */
        CARD16 mask_char;
        /** @brief Protocol name: fore-red. */
        CARD16 fore_red;
        /** @brief Protocol name: fore-green. */
        CARD16 fore_green;
        /** @brief Protocol name: fore-blue. */
        CARD16 fore_blue;
        /** @brief Protocol name: back-red. */
        CARD16 back_red;
        /** @brief Protocol name: back-green. */
        CARD16 back_green;
        /** @brief Protocol name: back-blue. */
        CARD16 back_blue;
    };
    /** @brief [mask-font](#Encoding::mask_font) enum names. */
    inline static const
    std::vector< std::string_view >& mask_font_names {
        protocol::enum_names::zero_none };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %FreeCursor request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   FREECURSOR(95) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct FreeCursor : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cursor. */
        CURSOR cursor;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %RecolorCursor request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   RECOLORCURSOR(96) and `tl_aligned_units` of 5.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct RecolorCursor : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: cursor. */
        CURSOR cursor;
        /** @brief Protocol name: fore-red. */
        CARD16 fore_red;
        /** @brief Protocol name: fore-green. */
        CARD16 fore_green;
        /** @brief Protocol name: fore-blue. */
        CARD16 fore_blue;
        /** @brief Protocol name: back-red. */
        CARD16 back_red;
        /** @brief Protocol name: back-green. */
        CARD16 back_green;
        /** @brief Protocol name: back-blue. */
        CARD16 back_blue;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %QueryBestSize request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct QueryBestSize : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal QUERYBESTSIZE(97). */
        uint8_t  opcode;
        /** @brief Protocol name: class; uses enum: 0=Cursor 1=Tile 2=Stipple. */
        uint8_t  class_;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 3
    };
    /**
     * @brief Fixed encoding after [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: drawable. */
        DRAWABLE drawable;
        /** @brief Protocol name: width. */
        CARD16   width;
        /** @brief Protocol name: height. */
        CARD16   height;
    };
    /** @brief [class](#Header::class_) enum names. */
    inline static const
    std::vector< std::string_view > class_names {
        "Cursor",  // 0
        "Tile",    // 1
        "Stipple"  // 2
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %QueryBestSize request [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;  // `extra_aligned_units` of 0.
            /** @brief Protocol name: width. */
            CARD16  width;
            /** @brief Protocol name: height. */
            CARD16  height;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[20];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %QueryExtension request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   QUERYEXTENSION(98) and `tl_aligned_units` of 2+pad(Encoding::name_len)/4.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct QueryExtension : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `STRING8 name` of pad(name_len)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Length of suffix `name` in bytes. */
        uint16_t name_len;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[2];
    };
    /** @brief Total encoding size in bytes (before suffixes). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %QueryExtension reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;  // `extra_aligned_units` of 0
            /** @brief Protocol name: present. */
            BOOL    present;
            /** @brief Protocol name: major-opcode. */
            CARD8   major_opcode;
            /** @brief Protocol name: first-event. */
            CARD8   first_event;
            /** @brief Protocol name: first-error. */
            CARD8   first_error;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[20];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %ListExtensions request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   LISTEXTENSIONS(99) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ListExtensions : public impl::SimpleRequest {
    /**
     * @brief Represents X11 %ListExtensions reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Length of suffix `names` in [STR](#protocol::STR)s. */
            CARD8    names_ct;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // pad(n) / 4
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         * @note Followed by suffix:
         *   - `LISTofSTR names` of [pad](#X11ProtocolParser::_Alignment::pad)(n)B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %ChangeKeyboardMapping request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ChangeKeyboardMapping : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal CHANGEKEYBOARDMAPPING(100). */
        uint8_t  opcode;
        /** @brief Protocol name: keycode-count; affects length of suffix `keysyms`. */
        uint8_t  keycode_count;     // n
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 2+nm
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofKEYSYM keysyms` of 4nmB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: first-keycode. */
        KEYCODE first_keycode;
        /** @brief Protocol name: keysyms-per-keycode; affects length of suffix `keysyms`. */
        uint8_t keysyms_per_keycode;  // m
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[2];
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GetKeyboardMapping request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETKEYBOARDMAPPING(101) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetKeyboardMapping : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: first-keycode. */
        KEYCODE first_keycode;
        /** @brief Protocol name: count. */
        uint8_t count;  // m
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[2];
    };
    /** @brief Total encoding size in bytes (before suffixes). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
    /**
     * @brief Represents X11 %GetKeyboardMapping reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: keysyms-per-keycode. */
            uint8_t  keysyms_per_keycode;  // n
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // nm
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         * @note Followed by suffix:
         *   - `LISTofKEYSYM keysyms` of
         *   ([keysyms_per_keycode](#Header::keysyms_per_keycode) *
         *   [count](#GetKeyboardMapping::Encoding::count))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %ChangeKeyboardControl request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   CHANGEKEYBOARDCONTROL(102) and `tl_aligned_units` of 2+n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ChangeKeyboardControl : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofVALUE value-list` of 4nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: value-mask (4B BITMASK, n bits set to 1). */
        uint32_t   value_mask;
    };
    /** @brief List of all parseable types potentially stored as VALUEs in
     *    `value-list`.
     *  - `INT8    key-click-percent`
     *  - `INT8    bell-percent`
     *  - `INT16   bell-pitch`
     *  - `INT16   bell-duration`
     *  - `CARD8   led`
     *  - `uint8_t led-mode` 0=Off 1=On
     *  - `KEYCODE key`
     *  - `uint8_t auto-repeat-mode` 0=Off 1=On 2=Default */
    inline static constexpr
    std::tuple<
        INT8, INT8, INT16, INT16, CARD8, uint8_t, KEYCODE, uint8_t
        > value_types {};
    /** @brief `value-mask` flag names/`value-list` member names. */
    inline static const
    std::vector< std::string_view > value_names {
        "key-click-percent",  // 1 << 0
        "bell-percent",       // 1 << 1
        "bell-pitch",         // 1 << 2
        "bell-duration",      // 1 << 3
        "led",                // 1 << 4
        "led-mode",           // 1 << 5
        "key",                // 1 << 6
        "auto-repeat-mode"    // 1 << 7
    };
    /** @brief VALUE `led-mode` enum names. */
    inline static const
    std::vector< std::string_view >& led_mode_names {
        protocol::enum_names::off_on };
    /** @brief VALUE `led-mode` maximum value to use enum. */
    static constexpr CARD8 LED_MODE_ENUM_MAX { 1 };
    /** @brief VALUE `auto-repeat-mode` enum names. */
    inline static const
    std::vector< std::string_view >& auto_repeat_mode_names {
        protocol::enum_names::off_on };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GetKeyboardControl request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETKEYBOARDCONTROL(103) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetKeyboardControl : public impl::SimpleRequest {
    /**
     * @brief Represents X11 %GetKeyboardControl reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: global-auto-repeat; uses enum 0=Off 1=On. */
            uint8_t  global_auto_repeat;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // 5
        };
        /** @brief Fixed length of [auto-repeats](#Encoding::auto_repeats) in CARD8s. */
        static constexpr uint32_t AUTO_REPEATS_SZ { 32 };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
            /** @brief Protocol name: led-mask. */
            CARD32  led_mask;
            /** @brief Protocol name: key-click-percent. */
            CARD8   key_click_percent;
            /** @brief Protocol name: bell-percent. */
            CARD8   bell_percent;
            /** @brief Protocol name: bell-pitch. */
            CARD16  bell_pitch;
            /** @brief Protocol name: bell-duration. */
            CARD16  bell_duration;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[2];
        public:
            /** @brief Protocol name: auto-repeats (fixed length LISTofCARD8). */
            CARD8   auto_repeats[AUTO_REPEATS_SZ];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ + (5 * ALIGN) );
        /** @brief [global-auto-repeat](#Header::global_auto_repeat) enum names. */
        inline static const
        std::vector< std::string_view >& global_auto_repeat_names {
            protocol::enum_names::off_on };
        /** @brief [global-auto-repeat](#Header::global_auto_repeat) maximum
         *    value to use enum. */
        static constexpr uint8_t GLOBAL_AUTO_REPREAT_ENUM_MAX { 1 };
    };
};
/**
 * @brief Represents X11 %Bell request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct Bell : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal BELL(104). */
        uint8_t  opcode;
        /** @brief Protocol name: percent. */
        INT8     percent;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 1
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ { sizeof( Header ) };
};
/**
 * @brief Represents X11 %ChangePointerControl request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   CHANGEPOINTERCONTROL(105) and `tl_aligned_units` of 3.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ChangePointerControl : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: acceleration-numerator. */
        INT16 acceleration_numerator;
        /** @brief Protocol name: acceleration-denominator. */
        INT16 acceleration_denominator;
        /** @brief Protocol name: threshold. */
        INT16 threshold;
        /** @brief Protocol name: do-acceleration. */
        BOOL  do_acceleration;
        /** @brief Protocol name: do-threshold. */
        BOOL  do_threshold;
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GetPointerControl request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETPOINTERCONTROL(106) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetPointerControl : public impl::SimpleRequest {
    /**
     * @brief Represents X11 %GetPointerControl reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;  // `extra_aligned_units` of 0
            /** @brief Protocol name: acceleration-numerator. */
            CARD16  acceleration_numerator;
            /** @brief Protocol name: acceleration-denominator. */
            CARD16  acceleration_denominator;
            /** @brief Protocol name: threshold. */
            CARD16  threshold;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[18];
        };
    };
};
/**
 * @brief Represents X11 %SetScreenSaver request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   SETSCREENSAVER(107) and `tl_aligned_units` of 3.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SetScreenSaver : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: timeout. */
        INT16   timeout;
        /** @brief Protocol name: interval. */
        INT16   interval;
        /** @brief Protocol name: prefer-blanking; uses enum: 0=No 1=Yes 2=Default. */
        uint8_t prefer_blanking;
        /** @brief Protocol name: allow-exposures; uses enum: 0=No 1=Yes 2=Default. */
        uint8_t allow_exposures;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[2];
    };
    /** @brief [prefer-blanking](#Encoding::prefer_blanking) enum names. */
    inline static const
    std::vector< std::string_view >& prefer_blanking_names {
        protocol::enum_names::no_yes };
    /** @brief [allow-exposures](#Encoding::allow_exposures) enum names. */
    inline static const
    std::vector< std::string_view >& allow_exposures_names {
        protocol::enum_names::no_yes };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %GetScreenSaver request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETSCREENSAVER(108) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetScreenSaver : public impl::SimpleRequest {
    /**
     * @brief Represents X11 %GetScreenSaver reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding, including [Header](#requests::Reply::Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;  // `extra_aligned_units` of 0
            /** @brief Protocol name: timeout. */
            CARD16  timeout;
            /** @brief Protocol name: interval. */
            CARD16  interval;
            /** @brief Protocol name: prefer-blanking; uses enum: 0=No 1=Yes. */
            uint8_t prefer_blanking;
            /** @brief Protocol name: allow-exposures; uses enum: 0=No 1=Yes. */
            uint8_t allow_exposures;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[18];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        /** @brief [prefer-blanking](#Encoding::prefer_blanking) enum names. */
        inline static const
        std::vector< std::string_view >& prefer_blanking_names {
            protocol::enum_names::no_yes };
        /** @brief [prefer-blanking](#Encoding::prefer_blanking) maximum value
         *    to use enum. */
        static constexpr int PREFER_BLANKING_ENUM_MAX { 1 };
        /** @brief [allow-exposures](#Encoding::allow_exposures) enum names. */
        inline static const
        std::vector< std::string_view >& allow_exposures_names {
            protocol::enum_names::no_yes };
        /** @brief [allow-exposures](#Encoding::allow_exposures) maximum value
         *    to use enum. */
        static constexpr int ALLOW_EXPOSURES_ENUM_MAX { 1 };
    };
};
/**
 * @brief Represents X11 %ChangeHosts request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ChangeHosts : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal CHANGEHOSTS(109). */
        uint8_t  opcode;
        /** @brief Protocol name: mode; uses enum: 0=Insert 1=Delete. */
        uint8_t  mode;
        /** @brief Total length of request encoding in 4B units. */
        uint16_t tl_aligned_units;  // 2+pad(address_len)/4
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffix.
     * @note Followed by suffix:
     *   - `LISTofCARD8 address` of pad(address_len)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: family; uses enum: 0=Internet 1=DECnet 2=Chaos. */
        uint8_t  family;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused;
    public:
        /** @brief Length of suffix `address` in bytes. */
        uint16_t address_len;
    };
    /** @brief [mode](#Header::mode) enum names. */
    inline static const
    std::vector< std::string_view > mode_names {
        "Insert",  // 0
        "Delete"   // 1
    };
    /** @brief [family](#Encoding::family) enum names. */
    inline static const
    std::vector< std::string_view >& family_names {
        protocol::enum_names::host_family };
    /** @brief [family](#Encoding::family) maximum value to use enum. */
    static constexpr uint8_t FAMILY_ENUM_MAX { 2 };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %ListHosts request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   LISTHOSTS(110) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ListHosts : public impl::SimpleRequest {
    /**
     * @brief Represents X11 %ListHosts reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: mode; uses enum: 0=Disabled 1=Enabled. */
            uint8_t  mode;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // hosts_ct
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         * @note Followed by suffix:
         *   - `LISTofHOST hosts` of (4 * [hosts_ct](#Encoding::hosts_ct))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
            /** @brief Length of suffix `hosts` in [HOST](#protocol::HOST)s. */
            CARD16  hosts_ct;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[22];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        /** @brief [mode](#Header::mode) enum names. */
        inline static const
        std::vector< std::string_view > mode_names {
            "Disabled",  // 0
            "Enabled"    // 1
        };
    };
};
/**
 * @brief Represents X11 %SetAccessControl request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SetAccessControl : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal SETACCESSCONTROL(111). */
        uint8_t    opcode;
        /** @brief Protocol name: mode; uses enum: 0=Disable 1=Enable. */
        uint8_t    mode;
        /** @brief Total encoded length in 4B units. */
        uint16_t   tl_aligned_units;  // 1
    };
    /** @brief [mode](#Header::mode) enum names. */
    inline static const
    std::vector< std::string_view > mode_names {
        "Disable",  // 0
        "Enable"    // 1
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };
};
/**
 * @brief Represents X11 %SetCloseDownMode request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SetCloseDownMode : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal SETCLOSEDOWNMODE(112). */
        uint8_t    opcode;
        /** @brief Protocol name: mode; uses enum:
         *    0=Destroy 1=RetainPermanent 2=RetainTemporary. */
        uint8_t    mode;
        /** @brief Total encoded length in 4B units. */
        uint16_t   tl_aligned_units;  // 1
    };
    /** @brief [mode](#Header::mode) enum names. */
    inline static const
    std::vector< std::string_view > mode_names {
        "Destroy",          // 0
        "RetainPermanent",  // 1
        "RetainTemporary"   // 2
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };
};
/**
 * @brief Represents X11 %KillClient request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   KILLCLIENT(113) and `tl_aligned_units` of 2.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct KillClient : public Request {
    /**
     * @brief Fixed encoding after [Header](#Request::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: resource; uses enum: 0=AllTemporary. */
        CARD32 resource;
    };
    /** @brief [resource](#Encoding::resource) enum names. */
    inline static const
    std::vector< std::string_view > resource_names {
        "AllTemporary"  // 0
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};
/**
 * @brief Represents X11 %RotateProperties request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   ROTATEPROPERTIES(114) and `tl_aligned_units` of 3+n.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct RotateProperties : public Request {
    /**
     * @brief Fixed encoding between [Header](#Request::Header) and suffixes.
     * @note Followed by suffix:
     *   - `LISTofATOM properties` of 4nB
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: window. */
        WINDOW   window;
        /** @brief Length of suffix `properties` in ATOMs. */
        uint16_t properties_ct;
        /** @brief Protocol name: delta. */
        INT16    delta;
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };
};
/**
 * @brief Represents X11 %ForceScreenSaver request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct ForceScreenSaver : public Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal FORCESCREENSAVER(115). */
        uint8_t  opcode;
        /** @brief Protocol name: mode; uses enum: 0=Reset 1=Activate. */
        uint8_t  mode;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 1
    };
    /** @brief [mode](#Header::mode) enum names. */
    inline static const
    std::vector< std::string_view > mode_names {
        "Reset",    // 0
        "Activate"  // 1
    };
    /** @brief Total encoding size in bytes. */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };
};
/**
 * @brief Represents X11 %SetPointerMapping request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SetPointerMapping : public Request {
    /**
     * @brief Fixed encoding prefix.
     * @note Followed by suffix:
     *   - `LISTofCARD8 map` of pad(map_len)B
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name: opcode; should equal SETPOINTERMAPPING(116). */
        uint8_t  opcode;
        /** @brief Length of suffix `map` in bytes. */
        uint8_t  map_len;
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;  // 1+pad(map_len)/4
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };
    /**
     * @brief Represents X11 %SetPointerMapping reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: status; uses enum: 0=Success 1=Busy. */
            uint8_t  status;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        /** @brief [status](#Header::status) enum names. */
        inline static const
        std::vector< std::string_view >& status_names {
            protocol::enum_names::mapping_status };
        /** @brief [status](#Header::status) maxmium value to use enum. */
        static constexpr int STATUS_ENUM_MAX { 1 };
    };
};
/**
 * @brief Represents X11 %GetPointerMapping request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETPOINTERMAPPING(117) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetPointerMapping : public impl::SimpleRequest {
    /**
     * @brief Represents X11 %GetPointerMapping reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Length of suffix `map` in bytes. */
            uint8_t  map_len;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // pad(n)/4
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         * @note Followed by suffix:
         *   - `LISTofCARD8 map` of pad(n)B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};
/**
 * @brief Represents X11 %SetModifierMapping request [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct SetModifierMapping : public Request {
    /**
     * @brief Fixed encoding prefix.
     * @note Followed by suffix:
     *   - `LISTofKEYCODE keycodes` of 8nB
     */
    struct [[gnu::packed]] Header {
        /** @brief Unique request identifier, should equal SETMODIFIERMAPPING(118). */
        uint8_t  opcode;
        /** @brief Protocol name: keycodes-per-modifier. */
        uint8_t  keycodes_per_modifier;  // n
        /** @brief Total encoded length in 4B units. */
        uint16_t tl_aligned_units;       // 1+2n
    };
    /** @brief Total encoding size in bytes (before suffix). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };
    /** @brief Used with [keycodes-per-modifier](#Header::keycodes_per_modifier)
     *    to [calculate] length of suffix `keycodes`.
     *  [calculate]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#requests:SetModifierMapping */
    static constexpr uint8_t MODIFIER_CT { 8 };
    /**
     * @brief Represents X11 %SetModifierMapping reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: status; uses enum: 0=Success 1=Busy 2=Failed. */
            uint8_t  status;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // 0
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        /** @brief [status](#Header::status) enum names. */
        inline static const
        std::vector< std::string_view >& status_names {
            protocol::enum_names::mapping_status };
    };
};
/**
 * @brief Represents X11 %GetModifierMapping request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   GETMODIFIERMAPPING(119) and `tl_aligned_units` of 1.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct GetModifierMapping : public impl::SimpleRequest {
    /**
     * @brief Represents X11 %GetModifierMapping reply [encoding].
     * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
     */
    struct Reply : public requests::Reply {
        /**
         * @brief Fixed encoding prefix.
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: reply; should always equal
             *    [REPLY](#requests::Reply::REPLY). */
            uint8_t  reply;
            /** @brief Protocol name: keycodes-per-modifier. */
            uint8_t  keycodes_per_modifier;
            /** @brief [Serial number on current connection](#Connection::sequence)
             *   of relevant request. */
            CARD16   sequence_num;
            /** @brief Length of encoding in excess of default, in 4B units. */
            uint32_t extra_aligned_units;  // 2 * keycodes_per_modifier
        };
        /**
         * @brief Fixed encoding, including [Header](#Header).
         * @note Followed by suffix:
         *   - `LISTofKEYCODE keycodes` of
         *     (8 * [keycodes_per_modifier](#Header::keycodes_per_modifier))B
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header  header;
        private:
            /** @brief Ignored bytes. */
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        /** @brief Used with [keycodes-per-modifier](#Header::keycodes_per_modifier)
         *    to [calculate] length of suffix `keycodes`.
         *  [calculate]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#requests:GetModifierMapping */
        static constexpr uint32_t MODIFIER_CT { 8 };
    };
};
/**
 * @brief Represents X11 %NoOperation request [encoding].
 * @note Uses [Request::Header](#Request::Header) with expected `opcode` of
 *   NOOPERATION(127) and `tl_aligned_units` of 1+n, possibly followed by 4nB
 *   of unused data.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests
 */
struct NoOperation : public impl::SimpleRequest {
};
        /** @brief Ignored bytes. */

}  // namespace requests

}  // namespace protocol


#endif  // PROTOCOL_REQUESTS_HPP
