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
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::errors::impl::SimpleError;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SimpleError::ENCODING_SZ );

    _ParsingOutputs error;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const SimpleError::Encoding* encoding {
        reinterpret_cast< const SimpleError::Encoding* >( data ) };
    error.bytes_parsed += sizeof( SimpleError::Encoding );
    assert( _ordered( encoding->header.error, byteswap ) ==
            SimpleError::ERROR );
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
            _formatVariable( encoding->header.error, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "(minor opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->minor_opcode, byteswap ), ws.separator,
        ws.memb_indent, "(major opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode, byteswap ), ws.separator,
        ws.encl_indent
        );
    return error;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::impl::ResourceIdError >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::errors::impl::ResourceIdError;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ResourceIdError::ENCODING_SZ );

    _ParsingOutputs error;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const ResourceIdError::Encoding* encoding {
        reinterpret_cast< const ResourceIdError::Encoding* >( data ) };
    error.bytes_parsed += sizeof( ResourceIdError::Encoding );
    assert( _ordered( encoding->header.error, byteswap ) ==
            ResourceIdError::ERROR );
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
            _formatVariable( encoding->header.error, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "(bad resource id)", memb_name_w, ws.equals,
        _formatVariable( encoding->bad_resource_id, byteswap ), ws.separator,
        ws.memb_indent, "(minor opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->minor_opcode, byteswap ), ws.separator,
        ws.memb_indent, "(major opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode, byteswap ), ws.separator,
        ws.encl_indent
        );
    return error;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Request >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Request;
    using protocol::errors::impl::SimpleError;
    static_assert( std::is_base_of_v< SimpleError, Request > );
    assert( data != nullptr );
    assert( sz >= sizeof( Request::Header ) );
    assert( _ordered( reinterpret_cast< const Request::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::REQUEST );
    return _parseError< SimpleError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Value >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::errors::Value;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= Value::ENCODING_SZ );

    _ParsingOutputs error;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const Value::Encoding* encoding {
        reinterpret_cast< const Value::Encoding* >( data ) };
    error.bytes_parsed += sizeof( Value::Encoding );
    assert( _ordered( encoding->header.error, byteswap ) ==
            Value::ERROR );
    assert( _ordered( encoding->header.code, conn->byteswap ) ==
            protocol::errors::codes::VALUE );
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
            _formatVariable( encoding->header.error, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "(bad value)", memb_name_w, ws.equals,
        _formatVariable( encoding->bad_value, byteswap ), ws.separator,
        ws.memb_indent, "(minor opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->minor_opcode, byteswap ), ws.separator,
        ws.memb_indent, "(major opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode, byteswap ), ws.separator,
        ws.encl_indent
        );
    return error;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Window >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Window;
    using protocol::errors::impl::ResourceIdError;
    static_assert( std::is_base_of_v< ResourceIdError, Window > );
    assert( data != nullptr );
    assert( sz >= sizeof( Window::Header ) );
    assert( _ordered( reinterpret_cast< const Window::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::WINDOW );
    return _parseError< ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Pixmap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Pixmap;
    using protocol::errors::impl::ResourceIdError;
    static_assert( std::is_base_of_v< ResourceIdError, Pixmap > );
    assert( data != nullptr );
    assert( sz >= sizeof( Pixmap::Header ) );
    assert( _ordered( reinterpret_cast< const Pixmap::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::PIXMAP );
    return _parseError< ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Atom >(
        Connection* conn, const uint8_t* data, [[maybe_unused]] const size_t sz ) {
    using protocol::errors::Atom;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= Atom::ENCODING_SZ );

    _ParsingOutputs error;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const Atom::Encoding* encoding {
        reinterpret_cast< const Atom::Encoding* >( data ) };
    error.bytes_parsed += sizeof( Atom::Encoding );
    assert( _ordered( encoding->header.error, byteswap ) ==
            Atom::ERROR );
    assert( _ordered( encoding->header.code, conn->byteswap ) ==
            protocol::errors::codes::ATOM );
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
            _formatVariable( encoding->header.error, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "code", memb_name_w, ws.equals,
            _formatVariable( encoding->header.code, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        ws.memb_indent, "(bad atom id)", memb_name_w, ws.equals,
        _formatVariable( encoding->bad_atom_id, byteswap ), ws.separator,
        ws.memb_indent, "(minor opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->minor_opcode, byteswap ), ws.separator,
        ws.memb_indent, "(major opcode)", memb_name_w, ws.equals,
        _formatVariable( encoding->major_opcode, byteswap ), ws.separator,
        ws.encl_indent
        );
    return error;
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Cursor >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Cursor;
    using protocol::errors::impl::ResourceIdError;
    static_assert( std::is_base_of_v< ResourceIdError, Cursor > );
    assert( data != nullptr );
    assert( sz >= sizeof( Cursor::Header ) );
    assert( _ordered( reinterpret_cast< const Cursor::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::CURSOR );
    return _parseError< ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Font >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Font;
    using protocol::errors::impl::ResourceIdError;
    static_assert( std::is_base_of_v< ResourceIdError, Font > );
    assert( data != nullptr );
    assert( sz >= sizeof( Font::Header ) );
    assert( _ordered( reinterpret_cast< const Font::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::FONT );
    return _parseError< ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Match >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Match;
    using protocol::errors::impl::SimpleError;
    static_assert( std::is_base_of_v< SimpleError, Match > );
    assert( data != nullptr );
    assert( sz >= sizeof( Match::Header ) );
    assert( _ordered( reinterpret_cast< const Match::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::MATCH );
    return _parseError< SimpleError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Drawable >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Drawable;
    using protocol::errors::impl::ResourceIdError;
    static_assert( std::is_base_of_v< ResourceIdError, Drawable > );
    assert( data != nullptr );
    assert( sz >= sizeof( Drawable::Header ) );
    assert( _ordered( reinterpret_cast< const Drawable::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::DRAWABLE );
    return _parseError< ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Access >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Access;
    using protocol::errors::impl::SimpleError;
    static_assert( std::is_base_of_v< SimpleError, Access > );
    assert( data != nullptr );
    assert( sz >= sizeof( Access::Header ) );
    assert( _ordered( reinterpret_cast< const Access::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::ACCESS );
    return _parseError< SimpleError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Alloc >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Alloc;
    using protocol::errors::impl::SimpleError;
    static_assert( std::is_base_of_v< SimpleError, Alloc > );
    assert( data != nullptr );
    assert( sz >= sizeof( Alloc::Header ) );
    assert( _ordered( reinterpret_cast< const Alloc::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::ALLOC );
    return _parseError< SimpleError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Colormap >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Colormap;
    using protocol::errors::impl::ResourceIdError;
    static_assert( std::is_base_of_v< ResourceIdError, Colormap > );
    assert( data != nullptr );
    assert( sz >= sizeof( Colormap::Header ) );
    assert( _ordered( reinterpret_cast< const Colormap::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::COLORMAP );
    return _parseError< ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::GContext >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::GContext;
    using protocol::errors::impl::ResourceIdError;
    static_assert( std::is_base_of_v< ResourceIdError, GContext > );
    assert( data != nullptr );
    assert( sz >= sizeof( GContext::Header ) );
    assert( _ordered( reinterpret_cast< const GContext::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::GCONTEXT );
    return _parseError< ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::IDChoice >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::IDChoice;
    using protocol::errors::impl::ResourceIdError;
    static_assert( std::is_base_of_v< ResourceIdError, IDChoice > );
    assert( data != nullptr );
    assert( sz >= sizeof( IDChoice::Header ) );
    assert( _ordered( reinterpret_cast< const IDChoice::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::IDCHOICE );
    return _parseError< ResourceIdError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Name >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Name;
    using protocol::errors::impl::SimpleError;
    static_assert( std::is_base_of_v< SimpleError, Name > );
    assert( data != nullptr );
    assert( sz >= sizeof( Name::Header ) );
    assert( _ordered( reinterpret_cast< const Name::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::NAME );
    return _parseError< SimpleError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Length >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Length;
    using protocol::errors::impl::SimpleError;
    static_assert( std::is_base_of_v< SimpleError, Length > );
    assert( data != nullptr );
    assert( sz >= sizeof( Length::Header ) );
    assert( _ordered( reinterpret_cast< const Length::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::LENGTH );
    return _parseError< SimpleError >( conn, data, sz );
}

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseError<
    protocol::errors::Implementation >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using protocol::errors::Implementation;
    using protocol::errors::impl::SimpleError;
    static_assert( std::is_base_of_v< SimpleError, Implementation > );
    assert( data != nullptr );
    assert( sz >= sizeof( Implementation::Header ) );
    assert( _ordered( reinterpret_cast< const Implementation::Header* >(
                          data )->code, conn->byteswap ) ==
            protocol::errors::codes::IMPLEMENTATION );
    return _parseError< SimpleError >( conn, data, sz );
}
