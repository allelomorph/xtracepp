// https://xcb.freedesktop.org/tutorial/basicwindowsanddrawing/

#include <xcb/xcb.h>

int main() {
    /* Open the connection to the X server */
    xcb_connection_t *connection = xcb_connect( nullptr, nullptr );

    // Get the first screen
    const xcb_setup_t      *setup  = xcb_get_setup( connection );
    xcb_screen_iterator_t   iter   = xcb_setup_roots_iterator( setup );
    xcb_screen_t           *screen = iter.data;

    /* Create the window */
    xcb_window_t window = xcb_generate_id ( connection );
    uint32_t value_list[15] {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };
    xcb_create_window(
        connection,                    /* Connection          */
        XCB_COPY_FROM_PARENT,          /* depth (same as root)*/
        window,                        /* window Id           */
        screen->root,                  /* parent window       */
        0, 0,                          /* x, y                */
        150, 150,                      /* width, height       */
        10,                            /* border_width        */
        XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
        screen->root_visual,           /* visual              */
        XCB_CW_BACK_PIXMAP | XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXMAP |
        XCB_CW_BORDER_PIXEL | XCB_CW_BIT_GRAVITY | XCB_CW_WIN_GRAVITY |
        XCB_CW_BACKING_STORE | XCB_CW_BACKING_PLANES | XCB_CW_BACKING_PIXEL |
        XCB_CW_OVERRIDE_REDIRECT | XCB_CW_SAVE_UNDER | XCB_CW_EVENT_MASK |
        XCB_CW_DONT_PROPAGATE | XCB_CW_COLORMAP | XCB_CW_CURSOR,
                                      /* value-mask          */
        value_list );                 /* value-list          */

    /* Make sure commands are sent before we pause so that the window gets shown */
    xcb_flush( connection );

    xcb_disconnect( connection );
}
