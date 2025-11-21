#ifndef BUFFERHEXDUMP_HPP
#define BUFFERHEXDUMP_HPP

/**
 * @file bufferHexDump.hpp
 */

#include <string>

#include <cstdint>


/**
 * @brief Debug tool to format buffers in the style of `hexdump(1)` with `-C`.
 * @param data buffer to be formatted
 * @param sz max bytes readable from `data`
 * @return buffer formatted in style of `hexdump(1)` with `-C`
 */
std::string
bufferHexDump( const uint8_t* data, const size_t sz );


#endif  // BUFFERHEXDUMP_HPP
