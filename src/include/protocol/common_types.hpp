#ifndef PROTOCOL_COMMON_TYPES_HPP
#define PROTOCOL_COMMON_TYPES_HPP

#include <cstdint>
#include <vector>
#include <string_view>

#include "protocol/enum_names.hpp"  // host_family


// https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Common_Types

namespace protocol {

using INT8   = int8_t;
using INT16  = int16_t;
using INT32  = int32_t;

using CARD8  = uint8_t;
using CARD16 = uint16_t;
using CARD32 = uint32_t;

using BYTE   = uint8_t;

// No BITMASK alias as it can be 2 or 4 bytes in core encoding
using VALUE  = CARD32;  // always 4B regardless of associated type

namespace impl {

struct [[gnu::packed]] Integer {};
struct [[gnu::packed]] Bitmask : public Integer {};
struct [[gnu::packed]] ResourceId : public Integer {
    CARD32 data;
    // must be zeroed in ResourceId values
    static constexpr uint32_t ZERO_BITS { 0xE0000000 };
};

struct [[gnu::packed]] Struct {};
struct [[gnu::packed]] StructWithSuffixes : public Struct {};


}  // namespace impl

struct [[gnu::packed]] WINDOW : public impl::ResourceId {};

struct [[gnu::packed]] PIXMAP : public impl::ResourceId {};

struct [[gnu::packed]] CURSOR : public impl::ResourceId {};

struct [[gnu::packed]] FONT : public impl::ResourceId {};

struct [[gnu::packed]] GCONTEXT : public impl::ResourceId {};

struct [[gnu::packed]] COLORMAP : public impl::ResourceId {};

// TBD should we make ResourceId rather than union?
union [[gnu::packed]] DRAWABLE {
    WINDOW window;
    PIXMAP pixmap;
};

// TBD should we make ResourceId rather than union?
union [[gnu::packed]] FONTABLE {
    FONT     font;
    GCONTEXT gcontext;
};

struct [[gnu::packed]] ATOM : public impl::ResourceId {};

struct [[gnu::packed]] VISUALID : public impl::ResourceId {};

struct [[gnu::packed]] TIMESTAMP : public impl::Integer {
    CARD32 data;
};

struct [[gnu::packed]] BITGRAVITY : public impl::Integer {
    CARD8 data;

    inline static const
    std::vector< std::string_view > enum_names {
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

struct [[gnu::packed]] WINGRAVITY : public impl::Integer {
    CARD8 data;

    inline static const
    std::vector< std::string_view > enum_names {
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

struct [[gnu::packed]] BOOL : public impl::Integer {
    CARD8 data;

    inline static const
    std::vector< std::string_view > enum_names {
        "False",  //  0
        "True"    //  1
    };
};

namespace impl {

struct [[gnu::packed]] SETofEVENTBase : public impl::Bitmask {
    inline static const
    std::vector< std::string_view >& flag_names {
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
};

}  // namespace impl

struct [[gnu::packed]] SETofEVENT : public impl::SETofEVENTBase {
    CARD32 data;

    // must be zeroed in SETofEVENT values
    static constexpr uint32_t ZERO_BITS { 0xFE000000 };
};

struct [[gnu::packed]] SETofPOINTEREVENT : public impl::SETofEVENTBase {
    CARD16 data;

    // must be zeroed in SETofPOINTEREVENT values
    //   (4B in case of use in LISTofVALUE)
    static constexpr uint32_t ZERO_BITS { 0xFFFF8003 };
};

struct [[gnu::packed]] SETofDEVICEEVENT : public impl::SETofEVENTBase {
    CARD16 data;

    // must be zeroed in SETofDEVICEEVENT values
    //   (4B in case of use in LISTofVALUE)
    static constexpr uint32_t ZERO_BITS { 0xFFFFC0B0 };
};

struct [[gnu::packed]] KEYSYM : public impl::Integer {
    CARD32 data;
};

struct [[gnu::packed]] KEYCODE : public impl::Integer {
    CARD8 data;
};

struct [[gnu::packed]] BUTTON : public impl::Integer {
    CARD8 data;
};

struct [[gnu::packed]] SETofKEYBUTMASK : public impl::Bitmask {
    CARD16 data;

    // must be zeroed in SETofKEYBUTMASK values
    static constexpr uint16_t ZERO_BITS { 0xE000 };
    inline static const
    std::vector< std::string_view > flag_names {
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
};

struct [[gnu::packed]] SETofKEYMASK : public SETofKEYBUTMASK {
    // must be zeroed in SETofKEYMASK values
    static constexpr uint16_t ZERO_BITS { 0xFF00 };
    // special alternate flag value for SETofKEYMASK (violates zero bits rule)
    static constexpr uint16_t ANYMODIFIER { 0x8000 };
    static constexpr std::string_view anymodifier_flag_name { "AnyModifier" };
};

// SETofBUTMASK named in protocol, but does not appear in core encoding
// struct [[gnu::packed]] SETofBUTMASK : public SETofKEYBUTMASK {
//     // must be zeroed in SETofBUTMASK values
//     static constexpr uint16_t ZERO_BITS { 0xE0FF };
// };

struct [[gnu::packed]] CHAR2B : public impl::Struct {
    CARD8 byte1, byte2;
};

// using STRING8  = LISTofCARD8
// using STRING16 = LISTofCHAR2B

struct [[gnu::packed]] POINT : public impl::Struct {
    INT16 x, y;
};

struct [[gnu::packed]] RECTANGLE : public impl::Struct {
    INT16  x, y;
    CARD16 width, height;
};

struct [[gnu::packed]] ARC : public impl::Struct {
    INT16  x, y;
    CARD16 width, height;
    INT16  angle1, angle2;
};

struct HOST : public impl::StructWithSuffixes {
    struct [[gnu::packed]] Header {
        CARD8  family;
    private:
        CARD8  _unused;
    public:
        CARD16 address_len;  // length of address in bytes
    };
    // followed by LISTofBYTE address pad(address_len)B

    inline static const std::vector< std::string_view >& family_names {
        protocol::enum_names::host_family };
};

struct STR : public impl::StructWithSuffixes {
    struct [[gnu::packed]] Header {
        uint8_t name_len;  // length of name in bytes
    };
    // followed by STRING8 name pad(name_len)B
};

}  // namespace protocol


#endif  // PROTOCOL_COMMON_TYPES_HPP
