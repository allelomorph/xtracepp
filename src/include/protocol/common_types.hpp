#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP

#include <cstdint>

// https://www.x.org/releases/current/doc/xproto/x11protocol.pdf
// Chapter 3. Common Types
// Appendix B. Protocol Encoding: Common Types

namespace protocol {

// base types

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

using BYTE         = uint8_t;
// using LISTofBYTE   = BYTE*;
// common simple types

// TBD switching to structs with data member from aliases to allow for overloading and type resolution

struct [[gnu::packed]] TIMESTAMP {
    CARD32 data;
};
// using LISTofTIMESTAMP = TIMESTAMP*;

struct [[gnu::packed]] CURSOR {
    CARD32 data;  // (top three bits guaranteed to be zero)
};
// using LISTofCURSOR    = CURSOR*;

struct [[gnu::packed]] COLORMAP {
    CARD32 data;  // (top three bits guaranteed to be zero)
};
// using LISTofCOLORMAP  = COLORMAP*;

struct [[gnu::packed]] ATOM {
    CARD32 data;  // (top three bits guaranteed to be zero)
};
// using LISTofATOM      = ATOM*;

struct [[gnu::packed]] VISUALID {
    CARD32 data;  // (top three bits guaranteed to be zero)
};
// using LISTofVISUALID  = VISUALID*;

//struct [[gnu::packed]] VALUE           = CARD32;  // (used only in LISTofVALUE)
// using LISTofVALUE     = VALUE*;

struct [[gnu::packed]] WINDOW {
    CARD32 data;  // (top three bits guaranteed to be zero)
};
struct [[gnu::packed]] PIXMAP {
    CARD32 data;  // (top three bits guaranteed to be zero)
};
union DRAWABLE {
    WINDOW window;
    PIXMAP pixmap;
};
// using LISTofWINDOW    = WINDOW*;
// using LISTofPIXMAP    = PIXMAP*;
// using LISTofDRAWABLE  = DRAWABLE*;

struct [[gnu::packed]] FONT {
    CARD32 data;  // (top three bits guaranteed to be zero)
};
struct [[gnu::packed]] GCONTEXT {
    CARD32 data;  // (top three bits guaranteed to be zero)
};
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

// TBD BITMASK can be 2 or 4 bytes (at least in requests)
//using BITMASK         = CARD32;

// BITGRAVITY and WINGRAVITY only appear as 1B in core protocol encoding (
//    requests CreateWindow and GetWindowAttributes)
// TBD need to decide between:
//   - enum class: collision protection but need to convert to compare
//   - enum: less verbose comparison but need prefixes or namespaces to avoid collisions
struct [[gnu::packed]] BITGRAVITY {
    CARD8 data;
};
// enum BitGravity {
//     FORGET,      //  0
//     NORTHWEST,   //  1
//     NORTH,       //  2
//     NORTHEAST,   //  3
//     WEST,        //  4
//     CENTER,      //  5
//     EAST,        //  6
//     SOUTHWEST,   //  7
//     SOUTH,       //  8
//     SOUTHEAST,   //  9
//     STATIC       // 10
// };
// enum BitGravity {
//     "Forget",      // 0
//     "NorthWest",   // 1
//     "North",       // 2
//     "NorthEast",   // 3
//     "West",        // 4
//     "Center",      // 5
//     "East",        // 6
//     "SouthWest",   // 7
//     "South",       // 8
//     "SouthEast",   // 9
//     "Static"       //10
// };

struct [[gnu::packed]] WINGRAVITY {
    CARD8 data;
};
// enum WinGravity {
//     UNMAP,       //  0
//     NORTHWEST,   //  1
//     NORTH,       //  2
//     NORTHEAST,   //  3
//     WEST,        //  4
//     CENTER,      //  5
//     EAST,        //  6
//     SOUTHWEST,   //  7
//     SOUTH,       //  8
//     SOUTHEAST,   //  9
//     STATIC       // 10
// };
// enum WinGravity {
//     "Unmap",       // 0
//     "NorthWest",   // 1
//     "North",       // 2
//     "NorthEast",   // 3
//     "West",        // 4
//     "Center",      // 5
//     "East",        // 6
//     "SouthWest",   // 7
//     "South",       // 8
//     "SouthEast",   // 9
//     "Static"       //10
// };

// enum class BOOL {
//     True, False
// };

// TBD encoding seems to always use 1 byte for BOOL
// TBD neither
//   - https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Common_Types
//   - https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Common_Types
//   give a width for BOOL, but seemingly all its other appearances in #Encoding
//   indicate 1 byte
struct [[gnu::packed]] BOOL {
    uint8_t data;
};

// events

struct [[gnu::packed]] SETofEVENT {
    CARD32 data;
};
struct [[gnu::packed]] SETofPOINTEREVENT {
    CARD16 data;
};
// TBD 2B in GetWindowAttributes reply, 4 in CreateWindow LISTofVALUE (disregard, VALUEs are always 4B)
struct [[gnu::packed]] SETofDEVICEEVENT {
    CARD16 data;
};
// enum EventMaskFlags {
//     KEYPRESS             = 1 <<  0,  // KeyPress
//     KEYRELEASE           = 1 <<  1,  // KeyRelease
//     BUTTONPRESS          = 1 <<  2,  // ButtonPress
//     BUTTONRELEASE        = 1 <<  3,  // ButtonRelease
//     ENTERWINDOW          = 1 <<  4,  // EnterWindow
//     LEAVEWINDOW          = 1 <<  5,  // LeaveWindow
//     POINTERMOTION        = 1 <<  6,  // PointerMotion
//     POINTERMOTIONHINT    = 1 <<  7,  // PointerMotionHint
//     BUTTON1MOTION        = 1 <<  8,  // Button1Motion
//     BUTTON2MOTION        = 1 <<  9,  // Button2Motion
//     BUTTON3MOTION        = 1 << 10,  // Button3Motion
//     BUTTON4MOTION        = 1 << 11,  // Button4Motion
//     BUTTON5MOTION        = 1 << 12,  // Button5Motion
//     BUTTONMOTION         = 1 << 13,  // ButtonMotion
//     KEYMAPSTATE          = 1 << 14,  // KeymapState
//     EXPOSURE             = 1 << 15,  // Exposure
//     VISIBILITYCHANGE     = 1 << 16,  // VisibilityChange
//     STRUCTURENOTIFY      = 1 << 17,  // StructureNotify
//     RESIZEREDIRECT       = 1 << 18,  // ResizeRedirect
//     SUBSTRUCTURENOTIFY   = 1 << 19,  // SubstructureNotify
//     SUBSTRUCTUREREDIRECT = 1 << 20,  // SubstructureRedirect
//     FOCUSCHANGE          = 1 << 21,  // FocusChange
//     PROPERTYCHANGE       = 1 << 22,  // PropertyChange
//     COLORMAPCHANGE       = 1 << 23,  // ColormapChange
//     OWNERGRABBUTTON      = 1 << 24,  // OwnerGrabButton
//     // must be zeroed in SETofEVENT values
//     _SETOFEVENT_ZERO_BITS        = 0XFE000000,
//     // must be zeroed in SETofPOINTEREVENT values
//     _SETOFPOINTEREVENT_ZERO_BITS = 0XFFFF8003,
//     // must be zeroed in SETofDEVICEEVENT values
//     _SETOFDEVICEEVENT_ZERO_BITS  = 0XFFFFC0B0
// };

// input

struct [[gnu::packed]] KEYSYM {
    CARD32 data;  // (top three bits guaranteed to be zero)
};
struct [[gnu::packed]] KEYCODE {
    CARD8 data;
};
struct [[gnu::packed]] BUTTON {
    CARD8 data;
};
/*
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
*/
// TBD are these sets only 2 bytes?
struct [[gnu::packed]] SETofKEYBUTMASK {
    CARD16 data;
};
struct [[gnu::packed]] SETofKEYMASK {
    CARD16 data;
};
/*
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
*/
struct [[gnu::packed]] CHAR2B {
    CARD8 byte1, byte2;
};
/*
// using LISTofCHAR2B = CHAR2B*;
using STRING16     = CHAR2B;  // LISTofCHAR2B

// other structs
*/
struct [[gnu::packed]] POINT {
    INT16 x, y;
};

// using LISTofPOINT = POINT*;

struct [[gnu::packed]] RECTANGLE {
    INT16  x, y;
    CARD16 width, height;
};
// using LISTofRECTANGLE = RECTANGLE*;

struct [[gnu::packed]] ARC {
    INT16  x, y;
    CARD16 width, height;
    INT16  angle1, angle2;
};
// using LISTofARC = ARC*;

struct [[gnu::packed]] HOST {
    CARD8      family;
    CARD8      unused;
    CARD16     n;  // length of address in bytes
};
// followed by LISTofBYTE address

}  // namespace protocol


#endif  // COMMON_TYPES_HPP
