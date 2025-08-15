#include <string_view>

#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/errors.hpp"


size_t X11ProtocolParser::_logServerError(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= protocol::errors::ENCODING_SZ ); // TBD

    size_t bytes_parsed {};
    const protocol::errors::Encoding* encoding {
        reinterpret_cast< const protocol::errors::Encoding* >( data ) };
    bytes_parsed += sizeof( protocol::errors::Encoding );

    assert( encoding->error == 0 );
    const uint8_t code { encoding->code };
    assert( code >= protocol::errors::codes::MIN &&
            code <= protocol::errors::codes::MAX );

    const std::string_view struct_indent {
        _multiline ? _tabIndent( 0 ) : "" };
    const std::string_view memb_indent {
        _multiline ? _tabIndent( 1 ) : "" };
    const uint32_t name_width (
        _multiline ? sizeof( "sequence number" ) - 1 : 0 );
    std::string bad_resource_str {};
    switch ( code ) {
    case protocol::errors::codes::VALUE:     //  2
        bad_resource_str = fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "bad value", name_width, _equals,
            _formatInteger( encoding->bad_value ), _separator );
        break;
    case protocol::errors::codes::ATOM:      //  5
        bad_resource_str = fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "bad atom id", name_width, _equals,
            _formatInteger( encoding->bad_atom_id ), _separator );
        break;
    case protocol::errors::codes::WINDOW:    //  3
    case protocol::errors::codes::PIXMAP:    //  4
    case protocol::errors::codes::CURSOR:    //  6
    case protocol::errors::codes::FONT:      //  7
    case protocol::errors::codes::DRAWABLE:  //  9
    case protocol::errors::codes::COLORMAP:  // 12
    case protocol::errors::codes::GCONTEXT:  // 13
    case protocol::errors::codes::IDCHOICE:  // 14
        bad_resource_str = fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "bad resource id", name_width, _equals,
            _formatInteger( encoding->bad_resource_id ), _separator );
        break;
    default:
        break;
    }
    fmt::println(
        _log_fs,
        "{:03d}:<:server error {}({}) "
        "{{{}"
        "{}{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
        "{}}}",
        conn->id, protocol::errors::names[code], code,
        _separator,
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "error", name_width, _equals,
            _formatInteger( encoding->error ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "code", name_width, _equals,
            _formatInteger( encoding->code ), _separator ),
        !_verbose ? "" :
        fmt::format(
            "{}{: <{}}{}{}{}",
            memb_indent, "sequence number", name_width, _equals,
            _formatInteger( encoding->sequence_number ), _separator ),
        bad_resource_str,
        memb_indent, "minor-opcode", name_width, _equals,
        _formatInteger( encoding->minor_opcode ), _separator,
        memb_indent, "major-opcode", name_width, _equals,
        _formatInteger( encoding->major_opcode ), _separator,
        struct_indent
        );
    assert( bytes_parsed == protocol::errors::ENCODING_SZ );
    return bytes_parsed;
}
