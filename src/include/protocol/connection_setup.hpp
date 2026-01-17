#ifndef PROTOCOL_CONNECTION_SETUP_HPP
#define PROTOCOL_CONNECTION_SETUP_HPP

/**
 * @file connection_setup.hpp
 * @see [X protocol encoding](https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Connection_Setup)
 */

#include "protocol/Message.hpp"
#include "protocol/common_types.hpp"


namespace protocol {

namespace connection_setup {

/**
 * @brief Represents client packet sent to [initiate] connection to X server.
 * [initiate]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Connection_Setup
 */
struct Initiation : public Message {
    /**
     * @brief Fixed encoding prefix.
     * @note Followed by suffixes:
     *   - `STRING8 authorization-protocol-name` of
     *   [pad](#X11ProtocolParser::_Alignment::pad)([name_len](#name_len))B
     *   - `STRING8 authorization-protocol-data` of
     *   [pad](#X11ProtocolParser::_Alignment::pad)([data_len](#data_len))B
     */
    struct [[gnu::packed]] Header {
        /** @brief Protocol name byte-order; either `MSBFIRST` or `LSBFIRST`. */
        CARD8    byte_order;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused1;
    public:
        /** @brief Protocol name: protocol-major-version. */
        CARD16   protocol_major_version;
        /** @brief Protocol name: protocol-minor-version. */
        CARD16   protocol_minor_version;
        /** @brief `authorization-protocol-name` length in bytes. */
        uint16_t name_len;
        /** @brief `authorization-protocol-data` length in bytes. */
        uint16_t data_len;
    private:
        /** @brief Ignored bytes. */
        uint16_t _unused2;
    };
    /**
     * @brief Constants used by [byte-order](#Header::byte_order).
     */
    enum { MSBFIRST = 'B', LSBFIRST = 'l' };
    /** @brief [byte-order](#Header::byte_order) enum names. */
    inline static const
    std::vector< std::string_view >& byte_order_names {
        protocol::enum_names::byte_order };
};

/**
 * @brief Interface class providing generic header for [Refusal](#Refusal),
 *   [RequireFurtherAuthentication](#RequireFurtherAuthentication),
 *   and [Acceptance](#Acceptance).
 */
struct InitResponse : public Message {
    /**
     * @brief Generic fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief First byte of message, used to differentiate response type;
         *    could be any of #FAILED, #SUCCESS, #AUTHENTICATE. */
        uint8_t  success;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused1;
        /** @brief Ignored bytes. */
        uint16_t _unused2[2];
    public:
        /** @brief Length of suffix after header in 4B units. */
        uint16_t following_aligned_units;
    };
    /**
     * @brief All valid values of [success](#Header::success).
     */
    enum Success { FAILED, SUCCESS, AUTHENTICATE };
    /** @brief Enum names of [Header::success](#Header::success) values. */
    inline static const
    std::vector< std::string_view > success_names {
        "Failed",
        "Success",
        "Authenticate"
    };
    virtual ~InitResponse() = 0;
};

/**
 * @brief Represents X server [refusal] of an [Initiation](#Initiation).
 * [refusal]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Connection_Setup
 */
struct Refusal : public InitResponse {
    /**
     * @brief Fixed encoding prefix.
     * @note Followed by suffix:
     *   - `STRING8 reason` of
     *   [pad](#X11ProtocolParser::_Alignment::pad)([reason_len](#reason_len))B
     */
    struct [[gnu::packed]] Header {
        /** @brief Should always equal [FAILED](#InitResponse::Success). */
        uint8_t  success;
        /** @brief `reason` length in bytes. */
        uint8_t  reason_len;
        /** @brief Protocol name: protocol-major-version. */
        CARD16   protocol_major_version;
        /** @brief Protocol name: protocol-minor-version. */
        CARD16   protocol_minor_version;
        /** @brief Length of suffix after header in 4B units. */
        uint16_t following_aligned_units;
    };
};

/**
 * @brief Represents X server request for further [authentication] negotiation
 *   after an [Initiation](#Initiation).
 * [authentication]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Connection_Setup
 * @note Uses [InitResponse::Header](#InitResponse::Header):
 *   - [success](#InitResponse::Header::success) should always equal
 *     [AUTHENTICATE](#InitResponse::Success)
 *   - followed by suffix:
 *     - `STRING8 reason` of [pad](#X11ProtocolParser::_Alignment::pad)(
 *     [following_aligned_units](#InitResponse::Header::following_aligned_units) * 4)B
 */
struct RequireFurtherAuthentication : public InitResponse {
};

/**
 * @brief Represents X server [acceptance] of an [Initiation](#Initiation).
 * [acceptance]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Connection_Setup
 */
struct Acceptance : public InitResponse {
    /**
     * @brief Fixed encoding prefix.
     */
    struct [[gnu::packed]] Header {
        /** @brief Should always equal [SUCCESS](#InitResponse::Success). */
        uint8_t  success;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused;
    public:
        /** @brief Protocol name: protocol-major-version. */
        CARD16   protocol_major_version;
        /** @brief Protocol name: protocol-minor-version. */
        CARD16   protocol_minor_version;
        /** @brief Total length of suffixes after header in 4B units. */
        uint16_t following_aligned_units;
    };
    /**
     * @brief Fixed encoding between [Header](#Header) and suffixes.
     * @note Followed by suffixes:
     *   - `STRING8 vendor` of
     *   [pad](#X11ProtocolParser::_Alignment::pad)([vendor_len](#vendor_len))B
     *   - `LISTofFORMAT pixmap-formats` of
     *   [pad](#X11ProtocolParser::_Alignment::pad)(
     *     [pixmap_formats_ct](#pixmap_formats_ct) * sizeof([FORMAT](#FORMAT)))B
     *   - `LISTofSCREEN roots` of (always multiple of 4)B
     */
    struct [[gnu::packed]] Encoding {
        /** @brief Protocol name: release-number. */
        CARD32   release_number;
        /** @brief Protocol name: resource-id-base. */
        CARD32   resource_id_base;
        /** @brief Protocol name: resource-id-mask. */
        CARD32   resource_id_mask;
        /** @brief Protocol name: motion-buffer-size. */
        CARD32   motion_buffer_size;
        /** @brief `vendor` length in bytes. */
        uint16_t vendor_len;
        /** @brief Protocol name: maximum-request-length. */
        CARD16   maximum_request_length;
        /** @brief `roots` length in [SCREEN](#SCREEN)s. */
        CARD8    roots_ct;
        /** @brief `pixmap-formats` length in [FORMAT](#FORMAT)s. */
        uint8_t  pixmap_formats_ct;
        /** @brief Protocol name: image-byte-order. */
        uint8_t  image_byte_order;
        /** @brief Protocol name: bitmap-format-bit-order. */
        uint8_t  bitmap_format_bit_order;
        /** @brief Protocol name: bitmap-format-scanline-unit. */
        CARD8    bitmap_format_scanline_unit;
        /** @brief Protocol name: bitmap-format-scanline-pad. */
        CARD8    bitmap_format_scanline_pad;
        /** @brief Protocol name: min-keycode. */
        KEYCODE  min_keycode;
        /** @brief Protocol name: max-keycode. */
        KEYCODE  max_keycode;
    private:
        /** @brief Ignored bytes. */
        uint8_t  _unused2[4];
    };
    /** @brief [image-byte-order](#Encoding::image_byte_order) enum names. */
    inline static const
    std::vector< std::string_view >& image_byte_order_names {
        protocol::enum_names::byte_order };
    /** @brief [bitmap-format-bit-order](#Encoding::bitmap_format_bit_order)
     *    enum names. */
    inline static const
    std::vector< std::string_view > bitmap_format_bit_order_names {
        "LeastSignificant",  // 0
        "MostSignificant"    // 1
    };
    /**
     * @brief List members of [Encoding](#Encoding) suffix `pixmap-formats`.
     */
    struct [[gnu::packed]] FORMAT : public protocol::impl::Struct {
        /** @brief Protocol name: depth. */
        CARD8   depth;
        /** @brief Protocol name: bits-per-pixel. */
        CARD8   bits_per_pixel;
        /** @brief Protocol name: scanline-pad. */
        CARD8   scanline_pad;
    private:
        /** @brief Ignored bytes. */
        uint8_t _unused[5];
    };
    /**
     * @brief List members of [Encoding](#Encoding) suffix `roots`.
     */
    struct SCREEN : public protocol::impl::StructWithSuffixes {
        /**
         * @brief Fixed encoding prefix.
         * @note Followed by suffix:
         *   - `LISTofDEPTH allowed-depths` of (always multiple of 4)B
         */
        struct [[gnu::packed]] Header {
            /** @brief Protocol name: root. */
            WINDOW     root;
            /** @brief Protocol name: default-colormap. */
            COLORMAP   default_colormap;
            /** @brief Protocol name: white-pixel. */
            CARD32     white_pixel;
            /** @brief Protocol name: black-pixel. */
            CARD32     black_pixel;
            /** @brief Protocol name: current-input-masks. */
            SETofEVENT current_input_masks;
            /** @brief Protocol name: width-in-pixels. */
            CARD16     width_in_pixels;
            /** @brief Protocol name: height-in-pixels. */
            CARD16     height_in_pixels;
            /** @brief Protocol name: width-in-millimeters. */
            CARD16     width_in_millimeters;
            /** @brief Protocol name: height-in-millimeters. */
            CARD16     height_in_millimeters;
            /** @brief Protocol name: min-installed-maps. */
            CARD16     min_installed_maps;
            /** @brief Protocol name: max-installed-maps. */
            CARD16     max_installed_maps;
            /** @brief Protocol name: root-visual. */
            VISUALID   root_visual;
            /** @brief Protocol name: backing-stores. */
            uint8_t    backing_stores;
            /** @brief Protocol name: save-unders. */
            BOOL       save_unders;
            /** @brief Protocol name: root-depth. */
            CARD8      root_depth;
            /** @brief length of `allowed-depths` length in [DEPTH](#DEPTH)s. */
            CARD8      allowed_depths_ct;
        };
        /** @brief [backing-stores](#Header::backing_stores) enum names. */
        inline static const
        std::vector< std::string_view > backing_stores_names {
            "Never",       // 0
            "WhenMapped",  // 1
            "Always"       // 2
        };
        /**
         * @brief List members of [Header](#SCREEN::Header) suffix `allowed-depths`.
         */
        struct DEPTH : public protocol::impl::StructWithSuffixes {
            /**
             * @brief Fixed encoding prefix.
             * @note Followed by suffix:
             *   - `LISTofVISUALTYPE visuals` of
             *     ([visuals_ct](#visuals_ct) * sizeof([VISUALTYPE](#VISUALTYPE)))B
             */
            struct [[gnu::packed]] Header {
                /** @brief Protocol name: depth. */
                CARD8    depth;
            private:
                /** @brief Ignored bytes. */
                uint8_t  _unused1;
            public:
                /** @brief `visuals` length in [VISUALTYPE](#VISUALTYPE)s. */
                uint16_t visuals_ct;
            private:
                /** @brief Ignored bytes. */
                uint8_t  _unused2[4];
            };
            /**
             * @brief List members of [Header](#DEPTH::Header) suffix `visuals`.
             */
            struct [[gnu::packed]] VISUALTYPE : public protocol::impl::Struct {
                /** @brief Protocol name: visual-id. */
                VISUALID visual_id;
                /** @brief Protocol name: class. */
                uint8_t  class_;
                /** @brief Protocol name: bits-per-rgb-value. */
                CARD8    bits_per_rgb_value;
                /** @brief Protocol name: colormap-entries. */
                CARD16   colormap_entries;
                /** @brief Protocol name: red-mask. */
                CARD32   red_mask;
                /** @brief Protocol name: green-mask. */
                CARD32   green_mask;
                /** @brief Protocol name: blue-mask. */
                CARD32   blue_mask;
            private:
                /** @brief Ignored bytes. */
                uint8_t  _unused[4];
            public:
                /** @brief [class](#class_) enum names. */
                inline static const
                std::vector< std::string_view > class_names {
                    "StaticGray",   // 0
                    "GrayScale",    // 1
                    "StaticColor",  // 2
                    "PseudoColor",  // 3
                    "TrueColor",    // 4
                    "DirectColor"   // 5
                };
            };
        };
    };
};

}  // namespace connection_setup

}  // namespace protocol


#endif  // PROTOCOL_CONNECTION_SETUP_HPP
