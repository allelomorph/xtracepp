#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP

#include <cstdint>

// https://www.x.org/releases/current/doc/xproto/x11protocol.pdf
// Chapter 3. Common Types
// Appendix B. Protocol Encoding: Common Types

namespace protocol {

// base types

using BYTE         = uint8_t;
// using LISTofBYTE   = BYTE*;

using INT8         = int8_t;
using INT16        = int16_t;
using INT32        = int32_t;
// using LISTofINT8   = INT8*;
// using LISTofINT16  = INT16*;
// using LISTofINT32  = INT32*;

using CARD8        = uint8_t;
using CARD16       = uint16_t;
using CARD32       = uint32_t;
// using LISTofCARD8  = CARD8*;
// using LISTofCARD16 = CARD16*;
// using LISTofCARD32 = CARD32*;

// common simple types

using TIMESTAMP       = CARD32;
// using LISTofTIMESTAMP = TIMESTAMP*;

using CURSOR          = CARD32;  // (top three bits guaranteed to be zero)
// using LISTofCURSOR    = CURSOR*;

using COLORMAP        = CARD32;  // (top three bits guaranteed to be zero)
// using LISTofCOLORMAP  = COLORMAP*;

using ATOM            = CARD32;  // (top three bits guaranteed to be zero)
// using LISTofATOM      = ATOM*;

using VISUALID        = CARD32;  // (top three bits guaranteed to be zero)
// using LISTofVISUALID  = VISUALID*;

using VALUE           = CARD32;  // (used only in LISTofVALUE)
// using LISTofVALUE     = VALUE*;

using WINDOW          = CARD32;  // (top three bits guaranteed to be zero)
using PIXMAP          = CARD32;  // (top three bits guaranteed to be zero)
union DRAWABLE {
    WINDOW window;
    PIXMAP pixmap;
};
// using LISTofWINDOW    = WINDOW*;
// using LISTofPIXMAP    = PIXMAP*;
// using LISTofDRAWABLE  = DRAWABLE*;

using FONT            = CARD32;  // (top three bits guaranteed to be zero)
using GCONTEXT        = CARD32;  // (top three bits guaranteed to be zero)
union FONTABLE {
    FONT     font;
    GCONTEXT gcontext;
};
// using LISTofFONT      = FONT*;
// using LISTofGCONTEXT  = GCONTEXT*;
// using LISTofFONTABLE  = FONTABLE*;

// "The types BITMASK and LISTofVALUE are somewhat special. Various requests
//   contain arguments of the form:
//       value-mask: BITMASK
//       value-list: LISTofVALUE
//   These are used to allow the client to specify a subset of a heterogeneous
//   collection of optional arguments. The value-mask specifies which arguments
//   are to be provided; each such argument is assigned a unique bit position.
//   The representation of the BITMASK will typically contain more bits than
//   there are defined arguments. The unused bits in the value-mask must be zero
//   (or the server generates a Value error). The value-list contains one value
//   for each bit set to 1 in the mask, from least significant to most
//   significant bit in the mask. Each value is represented with four bytes,
//   but the actual value occupies only the least significant bytes as required.
//   The values of the unused bytes do not matter."

using BITMASK         = CARD32;
/*
enum class BITGRAVITY {
    Forget, Static,
    NorthWest, North, NorthEast,
    West, Center, East,
    SouthWest, South, SouthEast
};

enum class WINGRAVITY {
    Unmap, Static,
    NorthWest, North, NorthEast,
    West, Center, East,
    SouthWest, South, SouthEast
};

enum class BOOL {
    True, False
};

// events

enum class EVENT {
    KeyPress, KeyRelease,
    OwnerGrabButton,
    ButtonPress, ButtonRelease,
    EnterWindow, LeaveWindow,
    PointerMotion, PointerMotionHint,
    Button1Motion, Button2Motion, Button3Motion, Button4Motion, Button5Motion, ButtonMotion,
    Exposure, VisibilityChange,
    StructureNotify, ResizeRedirect, SubstructureNotify, SubstructureRedirect,
    FocusChange, PropertyChange, ColormapChange, KeymapState
};

enum class POINTEREVENT {
    ButtonPress, ButtonRelease,
    EnterWindow, LeaveWindow,
    PointerMotion, PointerMotionHint,
    Button1Motion, Button2Motion, Button3Motion, Button4Motion, Button5Motion, ButtonMotion,
    KeymapState
};

enum class DEVICEEVENT {
    KeyPress, KeyRelease,
    ButtonPress, ButtonRelease,
    PointerMotion,
    Button1Motion, Button2Motion, Button3Motion, Button4Motion, Button5Motion, ButtonMotion
};

using SETofEVENT        = BITMASK;
using SETofPOINTEREVENT = BITMASK;
using SETofDEVICEEVENT  = BITMASK;
enum class EventMaskFlags {
    KeyPress             = 1 << 0,
    KeyRelease           = 1 << 1,
    ButtonPress          = 1 << 2,
    ButtonRelease        = 1 << 3,
    EnterWindow          = 1 << 4,
    LeaveWindow          = 1 << 5,
    PointerMotion        = 1 << 6,
    PointerMotionHint    = 1 << 7,
    Button1Motion        = 1 << 8,
    Button2Motion        = 1 << 9,
    Button3Motion        = 1 << 10,
    Button4Motion        = 1 << 11,
    Button5Motion        = 1 << 12,
    ButtonMotion         = 1 << 13,
    KeymapState          = 1 << 14,
    Exposure             = 1 << 15,
    VisibilityChange     = 1 << 16,
    StructureNotify      = 1 << 17,
    ResizeRedirect       = 1 << 18,
    SubstructureNotify   = 1 << 19,
    SubstructureRedirect = 1 << 20,
    FocusChange          = 1 << 21,
    PropertyChange       = 1 << 22,
    ColormapChange       = 1 << 23,
    OwnerGrabButton      = 1 << 24,
    // must be zeroed in SETofEVENT values
    _SETofEVENT_zero_bits        = 0xFE000000,
    // must be zeroed in SETofPOINTEREVENT values
    _SETofPOINTEREVENT_zero_bits = 0xFFFF8003,
    // must be zeroed in SETofDEVICEEVENT values
    _SETofDEVICEEVENT_zero_bits  = 0xFFFFC0B0
};

// input

using KEYSYM  = CARD32;  // (top three bits guaranteed to be zero)
using KEYCODE = CARD8;
using BUTTON  = CARD8;
enum class KEYMASK {
    Shift, Lock, Control,
    Mod1, Mod2, Mod3, Mod4, Mod5
};
enum class BUTMASK {
    Button1, Button2, Button3, Button4, Button5
};
union KEYBUTMASK {
    KEYMASK keymask;
    BUTMASK butmask;
};
// TBD are these sets only 2 bytes?
using SETofKEYBUTMASK = BITMASK;
using SETofKEYMASK    = BITMASK;
enum class KeyButMaskFlags {
    Shift   = 1 << 0,
    Lock    = 1 << 1,
    Control = 1 << 2,
    Mod1    = 1 << 3,
    Mod2    = 1 << 4,
    Mod3    = 1 << 5,
    Mod4    = 1 << 6,
    Mod5    = 1 << 7,
    Button1 = 1 << 8,
    Button2 = 1 << 9,
    Button3 = 1 << 10,
    Button4 = 1 << 11,
    Button5 = 1 << 12,
    // must be zeroed in SETofKEYBUTMASK values
    _SETofKEYBUTMASK_zero_bits = 0xE000,
    // must be zeroed in SETofKEYMASK values
    _SETofKEYMASK_zero_bits    = 0xFF00
};

// strings

using STRING8     = char;  // LISTofCARD8
struct STR {
    CARD8 n;       // length of name in bytes
    STRING8 name;
};

struct CHAR2B {
    CARD8 byte1, byte2;
};
// using LISTofCHAR2B = CHAR2B*;
using STRING16     = CHAR2B;  // LISTofCHAR2B

// other structs

struct POINT {
    INT16 x, y;
};
// using LISTofPOINT = POINT*;

struct RECTANGLE {
    INT16  x, y;
    CARD16 width, height;
};
// using LISTofRECTANGLE = RECTANGLE*;

struct ARC {
    INT16  x, y;
    CARD16 width, height;
    INT16  angle1, angle2;
};
// using LISTofARC = ARC*;

struct HOST {
    CARD8      family;
    CARD8      unused;
    CARD16     n;  // length of address in bytes
    //LISTofBYTE address;
};
*/

}  // namespace protocol


#endif  // COMMON_TYPES_HPP
