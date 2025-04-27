#include <string>

#include <cassert>

#include <sys/socket.h>  // send recv
#include <errno.h>

#include "SocketBuffer.hpp"
#include "errors.hpp"


size_t SocketBuffer::write( const int sockfd ) {
    std::string err_stem { "SocketBuffer::write(): " };
    ssize_t bytes_written { send(
            sockfd, _buffer.data(), _bytes_stored, _MSG_NONE ) };
    if ( bytes_written == -1 ) {
        throw errors::system::exception(
            err_stem + "send" );
    }
    assert( static_cast< size_t >( bytes_written ) == _bytes_stored );
    clear();
    return bytes_written;
}

size_t SocketBuffer::read( const int sockfd ) {
    std::string err_stem { "SocketBuffer::read(): " };
    ssize_t bytes_read { recv(
            sockfd, _buffer.data(), _buffer.size(), _MSG_NONE ) };
    if ( bytes_read == -1 ) {
        clear();
        throw errors::system::exception(
            err_stem + "recv" );
    }
    size_t bytes_stored ( bytes_read );
    while ( bytes_stored == _buffer.size() ) {
        _buffer.resize( _buffer.size() + _BLOCK_SZ );
        ++_block_ct;
        bytes_read = recv(
            sockfd, _buffer.data() + bytes_stored, _BLOCK_SZ, _MSG_NONE );
        if ( bytes_read == -1 ) {
            clear();
            throw errors::system::exception(
                err_stem + "recv" );
        }
        bytes_stored += bytes_read;
    }
    _bytes_stored = bytes_stored;
    return _bytes_stored;
}
