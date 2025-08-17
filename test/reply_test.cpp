// https://xcb.freedesktop.org/tutorial/basicwindowsanddrawing/

#include <unistd.h>  // free

#include <string>  // atoi

#include <cassert>

#include <xcb/xcb.h>
#include <protocol/requests.hpp>
#include <fmt/format.h>


int main(const int argc, const char* const* argv) {
    assert( argc > 1 );
    const int opcode { std::stoi(argv[1]) };
    assert( opcode >= 1 && opcode <= 127 );
    // Open the connection to the X server
    xcb_connection_t* connection  {
        xcb_connect( nullptr, nullptr ) };
    assert( connection != nullptr );

    // Get the first screen
    const xcb_setup_t*      setup  { xcb_get_setup( connection ) };
    xcb_screen_iterator_t   iter   { xcb_setup_roots_iterator( setup ) };
    xcb_screen_t*           screen { iter.data };
    assert( setup  != nullptr );
    assert( screen != nullptr );

    xcb_generic_error_t* error {};
    using namespace protocol::requests::opcodes;
    switch ( opcode ) {
    case GETWINDOWATTRIBUTES:      {  //   3
        const xcb_window_t      window { screen->root };
        const xcb_get_window_attributes_cookie_t cookie {
            xcb_get_window_attributes(connection, window) };
        const xcb_get_window_attributes_reply_t* reply {
            xcb_get_window_attributes_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_window_attributes_reply_t*>( reply ) );
    }   break;
    case GETGEOMETRY:              {  //  14
        const xcb_drawable_t    drawable { screen->root };
        const xcb_get_geometry_cookie_t cookie {
            xcb_get_geometry(connection, drawable) };
        const xcb_get_geometry_reply_t* reply {
            xcb_get_geometry_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_geometry_reply_t*>( reply ) );
    }   break;
    case QUERYTREE:                {  //  15
        const xcb_window_t      window {};
        const xcb_query_tree_cookie_t cookie {
            xcb_query_tree(connection, window) };
        const xcb_query_tree_reply_t* reply {
            xcb_query_tree_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_query_tree_reply_t*>( reply ) );
    }   break;
    case INTERNATOM:               {  //  16
        const uint8_t           only_if_exists {};
        const uint16_t          name_len       { 10 };
        const char*             name           { "TEST_ATOM" };
        const xcb_intern_atom_cookie_t cookie {
            xcb_intern_atom(connection, only_if_exists, name_len, name) };
        const xcb_intern_atom_reply_t* reply {
            xcb_intern_atom_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_intern_atom_reply_t*>( reply ) );
    }   break;
    case GETATOMNAME:              {  //  17
        const xcb_atom_t        atom { 1 };
        const xcb_get_atom_name_cookie_t cookie {
            xcb_get_atom_name(connection, atom) };
        const xcb_get_atom_name_reply_t* reply {
            xcb_get_atom_name_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_atom_name_reply_t*>( reply ) );
    }   break;
    case GETPROPERTY:              {  //  20
        const uint8_t           _delete     {};
        const xcb_window_t      window      {};
        const xcb_atom_t        property    { 1 };
        const xcb_atom_t        type        { 2 };
        const uint32_t          long_offset {};
        const uint32_t          long_length {};
        const xcb_get_property_cookie_t cookie {
            xcb_get_property(
                connection, _delete, window, property,
                type, long_offset, long_length) };
        const xcb_get_property_reply_t* reply {
            xcb_get_property_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_property_reply_t*>( reply ) );
    }   break;
    case LISTPROPERTIES:           {  //  21
        const xcb_window_t      window {};
        const xcb_list_properties_cookie_t cookie {
            xcb_list_properties(connection, window) };
        const xcb_list_properties_reply_t* reply {
            xcb_list_properties_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_list_properties_reply_t*>( reply ) );
    }   break;
    case GETSELECTIONOWNER:        {  //  23
        const xcb_atom_t        selection {};
        const xcb_get_selection_owner_cookie_t cookie {
            xcb_get_selection_owner(connection, selection) };
        const xcb_get_selection_owner_reply_t* reply {
            xcb_get_selection_owner_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_selection_owner_reply_t*>( reply ) );
    }   break;
    case GRABPOINTER:              {  //  26
        const uint8_t           owner_events {};
        const xcb_window_t      grab_window {};
        const uint16_t          event_mask {};
        const uint8_t           pointer_mode {};
        const uint8_t           keyboard_mode {};
        const xcb_window_t      confine_to {};
        const xcb_cursor_t      cursor {};
        const xcb_timestamp_t   time {};
        const xcb_grab_pointer_cookie_t cookie {
            xcb_grab_pointer(
                connection, owner_events, grab_window, event_mask,
                pointer_mode, keyboard_mode, confine_to, cursor, time) };
        const xcb_grab_pointer_reply_t* reply {
            xcb_grab_pointer_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_grab_pointer_reply_t*>( reply ) );
    }   break;
    case GRABKEYBOARD:             {  //  31
        const uint8_t           owner_events {};
        const xcb_window_t      grab_window {};
        const xcb_timestamp_t   time {};
        const uint8_t           pointer_mode {};
        const uint8_t           keyboard_mode {};
        const xcb_grab_keyboard_cookie_t cookie {
            xcb_grab_keyboard(
                connection, owner_events, grab_window, time,
                pointer_mode, keyboard_mode) };
        const xcb_grab_keyboard_reply_t* reply {
            xcb_grab_keyboard_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_grab_keyboard_reply_t*>( reply ) );
    }   break;
    case QUERYPOINTER:             {  //  38
        const xcb_window_t      window {};
        const xcb_query_pointer_cookie_t cookie {
            xcb_query_pointer(connection, window) };
        const xcb_query_pointer_reply_t* reply {
            xcb_query_pointer_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_query_pointer_reply_t*>( reply ) );
    }   break;
    case GETMOTIONEVENTS:          {  //  39
        const xcb_window_t      window {};
        const xcb_timestamp_t   start {};
        const xcb_timestamp_t   stop {};
        const xcb_get_motion_events_cookie_t cookie {
            xcb_get_motion_events(
                connection, window, start, stop) };
        const xcb_get_motion_events_reply_t* reply {
            xcb_get_motion_events_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_motion_events_reply_t*>( reply ) );
    }   break;
    case TRANSLATECOORDINATES:     {  //  40
        const xcb_window_t      src_window {};
        const xcb_window_t      dst_window {};
        const int16_t           src_x {  1 };
        const int16_t           src_y { -1 };
        const xcb_translate_coordinates_cookie_t cookie {
            xcb_translate_coordinates(
                connection, src_window, dst_window, src_x, src_y) };
        const xcb_translate_coordinates_reply_t* reply {
            xcb_translate_coordinates_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_translate_coordinates_reply_t*>( reply ) );
    }   break;
    case GETINPUTFOCUS:            {  //  43
        const xcb_get_input_focus_cookie_t cookie {
            xcb_get_input_focus(connection) };
        const xcb_get_input_focus_reply_t* reply {
            xcb_get_input_focus_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_input_focus_reply_t*>( reply ) );
    }   break;
    case QUERYKEYMAP:              {  //  44
        const xcb_query_keymap_cookie_t cookie {
            xcb_query_keymap(connection) };
        const xcb_query_keymap_reply_t* reply {
            xcb_query_keymap_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_query_keymap_reply_t*>( reply ) );
    }   break;
    case QUERYFONT:                {  //  47
        const xcb_fontable_t    font {};
        const xcb_query_font_cookie_t cookie {
            xcb_query_font(connection, font) };
        const xcb_query_font_reply_t* reply {
            xcb_query_font_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_query_font_reply_t*>( reply ) );
    }   break;
    case QUERYTEXTEXTENTS:         {  //  48
        const xcb_fontable_t    font {};
        const uint32_t          string_len { 3 };
        // const xcb_char2b_t*     string {
        //     reinterpret_cast<const xcb_char2b_t*>( u"¼½¾" ) };
        // const xcb_char2b_t*     string {
        //     reinterpret_cast<const xcb_char2b_t*>( u"ĀāĂ" ) };
        const xcb_char2b_t*     string {
            reinterpret_cast<const xcb_char2b_t*>( u"zß水" ) };
        const xcb_query_text_extents_cookie_t cookie {
            xcb_query_text_extents(
                connection, font, string_len, string) };
        const xcb_query_text_extents_reply_t* reply {
            xcb_query_text_extents_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_query_text_extents_reply_t*>( reply ) );
    }   break;
    case LISTFONTS:                {  //  49
        const uint16_t          max_names {};
        const uint16_t          pattern_len { 7 };
        const char*             pattern { "example" };
        const xcb_list_fonts_cookie_t cookie {
            xcb_list_fonts(
                connection, max_names, pattern_len, pattern) };
        const xcb_list_fonts_reply_t* reply {
            xcb_list_fonts_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_list_fonts_reply_t*>( reply ) );
    }   break;
    case LISTFONTSWITHINFO:        {  //  50
        const uint16_t          max_names {};
        const uint16_t          pattern_len { 7 };
        const char*             pattern { "example" };
        const xcb_list_fonts_with_info_cookie_t cookie {
            xcb_list_fonts_with_info(
                connection, max_names, pattern_len, pattern) };
        const xcb_list_fonts_with_info_reply_t* reply {
            xcb_list_fonts_with_info_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_list_fonts_with_info_reply_t*>( reply ) );
    }   break;
    case GETFONTPATH:              {  //  52
        const xcb_get_font_path_cookie_t cookie {
            xcb_get_font_path(connection) };
        const xcb_get_font_path_reply_t* reply {
            xcb_get_font_path_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_font_path_reply_t*>( reply ) );
    }   break;
   case GETIMAGE:                 {  //  73
        const uint8_t           format {};
        const xcb_drawable_t    drawable {};
        const int16_t           x {};
        const int16_t           y {};
        const uint16_t          width {};
        const uint16_t          height {};
        const uint32_t          plane_mask {};
        const xcb_get_image_cookie_t cookie {
            xcb_get_image(
                connection, format, drawable, x, y, width, height, plane_mask) };
        const xcb_get_image_reply_t* reply {
            xcb_get_image_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_image_reply_t*>( reply ) );
    }   break;
    case LISTINSTALLEDCOLORMAPS:   {  //  83
        const xcb_window_t      window {};
        const xcb_list_installed_colormaps_cookie_t cookie {
            xcb_list_installed_colormaps(connection, window) };
        const xcb_list_installed_colormaps_reply_t* reply {
            xcb_list_installed_colormaps_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_list_installed_colormaps_reply_t*>( reply ) );
    }   break;
    case ALLOCCOLOR:               {  //  84
        const xcb_colormap_t    cmap {};
        const uint16_t          red {};
        const uint16_t          green {};
        const uint16_t          blue {};
        const xcb_alloc_color_cookie_t cookie {
            xcb_alloc_color(
                connection, cmap, red, green, blue) };
        const xcb_alloc_color_reply_t* reply {
            xcb_alloc_color_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_alloc_color_reply_t*>( reply ) );
    }   break;
    case ALLOCNAMEDCOLOR:          {  //  85
        const xcb_colormap_t    cmap {};
        const uint16_t          name_len { 4 };
        const char*             name { "test" };
        const xcb_alloc_named_color_cookie_t cookie {
            xcb_alloc_named_color(
                connection, cmap, name_len, name) };
        const xcb_alloc_named_color_reply_t* reply {
            xcb_alloc_named_color_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_alloc_named_color_reply_t*>( reply ) );
    }   break;
    case ALLOCCOLORCELLS:          {  //  86
        const uint8_t           contiguous {};
        const xcb_colormap_t    cmap {};
        const uint16_t          colors {};
        const uint16_t          planes {};
        const xcb_alloc_color_cells_cookie_t cookie {
            xcb_alloc_color_cells(
                connection, contiguous, cmap, colors, planes) };
        const xcb_alloc_color_cells_reply_t* reply {
            xcb_alloc_color_cells_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_alloc_color_cells_reply_t*>( reply ) );
    }   break;
    case ALLOCCOLORPLANES:         {  //  87
        const uint8_t           contiguous {};
        const xcb_colormap_t    cmap {};
        const uint16_t          colors {};
        const uint16_t          reds {};
        const uint16_t          greens {};
        const uint16_t          blues {};
        const xcb_alloc_color_planes_cookie_t cookie {
            xcb_alloc_color_planes(
                connection, contiguous, cmap, colors, reds, greens, blues) };
        const xcb_alloc_color_planes_reply_t* reply {
            xcb_alloc_color_planes_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_alloc_color_planes_reply_t*>( reply ) );
    }   break;
    case QUERYCOLORS:              {  //  91
        const xcb_colormap_t    cmap {};
        const uint32_t          pixels_len { 3 };
        const uint32_t          pixels[3] {
            0x11111111, 0x22222222, 0x33333333
        };
        const xcb_query_colors_cookie_t cookie {
            xcb_query_colors(
                connection, cmap, pixels_len, pixels) };
        const xcb_query_colors_reply_t* reply {
            xcb_query_colors_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_query_colors_reply_t*>( reply ) );
    }   break;
    case LOOKUPCOLOR:              {  //  92
        const xcb_colormap_t    cmap {};
        const uint16_t          name_len { 4 };
        const char*             name { "test" };
        const xcb_lookup_color_cookie_t cookie {
            xcb_lookup_color(
                connection, cmap, name_len, name) };
        const xcb_lookup_color_reply_t* reply {
            xcb_lookup_color_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_lookup_color_reply_t*>( reply ) );
    }   break;
    case QUERYBESTSIZE:            {  //  97
        const uint8_t           _class {};
        const xcb_drawable_t    drawable {};
        const uint16_t          width {};
        const uint16_t          height {};
        const xcb_query_best_size_cookie_t cookie {
            xcb_query_best_size(
                connection, _class, drawable, width, height) };
        const xcb_query_best_size_reply_t* reply {
            xcb_query_best_size_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_query_best_size_reply_t*>( reply ) );
    }   break;
    case QUERYEXTENSION:           {  //  98
        const uint16_t          name_len { 4 };
        const char*             name { "test" };
        const xcb_query_extension_cookie_t cookie {
            xcb_query_extension(
                connection, name_len, name) };
        const xcb_query_extension_reply_t* reply {
            xcb_query_extension_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_query_extension_reply_t*>( reply ) );
    }   break;
    case LISTEXTENSIONS:           {  //  99
        const xcb_list_extensions_cookie_t cookie {
            xcb_list_extensions(connection) };
        const xcb_list_extensions_reply_t* reply {
            xcb_list_extensions_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_list_extensions_reply_t*>( reply ) );
    }   break;
    case GETKEYBOARDMAPPING:       {  // 101
        const xcb_keycode_t     first_keycode {};
        const uint8_t           count {};
        const xcb_get_keyboard_mapping_cookie_t cookie {
            xcb_get_keyboard_mapping(
                connection, first_keycode, count) };
        const xcb_get_keyboard_mapping_reply_t* reply {
            xcb_get_keyboard_mapping_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_keyboard_mapping_reply_t*>( reply ) );
    }   break;
    case GETKEYBOARDCONTROL:       {  // 103
        const xcb_get_keyboard_control_cookie_t cookie {
            xcb_get_keyboard_control(connection) };
        const xcb_get_keyboard_control_reply_t* reply {
            xcb_get_keyboard_control_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_keyboard_control_reply_t*>( reply ) );
    }   break;
    case GETPOINTERCONTROL:        {  // 106
        const xcb_get_pointer_control_cookie_t cookie {
            xcb_get_pointer_control(connection) };
        const xcb_get_pointer_control_reply_t* reply {
            xcb_get_pointer_control_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_pointer_control_reply_t*>( reply ) );
    }   break;
    case GETSCREENSAVER:           {  // 108
        const xcb_get_screen_saver_cookie_t cookie {
            xcb_get_screen_saver(connection) };
        const xcb_get_screen_saver_reply_t* reply {
            xcb_get_screen_saver_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_screen_saver_reply_t*>( reply ) );
    }   break;
    case LISTHOSTS:                {  // 110
        const xcb_list_hosts_cookie_t cookie {
            xcb_list_hosts(connection) };
        const xcb_list_hosts_reply_t* reply {
            xcb_list_hosts_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_list_hosts_reply_t*>( reply ) );
    }   break;
    case SETPOINTERMAPPING:        {  // 116
        const uint8_t           map_len { 4 };
        const uint8_t           map[4]  {
            0x01, 0x02, 0x03, 0x04
        };
        const xcb_set_pointer_mapping_cookie_t cookie {
            xcb_set_pointer_mapping(
                connection, map_len, map) };
        const xcb_set_pointer_mapping_reply_t* reply {
            xcb_set_pointer_mapping_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_set_pointer_mapping_reply_t*>( reply ) );
    }   break;
    case GETPOINTERMAPPING:        {  // 117
        const xcb_get_pointer_mapping_cookie_t cookie {
            xcb_get_pointer_mapping(connection) };
        const xcb_get_pointer_mapping_reply_t* reply {
            xcb_get_pointer_mapping_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_pointer_mapping_reply_t*>( reply ) );
    }   break;
    case SETMODIFIERMAPPING:       {  // 118
        // TBD "The number of keycodes in the list must be 8*keycodes-per-modifier
        //   (or a Length error results). "
        const uint8_t           keycodes_per_modifier { 2 };
        const xcb_keycode_t     keycodes[16] {
            0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
            0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
        };
        const xcb_set_modifier_mapping_cookie_t cookie {
            xcb_set_modifier_mapping(
                connection, keycodes_per_modifier, keycodes) };
        const xcb_set_modifier_mapping_reply_t* reply {
            xcb_set_modifier_mapping_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_set_modifier_mapping_reply_t*>( reply ) );
    }   break;
    case GETMODIFIERMAPPING:       {  // 119
        const xcb_get_modifier_mapping_cookie_t cookie {
            xcb_get_modifier_mapping(connection) };
        const xcb_get_modifier_mapping_reply_t* reply {
            xcb_get_modifier_mapping_reply(connection, cookie, &error) };
        assert( error == nullptr );
        assert( reply != nullptr );
        free( const_cast<xcb_get_modifier_mapping_reply_t*>( reply ) );
    }   break;
    default:
        fmt::println(
            "Core protocol request {}({}) does not have a reply",
            protocol::requests::names[ opcode ], opcode );
        break;
    }

    // Make sure commands are sent before we pause so that the window gets shown
    xcb_flush( connection );
    xcb_disconnect( connection );
}
