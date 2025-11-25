#ifndef PROTOCOL_COMMON_TYPES_HPP
#define PROTOCOL_COMMON_TYPES_HPP

/**
 * @file common_types.hpp
 * @see [X protocol](https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Common_Types)
 */

#include <cstdint>
#include <vector>
#include <string_view>

#include "protocol/enum_names.hpp"  // host_family


namespace protocol {

/** @brief X11 base integer type alias. */
using INT8   = int8_t;
/** @brief X11 base integer type alias. */
using INT16  = int16_t;
/** @brief X11 base integer type alias. */
using INT32  = int32_t;
/** @brief X11 base integer type alias. */
using CARD8  = uint8_t;
/** @brief X11 base integer type alias. */
using CARD16 = uint16_t;
/** @brief X11 base integer type alias. */
using CARD32 = uint32_t;
/** @brief X11 base integer type alias. */
using BYTE   = uint8_t;
// No BITMASK alias as it can be 2 or 4 bytes in core encoding
/**
 * @brief LISTofVALUE member alias.
 * @note `VALUE` is always encoded as [4B] despite containing values of many types.
 * [4B]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Common_Types
 */
using VALUE  = CARD32;

namespace impl {

/**
 * @brief Parent class to allow template filtering of protocol types via
 *   inheritance; represents protocol integer types.
 */
struct [[gnu::packed]] Integer {};
/**
 * @brief Parent class to allow template filtering of protocol types via
 *   inheritance; represents protocol bitmask types.
 */
struct [[gnu::packed]] Bitmask : public Integer {};
/**
 * @brief Parent class to allow template filtering of protocol types via
 *   inheritance; represents protocol X ID/resource ID types.
 */
struct [[gnu::packed]] ResourceId : public Integer {
    /**
     * @brief Actual integer value independent of typing.
     */
    CARD32 data;
    /**
     * @brief Bitmask for all bits that must be always be zero.
     */
    static constexpr uint32_t ZERO_BITS { 0xE0000000 };
};
/**
 * @brief Parent class to allow template filtering of protocol types via
 *   inheritance; represents protocol struct types.
 */
struct [[gnu::packed]] Struct {};
/**
 * @brief Parent class to allow template filtering of protocol types via
 *   inheritance; represents protocol struct types.
 */
struct [[gnu::packed]] StructWithSuffixes : public Struct {};


}  // namespace impl

/**
 * @brief Typed integer representing X11 WINDOW.
 */
struct [[gnu::packed]] WINDOW : public impl::ResourceId {};
/**
 * @brief Typed integer representing X11 PIXMAP.
 */
struct [[gnu::packed]] PIXMAP : public impl::ResourceId {};
/**
 * @brief Typed integer representing X11 CURSOR.
 */
struct [[gnu::packed]] CURSOR : public impl::ResourceId {};
/**
 * @brief Typed integer representing X11 FONT.
 */
struct [[gnu::packed]] FONT : public impl::ResourceId {};
/**
 * @brief Typed integer representing X11 GCONTEXT.
 */
struct [[gnu::packed]] GCONTEXT : public impl::ResourceId {};
/**
 * @brief Typed integer representing X11 COLORMAP.
 */
struct [[gnu::packed]] COLORMAP : public impl::ResourceId {};
/**
 * @brief Union representing X11 DRAWABLE.
 */
union [[gnu::packed]] DRAWABLE {
    WINDOW window;
    PIXMAP pixmap;
};
/**
 * @brief Union representing X11 FONTABLE.
 */
union [[gnu::packed]] FONTABLE {
    FONT     font;
    GCONTEXT gcontext;
};
/**
 * @brief Typed integer representing X11 ATOM.
 */
struct [[gnu::packed]] ATOM : public impl::ResourceId {};
/**
 * @brief Typed integer representing X11 VISUALID.
 */
struct [[gnu::packed]] VISUALID : public impl::ResourceId {};
/**
 * @brief Typed integer representing X11 TIMESTAMP.
 */
struct [[gnu::packed]] TIMESTAMP : public impl::Integer {
    /**
     * @brief Actual integer value independent of typing.
     */
    CARD32 data;
};
/**
 * @brief Typed integer representing X11 BITGRAVITY.
 */
struct [[gnu::packed]] BITGRAVITY : public impl::Integer {
    /**
     * @brief Actual integer value independent of typing.
     */
    CARD8 data;
    /**
     * @brief Enum name strings.
     */
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
/**
 * @brief Typed integer representing X11 WINGRAVITY.
 */
struct [[gnu::packed]] WINGRAVITY : public impl::Integer {
    /**
     * @brief Actual integer value independent of typing.
     */
    CARD8 data;
    /**
     * @brief Enum name strings.
     */
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
/**
 * @brief Typed integer representing X11 BOOL.
 */
struct [[gnu::packed]] BOOL : public impl::Integer {
    /**
     * @brief Actual integer value independent of typing.
     */
    CARD8 data;
    /**
     * @brief Enum name strings.
     */
    inline static const
    std::vector< std::string_view > enum_names {
        "False",  //  0
        "True"    //  1
    };
};

namespace impl {

/**
 * @brief Parent class to allow template filtering of protocol types via
 *   inheritance; represents protocol SETofEVENT types.
 */
struct [[gnu::packed]] SETofEVENTBase : public impl::Bitmask {
    /**
     * @brief Flag name strings.
     */
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

/**
 * @brief Typed integer bitmask representing X11 SETofEVENT.
 */
struct [[gnu::packed]] SETofEVENT : public impl::SETofEVENTBase {
    /**
     * @brief Actual integer value independent of typing.
     */
    CARD32 data;
    /**
     * @brief Bitmask for all bits that must be always be zero.
     */
    static constexpr uint32_t ZERO_BITS { 0xFE000000 };
};
/**
 * @brief Typed integer bitmask representing X11 SETofPOINTEREVENT.
 */
struct [[gnu::packed]] SETofPOINTEREVENT : public impl::SETofEVENTBase {
    /**
     * @brief Actual integer value independent of typing.
     */
    CARD16 data;
    /**
     * @brief Bitmask for all bits that must be always be zero.
     * @note Extended to 4B in case of use in `LISTofVALUE`.
     */
    static constexpr uint32_t ZERO_BITS { 0xFFFF8003 };
};
/**
 * @brief Typed integer bitmask representing X11 SETofDEVICEEVENT.
 */
struct [[gnu::packed]] SETofDEVICEEVENT : public impl::SETofEVENTBase {
    /**
     * @brief Actual integer value independent of typing.
     */
    CARD16 data;
    /**
     * @brief Bitmask for all bits that must be always be zero.
     * @note Extended to 4B in case of use in `LISTofVALUE`.
     */
    static constexpr uint32_t ZERO_BITS { 0xFFFFC0B0 };
};
/**
 * @brief Typed integer representing X11 KEYSYM.
 */
struct [[gnu::packed]] KEYSYM : public impl::Integer {
    /**
     * @brief Actual integer value independent of typing.
     */
    CARD32 data;
};
/**
 * @brief Typed integer representing X11 KEYCODE.
 */
struct [[gnu::packed]] KEYCODE : public impl::Integer {
    /**
     * @brief Actual integer value independent of typing.
     */
    CARD8 data;
};
/**
 * @brief Typed integer representing X11 BUTTON.
 */
struct [[gnu::packed]] BUTTON : public impl::Integer {
    /**
     * @brief Actual integer value independent of typing.
     */
    CARD8 data;
};
/**
 * @brief Typed integer bitmask representing X11 SETofKEYBUTMASK.
 */
struct [[gnu::packed]] SETofKEYBUTMASK : public impl::Bitmask {
    /**
     * @brief Actual integer value independent of typing.
     */
    CARD16 data;
    /**
     * @brief Bitmask for all bits that must be always be zero.
     */
    static constexpr uint16_t ZERO_BITS { 0xE000 };
    /**
     * @brief Flag name strings.
     */
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
/**
 * @brief Typed integer bitmask representing X11 SETofKEYMASK.
 */
struct [[gnu::packed]] SETofKEYMASK : public SETofKEYBUTMASK {
    /**
     * @brief Bitmask for all bits that must be always be zero.
     */
    static constexpr uint16_t ZERO_BITS { 0xFF00 };
    /**
     * @brief Special alternate flag value.
     * @warning Requires special handling, as this value overlaps with
     *   [ZERO_BITS](#ZERO_BITS).
     */
    static constexpr uint16_t ANYMODIFIER { 0x8000 };
    /**
     * @brief [ANYMODIFIER](#ANYMODIFIER) flag name string.
     */
    static constexpr std::string_view anymodifier_flag_name { "AnyModifier" };
};

// SETofBUTMASK named in protocol, but does not appear in core encoding
// struct [[gnu::packed]] SETofBUTMASK : public SETofKEYBUTMASK {
//     // must be zeroed in SETofBUTMASK values
//     static constexpr uint16_t ZERO_BITS { 0xE0FF };
// };

/**
 * @brief Encapsulates X11 CHAR2B.
 */
struct [[gnu::packed]] CHAR2B : public impl::Struct {
    /** @brief First byte of encoding. */
    CARD8 byte1;
    /** @brief Second byte of encoding. */
    CARD8 byte2;
};

// using STRING8  = LISTofCARD8
// using STRING16 = LISTofCHAR2B

/**
 * @brief Encapsulates X11 POINT.
 */
struct [[gnu::packed]] POINT : public impl::Struct {
    /** @brief x coordinate of upper-left corner. */
    INT16 x;
    /** @brief y coordinate of upper-left corner. */
    INT16 y;
};
/**
 * @brief Encapsulates X11 RECTANGLE.
 */
struct [[gnu::packed]] RECTANGLE : public impl::Struct {
    /** @brief x coordinate of upper-left corner. */
    INT16 x;
    /** @brief y coordinate of upper-left corner. */
    INT16 y;
    /** @brief Width in pixels. */
    CARD16 width;
    /** @brief Height in pixels. */
    CARD16 height;
};
/**
 * @brief Encapsulates X11 ARC.
 */
struct [[gnu::packed]] ARC : public impl::Struct {
    /** @brief x coordinate of upper-left corner of bounding rectangle. */
    INT16 x;
    /** @brief y coordinate of upper-left corner of bounding rectangle. */
    INT16 y;
    /** @brief Major axis of arc. */
    CARD16 width;
    /** @brief Minor axis of arc. */
    CARD16 height;
    /** @brief Start of the arc relative to the three-o’clock position from the
     *    center, in units of degrees * 64. */
    INT16  angle1;
    /** @brief Path and extent of the arc relative to the start of the arc, in
     *    units of degrees * 64. */
    INT16  angle2;
};
/**
 * @brief Represents X11 HOST.
 */
struct HOST : public impl::StructWithSuffixes {
    /**
     * @brief Fixed encoding of X11 HOST.
     * @note Followed by suffixes:
     *   - `LISTofBYTE address` of
     *     [pad](#X11ProtocolParser::_Alignment::pad)([address_len](#address_len))B
     */
    struct [[gnu::packed]] Header {
        /**
         * @brief X11 `Family*` value (may differ from `socket(2)` `domain` `AF_*` macros).
         */
        CARD8  family;
    private:
        CARD8  _unused;
    public:
        /**
         * @brief Length of suffix `address` in bytes.
         */
        CARD16 address_len;
    };
    /**
     * @brief Enum names for [family](#Header::family).
     */
    inline static const std::vector< std::string_view >& family_names {
        protocol::enum_names::host_family };
};
/**
 * @brief Represents X11 STR.
 */
struct STR : public impl::StructWithSuffixes {
    /**
     * @brief Fixed encoding of X11 STR.
     * @note Followed by suffixes:
     *   - `STRING8 name` of
     *     [pad](#X11ProtocolParser::_Alignment::pad)([name_len](#name_len))B
     */
    struct [[gnu::packed]] Header {
        /**
         * @brief Length of suffix `name` in bytes.
         */
        uint8_t name_len;
    };
};

}  // namespace protocol


#endif  // PROTOCOL_COMMON_TYPES_HPP
