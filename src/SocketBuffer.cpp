#include <algorithm>     // min

#include <cassert>
#include <cstring>       // memcpy

#include <sys/socket.h>  // send recv
#include <errno.h>

#include <fmt/format.h>

#include "SocketBuffer.hpp"
#include "errors.hpp"


/*         data()
             bw      size()      br       ba         _buffer.size()
             |                   |                   |
▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
 */


size_t SocketBuffer::write( const int sockfd,
                            const size_t bytes_to_write ) {
    assert( bytes_to_write <= size() );
    const ssize_t send_sz_ { ::send(
            sockfd, data(), bytes_to_write, _MSG_NONE ) };
    if ( send_sz_ == -1 ) {
        throw errors::system::exception(
            fmt::format( "{}: {}", __PRETTY_FUNCTION__, "send" ) );
    }
    const size_t send_sz ( send_sz_ );
    assert( send_sz == bytes_to_write );
    // bytes written removed (hidden) from front of buffer
    _bytes_written += send_sz;
    if ( _bytes_written == _bytes_read )
        clear();
    return send_sz;
}

size_t SocketBuffer::write( const int sockfd ) {
    return write( sockfd, size() );
}

size_t SocketBuffer::unload( void* output,
                             const size_t bytes_to_unload ) {
    assert( output != nullptr );
    assert( bytes_to_unload <= size() );
    ::memcpy( output, data(), bytes_to_unload );
    // bytes written removed (hidden) from front of buffer
    _bytes_written += bytes_to_unload;
    if ( _bytes_written == _bytes_read )
        clear();
    return bytes_to_unload;
}

size_t SocketBuffer::unload( const size_t bytes_to_unload ) {
    assert( bytes_to_unload <= size() );
    // bytes written removed (hidden) from front of buffer
    _bytes_written += bytes_to_unload;
    if ( _bytes_written == _bytes_read )
        clear();
    return bytes_to_unload;
}

size_t SocketBuffer::read( const int sockfd,
                           const size_t bytes_to_read ) {
    size_t tl_bytes_read {};
    while ( tl_bytes_read < bytes_to_read ) {
        const size_t max_recv_sz {
            std::min( bytes_to_read - tl_bytes_read, _bytes_available ) };
        const ssize_t recv_sz_ {
            ::recv( sockfd, _buffer.data() + ( _buffer.size() - _bytes_available ),
                    max_recv_sz, _MSG_NONE ) };
        if ( recv_sz_ == -1 ) {
            throw errors::system::exception(
                fmt::format( "{}: {}", __PRETTY_FUNCTION__, "recv" ) );
        }
        const size_t recv_sz ( recv_sz_ );
        assert( recv_sz <= max_recv_sz );
        tl_bytes_read += recv_sz;
        if ( recv_sz == _bytes_available ) {
            _buffer.resize( _buffer.size() + _BLOCK_SZ );
            _bytes_available = _BLOCK_SZ;
        } else {
            _bytes_available -= recv_sz;
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
    return read( sockfd, _READ_ALL );
}

size_t SocketBuffer::load( const void* input,
                           const size_t bytes_to_load ) {
    assert( input != nullptr );
    assert( _bytes_available > 0 );
    size_t tl_bytes_loaded {};
    while ( tl_bytes_loaded < bytes_to_load ) {
        const size_t segment_sz {
            std::min( bytes_to_load - tl_bytes_loaded, _bytes_available ) };
        ::memcpy( _buffer.data() + ( _buffer.size() - _bytes_available ),
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
