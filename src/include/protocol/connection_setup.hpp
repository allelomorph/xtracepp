#ifndef CONNECTION_SETUP_HPP
#define CONNECTION_SETUP_HPP

#include "protocol/common_types.hpp"


namespace protocol {
// where E is some expression, and pad(E) is the number of bytes needed to round E up to a multiple of four.
//   pad(E) = (4 - (E mod 4)) mod 4

// Connection Setup

// For TCP connections, displays on a given host are numbered starting from 0,
//   and the server for display N listens and accepts connections on port
//   6000 + N.

// Information sent by the client at connection setup:
struct ConnectionInitiation {
    struct Header {
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
    } header;
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
// class ServerConnectionReject {
// private:
//     // preceeded by uint8_t enum 0 Failed
//     struct _header_encoding {
//         uint8_t  n;  // length of reason in bytes
//         CARD16   protocol_major_version;  // protocol-major-version
//         CARD16   protocol_minor_version;  // protocol-minor-version
//         uint16_t reason_sz_padded; // (n+p)/4 length in 4-byte units of "additional data"
//     };
//     // followed by STRING8 reason of n bytes, plus p bytes to round up to multiple of 4
// public:
// };

// // Information received by the client if further authentication is required:
// class ServerConnectionRejectAuthentication {
// private:
//     // preceeded by uint8_t enum 2 Authenticate
//     struct _header_encoding {
//         uint8_t unused[5];
//         uint16_t reason_sz_padded; // (n+p)/4 length in 4-byte units of "additional data"
//     };
//     // followed by STRING8 reason of n bytes, plus p bytes to round up to multiple of 4
// public:
// };

// // Information received by the client if the connection is accepted:
// class ServerConnectionAccept {
// private:
//     // preceeded by uint8_t enum 1 Success
//     struct _Header {
//         uint8_t  unused1;
//         CARD16   protocol_major_version;  // protocol-major-version
//         CARD16   protocol_minor_version;  // protocol-minor-version
//         uint16_t ad;  // 8+2n+(v+p+m)/4 length in 4-byte units of "additional data" // unnamed in protocol
//         CARD32   release_number;   // release-number
//         CARD32   release_id_base;  // release-id-base
//         CARD32   release_id_mask;  // release-id-mask
//         uint16_t v;   // length of vendor
//         CARD16   maximum_request_length;  // maximum-request-length
//         CARD8    r;  // number of SCREENs in roots  !!! unnamed in protocol
//         uint8_t  n;  // number for FORMATs in pixmap-formats
//         uint8_t  image_byte_order;  // image-byte-order  // 0 LSBFirst 1 MSBFirst
//         uint8_t  bitmap_format_bit_order;  // bitmap-format-bit-order // 0 LeastSignificant 1 MostSignificant
//         CARD8    bitmap_format_scanline_unit;  // bitmap-format-scanline-unit
//         CARD8    bitmap_format_scanline_pad;  // bitmap-format-scanline-pad
//         KEYCODE  min_keycode;  // min-keycode
//         KEYCODE  max_keycode;  // max-keycode
//         uint8_t  unused2[4];
//     };
//     // followed by STRING8 vendor of v bytes, plus p bytes to round up to multiple of 4
//     // followed by LISTofFORMAT pixmap-formats of n*sizeof(_Format) bytes
//     struct _Format {  // FORMAT
//         CARD8   depth;
//         CARD8   bits_per_pixel;  // bits-per-pixel
//         CARD8   scanline_pad;  // scanline-pad
//         uint8_t unused[5];
//     };
//     // followed by LISTofSCREEN roots of m bytes (m is always a multiple of 4)
//     struct _ScreenHeader {  // SCREEN
//         WINDOW     root;
//         COLORMAP   default_colormap;  // default-colormap
//         CARD32     white_pixel;  // white-pixel
//         CARD32     black_pixel;  // black-pixel
//         SETofEVENT current_input_masks;  // current-input-masks
//         CARD16     width_in_pixels;  // width-in-pixels
//         CARD16     height_in_pixels;  // height-in-pixels
//         CARD16     width_in_millimeters;  // width-in-millimeters
//         CARD16     height_in_millimeters;  // height-in-millimeters
//         CARD16     min_installed_maps;  // min-installed-maps
//         CARD16     max_installed_maps;  // max-installed-maps
//         VISUALID   root_visual;  // root-visual
//         uint8_t    backing_stores;  // backing-stores  // 0 Never 1 WhenMapped 2 Always
//         BOOL       save_unders;  // save-unders
//         CARD8      root_depth;  // root-depth
//         CARD8      d;  // number of DEPTHs in allowed-depths // unnamed in protocol
//     };
//     // followed by LISTofDEPTH allowed-depths of n bytes (n is always a multiple of 4) ((d * sizeof(_DepthHeader) + lists of VISUALTYPE) )
//     struct _DepthHeader {  // DEPTH
//         CARD8    depth;
//         uint8_t  unused1;
//         uint16_t n;  // number of VISUALTYPES in visuals
//         uint8_t  unused2[4];
//     };
//     // followed by LISTofVISUALTYPE visuals of n*sizeof(_Format) bytes
//     struct _VisualType {  // VISUALTYPE
//         VISUALID visual_id;  // visual-id
//         uint8_t  class_;  // class // 0 StaticGray 1 GrayScale 2 StaticColor 3 PseudoColor 4 TrueColor 5 DirectColor
//         CARD8    bits_per_rgb_value;  // bits-per-rgb-value
//         CARD16   colormap_entries;  // colormap-entries
//         CARD32   red_mask;  // red-mask
//         CARD32   green_mask;  // green-mask
//         CARD32   blue_mask;  // blue-mask
//         uint8_t  unused[4];
//     };
// };

}  // namespace protocol

/*
Connection Setup
For TCP connections, displays on a given host are numbered starting from 0, and the server for display N listens and accepts connections on port 6000 + N. For DECnet connections, displays on a given host are numbered starting from 0, and the server for display N listens and accepts connections on the object name obtained by concatenating "X$X" with the decimal representation of N, for example, X$X0 and X$X1.

Information sent by the client at connection setup:

     1                       byte-order
          #x42     MSB first
          #x6C     LSB first
     1                       unused
     2     CARD16            protocol-major-version
     2     CARD16            protocol-minor-version
     2     n                 length of authorization-protocol-name
     2     d                 length of authorization-protocol-data
     2                       unused
     n     STRING8           authorization-protocol-name
     p                       unused, p=pad(n)
     d     STRING8           authorization-protocol-data
     q                       unused, q=pad(d)
Except where explicitly noted in the protocol, all 16-bit and 32-bit quantities sent by the client must be transmitted with the specified byte order, and all 16-bit and 32-bit quantities returned by the server will be transmitted with this byte order.

Information received by the client if the connection is refused:

     1     0                 Failed
     1     n                 length of reason in bytes
     2     CARD16            protocol-major-version
     2     CARD16            protocol-minor-version
     2     (n+p)/4           length in 4-byte units of "additional data"
     n     STRING8           reason
     p                       unused, p=pad(n)
Information received by the client if further authentication is required:

     1     2                 Authenticate
     5                       unused
     2     (n+p)/4           length in 4-byte units of "additional data"
     n     STRING8           reason
     p                       unused, p=pad(n)
Information received by the client if the connection is accepted:

     1     1                               Success
     1                                     unused
     2     CARD16                          protocol-major-version
     2     CARD16                          protocol-minor-version
     2     8+2n+(v+p+m)/4                  length in 4-byte units of
                                           "additional data"
     4     CARD32                          release-number
     4     CARD32                          resource-id-base
     4     CARD32                          resource-id-mask
     4     CARD32                          motion-buffer-size
     2     v                               length of vendor
     2     CARD16                          maximum-request-length
     1     CARD8                           number of SCREENs in roots
     1     n                               number for FORMATs in
                                           pixmap-formats
     1                                     image-byte-order
          0     LSBFirst
          1     MSBFirst
     1                                     bitmap-format-bit-order
          0     LeastSignificant
          1     MostSignificant
     1     CARD8                           bitmap-format-scanline-unit
     1     CARD8                           bitmap-format-scanline-pad
     1     KEYCODE                         min-keycode
     1     KEYCODE                         max-keycode
     4                                     unused
     v     STRING8                         vendor
     p                                     unused, p=pad(v)
     8n     LISTofFORMAT                   pixmap-formats
     m     LISTofSCREEN                    roots (m is always a multiple of 4)
FORMAT
     1     CARD8                           depth
     1     CARD8                           bits-per-pixel
     1     CARD8                           scanline-pad
     5                                     unused
SCREEN
     4     WINDOW                          root
     4     COLORMAP                        default-colormap
     4     CARD32                          white-pixel
     4     CARD32                          black-pixel
     4     SETofEVENT                      current-input-masks
     2     CARD16                          width-in-pixels
     2     CARD16                          height-in-pixels
     2     CARD16                          width-in-millimeters
     2     CARD16                          height-in-millimeters
     2     CARD16                          min-installed-maps
     2     CARD16                          max-installed-maps
     4     VISUALID                        root-visual
     1                                     backing-stores
          0     Never
          1     WhenMapped
          2     Always
     1     BOOL                            save-unders
     1     CARD8                           root-depth
     1     CARD8                           number of DEPTHs in allowed-depths
     n     LISTofDEPTH                     allowed-depths (n is always a
                                           multiple of 4)
DEPTH
     1     CARD8                           depth
     1                                     unused
     2     n                               number of VISUALTYPES in visuals
     4                                     unused
     24n     LISTofVISUALTYPE              visuals
VISUALTYPE
     4     VISUALID                        visual-id
     1                                     class
          0     StaticGray
          1     GrayScale
          2     StaticColor
          3     PseudoColor
          4     TrueColor
          5     DirectColor
     1     CARD8                           bits-per-rgb-value
     2     CARD16                          colormap-entries
     4     CARD32                          red-mask
     4     CARD32                          green-mask
     4     CARD32                          blue-mask
     4                                     unused
*/


#endif  // CONNECTION_SETUP_HPP
