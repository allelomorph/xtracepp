#ifndef SOCKETBUFFER_HPP
#define SOCKETBUFFER_HPP

#include <vector>
#include <type_traits>  // is_same_v

#include <cstdint>

#include <unistd.h>     // ssize_t


// TBD can this be done more elegantly/idiomatically with std::streambuf
//   or std::stringbuf? For example:
//   - https://gist.github.com/amezin/c4bfc0dd11dd200c13b3
//   discussion:
//   - https://stackoverflow.com/a/8116698
//   - https://web.archive.org/web/20240223211016/http://wordaligned.org/articles/cpp-streambufs

class SocketBuffer {
private:
    static constexpr int _BLOCK_SZ      { 2048 };
    static constexpr int _MSG_NONE      { 0 };

    int                  _block_ct      { 1 };
    std::vector<uint8_t> _buffer;

    static_assert( std::is_same_v<
                   decltype( _buffer )::size_type, size_t > );
    size_t               _bytes_stored  {};

public:
    SocketBuffer() : _buffer( _BLOCK_SZ ) {}

    size_t write( const int sockfd );
    size_t read( const int sockfd );

    inline uint8_t* buffer() {
        return _buffer.data();
    }

    inline size_t size() {
        return _bytes_stored;
    }

    inline bool empty() {
        return ( _bytes_stored == 0 );
    }

    inline void clear() {
        _bytes_stored = 0;
    }
};


#endif  // SOCKETBUFFER_HPP
