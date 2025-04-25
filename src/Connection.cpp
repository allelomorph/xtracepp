#include "Connnection.hpp"

#include <unistd.h>  // close


void
Connnection::closeClientSocket() {
    close( client_fd );
    // TBD error for any errno other than EBADF
    client_fd = _FD_CLOSED;
}

void
Connnection::closeServerSocket() {
    close( server_fd );
    // TBD error for any errno other than EBADF
    server_fd = _FD_CLOSED;
}
