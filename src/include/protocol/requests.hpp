#ifndef REQUESTS_HPP
#define REQUESTS_HPP


#include <cstdint>
#include <array>

#include "protocol/common_types.hpp"
#include "protocol/enum_names.hpp"


// TBD when parsing replies, we will need to fetch the appropriate struct
//   via the corresponding request, so we may need to keep a map of request
//   opcodes by sequence number & 0000FFFF; which will let us look up
//   an opcode for each seq num, and then a request by that opcode
// TBD map could be a vector, as protocol dicates that on each client connx
//   requests are 1-indexed by the server

// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#request_format

/*
Request Format
Every request contains an 8-bit major opcode and a 16-bit length field expressed
in units of four bytes. Every request consists of four bytes of a header
(containing the major opcode, the length field, and a data byte) followed by
zero or more additional bytes of data. The length field defines the total length
of the request, including the header. The length field in a request must equal
the minimum length required to contain the request. If the specified length is
smaller or larger than the required length, an error is generated. Unused bytes
in a request are not required to be zero. Major opcodes 128 through 255 are
reserved for extensions. Extensions are intended to contain multiple requests,
so extension requests typically have an additional minor opcode encoded in the
second data byte in the request header. However, the placement and
interpretation of this minor opcode and of all other fields in extension
requests are not defined by the core protocol. Every request on a given
connection is implicitly assigned a sequence number, starting with one, that is
used in replies, errors, and events.
*/

// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Requests
// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Requests

// client: single byte begins all messages (?): requests by 2+
// TBD seems like errors and requests are all 32 bytes, and since deciding code is
//   always first byte (derefable without cast,) we could better represent the standard by
//   including the codes in the structs
// !!! some replies exceed 32 bytes

namespace protocol {

namespace requests {

// // TBD make these extern or part of a logging func
// constexpr int REQUEST_CT { 127 };
// // names by opcode
// constexpr std::array< std::string_view, REUQEST_CT + 1 > names {
//     "",                         //   0 (reserved for TBD ??)
//     "CreateWindow",             //   1
//     "ChangeWindowAttributes",   //   2
//     "GetWindowAttributes",      //   3
//     "DestroyWindow",            //   4
//     "DestroySubwindows",        //   5
//     "ChangeSaveSet",            //   6
//     "ReparentWindow",           //   7
//     "MapWindow",                //   8
//     "MapSubwindows",            //   9
//     "UnmapWindow",              //  10
//     "UnmapSubwindows",          //  11
//     "ConfigureWindow",          //  12
//     "CirculateWindow",          //  13
//     "GetGeometry",              //  14
//     "QueryTree",                //  15
//     "InternAtom",               //  16
//     "GetAtomName",              //  17
//     "ChangeProperty",           //  18
//     "DeleteProperty",           //  19
//     "GetProperty",              //  20
//     "ListProperties",           //  21
//     "SetSelectionOwner",        //  22
//     "GetSelectionOwner",        //  23
//     "ConvertSelection",         //  24
//     "SendEvent",                //  25
//     "GrabPointer",              //  26
//     "UngrabPointer",            //  27
//     "GrabButton",               //  28
//     "UngrabButton",             //  29
//     "ChangeActivePointerGrab",  //  30
//     "GrabKeyboard",             //  31
//     "UngrabKeyboard",           //  32
//     "GrabKey",                  //  33
//     "UngrabKey",                //  34
//     "AllowEvents",              //  35
//     "GrabServer",               //  36
//     "UngrabServer",             //  37
//     "QueryPointer",             //  38
//     "GetMotionEvents",          //  39
//     "TranslateCoordinates",     //  40
//     "WarpPointer",              //  41
//     "SetInputFocus",            //  42
//     "GetInputFocus",            //  43
//     "QueryKeymap",              //  44
//     "OpenFont",                 //  45
//     "CloseFont",                //  46
//     "QueryFont",                //  47
//     "QueryTextExtents",         //  48
//     "ListFonts",                //  49
//     "ListFontsWithInfo",        //  50
//     "SetFontPath",              //  51
//     "GetFontPath",              //  52
//     "CreatePixmap",             //  53
//     "FreePixmap",               //  54
//     "CreateGC",                 //  55
//     "ChangeGC",                 //  56
//     "CopyGC",                   //  57
//     "SetDashes",                //  58
//     "SetClipRectangles",        //  59
//     "FreeGC",                   //  60
//     "ClearArea",                //  61
//     "CopyArea",                 //  62
//     "CopyPlane",                //  63
//     "PolyPoint",                //  64
//     "PolyLine",                 //  65
//     "PolySegment",              //  66
//     "PolyRectangle",            //  67
//     "PolyArc",                  //  68
//     "FillPoly",                 //  69
//     "PolyFillRectangle",        //  70
//     "PolyFillArc",              //  71
//     "PutImage",                 //  72
//     "GetImage",                 //  73
//     "PolyText8",                //  74
//     "PolyText16",               //  75
//     "ImageText8",               //  76
//     "ImageText16",              //  77
//     "CreateColormap",           //  78
//     "FreeColormap",             //  79
//     "CopyColormapAndFree",      //  80
//     "InstallColormap",          //  81
//     "UninstallColormap",        //  82
//     "ListInstalledColormaps",   //  83
//     "AllocColor",               //  84
//     "AllocNamedColor",          //  85
//     "AllocColorCells",          //  86
//     "AllocColorPlanes",         //  87
//     "FreeColors",               //  88
//     "StoreColors",              //  89
//     "StoreNamedColor",          //  90
//     "QueryColors",              //  91
//     "LookupColor",              //  92
//     "CreateCursor",             //  93
//     "CreateGlyphCursor",        //  94
//     "FreeCursor",               //  95
//     "RecolorCursor",            //  96
//     "QueryBestSize",            //  97
//     "QueryExtension",           //  98
//     "ListExtensions",           //  99
//     "ChangeKeyboardMapping",    // 100
//     "GetKeyboardMapping",       // 101
//     "ChangeKeyboardControl",    // 102
//     "GetKeyboardControl",       // 103
//     "Bell",                     // 104
//     "ChangePointerControl",     // 105
//     "GetPointerControl",        // 106
//     "SetScreenSaver",           // 107
//     "GetScreenSaver",           // 108
//     "ChangeHosts",              // 109
//     "ListHosts",                // 110
//     "SetAccessControl",         // 111
//     "SetCloseDownMode",         // 112
//     "KillClient",               // 113
//     "RotateProperties",         // 114
//     "ForceScreenSaver",         // 115
//     "SetPointerMapping",        // 116
//     "GetPointerMapping",        // 117
//     "SetModifierMapping",       // 118
//     "GetModifierMapping",       // 119
//     "",                         // 120 (unused) // TBD consider map instead?
//     "",                         // 121 (unused)
//     "",                         // 122 (unused)
//     "",                         // 123 (unused)
//     "",                         // 124 (unused)
//     "",                         // 125 (unused)
//     "",                         // 126 (unused)
//     "NoOperation"               // 127
// };

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

}  // namespace opcodes

// TBD request "header" in protocol refers to 4B { opcode, depth, request_length }

namespace impl {

}  // namespace impl

struct CreateWindow {
    inline static constexpr
    std::string_view name { "CreateWindow" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 1
        CARD8      depth;
        uint16_t   request_length;  // 8+n, request length
        WINDOW     wid;
        WINDOW     parent;
        INT16      x;
        INT16      y;
        CARD16     width;
        CARD16     height;
        CARD16     border_width;  // border-width
        uint16_t   class_; // class // 0 CopyFromParent 1 InputOutput 2 InputOnly
        VISUALID   visual; // 0 CopyFromParent
        uint32_t   value_mask; // 4B BITMASK value-mask (has n bits set to 1)
    };
    // followed by 4n bytes LISTofVALUE value-list
    // TBD !!! the usual method of casting the buffer to a struct ptr will not work here,
    //   as the nature of value masks is to indicate via flags which members will be serialized
    // struct [[gnu::packed]] Value {  // VALUE
    //     PIXMAP            background-pixmap;  // 0 None 1 ParentRelative
    //     CARD32            background-pixel;
    //     PIXMAP            border-pixmap;  // 0 CopyFromParent
    //     CARD32            border-pixel;
    //     BITGRAVITY        bit-gravity;
    //     WINGRAVITY        win-gravity;
    //     uint8_t           backing-store  // 0 NotUseful 1 WhenMapped 2 Always
    //     CARD32            backing-planes;
    //     CARD32            backing-pixel;
    //     BOOL              override-redirect;
    //     BOOL              save-under;
    //     SETofEVENT        event-mask
    //     SETofDEVICEEVENT  do-not-propagate-mask
    //     COLORMAP          colormap;  // 0 CopyFromParent
    //     CURSOR            cursor;  // 0 None
    // };

    inline static const
    std::vector< std::string_view >& class_names {
        protocol::enum_names::window_class };
    inline static const
    std::vector< std::string_view >& visual_names {
        protocol::enum_names::zero_copy_from_parent };
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
};

struct ChangeWindowAttributes {
    inline static constexpr
    std::string_view name { "ChangeWindowAttributes" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 2
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 3+n request length
        WINDOW     window;
        uint32_t   value_mask;      // 4B BITMASK value-mask (has n bits set to 1) // encodings are the same as for CreateWindow
    };
    // followed by 4n bytes LISTofVALUE value-list
    // 4n     LISTofVALUE                    value-list
    // encodings are the same as for CreateWindow

    inline static const
    std::vector< std::string_view >& class_names {
        protocol::enum_names::window_class };
    inline static const
    std::vector< std::string_view >& visual_names {
        protocol::enum_names::zero_copy_from_parent };
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
};

// TBD only inheriting from base to get enums for backing-store and class_
struct GetWindowAttributes {
    inline static constexpr
    std::string_view name { "GetWindowAttributes" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 3
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 3+n request length
        WINDOW     window;
    };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t          _prefix;   // 1 Reply
    public:
        uint8_t          backing_store;  // backing-store  // 0 NotUseful 1 WhenMapped 2 Always
        CARD16           sequence_number;  // sequence number  (request seq num & 0x0000ffff)
        uint32_t         reply_length;  // "3"? really 3B? reply length
        VISUALID         visual;
        uint16_t         class_;  // class, 1 InputOutput 2 InputOnly
        BITGRAVITY       bit_gravity;  // bit-gravity
        WINGRAVITY       win_gravity;  // win-gravity
        CARD32           backing_planes;  // backing-planes
        CARD32           backing_pixel;  // backing-pixel
        BOOL             save_under;  // save-under
        BOOL             map_is_installed;  // map-is-installed
        uint8_t          map_state;  // map-state // 0 Unmapped 1 Unviewable 2 Viewable
        BOOL             override_redirect;  // override-redirect
        COLORMAP         colormap;  // 0 None
        SETofEVENT       all_event_masks;  // all-event-masks
        SETofEVENT       your_event_mask;  // your-event-mask
        SETofDEVICEEVENT do_not_propagate_mask;  // do-not-propagate-mask
    private:
        uint8_t          _unused[2];
    };

    inline static const
    std::vector< std::string_view >& backing_store_names {
        protocol::enum_names::window_attribute_backing_store };
    inline static const
    std::vector< std::string_view >& map_state_names {
        protocol::enum_names::window_attribute_map_state };
};

namespace impl {

struct [[gnu::packed]] _SimpleWindowReqEncoding {
    uint8_t    opcode;
private:
    uint8_t    _unused;
public:
    uint16_t   request_length;
    WINDOW     window;
};

}  // namespace impl

struct DestroyWindow {
    inline static constexpr
    std::string_view name { "DestroyWindow" };

    using Encoding = impl::_SimpleWindowReqEncoding;
    // Encoding::opcode == 4
};

struct DestroySubwindows {
    inline static constexpr
    std::string_view name { "DestroySubwindows" };

    using Encoding = impl::_SimpleWindowReqEncoding;
    // Encoding::opcode == 5
};

struct ChangeSaveSet {
    inline static constexpr
    std::string_view name { "ChangeSaveSet" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;   // 6
        uint8_t    mode;     // 0 Insert 1 Delete
        uint16_t   request_length;
        WINDOW     window;
    };
};

struct ReparentWindow {
    inline static constexpr
    std::string_view name { "ReparentWindow" };

    struct [[gnu::packed]] Encoding {
    private:
        uint8_t _prefix;  // 7
        uint8_t   _unused;
    public:
        uint16_t  request_length;  // "4", really 4B? request length
        WINDOW    window;
        WINDOW    parent;
        INT16     x;
        INT16     y;
    };
};

struct MapWindow {
    inline static constexpr
    std::string_view name { "MapWindow" };

    using Encoding = impl::_SimpleWindowReqEncoding;
    // Encoding::opcode == 8
};

struct MapSubwindows {
    inline static constexpr
    std::string_view name { "MapSubwindows" };

    using Encoding = impl::_SimpleWindowReqEncoding;
    // Encoding::opcode == 9
};

struct UnmapWindow {
    inline static constexpr
    std::string_view name { "UnmapWindow" };

    using Encoding = impl::_SimpleWindowReqEncoding;
    // Encoding::opcode == 10
};

struct UnmapSubwindows {
    inline static constexpr
    std::string_view name { "UnmapSubwindows" };

    using Encoding = impl::_SimpleWindowReqEncoding;
    // Encoding::opcode == 11
};

struct ConfigureWindow {
    inline static constexpr
    std::string_view name { "ConfigureWindow" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 12
    private:
        uint8_t    _unused1;
    public:
        uint16_t   request_length;  // 3+n
        WINDOW     window;
        uint16_t   value_mask;  // 2B BITMASK value-mask (has n bits set to 1)
    private:
        uint8_t   _unused2[2];
    };
    // TBD use of n here implies only one VALUE set in list
// followed by
//     4n     LISTofVALUE                    value-list
    // // VALUE members
    // INT16     x;
    // INT16     y;
    // CARD16    width;
    // CARD16    height;
    // CARD16    border_width;  // border-width
    // WINDOW    sibling;
    // uint8_t   stack_mode;  // stack-mode  // 0 Above 1 Below 2 TopIf 3 BottomIf 4 Opposite

    inline static const
    std::vector< std::string_view >& value_names {
        protocol::enum_names::window_value_mask };
    inline static const
    std::vector< std::string_view >& stack_mode_names {
        protocol::enum_names::window_value_stack_mode };
};

struct CirculateWindow {
    inline static constexpr
    std::string_view name { "CirculateWindow" };

    struct [[gnu::packed]] Encoding {
        uint8_t   opcode;  // 13
        uint8_t   direction;  // 0 RaiseLowest 1 LowerHighest
        uint16_t  request_length;  // 2
        WINDOW    window;
    };

    inline static const
    std::vector< std::string_view >& direction_names {
        protocol::enum_names::circulate_direction };
};

struct GetGeometry {
    inline static constexpr
    std::string_view name { "GetGeometry" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 14
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 2
        DRAWABLE   drawable;
    };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t _prefix;  // 1 Reply
    public:
        CARD8    depth;
        CARD16   sequence_number;  // sequence number
        uint32_t reply_length;  // 0 reply length
        WINDOW   root;
        INT16    x;
        INT16    y;
        CARD16   width;
        CARD16   height;
        CARD16   border_width;  // border-width
    private:
        uint8_t  _unused[10];
    };
};

struct QueryTree {
    inline static constexpr
    std::string_view name { "QueryTree" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 15
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 2
        WINDOW     window;
    };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t    _prefix;  // 1 Reply
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        uint32_t   reply_length;   // "n" in protocol, but conflicts with later "n" reply length
        WINDOW     root;
        WINDOW     parent;  // 0 None
        uint16_t   n; // number of WINDOWs in children
    private:
        uint8_t    _unused2[14];
    };
    // followed by 4n LISTofWINDOW  children
};

struct InternAtom {
    inline static constexpr
    std::string_view name { "InternAtom" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 16
        BOOL      only_if_exists;  // only-if-exists
        uint16_t  request_length;  // 2+(n+p)/4 request length
        uint16_t  n;  // length of name
    private:
        uint8_t  _unused[2];
    };
    // followed by pad(n) STRING8 name

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t    _prefix;  // 1 Reply
        uint8_t   _unused1;
    public:
        CARD16    sequence_number;  // sequence number
        uint32_t  reply_length;  // 0 reply length
        ATOM      atom; // 0 None
    private:
        uint8_t  _unused2[20];
    };

    inline static const
    std::vector< std::string_view >& atom_names {
        protocol::enum_names::zero_none };
};

struct GetAtomName {
    inline static constexpr
    std::string_view name { "GetAtomName" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 17
    private:
        uint8_t   _unused1;
    public:
        uint16_t  request_length;  // 2 request length
        ATOM      atom;
    };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t   _prefix;  // 1 Reply
        uint8_t   _unused1;
    public:
        CARD16    sequence_number;  // sequence number
        uint32_t  reply_length;  // (n+p)/4 reply length
        uint16_t  n;  // length of name
    private:
        uint8_t  _unused2[22];
    };
    // followed by STRING8 name of pad(n) bytes
};

struct ChangeProperty {
    inline static constexpr
    std::string_view name { "ChangeProperty" };

    struct [[gnu::packed]] Encoding {
        uint8_t   opcode;  // 18
        uint8_t   mode;  // 0 Replace 1 Prepend 2 Append
        uint16_t  request_length;  // 6+(n+p)/4 request length
        // pad(n) == (request_length - 6) * 4
        WINDOW    window;
        ATOM      property;
        ATOM      type;
        CARD8     format;
    private:
        uint8_t   _unused[3];
    public:
        CARD32    fmt_unit_ct;  // unnnamed in protocol // length of data in format units
    };
    // format unit sz == pad(n) / fmt_unit_ct
    /*
      (= n for format = 8)
      (= n/2 for format = 16)
      (= n/4 for format = 32)
    */
    // followed by LISTofBYTE data (uint8_t[pad(n)])
    //                    (n is a multiple of 2 for format = 16)
    //                    (n is a multiple of 4 for format = 32)

    inline static const
    std::vector< std::string_view >& mode_names {
        protocol::enum_names::change_property_mode };
};

struct DeleteProperty {
    inline static constexpr
    std::string_view name { "DeleteProperty" };

    struct [[gnu::packed]] Encoding {
        uint8_t   opcode;  // 19
    private:
        uint8_t   _unused;
    public:
        uint16_t  request_length;  // 3 request length
        WINDOW    window;
        ATOM      property;
    };
};

struct GetProperty {
    inline static constexpr
    std::string_view name { "GetProperty" };

    struct [[gnu::packed]] Encoding {
        uint8_t   opcode;  // 20
        BOOL      delete_;  // delete
        uint16_t  request_length;  // 6 request length
        WINDOW    window;
        ATOM      property;
        ATOM      type;  // 0 AnyPropertyType
        CARD32    long_offset;  // long-offset
        CARD32    long_length;  // long-length
    };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t  _prefix;  // 1 Reply
    public:
        CARD8    format;
        CARD16   sequence_number;  // sequence number
        uint32_t reply_length;  // (n+p)/4 reply length
        // pad(n) == (request_length) * 4
        ATOM     type;  // 0 None
        CARD32   bytes_after;  // bytes-after
        CARD32   fmt_unit_ct;  // unnnamed in protocol // length of value in format units
        // format unit sz == pad(n) / fmt_unit_ct
        // (= 0 for format = 0) (= n for format = 8)
        // (= n/2 for format = 16) (= n/4 for format = 32)
    private:
        uint8_t  _unused[12];
    };
    // followed by pad(n)B LISTofBYTE value
    /*
      (n is zero for format = 0)
      (n is a multiple of 2 for format = 16)
      (n is a multiple of 4 for format = 32)
    */

    inline static const
    std::vector< std::string_view >& type_names_request {
        protocol::enum_names::property_atom };
    inline static const
    std::vector< std::string_view >& type_names_reply {
        protocol::enum_names::zero_none };

};

struct ListProperties {
    inline static constexpr
    std::string_view name { "ListProperties" };

    struct [[gnu::packed]] Encoding {
        uint8_t   opcode;  // 21
    private:
        uint8_t   _unused;
    public:
        uint16_t  request_length;  // 2 request length
        WINDOW    window;
    };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t  _prefix;  // 1 Reply
        uint8_t  _unused1;
    public:
        CARD16   sequence_number;  // sequence number
        uint32_t reply_length;  // n reply length
        uint16_t n;  // number of ATOMs in atoms
    private:
        uint8_t  _unused2[22];
    };
    // followed by 4nB LISTofATOM atoms
};

struct SetSelectionOwner {
    inline static constexpr
    std::string_view name { "SetSelectionOwner" };

    struct [[gnu::packed]] Encoding {
        uint8_t   opcode;  // 22
    private:
        uint8_t   _unused;
    public:
        uint16_t  request_length;  // 2 request length
        WINDOW    owner;  // 0 None
        ATOM      selection;
        TIMESTAMP time;  // 0 CurrentTime
    };

    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
};

struct GetSelectionOwner {
    inline static constexpr
    std::string_view name { "GetSelectionOwner" };

    struct [[gnu::packed]] Encoding {
        uint8_t   opcode;  // 23
    private:
        uint8_t   _unused;
    public:
        uint16_t  request_length;  // 2 request length
        ATOM      selection;
    };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t  _prefix;  // 1 Reply
        uint8_t  _unused1;
    public:
        CARD16   sequence_number;  // sequence number
        uint32_t reply_length;  // 0 reply length
        WINDOW   owner;  // 0 None
    private:
        uint8_t  _unused2[20];
    };

    inline static const
    std::vector< std::string_view >& owner_names {
        protocol::enum_names::zero_none };
};

struct ConvertSelection {
    inline static constexpr
    std::string_view name { "ConvertSelection" };

    struct [[gnu::packed]] Encoding {
        uint8_t   opcode;  // 24
    private:
        uint8_t   _unused;
    public:
        uint16_t  request_length;  // 6 request length
        WINDOW    requestor;
        ATOM      selection;
        ATOM      target;
        ATOM      property;  // 0 None
        TIMESTAMP time;   // 0 CurrentTime
    };

    inline static const
    std::vector< std::string_view >& property_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
};

struct SendEvent {
    inline static constexpr
    std::string_view name { "SendEvent" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 25
        BOOL       propagate;
        uint16_t   request_length;  // 11 request length
        WINDOW     destination;  // 0 PointerWindow 1 InputFocus
        SETofEVENT event_mask;  // event-mask
        // 32  event
        // TBD presumably events are parsed like a LISTofVALUE with value-mask
    };

    inline static const
    std::vector< std::string_view >& property_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
};

struct GrabPointer {
    inline static constexpr
    std::string_view name { "GrabPointer" };

    struct [[gnu::packed]] Encoding {
        uint8_t           opcode;  // 26
        BOOL              owner_events;  // owner-events
        uint16_t          request_length;  // 6 request length
        WINDOW            grab_window;  // grab-window
        SETofPOINTEREVENT event_mask;  // 2B event-mask
        uint8_t           pointer_mode;  // pointer-mode 0 Synchronous 1 Asynchronous
        uint8_t           keyboard_mode;  // keyboard-mode 0 Synchronous 1 Asynchronous
        WINDOW            confine_to;  // confine-to 0 None
        CURSOR            cursor; // 0 None
        TIMESTAMP         time;  // 0 CurrentTime
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

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t  _prefix;  // 1 Reply
    public:
        uint8_t  status;  // 0 Success 1 AlreadyGrabbed 2 InvalidTime 3 NotViewable 4 Frozen
        CARD16   sequence_number;  // sequence number
        uint32_t reply_length;  // 0 reply length
    private:
        uint8_t  _unused[24];
    };

    inline static const
    std::vector< std::string_view >& status_names {
        protocol::enum_names::grab_status };
};

struct UngrabPointer {
    inline static constexpr
    std::string_view name { "UngrabPointer" };

    struct [[gnu::packed]] Encoding {
        uint8_t           opcode;  // 27
    private:
        uint8_t           unused;
    public:
        uint16_t          request_length;  // 2 request length
        TIMESTAMP         time;
    };

    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
};

struct GrabButton {
    inline static constexpr
    std::string_view name { "GrabButton" };

    struct [[gnu::packed]] Encoding {
        uint8_t           opcode;  // 28
        BOOL              owner_events;  // owner-events
        uint16_t          request_length;  // 6 request length
        WINDOW            grab_window;  // grab-window
        SETofPOINTEREVENT event_mask;  // event-mask
        uint8_t           pointer_mode;  // pointer-mode 0 Synchronous 1 Asynchronous
        uint8_t           keyboard_mode;  // keyboard-mode 0 Synchronous 1 Asynchronous
        WINDOW            confine_to;  // confine-to 0 None
        CURSOR            cursor;  // 0 None
        BUTTON            button;  // 0 AnyButton
    private:
        uint8_t           unused;
    public:
        SETofKEYMASK      modifiers;  // 2B #x8000 AnyModifier
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
};

struct UngrabButton {
    inline static constexpr
    std::string_view name { "UngrabButton" };

    struct [[gnu::packed]] Encoding {
        uint8_t           opcode;  // 29
        BUTTON            button;  // 1B 0 AnyButton
        uint16_t          request_length;  // 3 request length
        WINDOW            grab_window;  // grab-window
        SETofKEYMASK      modifiers;  // 2B #x8000 AnyModifier
    private:
        uint8_t           _unused[2];
    };

    inline static const
    std::vector< std::string_view >& button_names {
        protocol::enum_names::button };
};

struct ChangeActivePointerGrab {
    inline static constexpr
    std::string_view name { "ChangeActivePointerGrab" };

    struct [[gnu::packed]] Encoding {
        uint8_t           opcode;  // 30
    private:
        uint8_t           _unused1;
    public:
        uint16_t          request_length;  // 4 request length
        CURSOR            cursor;  // 0 None
        TIMESTAMP         time;    // 0 CurrentTime
        SETofPOINTEREVENT event_mask;  // 2B event-mask
    private:
        uint8_t           _unused2[2];
    };

    inline static const
    std::vector< std::string_view >& cursor_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
};

struct GrabKeyboard {
    inline static constexpr
    std::string_view name { "GrabKeyboard" };

    struct [[gnu::packed]] Encoding {
        uint8_t           opcode;  // 31
        BOOL              owner_events;  // owner-events
        uint16_t          request_length;  // 4 request length
        WINDOW            grab_window;  // grab-window
        TIMESTAMP         time;  // 0 CurrentTime
        uint8_t           pointer_mode;  // pointer-mode 0 Synchronous 1 Asynchronous
        uint8_t           keyboard_mode;  // keyboard-mode 0 Synchronous 1 Asynchronous
    private:
        uint8_t           _unused[2];
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

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t  _prefix;  // 1 Reply
    public:
        uint8_t  status;  // 0 Success 1 AlreadyGrabbed 2 InvalidTime 3 NotViewable 4 Frozen
        CARD16   sequence_number;  // sequence number
        uint32_t reply_length;  // 0 reply length
    private:
        uint8_t  _unused[24];
    };

    inline static const
    std::vector< std::string_view >& status_names {
        protocol::enum_names::grab_status };
};

struct UngrabKeyboard {
    inline static constexpr
    std::string_view name { "UngrabKeyboard" };

    struct [[gnu::packed]] Encoding {
        uint8_t           opcode;  // 32
    private:
        uint8_t           _unused;  // 1B
    public:
        uint16_t          request_length;  // 2 request length
        TIMESTAMP         time;  // 0 CurrentTime
    };

    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
};

struct GrabKey {
    inline static constexpr
    std::string_view name { "GrabKey" };

    struct [[gnu::packed]] Encoding {
        uint8_t           opcode;  // 33
        BOOL              owner_events;  // owner-events
        uint16_t          request_length;  // 4 request length
        WINDOW            grab_window;  // grab-window
        SETofKEYMASK      modifiers;  // 2B #x8000 AnyModifier
        KEYCODE           key;  // 0 AnyKey
        uint8_t           pointer_mode;  // pointer-mode 0 Synchronous 1 Asynchronous
        uint8_t           keyboard_mode;  // keyboard-mode 0 Synchronous 1 Asynchronous
    private:
        uint8_t           _unused[3];
    };

    // TBD
    // inline static const
    // std::vector< std::string_view >& modifiers_names {
    //     protocol::enum_names:: };
    inline static const
    std::vector< std::string_view >& key_names {
        protocol::enum_names::key };
    inline static const
    std::vector< std::string_view >& pointer_mode_names {
        protocol::enum_names::input_mode };
    inline static const
    std::vector< std::string_view >& keyboard_mode_names {
        protocol::enum_names::input_mode };
};

struct UngrabKey {
    inline static constexpr
    std::string_view name { "UngrabKey" };

    struct [[gnu::packed]] Encoding {
        uint8_t           opcode;  // 34
        KEYCODE           key;  // 0 AnyKey
        uint16_t          request_length;  // 3 request length
        WINDOW            grab_window;  // grab-window
        SETofKEYMASK      modifiers;  // 2B #x8000 AnyModifier
    private:
        uint8_t           _unused[2];
    };

    inline static const
    std::vector< std::string_view >& key_names {
        protocol::enum_names::key };
    // TBD
    // inline static const
    // std::vector< std::string_view >& modifiers_names {
    //     protocol::enum_names:: };
};

struct AllowEvents {
    inline static constexpr
    std::string_view name { "AllowEvents" };

    struct [[gnu::packed]] Encoding {
        uint8_t           opcode;  // 35
        uint8_t           mode;  // 0 AsyncPointer 1 SyncPointer 2 ReplayPointer 3 AsyncKeyboard 4 SyncKeyboard 5 ReplayKeyboard 6 AsyncBoth 7 SyncBoth
        uint16_t          request_length;  // 2 request length
        TIMESTAMP         time;  // 0 CurrentTime
    };

    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
};

struct GrabServer {
    inline static constexpr
    std::string_view name { "GrabServer" };

    struct [[gnu::packed]] Encoding {
        uint8_t           opcode;  // 36
    private:
        uint8_t           _unused;
    public:
        uint16_t          request_length;  // 1 request length
    };
};

struct UngrabServer {
    inline static constexpr
    std::string_view name { "UngrabServer" };

    struct [[gnu::packed]] Encoding {
        uint8_t           opcode;  // 37
    private:
        uint8_t           _unused;
    public:
        uint16_t          request_length;  // 1 request length
    };
};

struct QueryPointer {
    inline static constexpr
    std::string_view name { "QueryPointer" };

    struct [[gnu::packed]] Encoding {
        uint8_t          opcode;  // 38
    private:
        uint8_t          _unused;
    public:
        uint16_t         request_length;  // 2 request length
        WINDOW           window;
    };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t          _prefix;  // 1 Reply
    public:
        BOOL             same_screen;  // same-screen
        CARD16           sequence_number;  // sequence number
        uint32_t         reply_length;  // 0 reply length
        WINDOW           root;
        WINDOW           child;  // 0 None
        INT16            root_x;  // root-x
        INT16            root_y;  // root-y
        INT16            win_x;  // win-x
        INT16            win_y;  // win-y
        SETofKEYBUTMASK  mask;
    private:
        uint8_t          _unused[6];
    };

    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
};

struct GetMotionEvents {
    inline static constexpr
    std::string_view name { "GetMotionEvents" };

    struct [[gnu::packed]] Encoding {
        uint8_t          opcode;  // 39
    private:
        uint8_t          _unused;
    public:
        uint16_t         request_length;  // 4 request length
        WINDOW           window;
        TIMESTAMP        start;  // 0 CurrentTime
        TIMESTAMP        stop;  // 0 CurrentTime
    };

    inline static const
    std::vector< std::string_view >& start_names {
        protocol::enum_names::time };
    inline static const
    std::vector< std::string_view >& stop_names {
        protocol::enum_names::time };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t          _prefix;  // 1 Reply
        uint8_t          _unused1;
    public:
        CARD16           sequence_number;  // sequence number
        uint32_t         reply_length;  // 2n reply length
        uint32_t         n;  // number of TIMECOORDs in events
    private:
        uint8_t          _unused[20];
    };
    // followed by 8nB LISTofTIMECOORD events

    struct [[gnu::packed]] TIMECOORD {
        TIMESTAMP  time;
        INT16      x;
        INT16      y;
    };
};

struct TranslateCoordinates {
    inline static constexpr
    std::string_view name { "TranslateCoordinates" };

    struct [[gnu::packed]] Encoding {
        uint8_t          opcode;  // 40
    private:
        uint8_t          _unused;
    public:
        uint16_t         request_length;  // 4 request length
        WINDOW           src_window;  // src-window
        WINDOW           dst_window;  // dst-window
        INT16            src_x;  // src-x
        INT16            src_y;  // src-y
    };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t          _prefix;  // 1 Reply
    public:
        BOOL             same_screen;  // same-screen
        CARD16           sequence_number;  // sequence number
        uint32_t         reply_length;  // 0 reply length
        WINDOW           child;  // 0 None
        INT16            dst_x;  // dst-x
        INT16            dst_y;  // dst-y
    private:
        uint8_t          _unused[16];
    };

    inline static const
    std::vector< std::string_view >& child_names {
        protocol::enum_names::zero_none };
};

struct WarpPointer {
    inline static constexpr
    std::string_view name { "WarpPointer" };

    struct [[gnu::packed]] Encoding {
        uint8_t          opcode;  // 41
    private:
        uint8_t          _unused;
    public:
        uint16_t         request_length;  // 6 request length
        WINDOW           src_window;  // src-window 0 None
        WINDOW           dst_window;  // dst-window 0 None
        INT16            src_x;  // src-x
        INT16            src_y;  // src-y
        CARD16           src_width;  // src-width
        CARD16           src_height;  // src-height
        INT16            dst_x;  // dst-x
        INT16            dst_y;  // dst-y
    };

    inline static const
    std::vector< std::string_view >& src_window_names {
        protocol::enum_names::zero_none };
    inline static const
    std::vector< std::string_view >& dst_window_names {
        protocol::enum_names::zero_none };
};

struct SetInputFocus {
    inline static constexpr
    std::string_view name { "SetInputFocus" };

    struct [[gnu::packed]] Encoding {
        uint8_t          opcode;  // 42
        uint8_t          revert_to;  // revert-to 0 None 1 PointerRoot 2 Parent
        uint16_t         request_length;  // 3 request length
        WINDOW           focus; // 0 None 1 PointerRoot
        TIMESTAMP        time; // 0 CurrentTime
    };

    inline static const
    std::vector< std::string_view >& revert_to_names {
        protocol::enum_names::input_focus };
    inline static const
    std::vector< std::string_view >& focus_names {
        protocol::enum_names::input_focus };  // up to 1
    inline static const
    std::vector< std::string_view >& time_names {
        protocol::enum_names::time };
};

struct GetInputFocus {
    inline static constexpr
    std::string_view name { "GetInputFocus" };

    struct [[gnu::packed]] Encoding {
        uint8_t          opcode;  // 43
    private:
        uint8_t          _unused;
    public:
        uint16_t         request_length;  // 1 request length
    };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t          _prefix;  // 1 Reply
    public:
        uint8_t          revert_to;  // revert-to 0 None 1 PointerRoot 2 Parent
        CARD16           sequence_number;  // sequence number
        uint32_t         reply_length;  // 0 reply length
        WINDOW           focus;  // 0 None 1 PointerRoot
    private:
        uint8_t          _unused[20];
    };

    inline static const
    std::vector< std::string_view >& revert_to_names {
        protocol::enum_names::input_focus };
    inline static const
    std::vector< std::string_view >& focus_names {
        protocol::enum_names::input_focus };  // up to 1
};

struct QueryKeymap {
    inline static constexpr
    std::string_view name { "QueryKeymap" };

    struct [[gnu::packed]] Encoding {
        uint8_t          opcode;  // 44
    private:
        uint8_t          _unused;
    public:
        uint16_t         request_length;  // 1 request length
    };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t          _prefix;  // 1 Reply
        uint8_t          _unused;
    public:
        CARD16           sequence_number;  // sequence number
        uint32_t         reply_length;  // 2 reply length
        CARD8            keys[32];  // LISTofCARD8 32B bit-vector
    };
};

struct OpenFont {
    inline static constexpr
    std::string_view name { "OpenFont" };

    struct [[gnu::packed]] Encoding {
        uint8_t          opcode;  // 45
    private:
        uint8_t          _unused1;
    public:
        uint16_t         request_length;  // 3+pad(n)/4 request length
        FONT             fid;
        uint16_t         n;  // length of name
    private:
        uint8_t          _unused2[2];
    };
    // followed by STRING8 name of pad(n) bytes
};

struct CloseFont {
    inline static constexpr
    std::string_view name { "CloseFont" };

    struct [[gnu::packed]] Encoding {
        uint8_t          opcode;  // 46
    private:
        uint8_t          _unused;
    public:
        uint16_t         request_length;  // 2 request length
        FONT             font;
    };
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

struct QueryFont {
    inline static constexpr
    std::string_view name { "QueryFont" };

    struct [[gnu::packed]] Encoding {
        uint8_t          opcode;  // 47
    private:
        uint8_t          _unused;
    public:
        uint16_t         request_length;  // 2 request length
        FONTABLE         font;
    };

    using FONTPROP = impl::FONTPROP;
    using CHARINFO = impl::CHARINFO;

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t          _prefix;  // 1 Reply
        uint8_t          _unused1;
    public:
        CARD16           sequence_number;  // sequence number
        uint32_t         reply_length;  // 7+2n+3m reply length
        CHARINFO         min_bounds;  // 12 min-bounds
    private:
        uint8_t          _unused2[4];
    public:
        CHARINFO         max_bounds;  // 12 max-bounds
    private:
        uint8_t          _unused3[4];
    public:
        CARD16           min_char_or_byte2;  // min-char-or-byte2
        CARD16           max_char_or_byte2;  // max-char-or-byte2
        CARD16           default_char;  // default-char
        uint16_t         n;  // number of FONTPROPs in properties
        uint8_t          draw_direction;  // draw_direction 0 LeftToRight 1 RightToLeft
        CARD8            min_byte1;  // min-byte1
        CARD8            max_byte1;  // max-byte1
        BOOL             all_chars_exist;  // all-chars-exist
        INT16            font_ascent;  // font-ascent
        INT16            font_descent;  // font-descent
        uint32_t         m;  //  number of CHARINFOs in char-infos
    };
    // followed by 8nB LISTofFONTPROP properties n FONTPROP
    // followed by 12mB LISTofCHARINFO char-infos m CHARINFO

    inline static const
    std::vector< std::string_view >& draw_direction_names {
        protocol::enum_names::draw_direction };
};

struct QueryTextExtents {
    inline static constexpr
    std::string_view name { "QueryTextExtents" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 48
        BOOL       odd_length;  // odd length, True if p = 2
        uint16_t   request_length;  // request length 2+(2n+p)/4
        FONTABLE   font;
    };
    // followed by pad(2n)B STRING16 string

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t    _prefix;  // 1 Reply
    public:
        uint8_t    draw_direction;  // draw-direction 0 LeftToRight 1 RightToLeft
        CARD16     sequence_number;  // sequence number
        uint32_t   reply_length;  // 0 reply length
        INT16      font_ascent;  // font-ascent
        INT16      font_descent;  // font-descent
        INT16      overall_ascent;  // overall-ascent
        INT16      overall_descent;  // overall-descent
        INT32      overall_width;  // overall-width
        INT32      overall_left;  // overall-left
        INT32      overall_right;  // overall-right
    private:
        uint8_t    _unused[4];
    };
    // followed by 8nB LISTofFONTPROP properties n FONTPROP
    // followed by 12mB LISTofCHARINFO char-infos m CHARINFO

    inline static const
    std::vector< std::string_view >& draw_direction_names {
        protocol::enum_names::draw_direction };
};

struct ListFonts {
    inline static constexpr
    std::string_view name { "ListFonts" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 49
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // request length 2+(2n+p)/4
        CARD16     max_names;  // max-names
        uint16_t   n;  // length of pattern
    };
    // followed by pad(n)B STRING8 pattern

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t    _prefix;  // 1 Reply
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        uint32_t   reply_length;  // (n+p)/4 reply length
        CARD16     name_ct;  // unnamed in protocol, number of STRs in names
    private:
        uint8_t    _unused2[22];
    };
    // TBD LISTofSTR type only named in protocol encoding section - it is
    //   LISTofSTRING8 in request description
    // TBD how do we know the length of the individual strings?
    // followed by pad(n)B LISTofSTR names
};

struct ListFontsWithInfo {
    inline static constexpr
    std::string_view name { "ListFontsWithInfo" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 50
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // request length 2+(n+p)/4
        CARD16     max_names;  // max-names
        uint16_t   n;  // length of pattern
    };
    // followed by pad(n)B STRING8 pattern

    using FONTPROP = impl::FONTPROP;
    using CHARINFO = impl::CHARINFO;

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t    _prefix;  // 1 Reply
    public:
        uint8_t    n;  // length of name in bytes
        CARD16     sequence_number;  // sequence number
        uint32_t   reply_length;  // 7+2m+(n+p)/4 reply length
        CHARINFO   min_bounds;  // min-bounds
    private:
        uint8_t    _unused1[4];
    public:
        CHARINFO   max_bounds;  // max-bounds
    private:
        uint8_t    _unused2[4];
    public:
        CARD16     min_char_or_byte2;  // min-char-or-byte2
        CARD16     max_char_or_byte2;  // max-char-or-byte2
        CARD16     default_char;  // default-char
        uint16_t   m;  // number of FONTPROPs in properties
        uint8_t    draw_direction;  // draw-direction 0 LeftToRight 1 RightToLeft
        CARD8      min_byte1;  // min-byte1
        CARD8      max_byte1;  // max-byte1
        BOOL       all_chars_exist;  // all-chars-exist
        INT16      font_ascent;  // font-ascent
        INT16      font_descent;  // font-descent
        CARD32     replies_hint;  // replies-hint
    };
    // followed by 8mB LISTofFONTPROP properties m FONTPROP
    // followed by pad(n)B STRING8 name

    inline static const
    std::vector< std::string_view >& draw_direction_names {
        protocol::enum_names::draw_direction };

    // TBD may not need second struct, can differentiate by testing
    //   ReplyEncoding.n == 0
    struct [[gnu::packed]] FinalReplyEncoding {
    private:
        uint8_t    _prefix;  // 1 Reply
    public:
        uint8_t    last_reply;  // 0 last-reply indicator
        CARD16     sequence_number;  // sequence number
        uint32_t   reply_length;  // 7 reply length
    private:
        uint8_t    _unused[52];
    };
};

struct SetFontPath {
    inline static constexpr
    std::string_view name { "SetFontPath" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 51
    private:
        uint8_t    _unused1;
    public:
        uint16_t   request_length;  // request length 2+pad(n)/4
        CARD16     str_ct;   // unnnamed in protocol; number of STRs in path
    private:
        uint8_t    _unused2[2];
    };
    // followed by pad(n)B LISTofSTR path
    // TBD LISTofSTR type only named in protocol encoding section - it is
    //   LISTofSTRING8 in request description
    // TBD how do we know the length of the individual strings?
};

struct GetFontPath {
    inline static constexpr
    std::string_view name { "GetFontPath" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 52
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_list;  // 1 request list TBD why not typical request length?
    };

    struct [[gnu::packed]] ReplyEncoding {
    private:
        uint8_t    _prefix;  // 1 Reply
        uint8_t    _unused1;
    public:
        CARD16     sequence_number;  // sequence number
        uint32_t   reply_length;  // (n+p)/4 reply length
        CARD16     str_ct;  // unnamed in protocol; number of STRs in path
    private:
        uint8_t    _unused2[22];
    };
    // followed by pad(n)B LISTofSTR path
    // TBD LISTofSTR type only named in protocol encoding section - it is
    //   LISTofSTRING8 in request description
    // TBD how do we know the length of the individual strings?
};

struct CreatePixmap {
    inline static constexpr
    std::string_view name { "CreatePixmap" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 53
        CARD8      depth;
        uint16_t   request_legnth;  // 4 request length?
        PIXMAP     pid;
        DRAWABLE   drawable;
        CARD16     width;
        CARD16     height;
    };
};

struct FreePixmap {
    inline static constexpr
    std::string_view name { "FreePixmap" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 54
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_legnth;  // 2 request length?
        PIXMAP     pixmap;
    };
};

struct CreateGC {
    inline static constexpr
    std::string_view name { "CreateGC" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 55
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 4+n request length
        GCONTEXT   cid;
        DRAWABLE   drawable;
        uint32_t   value_mask;  // 4B BITMASK value-mask (has n bits set to 1)
    };
    // followed by 4nB LISTofVALUE value-list

    inline static const
    std::vector< std::string_view >& value_names {
        protocol::enum_names::gc_value_mask };
    /*
  VALUEs
     uint8_t     function;  // 0 Clear 1 And 2 AndReverse 3 Copy 4 AndInverted 5 NoOp 6 Xor 7 Or 8 Nor 9 Equiv 10 Invert 11 OrReverse 12 CopyInverted 13 OrInverted 14 Nand 15 Set
     CARD32      plane_mask;  // plane-mask
     CARD32      foreground;
     CARD32      background;
     CARD16      line_width;  // line-width
     uint8_t     line_style;  // line-style 0 Solid 1 OnOffDash 2 DoubleDash
     uint8_t     cap_style;  // cap-style 0 NotLast 1 Butt 2 Round 3 Projecting
     uint8_t     join_style;  // join-style 0 Miter 1 Round 2 Bevel
     uint8_t     fill_style;  // fill-style 0 Solid 1 Tiled 2 Stippled 3 OpaqueStippled
     uint8_t     fill_rule;  // fill-rule 0 EvenOdd 1 Winding
     PIXMAP      tile;
     PIXMAP      stipple;
     INT16       tile_stipple_x_origin;  // tile-stipple-x-origin
     INT16       tile_stipple_y_origin;  // tile-stipple-y-origin
     FONT        font;
     uint8_t     subwindow_mode;  // subwindow-mode 0 ClipByChildren 1 IncludeInferiors
     BOOL        graphics_exposures;  // graphics-exposures
     INT16       clip_x_origin;  // clip-x-origin
     INT16       clip_y_origin;  // clip-y-origin
     PIXMAP      clip_mask;  // clip-mask 0 None
     CARD16      dash_offset;  // dash-offset
     CARD8       dashes;  // dashes
     uint8_t     arc-mode;  // arc_mode 0 Chord 1 PieSlice
    */
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

struct ChangeGC {
    inline static constexpr
    std::string_view name { "ChangeGC" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 56
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 3+n request length
        GCONTEXT   gc;
        uint32_t   value_mask;  // 4B BITMASK value-mask (has n bits set to 1)
    };
    // followed by 4nB LISTofVALUE value-list

    inline static const
    std::vector< std::string_view >& value_names {
        protocol::enum_names::gc_value_mask };
    /*
  VALUEs
     uint8_t     function;  // 0 Clear 1 And 2 AndReverse 3 Copy 4 AndInverted 5 NoOp 6 Xor 7 Or 8 Nor 9 Equiv 10 Invert 11 OrReverse 12 CopyInverted 13 OrInverted 14 Nand 15 Set
     CARD32      plane_mask;  // plane-mask
     CARD32      foreground;
     CARD32      background;
     CARD16      line_width;  // line-width
     uint8_t     line_style;  // line-style 0 Solid 1 OnOffDash 2 DoubleDash
     uint8_t     cap_style;  // cap-style 0 NotLast 1 Butt 2 Round 3 Projecting
     uint8_t     join_style;  // join-style 0 Miter 1 Round 2 Bevel
     uint8_t     fill_style;  // fill-style 0 Solid 1 Tiled 2 Stippled 3 OpaqueStippled
     uint8_t     fill_rule;  // fill-rule 0 EvenOdd 1 Winding
     PIXMAP      tile;
     PIXMAP      stipple;
     INT16       tile_stipple_x_origin;  // tile-stipple-x-origin
     INT16       tile_stipple_y_origin;  // tile-stipple-y-origin
     FONT        font;
     uint8_t     subwindow_mode;  // subwindow-mode 0 ClipByChildren 1 IncludeInferiors
     BOOL        graphics_exposures;  // graphics-exposures
     INT16       clip_x_origin;  // clip-x-origin
     INT16       clip_y_origin;  // clip-y-origin
     PIXMAP      clip_mask;  // clip-mask 0 None
     CARD16      dash_offset;  // dash-offset
     CARD8       dashes;  // dashes
     uint8_t     arc-mode;  // arc_mode 0 Chord 1 PieSlice
    */
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

struct CopyGC {
    inline static constexpr
    std::string_view name { "CopyGC" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 57
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 4 request length
        GCONTEXT   src_gc;  // src-gc
        GCONTEXT   dst_gc;  // dst-gc
        uint32_t   value_mask;  // 4B BITMASK value-mask (has n bits set to 1)
    };

    inline static const
    std::vector< std::string_view >& value_names {
        protocol::enum_names::gc_value_mask };
};

struct SetDashes {
    inline static constexpr
    std::string_view name { "SetDashes" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 58
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 3+(n+p)/4 request length
        GCONTEXT   gc;
        CARD16     dash_offset;  // dash-offset
        uint16_t   n;  // length of dashes
    };
    // followed by pad(n)B LISTofCARD8 dashes
};

struct SetClipRectangles {
    inline static constexpr
    std::string_view name { "SetClipRectangles" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 59
        uint8_t    ordering;  // 0 UnSorted 1 YSorted 2 YXSorted 3 YXBanded
        uint16_t   request_length;  // 3+2n request length
        GCONTEXT   gc;
        INT16      clip_x_origin;  // clip-x-origin
        INT16      clip_y_origin;  // clip-y-origin
    };
    // followed by 8nB LISTofRECTANGLE rectangles n RECTANGLE

    inline static const
    std::vector< std::string_view >& ordering_names {
        protocol::enum_names::clip_rect_ordering };
};

struct FreeGC {
    inline static constexpr
    std::string_view name { "FreeGC" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 60
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 2 request length
        GCONTEXT   gc;
    };
};

struct ClearArea {
    inline static constexpr
    std::string_view name { "ClearArea" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 61
        uint16_t   request_length;  // 4 request length
        WINDOW     window;
        INT16      x;
        INT16      y;
        CARD16     width;
        CARD16     height;
    };
};

struct CopyArea {
    inline static constexpr
    std::string_view name { "CopyArea" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 62
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 7 request length
        DRAWABLE   src_drawable;  // src-drawable
        DRAWABLE   dst_drawable;  // dst-drawable
        GCONTEXT   gc;  // gc
        INT16      src_x;  // src-x
        INT16      src_y;  // src-y
        INT16      dst_x;  // dst-x
        INT16      dst_y;  // dst-y
        CARD16     width;
        CARD16     height;
    };
};

struct CopyPlane {
    inline static constexpr
    std::string_view name { "CopyPlane" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 63
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 8 request length
        DRAWABLE   src_drawable;  // src-drawable
        DRAWABLE   dst_drawable;  // dst-drawable
        GCONTEXT   gc;
        INT16      src_x;  // src-x
        INT16      src_y;  // src-y
        INT16      dst_x;  // dst-x
        INT16      dst_y;  // dst-y
        CARD16     width;
        CARD16     height;
        CARD32     bit_plane;  // bit-plane
    };
};

struct PolyPoint {
    inline static constexpr
    std::string_view name { "PolyPoint" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 64
        uint8_t    coordinate_mode;  // coordinate-mode 0 Origin 1 Previous
        uint16_t   request_length;  // 3+n request length
        DRAWABLE   drawable;
        GCONTEXT   gc;
    };
    // followed by 4nB LISTofPOINT points

    inline static const
    std::vector< std::string_view >& coordinate_mode_names {
        protocol::enum_names::poly_coordinate_mode };
};

// TBD same as PolyPoint other than name, use base type?
struct PolyLine {
    inline static constexpr
    std::string_view name { "PolyLine" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 65
        uint8_t    coordinate_mode;  // coordinate-mode 0 Origin 1 Previous
        uint16_t   request_length;  // 3+n request length
        DRAWABLE   drawable;
        GCONTEXT   gc;
    };
    // followed by 4nB LISTofPOINT points

    inline static const
    std::vector< std::string_view >& coordinate_mode_names {
        protocol::enum_names::poly_coordinate_mode };
};

struct PolySegment {
    inline static constexpr
    std::string_view name { "PolySegment" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 66
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 3+2n request length
        DRAWABLE   drawable;
        GCONTEXT   gc;
    };
    // followed by 8nB LISTofSEGMENT segments n SEGMENT

    struct [[gnu::packed]] SEGMENT {
        INT16  x1, y1;
        INT16  x2, y2;
    };
};

struct PolyRectangle {
    inline static constexpr
    std::string_view name { "PolyRectangle" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 67
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 3+2n request length
        DRAWABLE   drawable;
        GCONTEXT   gc;
    };
    // followed by 8nB LISTofRECTANGLE rectangles n RECTANGLE
};

struct PolyArc {
    inline static constexpr
    std::string_view name { "PolyArc" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 68
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 3+3n request length
        DRAWABLE   drawable;
        GCONTEXT   gc;
    };
    // followed by 12nB LISTofARC arcs n ARC
};

struct FillPoly {
    inline static constexpr
    std::string_view name { "FillPoly" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 69
    private:
        uint8_t    _unused1;
    public:
        uint16_t   request_length;  // 4+n request length
        DRAWABLE   drawable;
        GCONTEXT   gc;
        uint8_t    shape;  // 0 Complex 1 Nonconvex 2 Convex
        uint8_t    coordinate_mode;  // coordinate-mode 0 Origin 1 Previous
    private:
        uint8_t    _unused2[2];
    };
    // followed by 4nB LISTofPOINT points n POINT
};

struct PolyFillRectangle {
    inline static constexpr
    std::string_view name { "PolyFillRectangle" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 70
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 3+2n request length
        DRAWABLE   drawable;
        GCONTEXT   gc;
    };
    // followed by 8nB LISTofRECTANGLE rectangles n RECTANGLE
};

struct PolyFillArc {
    inline static constexpr
    std::string_view name { "PolyFillArc" };

    struct [[gnu::packed]] Encoding {
        uint8_t    opcode;  // 71
    private:
        uint8_t    _unused;
    public:
        uint16_t   request_length;  // 3+3n request length
        DRAWABLE   drawable;
        GCONTEXT   gc;
    };
    // followed by 12nB LISTofARC arcs n ARC
};

/*

PutImage
     1     72                              opcode
     1                                     format
          0     Bitmap
          1     XYPixmap
          2     ZPixmap
     2     6+(n+p)/4                       request length
     4     DRAWABLE                        drawable
     4     GCONTEXT                        gc
     2     CARD16                          width
     2     CARD16                          height
     2     INT16                           dst-x
     2     INT16                           dst-y
     1     CARD8                           left-pad
     1     CARD8                           depth
     2                                     unused
     n     LISTofBYTE                      data
     p                                     unused, p=pad(n)

GetImage
     1     73                              opcode
     1                                     format
          1     XYPixmap
          2     ZPixmap
     2     5                               request length
     4     DRAWABLE                        drawable
     2     INT16                           x
     2     INT16                           y
     2     CARD16                          width
     2     CARD16                          height
     4     CARD32                          plane-mask

▶
     1     1                               Reply
     1     CARD8                           depth
     2     CARD16                          sequence number
     4     (n+p)/4                         reply length
     4     VISUALID                        visual
          0     None
     20                                    unused
     n     LISTofBYTE                      data
     p                                     unused, p=pad(n)

PolyText8
     1     74                              opcode
     1                                     unused
     2     4+(n+p)/4                       request length
     4     DRAWABLE                        drawable
     4     GCONTEXT                        gc
     2     INT16                           x
     2     INT16                           y
     n     LISTofTEXTITEM8                 items
     p                                     unused, p=pad(n)  (p is always 0
                                           or 1)

  TEXTITEM8
     1     m                               length of string (cannot be 255)
     1     INT8                            delta
     m     STRING8                         string
  or
     1     255                             font-shift indicator
     1                                     font byte 3 (most-significant)
     1                                     font byte 2
     1                                     font byte 1
     1                                     font byte 0 (least-significant)

PolyText16
     1     75                              opcode
     1                                     unused
     2     4+(n+p)/4                       request length
     4     DRAWABLE                        drawable
     4     GCONTEXT                        gc
     2     INT16                           x
     2     INT16                           y
     n     LISTofTEXTITEM16                items
     p                                     unused, p=pad(n)  (p must be 0 or
                                           1)

  TEXTITEM16
     1     m                               number of CHAR2Bs in string
                                           (cannot be 255)
     1     INT8                            delta
     2m     STRING16                       string
  or
     1     255                             font-shift indicator
     1                                     font byte 3 (most-significant)
     1                                     font byte 2
     1                                     font byte 1
     1                                     font byte 0 (least-significant)

ImageText8
     1     76                              opcode
     1     n                               length of string
     2     4+(n+p)/4                       request length
     4     DRAWABLE                        drawable
     4     GCONTEXT                        gc
     2     INT16                           x
     2     INT16                           y
     n     STRING8                         string
     p                                     unused, p=pad(n)

ImageText16
     1     77                              opcode
     1     n                               number of CHAR2Bs in string
     2     4+(2n+p)/4                      request length
     4     DRAWABLE                        drawable
     4     GCONTEXT                        gc
     2     INT16                           x
     2     INT16                           y
     2n     STRING16                       string
     p                                     unused, p=pad(2n)

CreateColormap
     1     78                              opcode
     1                                     alloc
          0     None
          1     All
     2     4                               request length
     4     COLORMAP                        mid
     4     WINDOW                          window
     4     VISUALID                        visual

FreeColormap
     1     79                              opcode
     1                                     unused
     2     2                               request length
     4     COLORMAP                        cmap

CopyColormapAndFree
     1     80                              opcode
     1                                     unused
     2     3                               request length
     4     COLORMAP                        mid
     4     COLORMAP                        src-cmap

InstallColormap
     1     81                              opcode
     1                                     unused
     2     2                               request length
     4     COLORMAP                        cmap

UninstallColormap
     1     82                              opcode
     1                                     unused
     2     2                               request length
     4     COLORMAP                        cmap

ListInstalledColormaps
     1     83                              opcode
     1                                     unused
     2     2                               request length
     4     WINDOW                          window

▶
     1     1                               Reply
     1                                     unused
     2     CARD16                          sequence number
     4     n                               reply length
     2     n                               number of COLORMAPs in cmaps
     22                                    unused
     4n     LISTofCOLORMAP                 cmaps

AllocColor
     1     84                              opcode
     1                                     unused
     2     4                               request length
     4     COLORMAP                        cmap
     2     CARD16                          red
     2     CARD16                          green
     2     CARD16                          blue
     2                                     unused

▶
     1     1                               Reply
     1                                     unused
     2     CARD16                          sequence number
     4     0                               reply length
     2     CARD16                          red
     2     CARD16                          green
     2     CARD16                          blue
     2                                     unused
     4     CARD32                          pixel
     12                                    unused

AllocNamedColor
     1     85                              opcode
     1                                     unused
     2     3+(n+p)/4                       request length
     4     COLORMAP                        cmap
     2     n                               length of name
     2                                     unused
     n     STRING8                         name
     p                                     unused, p=pad(n)

▶
     1     1                               Reply
     1                                     unused
     2     CARD16                          sequence number
     4     0                               reply length
     4     CARD32                          pixel
     2     CARD16                          exact-red
     2     CARD16                          exact-green
     2     CARD16                          exact-blue
     2     CARD16                          visual-red
     2     CARD16                          visual-green
     2     CARD16                          visual-blue
     8                                     unused

AllocColorCells
     1     86                              opcode
     1     BOOL                            contiguous
     2     3                               request length
     4     COLORMAP                        cmap
     2     CARD16                          colors
     2     CARD16                          planes

▶
     1     1                               Reply
     1                                     unused
     2     CARD16                          sequence number
     4     n+m                             reply length
     2     n                               number of CARD32s in pixels
     2     m                               number of CARD32s in masks
     20                                    unused
     4n     LISTofCARD32                   pixels
     4m     LISTofCARD32                   masks

AllocColorPlanes
     1     87                              opcode
     1     BOOL                            contiguous
     2     4                               request length
     4     COLORMAP                        cmap
     2     CARD16                          colors
     2     CARD16                          reds
     2     CARD16                          greens
     2     CARD16                          blues

▶
     1     1                               Reply
     1                                     unused
     2     CARD16                          sequence number
     4     n                               reply length
     2     n                               number of CARD32s in pixels
     2                                     unused
     4     CARD32                          red-mask
     4     CARD32                          green-mask
     4     CARD32                          blue-mask
     8                                     unused
     4n     LISTofCARD32                   pixels

FreeColors
     1     88                              opcode
     1                                     unused
     2     3+n                             request length
     4     COLORMAP                        cmap
     4     CARD32                          plane-mask
     4n     LISTofCARD32                   pixels

StoreColors
     1     89                              opcode
     1                                     unused
     2     2+3n                            request length
     4     COLORMAP                        cmap
     12n     LISTofCOLORITEM               items

  COLORITEM
     4     CARD32                          pixel
     2     CARD16                          red
     2     CARD16                          green
     2     CARD16                          blue
     1                                     do-red, do-green, do-blue
          #x01     do-red (1 is True, 0 is False)
          #x02     do-green (1 is True, 0 is False)
          #x04     do-blue (1 is True, 0 is False)
          #xF8     unused
     1                                     unused

StoreNamedColor
     1     90                              opcode
     1                                     do-red, do-green, do-blue
          #x01     do-red (1 is True, 0 is False)
          #x02     do-green (1 is True, 0 is False)
          #x04     do-blue (1 is True, 0 is False)
          #xF8     unused
     2     4+(n+p)/4                       request length
     4     COLORMAP                        cmap
     4     CARD32                          pixel
     2     n                               length of name
     2                                     unused
     n     STRING8                         name
     p                                     unused, p=pad(n)

QueryColors
     1     91                              opcode
     1                                     unused
     2     2+n                             request length
     4     COLORMAP                        cmap
     4n     LISTofCARD32                   pixels

▶
     1     1                               Reply
     1                                     unused
     2     CARD16                          sequence number
     4     2n                              reply length
     2     n                               number of RGBs in colors
     22                                    unused
     8n     LISTofRGB                      colors

  RGB
     2     CARD16                          red
     2     CARD16                          green
     2     CARD16                          blue
     2                                     unused

LookupColor
     1     92                              opcode
     1                                     unused
     2     3+(n+p)/4                       request length
     4     COLORMAP                        cmap
     2     n                               length of name
     2                                     unused
     n     STRING8                         name
     p                                     unused, p=pad(n)

▶
     1     1                               Reply
     1                                     unused
     2     CARD16                          sequence number
     4     0                               reply length
     2     CARD16                          exact-red
     2     CARD16                          exact-green
     2     CARD16                          exact-blue
     2     CARD16                          visual-red
     2     CARD16                          visual-green
     2     CARD16                          visual-blue
     12                                    unused

CreateCursor
     1     93                              opcode
     1                                     unused
     2     8                               request length
     4     CURSOR                          cid
     4     PIXMAP                          source
     4     PIXMAP                          mask
          0     None
     2     CARD16                          fore-red
     2     CARD16                          fore-green
     2     CARD16                          fore-blue
     2     CARD16                          back-red
     2     CARD16                          back-green
     2     CARD16                          back-blue
     2     CARD16                          x
     2     CARD16                          y

CreateGlyphCursor
     1     94                              opcode
     1                                     unused
     2     8                               request length
     4     CURSOR                          cid
     4     FONT                            source-font
     4     FONT                            mask-font
          0     None
     2     CARD16                          source-char
     2     CARD16                          mask-char
     2     CARD16                          fore-red
     2     CARD16                          fore-green
     2     CARD16                          fore-blue
     2     CARD16                          back-red
     2     CARD16                          back-green
     2     CARD16                          back-blue

FreeCursor
     1     95                              opcode
     1                                     unused
     2     2                               request length
     4     CURSOR                          cursor

RecolorCursor
     1     96                              opcode
     1                                     unused
     2     5                               request length
     4     CURSOR                          cursor
     2     CARD16                          fore-red
     2     CARD16                          fore-green
     2     CARD16                          fore-blue
     2     CARD16                          back-red
     2     CARD16                          back-green
     2     CARD16                          back-blue

QueryBestSize
     1     97                              opcode
     1                                     class
          0     Cursor
          1     Tile
          2     Stipple
     2     3                               request length
     4     DRAWABLE                        drawable
     2     CARD16                          width
     2     CARD16                          height

▶
     1     1                               Reply
     1                                     unused
     2     CARD16                          sequence number
     4     0                               reply length
     2     CARD16                          width
     2     CARD16                          height
     20                                    unused

QueryExtension
     1     98                              opcode
     1                                     unused
     2     2+(n+p)/4                       request length
     2     n                               length of name
     2                                     unused
     n     STRING8                         name
     p                                     unused, p=pad(n)

▶
     1     1                               Reply
     1                                     unused
     2     CARD16                          sequence number
     4     0                               reply length
     1     BOOL                            present
     1     CARD8                           major-opcode
     1     CARD8                           first-event
     1     CARD8                           first-error
     20                                    unused

ListExtensions
     1     99                              opcode
     1                                     unused
     2     1                               request length

▶
     1     1                               Reply
     1     CARD8                           number of STRs in names
     2     CARD16                          sequence number
     4     (n+p)/4                         reply length
     24                                    unused
     n     LISTofSTR                       names
     p                                     unused, p=pad(n)

ChangeKeyboardMapping
     1     100                             opcode
     1     n                               keycode-count
     2     2+nm                            request length
     1     KEYCODE                         first-keycode
     1     m                               keysyms-per-keycode
     2                                     unused
     4nm     LISTofKEYSYM                  keysyms

GetKeyboardMapping
     1     101                             opcode
     1                                     unused
     2     2                               request length
     1     KEYCODE                         first-keycode
     1     m                               count
     2                                     unused

▶
     1     1                               Reply
     1     n                               keysyms-per-keycode
     2     CARD16                          sequence number
     4     nm                              reply length (m = count field
                                           from the request)
     24                                    unused
     4nm     LISTofKEYSYM                  keysyms

ChangeKeyboardControl
     1     102                             opcode
     1                                     unused
     2     2+n                             request length
     4     BITMASK                         value-mask (has n bits set to 1)
          #x0001     key-click-percent
          #x0002     bell-percent
          #x0004     bell-pitch
          #x0008     bell-duration
          #x0010     led
          #x0020     led-mode
          #x0040     key
          #x0080     auto-repeat-mode
     4n     LISTofVALUE                    value-list

  VALUEs
     1     INT8                            key-click-percent
     1     INT8                            bell-percent
     2     INT16                           bell-pitch
     2     INT16                           bell-duration
     1     CARD8                           led
     1                                     led-mode
          0     Off
          1     On
     1     KEYCODE                         key
     1                                     auto-repeat-mode
          0     Off
          1     On
          2     Default

GetKeyboardControl
     1     103                             opcode
     1                                     unused
     2     1                               request length

▶
     1     1                               Reply
     1                                     global-auto-repeat
          0     Off
          1     On
     2     CARD16                          sequence number
     4     5                               reply length
     4     CARD32                          led-mask
     1     CARD8                           key-click-percent
     1     CARD8                           bell-percent
     2     CARD16                          bell-pitch
     2     CARD16                          bell-duration
     2                                     unused
     32     LISTofCARD8                    auto-repeats

Bell
     1     104                             opcode
     1     INT8                            percent
     2     1                               request length

ChangePointerControl
     1     105                             opcode
     1                                     unused
     2     3                               request length
     2     INT16                           acceleration-numerator
     2     INT16                           acceleration-denominator
     2     INT16                           threshold
     1     BOOL                            do-acceleration
     1     BOOL                            do-threshold

GetPointerControl
     1     106                             opcode
     1                                     unused
     2     1                               request length

▶
     1     1                               Reply
     1                                     unused
     2     CARD16                          sequence number
     4     0                               reply length
     2     CARD16                          acceleration-numerator
     2     CARD16                          acceleration-denominator
     2     CARD16                          threshold
     18                                    unused

SetScreenSaver
     1     107                             opcode
     1                                     unused
     2     3                               request length
     2     INT16                           timeout
     2     INT16                           interval
     1                                     prefer-blanking
          0     No
          1     Yes
          2     Default
     1                                     allow-exposures
          0     No
          1     Yes
          2     Default
     2                                     unused

GetScreenSaver
     1     108                             opcode
     1                                     unused
     2     1                               request length

▶
     1     1                               Reply
     1                                     unused
     2     CARD16                          sequence number
     4     0                               reply length
     2     CARD16                          timeout
     2     CARD16                          interval
     1                                     prefer-blanking
          0     No
          1     Yes
     1                                     allow-exposures
          0     No
          1     Yes
     18                                    unused

ChangeHosts
     1     109                             opcode
     1                                     mode
          0     Insert
          1     Delete
     2     2+(n+p)/4                       request length
     1                                     family
          0     Internet
          1     DECnet
          2     Chaos
     1                                     unused
     2     n                               length of address
     n     LISTofCARD8                     address
     p                                     unused, p=pad(n)

ListHosts
     1     110                             opcode
     1                                     unused
     2     1                               request length

▶
     1     1                               Reply
     1                                     mode
          0     Disabled
          1     Enabled
     2     CARD16                          sequence number
     4     n/4                             reply length
     2     CARD16                          number of HOSTs in hosts
     22                                    unused
     n     LISTofHOST                      hosts (n always a multiple of 4)

SetAccessControl
     1     111                             opcode
     1                                     mode
          0     Disable
          1     Enable
     2     1                               request length

SetCloseDownMode
     1     112                             opcode
     1                                     mode
          0     Destroy
          1     RetainPermanent
          2     RetainTemporary
     2     1                               request length

KillClient
     1     113                             opcode
     1                                     unused
     2     2                               request length
     4     CARD32                          resource
          0     AllTemporary

RotateProperties
     1     114                             opcode
     1                                     unused
     2     3+n                             request length
     4     WINDOW                          window
     2     n                               number of properties
     2     INT16                           delta
     4n    LISTofATOM                      properties

ForceScreenSaver
     1     115                             opcode
     1                                     mode
          0     Reset
          1     Activate
     2     1                               request length

SetPointerMapping
     1     116                             opcode
     1     n                               length of map
     2     1+(n+p)/4                       request length
     n     LISTofCARD8                     map
     p                                     unused, p=pad(n)

▶
     1     1                               Reply
     1                                     status
          0     Success
          1     Busy
     2     CARD16                          sequence number
     4     0                               reply length
     24                                    unused

GetPointerMapping
     1     117                             opcode
     1                                     unused
     2     1                               request length

▶
     1     1                               Reply
     1     n                               length of map
     2     CARD16                          sequence number
     4     (n+p)/4                         reply length
     24                                    unused
     n     LISTofCARD8                     map
     p                                     unused, p=pad(n)

SetModifierMapping
     1     118                             opcode
     1     n                               keycodes-per-modifier
     2     1+2n                            request length
     8n    LISTofKEYCODE                   keycodes

▶
     1     1                               Reply
     1                                     status
          0     Success
          1     Busy
          2     Failed
     2     CARD16                          sequence number
     4     0                               reply length
     24                                    unused

GetModifierMapping
     1     119                             opcode
     1                                     unused
     2     1                               request length

▶
     1     1                               Reply
     1     n                               keycodes-per-modifier
     2     CARD16                          sequence number
     4     2n                              reply length
     24                                    unused
     8n     LISTofKEYCODE                  keycodes

NoOperation
     1     127                             opcode
     1                                     unused
     2     1+n                             request length
     4n                                    unused
*/

}  // namespace requests

}  // namespace protocol


#endif  // REQUESTS_HPP
