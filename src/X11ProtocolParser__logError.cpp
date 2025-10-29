#include <string_view>

#include <cassert>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/errors.hpp"


template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::impl::SimpleError >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::impl::SimpleError;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SimpleError::ENCODING_SZ );

    _ParsingOutputs error;
    const _Whitespace& ws { _ROOT_WS };
    const SimpleError::Encoding* encoding {
        reinterpret_cast< const SimpleError::Encoding* >( data ) };
    error.bytes_parsed += sizeof( SimpleError::Encoding );
    assert( encoding->header.error == SimpleError::ERROR );
    const uint8_t code { encoding->header.code };
    assert( code >= protocol::errors::codes::MIN &&
            code <= protocol::errors::codes::MAX );
    assert( error.bytes_parsed == SimpleError::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "(minor opcode)" ) ) - 1 );
    error.str = fmt::format(
        "{{{}"
        "{}{}{}"
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
        ws.memb_indent, "(minor opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->minor_opcode ), ws.separator,
        ws.memb_indent, "(major opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode ), ws.separator,
        ws.encl_indent
        );
    return error;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::impl::ResourceIdError >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::impl::ResourceIdError;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ResourceIdError::ENCODING_SZ );

    _ParsingOutputs error;
    const _Whitespace& ws { _ROOT_WS };
    const ResourceIdError::Encoding* encoding {
        reinterpret_cast< const ResourceIdError::Encoding* >( data ) };
    error.bytes_parsed += sizeof( ResourceIdError::Encoding );
    assert( encoding->header.error == ResourceIdError::ERROR );
    const uint8_t code { encoding->header.code };
    assert( code >= protocol::errors::codes::MIN &&
            code <= protocol::errors::codes::MAX );
    assert( error.bytes_parsed == ResourceIdError::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "(bad resource id)" ) ) - 1 );
    error.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
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
        ws.memb_indent, "(bad resource id)", memb_name_w, ws.equals,
        _formatVariable( encoding->bad_resource_id ), ws.separator,
        ws.memb_indent, "(minor opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->minor_opcode ), ws.separator,
        ws.memb_indent, "(major opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode ), ws.separator,
        ws.encl_indent
        );
    return error;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Request >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::SimpleError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Value >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Value;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= Value::ENCODING_SZ );

    _ParsingOutputs error;
    const _Whitespace& ws { _ROOT_WS };
    const Value::Encoding* encoding {
        reinterpret_cast< const Value::Encoding* >( data ) };
    error.bytes_parsed += sizeof( Value::Encoding );
    assert( encoding->header.error == Value::ERROR );
    const uint8_t code { encoding->header.code };
    assert( code >= protocol::errors::codes::MIN &&
            code <= protocol::errors::codes::MAX );
    assert( error.bytes_parsed == Value::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "(bad resource id)" ) ) - 1 );
    error.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
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
        ws.memb_indent, "(bad value)", memb_name_w, ws.equals,
        _formatVariable( encoding->bad_value ), ws.separator,
        ws.memb_indent, "(minor opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->minor_opcode ), ws.separator,
        ws.memb_indent, "(major opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode ), ws.separator,
        ws.encl_indent
        );
    return error;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Window >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Pixmap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Atom >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Atom;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= Atom::ENCODING_SZ );

    _ParsingOutputs error;
    const _Whitespace& ws { _ROOT_WS };
    const Atom::Encoding* encoding {
        reinterpret_cast< const Atom::Encoding* >( data ) };
    error.bytes_parsed += sizeof( Atom::Encoding );
    assert( encoding->header.error == Atom::ERROR );
    const uint8_t code { encoding->header.code };
    assert( code >= protocol::errors::codes::MIN &&
            code <= protocol::errors::codes::MAX );
    assert( error.bytes_parsed == Atom::ENCODING_SZ );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : ( settings.verbose ?
                              sizeof( "(sequence number)" ) :
                              sizeof( "(bad resource id)" ) ) - 1 );
    error.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}{}{: <{}}{}{}{}{}{: <{}}{}{}{}"
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
        ws.memb_indent, "(bad atom id)", memb_name_w, ws.equals,
        _formatVariable( encoding->bad_atom_id ), ws.separator,
        ws.memb_indent, "(minor opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->minor_opcode ), ws.separator,
        ws.memb_indent, "(major opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode ), ws.separator,
        ws.encl_indent
        );
    return error;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Cursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Font >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Match >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::SimpleError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Drawable >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Access >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::SimpleError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Alloc >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::SimpleError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Colormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::GContext >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::IdChoice >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Name >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::SimpleError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Length >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::SimpleError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Implementation >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    return _parseError<
        protocol::errors::impl::SimpleError >( conn, data, sz );
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
        error = _parseError<
            protocol::errors::Request >( conn, data, sz );
        break;
    case protocol::errors::codes::VALUE:           //  2
        error = _parseError<
            protocol::errors::Value >( conn, data, sz );
        break;
    case protocol::errors::codes::WINDOW:          //  3
        error = _parseError<
            protocol::errors::Window >( conn, data, sz );
        break;
    case protocol::errors::codes::PIXMAP:          //  4
        error = _parseError<
            protocol::errors::Pixmap >( conn, data, sz );
        break;
    case protocol::errors::codes::ATOM:            //  5
        error = _parseError<
            protocol::errors::Atom >( conn, data, sz );
        break;
    case protocol::errors::codes::CURSOR:          //  6
        error = _parseError<
            protocol::errors::Cursor >( conn, data, sz );
        break;
    case protocol::errors::codes::FONT:            //  7
        error = _parseError<
            protocol::errors::Font >( conn, data, sz );
        break;
    case protocol::errors::codes::MATCH:           //  8
        error = _parseError<
            protocol::errors::Match >( conn, data, sz );
        break;
    case protocol::errors::codes::DRAWABLE:        //  9
        error = _parseError<
            protocol::errors::Drawable >( conn, data, sz );
        break;
    case protocol::errors::codes::ACCESS:          // 10
        error = _parseError<
            protocol::errors::Access >( conn, data, sz );
        break;
    case protocol::errors::codes::ALLOC:           // 11
        error = _parseError<
            protocol::errors::Alloc >( conn, data, sz );
        break;
    case protocol::errors::codes::COLORMAP:        // 12
        error = _parseError<
            protocol::errors::Colormap >( conn, data, sz );
        break;
    case protocol::errors::codes::GCONTEXT:        // 13
        error = _parseError<
            protocol::errors::GContext >( conn, data, sz );
        break;
    case protocol::errors::codes::IDCHOICE:        // 14
        error = _parseError<
            protocol::errors::IdChoice >( conn, data, sz );
        break;
    case protocol::errors::codes::NAME:            // 15
        error = _parseError<
            protocol::errors::Name >( conn, data, sz );
        break;
    case protocol::errors::codes::LENGTH:          // 16
        error = _parseError<
            protocol::errors::Length >( conn, data, sz );
        break;
    case protocol::errors::codes::IMPLEMENTATION:  // 17
        error = _parseError<
            protocol::errors::Implementation >( conn, data, sz );
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
