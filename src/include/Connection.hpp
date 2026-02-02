#ifndef CONNECTION_HPP
#define CONNECTION_HPP

/**
 * @file Connection.hpp
 */

#include <string>
#include <unordered_map>

#include <cstdint>

#include "SocketBuffer.hpp"
#include "Extensions.hpp"
#include "protocol/common_types.hpp"


/**
 * @brief Tracks state of individual client connection to X server.
 */
class Connection {
public:
    struct RequestOpcodes {
        uint8_t major {};
        uint8_t minor {};

        RequestOpcodes( const uint8_t major_, const uint8_t minor_ = {} ) :
            major( major_ ), minor( minor_ ) {}
    };

private:
    /**
     * @brief Sentinel indicating closed file descriptor.
     */
    static constexpr int   _FD_CLOSED { -1 };
    /**
     * @brief Static source of connection serial numbers.
     */
    inline static uint32_t _next_id {};
    /**
     * @brief Major opcodes of open requests, indexed by their order in the
     *   list of all requests made thus far.
     * @note A request is considered open until a corresponding error or reply
     *   is received from server.
     */
    std::unordered_map< uint16_t, RequestOpcodes >
    _request_opcodes_by_seq_num;

public:
    /**
     * @brief Unique serial number.
     */
    const uint32_t id;
    /**
     * @brief Timestamp of connection creation (seconds since Unix Epoch).
     */
    const uint64_t start_time;
    /**
     * @brief Client address string.
     * @note Format for:
     *   - `AF_INET`:  xxx.xxx.xxx.xxx:port
     *   - `AF_INET6`: [xxxx:xxxx::xxxx...]:port
     *   - `AF_UNIX`:  socket file path\|"unknown(local)"
     */
    std::string    client_desc;
    /**
     * @brief Socket `accept(2)`ed from client.
     */
    int            client_fd { _FD_CLOSED };
    /**
     * @brief Temporarily stores data read from [client_fd](#client_fd).
     */
    SocketBuffer   client_buffer;
    /**
     * @brief Socket `connect(2)`ed to server.
     * [connect]: https://www.man7.org/linux/man-pages/man2/connect.2.html
     */
    int            server_fd { _FD_CLOSED };
    /**
     * @brief Temporarily stores data read from [server_fd](#server_fd).
     */
    SocketBuffer   server_buffer;
    /**
     * @brief Serial number of last request processed.
     * @note Requests should be 1-indexed and index should match actual X server.
     */
    uint16_t       sequence {};
    /**
     * @brief Whether connection has requested a byte order that is the opposite
     *   of the logging host.
     */
    bool           byteswap {};
    /**
     * @brief Connection state constants.
     * - `UNESTABLISHED` before initial handshake is completed
     * - `OPEN` normal message exchange
     * - `CLOSED` closed properly after normal operation
     * - `FAILED` general failure to open (other than due to insufficient authentication)
     * - `AUTHENTICATION` follow-up authentication negotation after failure to open normally
     *
     * @note Possible connection state flow:
     *   ```
     *   UNESTABLISHED > OPEN ( no further auth needed ) > CLOSED
     *                 > CLOSED ( general failure )
     *                 > AUTHENTICATION > OPEN > CLOSED
     *                                  > CLOSED ( auth failure )
     *   ```
     */
    enum Status {
        UNESTABLISHED,  // open:false auth:false fail:false
        OPEN,           // open:true  auth:true  fail:false
        CLOSED,         // open:false auth:true  fail:false
        FAILED,         // open:false auth:false fail:true
        AUTHENTICATION  // open:true  auth:false fail:false
    };
    /**
     * @brief Current connection state.
     */
    Status status { UNESTABLISHED };
    /**
     * @brief Tracks activation of protocol extensions for connection.
     */
    Extensions extensions;
    /**
     * @brief Default ctor; assigns [id](#id) and [start_time](#start_time).
     */
    Connection();
    /**
     * @brief Closes client-facing socket.
     */
    void
    closeClientSide();
    /**
     * @brief Closes server-facing socket.
     */
    void
    closeServerSide();
    /**
     * @brief Syntax sweetener for checking socket status.
     * @return whether client-facing socket is closed
     */
    inline bool
    clientSideClosed() {
        return ( client_fd == _FD_CLOSED );
    }
    /**
     * @brief Syntax sweetener for checking socket status.
     * @return whether client-facing socket is open
     */
    inline bool
    clientSideOpen() {
        return ( client_fd != _FD_CLOSED );
    }
    /**
     * @brief Syntax sweetener for checking socket status.
     * @return whether server-facing socket is closed
     */
    inline bool
    serverSideClosed() {
        return ( server_fd == _FD_CLOSED );
    }
    /**
     * @brief Syntax sweetener for checking socket status.
     * @return whether server-facing socket is open
     */
    inline bool
    serverSideOpen() {
        return ( server_fd != _FD_CLOSED );
    }
    // /**
    //  * @brief Syntax sweetener for reading from socket.
    //  * @return bytes buffered from client-facing socket
    //  */
    // inline size_t
    // bufferFromClient() {
    //     return client_buffer.read( client_fd );
    // }
    // /**
    //  * @brief Syntax sweetener for reading from socket.
    //  * @return bytes buffered from server-facing socket
    //  */
    // inline size_t
    // bufferFromServer() {
    //     return server_buffer.read( server_fd );
    // }
    // /**
    //  * @brief Syntax sweetener for writing to socket.
    //  * @return bytes sent to client-facing socket
    //  */
    // inline size_t
    // forwardToClient() {
    //     return server_buffer.write( client_fd );
    // }
    // /**
    //  * @brief Syntax sweetener for writing to socket.
    //  * @return bytes sent to server-facing socket
    //  */
    // inline size_t
    // forwardToServer() {
    //     return client_buffer.write( server_fd );
    // }
    /**
     * @brief Assign serial number to request and store opcodes, thus
     *   marking request as open.
     * @param opcode major opcode of request
     * @return request sequence (serial) number
     * @note A request is considered open until a corresponding error or reply
     *   is received from server.
     */
    inline uint16_t
    registerRequest( const uint8_t major, const uint8_t minor = {} ) {
        _request_opcodes_by_seq_num.emplace(
            ++sequence, RequestOpcodes( major, minor ) );
        return sequence;
    }
    /**
     * @brief Retrieve open request major opcode by sequence number.
     * @param seq_num request sequence (serial) number
     * @return request major opcode
     */
    inline RequestOpcodes
    lookupRequest( const uint16_t seq_num ) {
        return _request_opcodes_by_seq_num.at( seq_num );
    }
    /**
     * @brief Remove major opcode from set of open requests.
     * @param seq_num request sequence (serial) number
     */
    inline void
    unregisterRequest( const uint16_t seq_num ) {
        // .erase is idempotent, no need for bounds check assert
        _request_opcodes_by_seq_num.erase( seq_num );
    }
};


#endif  // CONNECTION_HPP
