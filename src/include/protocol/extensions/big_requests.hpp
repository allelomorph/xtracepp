#ifndef PROTOCOL_EXTENSIONS_BIG_REQUESTS_HPP
#define PROTOCOL_EXTENSIONS_BIG_REQUESTS_HPP

/**
 * @file big_requests.hpp
 * @see [X protocol extension "BIG-REQUESTS"](https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html)
 */

#include <string_view>

#include <cstdint>

#include "big_requests/requests.hpp"


namespace protocol {

namespace extensions {

namespace big_requests {

/** @brief Protocol [defined] extension name.
 *  [defined]: https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html#Encoding */
inline constexpr std::string_view name { "BIG-REQUESTS" };

/** @brief Once BIG-REQUESTS is enabled, requests that have a
 *  [tl_aligned_units](#protocol::requests::Request::Header::tl_aligned_units)
 *  equal to this will have an extra 4B data field between the
 *  [Header](#protocol::requests::Request::Header) and
 *  [Encoding](#protocol::requests::Request::Encoding) which serves as
 *  `tl_aligned_units` instead. */
static constexpr uint16_t EXTENDED_LENGTH_FLAG {};

}  // namespace big_requests

}  // namespace extensions

}  // namespace protocol


#endif  // PROTOCOL_EXTENSIONS_BIG_REQUESTS_HPP
