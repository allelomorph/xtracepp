#include <string_view>

#include <cctype>      // isprint

#include <fmt/format.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "Settings.hpp"
#include "protocol/common_types.hpp"
#include "protocol/connection_setup.hpp"


namespace {

// TBD for debugging buffer parsing
void __bufferHexDump( const uint8_t* data, const size_t sz ) {
    assert( data != nullptr );

    static constexpr int BYTES_PER_ROW   { 16 };
    static constexpr int BYTES_PER_GROUP { 8 };

    const uint8_t* _data { data };
    for ( size_t bytes_to_parse { sz }, address_index {}; bytes_to_parse > 0;
        address_index += BYTES_PER_ROW ) {
        std::string group1, group2, as_ascii;
        size_t group_sz {
            ( bytes_to_parse < BYTES_PER_GROUP ) ? bytes_to_parse : BYTES_PER_GROUP };
        for ( size_t i {}; i < group_sz;
              ++i, ++_data, --bytes_to_parse ) {
            group1 += fmt::format( "{:02X}{}",
                                   *_data, ( i < group_sz - 1 ) ? " " : "" );
            as_ascii += fmt::format( "{:c}",
                                     ( std::isprint( *_data ) ) ? *_data : '.' );
        }
        group_sz =
                ( bytes_to_parse < BYTES_PER_GROUP ) ? bytes_to_parse : BYTES_PER_GROUP;
        for ( size_t i {}; i < group_sz;
              ++i, ++_data, --bytes_to_parse ) {
            group2 += fmt::format( "{:02X}{}",
                                   *_data, ( i < group_sz - 1 ) ? " " : "" );
            as_ascii += fmt::format( "{:c}",
                                     ( std::isprint( *_data ) ) ? *_data : '.' );
        }
        std::cerr << fmt::format(
            "{:08x}  {: <23}  {: <23}  {}\n",
            address_index, group1, group2, as_ascii );
    }
}

}  // namespace

size_t X11ProtocolParser::_logConnectionInitiation(
    const Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );

    const uint8_t* _data { data };
    using namespace protocol::connection_setup;
    const ClientInitiation::Header* header {
        reinterpret_cast< const ClientInitiation::Header* >( _data )
    };
    // TBD error if protocol is not 11.0
    _data += sizeof( ClientInitiation::Header );
    // STRING8 of d bytes authorization-protocol-name
    std::string_view authorization_protocol_name {
        reinterpret_cast< const char* >( _data ), header->n };
    _data += _pad( header->n );
    // STRING8 of d bytes authorization-protocol-data (may not be printable)
    // std::string_view authorization_protocol_data {
    //     reinterpret_cast< const char* >( _data ), header->d };
    _data += _pad( header->d );
    assert( _data - data == sz );

    // TBD asserts close log file without logging anything
    assert( header->byte_order == 'B' || header->byte_order == 'l' );
    _log_os << fmt::format(
        "{:03d}:<:client \"{}\" requesting connection: byte-order: {}, "
        "X protocol version: {:d}.{:d}, auth protocol: {}, auth data: {:d} bytes\n",
        conn->id, conn->client_desc,
        ( header->byte_order == 'B' ) ? "MSB first" : "LSB first",
        header->protocol_major_version, header->protocol_minor_version,
        authorization_protocol_name, header->d );
    return sz;
}

size_t X11ProtocolParser::_logClientMessage(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    return sz;
}

size_t X11ProtocolParser::_logServerResponse(
    Connection* conn, const uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    const uint8_t* _data { data };
    const uint8_t success { *_data };
    _data += sizeof( uint8_t );
    using namespace protocol::connection_setup;
    switch ( success ) {
    case FAILED: {
        const ServerRefusal::Header* header {
            reinterpret_cast< const ServerRefusal::Header* >( _data ) };
        _data += sizeof( ServerRefusal::Header );
        // STRING8 of n bytes reason
        std::string_view reason {
            reinterpret_cast< const char* >( _data ), header->n };
        _data += _pad( header->n );
        _log_os << fmt::format(
            "{:03d}:>:server refused connection: X protocol version: {:d}.{:d}, "
            "reason: \"{}\"\n",
            conn->id,  header->protocol_major_version,
            header->protocol_minor_version, reason );
        conn->status = Connection::FAILED;
    }
        break;
    case AUTHENTICATE: {
        const ServerRequireFurtherAuthentication::Header* header {
            reinterpret_cast<
                const ServerRequireFurtherAuthentication::Header* >( _data ) };
        _data += sizeof( ServerRequireFurtherAuthentication::Header );
        const size_t reason_padded_sz { header->reason_aligned_units * _ALIGN };
        // STRING8 of n bytes reason
        std::string_view reason {
            reinterpret_cast< const char* >( _data ), reason_padded_sz };
        _data += reason_padded_sz;
        _log_os << fmt::format(
            "{:03d}:>:server requested further authentication: reason: \"{}\"\n",
            conn->id, reason_padded_sz );
        conn->status = Connection::AUTHENTICATION;
    }
        break;
    case SUCCESS: {
        __bufferHexDump( _data, sz );
        const ServerAcceptance::Header* header {
            reinterpret_cast< const ServerAcceptance::Header* >( _data ) };
        _data += sizeof( ServerAcceptance::Header );
        // assert( header->image_byte_order < 2 );  // TBD enum
        // assert( header->bitmap_format_bit_order < 2 );  // TBD enum
        _log_os << fmt::format(
            R"({:03d}:>:server accepted connection
    X protocol version:          {:d}.{:d}
    release_number:              {}
    release_id_base:             {}
    release_id_mask:             {:#x}
    maximum_request_length:      {:d}
    image_byte_order:            {:d}
    bitmap_format_bit_order:     {:d}
    bitmap_format_scanline_unit: {:d}
    bitmap_format_scanline_pad:  {:d}
    min_keycode:                 {:d}
    max_keycode:                 {:d}
)",
            conn->id, header->protocol_major_version,
            header->protocol_minor_version,
            header->release_number,
            header->release_id_base,
            header->release_id_mask,
            header->maximum_request_length,
            header->image_byte_order,
            header->bitmap_format_bit_order,
            header->bitmap_format_scanline_unit,
            header->bitmap_format_scanline_pad,
            header->min_keycode,
            header->max_keycode ) << std::endl;
        // followed by STRING8 vendor of v bytes, plus p bytes to round up to multiple of 4
        // std::string_view vendor {
        //     reinterpret_cast< const char* >( _data ), header->v };
        // _data += _pad( header->v );
        // _log_os << " vendor: " << vendor << std::endl;
        // followed by LISTofFORMAT pixmap-formats of n*sizeof(Format) bytes
        // _log_os << "\tpixmap_formats:\n";
        // for ( uint8_t pf_ct { header->n }, pf_i {}; pf_i < pf_ct; ++pf_i ) {
        //     const ServerAcceptance::Format* pf {
        //         reinterpret_cast< const ServerAcceptance::Format* >( _data ) };
        //     _data += sizeof( ServerAcceptance::Format );
        //     _log_os << "\t\t{" <<
        //         " depth: " << std::dec << pf->depth <<
        //         " bits_per_pixel: " << std::dec << pf->bits_per_pixel <<
        //         " scanline_pad: " << std::dec << pf->scanline_pad <<
        //         "}\n";
        // }
        // // followed by LISTofSCREEN roots of m bytes (m is always a multiple of 4)
        // _log_os << "\troots:\n";
        // for ( uint8_t s_ct { header->r }, s_i {}; s_i < s_ct; ++s_i ) {
        //     const ServerAcceptance::Screen::Header* screen {
        //         reinterpret_cast< const ServerAcceptance::Screen::Header* >( _data ) };
        //     _data += sizeof( ServerAcceptance::Screen::Header );
        //     _log_os << "\t\t{" <<
        //         " root: " << screen->root <<
        //         " default_colormap: " << screen->default_colormap <<
        //         " white_pixel: " << screen->white_pixel <<
        //         " black_pixel: " << screen->black_pixel <<
        //         " current_input_masks: " << std::hex << "0x" << screen->current_input_masks <<
        //         " width_in_pixels: " << std::dec << screen->width_in_pixels <<
        //         " height_in_pixels: " << std::dec << screen->height_in_pixels <<
        //         " width_in_millimeters: " << std::dec << screen->width_in_millimeters <<
        //         " height_in_millimeters: " << std::dec << screen->height_in_millimeters <<
        //         " min_installed_maps: " << std::dec << screen->min_installed_maps <<
        //         " max_installed_maps: " << std::dec << screen->max_installed_maps <<
        //         " root_visual: " << screen->root_visual <<
        //         " backing_stores: " << std::dec << screen->backing_stores <<
        //         " save_unders: " << std::dec << screen->save_unders <<
        //         " root_depth: " << std::dec << screen->root_depth;
        //     // followed by LISTofDEPTH allowed-depths of n bytes (n is always a multiple of 4) ((d * sizeof(_DepthHeader) + lists of VISUALTYPE) )
        //     _log_os << " allowed_depths: {";
        //     for ( uint8_t d_ct { screen->d }, d_i {}; d_i < d_ct; ++d_i ) {
        //         const ServerAcceptance::Screen::Depth::Header* depth {
        //             reinterpret_cast< const ServerAcceptance::Screen::Depth::Header* >( _data ) };
        //         _data += sizeof( ServerAcceptance::Screen::Depth::Header );
        //         _log_os << "\t\t\t{" <<
        //             " depth: " << std::dec << depth->depth;
        //         _log_os << " visuals: {";
        //         for ( uint16_t v_ct { depth->n }, v_i {}; v_i < v_ct; ++v_i ) {
        //             const ServerAcceptance::Screen::Depth::VisualType* visual {
        //                 reinterpret_cast< const ServerAcceptance::Screen::Depth::VisualType* >( _data ) };
        //             _data += sizeof( ServerAcceptance::Screen::Depth::VisualType );
        //             _log_os << "\t\t\t\t{" <<
        //                 " visual_id: " << visual->visual_id <<
        //                 " class_: " << std::dec << visual->class_ <<
        //                 " bits_per_rgb_value: " << std::dec << visual->bits_per_rgb_value <<
        //                 " colormap_entries: " << std::dec << visual->colormap_entries <<
        //                 " red_mask: " << visual->red_mask <<
        //                 " green_mask: " << visual->green_mask <<
        //                 " blue_mask: " << visual->blue_mask <<
        //                 "}\n";
        //         }
        //         _log_os << "}\n";
        //     }
        //     _log_os << "}\n";
        // }
        conn->status = Connection::OPEN;
    }
        break;
    default:
        assert( 0 );
    }
    assert( _data - data == sz );
    return sz;
}

size_t X11ProtocolParser::_logServerMessage(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    return sz;
}

size_t X11ProtocolParser::_logClientPacket(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    switch ( conn->status ) {
    case Connection::UNESTABLISHED:
        return _logConnectionInitiation( conn, data, sz );
    case Connection::AUTHENTICATION:
        // authentication negotiation
    case Connection::CLOSED: [[fallthrough]];
    case Connection::FAILED:
        assert( 0 );
    default:
        break;
    }
    // case Connection::OPEN:
    return _logClientMessage( conn, data, sz );
}

size_t X11ProtocolParser::_logServerPacket(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    switch ( conn->status ) {
    case Connection::UNESTABLISHED:
        return _logServerResponse( conn, data, sz );
    case Connection::AUTHENTICATION:
        // authentication negotiation
    case Connection::CLOSED: [[fallthrough]];
    case Connection::FAILED:
        assert( 0 );
    default:
        break;
    }
    // case Connection::OPEN:
    return _logServerMessage( conn, data, sz );
}

void X11ProtocolParser::syncLogStream( const std::ostream& log_os ) {
    // TBD errors?
    assert( log_os.good() );
    _log_os.rdbuf( log_os.rdbuf() );
    assert( _log_os.good() );
}

size_t X11ProtocolParser::logClientPackets( Connection* conn,
                                            Settings* settings ) {
    assert( conn != nullptr );
    assert( settings != nullptr );
    uint8_t* data { conn->client_buffer.data() };
    size_t   tl_bytes_parsed {};
    for ( const size_t bytes_to_parse { conn->client_buffer.size() };
          tl_bytes_parsed < bytes_to_parse; ) {
        size_t bytes_parsed {
            _logClientPacket( conn, data, bytes_to_parse - tl_bytes_parsed ) };
        if ( settings->readwritedebug ) {
            _log_os << fmt::format(
                "{:03d}:<:parsed   {:4d} bytes\n",
                conn->id, bytes_parsed );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    return tl_bytes_parsed;
}

size_t X11ProtocolParser::logServerPackets( Connection* conn,
                                            Settings* settings ) {
    assert( conn != nullptr );
    assert( settings != nullptr );
    uint8_t* data { conn->server_buffer.data() };
    size_t   tl_bytes_parsed {};
    for ( const size_t bytes_to_parse { conn->server_buffer.size() };
          tl_bytes_parsed < bytes_to_parse; ) {
        size_t bytes_parsed {
            _logServerPacket( conn, data, bytes_to_parse - tl_bytes_parsed ) };
        if ( settings->readwritedebug ) {
            _log_os << fmt::format(
                "{:03d}:>:parsed   {:4d} bytes\n",
                conn->id, bytes_parsed );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    return tl_bytes_parsed;
}
