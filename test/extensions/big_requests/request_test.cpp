#include <cassert>
#include <cstdint>
#include <cstdio>                 // stderr
#include <cstdlib>                // free, EXIT_FAILURE, EXIT_SUCCESS

#include <sys/uio.h>              // iovec

#include <fmt/format.h>

#include <xcb/xcb.h>
#include <xcb/xcbext.h>           // xcb_protocol_request_t, xcb_send_request

#include <protocol/Message.hpp>
#include <protocol/requests.hpp>

#include <fmt/format.h>


int main( [[maybe_unused]] const int argc, const char* const* argv ) {
    assert( argc >= 1 );
    const char* process_name { argv[ 0 ] };

    // Open the connection to the X server
    xcb_connection_t* conn {
        xcb_connect( nullptr, nullptr ) };
    assert( conn != nullptr );
    [[maybe_unused]] const xcb_setup_t* setup  { xcb_get_setup( conn ) };
    assert( setup  != nullptr );

    // When greater customization of the request encoding than that provided
    //   by the `xcb_`(request name) functions is required, we can use
    //   xcb_send_request, see:
    //   - https://xcb.freedesktop.org/ProtocolExtensionApi/
    //   - https://gitlab.freedesktop.org/xorg/lib/libxcb/-/blob/master/src/xcbext.h?ref_type=heads#L47-L83
    const int flags { XCB_REQUEST_CHECKED };
    // BIG-REQUESTS encoding should automatically be used above 262140 bytes/
    //   65535 alignment units
    static constexpr size_t SHORT_LEN_MAX { 262140 };
    assert( SHORT_LEN_MAX % protocol::Message::ALIGN == 0 );
    assert( SHORT_LEN_MAX / protocol::Message::ALIGN ==
            setup->maximum_request_length );
    uint8_t iov0_data[ SHORT_LEN_MAX + protocol::Message::ALIGN ] {};
    ::iovec vector_[ 1 ] {
        { iov0_data, sizeof( iov0_data ) }
    };
    const xcb_protocol_request_t request {
        sizeof( vector_ ) / sizeof( ::iovec ),  // count (of elements in iovec array)
        nullptr,                                // ext (nullptr for core protocol)
        protocol::requests::opcodes::PUTIMAGE,  // opcode
        int(true)                               // isvoid (no reply expected)
    };
    const xcb_void_cookie_t cookie {
        xcb_send_request( conn, flags, vector_, &request ) };
    xcb_generic_error_t* error {
        xcb_request_check( conn, cookie ) };
    if ( error != nullptr ) {
        fmt::println( ::stderr, "{}: PutImage failure",
                      process_name );
        return EXIT_FAILURE;
    }

    xcb_flush( conn );
    xcb_disconnect( conn );
    return EXIT_SUCCESS;
}
