#ifndef CONNECTION_HPP
#define CONNECTION_HPP


#include <string>
//#include <string_view>
#include <system_error>  // generic_category
#include <vector>

#include <cstdint>

#include <sys/time.h>        // 'struct timeval' gettimeofday
#include <errno.h>

#include "SocketBuffer.hpp"


class Connection {
private:
    static constexpr int _FD_CLOSED  { -1 };
    inline static uint32_t  _next_id {};

    // 1-indexed so begin with one dummy member
    // TBD this allows for lookup of request traits when parsing Error/Reply
    std::vector< uint8_t > _request_opcodes_by_seq_num { 1 };

public:
    const uint32_t id;             // unique serial number
    const uint64_t start_time;     // timestamp of connection creation (seconds since Unix Epoch)
    // TBD from, client_fd, server_fd const and set in ctor?
    std::string    client_desc;    // (from) allocated string describing client address (x.x.x.x:port) for AF_INET or socket file path/"unknown(local)" for AF_UNIX
    int            client_fd { _FD_CLOSED };      // socket accept(2)ed from x client
    SocketBuffer   client_buffer;
    int            server_fd { _FD_CLOSED };      // socket connect(2)ed to x server
    SocketBuffer   server_buffer;

// TBD connection states are: uncontacted > open > closed
//                                        > closed ( general fail )
//                                        > authentication negotiation > closed ( auth fail )
//                                                                     > open > closed
    enum Status {
        // before handshake is completed to open
        UNESTABLISHED,    // open:false auth:false fail:false
        // normal packet exchange
        OPEN,             // open:true  auth:true  fail:false
        // closed properly after normal operation
        CLOSED,           // open:false auth:true  fail:false
        // general failure to open
        FAILED,           // open:false auth:false fail:true
        // follow-up authentication negotation after failure to open normally
        AUTHENTICATION    // open:true  auth:false fail:false
    };
    Status status { UNESTABLISHED };

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

    void
    closeClientSocket();

    void
    closeServerSocket();

    inline bool
    clientSocketIsClosed() {
        return ( client_fd == _FD_CLOSED );
    }

    inline bool
    clientSocketIsOpen() {
        return ( client_fd != _FD_CLOSED );
    }

    inline bool
    serverSocketIsClosed() {
        return ( server_fd == _FD_CLOSED );
    }

    inline bool
    serverSocketIsOpen() {
        return ( server_fd != _FD_CLOSED );
    }

    inline size_t
    bufferPacketFromClient() {
        return client_buffer.read( client_fd );
    }

    inline size_t
    bufferPacketFromServer() {
        return server_buffer.read( server_fd );
    }

    inline size_t
    forwardPacketToClient() {
        return server_buffer.write( client_fd );
    }

    inline size_t
    forwardPacketToServer() {
        return client_buffer.write( server_fd );
    }

    void registerRequest( const uint8_t opcode );
    uint8_t lookupRequest( const uint16_t seq_num );
};


#endif  // CONNECTION_HPP
