#ifndef CONNECTION_HPP
#define CONNECTION_HPP


#include <string>
//#include <string_view>
#include <system_error>  // generic_category

#include <cstdint>

#include <sys/time.h>        // 'struct timeval' gettimeofday
#include <errno.h>

#include "SocketBuffer.hpp"


class Connection {
private:
    static constexpr int _FD_CLOSED { -1 };

    inline static int  _next_id {};

public:
    const int      id;             // unique serial number
    const uint64_t start_time;     // timestamp of connection creation (seconds since Unix Epoch)
    // TBD from, client_fd, server_fd const and set in ctor?
    std::string    client_desc;    // (from) allocated string describing client address (x.x.x.x:port) for AF_INET or socket file path/"unknown(local)" for AF_UNIX
    int            client_fd { _FD_CLOSED };      // socket accept(2)ed from x client
    SocketBuffer   client_buffer;
    int            server_fd { _FD_CLOSED };      // socket connect(2)ed to x server
    SocketBuffer   server_buffer;
//    const bool bigendian;
//    enum client_state { c_start = 0, c_normal, c_amlost } client_state;
//    enum server_state { s_start=0, s_normal, s_amlost } server_state;
//    struct expectedreply* expectedreplies;
//    uint64_t seq;
//    struct usedextension* usedextensions;
//    struct unknownextension* waiting;
//    struct unknownextension* unknownextensions;

    Connection();
    ~Connection();
    // Connection() :
    //     id ( _next_id++ ),
    //     start_time ( [](){
    //         /*struct */timeval tv;
    //         if( gettimeofday( &tv, NULL ) != 0 ) {
    //             // TBD include string of errno name
    //             throw std::system_error(
    //                 errno, std::generic_category(),
    //                 "Connection::Connection()" );
    //         }
    //         return tv.tv_sec * uint64_t{ 1000 } + tv.tv_usec / 1000;
    //     }() ) {}

    // ~Connection() {
    //     closeClientSocket();
    //     closeServerSocket();
    // }

    void
    closeClientSocket();

    void
    closeServerSocket();

    inline bool
    clientSocketIsClosed() {
        return ( client_fd == _FD_CLOSED );
    }

    inline bool
    serverSocketIsClosed() {
        return ( server_fd == _FD_CLOSED );
    }

    // bufferPacketFromClient  // client_buffer.read(client_fd)
    // bufferPacketFromServer  // server_buffer.read(server_fd)
    // forwardPacketToClient   // server_buffer.write(client_fd)
    // forwardPacketToServer   // client_buffer.write(server_fd)

};


#endif  // CONNECTION_HPP
