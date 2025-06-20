// https://xcb.freedesktop.org/tutorial/basicwindowsanddrawing/

#include <string>  // atoi

#include <cassert>

#include <xcb/xcb.h>
#include <protocol/requests.hpp>


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

    switch ( opcode ) {
    case protocol::requests::opcodes::CREATEWINDOW:             {  //   1
        // Create the window
        const xcb_window_t window { xcb_generate_id ( connection ) };
        const uint32_t value_list[15] {
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0
        };
        xcb_create_window(
            connection,                    // Connection
            XCB_COPY_FROM_PARENT,          // depth (same as root)
            window,                        // window id
            screen->root,                  // parent window
            0, 0,                          // x, y
            150, 150,                      // width, height
            10,                            // border_width
            XCB_WINDOW_CLASS_INPUT_OUTPUT, // class
            screen->root_visual,           // visual
            XCB_CW_BACK_PIXMAP | XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXMAP |
            XCB_CW_BORDER_PIXEL | XCB_CW_BIT_GRAVITY | XCB_CW_WIN_GRAVITY |
            XCB_CW_BACKING_STORE | XCB_CW_BACKING_PLANES | XCB_CW_BACKING_PIXEL |
            XCB_CW_OVERRIDE_REDIRECT | XCB_CW_SAVE_UNDER | XCB_CW_EVENT_MASK |
            XCB_CW_DONT_PROPAGATE | XCB_CW_COLORMAP | XCB_CW_CURSOR,
            // value-mask
            value_list );                 // value-list
    }   break;
    case protocol::requests::opcodes::CHANGEWINDOWATTRIBUTES:   {  //   2
        const xcb_window_t      window {};
        const uint32_t          value_mask {};
        const uint32_t          value_list[15] {
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0
        };
        xcb_change_window_attributes(
            connection,
            window,
            XCB_CW_BACK_PIXMAP | XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXMAP |
            XCB_CW_BORDER_PIXEL | XCB_CW_BIT_GRAVITY | XCB_CW_WIN_GRAVITY |
            XCB_CW_BACKING_STORE | XCB_CW_BACKING_PLANES | XCB_CW_BACKING_PIXEL |
            XCB_CW_OVERRIDE_REDIRECT | XCB_CW_SAVE_UNDER | XCB_CW_EVENT_MASK |
            XCB_CW_DONT_PROPAGATE | XCB_CW_COLORMAP | XCB_CW_CURSOR,
            // value-mask
            value_list );                 // value-list
    }   break;
    case protocol::requests::opcodes::GETWINDOWATTRIBUTES:      {  //   3
        const xcb_window_t      window {};
        xcb_get_window_attributes(
            connection, window );
    }   break;
    case protocol::requests::opcodes::DESTROYWINDOW:            {  //   4
        const xcb_window_t      window {};
        xcb_destroy_window(
            connection, window);
    }   break;
    case protocol::requests::opcodes::DESTROYSUBWINDOWS:        {  //   5
        const xcb_window_t      window {};
        xcb_destroy_subwindows(
            connection, window);
    }   break;
    case protocol::requests::opcodes::CHANGESAVESET:            {  //   6
        const uint8_t           mode {};
        const xcb_window_t      window {};
        xcb_change_save_set
            (connection, mode, window);
    }   break;
    case protocol::requests::opcodes::REPARENTWINDOW:           {  //   7
        const xcb_window_t      window {};
        const xcb_window_t      parent {};
        const int16_t           x {};
        const int16_t           y {};
        xcb_reparent_window(
            connection, window, parent, x, y);
    }   break;
    case protocol::requests::opcodes::MAPWINDOW:                {  //   8
        const xcb_window_t      window {};
        xcb_map_window(
            connection, window);
    }   break;
    case protocol::requests::opcodes::MAPSUBWINDOWS:            {  //   9
        const xcb_window_t      window {};
        xcb_map_subwindows(
            connection, window);
    }   break;
    case protocol::requests::opcodes::UNMAPWINDOW:              {  //  10
        const xcb_window_t      window {};
        xcb_unmap_window(
            connection, window);
    }   break;
    case protocol::requests::opcodes::UNMAPSUBWINDOWS:          {  //  11
        const xcb_window_t      window {};
        xcb_unmap_subwindows(
            connection, window);
    }   break;
    case protocol::requests::opcodes::CONFIGUREWINDOW:          {  //  12
        const xcb_window_t      window {};
        const uint16_t          value_mask {};
        const void*             value_list {};
        xcb_configure_window(
            connection, window, value_mask, value_list);
    }   break;
    case protocol::requests::opcodes::CIRCULATEWINDOW:          {  //  13
        const uint8_t           direction {};
        const xcb_window_t      window {};
        xcb_circulate_window(
            connection, direction, window);
    }   break;
    case protocol::requests::opcodes::GETGEOMETRY:              {  //  14
        const xcb_drawable_t    drawable {};
        xcb_get_geometry(
            connection, drawable);
    }   break;
    case protocol::requests::opcodes::QUERYTREE:                {  //  15
        const xcb_window_t      window {};
        xcb_query_tree(
            connection, window);
    }   break;
    case protocol::requests::opcodes::INTERNATOM:               {  //  16
        const uint8_t           only_if_exists {};
        const uint16_t          name_len {};
        const char*             name {};
        xcb_intern_atom(
            connection, only_if_exists, name_len, name);
    }   break;
    case protocol::requests::opcodes::GETATOMNAME:              {  //  17
        const xcb_atom_t        atom {};
        xcb_get_atom_name(
            connection, atom);
    }   break;
    case protocol::requests::opcodes::CHANGEPROPERTY:           {  //  18
        const uint8_t           mode {};
        const xcb_window_t      window {};
        const xcb_atom_t        property {};
        const xcb_atom_t        type {};
        const uint8_t           format {};
        const uint32_t          data_len {};
        const void*             data {};
        xcb_change_property(
            connection, mode, window, property,
            type, format, data_len, data);
    }   break;
    case protocol::requests::opcodes::DELETEPROPERTY:           {  //  19
        const xcb_window_t      window {};
        const xcb_atom_t        property {};
        xcb_delete_property_checked(
            connection, window, property);
    }   break;
    case protocol::requests::opcodes::GETPROPERTY:              {  //  20
        const uint8_t           _delete {};
        const xcb_window_t      window {};
        const xcb_atom_t        property {};
        const xcb_atom_t        type {};
        const uint32_t          long_offset {};
        const uint32_t          long_length {};
        xcb_get_property(
            connection, _delete, window, property,
            type, long_offset, long_length);
    }   break;
    case protocol::requests::opcodes::LISTPROPERTIES:           {  //  21
        const xcb_window_t      window {};
        xcb_list_properties(
            connection, window);
    }   break;
    case protocol::requests::opcodes::SETSELECTIONOWNER:        {  //  22
        const xcb_window_t      owner {};
        const xcb_atom_t        selection {};
        const xcb_timestamp_t   time {};
        xcb_set_selection_owner(
            connection, owner, selection, time);
    }   break;
    case protocol::requests::opcodes::GETSELECTIONOWNER:        {  //  23
        const xcb_atom_t        selection {};
        xcb_get_selection_owner(
            connection, selection);
    }   break;
    case protocol::requests::opcodes::CONVERTSELECTION:         {  //  24
        const xcb_window_t      requestor {};
        const xcb_atom_t        selection {};
        const xcb_atom_t        target {};
        const xcb_atom_t        property {};
        const xcb_timestamp_t   time {};
        xcb_convert_selection(
            connection, requestor, selection, target, property, time);
    }   break;
    case protocol::requests::opcodes::SENDEVENT:                {  //  25
        const uint8_t           propagate {};
        const xcb_window_t      destination {};
        const uint32_t          event_mask {};
        const char*             event {};
        xcb_send_event(
            connection, propagate, destination, event_mask, event);
    }   break;
    case protocol::requests::opcodes::GRABPOINTER:              {  //  26
        const uint8_t           owner_events {};
        const xcb_window_t      grab_window {};
        const uint16_t          event_mask {};
        const uint8_t           pointer_mode {};
        const uint8_t           keyboard_mode {};
        const xcb_window_t      confine_to {};
        const xcb_cursor_t      cursor {};
        const xcb_timestamp_t   time {};
        xcb_grab_pointer(
            connection, owner_events, grab_window, event_mask,
            pointer_mode, keyboard_mode, confine_to, cursor, time);
    }   break;
    case protocol::requests::opcodes::UNGRABPOINTER:            {  //  27
        const xcb_timestamp_t   time {};
        xcb_ungrab_pointer(
            connection, time);
    }   break;
    case protocol::requests::opcodes::GRABBUTTON:               {  //  28
        const uint8_t           owner_events {};
        const xcb_window_t      grab_window {};
        const uint16_t          event_mask {};
        const uint8_t           pointer_mode {};
        const uint8_t           keyboard_mode {};
        const xcb_window_t      confine_to {};
        const xcb_cursor_t      cursor {};
        const uint8_t           button {};
        const uint16_t          modifiers {};
        xcb_grab_button(
            connection, owner_events, grab_window, event_mask,
            pointer_mode, keyboard_mode, confine_to, cursor, button, modifiers);
    }   break;
    case protocol::requests::opcodes::UNGRABBUTTON:             {  //  29
        const uint8_t           button {};
        const xcb_window_t      grab_window {};
        const uint16_t          modifiers {};
        xcb_ungrab_button(
            connection, button, grab_window, modifiers);
    }   break;
    case protocol::requests::opcodes::CHANGEACTIVEPOINTERGRAB:  {  //  30
        const xcb_cursor_t      cursor {};
        const xcb_timestamp_t   time {};
        const uint16_t          event_mask {};
        xcb_change_active_pointer_grab(
            connection, cursor, time, event_mask);
    }   break;
    case protocol::requests::opcodes::GRABKEYBOARD:             {  //  31
        const uint8_t           owner_events {};
        const xcb_window_t      grab_window {};
        const xcb_timestamp_t   time {};
        const uint8_t           pointer_mode {};
        const uint8_t           keyboard_mode {};
        xcb_grab_keyboard(
            connection, owner_events, grab_window, time,
            pointer_mode, keyboard_mode);
    }   break;
    case protocol::requests::opcodes::UNGRABKEYBOARD:           {  //  32
        const xcb_timestamp_t   time {};
        xcb_ungrab_keyboard(
            connection, time);
    }   break;
    case protocol::requests::opcodes::GRABKEY:                  {  //  33
        const uint8_t           owner_events {};
        const xcb_window_t      grab_window {};
        const uint16_t          modifiers {};
        const xcb_keycode_t     key {};
        const uint8_t           pointer_mode {};
        const uint8_t           keyboard_mode {};
        xcb_grab_key(
            connection, owner_events, grab_window,
            modifiers, key, pointer_mode, keyboard_mode);
    }   break;
    case protocol::requests::opcodes::UNGRABKEY:                {  //  34
        const xcb_keycode_t     key {};
        const xcb_window_t      grab_window {};
        const uint16_t          modifiers {};
        xcb_ungrab_key(
            connection, key, grab_window, modifiers);
    }   break;
    case protocol::requests::opcodes::ALLOWEVENTS:              {  //  35
        const uint8_t           mode {};
        const xcb_timestamp_t   time {};
        xcb_allow_events(
            connection, mode, time);
    }   break;
    case protocol::requests::opcodes::GRABSERVER:               {  //  36
        xcb_grab_server(
            connection);
    }   break;
    case protocol::requests::opcodes::UNGRABSERVER:             {  //  37
        xcb_grab_server(
            connection);
    }   break;
    case protocol::requests::opcodes::QUERYPOINTER:             {  //  38
        const xcb_window_t      window {};
        xcb_query_pointer(
            connection, window);
    }   break;
    case protocol::requests::opcodes::GETMOTIONEVENTS:          {  //  39
        const xcb_window_t      window {};
        const xcb_timestamp_t   start {};
        const xcb_timestamp_t   stop {};
        xcb_get_motion_events(
            connection, window, start, stop);
    }   break;
    case protocol::requests::opcodes::TRANSLATECOORDINATES:     {  //  40
        const xcb_window_t      src_window {};
        const xcb_window_t      dst_window {};
        const int16_t           src_x {};
        const int16_t           src_y {};
        xcb_translate_coordinates(
            connection, src_window, dst_window, src_x, src_y);
    }   break;
    case protocol::requests::opcodes::WARPPOINTER:              {  //  41
        const xcb_window_t      src_window {};
        const xcb_window_t      dst_window {};
        const int16_t           src_x {};
        const int16_t           src_y {};
        const uint16_t          src_width {};
        const uint16_t          src_height {};
        const int16_t           dst_x {};
        const int16_t           dst_y {};
        xcb_warp_pointer(
            connection, src_window, dst_window,
            src_x, src_y, src_width, src_height, dst_x, dst_y);
    }   break;
    case protocol::requests::opcodes::SETINPUTFOCUS:            {  //  42
        const uint8_t           revert_to {};
        const xcb_window_t      focus {};
        const xcb_timestamp_t   time {};
        xcb_set_input_focus(
            connection, revert_to, focus, time);
    }   break;
    case protocol::requests::opcodes::GETINPUTFOCUS:            {  //  43
        xcb_get_input_focus(
            connection);
    }   break;
    case protocol::requests::opcodes::QUERYKEYMAP:              {  //  44
        xcb_query_keymap (
            connection);
    }   break;
    case protocol::requests::opcodes::OPENFONT:                 {  //  45
        const xcb_font_t        fid {};
        const uint16_t          name_len {};
        const char*             name {};
        xcb_open_font(
            connection, fid, name_len, name);
    }   break;
    case protocol::requests::opcodes::CLOSEFONT:                {  //  46
        const xcb_font_t        font {};
        xcb_close_font(
            connection, font);
    }   break;
    case protocol::requests::opcodes::QUERYFONT:                {  //  47
        const xcb_fontable_t    font {};
        xcb_query_font(
            connection, font);
    }   break;
    case protocol::requests::opcodes::QUERYTEXTEXTENTS:         {  //  48
        const xcb_fontable_t    font {};
        const uint32_t          string_len {};
        const xcb_char2b_t*     string {};
        xcb_query_text_extents(
            connection, font, string_len, string);
    }   break;
    case protocol::requests::opcodes::LISTFONTS:                {  //  49
        const uint16_t          max_names {};
        const uint16_t          pattern_len {};
        const char*             pattern {};
        xcb_list_fonts(
            connection, max_names, pattern_len, pattern);
    }   break;
    case protocol::requests::opcodes::LISTFONTSWITHINFO:        {  //  50
        const uint16_t          max_names {};
        const uint16_t          pattern_len {};
        const char*             pattern {};
        xcb_list_fonts_with_info(
            connection, max_names, pattern_len, pattern);
    }   break;
    case protocol::requests::opcodes::SETFONTPATH:              {  //  51
        const uint16_t          font_qty {};
        const xcb_str_t*        font {};
        xcb_set_font_path(
            connection, font_qty, font);
    }   break;
    case protocol::requests::opcodes::GETFONTPATH:              {  //  52
        xcb_get_font_path(
            connection);
    }   break;
    case protocol::requests::opcodes::CREATEPIXMAP:             {  //  53
        const uint8_t           depth {};
        const xcb_pixmap_t      pid {};
        const xcb_drawable_t    drawable {};
        const uint16_t          width {};
        const uint16_t          height {};
        xcb_create_pixmap(
            connection, depth, pid, drawable, width, height);
    }   break;
    case protocol::requests::opcodes::FREEPIXMAP:               {  //  54
        const xcb_pixmap_t      pixmap {};
        xcb_free_pixmap(
            connection, pixmap);
    }   break;
    case protocol::requests::opcodes::CREATEGC:                 {  //  55
        const xcb_gcontext_t    cid {};
        const xcb_drawable_t    drawable {};
        const uint32_t          value_mask {};
        const void*             value_list {};
        xcb_create_gc(
            connection, cid, drawable, value_mask, value_list);
    }   break;
    case protocol::requests::opcodes::CHANGEGC:                 {  //  56
        const xcb_gcontext_t    gc {};
        const uint32_t          value_mask {};
        const void*             value_list {};
        xcb_change_gc(
            connection, gc, value_mask, value_list);
    }   break;
    case protocol::requests::opcodes::COPYGC:                   {  //  57
        const xcb_gcontext_t    src_gc {};
        const xcb_gcontext_t    dst_gc {};
        const uint32_t          value_mask {};
        xcb_copy_gc(
            connection, src_gc, dst_gc, value_mask);
    }   break;
    case protocol::requests::opcodes::SETDASHES:                {  //  58
        xcb_gcontext_t          gc {};
        const uint16_t          dash_offset {};
        const uint16_t          dashes_len {};
        const uint8_t*          dashes {};
        xcb_set_dashes(
            connection, gc, dash_offset, dashes_len, dashes);
    }   break;
    case protocol::requests::opcodes::SETCLIPRECTANGLES:        {  //  59
        const uint8_t           ordering {};
        const xcb_gcontext_t    gc {};
        const int16_t           clip_x_origin {};
        const int16_t           clip_y_origin {};
        const uint32_t          rectangles_len {};
        const xcb_rectangle_t*  rectangles {};
        xcb_set_clip_rectangles(
            connection, ordering, gc, clip_x_origin, clip_y_origin,
            rectangles_len, rectangles);
    }   break;
    case protocol::requests::opcodes::FREEGC:                   {  //  60
        const xcb_gcontext_t    gc {};
        xcb_free_gc(
            connection, gc);
    }   break;
    case protocol::requests::opcodes::CLEARAREA:                {  //  61
        const uint8_t           exposures {};
        const xcb_window_t      window {};
        const int16_t           x {};
        const int16_t           y {};
        const uint16_t          width {};
        const uint16_t          height {};
        xcb_clear_area(
            connection, exposures, window, x, y, width, height);
    }   break;
    case protocol::requests::opcodes::COPYAREA:                 {  //  62
        const xcb_drawable_t    src_drawable {};
        const xcb_drawable_t    dst_drawable {};
        const xcb_gcontext_t    gc {};
        const int16_t           src_x {};
        const int16_t           src_y {};
        const int16_t           dst_x {};
        const int16_t           dst_y {};
        const uint16_t          width {};
        const uint16_t          height {};
        xcb_copy_area(
            connection, src_drawable, dst_drawable, gc,
            src_x, src_y, dst_x, dst_y,
            width, height);
    }   break;
    case protocol::requests::opcodes::COPYPLANE:                {  //  63
        const xcb_drawable_t    src_drawable {};
        const xcb_drawable_t    dst_drawable {};
        const xcb_gcontext_t    gc {};
        const int16_t           src_x {};
        const int16_t           src_y {};
        const int16_t           dst_x {};
        const int16_t           dst_y {};
        const uint16_t          width {};
        const uint16_t          height {};
        const uint32_t          bit_plane {};
        xcb_copy_plane(
            connection, src_drawable, dst_drawable, gc,
            src_x, src_y, dst_x, dst_y,
            width, height, bit_plane);
    }
        break;
    case protocol::requests::opcodes::POLYPOINT:                {  //  64
        const uint8_t           coordinate_mode {};
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const uint32_t          points_len {};
        const xcb_point_t*      points {};
        xcb_poly_point(
            connection, coordinate_mode, drawable, gc, points_len, points);
    }   break;
    case protocol::requests::opcodes::POLYLINE:                 {  //  65
        const uint8_t           coordinate_mode {};
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const uint32_t          points_len {};
        const xcb_point_t*      points {};
        xcb_poly_line(
            connection, coordinate_mode, drawable, gc, points_len, points);
    }   break;
    case protocol::requests::opcodes::POLYSEGMENT:              {  //  66
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const uint32_t          segments_len {};
        const xcb_segment_t*    segments {};
        xcb_poly_segment(
            connection, drawable, gc, segments_len, segments);
    }   break;
    case protocol::requests::opcodes::POLYRECTANGLE:            {  //  67
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const uint32_t          rectangles_len {};
        const xcb_rectangle_t*  rectangles {};
        xcb_poly_rectangle(
            connection, drawable, gc, rectangles_len, rectangles);
    }   break;
    case protocol::requests::opcodes::POLYARC:                  {  //  68
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const uint32_t          arcs_len {};
        const xcb_arc_t*        arcs {};
        xcb_poly_arc(
            connection, drawable, gc, arcs_len, arcs);
    }   break;
    case protocol::requests::opcodes::FILLPOLY:                 {  //  69
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const uint8_t           shape {};
        const uint8_t           coordinate_mode {};
        const uint32_t          points_len {};
        const xcb_point_t*      points {};
        xcb_fill_poly(
            connection, drawable, gc, shape, coordinate_mode, points_len, points);
    }   break;
    case protocol::requests::opcodes::POLYFILLRECTANGLE:        {  //  70
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const uint32_t          rectangles_len {};
        const xcb_rectangle_t*  rectangles {};
        xcb_poly_fill_rectangle(
            connection, drawable, gc, rectangles_len, rectangles);
    }   break;
    case protocol::requests::opcodes::POLYFILLARC:              {  //  71
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const uint32_t          arcs_len {};
        const xcb_arc_t*        arcs {};
        xcb_poly_fill_arc(
            connection, drawable, gc, arcs_len, arcs);
    }   break;
    case protocol::requests::opcodes::PUTIMAGE:                 {  //  72
        const uint8_t           format {};
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const uint16_t          width {};
        const uint16_t          height {};
        const int16_t           dst_x {};
        const int16_t           dst_y {};
        const uint8_t           left_pad {};
        const uint8_t           depth {};
        const uint32_t          data_len {};
        const uint8_t*          data {};
        xcb_put_image(
            connection, format, drawable, gc, width, height,
            dst_x, dst_y, left_pad, depth, data_len, data);
    }   break;
    case protocol::requests::opcodes::GETIMAGE:                 {  //  73
        const uint8_t           format {};
        const xcb_drawable_t    drawable {};
        const int16_t           x {};
        const int16_t           y {};
        const uint16_t          width {};
        const uint16_t          height {};
        const uint32_t          plane_mask {};
        xcb_get_image(
            connection, format, drawable, x, y, width, height, plane_mask);
    }   break;
    case protocol::requests::opcodes::POLYTEXT8:                {  //  74
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const int16_t           x {};
        const int16_t           y {};
        const uint32_t          items_len {};
        const uint8_t*          items {};
        xcb_poly_text_8(
            connection, drawable, gc, x, y, items_len, items);
    }
        break;
    case protocol::requests::opcodes::POLYTEXT16:               {  //  75
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const int16_t           x {};
        const int16_t           y {};
        const uint32_t          items_len {};
        const uint8_t*          items {};
        xcb_poly_text_16(
            connection, drawable, gc, x, y, items_len, items);
    }   break;
    case protocol::requests::opcodes::IMAGETEXT8:               {  //  76
        const uint8_t           string_len {};
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const int16_t           x {};
        const int16_t           y {};
        const char*             string {};
        xcb_image_text_8(
            connection, string_len, drawable, gc, x, y, string);
    }   break;
    case protocol::requests::opcodes::IMAGETEXT16:              {  //  77
        const uint8_t           string_len {};
        const xcb_drawable_t    drawable {};
        const xcb_gcontext_t    gc {};
        const int16_t           x {};
        const int16_t           y {};
        const xcb_char2b_t*     string {};
        xcb_image_text_16(
            connection, string_len, drawable, gc, x, y, string);
    }   break;
    case protocol::requests::opcodes::CREATECOLORMAP:           {  //  78
        const uint8_t           alloc {};
        const xcb_colormap_t    mid {};
        const xcb_window_t      window {};
        const xcb_visualid_t    visual {};
        xcb_create_colormap(
            connection, alloc, mid, window, visual);
    }   break;
    case protocol::requests::opcodes::FREECOLORMAP:             {  //  79
        const xcb_colormap_t    cmap {};
        xcb_free_colormap(
            connection, cmap);
    }   break;
    case protocol::requests::opcodes::COPYCOLORMAPANDFREE:      {  //  80
        const xcb_colormap_t    mid {};
        const xcb_colormap_t    src_cmap {};
        xcb_copy_colormap_and_free(
            connection, mid, src_cmap);
    }   break;
    case protocol::requests::opcodes::INSTALLCOLORMAP:          {  //  81
        const xcb_colormap_t    cmap {};
        xcb_install_colormap (
            connection, cmap);
    }   break;
    case protocol::requests::opcodes::UNINSTALLCOLORMAP:        {  //  82
        const xcb_colormap_t    cmap {};
        xcb_uninstall_colormap(
            connection, cmap);
    }   break;
    case protocol::requests::opcodes::LISTINSTALLEDCOLORMAPS:   {  //  83
        const xcb_window_t      window {};
        xcb_list_installed_colormaps(
            connection, window);
    }   break;
    case protocol::requests::opcodes::ALLOCCOLOR:               {  //  84
        const xcb_colormap_t    cmap {};
        const uint16_t          red {};
        const uint16_t          green {};
        const uint16_t          blue {};
        xcb_alloc_color(
            connection, cmap, red, green, blue);
    }   break;
    case protocol::requests::opcodes::ALLOCNAMEDCOLOR:          {  //  85
        const xcb_colormap_t    cmap {};
        const uint16_t          name_len {};
        const char*             name {};
        xcb_alloc_named_color(
            connection, cmap, name_len, name);
    }   break;
    case protocol::requests::opcodes::ALLOCCOLORCELLS:          {  //  86
        const uint8_t           contiguous {};
        const xcb_colormap_t    cmap {};
        const uint16_t          colors {};
        const uint16_t          planes {};
        xcb_alloc_color_cells(
            connection, contiguous, cmap, colors, planes);
    }   break;
    case protocol::requests::opcodes::ALLOCCOLORPLANES:         {  //  87
        const uint8_t           contiguous {};
        const xcb_colormap_t    cmap {};
        const uint16_t          colors {};
        const uint16_t          reds {};
        const uint16_t          greens {};
        const uint16_t          blues {};
        xcb_alloc_color_planes(
            connection, contiguous, cmap, colors, reds, greens, blues);
    }
        break;
    case protocol::requests::opcodes::FREECOLORS:               {  //  88
        const xcb_colormap_t    cmap {};
        const uint32_t          plane_mask {};
        const uint32_t          pixels_len {};
        const uint32_t*         pixels {};
        xcb_free_colors(
            connection, cmap, plane_mask, pixels_len, pixels);
    }   break;
    case protocol::requests::opcodes::STORECOLORS:              {  //  89
        const xcb_colormap_t    cmap {};
        const uint32_t          items_len {};
        const xcb_coloritem_t*  items {};
        xcb_store_colors(
            connection, cmap, items_len, items);
    }   break;
    case protocol::requests::opcodes::STORENAMEDCOLOR:          {  //  90
        const uint8_t           flags {};
        const xcb_colormap_t    cmap {};
        const uint32_t          pixel {};
        const uint16_t          name_len {};
        const char*             name {};
        xcb_store_named_color(
            connection, flags, cmap, pixel, name_len, name);
    }   break;
    case protocol::requests::opcodes::QUERYCOLORS:              {  //  91
        const xcb_colormap_t    cmap {};
        const uint32_t          pixels_len {};
        const uint32_t*         pixels {};
        xcb_query_colors(
            connection, cmap, pixels_len, pixels);
    }   break;
    case protocol::requests::opcodes::LOOKUPCOLOR:              {  //  92
        const xcb_colormap_t    cmap {};
        const uint16_t          name_len {};
        const char*             name {};
        xcb_lookup_color(
            connection, cmap, name_len, name);
    }   break;
    case protocol::requests::opcodes::CREATECURSOR:             {  //  93
        const xcb_cursor_t      cid {};
        const xcb_pixmap_t      source {};
        const xcb_pixmap_t      mask {};
        const uint16_t          source_char {};
        const uint16_t          mask_char {};
        const uint16_t          fore_red {};
        const uint16_t          fore_green {};
        const uint16_t          fore_blue {};
        const uint16_t          back_red {};
        const uint16_t          back_green {};
        const uint16_t          back_blue {};
        const uint16_t          x {};
        const uint16_t          y {};
        xcb_create_cursor(
            connection, cid, source, mask,
            fore_red, fore_green, fore_blue,
            back_red, back_green, back_blue, x, y);
    }   break;
    case protocol::requests::opcodes::CREATEGLYPHCURSOR:        {  //  94
        const xcb_cursor_t      cid {};
        const xcb_font_t        source_font {};
        const xcb_font_t        mask_font {};
        const uint16_t          source_char {};
        const uint16_t          mask_char {};
        const uint16_t          fore_red {};
        const uint16_t          fore_green {};
        const uint16_t          fore_blue {};
        const uint16_t          back_red {};
        const uint16_t          back_green {};
        const uint16_t          back_blue {};
        xcb_create_glyph_cursor(
            connection, cid, source_font, mask_font, source_char, mask_char,
            fore_red, fore_green, fore_blue, back_red, back_green, back_blue);
    }   break;
    case protocol::requests::opcodes::FREECURSOR:               {  //  95
        const xcb_cursor_t      cursor {};
        xcb_free_cursor(
            connection, cursor);
    }   break;
    case protocol::requests::opcodes::RECOLORCURSOR:            {  //  96
        const xcb_cursor_t      cursor {};
        const uint16_t          fore_red {};
        const uint16_t          fore_green {};
        const uint16_t          fore_blue {};
        const uint16_t          back_red {};
        const uint16_t          back_green {};
        const uint16_t          back_blue {};
        xcb_recolor_cursor(
            connection, cursor, fore_red, fore_green, fore_blue,
            back_red, back_green, back_blue);
    }   break;
    case protocol::requests::opcodes::QUERYBESTSIZE:            {  //  97
        const uint8_t           _class {};
        const xcb_drawable_t    drawable {};
        const uint16_t          width {};
        const uint16_t          height {};
        xcb_query_best_size(
            connection, _class, drawable, width, height);
    }   break;
    case protocol::requests::opcodes::QUERYEXTENSION:           {  //  98
        const uint16_t          name_len {};
        const char*             name {};
        xcb_query_extension(
            connection, name_len, name);
    }   break;
    case protocol::requests::opcodes::LISTEXTENSIONS:           {  //  99
        xcb_list_extensions(
            connection);
    }   break;
    case protocol::requests::opcodes::CHANGEKEYBOARDMAPPING:    {  // 100
        const uint8_t           keycode_count {};
        const xcb_keycode_t     first_keycode {};
        const uint8_t           keysyms_per_keycode {};
        const xcb_keysym_t*     keysyms {};
        xcb_change_keyboard_mapping (
            connection, keycode_count, first_keycode,
            keysyms_per_keycode, keysyms);

    }   break;
    case protocol::requests::opcodes::GETKEYBOARDMAPPING:       {  // 101
        const xcb_keycode_t     first_keycode {};
        const uint8_t           count {};
        xcb_get_keyboard_mapping (
            connection, first_keycode, count);
    }   break;
    case protocol::requests::opcodes::CHANGEKEYBOARDCONTROL:    {  // 102
        const uint32_t          value_mask {};
        const void*             value_list {};
        xcb_change_keyboard_control(
            connection, value_mask, value_list);
    }   break;
    case protocol::requests::opcodes::GETKEYBOARDCONTROL:       {  // 103
        xcb_get_keyboard_control(
            connection);
    }   break;
    case protocol::requests::opcodes::BELL:                     {  // 104
        const int8_t            percent {};
        xcb_bell(
            connection, percent);
    }   break;
    case protocol::requests::opcodes::CHANGEPOINTERCONTROL:     {  // 105
        const int16_t           acceleration_numerator {};
        const int16_t           acceleration_denominator {};
        const int16_t           threshold {};
        const uint8_t           do_acceleration {};
        const uint8_t           do_threshold {};
        xcb_change_pointer_control(
            connection, acceleration_numerator, acceleration_denominator,
            threshold, do_acceleration, do_threshold);
    }   break;
    case protocol::requests::opcodes::GETPOINTERCONTROL:        {  // 106
        xcb_get_pointer_control(connection);
    }   break;
    case protocol::requests::opcodes::SETSCREENSAVER:           {  // 107
        const int16_t           timeout {};
        const int16_t           interval {};
        const uint8_t           prefer_blanking {};
        const uint8_t           allow_exposures {};
        xcb_set_screen_saver(
            connection, timeout, interval, prefer_blanking, allow_exposures);
    }   break;
    case protocol::requests::opcodes::GETSCREENSAVER:           {  // 108
        xcb_get_screen_saver(
            connection);
    }   break;
    case protocol::requests::opcodes::CHANGEHOSTS:              {  // 109
        const uint8_t           mode {};
        const uint8_t           family {};
        const uint16_t          address_len {};
        const uint8_t*          address {};
        xcb_change_hosts(
            connection, mode, family, address_len, address);
    }   break;
    case protocol::requests::opcodes::LISTHOSTS:                {  // 110
        xcb_list_hosts(
            connection);
    }   break;
    case protocol::requests::opcodes::SETACCESSCONTROL:         {  // 111
        const uint8_t           mode {};
        xcb_set_access_control(
            connection, mode);
    }   break;
    case protocol::requests::opcodes::SETCLOSEDOWNMODE:         {  // 112
        const uint8_t           mode {};
        xcb_set_close_down_mode(
            connection, mode);
    }   break;
    case protocol::requests::opcodes::KILLCLIENT:               {  // 113
        const uint32_t          resource {};
        xcb_kill_client (
            connection, resource);
    }   break;
    case protocol::requests::opcodes::ROTATEPROPERTIES:         {  // 114
        const xcb_window_t      window {};
        const uint16_t          atoms_len {};
        const int16_t           delta {};
        const xcb_atom_t*       atoms {};
        xcb_rotate_properties(
            connection, window, atoms_len, delta, atoms);
    }   break;
    case protocol::requests::opcodes::FORCESCREENSAVER:         {  // 115
        const uint8_t           mode {};
        xcb_force_screen_saver(
            connection, mode);

    }   break;
    case protocol::requests::opcodes::SETPOINTERMAPPING:        {  // 116
        const uint8_t           map_len {};
        const uint8_t*          map     {};
        xcb_set_pointer_mapping(
            connection, map_len, map);
    }   break;
    case protocol::requests::opcodes::GETPOINTERMAPPING:        {  // 117
        xcb_get_pointer_mapping(
            connection);
    }   break;
    case protocol::requests::opcodes::SETMODIFIERMAPPING:       {  // 118
        const uint8_t           keycodes_per_modifier {};
        const xcb_keycode_t*    keycodes {};
        xcb_set_modifier_mapping(
            connection, keycodes_per_modifier, keycodes);
    }   break;
    case protocol::requests::opcodes::GETMODIFIERMAPPING:       {  // 119
        xcb_get_modifier_mapping(
            connection);
    }   break;
    case protocol::requests::opcodes::NOOPERATION:              {  // 127
        xcb_no_operation(
            connection);
    }   break;
    default:
        break;
    }

    // Make sure commands are sent before we pause so that the window gets shown
    xcb_flush( connection );

    xcb_disconnect( connection );
}
