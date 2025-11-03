#ifndef SOCKETBUFFER_HPP
#define SOCKETBUFFER_HPP

#include <vector>
#include <limits>
#include <type_traits>  // is_same_v

#include <cstdint>
#include <cassert>

#include <unistd.h>     // ssize_t


// TBD can this be done more elegantly/idiomatically with std::streambuf
//   or std::stringbuf? For example:
//   - https://gist.github.com/amezin/c4bfc0dd11dd200c13b3
//   discussion:
//   - https://stackoverflow.com/a/8116698
//   - https://web.archive.org/web/20240223211016/http://wordaligned.org/articles/cpp-streambufs

class SocketBuffer {
private:
    static constexpr size_t _BLOCK_SZ { 2048 };
    static constexpr size_t _READ_ALL { std::numeric_limits< size_t >::max() };

    static constexpr int _MSG_NONE { 0 };

    std::vector<uint8_t> _buffer;
    static_assert( std::is_same_v<
                   decltype( _buffer )::size_type, size_t > );
    size_t               _bytes_read    {};
    size_t               _bytes_written {};
    // _buffer.size() - _bytes_read
    size_t               _bytes_available { _BLOCK_SZ };

public:
    SocketBuffer() : _buffer( _BLOCK_SZ ), _bytes_available( _buffer.size() ) {
        assert( !_buffer.empty() ); }
    // TBD needed for passing to _connections.emplace(): buffer is copied and ptr needs to be updated
    //SocketBuffer( const SocketBuffer& other ) { assert( _buffer.size() ); }

    size_t write( const int sockfd,
                  const size_t bytes_to_write );
    size_t write( const int sockfd );

    size_t unload( void* output,
                   const size_t bytes_to_unload );
    size_t unload( const size_t bytes_to_unload );

    size_t read( const int sockfd,
                 const size_t bytes_to_read );
    size_t read( const int sockfd );

    size_t load( const void* input,
                 const size_t bytes_to_load );

    inline uint8_t* data() {
        return _buffer.data() + _bytes_written;
    }

    inline size_t size() {
        return _bytes_read - _bytes_written;
    }

    inline bool empty() {
        return ( _bytes_read == 0 );
    }

    inline void clear() {
        _bytes_read    = 0;
        _bytes_written = 0;
        _bytes_available = _buffer.size();
    }

    inline size_t capacity() {
        return _bytes_available;
    }
};


#endif  // SOCKETBUFFER_HPP
