#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP

#include <cstdint>
#include <vector>
#include <string_view>

#include "protocol/enum_names.hpp"


// https://www.x.org/releases/current/doc/xproto/x11protocol.pdf
// Chapter 3. Common Types
// Appendix B. Protocol Encoding: Common Types

namespace protocol {

// base types

using INT8         = int8_t;
using INT16        = int16_t;
using INT32        = int32_t;

using CARD8        = uint8_t;
using CARD16       = uint16_t;
using CARD32       = uint32_t;

using BYTE         = uint8_t;
// common simple types

// TBD switching to structs with data member from aliases to allow for overloading and type resolution

struct [[gnu::packed]] TIMESTAMP {
    CARD32 data;
};

namespace impl {

inline static constexpr uint32_t XID_ZERO_BITS { 0xE0000000 };

}  // namespace impl

struct [[gnu::packed]] CURSOR {
    CARD32 data;

    // (top three bits guaranteed to be zero)
    inline static constexpr uint32_t ZERO_BITS { impl::XID_ZERO_BITS };
};

struct [[gnu::packed]] COLORMAP {
    CARD32 data;

    // (top three bits guaranteed to be zero)
    inline static constexpr uint32_t ZERO_BITS { impl::XID_ZERO_BITS };
};

struct [[gnu::packed]] ATOM {
    CARD32 data;

    // (top three bits guaranteed to be zero)
    inline static constexpr uint32_t ZERO_BITS { impl::XID_ZERO_BITS };
};

struct [[gnu::packed]] VISUALID {
    CARD32 data;

    // (top three bits guaranteed to be zero)
    inline static constexpr uint32_t ZERO_BITS { impl::XID_ZERO_BITS };
};

using VALUE = CARD32;  // TBD currently only used for sizeof()
//struct [[gnu::packed]] VALUE           = CARD32;  // (used only in LISTofVALUE)
// using LISTofVALUE     = VALUE*;

struct [[gnu::packed]] WINDOW {
    CARD32 data;

    // (top three bits guaranteed to be zero)
    inline static constexpr uint32_t ZERO_BITS { impl::XID_ZERO_BITS };
};
struct [[gnu::packed]] PIXMAP {
    CARD32 data;

    // (top three bits guaranteed to be zero)
    inline static constexpr uint32_t ZERO_BITS { impl::XID_ZERO_BITS };
};
union DRAWABLE {
    WINDOW window;
    PIXMAP pixmap;
};

struct [[gnu::packed]] FONT {
    CARD32 data;

    // (top three bits guaranteed to be zero)
    inline static constexpr uint32_t ZERO_BITS { impl::XID_ZERO_BITS };
};
struct [[gnu::packed]] GCONTEXT {
    CARD32 data;

    // (top three bits guaranteed to be zero)
    inline static constexpr uint32_t ZERO_BITS { impl::XID_ZERO_BITS };
};
union FONTABLE {
    FONT     font;
    GCONTEXT gcontext;
};

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

    inline static const std::vector<std::string_view> enum_names {
        "Forget",     //  0
        "NorthWest",  //  1
        "North",      //  2
        "NorthEast",  //  3
        "West",       //  4
        "Center",     //  5
        "East",       //  6
        "SouthWest",  //  7
        "South",      //  8
        "SouthEast",  //  9
        "Static"      // 10
    };
};

struct [[gnu::packed]] WINGRAVITY {
    CARD8 data;

    inline static const std::vector<std::string_view> enum_names {
        "Unmap",      //  0
        "NorthWest",  //  1
        "North",      //  2
        "NorthEast",  //  3
        "West",       //  4
        "Center",     //  5
        "East",       //  6
        "SouthWest",  //  7
        "South",      //  8
        "SouthEast",  //  9
        "Static"      // 10
    };
};

// TBD encoding seems to always use 1 byte for BOOL
// TBD neither
//   - https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Common_Types
//   - https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Common_Types
//   give a width for BOOL, but seemingly all its other appearances in #Encoding
//   indicate 1 byte
struct [[gnu::packed]] BOOL {
    uint8_t data;

    inline static const std::vector<std::string_view> enum_names {
        "False",  // 0
        "True"    // 1
    };
};

namespace impl {

inline const
std::vector<std::string_view> SET_OF_EVENT_FLAG_NAMES {
    "KeyPress",              // 1 <<  0
    "KeyRelease",            // 1 <<  1
    "ButtonPress",           // 1 <<  2
    "ButtonRelease",         // 1 <<  3
    "EnterWindow",           // 1 <<  4
    "LeaveWindow",           // 1 <<  5
    "PointerMotion",         // 1 <<  6
    "PointerMotionHint",     // 1 <<  7
    "Button1Motion",         // 1 <<  8
    "Button2Motion",         // 1 <<  9
    "Button3Motion",         // 1 << 10
    "Button4Motion",         // 1 << 11
    "Button5Motion",         // 1 << 12
    "ButtonMotion",          // 1 << 13
    "KeymapState",           // 1 << 14
    "Exposure",              // 1 << 15
    "VisibilityChange",      // 1 << 16
    "StructureNotify",       // 1 << 17
    "ResizeRedirect",        // 1 << 18
    "SubstructureNotify",    // 1 << 19
    "SubstructureRedirect",  // 1 << 20
    "FocusChange",           // 1 << 21
    "PropertyChange",        // 1 << 22
    "ColormapChange",        // 1 << 23
    "OwnerGrabButton"        // 1 << 24
};

}  // namespace impl

struct [[gnu::packed]] SETofEVENT {
    CARD32 data;

    // must be zeroed in SETofEVENT values
    inline static constexpr uint32_t ZERO_BITS { 0xFE000000 };
    inline static const std::vector<std::string_view>& flag_names {
        impl::SET_OF_EVENT_FLAG_NAMES };
};
struct [[gnu::packed]] SETofPOINTEREVENT {
    CARD16 data;

    // must be zeroed in SETofPOINTEREVENT values
    inline static constexpr uint32_t ZERO_BITS { 0xFFFF8003 };
    inline static const std::vector<std::string_view>& flag_names {
        impl::SET_OF_EVENT_FLAG_NAMES };
};
// TBD 2B in GetWindowAttributes reply
struct [[gnu::packed]] SETofDEVICEEVENT {
    CARD16 data;

    // must be zeroed in SETofDEVICEEVENT values
    inline static constexpr uint32_t ZERO_BITS { 0xFFFFC0B0 };
    inline static const std::vector<std::string_view>& flag_names {
        impl::SET_OF_EVENT_FLAG_NAMES };
};


// input

struct [[gnu::packed]] KEYSYM {
    CARD32 data;

    // (top three bits guaranteed to be zero)
    inline static constexpr uint32_t ZERO_BITS { impl::XID_ZERO_BITS };
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

namespace impl {

inline const
std::vector<std::string_view> SET_OF_KEYBUTMASK_FLAG_NAMES {
    "Shift",    // 1 <<  0
    "Lock",     // 1 <<  1
    "Control",  // 1 <<  2
    "Mod1",     // 1 <<  3
    "Mod2",     // 1 <<  4
    "Mod3",     // 1 <<  5
    "Mod4",     // 1 <<  6
    "Mod5",     // 1 <<  7
    "Button1",  // 1 <<  8
    "Button2",  // 1 <<  9
    "Button3",  // 1 << 10
    "Button4",  // 1 << 11
    "Button5"   // 1 << 12
};

}  // namespace impl

// TBD are these sets only 2 bytes?
struct [[gnu::packed]] SETofKEYBUTMASK {
    CARD16 data;

    // must be zeroed in SETofKEYBUTMASK values
    inline static constexpr uint16_t ZERO_BITS { 0xE000 };
    inline static const std::vector<std::string_view>& flag_names {
        impl::SET_OF_KEYBUTMASK_FLAG_NAMES };
};

struct [[gnu::packed]] SETofKEYMASK {
    CARD16 data;

    // must be zeroed in SETofKEYMASK values
    inline static constexpr uint16_t ZERO_BITS { 0xFF00 };
    // special alternate flag value for SETofKEYMASK (violates zero bits rule)
    inline static constexpr uint16_t ANYMODIFIER { 0x8000 };
    inline static constexpr std::string_view anymodifier_flag_name { "AnyModifier" };
    inline static const std::vector<std::string_view>& flag_names {
        impl::SET_OF_KEYBUTMASK_FLAG_NAMES };
};


/*
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
using STRING16     = CHAR2B;  // LISTofCHAR2B

// other structs
*/
struct [[gnu::packed]] POINT {
    INT16 x, y;
};

struct [[gnu::packed]] RECTANGLE {
    INT16  x, y;
    CARD16 width, height;
};

struct [[gnu::packed]] ARC {
    INT16  x, y;
    CARD16 width, height;
    INT16  angle1, angle2;
};

struct [[gnu::packed]] HOST {
    CARD8      family;
    CARD8      unused;
    CARD16     n;  // length of address in bytes

    inline static const std::vector<std::string_view>& family_names {
        protocol::enum_names::host_family };
};
// followed by LISTofBYTE address

struct [[gnu::packed]] STR {
    uint8_t  n;  // length of name in bytes
};
// followed by length n STRING8 name


}  // namespace protocol


#endif  // COMMON_TYPES_HPP
