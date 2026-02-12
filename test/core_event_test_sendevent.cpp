#include <string>   // stoi

#include <cassert>
#include <cstdint>
#include <cstdlib>  // EXIT_FAILURE EXIT_SUCCESS

#include <fmt/format.h>

#include <xcb/xcb.h>

#include <protocol/requests.hpp>
#include <protocol/events.hpp>


int main( const int argc, const char* const* argv ) {
    assert( argc > 1 );
    const char* process_name { argv[ 0 ] };
    const int code { std::stoi( argv[ 1 ] ) };
    namespace  evt_codes = protocol::events::codes;
    if ( !( code >= evt_codes::KEYPRESS &&          //  2
            code <= evt_codes::MAPPINGNOTIFY ) ) {  // 34
        fmt::println( stderr, "{}: invalid core event code: {} (expected 2-34)",
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

    // (minimum parameter population for xcb to send request)
    const uint8_t      propagate   {};
    const xcb_window_t destination {};
    const uint32_t     event_mask  { XCB_EVENT_MASK_STRUCTURE_NOTIFY };
    const char*        event       {};

    switch ( code ) {
    case evt_codes::KEYPRESS:         {  //  2
        xcb_key_press_event_t event_ {};
        event_.response_type = XCB_KEY_PRESS;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::KEYRELEASE:       {  //  3
        xcb_key_release_event_t event_ {};
        event_.response_type = XCB_KEY_RELEASE;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::BUTTONPRESS:      {  //  4
        xcb_button_press_event_t event_ {};
        event_.response_type = XCB_BUTTON_PRESS;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::BUTTONRELEASE:    {  //  5
        xcb_button_release_event_t event_ {};
        event_.response_type = XCB_BUTTON_RELEASE;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::MOTIONNOTIFY:     {  //  6
        xcb_motion_notify_event_t event_ {};
        event_.response_type = XCB_MOTION_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::ENTERNOTIFY:      {  //  7
        xcb_enter_notify_event_t event_ {};
        event_.response_type = XCB_ENTER_NOTIFY;
        // no libxcb constants found for same_screen_focus flags
        event_.same_screen_focus = 0x01 | 0x02;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::LEAVENOTIFY:      {  //  8
        xcb_leave_notify_event_t event_ {};
        event_.response_type = XCB_LEAVE_NOTIFY;
        // no libxcb constants found for same_screen_focus flags
        event_.same_screen_focus = 0x01 | 0x02;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::FOCUSIN:          {  //  9
        xcb_focus_in_event_t event_ {};
        event_.response_type = XCB_FOCUS_IN;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::FOCUSOUT:         {  // 10
        xcb_focus_out_event_t event_ {};
        event_.response_type = XCB_FOCUS_OUT;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::KEYMAPNOTIFY:     {  // 11
        xcb_keymap_notify_event_t event_ {};
        event_.response_type = XCB_KEYMAP_NOTIFY;
        event_.keys[0] = 0x01;
        event_.keys[1] = 0x02;
        event_.keys[2] = 0x04;
        event_.keys[3] = 0x08;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::EXPOSE:           {  // 12
        xcb_expose_event_t event_ {};
        event_.response_type = XCB_EXPOSE;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::GRAPHICSEXPOSURE: {  // 13
        xcb_graphics_exposure_event_t event_ {};
        event_.response_type = XCB_GRAPHICS_EXPOSURE;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::NOEXPOSURE:       {  // 14
        xcb_no_exposure_event_t event_ {};
        event_.response_type = XCB_NO_EXPOSURE;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::VISIBILITYNOTIFY: {  // 15
        xcb_visibility_notify_event_t event_ {};
        event_.response_type = XCB_VISIBILITY_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::CREATENOTIFY:     {  // 16
        xcb_create_notify_event_t event_ {};
        event_.response_type = XCB_CREATE_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::DESTROYNOTIFY:    {  // 17
        xcb_destroy_notify_event_t event_ {};
        event_.response_type = XCB_DESTROY_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::UNMAPNOTIFY:      {  // 18
        xcb_unmap_notify_event_t event_ {};
        event_.response_type = XCB_UNMAP_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::MAPNOTIFY:        {  // 19
        xcb_map_notify_event_t event_ {};
        event_.response_type = XCB_MAP_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::MAPREQUEST:       {  // 20
        xcb_map_request_event_t event_ {};
        event_.response_type = XCB_MAP_REQUEST;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::REPARENTNOTIFY:   {  // 21
        xcb_reparent_notify_event_t event_ {};
        event_.response_type = XCB_REPARENT_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::CONFIGURENOTIFY:  {  // 22
        xcb_configure_notify_event_t event_ {};
        event_.response_type = XCB_CONFIGURE_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::CONFIGUREREQUEST: {  // 23
        xcb_configure_request_event_t event_ {};
        event_.response_type = XCB_CONFIGURE_REQUEST;
        event_.value_mask =
            XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
            XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT |
            XCB_CONFIG_WINDOW_BORDER_WIDTH | XCB_CONFIG_WINDOW_SIBLING |
            XCB_CONFIG_WINDOW_STACK_MODE;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::GRAVITYNOTIFY:    {  // 24
        xcb_gravity_notify_event_t event_ {};
        event_.response_type = XCB_GRAVITY_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::RESIZEREQUEST:    {  // 25
        xcb_resize_request_event_t event_ {};
        event_.response_type = XCB_RESIZE_REQUEST;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::CIRCULATENOTIFY:  {  // 26
        xcb_circulate_notify_event_t event_ {};
        event_.response_type = XCB_CIRCULATE_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::CIRCULATEREQUEST: {  // 27
        xcb_circulate_request_event_t event_ {};
        event_.response_type = XCB_CIRCULATE_REQUEST;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::PROPERTYNOTIFY:   {  // 28
        xcb_property_notify_event_t event_ {};
        event_.response_type = XCB_PROPERTY_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::SELECTIONCLEAR:   {  // 29
        xcb_selection_clear_event_t event_ {};
        event_.response_type = XCB_SELECTION_CLEAR;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::SELECTIONREQUEST: {  // 30
        xcb_selection_request_event_t event_ {};
        event_.response_type = XCB_SELECTION_REQUEST;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::SELECTIONNOTIFY:  {  // 31
        xcb_selection_notify_event_t event_ {};
        event_.response_type = XCB_SELECTION_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::COLORMAPNOTIFY:   {  // 32
        xcb_colormap_notify_event_t event_ {};
        event_.response_type = XCB_COLORMAP_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::CLIENTMESSAGE:    {  // 33
        xcb_client_message_event_t event_ {};
        event_.response_type = XCB_CLIENT_MESSAGE;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    case evt_codes::MAPPINGNOTIFY:    {  // 34
        xcb_mapping_notify_event_t event_ {};
        event_.response_type = XCB_MAPPING_NOTIFY;
        event = reinterpret_cast< const char* >( &event_ );
    }   break;
    default:
        break;
    }

    xcb_send_event(
        conn, propagate, destination, event_mask, event );
    xcb_flush( conn );
    xcb_disconnect( conn );
}
