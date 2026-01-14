#ifndef PROTOCOL_RESPONSE_HPP
#define PROTOCOL_RESPONSE_HPP

/**
 * @file Response.hpp
 */

#include "protocol/common_types.hpp"


namespace protocol {

/**
 * @brief Interface class providing generic header for
 *   [Error](#protocol::errors::Error), [Reply](#protocol::requests::Reply),
 *   and [Event](#protocol::events::Event).
 */
struct Response {
    /**
     * @brief Generic fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /**
         * @brief First byte of message, used to differentiate response type.
         *   Could be [Error::Header::error](#protocol::errors::Error::Header::error),
         *   [Reply::Header::reply](#protocol::requests::Reply::Header::reply), or an
         *   [event code](#protocol::events::Event::Header::code).
         */
        uint8_t prefix;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused;
    public:
        /**
         * @brief [Serial number on current connection](#Connection::sequence)
         *   of relevant request.
         */
        CARD16  sequence_num;
    };

    /**
     * @brief Constants to interpret [Header::prefix](#Header::prefix) as
     *   [Error::Header::error](#protocol::errors::Error::Header::error) or
     *   [Reply::Header::reply](#protocol::requests::Reply::Header::reply).
     */
    enum {
        ERROR_PREFIX,
        REPLY_PREFIX
    };
    /**
     * @brief Memory alignment size in bytes, for use in making asserts on
     *   encoding sizes.
     */
    static constexpr uint32_t ALIGN { 4 };

    virtual ~Response() = 0;
};

}  // namespace protocol


#endif  // PROTOCOL_RESPONSE_HPP
