// https://xcb.freedesktop.org/tutorial/basicwindowsanddrawing/

#include <string>  // stoul

#include <cassert>
#include <cstdint>

#include <xcb/xcb.h>
#include <protocol/requests.hpp>
#include <protocol/events.hpp>

int main(const int argc, const char* const* argv) {
    assert( argc > 1 );
    const uint64_t code { std::stoul( argv[1] ) };
    assert( code >= protocol::events::codes::MIN &&
            code <= protocol::events::codes::MAX );
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

    // TBD adapted from example at https://www.x.org/releases/X11R7.7/doc/man/man3/xcb_send_event.3.xhtml
    //   (minimum parameter population for xcb to send request)
    const uint8_t        propagate {};
    const xcb_window_t   destination {};
    const uint32_t       event_mask { XCB_EVENT_MASK_STRUCTURE_NOTIFY };
    const char*          event_p {};

    using namespace protocol::events::codes;
    switch ( code ) {
    case KEYPRESS:         {  //  2
        // typedef struct xcb_key_press_event_t {
        //     uint8_t         response_type;
        //     xcb_keycode_t   detail;
        //     uint16_t        sequence;
        //     xcb_timestamp_t time;
        //     xcb_window_t    root;
        //     xcb_window_t    event;
        //     xcb_window_t    child;
        //     int16_t         root_x;
        //     int16_t         root_y;
        //     int16_t         event_x;
        //     int16_t         event_y;
        //     uint16_t        state;
        //     uint8_t         same_screen;
        //     uint8_t         pad0;
        // } xcb_key_press_event_t;
        xcb_key_press_event_t event {};
        event.response_type = XCB_KEY_PRESS;
        event_p = (const char*)&event;
    }   break;
    case KEYRELEASE:       {  //  3
        // typedef xcb_key_press_event_t xcb_key_release_event_t;
        xcb_key_release_event_t event {};
        event.response_type = XCB_KEY_RELEASE;
        event_p = (const char*)&event;
    }   break;
    case BUTTONPRESS:      {  //  4
        // typedef struct xcb_button_press_event_t {
        //     uint8_t         response_type;
        //     xcb_button_t    detail;
        //     uint16_t        sequence;
        //     xcb_timestamp_t time;
        //     xcb_window_t    root;
        //     xcb_window_t    event;
        //     xcb_window_t    child;
        //     int16_t         root_x;
        //     int16_t         root_y;
        //     int16_t         event_x;
        //     int16_t         event_y;
        //     uint16_t        state;
        //     uint8_t         same_screen;
        //     uint8_t         pad0;
        // } xcb_button_press_event_t;
        xcb_button_press_event_t event {};
        event.response_type = XCB_BUTTON_PRESS;
        event_p = (const char*)&event;
    }   break;
    case BUTTONRELEASE:    {  //  5
        // typedef xcb_button_press_event_t xcb_button_release_event_t;
        xcb_button_release_event_t event {};
        event.response_type = XCB_BUTTON_RELEASE;
        event_p = (const char*)&event;
    }   break;
    case MOTIONNOTIFY:     {  //  6
        // typedef struct xcb_motion_notify_event_t {
        //     uint8_t         response_type;
        //     uint8_t         detail;
        //     uint16_t        sequence;
        //     xcb_timestamp_t time;
        //     xcb_window_t    root;
        //     xcb_window_t    event;
        //     xcb_window_t    child;
        //     int16_t         root_x;
        //     int16_t         root_y;
        //     int16_t         event_x;
        //     int16_t         event_y;
        //     uint16_t        state;
        //     uint8_t         same_screen;
        //     uint8_t         pad0;
        // } xcb_motion_notify_event_t;
        xcb_motion_notify_event_t event {};
        event.response_type = XCB_MOTION_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case ENTERNOTIFY:      {  //  7
        // typedef struct xcb_enter_notify_event_t {
        //     uint8_t         response_type;
        //     uint8_t         detail;
        //     uint16_t        sequence;
        //     xcb_timestamp_t time;
        //     xcb_window_t    root;
        //     xcb_window_t    event;
        //     xcb_window_t    child;
        //     int16_t         root_x;
        //     int16_t         root_y;
        //     int16_t         event_x;
        //     int16_t         event_y;
        //     uint16_t        state;
        //     uint8_t         mode;
        //     uint8_t         same_screen_focus;
        // } xcb_enter_notify_event_t;
        xcb_enter_notify_event_t event {};
        event.response_type = XCB_ENTER_NOTIFY;
        // TBD can't find XCB constants for same_screen_focus flags
        event.same_screen_focus = 0x01 | 0x02;
        event_p = (const char*)&event;
    }   break;
    case LEAVENOTIFY:      {  //  8
        // typedef xcb_enter_notify_event_t xcb_leave_notify_event_t;
        xcb_leave_notify_event_t event {};
        event.response_type = XCB_LEAVE_NOTIFY;
        // TBD can't find XCB constants for same_screen_focus flags
        event.same_screen_focus = 0x01 | 0x02;
        event_p = (const char*)&event;
    }   break;
    case FOCUSIN:          {  //  9
        // typedef struct xcb_focus_in_event_t {
        //     uint8_t      response_type;
        //     uint8_t      detail;
        //     uint16_t     sequence;
        //     xcb_window_t event;
        //     uint8_t      mode;
        //     uint8_t      pad0[3];
        // } xcb_focus_in_event_t;
        xcb_focus_in_event_t event {};
        event.response_type = XCB_FOCUS_IN;
        event_p = (const char*)&event;
    }   break;
    case FOCUSOUT:         {  // 10
        // typedef xcb_focus_in_event_t xcb_focus_out_event_t;
        xcb_focus_out_event_t event {};
        event.response_type = XCB_FOCUS_OUT;
        event_p = (const char*)&event;
    }   break;
    case KEYMAPNOTIFY:     {  // 11
        // typedef struct xcb_keymap_notify_event_t {
        //     uint8_t response_type;
        //     uint8_t keys[31];
        // } xcb_keymap_notify_event_t;
        xcb_keymap_notify_event_t event {};
        event.response_type = XCB_KEYMAP_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case EXPOSE:           {  // 12
        // typedef struct xcb_expose_event_t {
        //     uint8_t      response_type;
        //     uint8_t      pad0;
        //     uint16_t     sequence;
        //     xcb_window_t window;
        //     uint16_t     x;
        //     uint16_t     y;
        //     uint16_t     width;
        //     uint16_t     height;
        //     uint16_t     count;
        //     uint8_t      pad1[2];
        // } xcb_expose_event_t;
        xcb_expose_event_t event {};
        event.response_type = XCB_EXPOSE;
        event_p = (const char*)&event;
    }   break;
    case GRAPHICSEXPOSURE: {  // 13
        // typedef struct xcb_graphics_exposure_event_t {
        //     uint8_t        response_type;
        //     uint8_t        pad0;
        //     uint16_t       sequence;
        //     xcb_drawable_t drawable;
        //     uint16_t       x;
        //     uint16_t       y;
        //     uint16_t       width;
        //     uint16_t       height;
        //     uint16_t       minor_opcode;
        //     uint16_t       count;
        //     uint8_t        major_opcode;
        //     uint8_t        pad1[3];
        // } xcb_graphics_exposure_event_t;
        xcb_graphics_exposure_event_t event {};
        event.response_type = XCB_GRAPHICS_EXPOSURE;
        event_p = (const char*)&event;
    }   break;
    case NOEXPOSURE:       {  // 14
        // typedef struct xcb_no_exposure_event_t {
        //     uint8_t        response_type;
        //     uint8_t        pad0;
        //     uint16_t       sequence;
        //     xcb_drawable_t drawable;
        //     uint16_t       minor_opcode;
        //     uint8_t        major_opcode;
        //     uint8_t        pad1;
        // } xcb_no_exposure_event_t;
        xcb_no_exposure_event_t event {};
        event.response_type = XCB_NO_EXPOSURE;
        event_p = (const char*)&event;
    }   break;
    case VISIBILITYNOTIFY: {  // 15
        // typedef struct xcb_visibility_notify_event_t {
        //     uint8_t      response_type;
        //     uint8_t      pad0;
        //     uint16_t     sequence;
        //     xcb_window_t window;
        //     uint8_t      state;
        //     uint8_t      pad1[3];
        // } xcb_visibility_notify_event_t;
        xcb_visibility_notify_event_t event {};
        event.response_type = XCB_VISIBILITY_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case CREATENOTIFY:     {  // 16
        // typedef struct xcb_create_notify_event_t {
        //     uint8_t      response_type;
        //     uint8_t      pad0;
        //     uint16_t     sequence;
        //     xcb_window_t parent;
        //     xcb_window_t window;
        //     int16_t      x;
        //     int16_t      y;
        //     uint16_t     width;
        //     uint16_t     height;
        //     uint16_t     border_width;
        //     uint8_t      override_redirect;
        //     uint8_t      pad1;
        // } xcb_create_notify_event_t;
        xcb_create_notify_event_t event {};
        event.response_type = XCB_CREATE_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case DESTROYNOTIFY:    {  // 17
        // typedef struct xcb_destroy_notify_event_t {
        //     uint8_t      response_type;
        //     uint8_t      pad0;
        //     uint16_t     sequence;
        //     xcb_window_t event;
        //     xcb_window_t window;
        // } xcb_destroy_notify_event_t;
        xcb_destroy_notify_event_t event {};
        event.response_type = XCB_DESTROY_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case UNMAPNOTIFY:      {  // 18
        // typedef struct xcb_unmap_notify_event_t {
        //     uint8_t      response_type;
        //     uint8_t      pad0;
        //     uint16_t     sequence;
        //     xcb_window_t event;
        //     xcb_window_t window;
        //     uint8_t      from_configure;
        //     uint8_t      pad1[3];
        // } xcb_unmap_notify_event_t;
        xcb_unmap_notify_event_t event {};
        event.response_type = XCB_UNMAP_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case MAPNOTIFY:        {  // 19
        // typedef struct xcb_map_notify_event_t {
        //     uint8_t      response_type;
        //     uint8_t      pad0;
        //     uint16_t     sequence;
        //     xcb_window_t event;
        //     xcb_window_t window;
        //     uint8_t      override_redirect;
        //     uint8_t      pad1[3];
        // } xcb_map_notify_event_t;
        xcb_map_notify_event_t event {};
        event.response_type = XCB_MAP_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case MAPREQUEST:       {  // 20
        // typedef struct xcb_map_request_event_t {
        //     uint8_t      response_type;
        //     uint8_t      pad0;
        //     uint16_t     sequence;
        //     xcb_window_t parent;
        //     xcb_window_t window;
        // } xcb_map_request_event_t;
        xcb_map_request_event_t event {};
        event.response_type = XCB_MAP_REQUEST;
        event_p = (const char*)&event;
    }   break;
    case REPARENTNOTIFY:   {  // 21
        // typedef struct xcb_reparent_notify_event_t {
        //     uint8_t      response_type;
        //     uint8_t      pad0;
        //     uint16_t     sequence;
        //     xcb_window_t event;
        //     xcb_window_t window;
        //     xcb_window_t parent;
        //     int16_t      x;
        //     int16_t      y;
        //     uint8_t      override_redirect;
        //     uint8_t      pad1[3];
        // } xcb_reparent_notify_event_t;
        xcb_reparent_notify_event_t event {};
        event.response_type = XCB_REPARENT_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case CONFIGURENOTIFY:  {  // 22
        // typedef struct xcb_configure_notify_event_t {
        //     uint8_t      response_type;
        //     uint8_t      pad0;
        //     uint16_t     sequence;
        //     xcb_window_t event;
        //     xcb_window_t window;
        //     xcb_window_t above_sibling;
        //     int16_t      x;
        //     int16_t      y;
        //     uint16_t     width;
        //     uint16_t     height;
        //     uint16_t     border_width;
        //     uint8_t      override_redirect;
        //     uint8_t      pad1;
        // } xcb_configure_notify_event_t;
        xcb_configure_notify_event_t event {};
        event.response_type = XCB_CONFIGURE_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case CONFIGUREREQUEST: {  // 23
        // typedef struct xcb_configure_request_event_t {
        //     uint8_t      response_type;
        //     uint8_t      stack_mode;
        //     uint16_t     sequence;
        //     xcb_window_t parent;
        //     xcb_window_t window;
        //     xcb_window_t sibling;
        //     int16_t      x;
        //     int16_t      y;
        //     uint16_t     width;
        //     uint16_t     height;
        //     uint16_t     border_width;
        //     uint16_t     value_mask;
        // } xcb_configure_request_event_t;
        xcb_configure_request_event_t event {};
        event.response_type = XCB_CONFIGURE_REQUEST;
        event.value_mask =
            XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
            XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT |
            XCB_CONFIG_WINDOW_BORDER_WIDTH | XCB_CONFIG_WINDOW_SIBLING |
            XCB_CONFIG_WINDOW_STACK_MODE;
        event_p = (const char*)&event;
    }   break;
    case GRAVITYNOTIFY:    {  // 24
        // typedef struct xcb_gravity_notify_event_t {
        //     uint8_t      response_type;
        //     uint8_t      pad0;
        //     uint16_t     sequence;
        //     xcb_window_t event;
        //     xcb_window_t window;
        //     int16_t      x;
        //     int16_t      y;
        // } xcb_gravity_notify_event_t;
        xcb_gravity_notify_event_t event {};
        event.response_type = XCB_GRAVITY_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case RESIZEREQUEST:    {  // 25
        // typedef struct xcb_resize_request_event_t {
        //     uint8_t      response_type;
        //     uint8_t      pad0;
        //     uint16_t     sequence;
        //     xcb_window_t window;
        //     uint16_t     width;
        //     uint16_t     height;
        // } xcb_resize_request_event_t;
        xcb_resize_request_event_t event {};
        event.response_type = XCB_RESIZE_REQUEST;
        event_p = (const char*)&event;
    }   break;
    case CIRCULATENOTIFY:  {  // 26
        // typedef struct xcb_circulate_notify_event_t {
        //     uint8_t      response_type;
        //     uint8_t      pad0;
        //     uint16_t     sequence;
        //     xcb_window_t event;
        //     xcb_window_t window;
        //     uint8_t      pad1[4];
        //     uint8_t      place;
        //     uint8_t      pad2[3];
        // } xcb_circulate_notify_event_t;
        xcb_circulate_notify_event_t event {};
        event.response_type = XCB_CIRCULATE_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case CIRCULATEREQUEST: {  // 27
        // typedef xcb_circulate_notify_event_t xcb_circulate_request_event_t;
        xcb_circulate_request_event_t event {};
        event.response_type = XCB_CIRCULATE_REQUEST;
        event_p = (const char*)&event;
    }   break;
    case PROPERTYNOTIFY:   {  // 28
        // typedef struct xcb_property_notify_event_t {
        //     uint8_t         response_type;
        //     uint8_t         pad0;
        //     uint16_t        sequence;
        //     xcb_window_t    window;
        //     xcb_atom_t      atom;
        //     xcb_timestamp_t time;
        //     uint8_t         state;
        //     uint8_t         pad1[3];
        // } xcb_property_notify_event_t;
        xcb_property_notify_event_t event {};
        event.response_type = XCB_PROPERTY_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case SELECTIONCLEAR:   {  // 29
        // typedef struct xcb_selection_clear_event_t {
        //     uint8_t         response_type;
        //     uint8_t         pad0;
        //     uint16_t        sequence;
        //     xcb_timestamp_t time;
        //     xcb_window_t    owner;
        //     xcb_atom_t      selection;
        // } xcb_selection_clear_event_t;
        xcb_selection_clear_event_t event {};
        event.response_type = XCB_SELECTION_CLEAR;
        event_p = (const char*)&event;
    }   break;
    case SELECTIONREQUEST: {  // 30
        // typedef struct xcb_selection_request_event_t {
        //     uint8_t         response_type;
        //     uint8_t         pad0;
        //     uint16_t        sequence;
        //     xcb_timestamp_t time;
        //     xcb_window_t    owner;
        //     xcb_window_t    requestor;
        //     xcb_atom_t      selection;
        //     xcb_atom_t      target;
        //     xcb_atom_t      property;
        // } xcb_selection_request_event_t;
        xcb_selection_request_event_t event {};
        event.response_type = XCB_SELECTION_REQUEST;
        event_p = (const char*)&event;
    }   break;
    case SELECTIONNOTIFY:  {  // 31
        // typedef struct xcb_selection_notify_event_t {
        //     uint8_t         response_type;
        //     uint8_t         pad0;
        //     uint16_t        sequence;
        //     xcb_timestamp_t time;
        //     xcb_window_t    requestor;
        //     xcb_atom_t      selection;
        //     xcb_atom_t      target;
        //     xcb_atom_t      property;
        // } xcb_selection_notify_event_t;
        xcb_selection_notify_event_t event {};
        event.response_type = XCB_SELECTION_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case COLORMAPNOTIFY:   {  // 32
        // typedef struct xcb_colormap_notify_event_t {
        //     uint8_t        response_type;
        //     uint8_t        pad0;
        //     uint16_t       sequence;
        //     xcb_window_t   window;
        //     xcb_colormap_t colormap;
        //     uint8_t        _new;
        //     uint8_t        state;
        //     uint8_t        pad1[2];
        // } xcb_colormap_notify_event_t;
        xcb_colormap_notify_event_t event {};
        event.response_type = XCB_COLORMAP_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case CLIENTMESSAGE:    {  // 33
        // typedef struct xcb_client_message_event_t {
        //     uint8_t                   response_type;
        //     uint8_t                   format;
        //     uint16_t                  sequence;
        //     xcb_window_t              window;
        //     xcb_atom_t                type;
        //     xcb_client_message_data_t data;
        // } xcb_client_message_event_t;
        xcb_client_message_event_t event {};
        event.response_type = XCB_CLIENT_MESSAGE;
        event_p = (const char*)&event;
    }   break;
    case MAPPINGNOTIFY:    {  // 34
        // typedef struct xcb_mapping_notify_event_t {
        //     uint8_t       response_type;
        //     uint8_t       pad0;
        //     uint16_t      sequence;
        //     uint8_t       request;
        //     xcb_keycode_t first_keycode;
        //     uint8_t       count;
        //     uint8_t       pad1;
        // } xcb_mapping_notify_event_t;
        xcb_mapping_notify_event_t event {};
        event.response_type = XCB_MAPPING_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    default:
        break;
    }

    xcb_send_event(
        connection, propagate, destination, event_mask, event_p);

    // Make sure commands are sent before we pause so that the window gets shown
    xcb_flush( connection );

    xcb_disconnect( connection );
}
