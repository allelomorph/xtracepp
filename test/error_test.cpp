// https://xcb.freedesktop.org/tutorial/basicwindowsanddrawing/

#include <string>  // atoi

#include <cassert>
#include <cstdlib>  // free
#include <sys/uio.h>  // iovec

#include <xcb/xcb.h>
#include <xcb/xcbext.h>  // xcb_protocol_request_t xcb_send_request
#include <protocol/errors.hpp>
#include <protocol/requests.hpp>

#include <iostream>


int main(const int argc, const char* const* argv) {
    assert( argc > 1 );
    const int code { std::stoi(argv[1]) };
    assert( code >= protocol::errors::codes::MIN &&
            code <= protocol::errors::codes::MAX );
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
    uint32_t bad_resource {};
    uint16_t request_opcode {};
    switch ( code ) {
    case protocol::errors::codes::REQUEST:         {  //  1
        // TBD since normal intended use of xcb contains all core requests in
        //   wrapper functions, we need a lower-level way to send a request
        //   with bad opcode, here using calls meant for extensions
        int flags {};
        // TBD xcb implementation cannot accept nullptr vector, or iov_lens that
        //   are not multiples of 4
        // TBD xtrace implements unparsed-data field, testing here with vector_[1]
        //   `000:<:0001:  8: Request(128): UNKNOWN opcode=0x80 opcode2=0x00 unparsed-data=0x01,0x02,0x03,0x04;`
        //    is xtrace vetting the opcodes before sending to the server?
        uint32_t iov0_data {};
        uint32_t iov1_data { 0x04030201 };  // TBD test unparsed-data
        iovec vector_[2] {
            { &iov0_data, sizeof(iov0_data) },
            { &iov1_data, sizeof(iov1_data) }
        };
        xcb_protocol_request_t request {
            sizeof( vector_ ) / sizeof( iovec ),
                       // count (of elements in iovec array)
            nullptr,   // ext (nullptr for core protocol)
            protocol::requests::opcodes::NOOPERATION + 1,
                       // opcode (core protocol major opcode deliberately out of range)
            int(true)  // isvoid (no reply expected)
        };
        unsigned int retval {
            xcb_send_request(connection, flags, vector_, &request) };
        assert( retval != 0 );
        // expects xcb_flush(connection) to ensure request is sent
    }   break;
    case protocol::errors::codes::VALUE:           {  //  2
        // mode deliberately outside enum range
        uint8_t         mode { XCB_ALLOW_SYNC_BOTH + 1 };
        xcb_timestamp_t time {};
        xcb_void_cookie_t cookie {
            xcb_allow_events_checked(connection, mode, time) };
        error = xcb_request_check(connection, cookie);
        bad_resource = mode;
        request_opcode = protocol::requests::opcodes::ALLOWEVENTS;
    }   break;
    case protocol::errors::codes::WINDOW:          {  //  3
        // TBD generate arbitrary id?
        xcb_window_t window {};
        xcb_void_cookie_t cookie {
            xcb_destroy_window_checked(connection, window) };
        error = xcb_request_check(connection, cookie);
        bad_resource = window;
        request_opcode = protocol::requests::opcodes::DESTROYWINDOW;
    }   break;
    case protocol::errors::codes::PIXMAP:          {  //  4
        xcb_pixmap_t pixmap {};
        xcb_void_cookie_t cookie {
            xcb_free_pixmap_checked(connection, pixmap) };
        error = xcb_request_check(connection, cookie);
        bad_resource = pixmap;
        request_opcode = protocol::requests::opcodes::FREEPIXMAP;
    }   break;
    case protocol::errors::codes::ATOM:            {  //  5
        xcb_atom_t selection { XCB_ATOM_NONE };
        xcb_get_selection_owner_cookie_t cookie {
            xcb_get_selection_owner(connection, selection) };
        xcb_get_selection_owner_reply_t* reply {
            xcb_get_selection_owner_reply(connection, cookie, &error) };
        assert( reply == nullptr );
        bad_resource = selection;
        request_opcode = protocol::requests::opcodes::GETSELECTIONOWNER;
    }   break;
    case protocol::errors::codes::CURSOR:          {  //  6
    }   break;
    case protocol::errors::codes::FONT:            {  //  7
    }   break;
    case protocol::errors::codes::MATCH:           {  //  8
    }   break;
    case protocol::errors::codes::DRAWABLE:        {  //  9
    }   break;
    case protocol::errors::codes::ACCESS:          {  // 10
    }   break;
    case protocol::errors::codes::ALLOC:           {  // 11
    }   break;
    case protocol::errors::codes::COLORMAP:        {  // 12
    }   break;
    case protocol::errors::codes::GCONTEXT:        {  // 13
    }   break;
    case protocol::errors::codes::IDCHOICE:        {  // 14
    }   break;
    case protocol::errors::codes::NAME:            {  // 15
    }   break;
    case protocol::errors::codes::LENGTH:          {  // 16
    }   break;
    case protocol::errors::codes::IMPLEMENTATION:  {  // 17
    }   break;
    default:
        break;
    }

    if ( code != protocol::errors::codes::REQUEST ) {
        assert( error != nullptr );
        assert( error->response_type == 0 );
        assert( error->error_code == code );
        assert( error->major_code == request_opcode );
        assert( error->resource_id == bad_resource );
        free( error );
    }
    // // prompt server to send error on request with no reply
    // xcb_void_cookie_t cookie {
    //     xcb_map_window_checked(connection, 0) };
    // xcb_generic_error_t* error {
    //     xcb_request_check(connection, cookie) };
    // assert( error != nullptr );
    // assert( error->response_type == 0 );
    // // std::cout <<
    // //     "response_type: " << int(error->response_type) << '\n' <<
    // //     "error_code: " << int(error->error_code) << '\n' <<
    // //     "sequence: " << int(error->sequence) << '\n' <<
    // //     "resouce_id: " << int(error->resource_id) << '\n' <<
    // //     "minor_code: " << int(error->minor_code) << '\n' <<
    // //     "major_code: " << int(error->major_code) << '\n' <<
    // //     "full_sequence: " << int(error->full_sequence) << '\n' <<
    // //     std::endl;
    // free( error );

    // // prompt server to send error on request with reply
    // xcb_get_geometry_cookie_t cookie2 {
    //     xcb_get_geometry(connection, window) };
    // xcb_generic_error_t* error2 {};
    // xcb_get_geometry_reply_t* reply {
    //     xcb_get_geometry_reply(connection, cookie2, &error2) };
    // if ( !reply ) {
    //     assert( error2 != nullptr );
    //     assert( error2->response_type == 0 );
    //     // std::cout <<
    //     //     "response_type: " << int(error2->response_type) << '\n' <<
    //     //     "error_code: " << int(error2->error_code) << '\n' <<
    //     //     "sequence: " << int(error2->sequence) << '\n' <<
    //     //     "resouce_id: " << int(error2->resource_id) << '\n' <<
    //     //     "minor_code: " << int(error2->minor_code) << '\n' <<
    //     //     "major_code: " << int(error2->major_code) << '\n' <<
    //     //     "full_sequence: " << int(error2->full_sequence) << '\n' <<
    //     //     std::endl;
    //     free( error2 );
    // } else {
    //     free( reply );
    // }
    // xcb_destroy_window(
    //     connection, window);

    xcb_flush( connection );
    xcb_disconnect( connection );
}
