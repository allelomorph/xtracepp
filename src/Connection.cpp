#include <unistd.h>  // close

#include "Connection.hpp"
#include "errors.hpp"


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
    closeClientSocket();
    closeServerSocket();
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
