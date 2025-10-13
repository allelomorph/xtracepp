#ifndef PROTOCOL_ERRORS_HPP
#define PROTOCOL_ERRORS_HPP


#include <cstdint>
#include <vector>

#include "protocol/Response.hpp"
#include "protocol/common_types.hpp"


// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Errors
// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors

namespace protocol {

namespace errors {

namespace codes {

enum Codes {
    REQUEST           = 1,
    VALUE,           // 2
    WINDOW,          // 3
    PIXMAP,          // 4
    ATOM,            // 5
    CURSOR,          // 6
    FONT,            // 7
    MATCH,           // 8
    DRAWABLE,        // 9
    ACCESS,          // 10
    ALLOC,           // 11
    COLORMAP,        // 12
    GCONTEXT,        // 13
    IDCHOICE,        // 14
    NAME,            // 15
    LENGTH,          // 16
    IMPLEMENTATION   // 17
};

inline constexpr
int MIN { REQUEST };

inline constexpr
int MAX { IMPLEMENTATION };

}  // namespace codes

struct Error : public Response {
    struct [[gnu::packed]] Header {
        uint8_t error;
        uint8_t code;
        CARD16  sequence_num;
    };
    struct [[gnu::packed]] Encoding {
        Header   header;
        union [[gnu::packed]] {
            uint32_t _unused1;
            uint32_t bad_value;
            CARD32   bad_resource_id;
            CARD32   bad_atom_id;
        };
        CARD16   minor_opcode;
        CARD8    major_opcode;
    private:
        uint8_t  _unused2[21];
    };

    static constexpr int ERROR { Response::ERROR_PREFIX };
    static constexpr uint32_t ENCODING_SZ { 32 };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

enum class ValueTypeCode {
    UNUSED,
    VALUE,
    RESOURCE_ID,
    ATOM_ID
};
// TBD core errors only, these plus extension errors could all be in map
inline static const std::vector< ValueTypeCode > value_types {
    ValueTypeCode::UNUSED,       // 0
    ValueTypeCode::UNUSED,       // 1  REQUEST
    ValueTypeCode::VALUE,        // 2  VALUE
    ValueTypeCode::RESOURCE_ID,  // 3  WINDOW
    ValueTypeCode::RESOURCE_ID,  // 4  PIXMAP
    ValueTypeCode::ATOM_ID,      // 5  ATOM
    ValueTypeCode::RESOURCE_ID,  // 6  CURSOR
    ValueTypeCode::RESOURCE_ID,  // 7  FONT
    ValueTypeCode::UNUSED,       // 8  MATCH
    ValueTypeCode::RESOURCE_ID,  // 9  DRAWABLE
    ValueTypeCode::UNUSED,       // 10 ACCESS
    ValueTypeCode::UNUSED,       // 11 ALLOC
    ValueTypeCode::RESOURCE_ID,  // 12 COLORMAP
    ValueTypeCode::RESOURCE_ID,  // 13 GCONTEXT
    ValueTypeCode::RESOURCE_ID,  // 14 IDCHOICE
    ValueTypeCode::UNUSED,       // 15 NAME
    ValueTypeCode::UNUSED,       // 16 LENGTH
    ValueTypeCode::UNUSED        // 17 IMPLEMENTATION
};

// by Error::code
inline static const std::vector< std::string_view > names {
    "",                // 0
    "Request",         // 1
    "Value",           // 2
    "Window",          // 3
    "Pixmap",          // 4
    "Atom",            // 5
    "Cursor",          // 6
    "Font",            // 7
    "Match",           // 8
    "Drawable",        // 9
    "Access",          // 10
    "Alloc",           // 11
    "Colormap",        // 12
    "GContext",        // 13
    "IDChoice",        // 14
    "Name",            // 15
    "Length",          // 16
    "Implementation"   // 17
};

}  // namespace errors

}  // namespace protocol


#endif  // PROTOCOL_ERRORS_HPP
