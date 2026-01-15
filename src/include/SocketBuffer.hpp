#ifndef SOCKETBUFFER_HPP
#define SOCKETBUFFER_HPP

/**
 * @file SocketBuffer.hpp
 */

#include <vector>
#include <limits>
#include <type_traits>  // is_same_v

#include <cstdint>
#include <cassert>

#include <unistd.h>     // ssize_t


/**
 * @brief Buffers read/write operations between two sockets into user-accessible
 *   byte array.
 * @note This [example] shows the possiblilty of doing this more idiomatically
 *   with `std::streambuf` or `std::stringbuf`. [Another example].
 *
 * [example]: https://gist.github.com/amezin/c4bfc0dd11dd200c13b3
 * [Another example]: https://stackoverflow.com/a/8116698
 */
class SocketBuffer {
private:
    /**
     * @brief Buffer allocation block size in bytes.
     */
    static constexpr size_t _BLOCK_SZ { 2048 };
    /**
     * @brief Indicates a read of all bytes available from socket when passed
     *   as second param to [read](#read).
     */
    static constexpr size_t _READ_ALL { std::numeric_limits< size_t >::max() };
    /**
     * @brief Indicates no MSG_* flags used with `send(2)`.
     */
    static constexpr int _MSG_NONE { 0 };
    /**
     * @brief User-inspectable data storage.
     */
    std::vector< uint8_t > _buffer;
    static_assert( std::is_same_v<
                   decltype( _buffer )::size_type, size_t > );
    /**
     * @brief Total bytes read/loaded since last [clear](#clear).
     */
    size_t _bytes_read    {};
    /**
     * @brief Total bytes written/unloaded since last [clear](#clear).
     */
    size_t _bytes_written {};
    /**
     * @brief Capacity for bytes to read/load without resize.
     */
    size_t _bytes_available { _BLOCK_SZ };

public:
    SocketBuffer() : _buffer( _BLOCK_SZ ), _bytes_available( _buffer.size() ) {
        assert( !_buffer.empty() ); }
    /**
     * @brief Writes bytes from buffer to `sockfd`.
     * @param sockfd fd to `send(2)` bytes
     * @param bytes_to_write bytes expected to write
     * @return bytes written
     */
    size_t write( const int sockfd,
                  const size_t bytes_to_write );
    /**
     * @brief Writes all bytes from buffer to `sockfd`.
     * @param sockfd fd to `send(2)` bytes
     * @return bytes written
     */
    size_t write( const int sockfd );
    /**
     * @brief Discard bytes from buffer without [write](#write).
     * @param output destination data pointer
     * @param bytes_to_unload bytes to discard
     * @return bytes discarded
     */
    size_t unload( void* output,
                   const size_t bytes_to_unload );
    /**
     * @brief Discard all bytes from buffer without [write](#write).
     * @return bytes discarded
     */
    size_t unload( const size_t bytes_to_unload );
    /**
     * @brief Read bytes from `sockfd` into buffer.
     * @param sockfd fd from which to `recv(2)` bytes
     * @param bytes_to_read bytes expected to read
     * @return bytes read
     */
    size_t read( const int sockfd,
                 const size_t bytes_to_read );
    /**
     * @brief Read all available bytes from `sockfd` into buffer.
     * @param sockfd fd from which to `recv(2)` bytes
     * @return bytes read
     */
    size_t read( const int sockfd );
    /**
     * @brief Directly write bytes to buffer, without [read](#read).
     * @param input source data pointer
     * @param bytes_to_load bytes to write
     * @return bytes written
     */
    size_t load( const void* input,
                 const size_t bytes_to_load );
    /**
     * @brief Get pointer to start of bytes read/loaded but not yet
     *   written/unloaded, or start of buffer if empty.
     * @return pointer to offset in buffer
     */
    inline uint8_t* data() {
        return _buffer.data() + _bytes_written;
    }
    /**
     * @brief Get number of bytes read/loaded but not yet written/unloaded.
     * @return pointer to offset in buffer
     */
    inline size_t size() {
        return _bytes_read - _bytes_written;
    }
    /**
     * @brief Detects whether there are any bytes to write/unload.
     * @return whether there are any bytes to write/unload
     */
    inline bool empty() {
        return ( _bytes_read == 0 );
    }
    /**
     * @brief Resets buffer to empty, but maintains current buffer size.
     */
    inline void clear() {
        _bytes_read    = 0;
        _bytes_written = 0;
        _bytes_available = _buffer.size();
    }
    /**
     * @brief Returns amount of bytes available for read/load.
     * @return bytes available for read/load
     */
    inline size_t capacity() {
        return _bytes_available;
    }
};


#endif  // SOCKETBUFFER_HPP
