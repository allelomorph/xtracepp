#ifndef PROTOCOL_MESSAGE_HPP
#define PROTOCOL_MESSAGE_HPP

/**
 * @file Message.hpp
 */


namespace protocol {

/**
 * @brief Interface class providing shared data [padding] alignment for
 *   [Request](#requests::Request), [Reply](#requests::Reply),
 *   [Event](#events::Event), and [Error](#errors::Error).
 * [padding]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Syntactic_Conventions_b
 */
struct Message {
    /**
     * @brief Alignment size in bytes; used to interpret values like:
     *   [following_aligned_units](#connection_setup::InitResponse::Header::following_aligned_units),
     *   [tl_aligned_units](#requests::Request::Length::tl_aligned_units),
     *   [extra_aligned_units](#requests::Reply::Header::extra_aligned_units).
     */
    static constexpr uint32_t ALIGN { 4 };
    virtual ~Message() = 0;
};

}  // namespace protocol


#endif  // PROTOCOL_MESSAGE_HPP
