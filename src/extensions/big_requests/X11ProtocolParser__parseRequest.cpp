#include <type_traits>  // is_base_of_v

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/extensions/requests.hpp"
#include "protocol/extensions/big_requests.hpp"


namespace ext = protocol::extensions;

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    ext::big_requests::requests::BigReqEnable >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using ext::big_requests::requests::BigReqEnable;
    using ext::requests::impl::SimpleRequest;
    static_assert( std::is_base_of_v< SimpleRequest, BigReqEnable > );
    assert( data != nullptr );
    assert( sz >= sizeof( BigReqEnable::Prefix ) );
    assert( _ordered( reinterpret_cast< const BigReqEnable::Prefix* >(
                          data )->minor_opcode, conn->byteswap ) ==
            ext::big_requests::requests::opcodes::BIGREQENABLE );
    return _parseRequest< SimpleRequest >( conn, data, sz );
}
