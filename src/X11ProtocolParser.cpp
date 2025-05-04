#include <string_view>
#include <iomanip>  // setw

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "Settings.hpp"
#include "protocol/common_types.hpp"
#include "protocol/connection_setup.hpp"


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
    std::string_view authorization_protocol_data {
        reinterpret_cast< const char* >( _data ), header->d };
    _data += _pad( header->d );
    assert( _data - data == sz );

    // TBD asserts close log file without logging anything
    _log_os << std::setw(3) << conn->id << ":<:" <<
        "connection request from " << conn->client_desc <<
        ", byte-order: '" << header->byte_order << "'";
    switch ( header->byte_order ) {
    case 'B': _log_os << " (MSB first)"; break;
    case 'l': _log_os << " (LSB first)"; break;
    default:  assert( 0 ); break;
    }
    _log_os << " protocol: " << std::dec << header->protocol_major_version <<
        "." << std::dec << header->protocol_minor_version <<
        " authorization-protocol-name[" << header->n << "]: " << authorization_protocol_name <<
        " authorization-protocol-data[" << header->d << "] (may be unprintable)" <<
        "\n";

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
        _log_os << std::setw(3) << conn->id << ":>:" <<
            "protocol v" << std::dec << header->protocol_major_version <<
            "." << std::dec << header->protocol_minor_version <<
            " server refused connection, reason[" << header->n << "]: ";
        // STRING8 of n bytes reason
        std::string_view reason {
            reinterpret_cast< const char* >( _data ), header->n };
        _data += _pad( header->n );
        _log_os << reason << '\n';
        conn->status = Connection::FAILED;
    }
        break;
    case AUTHENTICATE: {
        const ServerRequireFurtherAuthentication::Header* header {
            reinterpret_cast<
                const ServerRequireFurtherAuthentication::Header* >( _data ) };
        _data += sizeof( ServerRequireFurtherAuthentication::Header );
        const size_t reason_padded_sz { header->reason_aligned_units * _ALIGN };
        _log_os << std::setw(3) << conn->id << ":>:" <<
            "server requested further authentication, reason[" <<
            reason_padded_sz << "]: ";
        // STRING8 of n bytes reason
        std::string_view reason {
            reinterpret_cast< const char* >( _data ), reason_padded_sz };
        _data += reason_padded_sz;
        _log_os << reason << '\n';
        conn->status = Connection::AUTHENTICATION;
    }
        break;
    case SUCCESS: {
        const ServerAcceptance::Header* header {
            reinterpret_cast< const ServerAcceptance::Header* >( _data ) };
        _data += sizeof( ServerAcceptance::Header );
        // assert( header->image_byte_order < 2 );  // TBD enum
        // assert( header->bitmap_format_bit_order < 2 );  // TBD enum
        _log_os << std::setw(3) << conn->id << ":>:" <<
            "server accepted connection using protocol: " <<
            std::dec << header->protocol_major_version <<
            "." << std::dec << header->protocol_minor_version;
        _log_os << " release_number: " << header->release_number <<
            " release_id_base: " << header->release_id_base <<
            " release_id_mask: " << std::hex << "0x" << header->release_id_mask <<
            " maximum_request_length: " << std::dec << header->maximum_request_length <<
            " image_byte_order: " << std::dec << header->image_byte_order <<
            " bitmap_format_bit_order: " << std::dec << header->bitmap_format_bit_order <<
            " bitmap_format_scanline_unit: " << std::dec << header->bitmap_format_scanline_unit <<
            " bitmap_format_scanline_pad: " << std::dec << header->bitmap_format_scanline_pad <<
            " min_keycode: " << std::dec << header->min_keycode <<
            " max_keycode: " << std::dec << header->max_keycode << std::flush;
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
    // std::cerr << "X11ProtocolParser::logClientPackets(): conn->client_buffer.size(): " << conn->client_buffer.size() << std::endl;
    // std::cerr << "X11ProtocolParser::logClientPackets(): conn->status: " << conn->status << std::endl;
    for ( const size_t bytes_to_parse { conn->client_buffer.size() };
          tl_bytes_parsed < bytes_to_parse; ) {
        size_t bytes_parsed {
            _logClientPacket( conn, data, bytes_to_parse - tl_bytes_parsed ) };
        if ( settings->readwritedebug ) {
            _log_os << std::setw(3) << conn->id <<
                ":<:parsed   " << std::setw(4) << bytes_parsed << " bytes\n";
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
    // std::cerr << "X11ProtocolParser::logServerPackets(): conn->server_buffer.size(): " << conn->server_buffer.size() << std::endl;
    // std::cerr << "X11ProtocolParser::logServerPackets(): conn->status: " << conn->status << std::endl;
    for ( const size_t bytes_to_parse { conn->server_buffer.size() };
          tl_bytes_parsed < bytes_to_parse; ) {
        size_t bytes_parsed {
            _logServerPacket( conn, data, bytes_to_parse - tl_bytes_parsed ) };
        if ( settings->readwritedebug ) {
            _log_os << std::setw(3) << conn->id <<
                ":>:parsed   " << std::setw(4) << bytes_parsed << " bytes\n";
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    return tl_bytes_parsed;
}
