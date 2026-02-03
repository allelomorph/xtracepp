#include <string>                 // atoi
#include <string_view>

#include <cassert>
#include <cstdint>
#include <cstdlib>                // free EXIT_FAILURE EXIT_SUCCESS
#include <sys/uio.h>              // iovec

#include <xcb/xcb.h>
#include <xcb/xcbext.h>           // xcb_protocol_request_t xcb_send_request

#include <protocol/errors.hpp>
#include <protocol/requests.hpp>

#include <fmt/format.h>


int main( const int argc, const char* const* argv ) {
    assert( argc > 1 );
    const int code { std::stoi( argv[ 1 ] ) };
    if ( !( code >= protocol::errors::codes::REQUEST &&            //  1
            code <= protocol::errors::codes::IMPLEMENTATION ) ) {  // 17
        fmt::println( ::stderr, "{}: invalid core error code: {} (expected 1-17)",
                      argv[0], code );
        return EXIT_FAILURE;
    }

    // Open the connection to the X server
    xcb_connection_t* connection  {
        xcb_connect( nullptr, nullptr ) };
    assert( connection != nullptr );

    // Get the first screen
    const xcb_setup_t*    setup  { xcb_get_setup( connection ) };
    xcb_screen_iterator_t iter   { xcb_setup_roots_iterator( setup ) };
    xcb_screen_t*         screen { iter.data };
    assert( setup  != nullptr );
    assert( screen != nullptr );

    xcb_generic_error_t* error          {};
    uint32_t             bad_resource   {};
    uint16_t             request_opcode {};
    switch ( code ) {
    case protocol::errors::codes::REQUEST:         {  //  1
        // Since normal intended use of xcb contains all core requests in
        //   wrapper functions, we need a lower-level way to send a request
        //   with bad opcode, here using xcb_send_request
        //   - https://xcb.freedesktop.org/ProtocolExtensionApi/
        //   - https://gitlab.freedesktop.org/xorg/lib/libxcb/-/blob/master/src/xcbext.h?ref_type=heads#L47-L83
        // TBD xtrace logs unknown requests with unparsed-data field:
        //   `000:<:0001:  8: Request(128): UNKNOWN opcode=0x00 opcode2=0x00 unparsed-data=0x01,0x02,0x03,0x04;`
        const int flags          { XCB_REQUEST_CHECKED };
        // request header { major-opcode, minor-opcode, tl_aligned_units }
        //   (major-opcode is ignored in favor of one in xcb_protocol_request_t param)
        uint8_t   iov0_data[ 4 ] { 0x00, 0x00, 0x00, 0x00 };
        uint8_t   iov1_data[ 4 ] { 0x01, 0x02, 0x03, 0x04 };  // test for unparsed-data
        ::iovec   vector_[ 2 ] {
            { iov0_data, sizeof( iov0_data ) },
            { iov1_data, sizeof( iov1_data ) },
        };
        const xcb_protocol_request_t request {
            sizeof( vector_ ) / sizeof( ::iovec ),
                       // count (of elements in iovec array)
            nullptr,   // ext (nullptr for core protocol)
            protocol::requests::opcodes::MIN - 1,
                       // opcode (core protocol major opcode deliberately out of range)
            int(true)  // isvoid (no reply expected)
        };
        const xcb_void_cookie_t cookie {
            xcb_send_request( connection, flags, vector_, &request ) };
        error = xcb_request_check( connection, cookie );
    }   break;
    case protocol::errors::codes::VALUE:           {  //  2
        // mode deliberately outside enum range
        const uint8_t         mode   { XCB_ALLOW_SYNC_BOTH + 1 };
        const xcb_timestamp_t time   {};
        const xcb_void_cookie_t cookie {
            xcb_allow_events_checked(connection, mode, time) };
        error = xcb_request_check(connection, cookie);
        bad_resource = mode;
        request_opcode = protocol::requests::opcodes::ALLOWEVENTS;
    }   break;
    case protocol::errors::codes::WINDOW:          {  //  3
        const xcb_window_t window {};
        const xcb_void_cookie_t cookie {
            xcb_destroy_window_checked(connection, window) };
        error = xcb_request_check(connection, cookie);
        bad_resource = window;
        request_opcode = protocol::requests::opcodes::DESTROYWINDOW;
    }   break;
    case protocol::errors::codes::PIXMAP:          {  //  4
        const xcb_pixmap_t pixmap {};
        const xcb_void_cookie_t cookie {
            xcb_free_pixmap_checked(connection, pixmap) };
        error = xcb_request_check(connection, cookie);
        bad_resource = pixmap;
        request_opcode = protocol::requests::opcodes::FREEPIXMAP;
    }   break;
    case protocol::errors::codes::ATOM:            {  //  5
        const xcb_atom_t selection { XCB_ATOM_NONE };
        const xcb_get_selection_owner_cookie_t cookie {
            xcb_get_selection_owner( connection, selection ) };
        const xcb_get_selection_owner_reply_t* reply {
            xcb_get_selection_owner_reply( connection, cookie, &error ) };
        assert( reply == nullptr );
        bad_resource = selection;
        request_opcode = protocol::requests::opcodes::GETSELECTIONOWNER;
    }   break;
    case protocol::errors::codes::CURSOR:          {  //  6
        const xcb_cursor_t cursor {};
        const xcb_void_cookie_t cookie {
            xcb_free_cursor_checked( connection, cursor ) };
        error = xcb_request_check( connection, cookie );
        bad_resource = cursor;
        request_opcode = protocol::requests::opcodes::FREECURSOR;
    }   break;
    case protocol::errors::codes::FONT:            {  //  7
        const xcb_font_t font {};
        const xcb_void_cookie_t cookie {
            xcb_close_font_checked( connection, font ) };
        error = xcb_request_check( connection, cookie );
        bad_resource = font;
        request_opcode = protocol::requests::opcodes::CLOSEFONT;
    }   break;
    case protocol::errors::codes::MATCH:           {  //  8
        // issue Match error by specifying non-zero depth on InputOnly window
        const xcb_void_cookie_t cookie {
            xcb_create_window_checked(
                connection,
                1,                               // depth (must be 0/XCB_COPY_FROM_PARENT for InputOnly)
                xcb_generate_id ( connection ),  // wid
                screen->root,                    // parent window
                0, 0,                            // x, y
                150, 150,                        // width, height
                10,                              // border_width
                XCB_WINDOW_CLASS_INPUT_ONLY,     // class
                screen->root_visual,             // visual
                0,                               // value_mask
                nullptr )                        // value_list
        };
        error = xcb_request_check( connection, cookie );
        request_opcode = protocol::requests::opcodes::CREATEWINDOW;
    }   break;
    case protocol::errors::codes::DRAWABLE:        {  //  9
        const xcb_drawable_t drawable {};
        const xcb_get_geometry_cookie_t cookie {
            xcb_get_geometry( connection, drawable ) };
        const xcb_get_geometry_reply_t* reply {
            xcb_get_geometry_reply( connection, cookie, &error ) };
        assert( reply == nullptr );
        bad_resource = drawable;
        request_opcode = protocol::requests::opcodes::GETGEOMETRY;
    }   break;
    case protocol::errors::codes::ACCESS:          {  // 10
        // TBD how to issue this error without creating colormap?
        const uint8_t alloc { XCB_COLORMAP_ALLOC_NONE };
        const xcb_colormap_t cmap { xcb_generate_id( connection ) };
        xcb_void_cookie_t cookie {
            xcb_create_colormap_checked(
                connection, alloc, cmap, screen->root, screen->root_visual ) };
        error = xcb_request_check( connection, cookie );
        assert( error == nullptr );
        const     uint32_t plane_mask           {};
        constexpr uint32_t PIXELS_LEN           { 1 };
        const     uint32_t pixels[ PIXELS_LEN ] {};
        cookie = xcb_free_colors_checked(
            connection, cmap, plane_mask, PIXELS_LEN, pixels );
        error = xcb_request_check( connection, cookie );
        request_opcode = protocol::requests::opcodes::FREECOLORS;
        xcb_free_colormap( connection, cmap );
    }   break;
    case protocol::errors::codes::ALLOC:           {  // 11
        // TBD not sure how to force allocation failure in server
        //   (currently AllocColorCells(86) reply test fails with Alloc)
        fmt::println( ::stderr, "{}: No test yet for Alloc error parsing!",
                      argv[0] );
    }   break;
    case protocol::errors::codes::COLORMAP:        {  // 12
        const xcb_colormap_t colormap {};
        const xcb_void_cookie_t cookie {
            xcb_free_colormap_checked( connection, colormap ) };
        error = xcb_request_check( connection, cookie );
        bad_resource = colormap;
        request_opcode = protocol::requests::opcodes::FREECOLORMAP;
    }   break;
    case protocol::errors::codes::GCONTEXT:        {  // 13
        const xcb_gcontext_t gcontext {};
        const xcb_void_cookie_t cookie {
            xcb_free_gc_checked( connection, gcontext ) };
        error = xcb_request_check( connection, cookie );
        bad_resource = gcontext;
        request_opcode = protocol::requests::opcodes::FREEGC;
    }   break;
    case protocol::errors::codes::IDCHOICE:        {  // 14
        // wid guaranteed to be out of range (see connection resource-id-base/
        //   resource-id-mask)
        const xcb_window_t window { 0xffffffff };
        const xcb_void_cookie_t cookie {
            xcb_create_window_checked(
                connection,
                XCB_COPY_FROM_PARENT,          // depth (same as root)
                window,                        // wid
                screen->root,                  // parent wid
                0, 0,                          // x, y
                150, 150,                      // width, height
                10,                            // border_width
                XCB_WINDOW_CLASS_INPUT_OUTPUT, // class
                screen->root_visual,           // visual
                0,                             // value_mask
                nullptr )                      // value_list
        };
        error = xcb_request_check( connection, cookie );
        bad_resource = window;
        request_opcode = protocol::requests::opcodes::CREATEWINDOW;
    }   break;
    case protocol::errors::codes::NAME:            {  // 15
        const xcb_font_t       fid  { xcb_generate_id( connection ) };
        const std::string_view name { "" };
        const xcb_void_cookie_t cookie {
            xcb_open_font_checked( connection, fid, name.size(), name.data() ) };
        error = xcb_request_check( connection, cookie );
        request_opcode = protocol::requests::opcodes::OPENFONT;
    }   break;
    case protocol::errors::codes::LENGTH:          {  // 16
        // Length error from LISTofKEYCODE of length != 8 * keycodes-per-modifier
        // xcb_set_modifier_mapping does not seem able to send LISTofKEYCODE
        //   with incorrect length, so we go lower into xcb_send_request
        const int flags          { XCB_REQUEST_CHECKED };
        // SetModifierMapping header { opcode, keycodes-per-modifier, request length }
        uint8_t   iov0_data[ 4 ] { 0x00, 0x01, 0x00, 0x00 };
        // LISTofKEYCODE length 4
        uint8_t   iov1_data[ 4 ] { 0x50, 0x51, 0x52, 0x53 };
        ::iovec   vector_[ 2 ] {
            { iov0_data, sizeof( iov0_data ) },
            { iov1_data, sizeof( iov1_data ) }
        };
        const xcb_protocol_request_t request {
            sizeof( vector_ ) / sizeof( ::iovec ),
                       // count (of elements in iovec array)
            nullptr,   // ext (nullptr for core protocol)
            protocol::requests::opcodes::SETMODIFIERMAPPING,
                       // opcode
            int(false) // isvoid (reply expected)
        };
        const xcb_set_modifier_mapping_cookie_t cookie {
            xcb_send_request( connection, flags, vector_, &request ) };
        const xcb_set_modifier_mapping_reply_t* reply {
            xcb_set_modifier_mapping_reply( connection, cookie, &error ) };
        assert( reply == nullptr );
        request_opcode = protocol::requests::opcodes::SETMODIFIERMAPPING;
    }   break;
    case protocol::errors::codes::IMPLEMENTATION:  {  // 17
        // TBD not sure how to issue this error, as all core requests should
        //   be supported by a proper X11 server
        fmt::println( ::stderr, "{}: No test yet for Implementation error parsing!",
                      argv[0] );
    }   break;
    default:
        break;
    }

    switch ( code ) {
    case protocol::errors::codes::REQUEST:         //  1
    case protocol::errors::codes::ALLOC:           // 11
    case protocol::errors::codes::IMPLEMENTATION:  // 17
        break;
    default:
        assert( error != nullptr );
        switch ( code ) {
        case protocol::errors::codes::VALUE:       //  2
        case protocol::errors::codes::WINDOW:      //  3
        case protocol::errors::codes::PIXMAP:      //  4
        case protocol::errors::codes::ATOM:        //  5
        case protocol::errors::codes::CURSOR:      //  6
        case protocol::errors::codes::FONT:        //  7
        case protocol::errors::codes::DRAWABLE:    //  9
        case protocol::errors::codes::COLORMAP:    // 12
        case protocol::errors::codes::GCONTEXT:    // 13
        case protocol::errors::codes::IDCHOICE:    // 14
            assert( error->resource_id == bad_resource );
            [[fallthrough]];
        case protocol::errors::codes::MATCH:       //  8
        case protocol::errors::codes::ACCESS:      // 10
        case protocol::errors::codes::NAME:        // 15
        case protocol::errors::codes::LENGTH:      // 16
            assert( error->response_type == 0 );
            assert( error->error_code == code );
            assert( error->major_code == request_opcode );
            break;
        default:
            break;
        }
        free( error );
        break;
    }

    xcb_flush( connection );
    xcb_disconnect( connection );
    return EXIT_SUCCESS;
}
