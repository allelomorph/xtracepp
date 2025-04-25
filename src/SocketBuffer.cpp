#include "SocketBuffer.hpp"

#include <string_view>
#include <system_error>  // generic_category

#include <cassert>

#include <sys/socket.h>  // send recv
#include <errno.h>


ssize_t SocketBuffer::write(const int sockfd) {
    std::string_view exception_stem { "SocketBuffer: write: send" };
    ssize_t bytes_written { send(
            sockfd, _buffer.data(), _tl_bytes_used, _MSG_NONE ) };
    if ( bytes_written == -1 ) {
        // TBD include string of errno name
        throw std::system_error(
            errno, std::generic_category(),
            exception_stem.data() );
    }
    assert( bytes_written == _tl_bytes_used );
    return bytes_written;
}

ssize_t SocketBuffer::read(const int sockfd) {
    std::string_view exception_stem { "SocketBuffer: read: recv" };
    ssize_t bytes_read { recv(
            sockfd, _buffer.data(), _buffer.size(), _MSG_NONE ) };
    if ( bytes_read == -1 ) {
        // TBD include string of errno name
        throw std::system_error(
            errno, std::generic_category(),
            exception_stem.data() );
    }
    _tl_bytes_used = bytes_read;
    while ( _tl_bytes_used == _buffer.size() ) {
        _buffer.resize( _buffer.size() + _BLOCK_SZ );
        ++_block_ct;
        bytes_read = recv(
            sockfd, _buffer.data() + _tl_bytes_used, _BLOCK_SZ, _MSG_NONE );
        if ( bytes_read == -1 ) {
            // TBD include string of errno name
            throw std::system_error(
                errno, std::generic_category(),
                exception_stem.data() );
        }
        _tl_bytes_used += bytes_read;
    }
    return _tl_bytes_used;
}
