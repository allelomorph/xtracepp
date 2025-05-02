#include <string>

#include <cassert>

#include <sys/socket.h>  // send recv
#include <errno.h>

#include "SocketBuffer.hpp"
#include "errors.hpp"


// TBD determine if partial writes are really needed
size_t SocketBuffer::write( const int sockfd,
                            const size_t bytes_to_write ) {
    std::string err_stem { "SocketBuffer::write(): " };
    ssize_t bytes_written { send(
            sockfd, data(), bytes_to_write, _MSG_NONE ) };
    if ( bytes_written == -1 ) {
        throw errors::system::exception(
            err_stem + "send" );
    }
    assert( static_cast< size_t >( bytes_written ) == bytes_to_write );
    // bytes written removed (hidden) from front of buffer
    _bytes_written += bytes_written;
    _data += bytes_written;
    if ( _bytes_written == _bytes_read )
        clear();
    return bytes_written;
}

size_t SocketBuffer::write( const int sockfd ) {
    return write( sockfd, size() );
}

// TBD determine if append reads are necessary
//   (can we guarantee that the server or client will always alternately flag read/write readiness?)
size_t SocketBuffer::read( const int sockfd,
                           const size_t bytes_to_read ) {
    std::string err_stem { "SocketBuffer::read(): " };
    ssize_t bytes_read { recv(
            sockfd, data(), bytes_to_read, _MSG_NONE ) };
    if ( bytes_read == -1 ) {
        throw errors::system::exception(
            err_stem + "recv" );
    }
    size_t bytes_read_subtl ( bytes_read );
    while ( _bytes_read + bytes_read_subtl == _buffer.size() ) {
        _buffer.resize( _buffer.size() + _BLOCK_SZ );
        // TBD reallocation requires reassignment of pointer?
        _data = _buffer.data() + _bytes_written;
        bytes_read = recv(
            sockfd, _buffer.data() + _bytes_read + bytes_read_subtl,
            _BLOCK_SZ, _MSG_NONE );
        if ( bytes_read == -1 ) {
            throw errors::system::exception(
                err_stem + "recv" );
        }
        bytes_read_subtl += bytes_read;
    }
    _bytes_read += bytes_read_subtl;
    return bytes_read_subtl;
}

size_t SocketBuffer::read( const int sockfd ) {
    return read( sockfd, _buffer.size() - _bytes_read );
}
