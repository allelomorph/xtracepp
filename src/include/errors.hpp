#ifndef ERRORS_HPP
#define ERRORS_HPP

/**
 * @file errors.hpp
 */

#include <system_error>  // error_category
#include <string>


namespace errors {

namespace system {

/**
 * @brief Creates a more verbose version of `std::system_error::what()` for C
 *   stdlib `errno`.
 * @param msg prefix to appear at start of message
 * @return error message in format of "errno: {errno name}: {strerror(errno)}"
 *   with prefix of "{msg}: " if msg is not empty
 */
std::string
message( const std::string& msg = {} );

/**
 * @brief Creates a more verbose `std::system_error` for C stdlib `errno`.
 * @param msg prefix to appear at start of exception message
 * @return `std::system_error` with message in format of
 *   [errors::system::message](#errors::system::message)
 */
const std::system_error
exception( const std::string& msg = {} );

}  // namespace errors::system

namespace getaddrinfo {

namespace detail {

/**
 * @brief Child specialization of `std::error_category` for use with
 *   `getaddrinfo(3)` return values rather than C stdlib `errno`.
 * @note [Example] of this pattern of error customization
 *   [Example]: https://breese.github.io/2017/05/12/customizing-error-codes.html
 */
struct Category final : public std::error_category {
    /**
     * @brief Overrides `std::error_category::name()` to return "getaddrinfo".
     * @return "getaddrinfo"
     */
    const char*
    name() const noexcept final;
    /**
     * @brief Overrides `std::error_category::message()` to return the result of
     *   `gai_strerror(3)`.
     * @param gai_ret return value of `getaddrinfo(3)`
     * @return error message in format of `gai_strerror(3)`
     */
    std::string
    message( const int gai_ret ) const final;
};

}  // namespace errors::getaddrinfo::detail

/**
 * @brief Creates a more verbose version of `std::system_error::what()` for
 *   `getaddrinfo(3)` return values.
 * @param gai_ret return value of `getaddrinfo(3)`
 * @param msg prefix to appear at start of message
 * @return error message in format of
 *   "getaddrinfo: {getaddrinfo error name}: {gai_strerror(gai_ret)}" with
 *   prefix of "{msg}: " if msg is not empty
 */
std::string
message( const int gai_ret, const std::string& msg = {} );

/**
 * @brief Creates a more verbose `std::system_error` for `getaddrinfo(3)` return
 *   values.
 * @param gai_ret return value of `getaddrinfo(3)`
 * @param msg prefix to appear at start of exception message
 * @return `std::system_error` with message in format of
 *   [errors::getaddrinfo::message](#errors::getaddrinfo::message)
 */
const std::system_error
exception( const int gai_ret, const std::string& msg = {} );

}  // namespace errors::getaddrinfo

}  // namespace errors


#endif  // ERRORS_HPP
