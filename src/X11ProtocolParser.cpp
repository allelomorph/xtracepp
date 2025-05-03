#include <string_view>
#include <iomanip>  // setw

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "Settings.hpp"
#include "protocol/common_types.hpp"
#include "protocol/connection_setup.hpp"


size_t X11ProtocolParser::_logConnectionInitiation(
    const Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    const uint8_t* _data { data };
    const protocol::ConnectionInitiation::Header* header {
        reinterpret_cast< const protocol::ConnectionInitiation::Header* >( _data )
    };
    // TBD error if protocol is not 11.0
    _data += sizeof( protocol::ConnectionInitiation::Header );
    // STRING8 of d bytes authorization-protocol-name
    std::string_view authorization_protocol_name {
        reinterpret_cast< const char* >( _data ), header->n };
    _data += _pad( header->n );
    // STRING8 of d bytes authorization-protocol-data (may not be printable)
    std::string_view authorization_protocol_data {
        reinterpret_cast< const char* >( _data ), header->d };
    _data += _pad( header->d );
    assert( _data - data == sz );

    // TBD asserts close log file without logging anything
    _log_os << std::setw(3) << conn->id << ":<:" <<
        " byte-order: '" << header->byte_order << "'";
    switch ( header->byte_order ) {
    case 'B': _log_os << " (MSB first)"; break;
    case 'l': _log_os << " (LSB first)"; break;
    default:  assert( 0 ); break;
    }
    _log_os << " protocol: " << std::dec << header->protocol_major_version <<
        "." << std::dec << header->protocol_minor_version <<
        " authorization-protocol-name[" << header->n << "]: " << authorization_protocol_name <<
        " authorization-protocol-data[" << header->d << "] (may be unprintable)" <<
        "\n";

    return sz;
}

size_t X11ProtocolParser::_logClientMessage(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    return sz;
}

size_t X11ProtocolParser::_logServerResponse(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    // TBD temp
    conn->status = Connection::OPEN;
    return sz;
}

size_t X11ProtocolParser::_logServerMessage(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    return sz;
}

size_t X11ProtocolParser::_logClientPacket(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    switch ( conn->status ) {
    case Connection::UNESTABLISHED:
        return _logConnectionInitiation( conn, data, sz );
    case Connection::AUTHENTICATION:
        // authentication negotiation
    case Connection::CLOSED: [[fallthrough]];
    case Connection::FAILED:
        assert( 0 );
    default:
        break;
    }
    // case Connection::OPEN:
    return _logClientMessage( conn, data, sz );
}

size_t X11ProtocolParser::_logServerPacket(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    switch ( conn->status ) {
    case Connection::UNESTABLISHED:
        return _logServerResponse( conn, data, sz );
    case Connection::AUTHENTICATION:
        // authentication negotiation
    case Connection::CLOSED: [[fallthrough]];
    case Connection::FAILED:
        assert( 0 );
    default:
        break;
    }
    // case Connection::OPEN:
    return _logServerMessage( conn, data, sz );
}

void X11ProtocolParser::syncLogStream( const std::ostream& log_os ) {
    // TBD errors?
    assert( log_os.good() );
    _log_os.rdbuf( log_os.rdbuf() );
    assert( _log_os.good() );
}

size_t X11ProtocolParser::logClientPackets( Connection* conn,
                                            Settings* settings ) {
    assert( conn != nullptr );
    assert( settings != nullptr );
    uint8_t* data { conn->client_buffer.data() };
    size_t   tl_bytes_parsed {};
    // std::cerr << "X11ProtocolParser::logClientPackets(): conn->client_buffer.size(): " << conn->client_buffer.size() << std::endl;
    // std::cerr << "X11ProtocolParser::logClientPackets(): conn->status: " << conn->status << std::endl;
    for ( const size_t bytes_to_parse { conn->client_buffer.size() };
          tl_bytes_parsed < bytes_to_parse; ) {
        size_t bytes_parsed {
            _logClientPacket( conn, data, bytes_to_parse - tl_bytes_parsed ) };
        if ( settings->readwritedebug ) {
            _log_os << std::setw(3) << conn->id <<
                ":<:parsed   " << std::setw(4) << bytes_parsed << " bytes\n";
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    return tl_bytes_parsed;
}

size_t X11ProtocolParser::logServerPackets( Connection* conn,
                                            Settings* settings ) {
    assert( conn != nullptr );
    assert( settings != nullptr );
    uint8_t* data { conn->server_buffer.data() };
    size_t   tl_bytes_parsed {};
    // std::cerr << "X11ProtocolParser::logServerPackets(): conn->server_buffer.size(): " << conn->server_buffer.size() << std::endl;
    // std::cerr << "X11ProtocolParser::logServerPackets(): conn->status: " << conn->status << std::endl;
    for ( const size_t bytes_to_parse { conn->server_buffer.size() };
          tl_bytes_parsed < bytes_to_parse; ) {
        size_t bytes_parsed {
            _logServerPacket( conn, data, bytes_to_parse - tl_bytes_parsed ) };
        if ( settings->readwritedebug ) {
            _log_os << std::setw(3) << conn->id <<
                ":>:parsed   " << std::setw(4) << bytes_parsed << " bytes\n";
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    return tl_bytes_parsed;
}
