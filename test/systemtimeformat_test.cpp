#include <chrono>
#include <thread>

#include <cassert>
#include <cstdlib>  // EXIT_FAILURE EXIT_SUCCESS

#include <fmt/format.h>

#include <xcb/xcb.h>



int main( [[maybe_unused]] const int argc, const char* const* argv ) {
    assert( argc >= 1 );
    const char* process_name { argv[ 0 ] };

    // Open the connection to the X server
    xcb_connection_t* conn {
        xcb_connect( nullptr, nullptr ) };
    assert( conn != nullptr );
    // Get the first screen in `roots`
    const xcb_setup_t*  setup  { xcb_get_setup( conn ) };
    assert( setup  != nullptr );
    const xcb_screen_t* screen { xcb_setup_roots_iterator( setup ).data };
    assert( screen != nullptr );

    xcb_void_cookie_t cookie {};
    // toggle on reporting of property change events
    const xcb_window_t window { screen->root };
    const uint32_t     value_mask { XCB_CW_EVENT_MASK };
    const uint32_t     value_list[1] {
        XCB_EVENT_MASK_PROPERTY_CHANGE
    };
    cookie = xcb_change_window_attributes_checked(
        conn, window, value_mask, value_list );
    if ( const xcb_generic_error_t* error { xcb_request_check( conn, cookie ) };
         error != nullptr ) {
        fmt::println( ::stderr, "{}: ChangeWindowAttributes failure",
                      process_name );
        return EXIT_FAILURE;
    }

    using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
    for ( int i { 1 }; i <= 10; ++i ) {
        // 0-length property name append as no-op that triggers event with
        //   current timestamp
        const uint8_t      mode     { XCB_PROP_MODE_APPEND };
        const xcb_window_t window   { screen->root };
        const xcb_atom_t   property { XCB_ATOM_WM_NAME };
        const xcb_atom_t   type     { XCB_ATOM_STRING };
        const uint8_t      format   { 8 };  // (bits per fmt unit)
        const uint32_t     data_len { 0 };
        cookie = xcb_change_property(
            conn, mode, window, property,
            type, format, data_len, nullptr/*data*/ );
        if ( const xcb_generic_error_t* error { xcb_request_check( conn, cookie ) };
             error != nullptr ) {
            fmt::println( ::stderr, "{}: ChangeProperty failure",
                          process_name );
            return EXIT_FAILURE;
        }
        fmt::println( ::stderr, "{}: sleeping for {} seconds",
                      process_name, i );
        std::this_thread::sleep_for( 1s * i );
    }

    // Make sure commands are sent before we pause so that the window gets shown
    xcb_flush( conn );
    xcb_disconnect( conn );
}
