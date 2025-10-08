#include <unistd.h>    // close
#include <errno.h>
#include <sys/time.h>  // timeval gettimeofday

#include <cassert>

#include "Connection.hpp"
#include "errors.hpp"


Connection::Connection() :
    id ( _next_id++ ),
    start_time ( [](){
        timeval tv {};
        if ( gettimeofday( &tv, NULL ) != 0 ) {
            throw errors::system::exception( __PRETTY_FUNCTION__ );
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
        throw errors::system::exception( __PRETTY_FUNCTION__ );
    }
    client_fd = _FD_CLOSED;
}

void
Connection::closeServerSocket() {
    if ( serverSocketIsClosed() )
        return;
    if ( close( server_fd ) == -1 && errno != EBADF ) {
        throw errors::system::exception( __PRETTY_FUNCTION__ );
    }
    server_fd = _FD_CLOSED;
}
