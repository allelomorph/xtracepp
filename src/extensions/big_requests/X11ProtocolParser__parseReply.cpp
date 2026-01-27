#include <cassert>
#include <cstdint>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/extensions/requests.hpp"  // Reply::DEFAULT_ENCODING_SZ
#include "protocol/extensions/big_requests/requests.hpp"


namespace ext = protocol::extensions;

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseReply<
    ext::big_requests::requests::BigReqEnable::Reply >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using ext::big_requests::requests::BigReqEnable;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= ext::requests::Reply::DEFAULT_ENCODING_SZ );

    _ParsingOutputs reply;
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    const BigReqEnable::Reply::Encoding* encoding {
        reinterpret_cast< const BigReqEnable::Reply::Encoding* >( data ) };
    reply.bytes_parsed += sizeof( BigReqEnable::Reply::Encoding );
    assert( _ordered( encoding->header.reply, byteswap ) ==
            BigReqEnable::Reply::REPLY );
    assert( _ordered( encoding->header.extra_aligned_units, byteswap ) ==
            alignment.units(
                reply.bytes_parsed - ext::requests::Reply::DEFAULT_ENCODING_SZ ) );
    // BIG-REQUESTS is a special case in that it is not considered activated
    //   until the server replies to request BigReqEnable
    assert( !conn->extensions.active(
                protocol::extensions::big_requests::name ) );
    conn->extensions.activate(
        protocol::extensions::big_requests::name );

    const uint32_t memb_name_w (
        !ws.multiline ? 0 : sizeof( "maximum-request-length" ) - 1 );
    reply.str = fmt::format(
        "{{{}"
        "{}{}{}"
        "{}{: <{}}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "reply", memb_name_w, ws.equals,
            _formatVariable( encoding->header.reply, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(sequence number)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.sequence_num, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(extra aligned units)", memb_name_w, ws.equals,
            _formatVariable( encoding->header.extra_aligned_units, byteswap ), ws.separator ),
        ws.memb_indent, "maximum-request-length", memb_name_w, ws.equals,
        _formatVariable( encoding->maximum_request_length, byteswap ), ws.separator,
        ws.encl_indent
        );
    return reply;
}
