#ifndef PROTOCOL_EXTENSIONS_REQUESTS_HPP
#define PROTOCOL_EXTENSIONS_REQUESTS_HPP

/**
 * @file requests.hpp
 * @see [X protocol requests](https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Requests)
 */

#include <cstdint>

#include "protocol/common_types.hpp"
#include "protocol/Response.hpp"
#include "protocol/requests.hpp"


namespace protocol {

namespace extensions {

namespace requests {

/**
 * @brief Interface class providing default [encoding] for protocol extension requests.
 * @note Unlike with [Response](#protocol::Response)-derived classes like Reply,
 *   encoding is segmented into a [prefix](#Request::Prefix), a length, optionally
 *   more fixed encoding, and ending in variable length suffixes. This allows for
 *   support of the [BIG-REQUESTS] extension.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#request_format
 * [BIG-REQUESTS]: https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html
 */
struct Request : public protocol::requests::Request {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Prefix {
        /** @brief Extension major-opcode. */
        uint8_t major_opcode;
        /** @brief Extension minor-opcode. */
        uint8_t minor_opcode;
    };
    virtual ~Request() = 0;
};
/**
 * @brief Interface class providing default [encoding] for protocol request replies.
 * @note Not guaranteed to have a
 *   [default](#protocol::requests::Reply::DEFAULT_ENCODING_SZ) encoding size.
 * [encoding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#reply_format
 */
struct Reply : public protocol::requests::Reply {
    virtual ~Reply() = 0;
};

namespace impl {

// TBD add encoding reference link
/**
 * @brief Interface class representing minimal encoding for X11 extension requests.
 */
struct SimpleRequest : public protocol::extensions::requests::Request {
    /** @brief Total encoding size in bytes (before suffixes). */
    static constexpr size_t BASE_ENCODING_SZ {
        sizeof( Prefix ) + sizeof( Length ) };
    /** @brief Total encoding size in bytes (before suffixes) when using
     *    BIG-REQUESTS encoding. */
    static constexpr size_t BASE_BIG_ENCODING_SZ {
        sizeof( Prefix ) + sizeof( BigLength ) };
    virtual ~SimpleRequest() = 0;
};

}  // namespace impl

}  // namespace requests

}  // namespace extensions

}  // namespace protocol


#endif  // PROTOCOL_EXTENSIONS_REQUESTS_HPP
