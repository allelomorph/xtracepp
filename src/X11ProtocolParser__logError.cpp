#include <string_view>

#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/errors.hpp"


template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Error >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Error;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= Error::ENCODING_SZ );

    _ParsingOutputs error;
    const _Whitespace& ws { _ROOT_WS };
    const Error::Encoding* encoding {
        reinterpret_cast< const Error::Encoding* >( data ) };
    error.bytes_parsed += sizeof( Error::Encoding );
    assert( encoding->header.error == Error::ERROR );
    const uint8_t code { encoding->header.code };
    assert( code >= protocol::errors::codes::MIN &&
            code <= protocol::errors::codes::MAX );
    assert( error.bytes_parsed == Error::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "(bad resource id)" ) ) - 1 );
    std::string bad_resource_str;
    switch ( protocol::errors::value_types.at( code ) ) {
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
    error.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "error", memb_name_w, ws.equals,
            _formatVariable( encoding->header.error ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
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
    return error;
}


size_t
X11ProtocolParser::_logError(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Error;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= sizeof( Error::Encoding ) );

    const Error::Encoding* encoding {
        reinterpret_cast< const Error::Encoding* >( data ) };
    assert( encoding->header.error == Error::ERROR );
    const uint8_t code { encoding->header.code };

    _ParsingOutputs error;
    switch ( code ) {
    case protocol::errors::codes::REQUEST:         //  1
        [[fallthrough]];
    case protocol::errors::codes::VALUE:           //  2
        [[fallthrough]];
    case protocol::errors::codes::WINDOW:          //  3
        [[fallthrough]];
    case protocol::errors::codes::PIXMAP:          //  4
        [[fallthrough]];
    case protocol::errors::codes::ATOM:            //  5
        [[fallthrough]];
    case protocol::errors::codes::CURSOR:          //  6
        [[fallthrough]];
    case protocol::errors::codes::FONT:            //  7
        [[fallthrough]];
    case protocol::errors::codes::MATCH:           //  8
        [[fallthrough]];
    case protocol::errors::codes::DRAWABLE:        //  9
        [[fallthrough]];
    case protocol::errors::codes::ACCESS:          // 10
        [[fallthrough]];
    case protocol::errors::codes::ALLOC:           // 11
        [[fallthrough]];
    case protocol::errors::codes::COLORMAP:        // 12
        [[fallthrough]];
    case protocol::errors::codes::GCONTEXT:        // 13
        [[fallthrough]];
    case protocol::errors::codes::IDCHOICE:        // 14
        [[fallthrough]];
    case protocol::errors::codes::NAME:            // 15
        [[fallthrough]];
    case protocol::errors::codes::LENGTH:          // 16
        [[fallthrough]];
    case protocol::errors::codes::IMPLEMENTATION:  // 17
        error = _parseError<
            protocol::errors::Error >( conn, data, sz );
        break;
    default:
        // TBD extension error code
        break;
    }
    fmt::println( settings.log_fs,
                  "C{:03d}:{:04d}B:{}:S{:05d}: Error {}({}): {}",
                  conn->id, error.bytes_parsed, SERVER_TO_CLIENT,
                  encoding->header.sequence_num,
                  protocol::errors::names.at( code ), code,
                  error.str );
    // presume that no more messages will relate to this request
    conn->unregisterRequest( encoding->header.sequence_num );
    return error.bytes_parsed;
}
