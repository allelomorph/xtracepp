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
    private:
        uint32_t _unused1;
    public:
        CARD16   minor_opcode;
        CARD8    major_opcode;
    private:
        uint8_t  _unused2[21];
    };

    static constexpr int ERROR { Response::ERROR_PREFIX };
    static constexpr uint32_t ENCODING_SZ { 32 };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

namespace impl {

struct SimpleError : public Error {};

struct ResourceIdError : public Error {
    struct [[gnu::packed]] Encoding {
        Header   header;
        CARD32   bad_resource_id;
        CARD16   minor_opcode;
        CARD8    major_opcode;
    private:
        uint8_t  _unused2[21];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

}  // namespace impl

struct Request        : public impl::SimpleError {};

struct Value : public Error {
    struct [[gnu::packed]] Encoding {
        Header   header;
        uint32_t bad_value;
        CARD16   minor_opcode;
        CARD8    major_opcode;
    private:
        uint8_t  _unused[21];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct Window         : public impl::ResourceIdError {};

struct Pixmap         : public impl::ResourceIdError {};

struct Atom : public Error {
    struct [[gnu::packed]] Encoding {
        Header   header;
        CARD32   bad_atom_id;
        CARD16   minor_opcode;
        CARD8    major_opcode;
    private:
        uint8_t  _unused2[21];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

struct Cursor         : public impl::ResourceIdError {};

struct Font           : public impl::ResourceIdError {};

struct Match          : public impl::SimpleError {};

struct Drawable       : public impl::ResourceIdError {};

struct Access         : public impl::SimpleError {};

struct Alloc          : public impl::SimpleError {};

struct Colormap       : public impl::ResourceIdError {};

struct GContext       : public impl::ResourceIdError {};

struct IdChoice       : public impl::ResourceIdError {};

struct Name           : public impl::SimpleError {};

struct Length         : public impl::SimpleError {};

struct Implementation : public impl::SimpleError {};

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
