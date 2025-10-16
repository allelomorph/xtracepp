#ifndef PROTOCOL_CONNECTION_SETUP_HPP
#define PROTOCOL_CONNECTION_SETUP_HPP

#include "protocol/common_types.hpp"


namespace protocol {

namespace connection_setup {

struct ConnInitiation {
    struct [[gnu::packed]] Encoding {
        CARD8    byte_order;              // MSBFIRST or LSBFIRST
    private:
        uint8_t  _unused1;
    public:
        CARD16   protocol_major_version;  // protocol-major-version
        CARD16   protocol_minor_version;  // protocol-minor-version
        uint16_t name_len;                // authorization-protocol-name length in bytes
        uint16_t data_len;                // authorization-protocol-data length in bytes
    private:
        uint16_t _unused2;
    };
    // followed by pad(name_len) STRING8 authorization-protocol-name
    // followed by pad(name_len) STRING8 authorization-protocol-data

    enum {
        MSBFIRST = 'B',
        LSBFIRST = 'l'
    };
    inline static const
    std::vector< std::string_view >& byte_order_names {
        protocol::enum_names::image_byte_order };
};

struct ConnResponse {
    struct [[gnu::packed]] Header {
        uint8_t  success;
    private:
        uint8_t  _unused1;
        uint16_t _unused2[2];
    public:
        uint16_t following_aligned_units;  // message length after header in 4B units
    };

    enum success {
        FAILED,
        SUCCESS,
        AUTHENTICATE
    };
    inline static const
    std::vector< std::string_view >& success_names {
        protocol::enum_names::server_response_success };

    virtual ~ConnResponse() = 0;
};

struct ConnRefusal : public ConnResponse {
    struct [[gnu::packed]] Header {
        uint8_t  success;                  // 0 ServerResponse::FAILED
        uint8_t  reason_len;               // reason length in bytes
        CARD16   protocol_major_version;   // protocol-major-version
        CARD16   protocol_minor_version;   // protocol-minor-version
        uint16_t following_aligned_units;  // message length after header in 4B units
    };
    // followed by STRING8 reason _pad(Header.reason_len * 4)
};

struct ConnRequireFurtherAuthentication : public ConnResponse {
    // Header.success == 2 connection_setup::AUTHENTICATE
    // followed by STRING8 reason (Header.following_aligned_units * 4)B
};

struct ConnAcceptance : public ConnResponse {
    struct [[gnu::packed]] Header {
        uint8_t  success;                  // 1 ServerResponse::SUCCESS
    private:
        uint8_t  _unused;
    public:
        CARD16   protocol_major_version;   // protocol-major-version
        CARD16   protocol_minor_version;   // protocol-minor-version
        uint16_t following_aligned_units;  // message length after header in 4B units
    };
    struct [[gnu::packed]] Encoding {
        Header   header;
        CARD32   release_number;               // release-number
        CARD32   resource_id_base;             // resource-id-base
        CARD32   resource_id_mask;             // resource-id-mask
        CARD32   motion_buffer_size;           // motion-buffer-size
        uint16_t vendor_len;                   // vendor length in bytes
        CARD16   maximum_request_length;       // maximum-request-length
        CARD8    roots_ct;                     // roots length in SCREENs
        uint8_t  pixmap_formats_ct;            // pixmap-formats length in FORMATs
        uint8_t  image_byte_order;             // image-byte-order
        uint8_t  bitmap_format_bit_order;      // bitmap-format-bit-order
        CARD8    bitmap_format_scanline_unit;  // bitmap-format-scanline-unit
        CARD8    bitmap_format_scanline_pad;   // bitmap-format-scanline-pad
        KEYCODE  min_keycode;                  // min-keycode
        KEYCODE  max_keycode;                  // max-keycode
    private:
        uint8_t  _unused2[4];
    };
    // followed by STRING8 vendor pad(vendor_len)B
    // followed by LISTofFORMAT pixmap-formats (pixmap_formats_ct * sizeof(FORMAT))B
    // followed by LISTofSCREEN roots (always multiple of 4)B

    // TBD enum { LSBFIRST, MSBFIRST } for image-byte-order
    // TBD enum { LEASTSIGNIFICANT, MOSTSIGNIFICANT } for bitmap-format-bit-order

    inline static const std::vector< std::string_view >&
    byte_order_names { protocol::enum_names::image_byte_order };
    inline static const std::vector< std::string_view >&
    bit_order_names { protocol::enum_names::bitmap_format_bit_order };

    struct [[gnu::packed]] FORMAT {
        CARD8   depth;
        CARD8   bits_per_pixel;  // bits-per-pixel
        CARD8   scanline_pad;    // scanline-pad
    private:
        uint8_t _unused[5];
    };

    struct SCREEN {
        struct [[gnu::packed]] Encoding {
            WINDOW     root;
            COLORMAP   default_colormap;       // default-colormap
            CARD32     white_pixel;            // white-pixel
            CARD32     black_pixel;            // black-pixel
            SETofEVENT current_input_masks;    // current-input-masks
            CARD16     width_in_pixels;        // width-in-pixels
            CARD16     height_in_pixels;       // height-in-pixels
            CARD16     width_in_millimeters;   // width-in-millimeters
            CARD16     height_in_millimeters;  // height-in-millimeters
            CARD16     min_installed_maps;     // min-installed-maps
            CARD16     max_installed_maps;     // max-installed-maps
            VISUALID   root_visual;            // root-visual
            uint8_t    backing_stores;         // backing-stores
            BOOL       save_unders;            // save-unders
            CARD8      root_depth;             // root-depth
            CARD8      allowed_depths_ct;      // length of allowed-depths in DEPTHs
        };
        // followed by (always multiple of 4) LISTofDEPTH allowed-depths

        inline static const
        std::vector< std::string_view >& backing_stores_names {
            protocol::enum_names::screen_backing_stores };

        struct DEPTH {
            struct [[gnu::packed]] Encoding {
                CARD8    depth;
            private:
                uint8_t  _unused1;
            public:
                uint16_t visuals_ct;  // length of visuals in VISUALTYPEs
            private:
                uint8_t  _unused2[4];
            };
            // followed by LISTofVISUALTYPE visuals (visuals_ct * sizeof(VISUALTYPE))B

            struct [[gnu::packed]] VISUALTYPE {
                VISUALID visual_id;           // visual-id
                uint8_t  class_;              // class
                CARD8    bits_per_rgb_value;  // bits-per-rgb-value
                CARD16   colormap_entries;    // colormap-entries
                CARD32   red_mask;            // red-mask
                CARD32   green_mask;          // green-mask
                CARD32   blue_mask;           // blue-mask
            private:
                uint8_t  _unused[4];
            public:
                inline static const
                std::vector< std::string_view >& class_names {
                    protocol::enum_names::visualtype_class };
            };
        };
    };
};

}  // namespace connection_setup

}  // namespace protocol


#endif  // PROTOCOL_CONNECTION_SETUP_HPP
