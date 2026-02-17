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
    assert( argc >= 1 );
    const char* process_name { argv[ 0 ] };
    if ( argc == 1 ) {
        fmt::println( ::stderr, "{}: please provide error code",
                      process_name );
        return EXIT_FAILURE;
    }
    const int code { std::stoi( argv[ 1 ] ) };
    if ( !( code >= protocol::errors::codes::REQUEST &&            //  1
            code <= protocol::errors::codes::IMPLEMENTATION ) ) {  // 17
        fmt::println( ::stderr, "{}: invalid core error code: {} (expected 1-17)",
                      process_name, code );
        return EXIT_FAILURE;
    }

    // Open the connection to the X server
    xcb_connection_t* conn {
        xcb_connect( nullptr, nullptr ) };
    assert( conn != nullptr );
    // Get the first screen in `roots`
    const xcb_setup_t*  setup  { xcb_get_setup( conn ) };
    assert( setup  != nullptr );
    const xcb_screen_t* screen { xcb_setup_roots_iterator( setup ).data };
    assert( screen != nullptr );

    // When greater customization of the request encoding than that provided
    //   by the `xcb_`(request name) functions is required, we can use
    //   xcb_send_request, see:
    //   - https://xcb.freedesktop.org/ProtocolExtensionApi/
    //   - https://gitlab.freedesktop.org/xorg/lib/libxcb/-/blob/master/src/xcbext.h?ref_type=heads#L47-L83
    xcb_generic_error_t*      error          {};
    [[maybe_unused]] uint32_t bad_resource   {};
    [[maybe_unused]] uint16_t request_opcode {};
    namespace err_codes = protocol::errors::codes;
    namespace req_opcodes = protocol::requests::opcodes;
    switch ( code ) {
    case err_codes::REQUEST:         {  //  1
        const int     flags { XCB_REQUEST_CHECKED };
        // generic request header
        uint8_t       iov0_data[ 4 ] {
            0x00,       // opcode (will be set by xcb_send_request to request.opcode)
            0x00,       // unused
            0x00, 0x00  // length in 4B units (will be set by xcb_send_request)
        };
        // 4B unused suffix
        uint8_t       iov1_data[ 4 ] {
            0x01, 0x02, 0x03, 0x04
        };
        ::iovec vector_[ 2 ] {
            { iov0_data, sizeof( iov0_data ) },
            { iov1_data, sizeof( iov1_data ) }
        };
        const xcb_protocol_request_t request {
            sizeof( vector_ ) / sizeof( ::iovec ),  // count (of elements in iovec array)
            nullptr,                                // ext (nullptr for core protocol)
            req_opcodes::MIN - 1,                   // opcode (core protocol major opcode
                                                    //   deliberately out of range)
            int(true)                               // isvoid (no reply expected)
        };
        const xcb_void_cookie_t cookie {
            xcb_send_request( conn, flags, vector_, &request ) };
        error = xcb_request_check( conn, cookie );
    }   break;
    case err_codes::VALUE:           {  //  2
        // mode deliberately outside enum range
        const uint8_t         mode   { XCB_ALLOW_SYNC_BOTH + 1 };
        const xcb_timestamp_t time   {};
        const xcb_void_cookie_t cookie {
            xcb_allow_events_checked(conn, mode, time) };
        error = xcb_request_check(conn, cookie);
        bad_resource = mode;
        request_opcode = req_opcodes::ALLOWEVENTS;
    }   break;
    case err_codes::WINDOW:          {  //  3
        const xcb_window_t window {};
        const xcb_void_cookie_t cookie {
            xcb_destroy_window_checked(conn, window) };
        error = xcb_request_check(conn, cookie);
        bad_resource = window;
        request_opcode = req_opcodes::DESTROYWINDOW;
    }   break;
    case err_codes::PIXMAP:          {  //  4
        const xcb_pixmap_t pixmap {};
        const xcb_void_cookie_t cookie {
            xcb_free_pixmap_checked(conn, pixmap) };
        error = xcb_request_check(conn, cookie);
        bad_resource = pixmap;
        request_opcode = req_opcodes::FREEPIXMAP;
    }   break;
    case err_codes::ATOM:            {  //  5
        const xcb_atom_t selection { XCB_ATOM_NONE };
        const xcb_get_selection_owner_cookie_t cookie {
            xcb_get_selection_owner( conn, selection ) };
        [[maybe_unused]] const xcb_get_selection_owner_reply_t* reply {
            xcb_get_selection_owner_reply( conn, cookie, &error ) };
        assert( reply == nullptr );
        bad_resource = selection;
        request_opcode = req_opcodes::GETSELECTIONOWNER;
    }   break;
    case err_codes::CURSOR:          {  //  6
        const xcb_cursor_t cursor {};
        const xcb_void_cookie_t cookie {
            xcb_free_cursor_checked( conn, cursor ) };
        error = xcb_request_check( conn, cookie );
        bad_resource = cursor;
        request_opcode = req_opcodes::FREECURSOR;
    }   break;
    case err_codes::FONT:            {  //  7
        const xcb_font_t font {};
        const xcb_void_cookie_t cookie {
            xcb_close_font_checked( conn, font ) };
        error = xcb_request_check( conn, cookie );
        bad_resource = font;
        request_opcode = req_opcodes::CLOSEFONT;
    }   break;
    case err_codes::MATCH:           {  //  8
        // Match error on any depth other than 0/XCB_COPY_FROM_PARENT on
        //   InputOnly window
        const uint8_t        depth        { 1 };
        const xcb_window_t   wid          { xcb_generate_id ( conn ) };
        const xcb_window_t   parent       { screen->root };
        const int16_t        x            {};
        const int16_t        y            {};
        const uint16_t       width        { 150 };
        const uint16_t       height       { 150 };
        const uint16_t       border_width { 10 };
        const uint16_t       _class       { XCB_WINDOW_CLASS_INPUT_ONLY };
        const xcb_visualid_t visual       { screen->root_visual };
        const uint32_t       value_mask   {};
        const uint32_t*      value_list   {};
        const xcb_void_cookie_t cookie {
            xcb_create_window_checked(
                conn, depth, wid, parent, x, y, width, height, border_width,
                _class, visual, value_mask, value_list ) };
        error = xcb_request_check( conn, cookie );
        request_opcode = req_opcodes::CREATEWINDOW;
    }   break;
    case err_codes::DRAWABLE:        {  //  9
        const xcb_drawable_t drawable {};
        const xcb_get_geometry_cookie_t cookie {
            xcb_get_geometry( conn, drawable ) };
        [[maybe_unused]] const xcb_get_geometry_reply_t* reply {
            xcb_get_geometry_reply( conn, cookie, &error ) };
        assert( reply == nullptr );
        bad_resource = drawable;
        request_opcode = req_opcodes::GETGEOMETRY;
    }   break;
    case err_codes::ACCESS:          {  // 10
        // Access error from attempt to modify access control list by host other
        //   than localhost or unauthorized client
        const uint8_t      mode        { XCB_SET_MODE_DELETE };
        const uint8_t      family      { XCB_FAMILY_INTERNET };
        constexpr uint16_t address_len { 4 };
        const uint8_t      address[ address_len ]  {
            0x01, 0x02, 0x03, 0x04
        };
        const xcb_void_cookie_t cookie {
            xcb_change_hosts_checked(
                conn, mode, family, address_len, address ) };
        error = xcb_request_check( conn, cookie );
        request_opcode = req_opcodes::CHANGEHOSTS;
    }   break;
    case err_codes::ALLOC:           {  // 11
        // Alloc error can be emitted by AllocColorCells/AllocColorPlanes on a
        //   colormap with a VISUALTYPE of StaticGray, StaticColor, or TrueColor
        //   (which all prevent client allocation of cells and planes after initial
        //   CreateColormap request)
        xcb_visualtype_t* no_alloc_visualtype {};
        for ( xcb_depth_iterator_t depth_iter {
                xcb_screen_allowed_depths_iterator( screen ) };
              !no_alloc_visualtype && depth_iter.rem;
              xcb_depth_next( &depth_iter ) ) {
            for ( xcb_visualtype_iterator_t visual_iter {
                    xcb_depth_visuals_iterator( depth_iter.data ) };
                  !no_alloc_visualtype && visual_iter.rem;
                  xcb_visualtype_next( &visual_iter ) ) {
                switch ( visual_iter.data->_class ) {
                case XCB_VISUAL_CLASS_STATIC_GRAY:  [[fallthrough]];
                case XCB_VISUAL_CLASS_STATIC_COLOR: [[fallthrough]];
                case XCB_VISUAL_CLASS_TRUE_COLOR:
                    no_alloc_visualtype = visual_iter.data;
                    break;
                default:
                    break;
                }
            }
        }
        assert( no_alloc_visualtype != nullptr );
        const xcb_colormap_t mid   { xcb_generate_id( conn ) };
        const uint8_t        alloc { XCB_COLORMAP_ALLOC_NONE };
        xcb_create_colormap(
            conn, alloc, mid, screen->root, no_alloc_visualtype->visual_id );
        const uint8_t        contiguous {};
        const xcb_colormap_t cmap       { mid };
        const uint16_t       colors     { 1 };
        const uint16_t       planes     {};
        const xcb_alloc_color_cells_cookie_t cookie {
            xcb_alloc_color_cells(
                conn, contiguous, cmap, colors, planes ) };
        [[maybe_unused]] const xcb_alloc_color_cells_reply_t* reply {
            xcb_alloc_color_cells_reply( conn, cookie, &error ) };
        assert( reply == nullptr );
        request_opcode = req_opcodes::ALLOCCOLORCELLS;
        xcb_free_colormap( conn, cmap );
    }   break;
    case err_codes::COLORMAP:        {  // 12
        const xcb_colormap_t colormap {};
        const xcb_void_cookie_t cookie {
            xcb_free_colormap_checked( conn, colormap ) };
        error = xcb_request_check( conn, cookie );
        bad_resource = colormap;
        request_opcode = req_opcodes::FREECOLORMAP;
    }   break;
    case err_codes::GCONTEXT:        {  // 13
        const xcb_gcontext_t gcontext {};
        const xcb_void_cookie_t cookie {
            xcb_free_gc_checked( conn, gcontext ) };
        error = xcb_request_check( conn, cookie );
        bad_resource = gcontext;
        request_opcode = req_opcodes::FREEGC;
    }   break;
    case err_codes::IDCHOICE:        {  // 14
        const uint8_t        alloc { XCB_COLORMAP_ALLOC_NONE };
        // > "The resource-id-mask contains a single contiguous set of bits (at
        // > least 18). The client allocates resource IDs for types WINDOW,
        // >  PIXMAP, CURSOR, FONT, GCONTEXT, and COLORMAP by choosing a value
        // >  with only some subset of these bits set and ORing it with
        // >  resource-id-base.", see:`
        // - https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#server_information
        const xcb_colormap_t mid   {
            ( setup->resource_id_base | setup->resource_id_mask ) + 1 };
        const xcb_void_cookie_t cookie {
            xcb_create_colormap_checked(
                conn, alloc, mid, screen->root, screen->root_visual ) };
        error = xcb_request_check( conn, cookie );
        bad_resource = mid;
        request_opcode = req_opcodes::CREATECOLORMAP;
    }   break;
    case err_codes::NAME:            {  // 15
        const xcb_font_t       fid  { xcb_generate_id( conn ) };
        const std::string_view name { "" };
        const xcb_void_cookie_t cookie {
            xcb_open_font_checked( conn, fid, name.size(), name.data() ) };
        error = xcb_request_check( conn, cookie );
        request_opcode = req_opcodes::OPENFONT;
    }   break;
    case err_codes::LENGTH:          {  // 16
        // Length error from SetModifierMapping LISTofKEYCODE of length !=
        //   8 * keycodes-per-modifier
        const int flags          { XCB_REQUEST_CHECKED };
        // SetModifierMapping header
        uint8_t   iov0_data[ 4 ] {
            0x00,       // opcode (will be set by xcb_send_request to request.opcode)
            0x01,       // keycodes-per-modifier
            0x00, 0x00  // length in 4B units (will be set by xcb_send_request)
        };
        // LISTofKEYCODE length 4
        uint8_t   iov1_data[ 4 ] { 0x50, 0x51, 0x52, 0x53 };
        ::iovec   vector_[ 2 ] {
            { iov0_data, sizeof( iov0_data ) },
            { iov1_data, sizeof( iov1_data ) }
        };
        const xcb_protocol_request_t request {
            sizeof( vector_ ) / sizeof( ::iovec ), // count (of elements in iovec array)
            nullptr,                               // ext (nullptr for core protocol)
            req_opcodes::SETMODIFIERMAPPING,       // opcode
            int(false)                             // isvoid (reply expected)
         };
        const xcb_set_modifier_mapping_cookie_t cookie {
            xcb_send_request( conn, flags, vector_, &request ) };
        [[maybe_unused]] const xcb_set_modifier_mapping_reply_t* reply {
            xcb_set_modifier_mapping_reply( conn, cookie, &error ) };
        assert( reply == nullptr );
        request_opcode = req_opcodes::SETMODIFIERMAPPING;
    }   break;
    case err_codes::IMPLEMENTATION:  {  // 17
        // TBD not sure how to issue this error, as all core requests should
        //   be supported by a proper X11 server
        fmt::println( ::stderr, "{}: No test yet for Implementation error parsing!",
                      process_name );
    }   break;
    default:
        break;
    }

    switch ( code ) {
    case err_codes::IMPLEMENTATION:  // 17
        break;
    default:
        assert( error != nullptr );
        switch ( code ) {
        case err_codes::VALUE:       //  2
        case err_codes::WINDOW:      //  3
        case err_codes::PIXMAP:      //  4
        case err_codes::ATOM:        //  5
        case err_codes::CURSOR:      //  6
        case err_codes::FONT:        //  7
        case err_codes::DRAWABLE:    //  9
        case err_codes::COLORMAP:    // 12
        case err_codes::GCONTEXT:    // 13
        case err_codes::IDCHOICE:    // 14
            assert( error->resource_id == bad_resource );
            [[fallthrough]];
        case err_codes::REQUEST:     //  1
        case err_codes::MATCH:       //  8
        case err_codes::ACCESS:      // 10
        case err_codes::ALLOC:       // 11
        case err_codes::NAME:        // 15
        case err_codes::LENGTH:      // 16
            assert( error->response_type == 0 );
            assert( error->error_code == code );
            assert( error->major_code == request_opcode );
            break;
        default:
            break;
        }
        ::free( error );
        break;
    }

    xcb_flush( conn );
    xcb_disconnect( conn );
    return EXIT_SUCCESS;
}
