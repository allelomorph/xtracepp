#ifndef PROTOCOL_VERSION_HPP
#define PROTOCOL_VERSION_HPP

/**
 * @file version.hpp
 */

namespace protocol {

// Supporting 11.0, release 7.7
/**
 * @brief Supported major version of X protocol encoding.
 */
static constexpr uint8_t MAJOR_VERSION { 11 };
/**
 * @brief Supported minor version of X protocol encoding for
 *   [MAJOR_VERSION](#MAJOR_VERSION).
 */
static constexpr uint8_t MINOR_VERSION { 0 };

}  // namespace protocol


#endif  // PROTOCOL_VERSION_HPP
