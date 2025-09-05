#include <string>
#include <algorithm>  // min

#include <cassert>
#include <cstring>  // memcpy

#include <sys/socket.h>  // send recv
#include <errno.h>

#include "SocketBuffer.hpp"
#include "errors.hpp"


/*         data()
             bw      size()      br       ba         _buffer.size()
             |                   |                   |
▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
 */


// TBD determine if partial writes are really needed
size_t SocketBuffer::write( const int sockfd,
                            const size_t bytes_to_write ) {
    std::string err_stem { "SocketBuffer::write(): " };
    assert( bytes_to_write <= size() );
    ssize_t bytes_written { send(
            sockfd, data(), bytes_to_write, _MSG_NONE ) };
    if ( bytes_written == -1 ) {
        throw errors::system::exception(
            err_stem + "send" );
    }
    assert( static_cast< size_t >( bytes_written ) == bytes_to_write );
    // bytes written removed (hidden) from front of buffer
    _bytes_written += bytes_written;
    if ( _bytes_written == _bytes_read )
        clear();
    return bytes_written;
}

// TBD direct manual buffer unload for simple clients used during init
size_t SocketBuffer::unload( void* output,
                             const size_t bytes_to_unload ) {
    assert( bytes_to_unload <= size() );
    if ( output != nullptr )
        memcpy( output, data(), bytes_to_unload );
    // bytes written removed (hidden) from front of buffer
    _bytes_written += bytes_to_unload;
    if ( _bytes_written == _bytes_read )
        clear();
    return bytes_to_unload;
}

size_t SocketBuffer::write( const int sockfd ) {
    return write( sockfd, size() );
}

// TBD determine if append reads are necessary
//   (can we guarantee that the server or client will always alternately flag read/write readiness?)
size_t SocketBuffer::read( const int sockfd,
                           const size_t bytes_to_read ) {
    std::string err_stem { "SocketBuffer::read(): " };
    assert( _bytes_available > 0 );
    size_t tl_bytes_read {};
    ssize_t bytes_read {};
    while ( true ) {
        bytes_read = recv(
                sockfd, _buffer.data() + ( _buffer.size() - _bytes_available ),
                _bytes_available, _MSG_NONE );
        if ( bytes_read == -1 ) {
            throw errors::system::exception(
                err_stem + "recv" );
        }
        tl_bytes_read += bytes_read;
        if ( bytes_read == _bytes_available ) {
            _buffer.resize( _buffer.size() + _BLOCK_SZ );
            _bytes_available = _BLOCK_SZ;
        } else {
            _bytes_available -= bytes_read;
            break;
        }
    }
    _bytes_read += tl_bytes_read;
    return tl_bytes_read;
}

size_t SocketBuffer::read( const int sockfd ) {
    if ( _bytes_available == 0 ) {
        _buffer.resize( _buffer.size() + _BLOCK_SZ );
        _bytes_available = _BLOCK_SZ;
    }
    return read( sockfd, _bytes_available );
}

// TBD direct manual buffer load for simple clients used during init
size_t SocketBuffer::load( const void* input,
                           const size_t bytes_to_load ) {
    assert( input != nullptr );
    assert( _bytes_available > 0 );
    size_t tl_bytes_loaded {};
    while ( true ) {
        size_t segment_sz {
            std::min( bytes_to_load - tl_bytes_loaded, _bytes_available ) };
        memcpy( _buffer.data() + ( _buffer.size() - _bytes_available ),
                input, segment_sz );
        tl_bytes_loaded += segment_sz;
        if ( segment_sz == _bytes_available ) {
            _buffer.resize( _buffer.size() + _BLOCK_SZ );
            _bytes_available = _BLOCK_SZ;
        } else {
            _bytes_available -= segment_sz;
            break;
        }
    }
    _bytes_read += tl_bytes_loaded;
    return tl_bytes_loaded;
}
