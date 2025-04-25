#ifndef SOCKETBUFFER_HPP
#define SOCKETBUFFER_HPP


#include <vector>

#include <cstdint>

#include <unistd.h>  // ssize_t

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
    std::vector<uint8_t> _buffer        { std::vector<uint8_t>( _BLOCK_SZ ) };
    ssize_t              _tl_bytes_used {};

public:
    ssize_t write(const int sockfd);
    ssize_t read(const int sockfd);

    inline uint8_t* buffer() {
        return _buffer.data();
    }

    inline size_t size() {
        return _tl_bytes_used;
    }

    inline bool empty() {
        return (_tl_bytes_used == 0);
    }
};


#endif  // SOCKETBUFFER_HPP
