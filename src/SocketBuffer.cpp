#include <algorithm>     // min
#include <utility>       // pair
#include <optional>
#include <string>

#include <cassert>
#include <cstring>       // memcpy

#include <sys/socket.h>  // send recv
#include <errno.h>
#include <unistd.h>      // ssize_t

#include <fmt/format.h>

#include "SocketBuffer.hpp"
#include "errors.hpp"


/*         data()
           bw        size()      br       ba         _buffer.size()
           |                     |                   |
▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
*/

std::pair< size_t, std::optional< std::string > >
SocketBuffer::read( const int sockfd,
                    const size_t bytes_to_read ) {
    if ( bytes_to_read > _bytes_available ) {
        const size_t raw_sz {
            _buffer.size() + ( bytes_to_read - _bytes_available ) };
        // nearest larger multiple of _BLOCK_SZ
        _buffer.resize(
            raw_sz + ( ( _BLOCK_SZ - ( raw_sz % _BLOCK_SZ ) ) % _BLOCK_SZ ) );
        _bytes_available = _buffer.size() - _bytes_read;
        assert( _bytes_available >= bytes_to_read );
    }
    const ssize_t recv_ret {
        ::recv( sockfd, _buffer.data() + _bytes_read, bytes_to_read, _MSG_NONE ) };
    if ( recv_ret == -1 )
        return { 0, errors::system::message( "recv" ) };
    const size_t recv_sz ( recv_ret );
    _bytes_read      += recv_sz;
    _bytes_available -= recv_sz;
    return { recv_sz, std::nullopt };
}

std::pair< size_t, std::optional< std::string > >
SocketBuffer::read( const int sockfd ) {
    const size_t bytes_to_read {
        ( _next_message_sz == _UNKNOWN_SZ ) ? _bytes_available :
        _next_message_sz - size() };
    return read( sockfd, bytes_to_read );
}

size_t SocketBuffer::load( const void* input,
                           const size_t bytes_to_load ) {
    assert( input != nullptr );
    assert( bytes_to_load > 0 );
    if ( bytes_to_load > _bytes_available ) {
        const size_t raw_sz {
            _buffer.size() + ( bytes_to_load - _bytes_available ) };
        // nearest larger multiple of _BLOCK_SZ
        _buffer.resize(
            raw_sz + ( ( _BLOCK_SZ - ( raw_sz % _BLOCK_SZ ) ) % _BLOCK_SZ ) );
        _bytes_available = _buffer.size() - _bytes_read;
        assert( _bytes_available >= bytes_to_load );
    }
    ::memcpy( _buffer.data() + _bytes_read, input, bytes_to_load );
    _bytes_read      += bytes_to_load;
    _bytes_available -= bytes_to_load;
    return bytes_to_load;
}

std::pair< size_t, std::optional< std::string > >
SocketBuffer::write( const int sockfd,
                     const size_t bytes_to_write ) {
    assert( bytes_to_write <= _bytes_parsed );
    const ssize_t send_ret {
        ::send( sockfd, data(), bytes_to_write, _MSG_NONE ) };
    if ( send_ret == -1 )
        return { 0, errors::system::message( "send" ) };
    const size_t send_sz ( send_ret );
    assert( send_sz == bytes_to_write );
    // bytes written removed (hidden) from front of buffer
    _bytes_written += send_sz;
    _bytes_parsed  -= send_sz;
    if ( _bytes_written == _bytes_read )
        clear();
    return { send_sz, std::nullopt };
}

std::pair< size_t, std::optional< std::string > >
SocketBuffer::write( const int sockfd ) {
    return write( sockfd, _bytes_parsed );
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
