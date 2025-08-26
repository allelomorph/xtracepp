// https://xcb.freedesktop.org/tutorial/basicwindowsanddrawing/

#include <unordered_map>
#include <string_view>
#include <limits>

#include <unistd.h>  // free usleep

#include <cassert>
#include <cstdint>

#include <xcb/xcb.h>
#include <fmt/format.h>


int main(const int argc, const char* const* argv) {
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
    constexpr uint32_t MAX_ATOM { 0x1fffffff };
    constexpr uint32_t HEX_WIDTH { (sizeof(uint32_t) * 2) + sizeof("0x") };
    //constexpr char ASCII_ESC_RETURN_TO_COL_0[sizeof("\x1b[0G")] { "\x1b[0G" };
    for ( uint32_t atom {}, empty {}; atom < MAX_ATOM && empty < 100; ++atom) {
        const xcb_get_atom_name_cookie_t cookie {
            xcb_get_atom_name(connection, atom) };
        const xcb_get_atom_name_reply_t* reply {
            xcb_get_atom_name_reply(connection, cookie, &error) };
        if ( error == nullptr ) {
            fmt::println( "{:#{}x}(\"{}\")", atom, HEX_WIDTH,
                          std::string_view {
                              xcb_get_atom_name_name( reply ), reply->name_len } );
        } else {
            ++empty;
            //fmt::print( "{:#{}x}{}", atom, HEX_WIDTH, ASCII_ESC_RETURN_TO_COL_0 );
        }
        if ( reply != nullptr ) {
            free( const_cast<xcb_get_atom_name_reply_t*>( reply ) );
        }
        usleep(250);
    };

    // Make sure commands are sent before we pause so that the window gets shown
    xcb_flush( connection );
    xcb_disconnect( connection );
}
