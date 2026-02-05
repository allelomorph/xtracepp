#ifndef PROTOCOL_EXTENSIONS_BIG_REQUESTS_REQUESTS_HPP
#define PROTOCOL_EXTENSIONS_BIG_REQUESTS_REQUESTS_HPP

/**
 * @file requests.hpp
 * @see [X protocol extension "BIG-REQUESTS"](https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html)
 */

#include <cstdint>

#include <array>

#include "protocol/common_types.hpp"
#include "protocol/extensions/requests.hpp"


namespace protocol {

namespace extensions {

namespace big_requests {

namespace requests {

namespace opcodes {

/**
 * @brief Constants representing all BIG-REQUESTS request minor [opcodes].
 * [opcodes]: https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html#Encoding
 */
enum Opcodes {
    BIGREQENABLE
};

/** @brief Lowest request [opcode](#Opcodes). */
inline constexpr int MIN { BIGREQENABLE };
/** @brief Highest request [opcode](#Opcodes). */
inline constexpr int MAX { BIGREQENABLE };

}  // namespace opcodes

/** @brief Request names by opcode. */
inline constexpr
std::array< std::string_view, opcodes::MAX + 1 > names {
    "BigReqEnable"  //   0
};

//using hdr = protocol::extensions::requests::Reply::Header;
/**
 * @brief Represents X11 BIG-REQUESTS extension %BigReqEnable request [encoding].
 * [encoding]: https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html#Encoding
 */
struct BigReqEnable : public protocol::extensions::requests::impl::SimpleRequest {
    /**
     * @brief Represents X11 BIG-REQUESTS extension %BigReqEnable reply [encoding].
     * [encoding]: https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html#Encoding
     */
    struct Reply : public protocol::extensions::requests::Reply {
        /**
         * @brief Fixed encoding, including
         *   [Header](#protocol::requests::Reply::Header).
         * @note Documented [encoding] only implies 2 unused bytes at end, which
         *   would make the total less than the core protocol reply size
         *   [default](#protocol::requests::Reply::DEFAULT_ENCODING_SZ) of 32.
         *   Based on testing, the reply is in fact encoded as 32 bytes.
         * [encoding]: https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html#Encoding
         */
        struct [[gnu::packed]] Encoding {
            /** @brief Included prefix. */
            Header   header;  // `extra_aligned_units` of 0
            /** @brief Protocol name: maximum-request-length; new request max
             *    length, in 4B units. */
            CARD32   maximum_request_length;
        private:
            /** @brief Ignored bytes. */
            [[maybe_unused]]
            uint8_t _unused[20];
        };
        static_assert( sizeof( Encoding ) == DEFAULT_ENCODING_SZ );
    };
};

}  // namespace requests

}  // namespace big_requests

}  // namespace extensions

}  // namespace protocol


#endif  // PROTOCOL_EXTENSIONS_BIG_REQUESTS_REQUESTS_HPP
