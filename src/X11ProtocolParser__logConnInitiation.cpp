#include <string_view>

#include <cctype>      // isprint

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/connection_setup.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/version.hpp"


size_t X11ProtocolParser::_logConnInitiation(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::connection_setup::ConnInitiation;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( ConnInitiation::Encoding ) );

    size_t bytes_parsed {};
    const _Whitespace& ws { _ROOT_WS };
    const ConnInitiation::Encoding* encoding {
        reinterpret_cast< const ConnInitiation::Encoding* >( data ) };
    assert( encoding->byte_order == ConnInitiation::MSBFIRST ||
            encoding->byte_order == ConnInitiation::LSBFIRST );
    // determine if host byte order is same as client ( potentially different
    //   with remote clients )
    // TBD could use std::endian with C++20
    const bool little_endian { [](){
        const uint32_t i { 1 };
        const uint8_t* arr {
            reinterpret_cast< const uint8_t* >( &i ) };
        return ( arr[0] == 1 );
    }() };
    const bool byteswap { little_endian !=
                          ( encoding->byte_order == ConnInitiation::LSBFIRST ) };
    conn->byteswap = byteswap;
    // TBD version error instead of assert?
    assert( _hostByteOrder( encoding->protocol_major_version, byteswap ) ==
            protocol::MAJOR_VERSION );
    assert( _hostByteOrder( encoding->protocol_minor_version, byteswap ) ==
            protocol::MINOR_VERSION );
    bytes_parsed += sizeof( ConnInitiation::Encoding );
    // followed by STRING8 authorization-protocol-name
    const uint16_t name_len { _hostByteOrder( encoding->name_len, byteswap ) };
    const std::string_view auth_protocol_name {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        name_len };
    bytes_parsed += alignment.pad( name_len );
    // followed by STRING8 authorization-protocol-data
    const uint16_t data_len { _hostByteOrder( encoding->data_len, byteswap ) };
    const _ParsingOutputs authorization_protocol_data {
        _parseLISTof< protocol::CARD8 >(
            data + bytes_parsed, data_len, data_len,
            byteswap, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    bytes_parsed += alignment.pad( data_len );
    assert( bytes_parsed == sz );  // (should not be batched with other packetss)

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(authorization-protocol-name length)" ) :
                              sizeof( "authorization-protocol-name" ) ) - 1 );
    fmt::println(
        settings.log_fs,
        "C{:03d}:{:04d}B:{}: client {:?} attempting connection: "
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{}"                       // name_len, data_len
        "{}{: <{}}{}{:?}{}"          // authorization-protocol-name
        "{}{: <{}}{}({:d} bytes){}"  // authorization-protocol-data (hidden)
        "{}}}",
        conn->id, bytes_parsed, CLIENT_TO_SERVER, conn->client_desc,
        ws.separator,
        ws.memb_indent, "byte-order", memb_name_w, ws.equals,
        _formatVariable( uint8_t( encoding->byte_order ==
                                  ConnInitiation::MSBFIRST ), byteswap,
                         { ConnInitiation::byte_order_names } ), ws.separator,
        ws.memb_indent, "protocol-major-version", memb_name_w, ws.equals,
        _formatVariable( encoding->protocol_major_version, byteswap ), ws.separator,
        ws.memb_indent, "protocol-minor-version", memb_name_w, ws.equals,
        _formatVariable( encoding->protocol_minor_version, byteswap ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(authorization-protocol-name length)",
            memb_name_w, ws.equals,
            _formatVariable( encoding->name_len, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(authorization-protocol-data length)",
            memb_name_w, ws.equals,
            _formatVariable( encoding->data_len, byteswap ), ws.separator ),
        ws.memb_indent, "authorization-protocol-name", memb_name_w, ws.equals,
        auth_protocol_name, ws.separator,
        ws.memb_indent, "authorization-protocol-data", memb_name_w, ws.equals,
        encoding->data_len, ws.separator,
        ws.encl_indent
        );
    return bytes_parsed;
}
