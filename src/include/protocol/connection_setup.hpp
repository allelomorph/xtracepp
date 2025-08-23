#ifndef CONNECTION_SETUP_HPP
#define CONNECTION_SETUP_HPP

#include "protocol/common_types.hpp"


namespace protocol {
// where E is some expression, and pad(E) is the number of bytes needed to round E up to a multiple of four.
//   pad(E) = (4 - (E mod 4)) mod 4

// Connection Setup
namespace connection_setup {

// For TCP connections, displays on a given host are numbered starting from 0,
//   and the server for display N listens and accepts connections on port
//   6000 + N.

struct ClientInitiation {
    // TBD rename Encoding to match Request/Reply/Event
    struct [[gnu::packed]] Header {
        CARD8    byte_order;  //  byte-order #x42 'B' MSB first (bigend) #x6C 'l' LSB first (littleend)
    private:
        uint8_t  _unused1;
    public:
        CARD16   protocol_major_version;  // protocol-major-version
        CARD16   protocol_minor_version;  // protocol-minor-version
        uint16_t n;                       // length of authorization-protocol-name
        uint16_t d;                       // length of authorization-protocol-data
    private:
        uint16_t _unused2;
    };
    // header is followed by STRING8 of n bytes, authorization_protocol_name (authorization-protocol-name)
    // followed by p bytes to pad n to multiple of 4
    // then STRING8 of d bytes, authorization_protocol_data (authorization-protocol-data)
    // followed by q bytes to pad d to multiple of 4
};

// Except where explicitly noted in the protocol, all 16-bit and 32-bit
//   quantities sent by the client must be transmitted with the specified byte
//   order, and all 16-bit and 32-bit quantities returned by the server will be
//   transmitted with this byte order.

// Information received by the client if the connection is refused:
// TBD likely will need func to check first byte to determine accept/fail/auth

enum success { FAILED, SUCCESS, AUTHENTICATE };  // Failed, Success, Authenticate

struct ServerRefusal {
    // TBD rename Encoding to match Request/Reply/Event
    struct [[gnu::packed]] Header {
        uint8_t  success;  // 0 protocol::connection_setup::FAILED
        uint8_t  n;  // length of reason in bytes
        CARD16   protocol_major_version;  // protocol-major-version
        CARD16   protocol_minor_version;  // protocol-minor-version
        uint16_t reason_aligned_units; // (n+p)/4 length in 4-byte units of "additional data" // unnamed in protocol
    };
    // followed by STRING8 reason of n bytes, plus p bytes to round up to multiple of 4
};

// Information received by the client if further authentication is required:
struct ServerRequireFurtherAuthentication {
    // TBD rename Encoding to match Request/Reply/Event
    struct [[gnu::packed]] Header {
        uint8_t  success;  // 1 protocol::connection_setup::AUTHENTICATE
    private:
        uint8_t  _unused[5];
    public:
        uint16_t reason_aligned_units; // (n+p)/4 length in 4-byte units of "additional data" // unnamed in protocol
    };
    // followed by STRING8 reason of up to reason_aligned_units * 4 bytes
};

// Information received by the client if the connection is accepted:
struct ServerAcceptance {
    // TBD rename Encoding to match Request/Reply/Event
    struct [[gnu::packed]] Header {
        uint8_t  success;  // 2 protocol::connection_setup::SUCCESS
    private:
        uint8_t  _unused1;  // not to be confused with leading success byte
    public:
        CARD16   protocol_major_version;  // protocol-major-version
        CARD16   protocol_minor_version;  // protocol-minor-version
        uint16_t ad;  // 8+2n+(v+p+m)/4 length in 4-byte units of "additional data" // unnamed in protocol
        CARD32   release_number;   // release-number
        CARD32   resource_id_base;  // resource-id-base
        CARD32   resource_id_mask;  // resource-id-mask
        CARD32   motion_buffer_size;  // motion-buffer-size
        uint16_t v;   // length of vendor
        CARD16   maximum_request_length;  // maximum-request-length
        CARD8    r;  // number of SCREENs in roots  !!! unnamed in protocol
        uint8_t  n;  // number for FORMATs in pixmap-formats
        uint8_t  image_byte_order;  // image-byte-order  // 0 LSBFirst 1 MSBFirst
        uint8_t  bitmap_format_bit_order;  // bitmap-format-bit-order // 0 LeastSignificant 1 MostSignificant
        CARD8    bitmap_format_scanline_unit;  // bitmap-format-scanline-unit
        CARD8    bitmap_format_scanline_pad;  // bitmap-format-scanline-pad
        KEYCODE  min_keycode;  // min-keycode
        KEYCODE  max_keycode;  // max-keycode
    private:
        uint8_t  _unused2[4];
    };
    // followed by STRING8 vendor of v bytes, plus p bytes to round up to multiple of 4
    // followed by LISTofFORMAT pixmap-formats of n*sizeof(Format) bytes
    // followed by LISTofSCREEN roots of m bytes (m is always a multiple of 4)

    inline static const std::vector< std::string_view >&
    byte_order_names { protocol::enum_names::image_byte_order };
    inline static const std::vector< std::string_view >&
    bit_order_names { protocol::enum_names::bitmap_format_bit_order };

    struct [[gnu::packed]] FORMAT {  // FORMAT
        CARD8   depth;
        CARD8   bits_per_pixel;  // bits-per-pixel
        CARD8   scanline_pad;  // scanline-pad
    private:
        uint8_t _unused[5];
    };

    struct SCREEN {  // SCREEN
        // TBD rename Encoding to match Request/Reply/Event
        struct [[gnu::packed]] Header {
            WINDOW     root;
            COLORMAP   default_colormap;  // default-colormap
            CARD32     white_pixel;  // white-pixel
            CARD32     black_pixel;  // black-pixel
            SETofEVENT current_input_masks;  // current-input-masks
            CARD16     width_in_pixels;  // width-in-pixels
            CARD16     height_in_pixels;  // height-in-pixels
            CARD16     width_in_millimeters;  // width-in-millimeters
            CARD16     height_in_millimeters;  // height-in-millimeters
            CARD16     min_installed_maps;  // min-installed-maps
            CARD16     max_installed_maps;  // max-installed-maps
            VISUALID   root_visual;  // root-visual
            uint8_t    backing_stores;  // backing-stores  // 0 Never 1 WhenMapped 2 Always
            BOOL       save_unders;  // save-unders
            CARD8      root_depth;  // root-depth
            CARD8      d;  // number of DEPTHs in allowed-depths // unnamed in protocol
        };
        // followed by LISTofDEPTH allowed-depths of n bytes (n is always a multiple of 4) ((d * sizeof(_DepthHeader) + lists of VISUALTYPE) )

        inline static const
        std::vector< std::string_view >& backing_stores_names {
            protocol::enum_names::screen_backing_stores };

        struct DEPTH {  // DEPTH
            // TBD rename Encoding to match Request/Reply/Event
            struct [[gnu::packed]] Header {
                CARD8    depth;
            private:
                uint8_t  _unused1;
            public:
                uint16_t n;  // number of VISUALTYPES in visuals
            private:
                uint8_t  _unused2[4];
            };
            // followed by LISTofVISUALTYPE visuals of n*sizeof(VisualType) bytes

            struct [[gnu::packed]] VISUALTYPE {  // VISUALTYPE
                struct [[gnu::packed]] Encoding {
                    VISUALID visual_id;  // visual-id
                    uint8_t  class_;  // class // 0 StaticGray 1 GrayScale 2 StaticColor 3 PseudoColor 4 TrueColor 5 DirectColor
                    CARD8    bits_per_rgb_value;  // bits-per-rgb-value
                    CARD16   colormap_entries;  // colormap-entries
                    CARD32   red_mask;  // red-mask
                    CARD32   green_mask;  // green-mask
                    CARD32   blue_mask;  // blue-mask
                private:
                    uint8_t  _unused[4];
                };

                inline static const
                std::vector< std::string_view >& class_names {
                    protocol::enum_names::visualtype_class };
            };
        };
    };
};

}  // namespace connection_setup

}  // namespace protocol


#endif  // CONNECTION_SETUP_HPP
