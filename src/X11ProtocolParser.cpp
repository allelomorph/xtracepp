#include <string_view>

#include <cctype>      // isprint

//#include <sys/stat.h>  // stat struct stat
#include <stdio.h>  // FILE feof ferror

#include <fmt/format.h>
//#include <fmt/printf.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "Settings.hpp"
#include "protocol/common_types.hpp"
#include "protocol/connection_setup.hpp"


// TBD for debugging buffer parsing
void X11ProtocolParser::_bufferHexDump( const uint8_t* data, const size_t sz ) {
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
        fmt::println(
            stderr, "{:08x}  {: <23}  {: <23}  {}",
            address_index, group1, group2, as_ascii );
    }
}

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
    fmt::print(
        _log_fs, R"({:03d}:<:client "{}" requesting connection:
  byte-order:         {}
  X protocol version: {:d}.{:d}
  auth protocol:      {}
  auth data (bytes):  {:d}
)",
        conn->id, conn->client_desc,
        ( header->byte_order == 'B' ) ? "MSB first" : "LSB first",
        header->protocol_major_version, header->protocol_minor_version,
        authorization_protocol_name,
        header->d );
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
        fmt::println(
            _log_fs,
            "{:03d}:>:server refused connection: X protocol version: {:d}.{:d}, reason: \"{}\"",
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
        fmt::println(
            _log_fs,
            "{:03d}:>:server requested further authentication: reason: \"{}\"",
            conn->id, reason_padded_sz );
        conn->status = Connection::AUTHENTICATION;
    }
        break;
    case SUCCESS: {
//        __bufferHexDump( _data, 512 );
        const ServerAcceptance::Header* header {
            reinterpret_cast< const ServerAcceptance::Header* >( _data ) };
        _data += sizeof( ServerAcceptance::Header );
        assert( header->image_byte_order < 2 );  // TBD enum
        assert( header->bitmap_format_bit_order < 2 );  // TBD enum

        fmt::print(
            _log_fs, R"({:03d}:>:server accepted connection
  X protocol version:          {:d}.{:d}
  release_number:              {}
  release_id_base:             {}
  release_id_mask:             {:#x}
  motion_buffer_size:          {:d}
  maximum_request_length:      {:d}
  image_byte_order:            {}
  bitmap_format_bit_order:     {}
  bitmap_format_scanline_unit: {:d}
  bitmap_format_scanline_pad:  {:d}
  min_keycode:                 {}
  max_keycode:                 {}
)",
            conn->id,
            header->protocol_major_version, header->protocol_minor_version,
            header->release_number,
            header->release_id_base,
            header->release_id_mask,
            header->motion_buffer_size,
            header->maximum_request_length,
            // TBD enum
            ( header->image_byte_order == 0 ) ? "LSBFirst" : "MSBFirst",
            // TBD enum
            ( header->bitmap_format_bit_order == 0 ) ? "LeastSignificant" : "MostSignificant",
            header->bitmap_format_scanline_unit,
            header->bitmap_format_scanline_pad,
            _formatCommonType( header->min_keycode ),
            _formatCommonType( header->max_keycode )
            );

        // followed by STRING8 vendor of v bytes, plus p bytes to round up to multiple of 4
        std::string_view vendor {
            reinterpret_cast< const char* >( _data ), header->v };
        _data += _pad( header->v );
        fmt::println( _log_fs, "  vendor:                      \"{}\"",
                      vendor );

        // followed by LISTofFORMAT pixmap-formats of n * sizeof(FORMAT) bytes
        const ServerAcceptance::Format* pixmap_formats {
            reinterpret_cast< const ServerAcceptance::Format* >( _data ) };
        _data += sizeof( ServerAcceptance::Format ) * header->n;
        fmt::print( _log_fs, "  pixmap_formats: [\n" );
        for ( uint8_t pm_ct { header->n }, pm_i {}; pm_i < pm_ct; ++pm_i ) {
            fmt::println(
                _log_fs,
                "    [ depth: {:3d} bits_per_pixel: {:3d} scanline_pad: {:3d} ]{}",
                pixmap_formats[pm_i].depth,
                pixmap_formats[pm_i].bits_per_pixel,
                pixmap_formats[pm_i].scanline_pad,
                ( pm_i < pm_ct - 1 ) ? "," : "" );
        }
        fmt::print( _log_fs, "  ]\n" );

        // followed by LISTofSCREEN roots of m bytes (m is always a multiple of 4)
        fmt::println( _log_fs, "  roots: [" );
        for ( uint8_t s_ct { header->r }, s_i {}; s_i < s_ct; ++s_i ) {
            const ServerAcceptance::Screen::Header* screen {
                reinterpret_cast< const ServerAcceptance::Screen::Header* >( _data ) };
            _data += sizeof( ServerAcceptance::Screen::Header );
            fmt::print(
                _log_fs, R"(    [
      root:                  {}
      default_colormap:      {}
      white_pixel:           {}
      black_pixel:           {}
      current_input_masks:   {}
      width_in_pixels:       {:d}
      height_in_pixels:      {:d}
      width_in_millimeters:  {:d}
      height_in_millimeters: {:d}
      min_installed_maps:    {:d}
      max_installed_maps:    {:d}
      root_visual:           {}
      backing_stores:        {:d}
      save_unders:           {}
      root_depth:            {:d}
)",
                _formatInteger( screen->root.data ),
                _formatInteger( screen->default_colormap.data ),
                screen->white_pixel,
                screen->black_pixel,
                _formatCommonType( screen->current_input_masks ),
                screen->width_in_pixels,
                screen->height_in_pixels,
                screen->width_in_millimeters,
                screen->height_in_millimeters,
                screen->min_installed_maps,
                screen->max_installed_maps,
                _formatInteger( screen->root_visual.data ),
                screen->backing_stores,
                _formatCommonType( screen->save_unders ),
                screen->root_depth );
            // followed by LISTofDEPTH allowed-depths of n bytes (n is always a multiple of 4) ((d * sizeof(_DepthHeader) + lists of VISUALTYPE) )
            fmt::println( _log_fs, "      allowed_depths: [" );
            for ( uint8_t d_ct { screen->d }, d_i {}; d_i < d_ct; ++d_i ) {
                const ServerAcceptance::Screen::Depth::Header* depth {
                    reinterpret_cast< const ServerAcceptance::Screen::Depth::Header* >( _data ) };
                _data += sizeof( ServerAcceptance::Screen::Depth::Header );
                fmt::print(
                    _log_fs, R"(        [
          depth: {:d}
          visuals: [
)",
                    depth->depth );
                // followed by LISTofVISUALTYPE visuals of n * sizeof(VISUALTYPE) bytes
                for ( uint16_t v_ct { depth->n }, v_i {}; v_i < v_ct; ++v_i ) {
                    const ServerAcceptance::Screen::Depth::VisualType* visual {
                        reinterpret_cast< const ServerAcceptance::Screen::Depth::VisualType* >( _data ) };
                    _data += sizeof( ServerAcceptance::Screen::Depth::VisualType );
                    fmt::print(
                        _log_fs, R"(            [
              visual_id:          {}
              class_:             {:d}
              bits_per_rgb_value: {:d}
              colormap_entries:   {:d}
              red_mask:           {:#08x}
              green_mask:         {:#08x}
              blue_mask:          {:#08x}
            ]{}
)",
                        _formatInteger( visual->visual_id.data ),
                        visual->class_,
                        visual->bits_per_rgb_value,
                        visual->colormap_entries,
                        visual->red_mask,
                        visual->green_mask,
                        visual->blue_mask,
                        ( v_i < v_ct - 1 ) ? "," : "" );
                }
                fmt::println( _log_fs, "          ]" );              // end of visuals
                fmt::println( _log_fs, "        ]{}", // end of depth
                            ( d_i < d_ct - 1 ) ? "," : "" );
            }
            fmt::println( _log_fs, "      ]" );                      // end of depths
            fmt::println( _log_fs, "    ]{}",         // end of screen
                        ( s_i < s_ct - 1 ) ? "," : "" );
        }
        fmt::println( _log_fs, "  ]" );                              // end of roots (screens)
        fflush( _log_fs );
        conn->status = Connection::OPEN;
    }
        break;
    default:
        assert( 0 );
    }
    assert( _data - data == sz );
    return sz;
}

size_t X11ProtocolParser::_logClientPacket(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    assert( conn->status != Connection::CLOSED &&
            conn->status != Connection::FAILED );
    size_t bytes_parsed {};
    switch ( conn->status ) {
    case Connection::UNESTABLISHED:
        bytes_parsed = _logConnectionInitiation( conn, data, sz );
        break;
    case Connection::AUTHENTICATION:
        // authentication negotiation
        break;
    case Connection::OPEN:
        assert( sz >= 4 );  // TBD min size for Reuqests (1B opcode, 1B unused, 2B request length (with no extensions))
        assert( *data != 0 );
//        const uint8_t first_byte { *data };
        // TBD use enum instead of magic values
        bytes_parsed = _logClientRequest( conn, data, sz, *data );
        //assert( bytes_parsed == 32 );
        break;
    default:
        break;
    }
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerPacket(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz > 0 );
    assert( conn->status != Connection::CLOSED &&
            conn->status != Connection::FAILED );
    size_t bytes_parsed {};
    switch ( conn->status ) {
    case Connection::UNESTABLISHED:
        bytes_parsed = _logServerResponse( conn, data, sz );
        break;
    case Connection::AUTHENTICATION:
        // authentication negotiation
        break;
    case Connection::OPEN: {
        assert( sz >= 32 );  // TBD size of Error and Event, min size for Reply (outside of extensions)
        // TBD when parsing replies, we will need to fetch the appropriate struct
        //   via the corresponding request, so we may need to keep a map of request
        //   opcodes by sequence number & 0000FFFF; which will let us look up
        //   an opcode for each seq num, and then a request by that opcode
        // TBD map could be a vector, as protocol dicates that on each client connx
        //   requests are 1-indexed by the server
        // server packets begin with:
        //   - errors: 0
        //   - replies: 1
        //   - events: 2-34
        const uint8_t first_byte { *data };
        // TBD use enum instead of magic values
        assert( first_byte <= 35 );
        switch ( first_byte ) {
        case 0:   // Error
            return sz;  // TBD deactivated for testing
            // log error func distinguishing by `code` byte
            bytes_parsed = _logServerError( conn, data, sz );
            // assert( bytes_parsed == 32 );
            break;
        case 1:   // Reply
            return sz;  // TBD deactivated for testing
            // log reply func distingusishing by using provided sequence number to look up request opcode,
            //   then call func based on opcode
            // TBD modification of QueryExtension replies should happen here to filter extensions
            bytes_parsed = _logServerReply( conn, data, sz );
            // assert( bytes_parsed >= 32 );
            break;
        default:  // Event (2-35)
            return sz;  // TBD deactivated for testing
            bytes_parsed = _logServerEvent( conn, data, sz, first_byte );
            // assert( bytes_parsed == 32 );
            break;
        }
    }
        break;
    default:
        break;
    }
    return bytes_parsed;
}

size_t X11ProtocolParser::_logServerError(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= 32 );  // TBD
    // using namespace protocol::errors;
    // const Error* error {
    //     reinterpret_cast< Error* >( data ) };
    return sz;
}

size_t X11ProtocolParser::_logServerReply(
    Connection* conn, uint8_t* data, const size_t sz ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= 32 ); // TBD some extension replies may be smaller, eg BigReqEnable
    return sz;
}

size_t X11ProtocolParser::_logServerEvent(
    Connection* conn, uint8_t* data, const size_t sz,
    const uint8_t code ) {
    assert( conn != nullptr );
    assert( data != nullptr );
    assert( sz >= 32 ); // TBD
    return sz;
}

void X11ProtocolParser::setLogFileStream( FILE* log_fs ) {
    assert( log_fs != nullptr );
    assert( !feof( log_fs ) && !ferror( log_fs ) );
    _log_fs = log_fs;
}

// TBD pass in readwritedebug or keep as private parser var like verbosity?
// client packets begin with:
//   - first contact: 'B' or 'l' (66 or 108)
//   - requests: opcode of 1-119, 127
//   - extension enable: ??? (logs of xtrace indicate packets sent after
//       QueryExtension gets positive response from server, to then enable
//       that extension)
size_t X11ProtocolParser::logClientPackets( Connection* conn,
                                            const Settings& settings ) {
    assert( conn != nullptr );
//    assert( settings != nullptr );
    uint8_t* data { conn->client_buffer.data() };
    size_t   tl_bytes_parsed {};
    for ( const size_t bytes_to_parse { conn->client_buffer.size() };
          tl_bytes_parsed < bytes_to_parse; ) {
        size_t bytes_parsed {
            _logClientPacket( conn, data, bytes_to_parse - tl_bytes_parsed ) };
        if ( settings.readwritedebug ) {
            fmt::println( _log_fs, "{:03d}:<:parsed   {:4d} bytes",
                          conn->id, bytes_parsed );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    return tl_bytes_parsed;
}

size_t X11ProtocolParser::logServerPackets( Connection* conn,
                                            const Settings& settings ) {
    assert( conn != nullptr );
//    assert( settings != nullptr );
    uint8_t* data { conn->server_buffer.data() };
    size_t   tl_bytes_parsed {};
    for ( const size_t bytes_to_parse { conn->server_buffer.size() };
          tl_bytes_parsed < bytes_to_parse; ) {
        size_t bytes_parsed {
            _logServerPacket( conn, data, bytes_to_parse - tl_bytes_parsed ) };
        if ( settings.readwritedebug ) {
            fmt::println( _log_fs, "{:03d}:>:parsed   {:4d} bytes",
                          conn->id, bytes_parsed );
        }
        data += bytes_parsed;
        tl_bytes_parsed += bytes_parsed;
    }
    return tl_bytes_parsed;
}
