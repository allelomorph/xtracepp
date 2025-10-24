#include <string_view>

#include <cctype>      // isprint

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/connection_setup.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/version.hpp"


size_t X11ProtocolParser::_logConnInitiation(
    const Connection* conn, const uint8_t* data, const size_t sz ) {
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
    // TBD version error instead of assert?
    assert( encoding->protocol_major_version == protocol::MAJOR_VERSION );
    assert( encoding->protocol_minor_version == protocol::MINOR_VERSION );
    bytes_parsed += sizeof( ConnInitiation::Encoding );
    // followed by STRING8 authorization-protocol-name
    const std::string_view auth_protocol_name {
        reinterpret_cast< const char* >( data + bytes_parsed ),
        encoding->name_len };
    bytes_parsed += alignment.pad( encoding->name_len );
    // followed by STRING8 authorization-protocol-data
    const _ParsingOutputs authorization_protocol_data {
        _parseLISTof< protocol::CARD8 >(
            data + bytes_parsed, encoding->data_len,
            encoding->data_len, ws.nested( _Whitespace::FORCE_SINGLELINE ) ) };
    bytes_parsed += alignment.pad( encoding->data_len );
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
        _formatInteger( ( encoding->byte_order == ConnInitiation::MSBFIRST ) ? 0 : 1,
                        { ConnInitiation::byte_order_names } ), ws.separator,
        ws.memb_indent, "protocol-major-version", memb_name_w, ws.equals,
        _formatInteger( encoding->protocol_major_version ), ws.separator,
        ws.memb_indent, "protocol-minor-version", memb_name_w, ws.equals,
        _formatInteger( encoding->protocol_minor_version ), ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(authorization-protocol-name length)",
            memb_name_w, ws.equals,
            _formatInteger( encoding->name_len ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(authorization-protocol-data length)",
            memb_name_w, ws.equals,
            _formatInteger( encoding->data_len ), ws.separator ),
        ws.memb_indent, "authorization-protocol-name", memb_name_w, ws.equals,
        auth_protocol_name, ws.separator,
        ws.memb_indent, "authorization-protocol-data", memb_name_w, ws.equals,
        encoding->data_len, ws.separator,
        ws.encl_indent
        );
    return bytes_parsed;
}
