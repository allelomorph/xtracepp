#ifndef CONNECTION_HPP
#define CONNECTION_HPP


#include <string>
#include <unordered_map>

#include <cstdint>

#include "SocketBuffer.hpp"
#include "protocol/common_types.hpp"


class Connection {
private:
    static constexpr int   _FD_CLOSED { -1 };
    inline static uint32_t _next_id {};

    // TBD how does xtrace refer to seq nums before current one without storing anything?
    // TBD some requests seem to be closed out by replies/errors,
    //   others may theoretically be referenced several times over - for now
    //   using a map for parallel non-contiguous seq num access, and erasing
    //   entries when a reply or error is issued for a request's seq num
    std::unordered_map< uint16_t, uint8_t > _request_opcodes_by_seq_num;

public:
    const uint32_t id;           // unique serial number
    const uint64_t start_time;   // timestamp of connection creation (seconds since Unix Epoch)
    std::string    client_desc;  // client address (x.x.x.x:port) for AF_INET
                                 //   ([x:x::x...]:port) for AF_INET6
                                 //   <socket file path> or "unknown(local)" for AF_UNIX
    int            client_fd { _FD_CLOSED };  // socket accept(2)ed from client
    SocketBuffer   client_buffer;
    int            server_fd { _FD_CLOSED };  // socket connect(2)ed to server
    SocketBuffer   server_buffer;

    uint16_t       sequence {}; // Request sequence number, should match real server
                                //   starts at 1 after first call to registerRequest

    bool byteswap {};
    // const bool bigendian;

    // connection states are:
    //   uncontacted > open > closed
    //               > closed ( general fail )
    //               > authentication negotiation > closed ( auth fail )
    //                                            > open > closed
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

    inline uint16_t
    registerRequest( const uint8_t opcode ) {
        _request_opcodes_by_seq_num.emplace( ++sequence, opcode );
        return sequence;
    }

    inline uint8_t
    lookupRequest( const uint16_t seq_num ) {
        return _request_opcodes_by_seq_num.at( seq_num );
    }

    inline void
    unregisterRequest( const uint16_t seq_num ) {
        // .erase is idempotent, no need for bounds check assert
        _request_opcodes_by_seq_num.erase( seq_num );
    }
};


#endif  // CONNECTION_HPP
