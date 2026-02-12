#include <string>  // atoi

#include <cassert>
#include <cstdint>
#include <cstdlib>  // EXIT_FAILURE EXIT_SUCCESS free

#include <fmt/format.h>

#include <xcb/xcb.h>

#include <protocol/requests.hpp>


int main( const int argc, const char* const* argv ) {
    assert( argc > 1 );
    const char* process_name { argv[ 0 ] };
    const int opcode { std::stoi( argv[ 1 ] ) };
    namespace opcodes = protocol::requests::opcodes;
    if ( !( ( opcode >= opcodes::CREATEWINDOW &&          //   1
              opcode <= opcodes::GETMODIFIERMAPPING ) ||  // 119
            opcode == opcodes::NOOPERATION ) ) {          // 127
        fmt::println( ::stderr, "{}: invalid core opcode: {} (expected 1-119,127)",
                      process_name, opcode );
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

    // AllocColorCells, AllocColorPlanes tests require that we find a VISUALTYPE
    //   that has a class other than StaticGray, StaticColor, or TrueColor (this
    //   allows client allocation of cells and planes after initial
    //   CreateColormap request)
    xcb_visualtype_t* alloc_visualtype {};
    for ( xcb_depth_iterator_t depth_iter {
            xcb_screen_allowed_depths_iterator( screen ) };
          !alloc_visualtype && depth_iter.rem;
          xcb_depth_next( &depth_iter ) ) {
        for ( xcb_visualtype_iterator_t visual_iter {
                xcb_depth_visuals_iterator( depth_iter.data ) };
              !alloc_visualtype && visual_iter.rem;
              xcb_visualtype_next( &visual_iter ) ) {
            switch ( visual_iter.data->_class ) {
            case XCB_VISUAL_CLASS_STATIC_GRAY:  [[fallthrough]];
            case XCB_VISUAL_CLASS_STATIC_COLOR: [[fallthrough]];
            case XCB_VISUAL_CLASS_TRUE_COLOR:
                break;
            default:
                alloc_visualtype = visual_iter.data;
                break;
            }
        }
    }
    assert( alloc_visualtype != nullptr );

    // for more on individual libxcb request calls, see:
    // - https://man.archlinux.org/ (search xcb function name) (often better than official)
    // - https://www.x.org/releases/X11R7.7/doc/man/man3/ (official but many incomplete)
    xcb_generic_error_t* error {};
    namespace opcodes = protocol::requests::opcodes;
    switch ( opcode ) {
    case opcodes::GETWINDOWATTRIBUTES:      {  //   3
        const xcb_window_t window { screen->root };
        const xcb_get_window_attributes_cookie_t cookie {
            xcb_get_window_attributes( conn, window ) };
        const xcb_get_window_attributes_reply_t* reply {
            xcb_get_window_attributes_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_window_attributes_reply_t* >( reply ) );
    }   break;
    case opcodes::GETGEOMETRY:              {  //  14
        const xcb_drawable_t drawable { screen->root };
        const xcb_get_geometry_cookie_t cookie {
            xcb_get_geometry( conn, drawable ) };
        const xcb_get_geometry_reply_t* reply {
            xcb_get_geometry_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        assert( reply->width == screen->width_in_pixels );
        assert( reply->height == screen->height_in_pixels );
        ::free( const_cast< xcb_get_geometry_reply_t* >( reply ) );
    }   break;
    case opcodes::QUERYTREE:                {  //  15
        const xcb_window_t window { screen->root };
        const xcb_query_tree_cookie_t cookie {
            xcb_query_tree( conn, window ) };
        const xcb_query_tree_reply_t* reply {
            xcb_query_tree_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_query_tree_reply_t* >( reply ) );
    }   break;
    case opcodes::INTERNATOM:               {  //  16
        const uint8_t  only_if_exists { true };
        const uint16_t name_len       { sizeof( "PRIMARY" ) - 1 };
        const char*    name           { "PRIMARY" };
        const xcb_intern_atom_cookie_t cookie {
            xcb_intern_atom( conn, only_if_exists, name_len, name ) };
        const xcb_intern_atom_reply_t* reply {
            xcb_intern_atom_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_intern_atom_reply_t* >( reply ) );
    }   break;
    case opcodes::GETATOMNAME:              {  //  17
        const xcb_atom_t atom { XCB_ATOM_PRIMARY };
        const xcb_get_atom_name_cookie_t cookie {
            xcb_get_atom_name( conn, atom ) };
        const xcb_get_atom_name_reply_t* reply {
            xcb_get_atom_name_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_atom_name_reply_t* >( reply ) );
    }   break;
    case opcodes::GETPROPERTY:              {  //  20
        const xcb_window_t window      { screen->root };
        const uint8_t      _delete     {};
        const xcb_atom_t   property    { XCB_ATOM_WM_NAME };
        const xcb_atom_t   type        { XCB_ATOM_STRING
                                         /* or XCB_GET_PROPERTY_TYPE_ANY */ };
        const uint32_t     long_offset {};
        // should be >= size of property value in bytes / 4 to get all of value
        const uint32_t     long_length { 10 };
        const xcb_get_property_cookie_t cookie {
            xcb_get_property( conn, _delete, window, property,
                              type, long_offset, long_length ) };
        const xcb_get_property_reply_t* reply {
            xcb_get_property_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_property_reply_t* >( reply ) );
    }   break;
    case opcodes::LISTPROPERTIES:           {  //  21
        const xcb_window_t window { screen->root };
        const xcb_list_properties_cookie_t cookie {
            xcb_list_properties( conn, window ) };
        const xcb_list_properties_reply_t* reply {
            xcb_list_properties_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_list_properties_reply_t* >( reply ) );
    }   break;
    case opcodes::GETSELECTIONOWNER:        {  //  23
        const xcb_atom_t selection { XCB_ATOM_PRIMARY };
        const xcb_get_selection_owner_cookie_t cookie {
            xcb_get_selection_owner( conn, selection ) };
        const xcb_get_selection_owner_reply_t* reply {
            xcb_get_selection_owner_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_selection_owner_reply_t* >( reply ) );
    }   break;
    case opcodes::GRABPOINTER:              {  //  26
        const uint8_t         owner_events  {};
        const xcb_window_t    grab_window   { screen->root };
        const uint16_t        event_mask    {};
        const uint8_t         pointer_mode  {};
        const uint8_t         keyboard_mode {};
        const xcb_window_t    confine_to    {};
        const xcb_cursor_t    cursor        {};
        const xcb_timestamp_t time          {};
        const xcb_grab_pointer_cookie_t cookie {
            xcb_grab_pointer( conn, owner_events, grab_window, event_mask,
                              pointer_mode, keyboard_mode, confine_to, cursor, time ) };
        const xcb_grab_pointer_reply_t* reply {
            xcb_grab_pointer_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_grab_pointer_reply_t* >( reply ) );
    }   break;
    case opcodes::GRABKEYBOARD:             {  //  31
        const uint8_t         owner_events  {};
        const xcb_window_t    grab_window   { screen->root };
        const xcb_timestamp_t time          {};
        const uint8_t         pointer_mode  {};
        const uint8_t         keyboard_mode {};
        const xcb_grab_keyboard_cookie_t cookie {
            xcb_grab_keyboard( conn, owner_events, grab_window, time,
                               pointer_mode, keyboard_mode) };
        const xcb_grab_keyboard_reply_t* reply {
            xcb_grab_keyboard_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_grab_keyboard_reply_t* >( reply ) );
    }   break;
    case opcodes::QUERYPOINTER:             {  //  38
        const xcb_window_t window { screen->root };
        const xcb_query_pointer_cookie_t cookie {
            xcb_query_pointer( conn, window ) };
        const xcb_query_pointer_reply_t* reply {
            xcb_query_pointer_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_query_pointer_reply_t* >( reply ) );
    }   break;
    case opcodes::GETMOTIONEVENTS:          {  //  39
        const xcb_window_t    window { screen->root };
        // start of 0 CurrentTime returns no events
        const xcb_timestamp_t start  { 1 };
        // 0 CurrentTime
        const xcb_timestamp_t stop   {};
        const xcb_get_motion_events_cookie_t cookie {
            xcb_get_motion_events( conn, window, start, stop ) };
        const xcb_get_motion_events_reply_t* reply {
            xcb_get_motion_events_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_motion_events_reply_t* >( reply ) );
    }   break;
    case opcodes::TRANSLATECOORDINATES:     {  //  40
        const xcb_window_t src_window { screen->root };
        const xcb_window_t dst_window { screen->root };
        const int16_t      src_x      {};
        const int16_t      src_y      {};
        const xcb_translate_coordinates_cookie_t cookie {
            xcb_translate_coordinates(
                conn, src_window, dst_window, src_x, src_y ) };
        const xcb_translate_coordinates_reply_t* reply {
            xcb_translate_coordinates_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_translate_coordinates_reply_t* >( reply ) );
    }   break;
    case opcodes::GETINPUTFOCUS:            {  //  43
        const xcb_get_input_focus_cookie_t cookie {
            xcb_get_input_focus( conn ) };
        const xcb_get_input_focus_reply_t* reply {
            xcb_get_input_focus_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_input_focus_reply_t* >( reply ) );
    }   break;
    case opcodes::QUERYKEYMAP:              {  //  44
        const xcb_query_keymap_cookie_t cookie {
            xcb_query_keymap( conn ) };
        const xcb_query_keymap_reply_t* reply {
            xcb_query_keymap_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_query_keymap_reply_t* >( reply ) );
    }   break;
    case opcodes::QUERYFONT:                {  //  47
        xcb_font_t fid { xcb_generate_id ( conn ) };
        {
            const uint16_t max_names { 1 };
            // selected for testing due to lower amount of CHARINFOs
            constexpr char pattern[] { "-misc-fixed-medium-r-normal-*" };
            constexpr uint16_t pattern_len ( sizeof( pattern ) - 1 );
            const xcb_list_fonts_cookie_t cookie {
                xcb_list_fonts( conn, max_names, pattern_len, pattern ) };
            const xcb_list_fonts_reply_t* reply {
                xcb_list_fonts_reply( conn, cookie, &error ) };
            assert( error == nullptr );
            assert( reply != nullptr );
            assert( reply->names_len > 0 );
            const xcb_str_t* str {
                xcb_list_fonts_names_iterator( reply ).data };
            assert( str != nullptr );
            xcb_open_font( conn, fid, str->name_len,
                           reinterpret_cast< const char* >( &( str->name_len ) + 1 ) );
            ::free( const_cast< xcb_list_fonts_reply_t* >( reply ) );
        }
        {
            const xcb_query_font_cookie_t cookie {
                xcb_query_font( conn, fid ) };
            const xcb_query_font_reply_t* reply {
                xcb_query_font_reply( conn, cookie, &error ) };
            assert( error == nullptr );
            assert( reply != nullptr );
            ::free( const_cast< xcb_query_font_reply_t* >( reply ) );
        }
        xcb_close_font( conn, fid );
    }   break;
    case opcodes::QUERYTEXTEXTENTS:         {  //  48
        xcb_font_t fid { xcb_generate_id ( conn ) };
        {
            const uint16_t max_names { 1 };
            // selected for testing due to lower amount of CHARINFOs
            constexpr char pattern[] { "-misc-fixed-medium-r-normal-*" };
            constexpr uint16_t pattern_len ( sizeof( pattern ) - 1 );
            const xcb_list_fonts_cookie_t cookie {
                xcb_list_fonts( conn, max_names, pattern_len, pattern ) };
            const xcb_list_fonts_reply_t* reply {
                xcb_list_fonts_reply( conn, cookie, &error ) };
            assert( error == nullptr );
            assert( reply != nullptr );
            assert( reply->names_len > 0 );
            const xcb_str_t* str {
                xcb_list_fonts_names_iterator( reply ).data };
            assert( str != nullptr );
            xcb_open_font( conn, fid, str->name_len,
                           reinterpret_cast< const char* >( &( str->name_len ) + 1 ) );
            ::free( const_cast< xcb_list_fonts_reply_t* >( reply ) );
        }
        {
            constexpr char16_t string[]   { u"zß水" };
            static_assert( sizeof( char16_t ) == sizeof( xcb_char2b_t ) );
            constexpr uint32_t string_len {
                ( sizeof( string ) / sizeof( char16_t ) ) - 1 };
            const xcb_query_text_extents_cookie_t cookie {
                xcb_query_text_extents(
                    conn, fid, string_len,
                    reinterpret_cast< const xcb_char2b_t* >( string ) ) };
            const xcb_query_text_extents_reply_t* reply {
                xcb_query_text_extents_reply( conn, cookie, &error ) };
            assert( error == nullptr );
            assert( reply != nullptr );
            ::free( const_cast< xcb_query_text_extents_reply_t* >( reply ) );
        }
        xcb_close_font( conn, fid );
    }   break;
    case opcodes::LISTFONTS:                {  //  49
        const uint16_t     max_names   { 10 };
        constexpr char     pattern[]   { "*" };
        constexpr uint16_t pattern_len { sizeof( pattern ) - 1 };
        const xcb_list_fonts_cookie_t cookie {
            xcb_list_fonts( conn, max_names, pattern_len, pattern ) };
        const xcb_list_fonts_reply_t* reply {
            xcb_list_fonts_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_list_fonts_reply_t* >( reply ) );
    }   break;
    case opcodes::LISTFONTSWITHINFO:        {  //  50
        const uint16_t     max_names   { 2 };
        constexpr char     pattern[]   { "*" };
        constexpr uint16_t pattern_len { sizeof( pattern ) - 1 };
        const xcb_list_fonts_with_info_cookie_t cookie {
            xcb_list_fonts_with_info( conn, max_names, pattern_len, pattern ) };
        const xcb_list_fonts_with_info_reply_t* reply {
            xcb_list_fonts_with_info_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_list_fonts_with_info_reply_t* >( reply ) );
    }   break;
    case opcodes::GETFONTPATH:              {  //  52
        const xcb_get_font_path_cookie_t cookie {
            xcb_get_font_path( conn ) };
        const xcb_get_font_path_reply_t* reply {
            xcb_get_font_path_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_font_path_reply_t* >( reply ) );
    }   break;
    case opcodes::GETIMAGE:                 {  //  73
        // TBD attempting to reproduce screenshot example from:
        //   - https://www.apriorit.com/dev-blog/672-lin-how-to-take-multi-monitor-screenshots-on-linux
        //   but Match error if any of x, y, width, height are non-zero...
        //   > "If the drawable is a window, the window must be viewable, and it
        //   > must be the case that, if there were no inferiors or overlapping
        //   > windows, the specified rectangle of the window would be fully visible
        //   > on the screen and wholly contained within the outside edges of the
        //   > window (or a Match error results).", see:
        //   - https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#requests:GetImage
        //   (window can be proved viewable by checking
        //   xcb_get_window_attributes_reply()->map_state == XCB_MAP_STATE_VIEWABLE)
        const uint8_t        format     { XCB_IMAGE_FORMAT_Z_PIXMAP };
        const xcb_drawable_t drawable   { screen->root };
        const int16_t        x          {};
        const int16_t        y          {};
        const uint16_t       width      { screen->width_in_pixels };
        const uint16_t       height     { screen->height_in_pixels };
        const uint32_t       plane_mask ( -1 );
        const xcb_get_image_cookie_t cookie {
            xcb_get_image( conn, format, drawable, x, y, width, height, plane_mask ) };
        const xcb_get_image_reply_t* reply {
            xcb_get_image_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_image_reply_t* >( reply ) );
    }   break;
    case opcodes::LISTINSTALLEDCOLORMAPS:   {  //  83
        const xcb_window_t window { screen->root };
        const xcb_list_installed_colormaps_cookie_t cookie {
            xcb_list_installed_colormaps( conn, window ) };
        const xcb_list_installed_colormaps_reply_t* reply {
            xcb_list_installed_colormaps_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_list_installed_colormaps_reply_t* >( reply ) );
    }   break;
    case opcodes::ALLOCCOLOR:               {  //  84
        const xcb_colormap_t cmap  { screen->default_colormap };
        const uint16_t       red   {};
        const uint16_t       green {};
        const uint16_t       blue  {};
        const xcb_alloc_color_cookie_t cookie {
            xcb_alloc_color( conn, cmap, red, green, blue ) };
        const xcb_alloc_color_reply_t* reply {
            xcb_alloc_color_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_alloc_color_reply_t* >( reply ) );
    }   break;
    case opcodes::ALLOCNAMEDCOLOR:          {  //  85
        // https://en.wikipedia.org/wiki/X11_color_names
        const xcb_colormap_t cmap     { screen->default_colormap };
        const uint16_t       name_len { sizeof( "Red" ) - 1 };
        const char*          name     { "Red" };
        const xcb_alloc_named_color_cookie_t cookie {
            xcb_alloc_named_color( conn, cmap, name_len, name ) };
        const xcb_alloc_named_color_reply_t* reply {
            xcb_alloc_named_color_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_alloc_named_color_reply_t* >( reply ) );
    }   break;
    case opcodes::ALLOCCOLORCELLS:          {  //  86
        const xcb_colormap_t mid   { xcb_generate_id( conn ) };
        const uint8_t        alloc { XCB_COLORMAP_ALLOC_NONE };
        xcb_create_colormap(
            conn, alloc, mid, screen->root, alloc_visualtype->visual_id );
        const uint8_t        contiguous { 1 };
        const xcb_colormap_t cmap       { mid };
        const uint16_t       colors     { 1 };
        const uint16_t       planes     { 1 };
        const xcb_alloc_color_cells_cookie_t cookie {
            xcb_alloc_color_cells(
                conn, contiguous, cmap, colors, planes ) };
        const xcb_alloc_color_cells_reply_t* reply {
            xcb_alloc_color_cells_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_alloc_color_cells_reply_t* >( reply ) );
        xcb_free_colormap( conn, mid );
    }   break;
    case opcodes::ALLOCCOLORPLANES:         {  //  87
        const xcb_colormap_t mid   { xcb_generate_id( conn ) };
        const uint8_t        alloc { XCB_COLORMAP_ALLOC_NONE };
        xcb_create_colormap(
            conn, alloc, mid, screen->root, alloc_visualtype->visual_id );
        const uint8_t        contiguous { 1 };
        const xcb_colormap_t cmap       { mid };
        const uint16_t       colors     { 1 };
        const uint16_t       reds       { 1 };
        const uint16_t       greens     { 1 };
        const uint16_t       blues      { 1 };
        const xcb_alloc_color_planes_cookie_t cookie {
            xcb_alloc_color_planes(
                conn, contiguous, cmap, colors, reds, greens, blues ) };
        const xcb_alloc_color_planes_reply_t* reply {
            xcb_alloc_color_planes_reply(conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_alloc_color_planes_reply_t* >( reply ) );
        xcb_free_colormap( conn, mid );
    }   break;
    case opcodes::QUERYCOLORS:              {  //  91
        const xcb_colormap_t cmap        { screen->default_colormap };
        const uint32_t       pixels_len  { 3 };
        const uint32_t       pixels[ 3 ] { 1, 2, 3 };
        const xcb_query_colors_cookie_t cookie {
            xcb_query_colors( conn, cmap, pixels_len, pixels ) };
        const xcb_query_colors_reply_t* reply {
            xcb_query_colors_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_query_colors_reply_t* >( reply ) );
    }   break;
    case opcodes::LOOKUPCOLOR:              {  //  92
        // https://en.wikipedia.org/wiki/X11_color_names
        const xcb_colormap_t cmap     { screen->default_colormap };
        constexpr char       name[]   { "Red" };
        constexpr uint16_t   name_len { sizeof( name ) - 1 };
        const xcb_lookup_color_cookie_t cookie {
            xcb_lookup_color( conn, cmap, name_len, name ) };
        const xcb_lookup_color_reply_t* reply {
            xcb_lookup_color_reply(conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_lookup_color_reply_t* >( reply ) );
    }   break;
    case opcodes::QUERYBESTSIZE:            {  //  97
        const uint8_t        _class   {};
        const xcb_drawable_t drawable { screen->root };
        const uint16_t       width    {};
        const uint16_t       height   {};
        const xcb_query_best_size_cookie_t cookie {
            xcb_query_best_size( conn, _class, drawable, width, height ) };
        const xcb_query_best_size_reply_t* reply {
            xcb_query_best_size_reply(conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_query_best_size_reply_t* >( reply ) );
    }   break;
    case opcodes::QUERYEXTENSION:           {  //  98
        constexpr char     name[]   { "MIT-SHM" };
        constexpr uint16_t name_len { sizeof( name ) - 1 };
        const xcb_query_extension_cookie_t cookie {
            xcb_query_extension( conn, name_len, name ) };
        const xcb_query_extension_reply_t* reply {
            xcb_query_extension_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_query_extension_reply_t* >( reply ) );
    }   break;
    case opcodes::LISTEXTENSIONS:           {  //  99
        const xcb_list_extensions_cookie_t cookie {
            xcb_list_extensions( conn ) };
        const xcb_list_extensions_reply_t* reply {
            xcb_list_extensions_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_list_extensions_reply_t* >( reply ) );
    }   break;
    case opcodes::GETKEYBOARDMAPPING:       {  // 101
        const xcb_keycode_t first_keycode { 0x10 };
        assert( first_keycode >= setup->min_keycode );
        const uint8_t       count         { 2 };
        assert( first_keycode + count - 1 <= setup->max_keycode );
        const xcb_get_keyboard_mapping_cookie_t cookie {
            xcb_get_keyboard_mapping( conn, first_keycode, count ) };
        const xcb_get_keyboard_mapping_reply_t* reply {
            xcb_get_keyboard_mapping_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_keyboard_mapping_reply_t* >( reply ) );
    }   break;
    case opcodes::GETKEYBOARDCONTROL:       {  // 103
        const xcb_get_keyboard_control_cookie_t cookie {
            xcb_get_keyboard_control( conn ) };
        const xcb_get_keyboard_control_reply_t* reply {
            xcb_get_keyboard_control_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_keyboard_control_reply_t* >( reply ) );
    }   break;
    case opcodes::GETPOINTERCONTROL:        {  // 106
        const xcb_get_pointer_control_cookie_t cookie {
            xcb_get_pointer_control( conn ) };
        const xcb_get_pointer_control_reply_t* reply {
            xcb_get_pointer_control_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_pointer_control_reply_t* >( reply ) );
    }   break;
    case opcodes::GETSCREENSAVER:           {  // 108
        const xcb_get_screen_saver_cookie_t cookie {
            xcb_get_screen_saver( conn ) };
        const xcb_get_screen_saver_reply_t* reply {
            xcb_get_screen_saver_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_screen_saver_reply_t* >( reply ) );
    }   break;
    case opcodes::LISTHOSTS:                {  // 110
        // `hosts` empty by default
        // TBD to test a populated host list one could do a ChangeHosts Insert
        //   before and Delete after, but doing do results in Access error
        //   (trying SetAccessControl Enable before also gives Access error)
        //   > "client must reside on the same host as the server and/or have been
        //   > granted permission by a server-dependent method", see:
        //   - https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#requests:ChangeHosts
        //   > "An attempt is made to modify the access control list from other
        //   > than the local host (or otherwise authorized client).", see Access under:
        //   - https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Errors
        const xcb_list_hosts_cookie_t cookie {
            xcb_list_hosts( conn ) };
        const xcb_list_hosts_reply_t* reply {
            xcb_list_hosts_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_list_hosts_reply_t* >( reply ) );
    }   break;
    case opcodes::SETPOINTERMAPPING:        {  // 116
        // SetPointerMapping map length must match that returned by
        //   GetPointerMapping
        const xcb_get_pointer_mapping_cookie_t gpm_cookie {
            xcb_get_pointer_mapping( conn ) };
        const xcb_get_pointer_mapping_reply_t* gpm_reply {
            xcb_get_pointer_mapping_reply( conn, gpm_cookie, &error ) };
        assert( error == nullptr );
        assert( gpm_reply != nullptr );
        const uint8_t  map_len { gpm_reply->map_len };
        const uint8_t* map     { xcb_get_pointer_mapping_map( gpm_reply ) };
        const xcb_set_pointer_mapping_cookie_t cookie {
            xcb_set_pointer_mapping( conn, map_len, map ) };
        ::free( const_cast< xcb_get_pointer_mapping_reply_t* >( gpm_reply ) );
        const xcb_set_pointer_mapping_reply_t* reply {
            xcb_set_pointer_mapping_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_set_pointer_mapping_reply_t* >( reply ) );
    }   break;
    case opcodes::GETPOINTERMAPPING:        {  // 117
        const xcb_get_pointer_mapping_cookie_t cookie {
            xcb_get_pointer_mapping( conn ) };
        const xcb_get_pointer_mapping_reply_t* reply {
            xcb_get_pointer_mapping_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_pointer_mapping_reply_t* >( reply ) );
    }   break;
    case opcodes::SETMODIFIERMAPPING:       {  // 118
        const xcb_get_modifier_mapping_cookie_t gmm_cookie {
            xcb_get_modifier_mapping( conn ) };
        const xcb_get_modifier_mapping_reply_t* gmm_reply {
            xcb_get_modifier_mapping_reply( conn, gmm_cookie, &error ) };
        assert( error == nullptr );
        assert( gmm_reply != nullptr );
        const uint8_t keycodes_per_modifier { gmm_reply->keycodes_per_modifier };
        const xcb_keycode_t* keycodes {
            xcb_get_modifier_mapping_keycodes( gmm_reply ) };
        assert( keycodes != nullptr );
        const xcb_set_modifier_mapping_cookie_t cookie {
            xcb_set_modifier_mapping( conn, keycodes_per_modifier, keycodes ) };
        ::free( const_cast< xcb_get_modifier_mapping_reply_t* >( gmm_reply ) );
        const xcb_set_modifier_mapping_reply_t* reply {
            xcb_set_modifier_mapping_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_set_modifier_mapping_reply_t* >( reply ) );
    }   break;
    case opcodes::GETMODIFIERMAPPING:       {  // 119
        const xcb_get_modifier_mapping_cookie_t cookie {
            xcb_get_modifier_mapping( conn ) };
        const xcb_get_modifier_mapping_reply_t* reply {
            xcb_get_modifier_mapping_reply( conn, cookie, &error ) };
        assert( error == nullptr );
        assert( reply != nullptr );
        ::free( const_cast< xcb_get_modifier_mapping_reply_t* >( reply ) );
    }   break;
    default:
        fmt::println( ::stderr,
                      "{}: Core protocol request {}({}) does not have a reply",
                      process_name, protocol::requests::names[ opcode ], opcode );
        break;
    }

    xcb_flush( conn );
    xcb_disconnect( conn );
    return EXIT_SUCCESS;
}
