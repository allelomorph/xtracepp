#include <cassert>
#include <cstdlib>       // free EXIT_FAILURE EXIT_SUCCESS

#include <fmt/format.h>

#include <xcb/xcb.h>

#include <protocol/atoms.hpp>


int main( [[maybe_unused]] const int argc, const char* const* argv ) {
    assert( argc >= 1 );
    const char* process_name { argv[ 0 ] };

    // Open the connection to the X server
    xcb_connection_t* conn {
        xcb_connect( nullptr, nullptr ) };
    assert( conn != nullptr );

    xcb_generic_error_t* error {};
    const xcb_atom_t atom { protocol::atoms::predefined::MAX + 1 };
    const xcb_get_atom_name_cookie_t cookie {
        xcb_get_atom_name( conn, atom ) };
    const xcb_get_atom_name_reply_t* reply {
        xcb_get_atom_name_reply( conn, cookie, &error ) };
    if ( error != nullptr ) {
        fmt::println( ::stderr, "{}: GetAtomName failure",
                      process_name );
        return EXIT_FAILURE;
    }
    assert( reply != nullptr );
    ::free( const_cast< xcb_get_atom_name_reply_t* >( reply ) );

    // Make sure commands are sent before we pause so that the window gets shown
    xcb_flush( conn );
    xcb_disconnect( conn );
}
