#include <string_view>

#include <cctype>      // isprint

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/connection_setup.hpp"
#include "protocol/enum_names.hpp"


size_t X11ProtocolParser::_logConnInitiation(
    const Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( protocol::connection_setup::ConnInitiation::Encoding ) );

    size_t bytes_parsed {};
    const _Whitespace& ws { _ROOT_WS };
    using namespace protocol::connection_setup;
    const ConnInitiation::Encoding* encoding {
        reinterpret_cast< const ConnInitiation::Encoding* >( data ) };
    assert( encoding->byte_order == ConnInitiation::MSBFIRST ||
            encoding->byte_order == ConnInitiation::LSBFIRST );
    // TBD error if protocol is not 11.0
    bytes_parsed += sizeof( ConnInitiation::Encoding );
    // followed by STRING8 authorization-protocol-name
    const std::string_view auth_protocol_name {
        reinterpret_cast< const char* >( data + bytes_parsed ), encoding->name_len };
    bytes_parsed += _pad( encoding->name_len );
    // followed by STRING8 authorization-protocol-data
    // TBD may be security concerns with logging auth data
    _ParsingOutputs authorization_protocol_data_outputs {
        _parseLISTof< protocol::CARD8 >(
            data + bytes_parsed, sz - bytes_parsed, encoding->data_len,
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
        _formatInteger( ( encoding->byte_order == ConnInitiation::MSBFIRST ) ? 0 : 1,
                        ConnInitiation::byte_order_names ), ws.separator,
        ws.memb_indent, "protocol-major-version", name_width, ws.equals,
        _formatInteger( encoding->protocol_major_version ), ws.separator,
        ws.memb_indent, "protocol-minor-version", name_width, ws.equals,
        _formatInteger( encoding->protocol_minor_version ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(authorization-protocol-name length in bytes)",
            name_width, ws.equals,
            _formatInteger( encoding->name_len ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(authorization-protocol-data length in bytes)",
            name_width, ws.equals,
            _formatInteger( encoding->data_len ), ws.separator ),
        ws.memb_indent, "authorization-protocol-name", name_width, ws.equals,
        auth_protocol_name, ws.separator,
        // TBD may be security concerns with logging auth data
        ws.memb_indent, "authorization-protocol-data", name_width, ws.equals,
        encoding->data_len, ws.separator,
        ws.encl_indent
        );
    return bytes_parsed;
}
