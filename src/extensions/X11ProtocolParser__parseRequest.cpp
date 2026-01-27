#include <cassert>
#include <cstdint>

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/extensions/requests.hpp"
#include "protocol/extensions/big_requests.hpp"  // EXTENDED_LENGTH_FLAG


namespace ext = protocol::extensions;

template<>
X11ProtocolParser::_ParsingOutputs
X11ProtocolParser::_parseRequest<
    ext::requests::impl::SimpleRequest >(
        Connection* conn, const uint8_t* data, const size_t sz ) {
    using ext::requests::impl::SimpleRequest;
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= SimpleRequest::BASE_ENCODING_SZ );

    _ParsingOutputs request {};
    const _Whitespace& ws { _ROOT_WS };
    const bool byteswap { conn->byteswap };
    _RequestFixedEncoding< SimpleRequest > fe { conn, data, sz };
    request.bytes_parsed += fe.bytes_parsed;
    assert( fe.header_only );
    assert( fe.big_request ?
            _ordered( fe.big_length->tl_aligned_units, byteswap ) :
            _ordered( fe.length->tl_aligned_units, byteswap ) ==
            alignment.units( request.bytes_parsed ) );

    const uint32_t memb_name_w (
        !ws.multiline     ? 0 :
        fe.big_request    ? sizeof( "(extended length flag)" ) - 1 :
                            sizeof( "(total aligned units)" ) - 1 );
    request.str = fmt::format(
        "{{{}"
        "{}{}{}{}"
        "{}}}",
        ws.separator,
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "major-opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->major_opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "minor-opcode", memb_name_w, ws.equals,
            _formatVariable( fe.prefix->minor_opcode, byteswap ), ws.separator ),
        !settings.verbose ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, fe.big_request ? "(extended length flag)" : "(total aligned units)",
            memb_name_w, ws.equals,
            fe.big_request ? _formatVariable( fe.big_length->extended_length_flag, byteswap ) :
                             _formatVariable( fe.length->tl_aligned_units, byteswap ),
            ws.separator ),
        !settings.verbose || !fe.big_request ? "" : fmt::format(
            "{}{: <{}}{}{}{}",
            ws.memb_indent, "(total aligned units)", memb_name_w, ws.equals,
            _formatVariable( fe.big_length->tl_aligned_units, byteswap ), ws.separator ),
        ws.encl_indent
        );
    return request;
}
