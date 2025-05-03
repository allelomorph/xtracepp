#ifndef X11PROTOCOLPARSER_HPP
#define X11PROTOCOLPARSER_HPP


//#include <ostream>
#include <iostream>  // cout

#include <cassert>

#include "Connection.hpp"
#include "Settings.hpp"


class X11ProtocolParser {
private:
    static constexpr size_t _ALIGN { 4 };

    std::ostream _log_os { std::cout.rdbuf() };

    // "where E is some expression, and pad(E) is the number of bytes needed to
    //   round E up to a multiple of four."
    inline size_t _padToAlignment( const size_t n, const size_t align ) {
        return n + ( ( align - ( n % align ) ) % align );
    };
    inline size_t _pad( const size_t n ) {
        return _padToAlignment( n, _ALIGN );
    }

    size_t _logConnectionInitiation(
        const Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logClientMessage(
        Connection* conn, uint8_t* data, const size_t sz );

    size_t _logServerResponse(
        Connection* conn, uint8_t* data, const size_t sz );
    size_t _logServerMessage(
        Connection* conn, uint8_t* data, const size_t sz );

    size_t _logClientPacket(
        Connection* conn, uint8_t* data, const size_t sz );
    size_t _logServerPacket(
        Connection* conn, uint8_t* data, const size_t sz );

public:
    X11ProtocolParser() {}

    void syncLogStream( const std::ostream& log_os );
    size_t logClientPackets( Connection* conn, Settings* settings );
    size_t logServerPackets( Connection* conn, Settings* settings );
};


#endif  // X11PROTOCOLPARSER_HPP
