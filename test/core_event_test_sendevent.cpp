#include <string>               // stoi

#include <cassert>
#include <cstdint>
#include <cstdio>               // stderr
#include <cstdlib>              // EXIT_FAILURE

#include <fmt/format.h>

#include <xcb/xcb.h>

#include <protocol/events.hpp>


int main( const int argc, const char* const* argv ) {
    assert( argc >= 1 );
    const char* process_name { argv[ 0 ] };
    if ( argc == 1 ) {
        fmt::println( ::stderr, "{}: please provide event code",
                      process_name );
        return EXIT_FAILURE;
    }
    const int code { std::stoi( argv[ 1 ] ) };
    namespace evt_codes = protocol::events::codes;
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

    // (minimum parameter population for xcb to send request)
    const uint8_t      propagate   {};
    const xcb_window_t destination {};
    const uint32_t     event_mask  { XCB_EVENT_MASK_STRUCTURE_NOTIFY };

    switch ( code ) {
    case evt_codes::KEYPRESS:         {  //  2
        xcb_key_press_event_t event {};
        event.response_type = XCB_KEY_PRESS;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::KEYRELEASE:       {  //  3
        xcb_key_release_event_t event {};
        event.response_type = XCB_KEY_RELEASE;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::BUTTONPRESS:      {  //  4
        xcb_button_press_event_t event {};
        event.response_type = XCB_BUTTON_PRESS;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::BUTTONRELEASE:    {  //  5
        xcb_button_release_event_t event {};
        event.response_type = XCB_BUTTON_RELEASE;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::MOTIONNOTIFY:     {  //  6
        xcb_motion_notify_event_t event {};
        event.response_type = XCB_MOTION_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::ENTERNOTIFY:      {  //  7
        xcb_enter_notify_event_t event {};
        event.response_type = XCB_ENTER_NOTIFY;
        // no libxcb constants found for same_screen_focus flags
        event.same_screen_focus = 0x01 | 0x02;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::LEAVENOTIFY:      {  //  8
        xcb_leave_notify_event_t event {};
        event.response_type = XCB_LEAVE_NOTIFY;
        // no libxcb constants found for same_screen_focus flags
        event.same_screen_focus = 0x01 | 0x02;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::FOCUSIN:          {  //  9
        xcb_focus_in_event_t event {};
        event.response_type = XCB_FOCUS_IN;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::FOCUSOUT:         {  // 10
        xcb_focus_out_event_t event {};
        event.response_type = XCB_FOCUS_OUT;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::KEYMAPNOTIFY:     {  // 11
        xcb_keymap_notify_event_t event {};
        event.response_type = XCB_KEYMAP_NOTIFY;
        event.keys[0] = 0x01;
        event.keys[1] = 0x02;
        event.keys[2] = 0x04;
        event.keys[3] = 0x08;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::EXPOSE:           {  // 12
        xcb_expose_event_t event {};
        event.response_type = XCB_EXPOSE;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::GRAPHICSEXPOSURE: {  // 13
        xcb_graphics_exposure_event_t event {};
        event.response_type = XCB_GRAPHICS_EXPOSURE;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::NOEXPOSURE:       {  // 14
        xcb_no_exposure_event_t event {};
        event.response_type = XCB_NO_EXPOSURE;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::VISIBILITYNOTIFY: {  // 15
        xcb_visibility_notify_event_t event {};
        event.response_type = XCB_VISIBILITY_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::CREATENOTIFY:     {  // 16
        xcb_create_notify_event_t event {};
        event.response_type = XCB_CREATE_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::DESTROYNOTIFY:    {  // 17
        xcb_destroy_notify_event_t event {};
        event.response_type = XCB_DESTROY_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::UNMAPNOTIFY:      {  // 18
        xcb_unmap_notify_event_t event {};
        event.response_type = XCB_UNMAP_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::MAPNOTIFY:        {  // 19
        xcb_map_notify_event_t event {};
        event.response_type = XCB_MAP_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::MAPREQUEST:       {  // 20
        xcb_map_request_event_t event {};
        event.response_type = XCB_MAP_REQUEST;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::REPARENTNOTIFY:   {  // 21
        xcb_reparent_notify_event_t event {};
        event.response_type = XCB_REPARENT_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::CONFIGURENOTIFY:  {  // 22
        xcb_configure_notify_event_t event {};
        event.response_type = XCB_CONFIGURE_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::CONFIGUREREQUEST: {  // 23
        xcb_configure_request_event_t event {};
        event.response_type = XCB_CONFIGURE_REQUEST;
        event.value_mask =
            XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
            XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT |
            XCB_CONFIG_WINDOW_BORDER_WIDTH | XCB_CONFIG_WINDOW_SIBLING |
            XCB_CONFIG_WINDOW_STACK_MODE;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::GRAVITYNOTIFY:    {  // 24
        xcb_gravity_notify_event_t event {};
        event.response_type = XCB_GRAVITY_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::RESIZEREQUEST:    {  // 25
        xcb_resize_request_event_t event {};
        event.response_type = XCB_RESIZE_REQUEST;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::CIRCULATENOTIFY:  {  // 26
        xcb_circulate_notify_event_t event {};
        event.response_type = XCB_CIRCULATE_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::CIRCULATEREQUEST: {  // 27
        xcb_circulate_request_event_t event {};
        event.response_type = XCB_CIRCULATE_REQUEST;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::PROPERTYNOTIFY:   {  // 28
        xcb_property_notify_event_t event {};
        event.response_type = XCB_PROPERTY_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::SELECTIONCLEAR:   {  // 29
        xcb_selection_clear_event_t event {};
        event.response_type = XCB_SELECTION_CLEAR;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::SELECTIONREQUEST: {  // 30
        xcb_selection_request_event_t event {};
        event.response_type = XCB_SELECTION_REQUEST;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::SELECTIONNOTIFY:  {  // 31
        xcb_selection_notify_event_t event {};
        event.response_type = XCB_SELECTION_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::COLORMAPNOTIFY:   {  // 32
        xcb_colormap_notify_event_t event {};
        event.response_type = XCB_COLORMAP_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::CLIENTMESSAGE:    {  // 33
        xcb_client_message_event_t event {};
        event.response_type = XCB_CLIENT_MESSAGE;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    case evt_codes::MAPPINGNOTIFY:    {  // 34
        xcb_mapping_notify_event_t event {};
        event.response_type = XCB_MAPPING_NOTIFY;
        xcb_send_event(
            conn, propagate, destination, event_mask,
            reinterpret_cast< const char* >( &event ) );
    }   break;
    default:
        break;
    }

    xcb_flush( conn );
    xcb_disconnect( conn );
}
