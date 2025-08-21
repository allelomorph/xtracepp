#include <unistd.h>  // close

#include <cassert>

#include "Connection.hpp"
#include "errors.hpp"
#include "protocol/predefined_atoms.hpp"


Connection::Connection() :
    id ( _next_id++ ),
    start_time ( [](){
        /*struct */timeval tv;
        if ( gettimeofday( &tv, NULL ) != 0 ) {
            throw errors::system::exception(
                "Connection::Connection()" );
        }
        return tv.tv_sec * uint64_t{ 1000 } + tv.tv_usec / 1000;
    }() ) {
}

Connection::~Connection() {
    // TBD emplacing connections into the _connections map calls the dtor and prematurely closes the fds
    // closeClientSocket();
    // closeServerSocket();
}

void
Connection::closeClientSocket() {
    if ( clientSocketIsClosed() )
        return;
    if ( close( client_fd ) == -1 && errno != EBADF ) {
        throw errors::system::exception(
            "Connection::closeClientSocket()" );
    }
    client_fd = _FD_CLOSED;
}

void
Connection::closeServerSocket() {
    if ( serverSocketIsClosed() )
        return;
    if ( close( server_fd ) == -1 && errno != EBADF ) {
        throw errors::system::exception(
            "Connection::closeServerSocket()" );
    }
    server_fd = _FD_CLOSED;
}

// TBD using a vector to look up by index allows for reaching into request history,
//   but eats up memory and may not be useful. xtrace just uses static var for
//   most recent value.
void
Connection::registerRequest( const uint8_t opcode ) {
    _request_opcodes_by_seq_num.emplace_back( opcode );
}

uint8_t
Connection::lookupRequest( const uint16_t seq_num ) {
    assert( seq_num < _request_opcodes_by_seq_num.size() );
   // TBD temp assert here to see if replies ever sent out of sequence
    assert( seq_num == _request_opcodes_by_seq_num.size() - 1 );
    return _request_opcodes_by_seq_num[ seq_num ];
}
