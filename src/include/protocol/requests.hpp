#ifndef PROTOCOL_REQUESTS_HPP
#define PROTOCOL_REQUESTS_HPP


#include <cstdint>
#include <array>
#include <tuple>

#include "protocol/Response.hpp"
#include "protocol/common_types.hpp"
#include "protocol/enum_names.hpp"


// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#request_format
// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Requests
// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests

namespace protocol {

namespace requests {

namespace opcodes {

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
inline constexpr int MIN { CREATEWINDOW };
inline constexpr int MAX { NOOPERATION };

}  // namespace opcodes

// TBD unlike Reply/Event/Error/Conn*, encoding will not contain header
//   (in anticipation of implementing BIG-REQUESTS)
struct Request {
    struct [[gnu::packed]] Header {
        union [[gnu::packed]] {
            uint8_t opcode;
            uint8_t major_opcode;   // extensions
        };
        union [[gnu::packed]] {
            uint8_t _unused;
            uint8_t minor_opcode;   // extensions
        };
        uint16_t tl_aligned_units;  // total request length in 4B units
    };

    static constexpr uint32_t ALIGN { 4 };

    virtual ~Request() = 0;
};

struct Reply : public Response {
    struct [[gnu::packed]] Header {
        uint8_t  reply;
    private:
        uint8_t  _unused;
    public:
        CARD16   sequence_num;
        uint32_t extra_aligned_units;  // (reply length - default) / 4
    };
    struct [[gnu::packed]] Encoding {
        Header  header;
    private:
        uint8_t _unused[24];
    };

    static constexpr uint8_t  REPLY { Response::REPLY_PREFIX };
    static constexpr uint32_t DEFAULT_ENCODING_SZ { 32 };
    static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );

    virtual ~Reply() = 0;
};

inline constexpr
std::string_view UNUSED_OPCODE_STRING { "(unused core opcode)" };

// names by opcode
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

struct WindowValueListRequest : public Request {
    inline static constexpr
    std::tuple<
        PIXMAP,            // background-pixmap // 0 None 1 ParentRelative
        CARD32,            // background-pixel
        PIXMAP,            // border-pixmap // 0 CopyFromParent
        CARD32,            // border-pixel
        BITGRAVITY,        // bit-gravity
        WINGRAVITY,        // win-gravity
        uint8_t,           // backing-store // 0 NotUseful 1 WhenMapped 2 Always
        CARD32,            // backing-planes
        CARD32,            // backing-pixel
        BOOL,              // override-redirect
        BOOL,              // save-under
        SETofEVENT,        // event-mask
        SETofDEVICEEVENT,  // do-not-propagate-mask
        COLORMAP,          // colormap // 0 CopyFromParent
        CURSOR             // cursor // 0 None
        > value_types {};

    inline static const
    std::vector< std::string_view >& value_names {
        protocol::enum_names::window_attribute_value_mask };
    inline static const
    std::vector< std::string_view >& background_pixmap_names {
        protocol::enum_names::window_attribute_background_pixmap };
    inline static const
    std::vector< std::string_view >& border_pixmap_names {
        protocol::enum_names::zero_copy_from_parent };
    inline static const
    std::vector< std::string_view >& backing_store_names {
        protocol::enum_names::window_attribute_backing_store };
    inline static const
    std::vector< std::string_view >& colormap_names {
        protocol::enum_names::zero_copy_from_parent };
    inline static const
    std::vector< std::string_view >& cursor_names {
        protocol::enum_names::zero_none };

    virtual ~WindowValueListRequest() = 0;
};

}  // namespace impl

struct CreateWindow : public impl::WindowValueListRequest {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 1
        CARD8    depth;
        uint16_t tl_aligned_units;  // 8+n
    };
    struct [[gnu::packed]] Encoding {
        WINDOW   wid;
        WINDOW   parent;
        INT16    x;
        INT16    y;
        CARD16   width;
        CARD16   height;
        CARD16   border_width;  // border-width
        uint16_t class_;        // class  // 0 CopyFromParent 1 InputOutput 2 InputOnly
        VISUALID visual;        // 0 CopyFromParent
        uint32_t value_mask;    // BITMASK value-mask 4B (has n bits set to 1)
    };
    // followed by LISTofVALUE value-list (4n)B

    inline static const
    std::vector< std::string_view >& class_names {
        protocol::enum_names::window_class };
    inline static const
    std::vector< std::string_view >& visual_names {
        protocol::enum_names::zero_copy_from_parent };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct ChangeWindowAttributes : public impl::WindowValueListRequest {
    // Header.opcode == 2
    // Header.tl_aligned_units == 3+n
    struct [[gnu::packed]] Encoding {
        WINDOW   window;
        uint32_t value_mask;      // BITMASK value-mask 4B (has n bits set to 1)
    };
    // followed by LISTofVALUE value-list (4n)B

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

namespace impl {

struct SimpleWindowRequest : public Request {
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        WINDOW window;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    virtual ~SimpleWindowRequest() = 0;
};

}  // namespace impl

struct GetWindowAttributes : public impl::SimpleWindowRequest {
    // Header.opcode == 3
    // Header.tl_aligned_units == 2

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            uint8_t  backing_store;        // backing-store  // 0 NotUseful 1 WhenMapped 2 Always
            CARD16   sequence_num;
            uint32_t extra_aligned_units;  // 3 ((reply length - default) / 4)
        };
        struct [[gnu::packed]] Encoding {
            Header           header;
            VISUALID         visual;
            uint16_t         class_;                 // class  // 1 InputOutput 2 InputOnly
            BITGRAVITY       bit_gravity;            // bit-gravity
            WINGRAVITY       win_gravity;            // win-gravity
            CARD32           backing_planes;         // backing-planes
            CARD32           backing_pixel;          // backing-pixel
            BOOL             save_under;             // save-under
            BOOL             map_is_installed;       // map-is-installed
            uint8_t          map_state;              // map-state  // 0 Unmapped 1 Unviewable 2 Viewable
            BOOL             override_redirect;      // override-redirect
            COLORMAP         colormap;               // 0 None
            SETofEVENT       all_event_masks;        // all-event-masks
            SETofEVENT       your_event_mask;        // your-event-mask
            SETofDEVICEEVENT do_not_propagate_mask;  // do-not-propagate-mask
        private:
            uint8_t          _unused[2];
        };
        static_assert( sizeof( Encoding ) ==
                       DEFAULT_ENCODING_SZ + ( 3 * ALIGN ) );

        inline static const
        std::vector< std::string_view >& backing_store_names {
            protocol::enum_names::window_attribute_backing_store };
        inline static const
        std::vector< std::string_view >& class_names {
            protocol::enum_names::window_class };
        static constexpr uint16_t CLASS_ENUM_MIN { 1 };
        inline static const
        std::vector< std::string_view > map_state_names {
            "Unmapped",    // 0
            "Unviewable",  // 1
            "Viewable"     // 2
        };
        inline static const
        std::vector< std::string_view >& colormap_names {
            protocol::enum_names::zero_none };
    };
};

struct DestroyWindow : public impl::SimpleWindowRequest {
    // Header.opcode == 4
    // Header.tl_aligned_units == 2
};

struct DestroySubwindows : public impl::SimpleWindowRequest {
    // Header.opcode == 5
    // Header.tl_aligned_units == 2
};

struct ChangeSaveSet : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 6
        uint8_t  mode;              // 0 Insert 1 Delete
        uint16_t tl_aligned_units;  // 2
    };
    struct [[gnu::packed]] Encoding {
        WINDOW window;
    };

    inline static const
    std::vector< std::string_view >& mode_names {
        protocol::enum_names::change_mode };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct ReparentWindow : public Request {
    // Header.opcode == 7
    // Header.tl_aligned_units == 4
    struct [[gnu::packed]] Encoding {
        WINDOW window;
        WINDOW parent;
        INT16  x;
        INT16  y;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct MapWindow : public impl::SimpleWindowRequest {
    // Header.opcode == 8
    // Header.tl_aligned_units == 2
};

struct MapSubwindows : public impl::SimpleWindowRequest {
    // Header.opcode == 9
    // Header.tl_aligned_units == 2
};

struct UnmapWindow : public impl::SimpleWindowRequest {
    // Header.opcode == 10
    // Header.tl_aligned_units == 2
};

struct UnmapSubwindows : public impl::SimpleWindowRequest {
    // Header.opcode == 11
    // Header.tl_aligned_units == 2
};

struct ConfigureWindow : public Request {
    // Header.opcode == 12
    // Header.tl_aligned_units == 3+n
    struct [[gnu::packed]] Encoding {
        WINDOW   window;
        uint16_t value_mask;  // BITMASK value-mask 2B (has n bits set to 1)
    private:
        uint8_t  _unused[2];
    };
    // followed by LISTofVALUE value-list (4n)B

    inline static constexpr
    std::tuple<
        INT16,   // x
        INT16,   // y
        CARD16,  // width
        CARD16,  // height
        CARD16,  // border-width
        WINDOW,  // sibling
        uint8_t  // stack-mode  // 0 Above 1 Below 2 TopIf 3 BottomIf 4 Opposite
        > value_types {};

    inline static const
    std::vector< std::string_view >& value_names {
        protocol::enum_names::window_value_mask };
    inline static const
    std::vector< std::string_view >& stack_mode_names {
        protocol::enum_names::window_value_stack_mode };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct CirculateWindow : public Request {
    struct [[gnu::packed]] Header {
        uint8_t   opcode;            // 13
        uint8_t   direction;         // 0 RaiseLowest 1 LowerHighest
        uint16_t  tl_aligned_units;  // 2
    };
    struct [[gnu::packed]] Encoding {
        WINDOW    window;
    };

    inline static const
    std::vector< std::string_view > direction_names {
        "RaiseLowest",  // 0
        "LowerHighest"  // 1
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GetGeometry : public Request {
    // Header.opcode == 14
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        DRAWABLE   drawable;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            CARD8    depth;
            CARD16   sequence_num;
            uint32_t extra_aligned_units;  // 0 ((reply length - default) / 4)
        };
        struct [[gnu::packed]] Encoding {
            Header   header;
            WINDOW   root;
            INT16    x;
            INT16    y;
            CARD16   width;
            CARD16   height;
            CARD16   border_width;  // border-width
        private:
            uint8_t  _unused[10];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
    };
};

struct QueryTree : public impl::SimpleWindowRequest {
    // Header.opcode == 15
    // Header.tl_aligned_units == 2

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == children_ct ((reply length - default) / 4)
        struct [[gnu::packed]] Encoding {
            Header   header;
            WINDOW   root;
            WINDOW   parent;      // 0 None
            uint16_t children_ct; // number of WINDOWs in children
        private:
            uint8_t  _unused[14];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
        // followed by LISTofWINDOW children (4 * children_ct)B

        inline static const
        std::vector< std::string_view >& parent_names {
            protocol::enum_names::zero_none };
    };
};

struct InternAtom : public Request {
    struct [[gnu::packed]] Header {
        uint8_t   opcode;            // 16
        BOOL      only_if_exists;    // only-if-exists
        uint16_t  tl_aligned_units;  // 2 + pad(name_len)/4
    };
    struct [[gnu::packed]] Encoding {
        uint16_t  name_len;     // length of name in bytes
    private:
        uint8_t   _unused[2];
    };
    // followed by STRING8 name pad(n)B

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == 0 ((reply length - default) / 4)
        struct [[gnu::packed]] Encoding {
            Header  header;
            ATOM    atom;          // 0 None
        private:
            uint8_t _unused2[20];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );

        inline static const
        std::vector< std::string_view >& atom_names {
            protocol::enum_names::zero_none };
    };
};

struct GetAtomName : public Request {
    // Header.opcode == 17
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        ATOM atom;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == pad(name_len)/4 ((reply length - default) / 4)
        struct [[gnu::packed]] Encoding {
            Header   header;
            uint16_t name_len;    // length of name in bytes
        private:
            uint8_t _unused[22];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
    };
    // followed by STRING8 name pad(name_len)B
};

struct ChangeProperty : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 18
        uint8_t  mode;              // 0 Replace 1 Prepend 2 Append
        uint16_t tl_aligned_units;  // 6+pad(n)/4
    };
    struct [[gnu::packed]] Encoding {
        WINDOW  window;
        ATOM    property;
        ATOM    type;
        CARD8   format;             // size of format units in bits (0,8,16,32)
    private:
        uint8_t _unused[3];
    public:
        CARD32  data_fmt_unit_len;  // length of data in format units
    };
    // followed by LISTofBYTE data pad((format/8)*fmt_unit_len)B

    inline static const
    std::vector< std::string_view > mode_names {
        "Replace",  // 0
        "Prepend",  // 1
        "Append"    // 2
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct DeleteProperty : public Request {
    // Header.opcode == 19
    // Header.tl_aligned_units == 3
    struct [[gnu::packed]] Encoding {
        WINDOW    window;
        ATOM      property;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GetProperty : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 20
        BOOL     delete_;           // delete
        uint16_t tl_aligned_units;  // 6
    };
    struct [[gnu::packed]] Encoding {
        WINDOW window;
        ATOM   property;
        ATOM   type;         // 0 AnyPropertyType
        CARD32 long_offset;  // long-offset
        CARD32 long_length;  // long-length
    };

    inline static const
    std::vector< std::string_view > type_names {
        "AnyPropertyType"  // 0
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            CARD8    format;               // format unit sz in bytes
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // 0 (reply length - default) / 4
        };
        struct [[gnu::packed]] Encoding {
            Header   header;
            ATOM     type;                // 0 None
            CARD32   bytes_after;         // bytes-after
            CARD32   value_fmt_unit_len;  // length of value in format units
            // format unit sz == value size / value_fmt_unit_len
        private:
            uint8_t  _unused[12];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
        // followed by LISTofBYTE value pad(value_fmt_unit_len * format)B

        inline static const
        std::vector< std::string_view >& type_names {
            protocol::enum_names::zero_none };
        static constexpr CARD8 MAX_FORMAT    { 32 };
        static constexpr CARD8 FORMAT_FACTOR { 8 };
    };
};

struct ListProperties : public impl::SimpleWindowRequest {
    // Header.opcode == 21
    // Header.tl_aligned_units == 2

    struct Reply : public requests::Reply {
        // header.extra_aligned_units == atoms_ct ((reply length - default) / 4)
        struct [[gnu::packed]] Encoding {
            Header   header;
            uint16_t atoms_ct;  // number of ATOMs in atoms
        private:
            uint8_t  _unused[22];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
        // followed by LISTofATOM atoms (sizeof(ATOM) * atoms_ct)B
    };
};

struct SetSelectionOwner : public Request {
    // Header.opcode == 22
    // Header.tl_aligned_units == 4
    struct [[gnu::packed]] Encoding {
        WINDOW    owner;      // 0 None
        ATOM      selection;
        TIMESTAMP time;       // 0 CurrentTime
    };

    inline static const
    std::vector< std::string_view >& owner_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GetSelectionOwner : public Request {
    // Header.opcode == 23
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        ATOM selection;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        // header.extra_aligned_units == 0 ((reply length - default) / 4)
        struct [[gnu::packed]] Encoding {
            Header  header;
            WINDOW  owner;  // 0 None
        private:
            uint8_t _unused[20];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );

        inline static const
        std::vector< std::string_view >& owner_names {
            protocol::enum_names::zero_none };
    };
};

struct ConvertSelection : public Request {
    // Header.opcode == 24
    // Header.tl_aligned_units == 6
    struct [[gnu::packed]] Encoding {
        WINDOW    requestor;
        ATOM      selection;
        ATOM      target;
        ATOM      property;  // 0 None
        TIMESTAMP time;      // 0 CurrentTime
    };

    inline static const
    std::vector< std::string_view >& property_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct SendEvent : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 25
        BOOL     propagate;
        uint16_t tl_aligned_units;  // 11
    };
    struct [[gnu::packed]] Encoding {
        WINDOW     destination;  // 0 PointerWindow 1 InputFocus
        SETofEVENT event_mask;   // event-mask
    };
    // followed by Event 32B

    inline static const
    std::vector< std::string_view > destination_names {
        "PointerWindow",  // 0
        "InputFocus"      // 1
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GrabPointer : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 26
        BOOL     owner_events;      // owner-events
        uint16_t tl_aligned_units;  // 6 request length
    };
    struct [[gnu::packed]] Encoding {
        WINDOW            grab_window;    // grab-window
        SETofPOINTEREVENT event_mask;     // 2B event-mask
        uint8_t           pointer_mode;   // pointer-mode  // 0 Synchronous 1 Asynchronous
        uint8_t           keyboard_mode;  // keyboard-mode  // 0 Synchronous 1 Asynchronous
        WINDOW            confine_to;     // confine-to  // 0 None
        CURSOR            cursor;         // 0 None
        TIMESTAMP         time;           // 0 CurrentTime
    };

    inline static const
    std::vector< std::string_view >& pointer_mode_names {
        protocol::enum_names::input_mode };
    inline static const
    std::vector< std::string_view >& keyboard_mode_names {
        protocol::enum_names::input_mode };
    inline static const
    std::vector< std::string_view >& confine_to_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& cursor_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            uint8_t  status;               // 0 Success 1 AlreadyGrabbed 2 InvalidTime 3 NotViewable 4 Frozen
            CARD16   sequence_num;
            uint32_t extra_aligned_units;  // 0 ((reply length - default) / 4)
        };
        struct [[gnu::packed]] Encoding {
            Header  header;
        private:
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );

        inline static const
        std::vector< std::string_view >& status_names {
            protocol::enum_names::grab_status };
    };
};

struct UngrabPointer : public Request {
    // Header.opcode == 27
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        TIMESTAMP time;
    };

    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GrabButton : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 28
        BOOL     owner_events;      // owner-events
        uint16_t tl_aligned_units;  // 6 request length
    };
    struct [[gnu::packed]] Encoding {
        WINDOW            grab_window;    // grab-window
        SETofPOINTEREVENT event_mask;     // event-mask
        uint8_t           pointer_mode;   // pointer-mode  // 0 Synchronous 1 Asynchronous
        uint8_t           keyboard_mode;  // keyboard-mode  // 0 Synchronous 1 Asynchronous
        WINDOW            confine_to;     // confine-to  // 0 None
        CURSOR            cursor;         // 0 None
        BUTTON            button;         // 0 AnyButton
    private:
        uint8_t           unused;
    public:
        SETofKEYMASK      modifiers;       // 2B #x8000 AnyModifier
    };

    inline static const
    std::vector< std::string_view >& pointer_mode_names {
        protocol::enum_names::input_mode };
    inline static const
    std::vector< std::string_view >& keyboard_mode_names {
        protocol::enum_names::input_mode };
    inline static const
    std::vector< std::string_view >& confine_to_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& cursor_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& button_names {
        protocol::enum_names::button };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct UngrabButton : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 29
        BUTTON   button;            // 0 AnyButton
        uint16_t tl_aligned_units;  // 3
    };
    struct [[gnu::packed]] Encoding {
        WINDOW       grab_window;  // grab-window
        SETofKEYMASK modifiers;    // 2B #x8000 AnyModifier
    private:
        uint8_t      _unused[2];
    };

    inline static const
    std::vector< std::string_view >& button_names {
        protocol::enum_names::button };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct ChangeActivePointerGrab : public Request {
    // Header.opcode == 30
    // Header.tl_aligned_units == 4
    struct [[gnu::packed]] Encoding {
        CURSOR            cursor;      // 0 None
        TIMESTAMP         time;        // 0 CurrentTime
        SETofPOINTEREVENT event_mask;  // 2B event-mask
    private:
        uint8_t           _unused[2];
    };

    inline static const
    std::vector< std::string_view >& cursor_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GrabKeyboard : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 31
        BOOL     owner_events;      // owner-events
        uint16_t tl_aligned_units;  // 4
    };
    struct [[gnu::packed]] Encoding {
        WINDOW    grab_window;    // grab-window
        TIMESTAMP time;           // 0 CurrentTime
        uint8_t   pointer_mode;   // pointer-mode  // 0 Synchronous 1 Asynchronous
        uint8_t   keyboard_mode;  // keyboard-mode  // 0 Synchronous 1 Asynchronous
    private:
        uint8_t   _unused[2];
    };

    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
    inline static const
    std::vector< std::string_view >& pointer_mode_names {
        protocol::enum_names::input_mode };
    inline static const
    std::vector< std::string_view >& keyboard_mode_names {
        protocol::enum_names::input_mode };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            uint8_t  status;               // 0 Success 1 AlreadyGrabbed 2 InvalidTime 3 NotViewable 4 Frozen
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // 0 (reply length - default) / 4
        };
        struct [[gnu::packed]] Encoding {
            Header  header;
        private:
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );

        inline static const
        std::vector< std::string_view >& status_names {
            protocol::enum_names::grab_status };
    };
};

struct UngrabKeyboard : public Request {
    // Header.opcode == 32
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        TIMESTAMP time;  // 0 CurrentTime
    };

    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GrabKey : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 33
        BOOL     owner_events;      // owner-events
        uint16_t tl_aligned_units;  // 4
    };
    struct [[gnu::packed]] Encoding {
        WINDOW       grab_window;    // grab-window
        SETofKEYMASK modifiers;      // 2B #x8000 AnyModifier
        KEYCODE      key;            // 0 AnyKey
        uint8_t      pointer_mode;   // pointer-mode  // 0 Synchronous 1 Asynchronous
        uint8_t      keyboard_mode;  // keyboard-mode  // 0 Synchronous 1 Asynchronous
    private:
        uint8_t      _unused[3];
    };

    inline static const
    std::vector< std::string_view >& key_names {
        protocol::enum_names::key };
    inline static const
    std::vector< std::string_view >& pointer_mode_names {
        protocol::enum_names::input_mode };
    inline static const
    std::vector< std::string_view >& keyboard_mode_names {
        protocol::enum_names::input_mode };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct UngrabKey : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 34
        KEYCODE  key;               // 0 AnyKey
        uint16_t tl_aligned_units;  // 3
    };
    struct [[gnu::packed]] Encoding {
        WINDOW       grab_window;  // grab-window
        SETofKEYMASK modifiers;    // 2B #x8000 AnyModifier
    private:
        uint8_t      _unused[2];
    };

    inline static const
    std::vector< std::string_view >& key_names {
        protocol::enum_names::key };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct AllowEvents : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 35
        uint8_t  mode;              // 0 AsyncPointer 1 SyncPointer 2 ReplayPointer 3 AsyncKeyboard 4 SyncKeyboard 5 ReplayKeyboard 6 AsyncBoth 7 SyncBoth
        uint16_t tl_aligned_units;  // 2
    };
    struct [[gnu::packed]] Encoding {
        TIMESTAMP time;  // 0 CurrentTime
    };

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
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

namespace impl {

struct SimpleRequest : public Request {
    // Header.tl_aligned_units == 1
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };

    virtual ~SimpleRequest() = 0;
};

}  // namespace impl

struct GrabServer : public impl::SimpleRequest {
    // Header.opcode == 36
    // Header.tl_aligned_units == 1
};

struct UngrabServer : public impl::SimpleRequest {
    // Header.opcode == 37
    // Header.tl_aligned_units == 1
};

struct QueryPointer : public impl::SimpleWindowRequest {
    // Header.opcode == 38
    // Header.tl_aligned_units == 1

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            BOOL     same_screen;          // same-screen
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // 0 ((reply length - default) / 4)
        };
        struct [[gnu::packed]] Encoding {
            Header          header;
            WINDOW          root;
            WINDOW          child;       // 0 None
            INT16           root_x;      // root-x
            INT16           root_y;      // root-y
            INT16           win_x;       // win-x
            INT16           win_y;       // win-y
            SETofKEYBUTMASK mask;
        private:
            uint8_t         _unused[6];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );

        inline static const
        std::vector< std::string_view >& child_names {
            protocol::enum_names::zero_none };
    };
};

struct GetMotionEvents : public Request {
    // Header.opcode == 39
    // Header.tl_aligned_units == 4
    struct [[gnu::packed]] Encoding {
        WINDOW    window;
        TIMESTAMP start;  // 0 CurrentTime
        TIMESTAMP stop;   // 0 CurrentTime
    };

    inline static const
    std::vector< std::string_view >& start_names {
        protocol::enum_names::time };
    inline static const
    std::vector< std::string_view >& stop_names {
        protocol::enum_names::time };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        // header.extra_aligned_units == 2 * events_ct ((reply length - default) / 4)
        struct [[gnu::packed]] Encoding {
            Header   header;
            uint32_t events_ct;  // number of TIMECOORDs in events
        private:
            uint8_t  _unused[20];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
        // followed by LISTofTIMECOORD events (sizeof(TIMECOORD) * events_ct)B

        struct [[gnu::packed]] TIMECOORD {
            TIMESTAMP time;
            INT16     x;
            INT16     y;
        };
    };
};

struct TranslateCoordinates : public Request {
    // Header.opcode == 40
    // Header.tl_aligned_units == 4
    struct [[gnu::packed]] Encoding {
        WINDOW src_window;  // src-window
        WINDOW dst_window;  // dst-window
        INT16  src_x;       // src-x
        INT16  src_y;       // src-y
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            BOOL     same_screen;          // same-screen
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // 0 ((reply length - default) / 4)
        };
        struct [[gnu::packed]] Encoding {
            Header  header;
            WINDOW  child;  // 0 None
            INT16   dst_x;  // dst-x
            INT16   dst_y;  // dst-y
        private:
            uint8_t _unused[16];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );

        inline static const
        std::vector< std::string_view >& child_names {
            protocol::enum_names::zero_none };
    };
};

struct WarpPointer : public Request {
    // Header.opcode == 41
    // Header.tl_aligned_units == 6
    struct [[gnu::packed]] Encoding {
        WINDOW src_window;  // src-window  // 0 None
        WINDOW dst_window;  // dst-window  // 0 None
        INT16  src_x;       // src-x
        INT16  src_y;       // src-y
        CARD16 src_width;   // src-width
        CARD16 src_height;  // src-height
        INT16  dst_x;       // dst-x
        INT16  dst_y;       // dst-y
    };

    inline static const
    std::vector< std::string_view >& src_window_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& dst_window_names {
        protocol::enum_names::zero_none };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct SetInputFocus : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 42
        uint8_t  revert_to;         // revert-to  // 0 None 1 PointerRoot 2 Parent
        uint16_t tl_aligned_units;  // 3
    };
    struct [[gnu::packed]] Encoding {
        WINDOW    focus;  // 0 None 1 PointerRoot
        TIMESTAMP time;   // 0 CurrentTime
    };

    inline static const
    std::vector< std::string_view >& revert_to_names {
        protocol::enum_names::input_focus };
    inline static const
    std::vector< std::string_view >& focus_names {
        protocol::enum_names::input_focus };
    static constexpr uint8_t FOCUS_ENUM_MAX { 1 };
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GetInputFocus : public impl::SimpleRequest {
    // Header.opcode == 43
    // Header.tl_aligned_units == 1

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            uint8_t  revert_to;            // revert-to  // 0 None 1 PointerRoot 2 Parent
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // 0 ((reply length - default) / 4)
        };
        struct [[gnu::packed]] Encoding {
            Header  header;
            WINDOW  focus;        // 0 None 1 PointerRoot
        private:
            uint8_t _unused[20];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );

        inline static const
        std::vector< std::string_view >& revert_to_names {
            protocol::enum_names::input_focus };
        inline static const
        std::vector< std::string_view >& focus_names {
            protocol::enum_names::input_focus };
        static constexpr int FOCUS_ENUM_MAX { 1 };
    };
};

struct QueryKeymap : public impl::SimpleRequest {
    // Header.opcode == 44
    // Header.tl_aligned_units == 1

    struct Reply : public requests::Reply {
        // header.extra_aligned_units == 2 ((reply length - default) / 4)
        struct [[gnu::packed]] Encoding {
            Header header;
            CARD8  keys[32];  // LISTofCARD8 32B bit-vector
        };
        static_assert( sizeof( Encoding ) ==
                       DEFAULT_ENCODING_SZ + ( 2 * ALIGN ) );
    };
};

struct OpenFont : public Request {
    // Header.opcode == 45
    // Header.tl_aligned_units == 3+pad(n)/4
    struct [[gnu::packed]] Encoding {
        FONT     fid;
        uint16_t name_len;  // length of name in bytes
    private:
        uint8_t  _unused[2];
    };
    // followed by STRING8 name pad(n)B

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct CloseFont : public Request {
    // Header.opcode == 46
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        FONT font;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

namespace impl {

struct [[gnu::packed]] FONTPROP {
    ATOM     name;
    uint32_t value;  // <32-bits>
};

struct [[gnu::packed]] CHARINFO {
    INT16    left_side_bearing;  // left-side-bearing
    INT16    right_side_bearing;  // right-side-bearing
    INT16    character_width;  // character-width
    INT16    ascent;
    INT16    descent;
    CARD16   attributes;
};

}  // namespace impl

struct QueryFont : public Request {
    // Header.opcode == 47
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        FONTABLE font;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        using FONTPROP = impl::FONTPROP;
        using CHARINFO = impl::CHARINFO;

        // header.extra_aligned_units == 7 + 2 * properties_ct + 3 * char_infos_ct
        //   ((reply length - default) / 4)
        struct [[gnu::packed]] Encoding {
            Header   header;
            CHARINFO min_bounds;         // min-bounds
        private:
            uint8_t  _unused1[4];
        public:
            CHARINFO max_bounds;         // max-bounds
        private:
            uint8_t  _unused2[4];
        public:
            CARD16   min_char_or_byte2;  // min-char-or-byte2
            CARD16   max_char_or_byte2;  // max-char-or-byte2
            CARD16   default_char;       // default-char
            uint16_t properties_ct;      // number of FONTPROPs in properties
            uint8_t  draw_direction;     // draw_direction  // 0 LeftToRight 1 RightToLeft
            CARD8    min_byte1;          // min-byte1
            CARD8    max_byte1;          // max-byte1
            BOOL     all_chars_exist;    // all-chars-exist
            INT16    font_ascent;        // font-ascent
            INT16    font_descent;       // font-descent
            uint32_t char_infos_ct;      // number of CHARINFOs in char-infos
        };
        static_assert( sizeof( Encoding ) ==
                       DEFAULT_ENCODING_SZ + ( 7 * ALIGN ) );
        // followed by LISTofFONTPROP properties ( 8 * properties_ct )B
        // followed by LISTofCHARINFO char-infos ( 12 * char_infos_ct )B

        inline static const
        std::vector< std::string_view >& draw_direction_names {
            protocol::enum_names::draw_direction };
    };
};

struct QueryTextExtents : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 48
        BOOL     odd_length;        // odd length, True if padded by 2
        uint16_t tl_aligned_units;  // 2+pad(2n)/4
    };
    struct [[gnu::packed]] Encoding {
        FONTABLE   font;
    };
    // followed by STRING16 string pad(2n)B

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            uint8_t  draw_direction;       // draw-direction  // 0 LeftToRight 1 RightToLeft
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // 0 ((reply length - default) / 4)
        };
        struct [[gnu::packed]] Encoding {
            Header  header;
            INT16   font_ascent;      // font-ascent
            INT16   font_descent;     // font-descent
            INT16   overall_ascent;   // overall-ascent
            INT16   overall_descent;  // overall-descent
            INT32   overall_width;    // overall-width
            INT32   overall_left;     // overall-left
            INT32   overall_right;    // overall-right
        private:
            uint8_t _unused[4];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );

        inline static const
        std::vector< std::string_view >& draw_direction_names {
            protocol::enum_names::draw_direction };
    };
};

namespace impl {

struct ListFontsRequest : public Request {
    // Header.tl_aligned_units == 2 + pad(pattern_len)/4
    struct [[gnu::packed]] Encoding {
        CARD16   max_names;    // max-names
        uint16_t pattern_len;  // length of pattern in bytes
    };
    // followed by STRING8 pattern pad(pattern_len)B

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    virtual ~ListFontsRequest() = 0;
};

}  // namespace impl

struct ListFonts : public impl::ListFontsRequest {
    // Header.opcode == 49
    // Header.tl_aligned_units == 2+pad(Header.pattern_len)/4
    // Encoding followed by STRING8 pattern pad(Header.pattern_len)B

    struct Reply : public requests::Reply {
        // header.extra_aligned_units == pad(n)/4 ((reply length - default) / 4)
        struct [[gnu::packed]] Encoding {
            Header  header;
            CARD16  names_ct;       // number of STRs in names
        private:
            uint8_t _unused2[22];
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ );
        // followed by LISTofSTR names pad(n)B
        // TBD note that in protocol, encoding uses LISTofSTR, while request
        //   description uses LISTofSTRING8
    };
};

struct ListFontsWithInfo : public impl::ListFontsRequest {
    // encoding.opcode == 50
    // Header.tl_aligned_units == 2+pad(Header.pattern_len)/4
    // Encoding followed by STRING8 pattern pad(Header.pattern_len)B

    struct Reply : public requests::Reply {
        using FONTPROP = impl::FONTPROP;
        using CHARINFO = impl::CHARINFO;

       struct [[gnu::packed]] Header {
           uint8_t  reply;                // 1
           union [[gnu::packed]] {
               uint8_t name_len;             // length of name in bytes, never 0
               uint8_t last_reply;           // 0 last-reply indicator
           };
           CARD16   sequence_num;         // sequence number
           uint32_t extra_aligned_units;  // 7 + 2 * properties_ct + pad(name_len) / 4
                                          //   ((reply length - default) / 4)
       };
        static constexpr uint8_t LAST_REPLY {};
        // if header.last_reply == LAST_REPLY, then fields after header are ignored
        struct [[gnu::packed]] Encoding {
            Header   header;
            CHARINFO min_bounds;         // min-bounds
        private:
            uint8_t  _unused1[4];
        public:
            CHARINFO max_bounds;         // max-bounds
        private:
            uint8_t  _unused2[4];
        public:
            CARD16   min_char_or_byte2;  // min-char-or-byte2
            CARD16   max_char_or_byte2;  // max-char-or-byte2
            CARD16   default_char;       // default-char
            uint16_t properties_ct;      // number of FONTPROPs in properties
            uint8_t  draw_direction;     // draw-direction  // 0 LeftToRight 1 RightToLeft
            CARD8    min_byte1;          // min-byte1
            CARD8    max_byte1;          // max-byte1
            BOOL     all_chars_exist;    // all-chars-exist
            INT16    font_ascent;        // font-ascent
            INT16    font_descent;       // font-descent
            CARD32   replies_hint;       // replies-hint
        };
        static_assert( sizeof(Encoding) == DEFAULT_ENCODING_SZ + ( 7 * ALIGN ) );
        // followed by LISTofFONTPROP properties ( 8 * properties_ct )B
        // followed by STRING8 name pad(name_len)B

        inline static const
        std::vector< std::string_view >& draw_direction_names {
            protocol::enum_names::draw_direction };
    };
};

struct SetFontPath : public Request {
    // Header.opcode == 51
    // Header.tl_aligned_units == 2+pad(n)/4
    struct [[gnu::packed]] Encoding {
        CARD16  path_ct;      // number of STRs in path
    private:
        uint8_t _unused[2];
    };
    // followed by LISTofSTR path pad(n)B
    // TBD note use of LISTofSTR in encoding while request description uses LISTofSTRING8

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GetFontPath : public impl::SimpleRequest {
    // Header.opcode == 52
    // Header.tl_aligned_units == 1

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == pad(n)/4 ((reply length - default) / 4)
        struct [[gnu::packed]] Encoding {
            Header  header;
            CARD16  path_ct;      // number of STRs in path
        private:
            uint8_t _unused[22];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        // followed by LISTofSTR path pad(n)B
        // TBD note use of LISTofSTR in encoding while request description uses LISTofSTRING8
    };
};

struct CreatePixmap : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 53
        CARD8    depth;
        uint16_t tl_aligned_units;  // 4
    };
    struct [[gnu::packed]] Encoding {
        PIXMAP   pid;
        DRAWABLE drawable;
        CARD16   width;
        CARD16   height;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct FreePixmap : public Request {
    // Header.opcode == 54
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        PIXMAP     pixmap;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

namespace impl {

struct GCRequest : public Request {
    inline static constexpr
    std::tuple<
        uint8_t,  // function  // 0 Clear 1 And 2 AndReverse 3 Copy 4 AndInverted 5 NoOp 6 Xor 7 Or 8 Nor 9 Equiv 10 Invert 11 OrReverse 12 CopyInverted 13 OrInverted 14 Nand 15 Set
        CARD32,   // plane-mask
        CARD32,   // foreground
        CARD32,   // background
        CARD16,   // line-width
        uint8_t,  // line-style  // 0 Solid 1 OnOffDash 2 DoubleDash
        uint8_t,  // cap-style  // 0 NotLast 1 Butt 2 Round 3 Projecting
        uint8_t,  // join-style  // 0 Miter 1 Round 2 Bevel
        uint8_t,  // fill-style  // 0 Solid 1 Tiled 2 Stippled 3 OpaqueStippled
        uint8_t,  // fill-rule  // 0 EvenOdd 1 Winding
        PIXMAP,   // tile
        PIXMAP,   // stipple
        INT16,    // tile-stipple-x-origin
        INT16,    // tile-stipple-y-origin
        FONT,     // font
        uint8_t,  // subwindow-mode  // 0 ClipByChildren 1 IncludeInferiors
        BOOL,     // graphics-exposures
        INT16,    // clip-x-origin
        INT16,    // clip-y-origin
        PIXMAP,   // clip-mask  // 0 None
        CARD16,   // dash-offset
        CARD8,    // dashes
        uint8_t   // arc-mode  // 0 Chord 1 PieSlice
        > value_types {};

    inline static const
    std::vector< std::string_view >& value_names {
        protocol::enum_names::gc_value_mask };
    inline static const
    std::vector< std::string_view >& function_names {
        protocol::enum_names::gc_value_function };
    inline static const
    std::vector< std::string_view >& line_style_names {
        protocol::enum_names::gc_value_line_style };
    inline static const
    std::vector< std::string_view >& cap_style_names {
        protocol::enum_names::gc_value_cap_style };
    inline static const
    std::vector< std::string_view >& join_style_names {
        protocol::enum_names::gc_value_join_style };
    inline static const
    std::vector< std::string_view >& fill_style_names {
        protocol::enum_names::gc_value_fill_style };
    inline static const
    std::vector< std::string_view >& fill_rule_names {
        protocol::enum_names::gc_value_fill_rule };
    inline static const
    std::vector< std::string_view >& subwindow_mode_names {
        protocol::enum_names::gc_value_subwindow_mode };
    inline static const
    std::vector< std::string_view >& clip_mask_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& arc_mode_names {
        protocol::enum_names::gc_value_arc_mode };
};

}  // namespace impl

struct CreateGC : public impl::GCRequest {
    // Header.opcode == 55
    // Header.tl_aligned_units == 4+n
    struct [[gnu::packed]] Encoding {
        GCONTEXT cid;
        DRAWABLE drawable;
        uint32_t value_mask;  // BITMASK value-mask 4B (has n bits set to 1)
    };
    // followed by LISTofVALUE value-list 4nB

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct ChangeGC : public impl::GCRequest {
    // Header.opcode == 56
    // Header.tl_aligned_units == 3+n
    struct [[gnu::packed]] Encoding {
        GCONTEXT gc;
        uint32_t value_mask;  // 4B BITMASK value-mask (has n bits set to 1)
    };
    // followed by LISTofVALUE value-list 4nB

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct CopyGC : public impl::GCRequest {
    // Header.opcode == 57
    // Header.tl_aligned_units == 4
    struct [[gnu::packed]] Encoding {
        GCONTEXT src_gc;      // src-gc
        GCONTEXT dst_gc;      // dst-gc
        uint32_t value_mask;  // 4B BITMASK value-mask (has n bits set to 1)
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct SetDashes : public Request {
    // Header.opcode == 58
    // Header.tl_aligned_units == 3+pad(dashes_len)/4
    struct [[gnu::packed]] Encoding {
        GCONTEXT gc;
        CARD16   dash_offset;  // dash-offset
        uint16_t dashes_len;   // length of dashes in bytes
    };
    // followed by LISTofCARD8 dashes pad(dashes_len)B

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct SetClipRectangles : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 59
        uint8_t  ordering;          // 0 UnSorted 1 YSorted 2 YXSorted 3 YXBanded
        uint16_t tl_aligned_units;  // 3+2n
    };
    struct [[gnu::packed]] Encoding {
        GCONTEXT gc;
        INT16    clip_x_origin;  // clip-x-origin
        INT16    clip_y_origin;  // clip-y-origin
    };
    // followed by LISTofRECTANGLE rectangles 8nB

    inline static const
    std::vector< std::string_view > ordering_names {
        "UnSorted",  // 0
        "YSorted",   // 1
        "YXSorted",  // 2
        "YXBanded"   // 3
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct FreeGC : public Request {
    // Header.opcode == 60
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        GCONTEXT gc;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct ClearArea : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 61
        BOOL     exposures;
        uint16_t tl_aligned_units;  // 4
    };
    struct [[gnu::packed]] Encoding {
        WINDOW window;
        INT16  x;
        INT16  y;
        CARD16 width;
        CARD16 height;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct CopyArea : public Request {
    // Header.opcode == 62
    // Header.tl_aligned_units == 7
    struct [[gnu::packed]] Encoding {
        DRAWABLE src_drawable;  // src-drawable
        DRAWABLE dst_drawable;  // dst-drawable
        GCONTEXT gc;            // gc
        INT16    src_x;         // src-x
        INT16    src_y;         // src-y
        INT16    dst_x;         // dst-x
        INT16    dst_y;         // dst-y
        CARD16   width;
        CARD16   height;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct CopyPlane : public Request {
    // Header.opcode == 63
    // Header.tl_aligned_units == 8
    struct [[gnu::packed]] Encoding {
        DRAWABLE src_drawable;  // src-drawable
        DRAWABLE dst_drawable;  // dst-drawable
        GCONTEXT gc;
        INT16    src_x;         // src-x
        INT16    src_y;         // src-y
        INT16    dst_x;         // dst-x
        INT16    dst_y;         // dst-y
        CARD16   width;
        CARD16   height;
        CARD32   bit_plane;     // bit-plane
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

namespace impl {

struct PolyPointRequest : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;
        uint8_t  coordinate_mode;   // coordinate-mode  // 0 Origin 1 Previous
        uint16_t tl_aligned_units;  // 3+n
    };
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        GCONTEXT gc;
    };
    // followed by LISTofPOINT points 4nB

    inline static const
    std::vector< std::string_view >& coordinate_mode_names {
        protocol::enum_names::poly_coordinate_mode };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

}  // namespace impl

struct PolyPoint : public impl::PolyPointRequest {
    // Header.opcode == 64
};

struct PolyLine : public impl::PolyPointRequest {
    // Header.opcode == 65
};

// TBD PolySegment, PolyRectangle, PolyArc, PolyFillRectangle, PolyFillArc parent?
struct PolySegment : public Request {
    // Header.opcode == 66
    // Header.tl_aligned_units == 3+2n
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        GCONTEXT gc;
    };
    // followed by LISTofSEGMENT segments 8nB

    struct [[gnu::packed]] SEGMENT {
        INT16 x1, y1;
        INT16 x2, y2;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct PolyRectangle : public Request {
    // Header.opcode == 67
    // Header.tl_aligned_units == 3+2n
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        GCONTEXT gc;
    };
    // followed by LISTofRECTANGLE rectangles 8nB

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct PolyArc : public Request {
    // Header.opcode == 68
    // Header.tl_aligned_units == 3+3n
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        GCONTEXT gc;
    };
    // followed by LISTofARC arcs 12nB

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct FillPoly : public Request {
    // Header.opcode == 69
    // Header.tl_aligned_units == 4+n
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        GCONTEXT gc;
        uint8_t  shape;            // 0 Complex 1 Nonconvex 2 Convex
        uint8_t  coordinate_mode;  // coordinate-mode  // 0 Origin 1 Previous
    private:
        uint8_t  _unused[2];
    };
    // followed by LISTofPOINT points 4nB

    inline static const
    std::vector< std::string_view > shape_names {
        "Complex",    // 0
        "Nonconvex",  // 1
        "Convex"      // 2
    };
    inline static const
    std::vector< std::string_view >& coordinate_mode_names {
        protocol::enum_names::poly_coordinate_mode };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct PolyFillRectangle : public Request {
    // Header.opcode == 70
    // Header.tl_aligned_units == 3+2n
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        GCONTEXT gc;
    };
    // followed by LISTofRECTANGLE rectangles 8nB

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct PolyFillArc : public Request {
    // Header.opcode == 71
    // Header.tl_aligned_units == 3+3n
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        GCONTEXT gc;
    };
    // followed by LISTofARC arcs 12nB

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct PutImage : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 72
        uint8_t  format;            // 0 Bitmap 1 XYPixmap 2 ZPixmap
        uint16_t tl_aligned_units;  // 6+pad(n)/4
    };
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        GCONTEXT gc;
        CARD16   width;
        CARD16   height;
        INT16    dst_x;     // dst-x
        INT16    dst_y;     // dst-y
        CARD8    left_pad;  // left-pad
        CARD8    depth;
    private:
        uint8_t  _unused[2];
    };
    // followed by LISTofBYTE data pad(n)B

    inline static const
    std::vector< std::string_view >& format_names {
        protocol::enum_names::image_format };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GetImage : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 73
        uint8_t  format;            // 1 XYPixmap 2 ZPixmap
        uint16_t tl_aligned_units;  // 5
    };
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        INT16    x;
        INT16    y;
        CARD16   width;
        CARD16   height;
        CARD32   plane_mask;  // plane-mask
    };

    inline static const
    std::vector< std::string_view >& format_names {
        protocol::enum_names::image_format };
    static constexpr uint8_t FORMAT_ENUM_MIN { 1 };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            CARD8    depth;
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // pad(n) / 4
        };
        struct [[gnu::packed]] Encoding {
            Header   header;
            VISUALID visual;       // 0 None
        private:
            uint8_t  _unused[20];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        // followed by LISTofBYTE data pad(n)B

        inline static const
        std::vector< std::string_view >& visual_names {
            protocol::enum_names::zero_none };
    };
};

struct PolyText8 : public Request {
    // Header.opcode == 74
    // Header.tl_aligned_units == 4+pad(n)/4
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        GCONTEXT gc;
        INT16    x;
        INT16    y;
    };
    // followed by LISTofTEXTITEM8 items pad(n)B

    // TBD core X11 font storage: https://www.x.org/wiki/guide/fonts/
    static constexpr uint8_t FONT_SHIFT { 255 };
    union TEXTITEM8 {
        struct [[gnu::packed]] TEXTELT8 {
            uint8_t string_len;  // string length in bytes (cannot be FONT_SHIFT)
            INT8    delta;
        } text_element;
        // followed by STRING8 string string_lenB
        struct [[gnu::packed]] FONT {
            uint8_t font_shift;     // font-shift FONT_SHIFT
            uint8_t font_bytes[4];  // font byte 3 (most-significant) to
                                    //   font byte 0 (least-significant)
                                    //   (should convert to protocol::FONT)
        } font;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct PolyText16 : public Request {
    // Header.opcode == 75
    // Header.tl_aligned_units == 4+pad(n)/4
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        GCONTEXT gc;
        INT16    x;
        INT16    y;
    };
    // followed by LISTofTEXTITEM16 items pad(n)B

    // TBD core X11 font storage: https://www.x.org/wiki/guide/fonts/
    static constexpr uint8_t FONT_SHIFT { 255 };
    union TEXTITEM16 {
        struct [[gnu::packed]] TEXTELT16 {
            uint8_t string_2B_len;  // string length in CHAR2B (cannot be FONT_SHIFT)
            INT8    delta;
        } text_element;
        // followed by STRING16 string 2string_lenB
        struct [[gnu::packed]] FONT {
            uint8_t font_shift;     // font-shift FONT_SHIFT
            uint8_t font_bytes[4];  // font byte 3 (most-significant) to
                                    //   font byte 0 (least-significant)
                                    //   (should convert to protocol::FONT)
        } font;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct ImageText8 : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 76
        uint8_t  string_len;        // length of string in bytes
        uint16_t tl_aligned_units;  // 4+(n+p)/4
    };
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        GCONTEXT gc;
        INT16    x;
        INT16    y;
    };
    // followed by STRING8 string pad(n)B

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct ImageText16 : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 77
        uint8_t  string_2B_len;     // length of string in CHAR2B
        uint16_t tl_aligned_units;  // 4+(2n+p)/4
    };
    struct [[gnu::packed]] Encoding {
        DRAWABLE   drawable;
        GCONTEXT   gc;
        INT16      x;
        INT16      y;
    };
    // followed by STRING16 string pad(2n)B

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct CreateColormap : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 78
        uint8_t  alloc;             // 0 None 1 All
        uint16_t tl_aligned_units;  // 4
    };
    struct [[gnu::packed]] Encoding {
        COLORMAP mid;
        WINDOW   window;
        VISUALID visual;
    };

    inline static const
    std::vector< std::string_view > alloc_names {
        "None",  // 0
        "All"    // 1
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

namespace impl {

// TBD StoreColors, QueryColors?
struct SimpleCmapRequest : public Request {
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        COLORMAP cmap;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

}  // namespace impl

struct FreeColormap : public impl::SimpleCmapRequest {
    // Header.opcode == 79
    // Header.tl_aligned_units == 2
};

struct CopyColormapAndFree : public Request {
    // Header.opcode == 80
    // Header.tl_aligned_units == 3
    struct [[gnu::packed]] Encoding {
        COLORMAP mid;
        COLORMAP src_cmap;  // src-cmap
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct InstallColormap : public impl::SimpleCmapRequest {
    // Header.opcode == 81
    // Header.tl_aligned_units == 2
};

struct UninstallColormap : public impl::SimpleCmapRequest {
    // Header.opcode == 82
    // Header.tl_aligned_units == 2
};

struct ListInstalledColormaps : public impl::SimpleWindowRequest {
    // Header.opcode == 83
    // Header.tl_aligned_units == 2

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            CARD8    depth;
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // cmaps_ct
        };
        struct [[gnu::packed]] Encoding {
            Header   header;
            uint16_t cmaps_ct;  // number of COLORMAPs in cmaps
        private:
            uint8_t  _unused[22];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        // followed by LISTofCOLORMAP cmaps (4 * cmaps_ct)B
    };
};

struct AllocColor : public Request {
    // Header.opcode == 84
    // Header.tl_aligned_units == 4
    struct [[gnu::packed]] Encoding {
        COLORMAP cmap;
        CARD16   red;
        CARD16   green;
        CARD16   blue;
    private:
        uint8_t  _unused[2];
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == 0
        struct [[gnu::packed]] Encoding {
            Header  header;
            CARD16  red;
            CARD16  green;
            CARD16  blue;
        private:
            uint8_t _unused1[2];
        public:
            CARD32  pixel;
        private:
            uint8_t _unused2[12];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};

struct AllocNamedColor : public Request {
    // Header.opcode == 85
    // Header.tl_aligned_units == 3+pad(name_len)/4
    struct [[gnu::packed]] Encoding {
        COLORMAP cmap;
        uint16_t name_len;  // length of name in bytes
    private:
        uint8_t  _unused[2];
    };
    // followed by STRING8 name pad(name_len)B

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == 0
        struct [[gnu::packed]] Encoding {
            Header  header;
            CARD32  pixel;
            CARD16  exact_red;     // exact-red
            CARD16  exact_green;   // exact-green
            CARD16  exact_blue;    // exact-blue
            CARD16  visual_red;    // visual-red
            CARD16  visual_green;  // visual-green
            CARD16  visual_blue;   // visual-blue
        private:
            uint8_t _unused[8];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};

struct AllocColorCells : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 86
        BOOL     contiguous;
        uint16_t tl_aligned_units;  // 3
    };
    struct [[gnu::packed]] Encoding {
        COLORMAP cmap;
        CARD16   colors;
        CARD16   planes;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == pixels_ct + masks_ct
        struct [[gnu::packed]] Encoding {
            Header   header;
            uint16_t pixels_ct;  // number of CARD32s in pixels
            uint16_t masks_ct;   // number of CARD32s in masks
        private:
            uint8_t  _unused[20];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        // followed by LISTofCARD32 pixels (4 * pixels_ct)B
        // followed by LISTofCARD32 masks (4 * masks_ct)B
    };
};

struct AllocColorPlanes : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 87
        BOOL     contiguous;
        uint16_t tl_aligned_units;  // 4
    };
    struct [[gnu::packed]] Encoding {
        COLORMAP cmap;
        CARD16   colors;
        CARD16   reds;
        CARD16   greens;
        CARD16   blues;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == pixels_ct
        struct [[gnu::packed]] Encoding {
            Header   header;
            uint16_t pixels_ct;   // number of CARD32s in pixels
        private:
            uint8_t  _unused1[2];
        public:
            CARD32   red_mask;    // red-mask
            CARD32   green_mask;  // green-mask
            CARD32   blue_mask;   // blue-mask
        private:
            uint8_t  _unused2[8];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        // followed by LISTofCARD32 pixels (4 * pixels_ct)B
    };
};

struct FreeColors : public Request {
    // Header.opcode == 88
    // Header.tl_aligned_units == 3+n
    struct [[gnu::packed]] Encoding {
        COLORMAP cmap;
        CARD32   plane_mask;  // plane-mask
    };
    // followed by LISTofCARD32 pixels 4nB

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct StoreColors : public Request {
    // Header.opcode == 89
    // Header.tl_aligned_units == 2+3n
    struct [[gnu::packed]] Encoding {
        COLORMAP cmap;
    };
    // followed by LISTofCOLORITEM items 12nB

    struct [[gnu::packed]] COLORITEM {
        CARD32  pixel;
        CARD16  red;
        CARD16  green;
        CARD16  blue;
        uint8_t do_rgb_mask;  // 0x01 do-red 0x02 do-green 0x04 do-blue 0xF8 unused
    private:
        uint8_t _unused;
    public:

        inline static const
        std::vector< std::string_view >& do_rgb_names {
            protocol::enum_names::do_rgb_mask };
        static constexpr uint8_t DO_RGB_ZERO_BITS { 0xF8 };
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct StoreNamedColor : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 90
        uint8_t  do_rgb_mask;       // 0x01 do-red 0x02 do-green 0x04 do-blue 0xF8 unused
        uint16_t tl_aligned_units;  // 4+pad(name_len)/4
    };
    struct [[gnu::packed]] Encoding {
        COLORMAP cmap;
        CARD32   pixel;
        uint16_t name_len;  // length of name in bytes
    private:
        uint16_t _unused;
    };
    // followed by STRING8 name pad(name_len)B

    inline static const
    std::vector< std::string_view >& do_rgb_names {
        protocol::enum_names::do_rgb_mask };
    static constexpr uint8_t DO_RGB_ZERO_BITS { 0xF8 };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct QueryColors : public Request {
    // Header.opcode == 91
    // Header.tl_aligned_units == 2+n
    struct [[gnu::packed]] Encoding {
        COLORMAP   cmap;
    };
    // followed by LISTofCARD32 pixels 4nB

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == 2 * colors_ct
        struct [[gnu::packed]] Encoding {
            Header   header;
            uint16_t colors_ct;    // number of RGBs in colors
        private:
            uint8_t  _unused[22];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        // followed by LISTofRGB colors ( 8 * colors_ct )B

        struct [[gnu::packed]] RGB {
            CARD16  red;
            CARD16  green;
            CARD16  blue;
        private:
            uint8_t _unused[2];
        };
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct LookupColor : public Request {
    // Header.opcode == 92
    // Header.tl_aligned_units == 3+pad(name_len)/4
    struct [[gnu::packed]] Encoding {
        COLORMAP cmap;
        uint16_t name_len;  // length of name in bytes
    private:
        uint8_t  _unused[2];
    };
    // followed by STRING8 name pad(name_len)B

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == 0
        struct [[gnu::packed]] Encoding {
            Header  header;
            CARD16  exact_red;     // exact-red
            CARD16  exact_green;   // exact-green
            CARD16  exact_blue;    // exact-blue
            CARD16  visual_red;    // visual-red
            CARD16  visual_green;  // visual-green
            CARD16  visual_blue;   // visual-blue
        private:
            uint8_t _unused[12];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct CreateCursor : public Request {
    // Header.opcode == 93
    // Header.tl_aligned_units == 8
    struct [[gnu::packed]] Encoding {
        CURSOR cid;
        PIXMAP source;
        PIXMAP mask;        // 0 None
        CARD16 fore_red;    // fore-red
        CARD16 fore_green;  // fore-green
        CARD16 fore_blue;   // fore-blue
        CARD16 back_red;    // back-red
        CARD16 back_green;  // back-green
        CARD16 back_blue;   // back-blue
        CARD16 x;
        CARD16 y;
    };

    inline static const
    std::vector< std::string_view >& mask_names {
        protocol::enum_names::zero_none };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct CreateGlyphCursor : public Request {
    // Header.opcode == 94
    // Header.tl_aligned_units == 8
    struct [[gnu::packed]] Encoding {
        CURSOR cid;
        FONT   source_font;  // source-font
        FONT   mask_font;    // mask-font 0 None
        CARD16 source_char;  // source-char
        CARD16 mask_char;    // mask-char
        CARD16 fore_red;     // fore-red
        CARD16 fore_green;   // fore-green
        CARD16 fore_blue;    // fore-blue
        CARD16 back_red;     // back-red
        CARD16 back_green;   // back-green
        CARD16 back_blue;    // back-blue
    };

    inline static const
    std::vector< std::string_view >& mask_font_names {
        protocol::enum_names::zero_none };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct FreeCursor : public Request {
    // Header.opcode == 95
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        CURSOR cursor;
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct RecolorCursor : public Request {
    // Header.opcode == 96
    // Header.tl_aligned_units == 5
    struct [[gnu::packed]] Encoding {
        CURSOR cursor;
        CARD16 fore_red;    // fore-red
        CARD16 fore_green;  // fore-green
        CARD16 fore_blue;   // fore-blue
        CARD16 back_red;    // back-red
        CARD16 back_green;  // back-green
        CARD16 back_blue;   // back-blue
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct QueryBestSize : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 97
        uint8_t  class_;            // 0 Cursor 1 Tile 2 Stipple
        uint16_t tl_aligned_units;  // 3
    };
    struct [[gnu::packed]] Encoding {
        DRAWABLE drawable;
        CARD16   width;
        CARD16   height;
    };

    inline static const
    std::vector< std::string_view > class_names {
        "Cursor",  // 0
        "Tile",    // 1
        "Stipple"  // 2
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == 0
        struct [[gnu::packed]] Encoding {
            Header  header;
            CARD16  width;
            CARD16  height;
        private:
            uint8_t _unused[20];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};

struct QueryExtension : public Request {
    // Header.opcode == 98
    // Header.tl_aligned_units == 2+pad(name_len)/4
    struct [[gnu::packed]] Encoding {
        uint16_t name_len;  // length of name in bytes
    private:
        uint8_t  _unused[2];
    };
    // followed by STRING8 name pad(name_len)B

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == 0
        struct [[gnu::packed]] Encoding {
            Header  header;
            BOOL    present;       // present
            CARD8   major_opcode;  // major-opcode
            CARD8   first_event;   // first-event
            CARD8   first_error;   // first-error
        private:
            uint8_t _unused[20];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};

struct ListExtensions : public impl::SimpleRequest {
    // Header.opcode == 99
    // Header.tl_aligned_units == 1

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            CARD8    names_ct;             // number of STRs in names
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // pad(n) / 4
        };
        struct [[gnu::packed]] Encoding {
            Header  header;
        private:
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        // followed by LISTofSTR names pad(n)B
    };
};

struct ChangeKeyboardMapping : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 100
        uint8_t  keycode_count;     // n keycode-count
        uint16_t tl_aligned_units;  // 2+nm
    };
    struct [[gnu::packed]] Encoding {
        KEYCODE first_keycode;        // first-keycode
        uint8_t keysyms_per_keycode;  // m keysyms-per-keycode
    private:
        uint8_t _unused[2];
    };
    // followed by LISTofKEYSYM keysyms 4nmB

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GetKeyboardMapping : public Request {
    // Header.opcode == 101
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        KEYCODE first_keycode;  // first-keycode
        uint8_t count;          // count
    private:
        uint8_t _unused[2];
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            uint8_t  keysyms_per_keycode;  // keysyms-per-keycode
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // keysyms_per_keycode * (count from request)
        };
        struct [[gnu::packed]] Encoding {
            Header  header;
        private:
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        // followed by LISTofKEYSYM keysyms (keysyms_per_keycode * (count from request))B
    };
};

struct ChangeKeyboardControl : public Request {
    // Header.opcode == 102
    // Header.tl_aligned_units == 2+n
    struct [[gnu::packed]] Encoding {
        uint32_t   value_mask;  // 4B BITMASK value-mask (has n bits set to 1)
    };
    // followed by LISTofVALUE value-list 4nB

    inline static constexpr
    std::tuple<
        INT8,     // key-click-percent
        INT8,     // bell-percent
        INT16,    // bell-pitch
        INT16,    // bell-duration
        CARD8,    // led;
        uint8_t,  // led-mode  // 0 Off 1 On
        KEYCODE,  // key;
        uint8_t   // auto-repeat-mode  // 0 Off 1 On 2 Default
        > value_types {};

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
    inline static const
    std::vector< std::string_view >& led_mode_names {
        protocol::enum_names::off_on };
    static constexpr CARD8 LED_MODE_ENUM_MAX { 1 };
    inline static const
    std::vector< std::string_view >& auto_repeat_mode_names {
        protocol::enum_names::off_on };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GetKeyboardControl : public impl::SimpleRequest {
    // Header.opcode == 103
    // Header.tl_aligned_units == 1

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            uint8_t  global_auto_repeat;   // global-auto-repeat  // 0 Off 1 On
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // 5
        };
        static constexpr uint32_t AUTO_REPEATS_SZ { 32 };
        struct [[gnu::packed]] Encoding {
            Header  header;
            CARD32  led_mask;           // led-mask
            CARD8   key_click_percent;  // key-click-percent
            CARD8   bell_percent;       // bell-percent
            CARD16  bell_pitch;         // bell-pitch
            CARD16  bell_duration;      // bell-duration
        private:
            uint8_t _unused[2];
        public:
            CARD8   auto_repeats[AUTO_REPEATS_SZ];  // LISTofCARD8 auto-repeats
                                                    //   (included due to fixed size)
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ + (5 * ALIGN) );

        inline static const
        std::vector< std::string_view >& global_auto_repeat_names {
            protocol::enum_names::off_on };
        static constexpr uint8_t GLOBAL_AUTO_REPREAT_ENUM_MAX { 1 };
    };
};

struct Bell : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 104
        INT8     percent;
        uint16_t tl_aligned_units;  // 1
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };
};

struct ChangePointerControl : public Request {
    // Header.opcode == 105
    // Header.tl_aligned_units == 3
    struct [[gnu::packed]] Encoding {
        INT16 acceleration_numerator;    // acceleration-numerator
        INT16 acceleration_denominator;  // acceleration-denominator
        INT16 threshold;
        BOOL  do_acceleration;           // do-acceleration
        BOOL  do_threshold;              // do-threshold
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GetPointerControl : public impl::SimpleRequest {
    // Header.opcode == 106
    // Header.tl_aligned_units == 1

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == 0
        struct [[gnu::packed]] Encoding {
            Header  header;
            CARD16  acceleration_numerator;    // acceleration-numerator
            CARD16  acceleration_denominator;  // acceleration-denominator
            CARD16  threshold;
        private:
            uint8_t _unused[18];
        };
    };
};

struct SetScreenSaver : public Request {
    // Header.opcode == 107
    // Header.tl_aligned_units == 3
    struct [[gnu::packed]] Encoding {
        INT16   timeout;
        INT16   interval;
        uint8_t prefer_blanking;  // prefer-blanking  // 0 No 1 Yes 2 Default
        uint8_t allow_exposures;  // allow-exposures  // 0 No 1 Yes 2 Default
    private:
        uint8_t _unused[2];
    };

    inline static const
    std::vector< std::string_view >& prefer_blanking_names {
        protocol::enum_names::no_yes };
    inline static const
    std::vector< std::string_view >& allow_exposures_names {
        protocol::enum_names::no_yes };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct GetScreenSaver : public impl::SimpleRequest {
    // Header.opcode == 108
    // Header.tl_aligned_units == 1

    struct Reply : public requests::Reply {
        // Header.extra_aligned_units == 0
        struct [[gnu::packed]] Encoding {
            Header  header;
            CARD16  timeout;
            CARD16  interval;
            uint8_t prefer_blanking;  // prefer-blanking  // 0 No 1 Yes
            uint8_t allow_exposures;  // allow-exposures  // 0 No 1 Yes
        private:
            uint8_t _unused[18];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );

        inline static const
        std::vector< std::string_view >& prefer_blanking_names {
            protocol::enum_names::no_yes };
        static constexpr int PREFER_BLANKING_ENUM_MAX { 1 };
        inline static const
        std::vector< std::string_view >& allow_exposures_names {
            protocol::enum_names::no_yes };
        static constexpr int ALLOW_EXPOSURES_ENUM_MAX { 1 };
    };
};

struct ChangeHosts : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;             // 109
        uint8_t  mode;              // 0 Insert 1 Delete
        uint16_t tl_aligned_units;  // 2+pad(address_len)/4
    };
    struct [[gnu::packed]] Encoding {
        uint8_t  family;       // 0 Internet 1 DECnet 2 Chaos
    private:
        uint8_t  _unused;
    public:
        uint16_t address_len;  // length of address in bytes
    };
    // followed by LISTofCARD8 address pad(address_len)B

    inline static const
    std::vector< std::string_view > mode_names {
        "Insert",  // 0
        "Delete"   // 1
    };
    inline static const
    std::vector< std::string_view >& family_names {
        protocol::enum_names::host_family };
    static constexpr uint8_t FAMILY_ENUM_MAX { 2 };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct ListHosts : public impl::SimpleRequest {
    // Header.opcode == 110
    // Header.tl_aligned_units == 1

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            uint8_t  mode;                 // 0 Disabled 1 Enabled
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // hosts_ct
        };
        struct [[gnu::packed]] Encoding {
            Header  header;
            CARD16  hosts_ct;        // number of HOSTs in hosts
        private:
            uint8_t _unused[22];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        // followed by LISTofHOST hosts (4*hosts_ct)B

        inline static const
        std::vector< std::string_view > mode_names {
            "Disabled",  // 0
            "Enabled"    // 1
        };
    };
};

struct SetAccessControl : public Request {
    struct [[gnu::packed]] Header {
        uint8_t    opcode;            // 111
        uint8_t    mode;              // 0 Disable 1 Enable
        uint16_t   tl_aligned_units;  // 1
    };

    inline static const
    std::vector< std::string_view > mode_names {
        "Disable",  // 0
        "Enable"    // 1
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };
};

struct SetCloseDownMode : public Request {
    struct [[gnu::packed]] Header {
        uint8_t    opcode;            // 112
        uint8_t    mode;              // 0 Destroy 1 RetainPermanent 2 RetainTemporary
        uint16_t   tl_aligned_units;  // 1
    };

    inline static const
    std::vector< std::string_view > mode_names {
        "Destroy",          // 0
        "RetainPermanent",  // 1
        "RetainTemporary"   // 2
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };
};

struct KillClient : public Request {
    // Header.opcode == 113
    // Header.tl_aligned_units == 2
    struct [[gnu::packed]] Encoding {
        CARD32 resource;  // 0 AllTemporary
    };

    inline static const
    std::vector< std::string_view > resource_names {
        "AllTemporary"  // 0
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) + sizeof( Encoding ) };
};

struct RotateProperties : public Request {
    // Header.opcode == 114
    // Header.tl_aligned_units == 3+n
    struct [[gnu::packed]] Encoding {
        WINDOW   window;
        uint16_t properties_ct;  // n properties
        INT16    delta;
    };
    // followed by LISTofATOM properties 4nB

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };
};

struct ForceScreenSaver : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 115
        uint8_t  mode;              // 0 Reset 1 Activate
        uint16_t tl_aligned_units;  // 1
    };

    inline static const
    std::vector< std::string_view > mode_names {
        "Reset",    // 0
        "Activate"  // 1
    };

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };
};

struct SetPointerMapping : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;            // 116
        uint8_t  map_len;           // length of map in bytes
        uint16_t tl_aligned_units;  // 1+pad(map_len)/4
    };
    // followed by LISTofCARD8 map pad(map_len)B

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            uint8_t  status;               // 0 Success 1 Busy
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // 0
        };
        struct [[gnu::packed]] Encoding {
            Header  header;
        private:
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );

        inline static const
        std::vector< std::string_view >& status_names {
            protocol::enum_names::mapping_status };
        static constexpr int STATUS_ENUM_MAX { 1 };
    };
};

struct GetPointerMapping : public impl::SimpleRequest {
    // Header.opcode == 117
    // Header.tl_aligned_units == 1

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            uint8_t  map_len;              // length of map in bytes
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // pad(n)/4
        };
        struct [[gnu::packed]] Encoding {
            Header  header;
        private:
            uint8_t    _unused[24];
        };
        // followed by LISTofCARD8 map pad(n)B
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};

struct SetModifierMapping : public Request {
    struct [[gnu::packed]] Header {
        uint8_t  opcode;                 // 118
        uint8_t  keycodes_per_modifier;  // n keycodes-per-modifier
        uint16_t tl_aligned_units;       // 1+2n
    };
    // followed by LISTofKEYCODE keycodes 8nB

    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Header ) };
    // https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#requests:SetModifierMapping
    //   "number of keycodes in the list must be 8*keycodes-per-modifier"
    static constexpr uint8_t MODIFIER_CT { 8 };

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                // 1
            uint8_t  status;               // 0 Success 1 Busy 2 Failed
            CARD16   sequence_num;         // sequence number
            uint32_t extra_aligned_units;  // 0
        };
        struct [[gnu::packed]] Encoding {
            Header  header;
        private:
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );

        inline static const
        std::vector< std::string_view >& status_names {
            protocol::enum_names::mapping_status };
    };
};

struct GetModifierMapping : public impl::SimpleRequest {
    // Header.opcode == 119
    // Header.tl_aligned_units == 1

    struct Reply : public requests::Reply {
        struct [[gnu::packed]] Header {
            uint8_t  reply;                  // 1
            uint8_t  keycodes_per_modifier;  // keycodes-per-modifier
            CARD16   sequence_num;           // sequence number
            uint32_t extra_aligned_units;    // 2 * keycodes_per_modifier
        };
        struct [[gnu::packed]] Encoding {
            Header  header;
        private:
            uint8_t _unused[24];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
        // followed by LISTofKEYCODE keycodes (8 * keycodes_per_modifier)

        static constexpr uint32_t MODIFIER_CT { 8 };
    };
};

struct NoOperation : public impl::SimpleRequest {
    // Header.opcode == 127
    // Header.tl_aligned_units == 1+n
    // followed by uint8_t unused[4n]
};

}  // namespace requests

}  // namespace protocol


#endif  // PROTOCOL_REQUESTS_HPP
