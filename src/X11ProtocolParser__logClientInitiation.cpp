#include <string_view>

#include <cctype>      // isprint

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/connection_setup.hpp"
#include "protocol/enum_names.hpp"


size_t X11ProtocolParser::_logClientInitiation(
    const Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );

    size_t bytes_parsed {};
    using namespace protocol::connection_setup;
    const ClientInitiation::Header* header {
        reinterpret_cast< const ClientInitiation::Header* >( data ) };
    assert( header->byte_order == 'B' || header->byte_order == 'l' );
    // TBD error if protocol is not 11.0
    bytes_parsed += sizeof( ClientInitiation::Header );
    // STRING8 of d bytes authorization-protocol-name
    const std::string_view auth_protocol_name {
        reinterpret_cast< const char* >( data + bytes_parsed ), header->n };
    bytes_parsed += _pad( header->n );
    // STRING8 of d bytes authorization-protocol-data (may not be printable)
    const uint8_t* auth_protocol_data { data + bytes_parsed };
    bytes_parsed += _pad( header->d );
    assert( bytes_parsed == sz );

    const uint32_t name_width (
        settings.multiline ? sizeof( "authorization-protocol-data" ) - 1 : 0 );
    // TBD may be security concerns with logging auth data
    // std::string auth_data_hex;
    // for ( uint16_t i {}, sz { header->d }; i < sz; ++i ) {
    //     auth_data_hex += fmt::format(
    //         "{}{:02X}", auth_data_hex.empty() ? "" : " ",
    //         auth_protocol_data[i] );
    // }
    fmt::println(
        settings.log_fs,
        "C{:03d}:{:04d}B:{}: client \"{}\" attempting connection: "
        "{{{}"
        "{}{: <{}}{}{}{}"
        "{}"
        "{}"
        "{}{: <{}}{}{:?}{}"
        "{}"
        "{}{: <{}}{}({:d} bytes){}"
        "{}}}",
        conn->id, bytes_parsed, _CLIENT_TO_SERVER, conn->client_desc,
        _ROOT_WS.separator,
        _ROOT_WS.memb_indent, "byte-order", name_width, _ROOT_WS.equals,
        _formatInteger( ( header->byte_order == 'l' ) ? 0 : 1,
                        protocol::enum_names::image_byte_order ), _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{}{}"
            "{}{: <{}}{}{}{}",
            _ROOT_WS.memb_indent, "protocol-major-version", name_width, _ROOT_WS.equals,
            _formatInteger( header->protocol_major_version ), _ROOT_WS.separator,
            _ROOT_WS.memb_indent, "protocol-minor-version", name_width, _ROOT_WS.equals,
            _formatInteger( header->protocol_minor_version ), _ROOT_WS.separator ) :
        fmt::format(
            "{}{: <{}}{}{:d}.{:d}{}",
            _ROOT_WS.memb_indent, "protocol version", name_width, _ROOT_WS.equals,
            header->protocol_major_version,
            header->protocol_minor_version, _ROOT_WS.separator ),
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length in bytes of authorization-protocol-name){}",
            _ROOT_WS.memb_indent, "n", name_width, _ROOT_WS.equals,
            _formatInteger( header->n ), _ROOT_WS.separator ) : "",
        _ROOT_WS.memb_indent, "authorization-protocol-name", name_width, _ROOT_WS.equals,
        auth_protocol_name, _ROOT_WS.separator,
        settings.verbose ?
        fmt::format(
            "{}{: <{}}{}{} (length in bytes of authorization-protocol-data){}",
            _ROOT_WS.memb_indent, "d", name_width, _ROOT_WS.equals,
            _formatInteger( header->d ), _ROOT_WS.separator ) : "",
        // TBD may be security concerns with logging auth data
        _ROOT_WS.memb_indent, "authorization-protocol-data", name_width, _ROOT_WS.equals,
        header->d, _ROOT_WS.separator,
        _ROOT_WS.encl_indent
        );
    return bytes_parsed;
}
