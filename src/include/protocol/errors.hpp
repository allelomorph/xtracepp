#ifndef ERRORS_HPP
#define ERRORS_HPP


#include <cstdint>
#include <array>

#include "protocol/common_types.hpp"


// https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Errors

/*
In general, when a request terminates with an error, the request has no side
effects (that is, there is no partial execution). The only requests for which
this is not true are ChangeWindowAttributes, ChangeGC, PolyText8, PolyText16,
FreeColors, StoreColors and ChangeKeyboardControl.

The following error codes result from various requests as follows:

Error	Description
Access	An attempt is made to grab a key/button combination already grabbed by
        another client. An attempt is made to free a colormap entry not
        allocated by the client or to free an entry in a colormap that was
        created with all entries writable. An attempt is made to store into a
        read-only or an unallocated colormap entry. An attempt is made to modify
        the access control list from other than the local host (or otherwise
        authorized client). An attempt is made to select an event type that only
        one client can select at a time when another client has already selected
        it.
Alloc	The server failed to allocate the requested resource. Note that the
        explicit listing of Alloc errors in request only covers allocation
        errors at a very coarse level and is not intended to cover all cases of
        a server running out of allocation space in the middle of service. The
        semantics when a server runs out of allocation space are left
        unspecified, but a server may generate an Alloc error on any request for
        this reason, and clients should be prepared to receive such errors and
        handle or discard them.
Atom	        A value for an ATOM argument does not name a defined ATOM.
Colormap	A value for a COLORMAP argument does not name a defined COLORMAP.
Cursor	        A value for a CURSOR argument does not name a defined CURSOR.
Drawable	A value for a DRAWABLE argument does not name a defined WINDOW
                or PIXMAP.
Font	        A value for a FONT argument does not name a defined FONT. A
                value for a FONTABLE argument does not name a defined FONT or a
                defined GCONTEXT.
GContext	A value for a GCONTEXT argument does not name a defined GCONTEXT.
IDChoice	The value chosen for a resource identifier either is not
                included in the range assigned to the client or is already in
                use.
Implementation	The server does not implement some aspect of the request. A
                server that generates this error for a core request is deficient.
                As such, this error is not listed for any of the requests, but
                clients should be prepared to receive such errors and handle or
                discard them.
Length	The length of a request is shorter or longer than that required to
        minimally contain the arguments. The length of a request exceeds the
        maximum length accepted by the server.
Match	An InputOnly window is used as a DRAWABLE. In a graphics request, the
        GCONTEXT argument does not have the same root and depth as the
        destination DRAWABLE argument. Some argument (or pair of arguments) has
        the correct type and range, but it fails to match in some other way
        required by the request.
Name	A font or color of the specified name does not exist.
Pixmap	A value for a PIXMAP argument does not name a defined PIXMAP.
Request	The major or minor opcode does not specify a valid request.
Value	Some numeric value falls outside the range of values accepted by the
        request. Unless a specific range is specified for an argument, the full
        range defined by the argument's type is accepted. Any argument defined
        as a set of alternatives typically can generate this error (due to the
        encoding).
Window	A value for a WINDOW argument does not name a defined WINDOW.


Note: The Atom, Colormap, Cursor, Drawable, Font, GContext, Pixmap and Window
    errors are also used when the argument type is extended by union with a set
    of fixed alternatives, for example, <WINDOW or PointerRoot or None>.
 */

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

constexpr int ERROR_CT { 17 };

// by Error::code
constexpr std::array<std::string_view, ERROR_CT + 1> names {
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

struct __attribute__((packed)) Error {
public:
    uint8_t prefix;  // always 0
    uint8_t code;
    CARD16  sequence_number;
    union {
        uint32_t _unused1;          // Request(1) Match(8) Access(10) Alloc(11) Name(15) Length(16) Implementation(17)
        uint32_t bad_value;       // Value(2)
        CARD32   bad_resource_id; // Window(3) Pixmap(4) Cursor(6) Font(7) Drawable(9) Colormap(12) GContext(13) IDChoice(14)
        CARD32   bad_atom_id;     // Atom(5)
    };
    CARD16 minor_opcode;
    CARD8  major_opcode;
private:
    uint8_t _unused2[21];
};

// // by Error::code
// constexpr std::array<std::string_view, ERROR_CT + 1> causes {
//     //  0
//     "",
//     //  1 Request
//     "- the major or minor opcode does not specify a valid request",
//     //  2 Value
//     "- some numeric value falls outside the range of values accepted by the request",
//     /* Unless a specific range is specified for an argument, the full
//         range defined by the argument's type is accepted. Any argument defined
//         as a set of alternatives typically can generate this error (due to the
//         encoding).
//     */
//     //  3 Window
//     "- a value for a WINDOW argument does not name a defined WINDOW",
//     //  4 Pixmap
//     "- a value for a PIXMAP argument does not name a defined PIXMAP",
//     //  5 Atom
//     "- a value for an ATOM argument does not name a defined ATOM",
//     //  6 Cursor
//     "- a value for a CURSOR argument does not name a defined CURSOR",
//     //  7 Font
//     R"(- a value for a FONT argument does not name a defined FONT
// - a value for a FONTABLE argument does not name a defined FONT or a defined GCONTEXT)",
//     //  8 Match
//     "- an InputOnly window is used as a DRAWABLE"
//     /*In a graphics request, the
//         GCONTEXT argument does not have the same root and depth as the
//         destination DRAWABLE argument. Some argument (or pair of arguments) has
//         the correct type and range, but it fails to match in some other way
//         required by the request.
//     */
//     //  9 Drawable
//     "- a value for a DRAWABLE argument does not name a defined WINDOW or PIXMAP",
//     // 10 Access
//     R"(- an attempt was made to grab a key/button combination already grabbed by
//   another client
// - an attempt was made to free a colormap entry not allocated by the client or to
//   free an entry in a colormap that was created with all entries writable
// - an attempt was made to store into a read-only or an unallocated colormap entry
// - an attempt was made to modify the access control list from other than the
//   local host (or otherwise authorized client)
// - an attempt was made to select an event type that only one client can select at
// a time when another client has already selected it)",
//     // 11 Alloc
//     "- the server failed to allocate the requested resource",
//     /* Note that the explicit listing of Alloc errors in request only covers allocation
//        errors at a very coarse level and is not intended to cover all cases of
//        a server running out of allocation space in the middle of service. The
//        semantics when a server runs out of allocation space are left
//        unspecified, but a server may generate an Alloc error on any request for
//        this reason, and clients should be prepared to receive such errors and
//        handle or discard them.
//     */
//     // 12 Colormap
//     "- a value for a COLORMAP argument does not name a defined COLORMAP",
//     // 13 GContext
//     "- a value for a GCONTEXT argument does not name a defined GCONTEXT",
//     // 14 IDChoice
//     R"(- the value chosen for a resource identifier either is not included in the
//   range assigned to the client or is already in use)",
//     // 15 Name
//     "- a font or color of the specified name does not exist",
//     // 16 Length
//     R"(- the length of the request was shorter or longer than that required to
//   minimally contain the arguments
// - the length of the request exceeds the maximum length accepted by the server)",
//     // 17 Implementation
//     "- the server does not implement some aspect of the request"
//     /* A server that generates this error for a core request is deficient. As
//        such, this error is not listed for any of the requests, but clients
//        should be prepared to receive such errors and handle or discard them.
//     */
// };

}  // namespace errors

}  // namespace protocol


#endif  // ERRORS_HPP
