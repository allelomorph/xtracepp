#include <string>  // atoi

#include <cassert>
#include <cstdint>
#include <cstdlib>  // EXIT_FAILURE EXIT_SUCCESS

#include <fmt/format.h>

#include <xcb/xcb.h>

#include <protocol/requests.hpp>


int main( const int argc, const char* const* argv ) {
    assert( argc >= 1 );
    const char* process_name { argv[ 0 ] };
    if ( argc == 1 ) {
        fmt::println( ::stderr, "{}: please provide request opcode",
                      process_name );
        return EXIT_FAILURE;
    }
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

    // for more on individual libxcb request calls, see:
    // - https://man.archlinux.org/ (search xcb function name) (often better than official)
    // - https://www.x.org/releases/X11R7.7/doc/man/man3/ (official but many incomplete)
    switch ( opcode ) {
    case opcodes::CREATEWINDOW:             {  //   1
        const uint8_t        depth        { XCB_COPY_FROM_PARENT };  // (same as root)
        const xcb_window_t   wid          {};
        const xcb_window_t   parent       { screen->root };
        const int16_t        x            {};
        const int16_t        y            {};
        const uint16_t       width        { 150 };
        const uint16_t       height       { 150 };
        const uint16_t       border_width { 10 };
        const uint16_t       _class       { XCB_WINDOW_CLASS_INPUT_OUTPUT };
        const xcb_visualid_t visual       { screen->root_visual };
        const uint32_t       value_mask {
            XCB_CW_BACK_PIXMAP | XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXMAP |
            XCB_CW_BORDER_PIXEL | XCB_CW_BIT_GRAVITY | XCB_CW_WIN_GRAVITY |
            XCB_CW_BACKING_STORE | XCB_CW_BACKING_PLANES | XCB_CW_BACKING_PIXEL |
            XCB_CW_OVERRIDE_REDIRECT | XCB_CW_SAVE_UNDER | XCB_CW_EVENT_MASK |
            XCB_CW_DONT_PROPAGATE | XCB_CW_COLORMAP | XCB_CW_CURSOR };
        const uint32_t       value_list[15] {};
        xcb_create_window(
            conn, depth, wid, parent, x, y, width, height, border_width,
            _class, visual, value_mask, value_list );
    }   break;
    case opcodes::CHANGEWINDOWATTRIBUTES:   {  //   2
        const xcb_window_t window     {};
        const uint32_t     value_mask {
            XCB_CW_BACK_PIXMAP | XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXMAP |
            XCB_CW_BORDER_PIXEL | XCB_CW_BIT_GRAVITY | XCB_CW_WIN_GRAVITY |
            XCB_CW_BACKING_STORE | XCB_CW_BACKING_PLANES | XCB_CW_BACKING_PIXEL |
            XCB_CW_OVERRIDE_REDIRECT | XCB_CW_SAVE_UNDER | XCB_CW_EVENT_MASK |
            XCB_CW_DONT_PROPAGATE | XCB_CW_COLORMAP | XCB_CW_CURSOR
        };
        const uint32_t     value_list[15] {};
        xcb_change_window_attributes(
            conn, window, value_mask, value_list );
    }   break;
    case opcodes::GETWINDOWATTRIBUTES:      {  //   3
        const xcb_window_t window {};
        xcb_get_window_attributes(
            conn, window );
    }   break;
    case opcodes::DESTROYWINDOW:            {  //   4
        const xcb_window_t window {};
        xcb_destroy_window(
            conn, window );
    }   break;
    case opcodes::DESTROYSUBWINDOWS:        {  //   5
        const xcb_window_t window {};
        xcb_destroy_subwindows(
            conn, window );
    }   break;
    case opcodes::CHANGESAVESET:            {  //   6
        const uint8_t      mode {};
        const xcb_window_t window {};
        xcb_change_save_set(
            conn, mode, window );
    }   break;
    case opcodes::REPARENTWINDOW:           {  //   7
        const xcb_window_t window {};
        const xcb_window_t parent {};
        const int16_t      x {};
        const int16_t      y {};
        xcb_reparent_window(
            conn, window, parent, x, y );
    }   break;
    case opcodes::MAPWINDOW:                {  //   8
        const xcb_window_t window {};
        xcb_map_window(
            conn, window );
    }   break;
    case opcodes::MAPSUBWINDOWS:            {  //   9
        const xcb_window_t window {};
        xcb_map_subwindows(
            conn, window );
    }   break;
    case opcodes::UNMAPWINDOW:              {  //  10
        const xcb_window_t window {};
        xcb_unmap_window(
            conn, window );
    }   break;
    case opcodes::UNMAPSUBWINDOWS:          {  //  11
        const xcb_window_t window {};
        xcb_unmap_subwindows(
            conn, window );
    }   break;
    case opcodes::CONFIGUREWINDOW:          {  //  12
        const xcb_window_t window {};
        const uint16_t     value_mask {
            XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
            XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT |
            XCB_CONFIG_WINDOW_BORDER_WIDTH | XCB_CONFIG_WINDOW_SIBLING |
            XCB_CONFIG_WINDOW_STACK_MODE
        };
        const uint32_t     value_list[7] {};
        xcb_configure_window(
            conn, window, value_mask, value_list );
    }   break;
    case opcodes::CIRCULATEWINDOW:          {  //  13
        const uint8_t      direction {};
        const xcb_window_t window {};
        xcb_circulate_window(
            conn, direction, window );
    }   break;
    case opcodes::GETGEOMETRY:              {  //  14
        const xcb_drawable_t drawable {};
        xcb_get_geometry(
            conn, drawable );
    }   break;
    case opcodes::QUERYTREE:                {  //  15
        const xcb_window_t window {};
        xcb_query_tree(
            conn, window );
    }   break;
    case opcodes::INTERNATOM:               {  //  16
        const uint8_t  only_if_exists { true };
        const uint16_t name_len       { sizeof( "TEST_ATOM\x01\x02äß水" ) - 1 };
        const char*    name           { "TEST_ATOM\x01\x02äß水" };
        xcb_intern_atom(
            conn, only_if_exists, name_len, name );
    }   break;
    case opcodes::GETATOMNAME:              {  //  17
        const xcb_atom_t atom { 1 };
        xcb_get_atom_name(
            conn, atom );
    }   break;
    case opcodes::CHANGEPROPERTY:           {  //  18
        const uint8_t      mode     {};
        const xcb_window_t window   {};
        const xcb_atom_t   property { 1 };
        const xcb_atom_t   type     { 2 };
        const uint8_t      format   { 8 };  // 8 / 16 / 32
        const uint32_t     data_len { 4 };
        const uint8_t      data[4]  { 1, 2, 3, 4 };
        xcb_change_property(
            conn, mode, window, property,
            type, format, data_len, data );
    }   break;
    case opcodes::DELETEPROPERTY:           {  //  19
        const xcb_window_t window   {};
        const xcb_atom_t   property { 1 };
        xcb_delete_property(
            conn, window, property );
    }   break;
    case opcodes::GETPROPERTY:              {  //  20
        const uint8_t      _delete     {};
        const xcb_window_t window      {};
        const xcb_atom_t   property    { 1 };
        const xcb_atom_t   type        { 2 };
        const uint32_t     long_offset {};
        const uint32_t     long_length {};
        xcb_get_property(
            conn, _delete, window, property,
            type, long_offset, long_length );
    }   break;
    case opcodes::LISTPROPERTIES:           {  //  21
        const xcb_window_t window {};
        xcb_list_properties(
            conn, window );
    }   break;
    case opcodes::SETSELECTIONOWNER:        {  //  22
        const xcb_window_t    owner     {};
        const xcb_atom_t      selection { 1 };
        const xcb_timestamp_t time      { 61 };
        xcb_set_selection_owner(
            conn, owner, selection, time );
    }   break;
    case opcodes::GETSELECTIONOWNER:        {  //  23
        const xcb_atom_t selection {};
        xcb_get_selection_owner(
            conn, selection );
    }   break;
    case opcodes::CONVERTSELECTION:         {  //  24
        const xcb_window_t    requestor {};
        const xcb_atom_t      selection {};
        const xcb_atom_t      target    {};
        const xcb_atom_t      property  {};
        const xcb_timestamp_t time      {};
        xcb_convert_selection(
            conn, requestor, selection, target, property, time );
    }   break;
    case opcodes::SENDEVENT:                {  //  25
        // (minimum parameter population for xcb to send request)
        const uint8_t                propagate   {};
        const xcb_window_t           destination {};
        const uint32_t               event_mask  { XCB_EVENT_MASK_STRUCTURE_NOTIFY };
        xcb_configure_notify_event_t event       {};
        event.response_type = XCB_CONFIGURE_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask, (const char*)&event );
    }   break;
    case opcodes::GRABPOINTER:              {  //  26
        const uint8_t         owner_events  {};
        const xcb_window_t    grab_window   {};
        const uint16_t        event_mask    {};
        const uint8_t         pointer_mode  {};
        const uint8_t         keyboard_mode {};
        const xcb_window_t    confine_to    {};
        const xcb_cursor_t    cursor        {};
        const xcb_timestamp_t time          {};
        xcb_grab_pointer(
            conn, owner_events, grab_window, event_mask,
            pointer_mode, keyboard_mode, confine_to, cursor, time );
    }   break;
    case opcodes::UNGRABPOINTER:            {  //  27
        const xcb_timestamp_t time {};
        xcb_ungrab_pointer(
            conn, time);
    }   break;
    case opcodes::GRABBUTTON:               {  //  28
        const uint8_t      owner_events  {};
        const xcb_window_t grab_window   {};
        const uint16_t     event_mask    {};
        const uint8_t      pointer_mode  {};
        const uint8_t      keyboard_mode {};
        const xcb_window_t confine_to    {};
        const xcb_cursor_t cursor        {};
        const uint8_t      button        {};
        const uint16_t     modifiers     { XCB_MOD_MASK_ANY };  // AnyModifier
        xcb_grab_button(
            conn, owner_events, grab_window, event_mask,
            pointer_mode, keyboard_mode, confine_to, cursor, button, modifiers );
    }   break;
    case opcodes::UNGRABBUTTON:             {  //  29
        const uint8_t      button      {};
        const xcb_window_t grab_window {};
        const uint16_t     modifiers     { XCB_MOD_MASK_ANY };  // AnyModifier
        xcb_ungrab_button(
            conn, button, grab_window, modifiers );
    }   break;
    case opcodes::CHANGEACTIVEPOINTERGRAB:  {  //  30
        const xcb_cursor_t    cursor     {};
        const xcb_timestamp_t time       {};
        const uint16_t        event_mask { 0x0c };
        xcb_change_active_pointer_grab(
            conn, cursor, time, event_mask );
    }   break;
    case opcodes::GRABKEYBOARD:             {  //  31
        const uint8_t         owner_events  {};
        const xcb_window_t    grab_window   {};
        const xcb_timestamp_t time          {};
        const uint8_t         pointer_mode  {};
        const uint8_t         keyboard_mode {};
        xcb_grab_keyboard(
            conn, owner_events, grab_window, time,
            pointer_mode, keyboard_mode );
    }   break;
    case opcodes::UNGRABKEYBOARD:           {  //  32
        const xcb_timestamp_t time {};
        xcb_ungrab_keyboard(
            conn, time);
    }   break;
    case opcodes::GRABKEY:                  {  //  33
        const uint8_t       owner_events  {};
        const xcb_window_t  grab_window   {};
        const uint16_t      modifiers     {};
        const xcb_keycode_t key           {};
        const uint8_t       pointer_mode  {};
        const uint8_t       keyboard_mode {};
        xcb_grab_key(
            conn, owner_events, grab_window,
            modifiers, key, pointer_mode, keyboard_mode );
    }   break;
    case opcodes::UNGRABKEY:                {  //  34
        const xcb_keycode_t key         {};
        const xcb_window_t  grab_window {};
        const uint16_t      modifiers   { 0x03 };
        xcb_ungrab_key(
            conn, key, grab_window, modifiers );
    }   break;
    case opcodes::ALLOWEVENTS:              {  //  35
        const uint8_t         mode {};
        const xcb_timestamp_t time {};
        xcb_allow_events(
            conn, mode, time );
    }   break;
    case opcodes::GRABSERVER:               {  //  36
        xcb_grab_server(
            conn );
    }   break;
    case opcodes::UNGRABSERVER:             {  //  37
        xcb_grab_server(
            conn );
    }   break;
    case opcodes::QUERYPOINTER:             {  //  38
        const xcb_window_t window {};
        xcb_query_pointer(
            conn, window );
    }   break;
    case opcodes::GETMOTIONEVENTS:          {  //  39
        const xcb_window_t    window {};
        const xcb_timestamp_t start  {};
        const xcb_timestamp_t stop   {};
        xcb_get_motion_events(
            conn, window, start, stop );
    }   break;
    case opcodes::TRANSLATECOORDINATES:     {  //  40
        const xcb_window_t src_window {};
        const xcb_window_t dst_window {};
        const int16_t      src_x      { 1 };
        const int16_t      src_y      { -1 };
        xcb_translate_coordinates(
            conn, src_window, dst_window, src_x, src_y );
    }   break;
    case opcodes::WARPPOINTER:              {  //  41
        const xcb_window_t src_window {};
        const xcb_window_t dst_window {};
        const int16_t      src_x      {};
        const int16_t      src_y      {};
        const uint16_t     src_width  {};
        const uint16_t     src_height {};
        const int16_t      dst_x      {};
        const int16_t      dst_y      {};
        xcb_warp_pointer(
            conn, src_window, dst_window,
            src_x, src_y, src_width, src_height, dst_x, dst_y );
    }   break;
    case opcodes::SETINPUTFOCUS:            {  //  42
        const uint8_t         revert_to {};
        const xcb_window_t    focus     {};
        const xcb_timestamp_t time      {};
        xcb_set_input_focus(
            conn, revert_to, focus, time );
    }   break;
    case opcodes::GETINPUTFOCUS:            {  //  43
        xcb_get_input_focus(
            conn );
    }   break;
    case opcodes::QUERYKEYMAP:              {  //  44
        xcb_query_keymap (
            conn );
    }   break;
    case opcodes::OPENFONT:                 {  //  45
        const xcb_font_t fid      {};
        const uint16_t   name_len { 11 };
        const char*      name     { "ExampleFont" };
        xcb_open_font(
            conn, fid, name_len, name );
    }   break;
    case opcodes::CLOSEFONT:                {  //  46
        const xcb_font_t font {};
        xcb_close_font(
            conn, font );
    }   break;
    case opcodes::QUERYFONT:                {  //  47
        const xcb_fontable_t font {};
        xcb_query_font(
            conn, font );
    }   break;
    case opcodes::QUERYTEXTEXTENTS:         {  //  48
        const xcb_fontable_t font       {};
        const uint32_t       string_len { 3 };
        const xcb_char2b_t*  string {
            reinterpret_cast< const xcb_char2b_t* >( u"zß水" ) };
        xcb_query_text_extents(
            conn, font, string_len, string );
    }   break;
    case opcodes::LISTFONTS:                {  //  49
        const uint16_t max_names   {};
        const uint16_t pattern_len { 7 };
        const char*    pattern     { "example" };
        xcb_list_fonts(
            conn, max_names, pattern_len, pattern );
    }   break;
    case opcodes::LISTFONTSWITHINFO:        {  //  50
        const uint16_t max_names   {};
        const uint16_t pattern_len { 7 };
        const char*    pattern     { "example" };
        xcb_list_fonts_with_info(
            conn, max_names, pattern_len, pattern );
    }   break;
    case opcodes::SETFONTPATH:              {  //  51
        constexpr uint16_t font_qty { 3 };
        const char* data {
            "\x05" "first"
            "\x06" "second"
            "\x05" "third"
        };
        xcb_set_font_path(
            conn, font_qty, (const xcb_str_t*)data );
    }   break;
    case opcodes::GETFONTPATH:              {  //  52
        xcb_get_font_path(
            conn );
    }   break;
    case opcodes::CREATEPIXMAP:             {  //  53
        const uint8_t        depth    {};
        const xcb_pixmap_t   pid      {};
        const xcb_drawable_t drawable {};
        const uint16_t       window    {};
        const uint16_t       height   {};
        xcb_create_pixmap(
            conn, depth, pid, drawable, window, height );
    }   break;
    case opcodes::FREEPIXMAP:               {  //  54
        const xcb_pixmap_t pixmap {};
        xcb_free_pixmap(
            conn, pixmap );
    }   break;
    case opcodes::CREATEGC:                 {  //  55
        const xcb_gcontext_t cid {};
        const xcb_drawable_t drawable {};
        const uint32_t       value_mask {
            XCB_GC_FUNCTION | XCB_GC_PLANE_MASK | XCB_GC_FOREGROUND | XCB_GC_BACKGROUND |
            XCB_GC_LINE_WIDTH | XCB_GC_LINE_STYLE | XCB_GC_CAP_STYLE | XCB_GC_JOIN_STYLE |
            XCB_GC_FILL_STYLE | XCB_GC_FILL_RULE | XCB_GC_TILE | XCB_GC_STIPPLE |
            XCB_GC_TILE_STIPPLE_ORIGIN_X | XCB_GC_TILE_STIPPLE_ORIGIN_Y | XCB_GC_FONT |
            XCB_GC_SUBWINDOW_MODE | XCB_GC_GRAPHICS_EXPOSURES | XCB_GC_CLIP_ORIGIN_X |
            XCB_GC_CLIP_ORIGIN_Y | XCB_GC_CLIP_MASK | XCB_GC_DASH_OFFSET |
            XCB_GC_DASH_LIST | XCB_GC_ARC_MODE
        };
        const uint32_t       value_list[23] {};
        xcb_create_gc(
            conn, cid, drawable, value_mask, (const void*)value_list );
    }   break;
    case opcodes::CHANGEGC:                 {  //  56
        const xcb_gcontext_t gc {};
        const uint32_t       value_mask {
            XCB_GC_FUNCTION | XCB_GC_PLANE_MASK | XCB_GC_FOREGROUND | XCB_GC_BACKGROUND |
            XCB_GC_LINE_WIDTH | XCB_GC_LINE_STYLE | XCB_GC_CAP_STYLE | XCB_GC_JOIN_STYLE |
            XCB_GC_FILL_STYLE | XCB_GC_FILL_RULE | XCB_GC_TILE | XCB_GC_STIPPLE |
            XCB_GC_TILE_STIPPLE_ORIGIN_X | XCB_GC_TILE_STIPPLE_ORIGIN_Y | XCB_GC_FONT |
            XCB_GC_SUBWINDOW_MODE | XCB_GC_GRAPHICS_EXPOSURES | XCB_GC_CLIP_ORIGIN_X |
            XCB_GC_CLIP_ORIGIN_Y | XCB_GC_CLIP_MASK | XCB_GC_DASH_OFFSET |
            XCB_GC_DASH_LIST | XCB_GC_ARC_MODE
        };
        const uint32_t       value_list[23] {};
        xcb_change_gc(
            conn, gc, value_mask, (const void*)value_list );
    }   break;
    case opcodes::COPYGC:                   {  //  57
        const xcb_gcontext_t src_gc {};
        const xcb_gcontext_t dst_gc {};
        const uint32_t       value_mask {
            XCB_GC_FUNCTION | XCB_GC_PLANE_MASK | XCB_GC_FOREGROUND | XCB_GC_BACKGROUND |
            XCB_GC_LINE_WIDTH | XCB_GC_LINE_STYLE | XCB_GC_CAP_STYLE | XCB_GC_JOIN_STYLE |
            XCB_GC_FILL_STYLE | XCB_GC_FILL_RULE | XCB_GC_TILE | XCB_GC_STIPPLE |
            XCB_GC_TILE_STIPPLE_ORIGIN_X | XCB_GC_TILE_STIPPLE_ORIGIN_Y | XCB_GC_FONT |
            XCB_GC_SUBWINDOW_MODE | XCB_GC_GRAPHICS_EXPOSURES | XCB_GC_CLIP_ORIGIN_X |
            XCB_GC_CLIP_ORIGIN_Y | XCB_GC_CLIP_MASK | XCB_GC_DASH_OFFSET |
            XCB_GC_DASH_LIST | XCB_GC_ARC_MODE
        };
        xcb_copy_gc(
            conn, src_gc, dst_gc, value_mask );
    }   break;
    case opcodes::SETDASHES:                {  //  58
        xcb_gcontext_t gc          {};
        const uint16_t dash_offset {};
        const uint16_t dashes_len  { 4 };
        const uint8_t  dashes[4]   { 0x01, 0x02, 0x03, 0x04 };
        xcb_set_dashes(
            conn, gc, dash_offset, dashes_len, dashes );
    }   break;
    case opcodes::SETCLIPRECTANGLES:        {  //  59
        const uint8_t         ordering       {};
        const xcb_gcontext_t  gc             {};
        const int16_t         clip_x_origin  {};
        const int16_t         clip_y_origin  {};
        const uint32_t        rectangles_len { 2 };
        const xcb_rectangle_t rectangles[2]  {
            { 1, -1, 10, 10 },
            { 2, -2, 20, 20 }
        };
        xcb_set_clip_rectangles(
            conn, ordering, gc, clip_x_origin, clip_y_origin,
            rectangles_len, rectangles );
    }   break;
    case opcodes::FREEGC:                   {  //  60
        const xcb_gcontext_t gc {};
        xcb_free_gc(
            conn, gc );
    }   break;
    case opcodes::CLEARAREA:                {  //  61
        const uint8_t      exposures {};
        const xcb_window_t window    {};
        const int16_t      x         {};
        const int16_t      y         {};
        const uint16_t     width     {};
        const uint16_t     height    {};
        xcb_clear_area(
            conn, exposures, window, x, y, width, height );
    }   break;
    case opcodes::COPYAREA:                 {  //  62
        const xcb_drawable_t src_drawable {};
        const xcb_drawable_t dst_drawable {};
        const xcb_gcontext_t gc           {};
        const int16_t        src_x        {};
        const int16_t        src_y        {};
        const int16_t        dst_x        {};
        const int16_t        dst_y        {};
        const uint16_t       width        {};
        const uint16_t       height       {};
        xcb_copy_area(
            conn, src_drawable, dst_drawable, gc,
            src_x, src_y, dst_x, dst_y, width, height );
    }   break;
    case opcodes::COPYPLANE:                {  //  63
        const xcb_drawable_t src_drawable {};
        const xcb_drawable_t dst_drawable {};
        const xcb_gcontext_t gc           {};
        const int16_t        src_x        {};
        const int16_t        src_y        {};
        const int16_t        dst_x        {};
        const int16_t        dst_y        {};
        const uint16_t       width        {};
        const uint16_t       height       {};
        const uint32_t       bit_plane    {};
        xcb_copy_plane(
            conn, src_drawable, dst_drawable, gc,
            src_x, src_y, dst_x, dst_y, width, height, bit_plane );
    }
        break;
    case opcodes::POLYPOINT:                {  //  64
        const uint8_t        coordinate_mode {};
        const xcb_drawable_t drawable        {};
        const xcb_gcontext_t gc              {};
        const uint32_t       points_len      { 2 };
        const xcb_point_t    points[2]       {
            { 1, -1 },
            { 2, -2 }
        };
        xcb_poly_point(
            conn, coordinate_mode, drawable, gc, points_len, points );
    }   break;
    case opcodes::POLYLINE:                 {  //  65
        const uint8_t        coordinate_mode {};
        const xcb_drawable_t drawable        {};
        const xcb_gcontext_t gc              {};
        const uint32_t       points_len      { 2 };
        const xcb_point_t    points[2]       {
            { 1, -1 },
            { 2, -2 }
        };
        xcb_poly_line(
            conn, coordinate_mode, drawable, gc, points_len, points);
    }   break;
    case opcodes::POLYSEGMENT:              {  //  66
        const xcb_drawable_t drawable     {};
        const xcb_gcontext_t gc           {};
        const uint32_t       segments_len { 2 };
        const xcb_segment_t  segments[2]  {
            { 1, -1, 2, -2 },
            { 3, -3, 4, -4 }
        };
        xcb_poly_segment(
            conn, drawable, gc, segments_len, segments );
    }   break;
    case opcodes::POLYRECTANGLE:            {  //  67
        const xcb_drawable_t  drawable       {};
        const xcb_gcontext_t  gc             {};
        const uint32_t        rectangles_len { 2 };
        const xcb_rectangle_t rectangles[2]  {
            { 1, -1, 10, 10 },
            { 2, -2, 20, 20 }
        };
        xcb_poly_rectangle(
            conn, drawable, gc, rectangles_len, rectangles );
    }   break;
    case opcodes::POLYARC:                  {  //  68
        const xcb_drawable_t drawable {};
        const xcb_gcontext_t gc       {};
        const uint32_t       arcs_len { 2 };
        const xcb_arc_t      arcs[2]  {
            { 1, -1, 10, 11, 100, 101 },
            { 2, -2, 20, 21, 200, 201 }
        };
        xcb_poly_arc(
            conn, drawable, gc, arcs_len, arcs );
    }   break;
    case opcodes::FILLPOLY:                 {  //  69
        const xcb_drawable_t drawable        {};
        const xcb_gcontext_t gc              {};
        const uint8_t        shape           {};
        const uint8_t        coordinate_mode {};
        const uint32_t       points_len      { 2 };
        const xcb_point_t    points[2]       {
            { 1, -1 },
            { 2, -2 }
        };
        xcb_fill_poly(
            conn, drawable, gc, shape, coordinate_mode, points_len, points );
    }   break;
    case opcodes::POLYFILLRECTANGLE:        {  //  70
        const xcb_drawable_t  drawable       {};
        const xcb_gcontext_t  gc             {};
        const uint32_t        rectangles_len { 2 };
        const xcb_rectangle_t rectangles[2]  {
            { 1, -1, 10, 10 },
            { 2, -2, 20, 20 }
        };
        xcb_poly_fill_rectangle(
            conn, drawable, gc, rectangles_len, rectangles );
    }   break;
    case opcodes::POLYFILLARC:              {  //  71
        const xcb_drawable_t drawable {};
        const xcb_gcontext_t gc       {};
        const uint32_t       arcs_len { 2 };
        const xcb_arc_t      arcs[2]  {
            { 1, -1, 10, 11, 100, 101 },
            { 2, -2, 20, 21, 200, 201 }
        };
        xcb_poly_fill_arc(
            conn, drawable, gc, arcs_len, arcs );
    }   break;
    case opcodes::PUTIMAGE:                 {  //  72
        const uint8_t        format   {};
        const xcb_drawable_t drawable {};
        const xcb_gcontext_t gc       {};
        const uint16_t       window   {};
        const uint16_t       height   {};
        const int16_t        dst_x    {};
        const int16_t        dst_y    {};
        const uint8_t        left_pad {};
        const uint8_t        depth    {};
        const uint32_t       data_len { 4 };
        const uint8_t        data[4]  {
            0x01, 0x02, 0x03, 0x04
        };
        xcb_put_image(
            conn, format, drawable, gc, window, height,
            dst_x, dst_y, left_pad, depth, data_len, data );
    }   break;
    case opcodes::GETIMAGE:                 {  //  73
        const uint8_t        format     {};
        const xcb_drawable_t drawable   {};
        const int16_t        x          {};
        const int16_t        y          {};
        const uint16_t       window     {};
        const uint16_t       height     {};
        const uint32_t       plane_mask {};
        xcb_get_image(
            conn, format, drawable, x, y, window, height, plane_mask );
    }   break;
    case opcodes::POLYTEXT8:                {  //  74
        const xcb_drawable_t drawable  {};
        const xcb_gcontext_t gc        {};
        const int16_t        x         {};
        const int16_t        y         {};
        const uint32_t       items_len { 11 };
        const uint8_t        items[11] {
            // TEXTITEM8::TEXTELT8
            0x04,                // length in CHAR
            0x00,                // delta
            't', 'e', 's', 't',  // string
            // TEXTITEM8::FONT
            0xff,                   // font-shift
            0x01, 0x02, 0x03, 0x04  // font as big endian (nbo) uint32_t
        };
        xcb_poly_text_8(
            conn, drawable, gc, x, y, items_len, items );
    }
        break;
    case opcodes::POLYTEXT16:               {  //  75
        const xcb_drawable_t drawable  {};
        const xcb_gcontext_t gc        {};
        const int16_t        x         {};
        const int16_t        y         {};
        const uint32_t       items_len { 13 };
        const uint8_t           items[13] {
            // TEXTITEM16::TEXTELT16
            0x03,        // length in CHAR2B
            0x00,        // delta
                         // string:
            0x7a, 0x00,  // u'z' as CHAR2B/little endian (hbo) uint16_t
            0xdf, 0x00,  // u'ß' as CHAR2B/little endian (hbo) uint16_t
            0x34, 0x6c,  // u'水' as CHAR2B/little endian (hbo) uint16_t
            // TEXTITEM16::FONT
            0xff,                   // font-shift
            0x01, 0x02, 0x03, 0x04  // font as big endian (nbo) uint32_t
        };
        xcb_poly_text_16(
            conn, drawable, gc, x, y, items_len, items );
    }   break;
    case opcodes::IMAGETEXT8:               {  //  76
        const uint8_t        string_len { 4 };
        const xcb_drawable_t drawable   {};
        const xcb_gcontext_t gc         {};
        const int16_t        x          {};
        const int16_t        y          {};
        const char*          string     { "test" };
        xcb_image_text_8(
            conn, string_len, drawable, gc, x, y, string );
    }   break;
    case opcodes::IMAGETEXT16:              {  //  77
        const uint8_t        string_len { 3 };
        const xcb_drawable_t drawable   {};
        const xcb_gcontext_t gc         {};
        const int16_t        x          {};
        const int16_t        y          {};
        const xcb_char2b_t*  string     {
            reinterpret_cast<const xcb_char2b_t*>( u"zß水" ) };
        xcb_image_text_16(
            conn, string_len, drawable, gc, x, y, string );
    }   break;
    case opcodes::CREATECOLORMAP:           {  //  78
        const uint8_t        alloc  {};
        const xcb_colormap_t mid    {};
        const xcb_window_t   window {};
        const xcb_visualid_t visual {};
        xcb_create_colormap(
            conn, alloc, mid, window, visual );
    }   break;
    case opcodes::FREECOLORMAP:             {  //  79
        const xcb_colormap_t cmap {};
        xcb_free_colormap(
            conn, cmap );
    }   break;
    case opcodes::COPYCOLORMAPANDFREE:      {  //  80
        const xcb_colormap_t mid      {};
        const xcb_colormap_t src_cmap {};
        xcb_copy_colormap_and_free(
            conn, mid, src_cmap );
    }   break;
    case opcodes::INSTALLCOLORMAP:          {  //  81
        const xcb_colormap_t cmap {};
        xcb_install_colormap (
            conn, cmap );
    }   break;
    case opcodes::UNINSTALLCOLORMAP:        {  //  82
        const xcb_colormap_t cmap {};
        xcb_uninstall_colormap(
            conn, cmap );
    }   break;
    case opcodes::LISTINSTALLEDCOLORMAPS:   {  //  83
        const xcb_window_t window {};
        xcb_list_installed_colormaps(
            conn, window );
    }   break;
    case opcodes::ALLOCCOLOR:               {  //  84
        const xcb_colormap_t cmap {};
        const uint16_t       red {};
        const uint16_t       green {};
        const uint16_t       blue {};
        xcb_alloc_color(
            conn, cmap, red, green, blue );
    }   break;
    case opcodes::ALLOCNAMEDCOLOR:          {  //  85
        const xcb_colormap_t cmap     {};
        const uint16_t       name_len { 4 };
        const char*          name     { "test" };
        xcb_alloc_named_color(
            conn, cmap, name_len, name );
    }   break;
    case opcodes::ALLOCCOLORCELLS:          {  //  86
        const uint8_t        contiguous {};
        const xcb_colormap_t cmap       {};
        const uint16_t       colors     {};
        const uint16_t       planes     {};
        xcb_alloc_color_cells(
            conn, contiguous, cmap, colors, planes );
    }   break;
    case opcodes::ALLOCCOLORPLANES:         {  //  87
        const uint8_t        contiguous {};
        const xcb_colormap_t cmap       {};
        const uint16_t       colors     {};
        const uint16_t       reds       {};
        const uint16_t       greens     {};
        const uint16_t       blues      {};
        xcb_alloc_color_planes(
            conn, contiguous, cmap, colors, reds, greens, blues );
    }
        break;
    case opcodes::FREECOLORS:               {  //  88
        const xcb_colormap_t cmap       {};
        const uint32_t       plane_mask {};
        const uint32_t       pixels_len { 3 };
        const uint32_t       pixels[3]  {
            0x11111111, 0x22222222, 0x33333333
        };
        xcb_free_colors(
            conn, cmap, plane_mask, pixels_len, pixels );
    }   break;
    case opcodes::STORECOLORS:              {  //  89
        const xcb_colormap_t  cmap      {};
        const uint32_t        items_len { 2 };
        // TBD use C++20 for member dot initialization
        const xcb_coloritem_t items[2]  {
            // { pixel, red, green, blue, flags, pad0(unused) }
            { 1, 2, 3, 4, XCB_COLOR_FLAG_GREEN, 0 },
            { 5, 6, 7, 8, XCB_COLOR_FLAG_RED | XCB_COLOR_FLAG_BLUE, 0 }
        };
        xcb_store_colors(
            conn, cmap, items_len, items );
    }   break;
    case opcodes::STORENAMEDCOLOR:          {  //  90
        const uint8_t        flags    {};
        const xcb_colormap_t cmap     {};
        const uint32_t       pixel    {};
        const uint16_t       name_len { 4 };
        const char*          name     { "test" };
        xcb_store_named_color(
            conn, flags, cmap, pixel, name_len, name );
    }   break;
    case opcodes::QUERYCOLORS:              {  //  91
        const xcb_colormap_t cmap       {};
        const uint32_t       pixels_len { 3 };
        const uint32_t       pixels[3]  {
            0x11111111, 0x22222222, 0x33333333
        };
        xcb_query_colors(
            conn, cmap, pixels_len, pixels );
    }   break;
    case opcodes::LOOKUPCOLOR:              {  //  92
        const xcb_colormap_t cmap     {};
        const uint16_t       name_len { 4 };
        const char*          name     { "test" };
        xcb_lookup_color(
            conn, cmap, name_len, name );
    }   break;
    case opcodes::CREATECURSOR:             {  //  93
        const xcb_cursor_t cid         {};
        const xcb_pixmap_t source      {};
        const xcb_pixmap_t mask        {};
        const uint16_t     fore_red    {};
        const uint16_t     fore_green  {};
        const uint16_t     fore_blue   {};
        const uint16_t     back_red    {};
        const uint16_t     back_green  {};
        const uint16_t     back_blue   {};
        const uint16_t     x           {};
        const uint16_t     y           {};
        xcb_create_cursor(
            conn, cid, source, mask,
            fore_red, fore_green, fore_blue,
            back_red, back_green, back_blue, x, y );
    }   break;
    case opcodes::CREATEGLYPHCURSOR:        {  //  94
        const xcb_cursor_t cid         {};
        const xcb_font_t   source_font {};
        const xcb_font_t   mask_font   {};
        const uint16_t     source_char {};
        const uint16_t     mask_char   {};
        const uint16_t     fore_red    {};
        const uint16_t     fore_green  {};
        const uint16_t     fore_blue   {};
        const uint16_t     back_red    {};
        const uint16_t     back_green  {};
        const uint16_t     back_blue   {};
        xcb_create_glyph_cursor(
            conn, cid, source_font, mask_font, source_char, mask_char,
            fore_red, fore_green, fore_blue, back_red, back_green, back_blue );
    }   break;
    case opcodes::FREECURSOR:               {  //  95
        const xcb_cursor_t cursor {};
        xcb_free_cursor(
            conn, cursor );
    }   break;
    case opcodes::RECOLORCURSOR:            {  //  96
        const xcb_cursor_t cursor     {};
        const uint16_t     fore_red   {};
        const uint16_t     fore_green {};
        const uint16_t     fore_blue  {};
        const uint16_t     back_red   {};
        const uint16_t     back_green {};
        const uint16_t     back_blue  {};
        xcb_recolor_cursor(
            conn, cursor, fore_red, fore_green, fore_blue,
            back_red, back_green, back_blue );
    }   break;
    case opcodes::QUERYBESTSIZE:            {  //  97
        const uint8_t        _class   {};
        const xcb_drawable_t drawable {};
        const uint16_t       window   {};
        const uint16_t       height   {};
        xcb_query_best_size(
            conn, _class, drawable, window, height );
    }   break;
    case opcodes::QUERYEXTENSION:           {  //  98
        const uint16_t name_len { 4 };
        const char*    name     { "test" };
        xcb_query_extension(
            conn, name_len, name );
    }   break;
    case opcodes::LISTEXTENSIONS:           {  //  99
        xcb_list_extensions(
            conn );
    }   break;
    case opcodes::CHANGEKEYBOARDMAPPING:    {  // 100
        const uint8_t       keycode_count       { 2 };
        const xcb_keycode_t first_keycode       { setup->min_keycode };
        const uint8_t       keysyms_per_keycode { 3 };
        const xcb_keysym_t  keysyms[6]          {
            0x11111111, 0x22222222, 0x33333333,
            0x44444444, 0x55555555, 0x66666666
        };
        assert( sizeof(keysyms) / sizeof(xcb_keysym_t) ==
                (keycode_count * keysyms_per_keycode) );
        assert( first_keycode + (sizeof(keysyms) / keysyms_per_keycode) - 1 <=
                setup->max_keycode );
        xcb_change_keyboard_mapping (
            conn, keycode_count, first_keycode,
            keysyms_per_keycode, keysyms );

    }   break;
    case opcodes::GETKEYBOARDMAPPING:       {  // 101
        const xcb_keycode_t first_keycode {};
        const uint8_t       count         {};
        xcb_get_keyboard_mapping (
            conn, first_keycode, count);
    }   break;
    case opcodes::CHANGEKEYBOARDCONTROL:    {  // 102
        const uint32_t value_mask {
            XCB_KB_KEY_CLICK_PERCENT | XCB_KB_BELL_PERCENT | XCB_KB_BELL_PITCH |
            XCB_KB_BELL_DURATION | XCB_KB_LED | XCB_KB_LED_MODE | XCB_KB_KEY |
            XCB_KB_AUTO_REPEAT_MODE
        };
        const uint32_t value_list[8] {};
        xcb_change_keyboard_control(
            conn, value_mask, (const void*)value_list );
    }   break;
    case opcodes::GETKEYBOARDCONTROL:       {  // 103
        xcb_get_keyboard_control(
            conn );
    }   break;
    case opcodes::BELL:                     {  // 104
        const int8_t percent {};
        xcb_bell(
            conn, percent );
    }   break;
    case opcodes::CHANGEPOINTERCONTROL:     {  // 105
        const int16_t acceleration_numerator   {};
        const int16_t acceleration_denominator {};
        const int16_t threshold                {};
        const uint8_t do_acceleration          {};
        const uint8_t do_threshold             {};
        xcb_change_pointer_control(
            conn, acceleration_numerator, acceleration_denominator,
            threshold, do_acceleration, do_threshold );
    }   break;
    case opcodes::GETPOINTERCONTROL:        {  // 106
        xcb_get_pointer_control(conn);
    }   break;
    case opcodes::SETSCREENSAVER:           {  // 107
        const int16_t           timeout {};
        const int16_t           interval {};
        const uint8_t           prefer_blanking {};
        const uint8_t           allow_exposures {};
        xcb_set_screen_saver(
            conn, timeout, interval, prefer_blanking, allow_exposures);
    }   break;
    case opcodes::GETSCREENSAVER:           {  // 108
        xcb_get_screen_saver(
            conn );
    }   break;
    case opcodes::CHANGEHOSTS:              {  // 109
        const uint8_t  mode        {};
        const uint8_t  family      {};
        const uint16_t address_len { 4 };
        const uint8_t  address[4]  {
            0x01, 0x02, 0x03, 0x04
        };
        xcb_change_hosts(
            conn, mode, family, address_len, address );
    }   break;
    case opcodes::LISTHOSTS:                {  // 110
        xcb_list_hosts(
            conn );
    }   break;
    case opcodes::SETACCESSCONTROL:         {  // 111
        const uint8_t mode {};
        xcb_set_access_control(
            conn, mode );
    }   break;
    case opcodes::SETCLOSEDOWNMODE:         {  // 112
        const uint8_t mode {};
        xcb_set_close_down_mode(
            conn, mode );
    }   break;
    case opcodes::KILLCLIENT:               {  // 113
        const uint32_t resource {};
        xcb_kill_client (
            conn, resource );
    }   break;
    case opcodes::ROTATEPROPERTIES:         {  // 114
        const xcb_window_t window    {};
        const uint16_t     atoms_len { 3 };
        const int16_t      delta     {};
        const xcb_atom_t   atoms[3]  {
            0x01, 0x02, 0x03
        };
        xcb_rotate_properties(
            conn, window, atoms_len, delta, atoms );
    }   break;
    case opcodes::FORCESCREENSAVER:         {  // 115
        const uint8_t mode {};
        xcb_force_screen_saver(
            conn, mode );

    }   break;
    case opcodes::SETPOINTERMAPPING:        {  // 116
        const uint8_t map_len { 4 };
        const uint8_t map[4]  {
            0x01, 0x02, 0x03, 0x04
        };
        xcb_set_pointer_mapping(
            conn, map_len, map );
    }   break;
    case opcodes::GETPOINTERMAPPING:        {  // 117
        xcb_get_pointer_mapping(
            conn );
     }   break;
    case opcodes::SETMODIFIERMAPPING:       {  // 118
        const uint8_t       keycodes_per_modifier { 2 };
        const xcb_keycode_t keycodes[16]          {
            0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
            0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
        };
        assert( sizeof(keycodes) / sizeof(xcb_keycode_t) ==
                8 * keycodes_per_modifier );
        xcb_set_modifier_mapping(
            conn, keycodes_per_modifier, keycodes );
    }   break;
    case opcodes::GETMODIFIERMAPPING:       {  // 119
        xcb_get_modifier_mapping(
            conn );
    }   break;
    case opcodes::NOOPERATION:              {  // 127
        xcb_no_operation(
            conn );
    }   break;
    default:
        break;
    }

    xcb_flush( conn );
    xcb_disconnect( conn );
    return EXIT_SUCCESS;
}
