#ifndef PROTOCOL_RESPONSE_HPP
#define PROTOCOL_RESPONSE_HPP

/**
 * @file Response.hpp
 */

#include "protocol/Message.hpp"
#include "protocol/common_types.hpp"  // CARD16


namespace protocol {

/**
 * @brief Interface class providing generic header for [Reply](#requests::Reply),
 *   [Event](#events::Event), and [Error](#errors::Error).
 */
struct Response : public Message {
    /**
     * @brief Generic fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /**
         * @brief First byte of message, used to differentiate
         *   [Reply](#requests::Reply) ([REPLY](#requests::Reply::REPLY)) and
         *   [Error](#errors::Error) ([ERROR](#errors::Error::ERROR)) from
         *   [Event](#events::Event)s.
         */
        uint8_t prefix;
    private:
        /** @brief Ignored bytes. */
        [[maybe_unused]]
        uint8_t _unused;
    public:
        /**
         * @brief [Serial number on current connection](#Connection::sequence)
         *   of relevant request.
         */
        CARD16  sequence_num;
    };
    /**
     * @brief Constants to interpret [prefix](#Header::prefix).
     */
    enum { ERROR_PREFIX, REPLY_PREFIX };
    virtual ~Response() = 0;
};

}  // namespace protocol


#endif  // PROTOCOL_RESPONSE_HPP
