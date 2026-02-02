#ifndef SOCKETBUFFER_HPP
#define SOCKETBUFFER_HPP

/**
 * @file SocketBuffer.hpp
 */

#include <vector>
#include <limits>
#include <type_traits>  // is_same_v
#include <utility>      // pair
#include <optional>
#include <string>

#include <cstdint>
#include <cassert>

#include <unistd.h>     // ssize_t


/**
 * @brief Buffers read/write operations between two sockets into user-accessible
 *   byte array.
 *   Message bytes go through four stages:
 *   - reading/loading into buffer (advances #_bytes_read (`br`))
 *   - measuring (#_next_message_sz (`nms`) is set with #setMessageSize)
 *   - parsing (#markMessageParsed adds #_next_message_sz to #_bytes_parsed (`bp`))
 *   - writing/unloading from buffer (advances #_bytes_written (`bw`)
 * ```
 *        <------------size()-----------><--capacity()-->
 *        data()
 *        bw                            br              _buffer.size()
 *        |                             |               |
 * ▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
 *        <--prev nms--><--nms-->
 *        <---------bp---------->
 * ````
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
     * @brief Total bytes marked as write-ready since last [write](#write).
     */
    size_t _bytes_parsed  {};
    /**
     * @brief Total bytes written/unloaded since last [clear](#clear).
     */
    size_t _bytes_written {};
    /**
     * @brief Default for next message size, indicates that it has not been set.
     */
    static constexpr size_t
    _UNKNOWN_SZ { std::numeric_limits< size_t >::max() };
    /**
     * @brief Size of next message to be parsed.
     */
    size_t _next_message_sz { _UNKNOWN_SZ };

public:
    /**
     * @brief Default ctor.
     */
    SocketBuffer() : _buffer( _BLOCK_SZ ) {
        assert( !_buffer.empty() );
    }
    /**
     * @brief Sets next message size.
     */
    inline void setMessageSize( const size_t message_sz ) {
        assert( message_sz != _UNKNOWN_SZ );
        assert( !messageSizeSet() );
        _next_message_sz = message_sz;
    }
    /**
     * @brief Idicates whether next message size has been set.
     * @return whether next message size has been set
     */
    inline bool messageSizeSet() {
        return _next_message_sz != _UNKNOWN_SZ;
    }
    /**
     * @brief Gets next message size.
     * @return next message size
     */
    inline size_t messageSize() {
        return _next_message_sz;
    }
    /**
     * @brief Returns n bytes buffered and marked as parsed.
     * @return n bytes buffered and marked as parsed.
     */
    inline size_t parsed() {
        return _bytes_parsed;
    }
    /**
     * @brief Returns n bytes buffered but not marked as parsed.
     * @return n bytes buffered but not marked as parsed.
     */
    inline size_t unparsed() {
        return size() - _bytes_parsed;
    }
    /**
     * @brief Detects whether last #read recieved a partial message.
     * @return whether last #read recieved a partial message
     */
    inline bool incompleteMessage() {
        return unparsed() > 0 &&
            ( !messageSizeSet() || ( unparsed() < messageSize() ) );
    }
    /**
     * @brief Marks current message as length verified, parsed, and ready
     *   for #write/#unload.
     */
    inline void markMessageParsed() {
        assert( messageSizeSet() );
        assert( ( _bytes_parsed + _next_message_sz ) <= size() );
        _bytes_parsed += _next_message_sz;
        _next_message_sz = _UNKNOWN_SZ;
    }
    /**
     * @brief Detects whether buffer is empty or is still building incomplete
     *   message.
     * @return whether buffer is empty or is still building incomplete
     *   message
     */
    inline bool readReady() {
        return empty() || incompleteMessage();
    }
    /**
     * @brief Detects if complete message(s) are stored in buffer.
     * @return whether complete message(s) are stored in buffer
     */
    inline bool writeReady() {
        return !empty() && _bytes_parsed > 0;
    }
    /**
     * @brief Read bytes from `sockfd` into buffer.
     * @param sockfd fd from which to `recv(2)` bytes
     * @param bytes_to_read n bytes to read
     * @return bytes read
     */
    std::pair< size_t, std::optional< std::string > >
    read( const int sockfd,
          const size_t bytes_to_read );
    /**
     * @brief Read all available bytes from `sockfd` into buffer.
     * @param sockfd fd from which to `recv(2)` bytes
     * @return bytes read
     */
    std::pair< size_t, std::optional< std::string > >
    read( const int sockfd );
    /**
     * @brief Directly read bytes from another buffer, instead of from fd as
     *   with [read](#read).
     * @param input source buffer
     * @param bytes_to_load n bytes to read
     * @return bytes written
     */
    size_t load( const void* input,
                 const size_t bytes_to_load );
    /**
     * @brief Writes bytes from buffer to `sockfd`.
     * @param sockfd fd to `send(2)` bytes
     * @param bytes_to_write n bytes expected to write
     * @return bytes written
     */
    std::pair< size_t, std::optional< std::string > >
    write( const int sockfd,
           const size_t bytes_to_write );
    /**
     * @brief Writes all bytes from buffer to `sockfd`.
     * @param sockfd fd to `send(2)` bytes
     * @return bytes written
     */
    std::pair< size_t, std::optional< std::string > >
    write( const int sockfd );
    /**
     * @brief Directly write bytes to another buffer, instead of to fd as with
     *   [write](#write).
     * @param[out] output destination buffer
     * @param bytes_to_unload n bytes to write
     * @return bytes discarded
     */
    size_t unload( void* output,
                   const size_t bytes_to_unload );
    /**
     * @brief Discard bytes from buffer without [write](#write).
     * @param bytes_to_unload n bytes to unload
     * @return bytes discarded
     */
    size_t unload( const size_t bytes_to_unload );
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
     * @brief Resets buffer to empty, but maintains current storage size.
     */
    inline void clear() {
        assert( parsed() == size() );
        _bytes_read      = 0;
        _next_message_sz = _UNKNOWN_SZ;
        _bytes_parsed    = 0;
        _bytes_written   = 0;
    }
    /**
     * @brief Returns amount of bytes available for read/load.
     * @return bytes available for read/load
     */
    inline size_t capacity() {
        return _buffer.size() - _bytes_read;
    }
};


#endif  // SOCKETBUFFER_HPP
