#include "Connection.hpp"

#include <unistd.h>  // close


Connection::Connection() :
    id ( _next_id++ ),
    start_time ( [](){
        /*struct */timeval tv;
        if ( gettimeofday( &tv, NULL ) != 0 ) {
            // TBD include string of errno name
            throw std::system_error(
                errno, std::generic_category(),
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
    close( client_fd );
    // TBD error for any errno other than EBADF
    client_fd = _FD_CLOSED;
}

void
Connection::closeServerSocket() {
    close( server_fd );
    // TBD error for any errno other than EBADF
    server_fd = _FD_CLOSED;
}
