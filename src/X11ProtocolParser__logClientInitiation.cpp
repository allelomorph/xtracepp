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
    const _Whitespace& ws { _ROOT_WS };
    using namespace protocol::connection_setup;
    const ClientInitiation::Header* header {
        reinterpret_cast< const ClientInitiation::Header* >( data ) };
    assert( header->byte_order == ClientInitiation::MSBFIRST ||
            header->byte_order == ClientInitiation::LSBFIRST );
    // TBD error if protocol is not 11.0
    bytes_parsed += sizeof( ClientInitiation::Header );
    // followed by STRING8 authorization-protocol-name
    const std::string_view auth_protocol_name {
        reinterpret_cast< const char* >( data + bytes_parsed ), header->name_len };
    bytes_parsed += _pad( header->name_len );
    // followed by STRING8 authorization-protocol-data
    // TBD may be security concerns with logging auth data
    _ParsingOutputs authorization_protocol_data_outputs {
        _parseLISTof< protocol::CARD8 >(
            data + bytes_parsed, sz - bytes_parsed, header->data_len,
            ws.nested() ) };
    bytes_parsed += authorization_protocol_data_outputs.bytes_parsed;
    assert( bytes_parsed == sz );

    const uint32_t name_width (
        settings.multiline ? sizeof( "authorization-protocol-name" ) - 1 : 0 );
    fmt::println(
        settings.log_fs,
        "C{:03d}:{:04d}B:{}: client \"{}\" attempting connection: "
        "{{{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}{}"                       // name_len, data_len
        "{}{: <{}}{}{:?}{}"          // authorization-protocol-name
        "{}{: <{}}{}({:d} bytes){}"  // authorization-protocol-data (hidden)
        "{}}}",
        conn->id, bytes_parsed, _CLIENT_TO_SERVER, conn->client_desc,
        ws.separator,
        ws.memb_indent, "byte-order", name_width, ws.equals,
        _formatInteger( ( header->byte_order == ClientInitiation::MSBFIRST ) ? 0 : 1,
                        protocol::enum_names::image_byte_order ), ws.separator,
        ws.memb_indent, "protocol-major-version", name_width, ws.equals,
        _formatInteger( header->protocol_major_version ), ws.separator,
        ws.memb_indent, "protocol-minor-version", name_width, ws.equals,
        _formatInteger( header->protocol_minor_version ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(authorization-protocol-name length in bytes)",
            name_width, ws.equals,
            _formatInteger( header->name_len ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(authorization-protocol-data length in bytes)",
            name_width, ws.equals,
            _formatInteger( header->data_len ), ws.separator ),
        ws.memb_indent, "authorization-protocol-name", name_width, ws.equals,
        auth_protocol_name, ws.separator,
        // TBD may be security concerns with logging auth data
        ws.memb_indent, "authorization-protocol-data", name_width, ws.equals,
        header->data_len, ws.separator,
        ws.encl_indent
        );
    return bytes_parsed;
}
