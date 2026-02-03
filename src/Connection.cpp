#include <unistd.h>    // close exit EXIT_FAILURE
#include <errno.h>
#include <sys/time.h>  // timeval gettimeofday
#include <stdio.h>     // stderr

#include <fmt/format.h>

#include "Connection.hpp"
#include "errors.hpp"


Connection::Connection() :
    id ( _next_id++ ),
    start_time ( [](){
        ::timeval tv {};
        if ( ::gettimeofday( &tv, NULL ) != 0 ) {
            fmt::println(
                ::stderr, "{}: {}",
                __PRETTY_FUNCTION__, errors::system::message( "gettimeofday") );
            ::exit( EXIT_FAILURE );
        }
        return tv.tv_sec * uint64_t{ 1000 } + tv.tv_usec / 1000;
    }() ) {
}

void
Connection::closeClientSide() {
    if ( clientSideClosed() )
        return;
    if ( ::close( client_fd ) == -1 ) {
        fmt::println(
            ::stderr, "{}: {}",
            __PRETTY_FUNCTION__, errors::system::message( "close" ) );
        ::exit( EXIT_FAILURE );
    }
    client_fd = _FD_CLOSED;
}

void
Connection::closeServerSide() {
    if ( serverSideClosed() )
        return;
    if ( ::close( server_fd ) == -1 ) {
        fmt::println(
            ::stderr, "{}: {}",
            __PRETTY_FUNCTION__, errors::system::message( "close" ) );
        ::exit( EXIT_FAILURE );
    }
    server_fd = _FD_CLOSED;
}
