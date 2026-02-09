#include <string>        // atoi

#include <cassert>
#include <cstdint>
#include <cstdlib>       // EXIT_FAILURE EXIT_SUCCESS

#include <unistd.h>      // free

#include <fmt/format.h>

#include <xcb/xcb.h>

#include <protocol/atoms.hpp>


int main( const int argc, const char* const* argv ) {
    assert( argc >= 1 );
    [[maybe_unused]] const char* process_name { argv[ 0 ] };

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
    const xcb_atom_t atom { protocol::atoms::predefined::MAX + 1 };
    const xcb_get_atom_name_cookie_t cookie {
        xcb_get_atom_name( connection, atom ) };
    const xcb_get_atom_name_reply_t* reply {
        xcb_get_atom_name_reply( connection, cookie, &error ) };
    assert( error == nullptr );
    assert( reply != nullptr );
    free( const_cast< xcb_get_atom_name_reply_t* >( reply ) );

    // Make sure commands are sent before we pause so that the window gets shown
    xcb_flush( connection );
    xcb_disconnect( connection );
}
