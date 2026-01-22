#ifndef PROTOCOL_EXTENSIONS_BIG_REQUESTS_NAME_HPP
#define PROTOCOL_EXTENSIONS_BIG_REQUESTS_NAME_HPP

/**
 * @file name.hpp
 * @see [X protocol extension "BIG-REQUESTS"](https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html)
 */

namespace protocol {

namespace extensions {

namespace big_requests {

/** @brief Protocol [defined] extension name.
 *  [defined]: https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html#Encoding */
inline constexpr std::string_view name { "BIG-REQUESTS" };

}  // namespace big_requests

}  // namespace extensions

}  // namespace protocol


#endif  // PROTOCOL_EXTENSIONS_BIG_REQUESTS_NAME_HPP
