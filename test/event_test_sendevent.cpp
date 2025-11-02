#include <string>                  // stoi

#include <cassert>
#include <cstdint>
#include <cstdlib>  // EXIT_FAILURE EXIT_SUCCESS

#include <fmt/format.h>

#include <xcb/xcb.h>

#include <protocol/requests.hpp>
#include <protocol/events.hpp>


int main( const int argc, const char* const* argv ) {
    assert( argc > 1 );
    const int code { std::stoi( argv[ 1 ] ) };
    using namespace protocol::events::codes;
    if ( !( code >= KEYPRESS &&          //  2
            code <= MAPPINGNOTIFY ) ) {  // 34
        fmt::println( stderr, "{}: invalid core event code: {} (expected 2-34)",
                      argv[0], code );
        return EXIT_FAILURE;
    }

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
    const uint8_t      propagate   {};
    const xcb_window_t destination {};
    const uint32_t     event_mask  { XCB_EVENT_MASK_STRUCTURE_NOTIFY };
    const char*        event_p     {};

    switch ( code ) {
    case KEYPRESS:         {  //  2
        xcb_key_press_event_t event {};
        event.response_type = XCB_KEY_PRESS;
        event_p = (const char*)&event;
    }   break;
    case KEYRELEASE:       {  //  3
        xcb_key_release_event_t event {};
        event.response_type = XCB_KEY_RELEASE;
        event_p = (const char*)&event;
    }   break;
    case BUTTONPRESS:      {  //  4
        xcb_button_press_event_t event {};
        event.response_type = XCB_BUTTON_PRESS;
        event_p = (const char*)&event;
    }   break;
    case BUTTONRELEASE:    {  //  5
        xcb_button_release_event_t event {};
        event.response_type = XCB_BUTTON_RELEASE;
        event_p = (const char*)&event;
    }   break;
    case MOTIONNOTIFY:     {  //  6
        xcb_motion_notify_event_t event {};
        event.response_type = XCB_MOTION_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case ENTERNOTIFY:      {  //  7
        xcb_enter_notify_event_t event {};
        event.response_type = XCB_ENTER_NOTIFY;
        // TBD can't find XCB constants for same_screen_focus flags
        event.same_screen_focus = 0x01 | 0x02;
        event_p = (const char*)&event;
    }   break;
    case LEAVENOTIFY:      {  //  8
        xcb_leave_notify_event_t event {};
        event.response_type = XCB_LEAVE_NOTIFY;
        // TBD can't find XCB constants for same_screen_focus flags
        event.same_screen_focus = 0x01 | 0x02;
        event_p = (const char*)&event;
    }   break;
    case FOCUSIN:          {  //  9
        xcb_focus_in_event_t event {};
        event.response_type = XCB_FOCUS_IN;
        event_p = (const char*)&event;
    }   break;
    case FOCUSOUT:         {  // 10
        xcb_focus_out_event_t event {};
        event.response_type = XCB_FOCUS_OUT;
        event_p = (const char*)&event;
    }   break;
    case KEYMAPNOTIFY:     {  // 11
        xcb_keymap_notify_event_t event {};
        event.response_type = XCB_KEYMAP_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case EXPOSE:           {  // 12
        xcb_expose_event_t event {};
        event.response_type = XCB_EXPOSE;
        event_p = (const char*)&event;
    }   break;
    case GRAPHICSEXPOSURE: {  // 13
        xcb_graphics_exposure_event_t event {};
        event.response_type = XCB_GRAPHICS_EXPOSURE;
        event_p = (const char*)&event;
    }   break;
    case NOEXPOSURE:       {  // 14
        xcb_no_exposure_event_t event {};
        event.response_type = XCB_NO_EXPOSURE;
        event_p = (const char*)&event;
    }   break;
    case VISIBILITYNOTIFY: {  // 15
        xcb_visibility_notify_event_t event {};
        event.response_type = XCB_VISIBILITY_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case CREATENOTIFY:     {  // 16
        xcb_create_notify_event_t event {};
        event.response_type = XCB_CREATE_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case DESTROYNOTIFY:    {  // 17
        xcb_destroy_notify_event_t event {};
        event.response_type = XCB_DESTROY_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case UNMAPNOTIFY:      {  // 18
        xcb_unmap_notify_event_t event {};
        event.response_type = XCB_UNMAP_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case MAPNOTIFY:        {  // 19
        xcb_map_notify_event_t event {};
        event.response_type = XCB_MAP_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case MAPREQUEST:       {  // 20
        xcb_map_request_event_t event {};
        event.response_type = XCB_MAP_REQUEST;
        event_p = (const char*)&event;
    }   break;
    case REPARENTNOTIFY:   {  // 21
        xcb_reparent_notify_event_t event {};
        event.response_type = XCB_REPARENT_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case CONFIGURENOTIFY:  {  // 22
        xcb_configure_notify_event_t event {};
        event.response_type = XCB_CONFIGURE_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case CONFIGUREREQUEST: {  // 23
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
        xcb_gravity_notify_event_t event {};
        event.response_type = XCB_GRAVITY_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case RESIZEREQUEST:    {  // 25
        xcb_resize_request_event_t event {};
        event.response_type = XCB_RESIZE_REQUEST;
        event_p = (const char*)&event;
    }   break;
    case CIRCULATENOTIFY:  {  // 26
        xcb_circulate_notify_event_t event {};
        event.response_type = XCB_CIRCULATE_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case CIRCULATEREQUEST: {  // 27
        xcb_circulate_request_event_t event {};
        event.response_type = XCB_CIRCULATE_REQUEST;
        event_p = (const char*)&event;
    }   break;
    case PROPERTYNOTIFY:   {  // 28
        xcb_property_notify_event_t event {};
        event.response_type = XCB_PROPERTY_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case SELECTIONCLEAR:   {  // 29
        xcb_selection_clear_event_t event {};
        event.response_type = XCB_SELECTION_CLEAR;
        event_p = (const char*)&event;
    }   break;
    case SELECTIONREQUEST: {  // 30
        xcb_selection_request_event_t event {};
        event.response_type = XCB_SELECTION_REQUEST;
        event_p = (const char*)&event;
    }   break;
    case SELECTIONNOTIFY:  {  // 31
        xcb_selection_notify_event_t event {};
        event.response_type = XCB_SELECTION_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case COLORMAPNOTIFY:   {  // 32
        xcb_colormap_notify_event_t event {};
        event.response_type = XCB_COLORMAP_NOTIFY;
        event_p = (const char*)&event;
    }   break;
    case CLIENTMESSAGE:    {  // 33
        xcb_client_message_event_t event {};
        event.response_type = XCB_CLIENT_MESSAGE;
        event_p = (const char*)&event;
    }   break;
    case MAPPINGNOTIFY:    {  // 34
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
