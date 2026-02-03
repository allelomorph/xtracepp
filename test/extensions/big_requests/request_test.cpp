#include <cassert>
#include <cstdint>
#include <cstdlib>                // free EXIT_FAILURE EXIT_SUCCESS
#include <sys/uio.h>              // iovec

#include <xcb/xcb.h>
#include <xcb/xcbext.h>           // xcb_protocol_request_t xcb_send_request

#include <protocol/errors.hpp>
#include <protocol/requests.hpp>

#include <fmt/format.h>


int main() {
    // Open the connection to the X server
    xcb_connection_t* connection  {
        xcb_connect( nullptr, nullptr ) };
    assert( connection != nullptr );

    // Get the first screen
    const xcb_setup_t*    setup  { xcb_get_setup( connection ) };
    xcb_screen_iterator_t iter   { xcb_setup_roots_iterator( setup ) };
    xcb_screen_t*         screen { iter.data };
    assert( setup  != nullptr );
    assert( screen != nullptr );

    xcb_generic_error_t* error          {};
    // - https://xcb.freedesktop.org/ProtocolExtensionApi/
    // - https://gitlab.freedesktop.org/xorg/lib/libxcb/-/blob/master/src/xcbext.h?ref_type=heads#L47-L83
    const int flags          { XCB_REQUEST_CHECKED };
    // BIG-REQUESTS encoding should automatically be used above 262140 bytes/
    //   65535 alignment units
    // byte count must be aligned to mulitple of 4
    static constexpr size_t SHORT_LEN_MAX { 262140 };
    uint8_t buf[ SHORT_LEN_MAX + protocol::Message::ALIGN ] {};
    // major-opcode encoded in header is ignored in favor of one passed in
    //   xcb_protocol_request_t param)
    ::iovec vector_[ 1 ] {
        { buf, sizeof( buf ) }
    };
    const xcb_protocol_request_t request {
        sizeof( vector_ ) / sizeof( ::iovec ),  // count (of elements in iovec array)
        nullptr,                                // ext (nullptr for core protocol)
        protocol::requests::opcodes::PUTIMAGE,  // opcode
        int(true)                               // isvoid (no reply expected)
    };
    const xcb_void_cookie_t cookie {
        xcb_send_request( connection, flags, vector_, &request ) };
    error = xcb_request_check( connection, cookie );
    assert( error != nullptr );

    xcb_flush( connection );
    xcb_disconnect( connection );
    return EXIT_SUCCESS;
}
