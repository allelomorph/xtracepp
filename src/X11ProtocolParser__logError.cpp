#include <string_view>

#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/errors.hpp"


size_t X11ProtocolParser::_logError(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Error;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= Error::ENCODING_SZ );

    size_t bytes_parsed {};
    const Error::Encoding* encoding {
        reinterpret_cast< const Error::Encoding* >( data ) };
    bytes_parsed += sizeof( Error::Encoding );
    assert( encoding->header.error == Error::ERROR );
    // TBD will change with extensions
    assert( encoding->header.code >= protocol::errors::codes::MIN &&
            encoding->header.code <= protocol::errors::codes::MAX );

    const _Whitespace& ws { _ROOT_WS };
    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "(minor opcode)" ) ) - 1 );
    std::string bad_resource_str {};
    switch ( protocol::errors::value_types[ encoding->header.code ] ) {
    case protocol::errors::ValueTypeCode::VALUE:
        bad_resource_str = fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(bad value)", memb_name_w, ws.equals,
            _formatVariable( encoding->bad_value ), ws.separator );
        break;
    case protocol::errors::ValueTypeCode::RESOURCE_ID:
        bad_resource_str = fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(bad resource id)", memb_name_w, ws.equals,
            _formatVariable( encoding->bad_resource_id ), ws.separator );
        break;
    case protocol::errors::ValueTypeCode::ATOM_ID:
        bad_resource_str = fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(bad atom id)", memb_name_w, ws.equals,
            _formatVariable( encoding->bad_atom_id ), ws.separator );
        break;
    default:
        break;
    }
    fmt::println(
        settings.log_fs,
        "C{:03d}:{:04d}B:{}:S{:05d}: Error {}({}): "
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        conn->id, bytes_parsed, SERVER_TO_CLIENT, encoding->header.sequence_num,
        protocol::errors::names[ encoding->header.code ], encoding->header.code,
        ws.separator,
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "error", memb_name_w, ws.equals,
            _formatVariable( encoding->header.error ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num ), ws.separator ),
        bad_resource_str,
        ws.memb_indent, "(minor opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->minor_opcode ), ws.separator,
        ws.memb_indent, "(major opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode ), ws.separator,
        ws.encl_indent
        );
    conn->unregisterRequest( encoding->header.sequence_num );
    assert( bytes_parsed == protocol::errors::Error::ENCODING_SZ );
    return bytes_parsed;
}
