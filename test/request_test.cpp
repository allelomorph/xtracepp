// https://xcb.freedesktop.org/tutorial/basicwindowsanddrawing/

#include <string>  // atoi

#include <cassert>

#include <xcb/xcb.h>


enum Opcodes {
    CREATEWINDOW              =    1,
    CHANGEWINDOWATTRIBUTES,   //   2
    GETWINDOWATTRIBUTES,      //   3
    DESTROYWINDOW,            //   4
    DESTROYSUBWINDOWS,        //   5
    CHANGESAVESET,            //   6
    REPARENTWINDOW,           //   7
    MAPWINDOW,                //   8
    MAPSUBWINDOWS,            //   9
    UNMAPWINDOW,              //  10
    UNMAPSUBWINDOWS,          //  11
    CONFIGUREWINDOW,          //  12
    CIRCULATEWINDOW,          //  13
    GETGEOMETRY,              //  14
    QUERYTREE,                //  15
    INTERNATOM,               //  16
    GETATOMNAME,              //  17
    CHANGEPROPERTY,           //  18
    DELETEPROPERTY,           //  19
    GETPROPERTY,              //  20
    LISTPROPERTIES,           //  21
    SETSELECTIONOWNER,        //  22
    GETSELECTIONOWNER,        //  23
    CONVERTSELECTION,         //  24
    SENDEVENT,                //  25
    GRABPOINTER,              //  26
    UNGRABPOINTER,            //  27
    GRABBUTTON,               //  28
    UNGRABBUTTON,             //  29
    CHANGEACTIVEPOINTERGRAB,  //  30
    GRABKEYBOARD,             //  31
    UNGRABKEYBOARD,           //  32
    GRABKEY,                  //  33
    UNGRABKEY,                //  34
    ALLOWEVENTS,              //  35
    GRABSERVER,               //  36
    UNGRABSERVER,             //  37
    QUERYPOINTER,             //  38
    GETMOTIONEVENTS,          //  39
    TRANSLATECOORDINATES,     //  40
    WARPPOINTER,              //  41
    SETINPUTFOCUS,            //  42
    GETINPUTFOCUS,            //  43
    QUERYKEYMAP,              //  44
    OPENFONT,                 //  45
    CLOSEFONT,                //  46
    QUERYFONT,                //  47
    QUERYTEXTEXTENTS,         //  48
    LISTFONTS,                //  49
    LISTFONTSWITHINFO,        //  50
    SETFONTPATH,              //  51
    GETFONTPATH,              //  52
    CREATEPIXMAP,             //  53
    FREEPIXMAP,               //  54
    CREATEGC,                 //  55
    CHANGEGC,                 //  56
    COPYGC,                   //  57
    SETDASHES,                //  58
    SETCLIPRECTANGLES,        //  59
    FREEGC,                   //  60
    CLEARAREA,                //  61
    COPYAREA,                 //  62
    COPYPLANE,                //  63
    POLYPOINT,                //  64
    POLYLINE,                 //  65
    POLYSEGMENT,              //  66
    POLYRECTANGLE,            //  67
    POLYARC,                  //  68
    FILLPOLY,                 //  69
    POLYFILLRECTANGLE,        //  70
    POLYFILLARC,              //  71
    PUTIMAGE,                 //  72
    GETIMAGE,                 //  73
    POLYTEXT8,                //  74
    POLYTEXT16,               //  75
    IMAGETEXT8,               //  76
    IMAGETEXT16,              //  77
    CREATECOLORMAP,           //  78
    FREECOLORMAP,             //  79
    COPYCOLORMAPANDFREE,      //  80
    INSTALLCOLORMAP,          //  81
    UNINSTALLCOLORMAP,        //  82
    LISTINSTALLEDCOLORMAPS,   //  83
    ALLOCCOLOR,               //  84
    ALLOCNAMEDCOLOR,          //  85
    ALLOCCOLORCELLS,          //  86
    ALLOCCOLORPLANES,         //  87
    FREECOLORS,               //  88
    STORECOLORS,              //  89
    STORENAMEDCOLOR,          //  90
    QUERYCOLORS,              //  91
    LOOKUPCOLOR,              //  92
    CREATECURSOR,             //  93
    CREATEGLYPHCURSOR,        //  94
    FREECURSOR,               //  95
    RECOLORCURSOR,            //  96
    QUERYBESTSIZE,            //  97
    QUERYEXTENSION,           //  98
    LISTEXTENSIONS,           //  99
    CHANGEKEYBOARDMAPPING,    // 100
    GETKEYBOARDMAPPING,       // 101
    CHANGEKEYBOARDCONTROL,    // 102
    GETKEYBOARDCONTROL,       // 103
    BELL,                     // 104
    CHANGEPOINTERCONTROL,     // 105
    GETPOINTERCONTROL,        // 106
    SETSCREENSAVER,           // 107
    GETSCREENSAVER,           // 108
    CHANGEHOSTS,              // 109
    LISTHOSTS,                // 110
    SETACCESSCONTROL,         // 111
    SETCLOSEDOWNMODE,         // 112
    KILLCLIENT,               // 113
    ROTATEPROPERTIES,         // 114
    FORCESCREENSAVER,         // 115
    SETPOINTERMAPPING,        // 116
    GETPOINTERMAPPING,        // 117
    SETMODIFIERMAPPING,       // 118
    GETMODIFIERMAPPING,       // 119
    NOOPERATION               =  127
};

int main(const int argc, const char* const* argv) {
    assert( argc > 1 );
    const int opcode { std::stoi(argv[1]) };
    assert( opcode >= 1 && opcode <= 127 );
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

    switch ( opcode ) {
    case CREATEWINDOW:             {  //   1
        // Create the window
        xcb_window_t window { xcb_generate_id ( connection ) };
        uint32_t value_list[15] {
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0
        };
        xcb_create_window(
            connection,                    // Connection
            XCB_COPY_FROM_PARENT,          // depth (same as root)
            window,                        // window id
            screen->root,                  // parent window
            0, 0,                          // x, y
            150, 150,                      // width, height
            10,                            // border_width
            XCB_WINDOW_CLASS_INPUT_OUTPUT, // class
            screen->root_visual,           // visual
            XCB_CW_BACK_PIXMAP | XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXMAP |
            XCB_CW_BORDER_PIXEL | XCB_CW_BIT_GRAVITY | XCB_CW_WIN_GRAVITY |
            XCB_CW_BACKING_STORE | XCB_CW_BACKING_PLANES | XCB_CW_BACKING_PIXEL |
            XCB_CW_OVERRIDE_REDIRECT | XCB_CW_SAVE_UNDER | XCB_CW_EVENT_MASK |
            XCB_CW_DONT_PROPAGATE | XCB_CW_COLORMAP | XCB_CW_CURSOR,
                                          // value-mask
            value_list );                 // value-list
    }
        break;
    case CHANGEWINDOWATTRIBUTES:   {  //   2
    }
        break;
    case GETWINDOWATTRIBUTES:      {  //   3
    }
        break;
    case DESTROYWINDOW:            {  //   4
    }
        break;
    case DESTROYSUBWINDOWS:        {  //   5
    }
        break;
    case CHANGESAVESET:            {  //   6
    }
        break;
    case REPARENTWINDOW:           {  //   7
    }
        break;
    case MAPWINDOW:                {  //   8
    }
        break;
    case MAPSUBWINDOWS:            {  //   9
    }
        break;
    case UNMAPWINDOW:              {  //  10
    }
        break;
    case UNMAPSUBWINDOWS:          {  //  11
    }
        break;
    case CONFIGUREWINDOW:          {  //  12
    }
        break;
    case CIRCULATEWINDOW:          {  //  13
    }
        break;
    case GETGEOMETRY:              {  //  14
    }
        break;
    case QUERYTREE:                {  //  15
    }
        break;
    case INTERNATOM:               {  //  16
    }
        break;
    case GETATOMNAME:              {  //  17
    }
        break;
    case CHANGEPROPERTY:           {  //  18
    }
        break;
    case DELETEPROPERTY:           {  //  19
    }
        break;
    case GETPROPERTY:              {  //  20
    }
        break;
    case LISTPROPERTIES:           {  //  21
    }
        break;
    case SETSELECTIONOWNER:        {  //  22
    }
        break;
    case GETSELECTIONOWNER:        {  //  23
    }
        break;
    case CONVERTSELECTION:         {  //  24
    }
        break;
    case SENDEVENT:                {  //  25
    }
        break;
    case GRABPOINTER:              {  //  26
    }
        break;
    case UNGRABPOINTER:            {  //  27
    }
        break;
    case GRABBUTTON:               {  //  28
    }
        break;
    case UNGRABBUTTON:             {  //  29
    }
        break;
    case CHANGEACTIVEPOINTERGRAB:  {  //  30
    }
        break;
    case GRABKEYBOARD:             {  //  31
    }
        break;
    case UNGRABKEYBOARD:           {  //  32
    }
        break;
    case GRABKEY:                  {  //  33
    }
        break;
    case UNGRABKEY:                {  //  34
    }
        break;
    case ALLOWEVENTS:              {  //  35
    }
        break;
    case GRABSERVER:               {  //  36
    }
        break;
    case UNGRABSERVER:             {  //  37
    }
        break;
    case QUERYPOINTER:             {  //  38
    }
        break;
    case GETMOTIONEVENTS:          {  //  39
    }
        break;
    case TRANSLATECOORDINATES:     {  //  40
    }
        break;
    case WARPPOINTER:              {  //  41
    }
        break;
    case SETINPUTFOCUS:            {  //  42
    }
        break;
    case GETINPUTFOCUS:            {  //  43
    }
        break;
    case QUERYKEYMAP:              {  //  44
    }
        break;
    case OPENFONT:                 {  //  45
    }
        break;
    case CLOSEFONT:                {  //  46
    }
        break;
    case QUERYFONT:                {  //  47
    }
        break;
    case QUERYTEXTEXTENTS:         {  //  48
    }
        break;
    case LISTFONTS:                {  //  49
    }
        break;
    case LISTFONTSWITHINFO:        {  //  50
    }
        break;
    case SETFONTPATH:              {  //  51
    }
        break;
    case GETFONTPATH:              {  //  52
    }
        break;
    case CREATEPIXMAP:             {  //  53
    }
        break;
    case FREEPIXMAP:               {  //  54
    }
        break;
    case CREATEGC:                 {  //  55
    }
        break;
    case CHANGEGC:                 {  //  56
    }
        break;
    case COPYGC:                   {  //  57
    }
        break;
    case SETDASHES:                {  //  58
    }
        break;
    case SETCLIPRECTANGLES:        {  //  59
    }
        break;
    case FREEGC:                   {  //  60
    }
        break;
    case CLEARAREA:                {  //  61
    }
        break;
    case COPYAREA:                 {  //  62
    }
        break;
    case COPYPLANE:                {  //  63
    }
        break;
    case POLYPOINT:                {  //  64
    }
        break;
    case POLYLINE:                 {  //  65
    }
        break;
    case POLYSEGMENT:              {  //  66
    }
        break;
    case POLYRECTANGLE:            {  //  67
    }
        break;
    case POLYARC:                  {  //  68
    }
        break;
    case FILLPOLY:                 {  //  69
    }
        break;
    case POLYFILLRECTANGLE:        {  //  70
    }
        break;
    case POLYFILLARC:              {  //  71
    }
        break;
    case PUTIMAGE:                 {  //  72
    }
        break;
    case GETIMAGE:                 {  //  73
    }
        break;
    case POLYTEXT8:                {  //  74
    }
        break;
    case POLYTEXT16:               {  //  75
    }
        break;
    case IMAGETEXT8:               {  //  76
    }
        break;
    case IMAGETEXT16:              {  //  77
    }
        break;
    case CREATECOLORMAP:           {  //  78
    }
        break;
    case FREECOLORMAP:             {  //  79
    }
        break;
    case COPYCOLORMAPANDFREE:      {  //  80
    }
        break;
    case INSTALLCOLORMAP:          {  //  81
    }
        break;
    case UNINSTALLCOLORMAP:        {  //  82
    }
        break;
    case LISTINSTALLEDCOLORMAPS:   {  //  83
    }
        break;
    case ALLOCCOLOR:               {  //  84
    }
        break;
    case ALLOCNAMEDCOLOR:          {  //  85
    }
        break;
    case ALLOCCOLORCELLS:          {  //  86
    }
        break;
    case ALLOCCOLORPLANES:         {  //  87
    }
        break;
    case FREECOLORS:               {  //  88
    }
        break;
    case STORECOLORS:              {  //  89
    }
        break;
    case STORENAMEDCOLOR:          {  //  90
    }
        break;
    case QUERYCOLORS:              {  //  91
    }
        break;
    case LOOKUPCOLOR:              {  //  92
    }
        break;
    case CREATECURSOR:             {  //  93
    }
        break;
    case CREATEGLYPHCURSOR:        {  //  94
    }
        break;
    case FREECURSOR:               {  //  95
    }
        break;
    case RECOLORCURSOR:            {  //  96
    }
        break;
    case QUERYBESTSIZE:            {  //  97
    }
        break;
    case QUERYEXTENSION:           {  //  98
    }
        break;
    case LISTEXTENSIONS:           {  //  99
    }
        break;
    case CHANGEKEYBOARDMAPPING:    {  // 100
    }
        break;
    case GETKEYBOARDMAPPING:       {  // 101
    }
        break;
    case CHANGEKEYBOARDCONTROL:    {  // 102
    }
        break;
    case GETKEYBOARDCONTROL:       {  // 103
    }
        break;
    case BELL:                     {  // 104
    }
        break;
    case CHANGEPOINTERCONTROL:     {  // 105
    }
        break;
    case GETPOINTERCONTROL:        {  // 106
    }
        break;
    case SETSCREENSAVER:           {  // 107
    }
        break;
    case GETSCREENSAVER:           {  // 108
    }
        break;
    case CHANGEHOSTS:              {  // 109
    }
        break;
    case LISTHOSTS:                {  // 110
    }
        break;
    case SETACCESSCONTROL:         {  // 111
    }
        break;
    case SETCLOSEDOWNMODE:         {  // 112
    }
        break;
    case KILLCLIENT:               {  // 113
    }
        break;
    case ROTATEPROPERTIES:         {  // 114
    }
        break;
    case FORCESCREENSAVER:         {  // 115
    }
        break;
    case SETPOINTERMAPPING:        {  // 116
    }
        break;
    case GETPOINTERMAPPING:        {  // 117
    }
        break;
    case SETMODIFIERMAPPING:       {  // 118
    }
        break;
    case GETMODIFIERMAPPING:       {  // 119
    }
        break;
    case NOOPERATION:              {  // 127
    }
        break;
    default:
        break;
    }

    // Make sure commands are sent before we pause so that the window gets shown
    xcb_flush( connection );

    xcb_disconnect( connection );
}
