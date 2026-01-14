#ifndef PROTOCOL_ERRORS_HPP
#define PROTOCOL_ERRORS_HPP

/**
 * @file errors.hpp
 * @see [X protocol errors](https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Errors)
 */

#include <cstdint>
#include <vector>

#include "protocol/Response.hpp"
#include "protocol/common_types.hpp"


namespace protocol {

namespace errors {

namespace codes {

/**
 * @brief Constants representing all core error [codes].
 * [codes]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
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
/** @brief Highest core error [code](#Codes). */
inline constexpr
int MIN { REQUEST };
/** @brief Lowest core error [code](#Codes). */
inline constexpr
int MAX { IMPLEMENTATION };

}  // namespace codes

// TBD add pure virtual dtor
/**
 * @brief Interface class providing default [encoding] for protocol errors.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Error : public Response {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Identifies message as error; should always equal [ERROR](#ERROR). */
        uint8_t error;
        /** @brief Unique error type identifier. */
        uint8_t code;
        /** @brief [Serial number on current connection](#Connection::sequence)
         *   of relevant request. */
        CARD16  sequence_num;
    };
    /**
     * @brief Complete fixed encoding, including [Header](#Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header   header;
    private:
        /** @brief Ignored bytes. */
        uint32_t _unused1;
    public:
        /** @brief Major opcode of relavant request. */
        CARD16   minor_opcode;
        /** @brief Minor opcode of relavant request. */
        CARD8    major_opcode;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused2[21];
    };
    /** @brief Identifies message as error when found in
     *   [Header::error](#Header::error). */
    static constexpr int ERROR { Response::ERROR_PREFIX };
    /** @brief Expected size in bytes of [Encoding](#Encoding). */
    static constexpr uint32_t ENCODING_SZ { 32 };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

namespace impl {

// TBD add pure virtual dtor
/**
 * @brief Represents shared default [encoding] of errors.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct SimpleError : public Error {};
// TBD add pure virtual dtor
/**
 * @brief Represents shared [encoding] of XID/resource ID errors.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct ResourceIdError : public Error {
    /**
     * @brief Complete fixed encoding, including [Error::Header](#Error::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header   header;
        /** @brief Value of resource ID that caused error. */
        CARD32   bad_resource_id;
        /** @brief Major opcode of relavant request. */
        CARD16   minor_opcode;
        /** @brief Minor opcode of relavant request. */
        CARD8    major_opcode;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused2[21];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};

}  // namespace impl

/**
 * @brief Represents X11 %Request error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Request        : public impl::SimpleError {};
/**
 * @brief Represents X11 %Value error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Value : public Error {
    /**
     * @brief Complete fixed encoding, including [Error::Header](#Error::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header   header;
        /** @brief Value that caused error. */
        uint32_t bad_value;
        /** @brief Major opcode of relavant request. */
        CARD16   minor_opcode;
        /** @brief Minor opcode of relavant request. */
        CARD8    major_opcode;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused[21];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %Window error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Window         : public impl::ResourceIdError {};
/**
 * @brief Represents X11 %Pixmap error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Pixmap         : public impl::ResourceIdError {};
/**
 * @brief Represents X11 %Atom error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Atom : public Error {
    /**
     * @brief Complete fixed encoding, including [Error::Header](#Error::Header).
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Included prefix. */
        Header   header;
        /** @brief Value of [ATOM](#protocol::ATOM) that caused error. */
        CARD32   bad_atom_id;
        /** @brief Major opcode of relavant request. */
        CARD16   minor_opcode;
        /** @brief Minor opcode of relavant request. */
        CARD8    major_opcode;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused2[21];
    };
    static_assert( sizeof( Encoding ) == ENCODING_SZ );
};
/**
 * @brief Represents X11 %Cursor error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Cursor         : public impl::ResourceIdError {};
/**
 * @brief Represents X11 %Font error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Font           : public impl::ResourceIdError {};
/**
 * @brief Represents X11 %Match error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Match          : public impl::SimpleError {};
/**
 * @brief Represents X11 %Drawable error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Drawable       : public impl::ResourceIdError {};
/**
 * @brief Represents X11 %Access error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Access         : public impl::SimpleError {};
/**
 * @brief Represents X11 %Alloc error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Alloc          : public impl::SimpleError {};
/**
 * @brief Represents X11 %Colormap error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Colormap       : public impl::ResourceIdError {};
/**
 * @brief Represents X11 %GContext error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct GContext       : public impl::ResourceIdError {};
/**
 * @brief Represents X11 %IdChoice error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct IdChoice       : public impl::ResourceIdError {};
/**
 * @brief Represents X11 %Name error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Name           : public impl::SimpleError {};
/**
 * @brief Represents X11 %Length error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Length         : public impl::SimpleError {};
/**
 * @brief Represents X11 %Implementation error [encoding].
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
struct Implementation : public impl::SimpleError {};
// TBD move closer to Codes
/**
 * @brief Name strings for all core error [codes].
 * [codes]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Errors
 */
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
