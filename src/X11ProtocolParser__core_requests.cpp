#include <unordered_map>
#include <cstdint>

#include "X11ProtocolParser.hpp"
#include "protocol/requests.hpp"


using namespace protocol::requests;

const std::unordered_map< uint8_t, X11ProtocolParser::_MajorOpcodeTraits >
X11ProtocolParser::_core_requests {
    { opcodes::CREATEWINDOW,             //   1
      _MajorOpcodeTraits( names.at( opcodes::CREATEWINDOW ),
                          &X11ProtocolParser::_parseRequest< CreateWindow > ) },
    { opcodes::CHANGEWINDOWATTRIBUTES,   //   2
      _MajorOpcodeTraits( names.at( opcodes::CHANGEWINDOWATTRIBUTES ),
                          &X11ProtocolParser::_parseRequest< ChangeWindowAttributes > ) },
    { opcodes::GETWINDOWATTRIBUTES,      //   3
      _MajorOpcodeTraits( names.at( opcodes::GETWINDOWATTRIBUTES ),
                          &X11ProtocolParser::_parseRequest< GetWindowAttributes >,
                          &X11ProtocolParser::_parseReply< GetWindowAttributes > ) },
    { opcodes::DESTROYWINDOW,            //   4
      _MajorOpcodeTraits( names.at( opcodes::DESTROYWINDOW ),
                          &X11ProtocolParser::_parseRequest< DestroyWindow > ) },
    { opcodes::DESTROYSUBWINDOWS,        //   5
      _MajorOpcodeTraits( names.at( opcodes::DESTROYSUBWINDOWS ),
                          &X11ProtocolParser::_parseRequest< DestroySubwindows > ) },
    { opcodes::CHANGESAVESET,            //   6
      _MajorOpcodeTraits( names.at( opcodes::CHANGESAVESET ),
                          &X11ProtocolParser::_parseRequest< ChangeSaveSet > ) },
    { opcodes::REPARENTWINDOW,           //   7
      _MajorOpcodeTraits( names.at( opcodes::REPARENTWINDOW ),
                          &X11ProtocolParser::_parseRequest< ReparentWindow > ) },
    { opcodes::MAPWINDOW,                //   8
      _MajorOpcodeTraits( names.at( opcodes::MAPWINDOW ),
                          &X11ProtocolParser::_parseRequest< MapWindow > ) },
    { opcodes::MAPSUBWINDOWS,            //   9
      _MajorOpcodeTraits( names.at( opcodes::MAPSUBWINDOWS ),
                          &X11ProtocolParser::_parseRequest< MapSubwindows > ) },
    { opcodes::UNMAPWINDOW,              //  10
      _MajorOpcodeTraits( names.at( opcodes::UNMAPWINDOW ),
                          &X11ProtocolParser::_parseRequest< UnmapWindow > ) },
    { opcodes::UNMAPSUBWINDOWS,          //  11
      _MajorOpcodeTraits( names.at( opcodes::UNMAPSUBWINDOWS ),
                          &X11ProtocolParser::_parseRequest< UnmapSubwindows > ) },
    { opcodes::CONFIGUREWINDOW,          //  12
      _MajorOpcodeTraits( names.at( opcodes::CONFIGUREWINDOW ),
                          &X11ProtocolParser::_parseRequest< ConfigureWindow > ) },
    { opcodes::CIRCULATEWINDOW,          //  13
      _MajorOpcodeTraits( names.at( opcodes::CIRCULATEWINDOW ),
                          &X11ProtocolParser::_parseRequest< CirculateWindow > ) },
    { opcodes::GETGEOMETRY,              //  14
      _MajorOpcodeTraits( names.at( opcodes::GETGEOMETRY ),
                          &X11ProtocolParser::_parseRequest< GetGeometry >,
                          &X11ProtocolParser::_parseReply< GetGeometry > ) },
    { opcodes::QUERYTREE,                //  15
      _MajorOpcodeTraits( names.at( opcodes::QUERYTREE ),
                          &X11ProtocolParser::_parseRequest< QueryTree >,
                          &X11ProtocolParser::_parseReply< QueryTree > ) },
    { opcodes::INTERNATOM,               //  16
      _MajorOpcodeTraits( names.at( opcodes::INTERNATOM ),
                          &X11ProtocolParser::_parseRequest< InternAtom >,
                          &X11ProtocolParser::_parseReply< InternAtom > ) },
    { opcodes::GETATOMNAME,              //  17
      _MajorOpcodeTraits( names.at( opcodes::GETATOMNAME ),
                          &X11ProtocolParser::_parseRequest< GetAtomName >,
                          &X11ProtocolParser::_parseReply< GetAtomName > ) },
    { opcodes::CHANGEPROPERTY,           //  18
      _MajorOpcodeTraits( names.at( opcodes::CHANGEPROPERTY ),
                          &X11ProtocolParser::_parseRequest< ChangeProperty > ) },
    { opcodes::DELETEPROPERTY,           //  19
      _MajorOpcodeTraits( names.at( opcodes::DELETEPROPERTY ),
                          &X11ProtocolParser::_parseRequest< DeleteProperty > ) },
    { opcodes::GETPROPERTY,              //  20
      _MajorOpcodeTraits( names.at( opcodes::GETPROPERTY ),
                          &X11ProtocolParser::_parseRequest< GetProperty >,
                          &X11ProtocolParser::_parseReply< GetProperty > ) },
    { opcodes::LISTPROPERTIES,           //  21
      _MajorOpcodeTraits( names.at( opcodes::LISTPROPERTIES ),
                          &X11ProtocolParser::_parseRequest< ListProperties >,
                          &X11ProtocolParser::_parseReply< ListProperties > ) },
    { opcodes::SETSELECTIONOWNER,        //  22
      _MajorOpcodeTraits( names.at( opcodes::SETSELECTIONOWNER ),
                          &X11ProtocolParser::_parseRequest< SetSelectionOwner > ) },
    { opcodes::GETSELECTIONOWNER,        //  23
      _MajorOpcodeTraits( names.at( opcodes::GETSELECTIONOWNER ),
                          &X11ProtocolParser::_parseRequest< GetSelectionOwner >,
                          &X11ProtocolParser::_parseReply< GetSelectionOwner > ) },
    { opcodes::CONVERTSELECTION,         //  24
      _MajorOpcodeTraits( names.at( opcodes::CONVERTSELECTION ),
                          &X11ProtocolParser::_parseRequest< ConvertSelection > ) },
    { opcodes::SENDEVENT,                //  25
      _MajorOpcodeTraits( names.at( opcodes::SENDEVENT ),
                          &X11ProtocolParser::_parseRequest< SendEvent > ) },
    { opcodes::GRABPOINTER,              //  26
      _MajorOpcodeTraits( names.at( opcodes::GRABPOINTER ),
                          &X11ProtocolParser::_parseRequest< GrabPointer >,
                          &X11ProtocolParser::_parseReply< GrabPointer > ) },
    { opcodes::UNGRABPOINTER,            //  27
      _MajorOpcodeTraits( names.at( opcodes::UNGRABPOINTER ),
                          &X11ProtocolParser::_parseRequest< UngrabPointer > ) },
    { opcodes::GRABBUTTON,               //  28
      _MajorOpcodeTraits( names.at( opcodes::GRABBUTTON ),
                          &X11ProtocolParser::_parseRequest< GrabButton > ) },
    { opcodes::UNGRABBUTTON,             //  29
      _MajorOpcodeTraits( names.at( opcodes::UNGRABBUTTON ),
                          &X11ProtocolParser::_parseRequest< UngrabButton > ) },
    { opcodes::CHANGEACTIVEPOINTERGRAB,  //  30
      _MajorOpcodeTraits( names.at( opcodes::CHANGEACTIVEPOINTERGRAB ),
                          &X11ProtocolParser::_parseRequest< ChangeActivePointerGrab > ) },
    { opcodes::GRABKEYBOARD,             //  31
      _MajorOpcodeTraits( names.at( opcodes::GRABKEYBOARD ),
                          &X11ProtocolParser::_parseRequest< GrabKeyboard >,
                          &X11ProtocolParser::_parseReply< GrabKeyboard > ) },
    { opcodes::UNGRABKEYBOARD,           //  32
      _MajorOpcodeTraits( names.at( opcodes::UNGRABKEYBOARD ),
                          &X11ProtocolParser::_parseRequest< UngrabKeyboard > ) },
    { opcodes::GRABKEY,                  //  33
      _MajorOpcodeTraits( names.at( opcodes::GRABKEY ),
                          &X11ProtocolParser::_parseRequest< GrabKey > ) },
    { opcodes::UNGRABKEY,                //  34
      _MajorOpcodeTraits( names.at( opcodes::UNGRABKEY ),
                          &X11ProtocolParser::_parseRequest< UngrabKey > ) },
    { opcodes::ALLOWEVENTS,              //  35
      _MajorOpcodeTraits( names.at( opcodes::ALLOWEVENTS ),
                          &X11ProtocolParser::_parseRequest< AllowEvents > ) },
    { opcodes::GRABSERVER,               //  36
      _MajorOpcodeTraits( names.at( opcodes::GRABSERVER ),
                          &X11ProtocolParser::_parseRequest< GrabServer > ) },
    { opcodes::UNGRABSERVER,             //  37
      _MajorOpcodeTraits( names.at( opcodes::UNGRABSERVER ),
                          &X11ProtocolParser::_parseRequest< UngrabServer > ) },
    { opcodes::QUERYPOINTER,             //  38
      _MajorOpcodeTraits( names.at( opcodes::QUERYPOINTER ),
                          &X11ProtocolParser::_parseRequest< QueryPointer >,
                          &X11ProtocolParser::_parseReply< QueryPointer > ) },
    { opcodes::GETMOTIONEVENTS,          //  39
      _MajorOpcodeTraits( names.at( opcodes::GETMOTIONEVENTS ),
                          &X11ProtocolParser::_parseRequest< GetMotionEvents >,
                          &X11ProtocolParser::_parseReply< GetMotionEvents > ) },
    { opcodes::TRANSLATECOORDINATES,     //  40
      _MajorOpcodeTraits( names.at( opcodes::TRANSLATECOORDINATES ),
                          &X11ProtocolParser::_parseRequest< TranslateCoordinates >,
                          &X11ProtocolParser::_parseReply< TranslateCoordinates > ) },
    { opcodes::WARPPOINTER,              //  41
      _MajorOpcodeTraits( names.at( opcodes::WARPPOINTER ),
                          &X11ProtocolParser::_parseRequest< WarpPointer > ) },
    { opcodes::SETINPUTFOCUS,            //  42
      _MajorOpcodeTraits( names.at( opcodes::SETINPUTFOCUS ),
                          &X11ProtocolParser::_parseRequest< SetInputFocus > ) },
    { opcodes::GETINPUTFOCUS,            //  43
      _MajorOpcodeTraits( names.at( opcodes::GETINPUTFOCUS ),
                          &X11ProtocolParser::_parseRequest< GetInputFocus >,
                          &X11ProtocolParser::_parseReply< GetInputFocus > ) },
    { opcodes::QUERYKEYMAP,              //  44
      _MajorOpcodeTraits( names.at( opcodes::QUERYKEYMAP ),
                          &X11ProtocolParser::_parseRequest< QueryKeymap >,
                          &X11ProtocolParser::_parseReply< QueryKeymap > ) },
    { opcodes::OPENFONT,                 //  45
      _MajorOpcodeTraits( names.at( opcodes::OPENFONT ),
                          &X11ProtocolParser::_parseRequest< OpenFont > ) },
    { opcodes::CLOSEFONT,                //  46
      _MajorOpcodeTraits( names.at( opcodes::CLOSEFONT ),
                          &X11ProtocolParser::_parseRequest< CloseFont > ) },
    { opcodes::QUERYFONT,                //  47
      _MajorOpcodeTraits( names.at( opcodes::QUERYFONT ),
                          &X11ProtocolParser::_parseRequest< QueryFont >,
                          &X11ProtocolParser::_parseReply< QueryFont > ) },
    { opcodes::QUERYTEXTEXTENTS,         //  48
      _MajorOpcodeTraits( names.at( opcodes::QUERYTEXTEXTENTS ),
                          &X11ProtocolParser::_parseRequest< QueryTextExtents >,
                          &X11ProtocolParser::_parseReply< QueryTextExtents > ) },
    { opcodes::LISTFONTS,                //  49
      _MajorOpcodeTraits( names.at( opcodes::LISTFONTS ),
                          &X11ProtocolParser::_parseRequest< ListFonts >,
                          &X11ProtocolParser::_parseReply< ListFonts > ) },
    { opcodes::LISTFONTSWITHINFO,        //  50
      _MajorOpcodeTraits( names.at( opcodes::LISTFONTSWITHINFO ),
                          &X11ProtocolParser::_parseRequest< ListFontsWithInfo >,
                          &X11ProtocolParser::_parseReply< ListFontsWithInfo > ) },
    { opcodes::SETFONTPATH,              //  51
      _MajorOpcodeTraits( names.at( opcodes::SETFONTPATH ),
                          &X11ProtocolParser::_parseRequest< SetFontPath > ) },
    { opcodes::GETFONTPATH,              //  52
      _MajorOpcodeTraits( names.at( opcodes::GETFONTPATH ),
                          &X11ProtocolParser::_parseRequest< GetFontPath >,
                          &X11ProtocolParser::_parseReply< GetFontPath > ) },
    { opcodes::CREATEPIXMAP,             //  53
      _MajorOpcodeTraits( names.at( opcodes::CREATEPIXMAP ),
                          &X11ProtocolParser::_parseRequest< CreatePixmap > ) },
    { opcodes::FREEPIXMAP,               //  54
      _MajorOpcodeTraits( names.at( opcodes::FREEPIXMAP ),
                          &X11ProtocolParser::_parseRequest< FreePixmap > ) },
    { opcodes::CREATEGC,                 //  55
      _MajorOpcodeTraits( names.at( opcodes::CREATEGC ),
                          &X11ProtocolParser::_parseRequest< CreateGC > ) },
    { opcodes::CHANGEGC,                 //  56
      _MajorOpcodeTraits( names.at( opcodes::CHANGEGC ),
                          &X11ProtocolParser::_parseRequest< ChangeGC > ) },
    { opcodes::COPYGC,                   //  57
      _MajorOpcodeTraits( names.at( opcodes::COPYGC ),
                          &X11ProtocolParser::_parseRequest< CopyGC > ) },
    { opcodes::SETDASHES,                //  58
      _MajorOpcodeTraits( names.at( opcodes::SETDASHES ),
                          &X11ProtocolParser::_parseRequest< SetDashes > ) },
    { opcodes::SETCLIPRECTANGLES,        //  59
      _MajorOpcodeTraits( names.at( opcodes::SETCLIPRECTANGLES ),
                          &X11ProtocolParser::_parseRequest< SetClipRectangles > ) },
    { opcodes::FREEGC,                   //  60
      _MajorOpcodeTraits( names.at( opcodes::FREEGC ),
                          &X11ProtocolParser::_parseRequest< FreeGC > ) },
    { opcodes::CLEARAREA,                //  61
      _MajorOpcodeTraits( names.at( opcodes::CLEARAREA ),
                          &X11ProtocolParser::_parseRequest< ClearArea > ) },
    { opcodes::COPYAREA,                 //  62
      _MajorOpcodeTraits( names.at( opcodes::COPYAREA ),
                          &X11ProtocolParser::_parseRequest< CopyArea > ) },
    { opcodes::COPYPLANE,                //  63
      _MajorOpcodeTraits( names.at( opcodes::COPYPLANE ),
                          &X11ProtocolParser::_parseRequest< CopyPlane > ) },
    { opcodes::POLYPOINT,                //  64
      _MajorOpcodeTraits( names.at( opcodes::POLYPOINT ),
                          &X11ProtocolParser::_parseRequest< PolyPoint > ) },
    { opcodes::POLYLINE,                 //  65
      _MajorOpcodeTraits( names.at( opcodes::POLYLINE ),
                          &X11ProtocolParser::_parseRequest< PolyLine > ) },
    { opcodes::POLYSEGMENT,              //  66
      _MajorOpcodeTraits( names.at( opcodes::POLYSEGMENT ),
                          &X11ProtocolParser::_parseRequest< PolySegment > ) },
    { opcodes::POLYRECTANGLE,            //  67
      _MajorOpcodeTraits( names.at( opcodes::POLYRECTANGLE ),
                          &X11ProtocolParser::_parseRequest< PolyRectangle > ) },
    { opcodes::POLYARC,                  //  68
      _MajorOpcodeTraits( names.at( opcodes::POLYARC ),
                          &X11ProtocolParser::_parseRequest< PolyArc > ) },
    { opcodes::FILLPOLY,                 //  69
      _MajorOpcodeTraits( names.at( opcodes::FILLPOLY ),
                          &X11ProtocolParser::_parseRequest< FillPoly > ) },
    { opcodes::POLYFILLRECTANGLE,        //  70
      _MajorOpcodeTraits( names.at( opcodes::POLYFILLRECTANGLE ),
                          &X11ProtocolParser::_parseRequest< PolyFillRectangle > ) },
    { opcodes::POLYFILLARC,              //  71
      _MajorOpcodeTraits( names.at( opcodes::POLYFILLARC ),
                          &X11ProtocolParser::_parseRequest< PolyFillArc > ) },
    { opcodes::PUTIMAGE,                 //  72
      _MajorOpcodeTraits( names.at( opcodes::PUTIMAGE ),
                          &X11ProtocolParser::_parseRequest< PutImage > ) },
    { opcodes::GETIMAGE,                 //  73
      _MajorOpcodeTraits( names.at( opcodes::GETIMAGE ),
                          &X11ProtocolParser::_parseRequest< GetImage >,
                          &X11ProtocolParser::_parseReply< GetImage > ) },
    { opcodes::POLYTEXT8,                //  74
      _MajorOpcodeTraits( names.at( opcodes::POLYTEXT8 ),
                          &X11ProtocolParser::_parseRequest< PolyText8 > ) },
    { opcodes::POLYTEXT16,               //  75
      _MajorOpcodeTraits( names.at( opcodes::POLYTEXT16 ),
                          &X11ProtocolParser::_parseRequest< PolyText16 > ) },
    { opcodes::IMAGETEXT8,               //  76
      _MajorOpcodeTraits( names.at( opcodes::IMAGETEXT8 ),
                          &X11ProtocolParser::_parseRequest< ImageText8 > ) },
    { opcodes::IMAGETEXT16,              //  77
      _MajorOpcodeTraits( names.at( opcodes::IMAGETEXT16 ),
                          &X11ProtocolParser::_parseRequest< ImageText16 > ) },
    { opcodes::CREATECOLORMAP,           //  78
      _MajorOpcodeTraits( names.at( opcodes::CREATECOLORMAP ),
                          &X11ProtocolParser::_parseRequest< CreateColormap > ) },
    { opcodes::FREECOLORMAP,             //  79
      _MajorOpcodeTraits( names.at( opcodes::FREECOLORMAP ),
                          &X11ProtocolParser::_parseRequest< FreeColormap > ) },
    { opcodes::COPYCOLORMAPANDFREE,      //  80
      _MajorOpcodeTraits( names.at( opcodes::COPYCOLORMAPANDFREE ),
                          &X11ProtocolParser::_parseRequest< CopyColormapAndFree > ) },
    { opcodes::INSTALLCOLORMAP,          //  81
      _MajorOpcodeTraits( names.at( opcodes::INSTALLCOLORMAP ),
                          &X11ProtocolParser::_parseRequest< InstallColormap > ) },
    { opcodes::UNINSTALLCOLORMAP,        //  82
      _MajorOpcodeTraits( names.at( opcodes::UNINSTALLCOLORMAP ),
                          &X11ProtocolParser::_parseRequest< UninstallColormap > ) },
    { opcodes::LISTINSTALLEDCOLORMAPS,   //  83
      _MajorOpcodeTraits( names.at( opcodes::LISTINSTALLEDCOLORMAPS ),
                          &X11ProtocolParser::_parseRequest< ListInstalledColormaps >,
                          &X11ProtocolParser::_parseReply< ListInstalledColormaps > ) },
    { opcodes::ALLOCCOLOR,               //  84
      _MajorOpcodeTraits( names.at( opcodes::ALLOCCOLOR ),
                          &X11ProtocolParser::_parseRequest< AllocColor >,
                          &X11ProtocolParser::_parseReply< AllocColor > ) },
    { opcodes::ALLOCNAMEDCOLOR,          //  85
      _MajorOpcodeTraits( names.at( opcodes::ALLOCNAMEDCOLOR ),
                          &X11ProtocolParser::_parseRequest< AllocNamedColor >,
                          &X11ProtocolParser::_parseReply< AllocNamedColor > ) },
    { opcodes::ALLOCCOLORCELLS,          //  86
      _MajorOpcodeTraits( names.at( opcodes::ALLOCCOLORCELLS ),
                          &X11ProtocolParser::_parseRequest< AllocColorCells >,
                          &X11ProtocolParser::_parseReply< AllocColorCells > ) },
    { opcodes::ALLOCCOLORPLANES,         //  87
      _MajorOpcodeTraits( names.at( opcodes::ALLOCCOLORPLANES ),
                          &X11ProtocolParser::_parseRequest< AllocColorPlanes >,
                          &X11ProtocolParser::_parseReply< AllocColorPlanes > ) },
    { opcodes::FREECOLORS,               //  88
      _MajorOpcodeTraits( names.at( opcodes::FREECOLORS ),
                          &X11ProtocolParser::_parseRequest< FreeColors > ) },
    { opcodes::STORECOLORS,              //  89
      _MajorOpcodeTraits( names.at( opcodes::STORECOLORS ),
                          &X11ProtocolParser::_parseRequest< StoreColors > ) },
    { opcodes::STORENAMEDCOLOR,          //  90
      _MajorOpcodeTraits( names.at( opcodes::STORENAMEDCOLOR ),
                          &X11ProtocolParser::_parseRequest< StoreNamedColor > ) },
    { opcodes::QUERYCOLORS,              //  91
      _MajorOpcodeTraits( names.at( opcodes::QUERYCOLORS ),
                          &X11ProtocolParser::_parseRequest< QueryColors >,
                          &X11ProtocolParser::_parseReply< QueryColors > ) },
    { opcodes::LOOKUPCOLOR,              //  92
      _MajorOpcodeTraits( names.at( opcodes::LOOKUPCOLOR ),
                          &X11ProtocolParser::_parseRequest< LookupColor >,
                          &X11ProtocolParser::_parseReply< LookupColor > ) },
    { opcodes::CREATECURSOR,             //  93
      _MajorOpcodeTraits( names.at( opcodes::CREATECURSOR ),
                          &X11ProtocolParser::_parseRequest< CreateCursor > ) },
    { opcodes::CREATEGLYPHCURSOR,        //  94
      _MajorOpcodeTraits( names.at( opcodes::CREATEGLYPHCURSOR ),
                          &X11ProtocolParser::_parseRequest< CreateGlyphCursor > ) },
    { opcodes::FREECURSOR,               //  95
      _MajorOpcodeTraits( names.at( opcodes::FREECURSOR ),
                          &X11ProtocolParser::_parseRequest< FreeCursor > ) },
    { opcodes::RECOLORCURSOR,            //  96
      _MajorOpcodeTraits( names.at( opcodes::RECOLORCURSOR ),
                          &X11ProtocolParser::_parseRequest< RecolorCursor > ) },
    { opcodes::QUERYBESTSIZE,            //  97
      _MajorOpcodeTraits( names.at( opcodes::QUERYBESTSIZE ),
                          &X11ProtocolParser::_parseRequest< QueryBestSize >,
                          &X11ProtocolParser::_parseReply< QueryBestSize > ) },
    { opcodes::QUERYEXTENSION,           //  98
      _MajorOpcodeTraits( names.at( opcodes::QUERYEXTENSION ),
                          &X11ProtocolParser::_parseRequest< QueryExtension >,
                          &X11ProtocolParser::_parseReply< QueryExtension > ) },
    { opcodes::LISTEXTENSIONS,           //  99
      _MajorOpcodeTraits( names.at( opcodes::LISTEXTENSIONS ),
                          &X11ProtocolParser::_parseRequest< ListExtensions >,
                          &X11ProtocolParser::_parseReply< ListExtensions > ) },
    { opcodes::CHANGEKEYBOARDMAPPING,    // 100
      _MajorOpcodeTraits( names.at( opcodes::CHANGEKEYBOARDMAPPING ),
                          &X11ProtocolParser::_parseRequest< ChangeKeyboardMapping > ) },
    { opcodes::GETKEYBOARDMAPPING,       // 101
      _MajorOpcodeTraits( names.at( opcodes::GETKEYBOARDMAPPING ),
                          &X11ProtocolParser::_parseRequest< GetKeyboardMapping >,
                          &X11ProtocolParser::_parseReply< GetKeyboardMapping > ) },
    { opcodes::CHANGEKEYBOARDCONTROL,    // 102
      _MajorOpcodeTraits( names.at( opcodes::CHANGEKEYBOARDCONTROL ),
                          &X11ProtocolParser::_parseRequest< ChangeKeyboardControl > ) },
    { opcodes::GETKEYBOARDCONTROL,       // 103
      _MajorOpcodeTraits( names.at( opcodes::GETKEYBOARDCONTROL ),
                          &X11ProtocolParser::_parseRequest< GetKeyboardControl >,
                          &X11ProtocolParser::_parseReply< GetKeyboardControl > ) },
    { opcodes::BELL,                     // 104
      _MajorOpcodeTraits( names.at( opcodes::BELL ),
                          &X11ProtocolParser::_parseRequest< Bell > ) },
    { opcodes::CHANGEPOINTERCONTROL,     // 105
      _MajorOpcodeTraits( names.at( opcodes::CHANGEPOINTERCONTROL ),
                          &X11ProtocolParser::_parseRequest< ChangePointerControl > ) },
    { opcodes::GETPOINTERCONTROL,        // 106
      _MajorOpcodeTraits( names.at( opcodes::GETPOINTERCONTROL ),
                          &X11ProtocolParser::_parseRequest< GetPointerControl >,
                          &X11ProtocolParser::_parseReply< GetPointerControl > ) },
    { opcodes::SETSCREENSAVER,           // 107
      _MajorOpcodeTraits( names.at( opcodes::SETSCREENSAVER ),
                          &X11ProtocolParser::_parseRequest< SetScreenSaver > ) },
    { opcodes::GETSCREENSAVER,           // 108
      _MajorOpcodeTraits( names.at( opcodes::GETSCREENSAVER ),
                          &X11ProtocolParser::_parseRequest< GetScreenSaver >,
                          &X11ProtocolParser::_parseReply< GetScreenSaver > ) },
    { opcodes::CHANGEHOSTS,              // 109
      _MajorOpcodeTraits( names.at( opcodes::CHANGEHOSTS ),
                          &X11ProtocolParser::_parseRequest< ChangeHosts > ) },
    { opcodes::LISTHOSTS,                // 110
      _MajorOpcodeTraits( names.at( opcodes::LISTHOSTS ),
                          &X11ProtocolParser::_parseRequest< ListHosts >,
                          &X11ProtocolParser::_parseReply< ListHosts > ) },
    { opcodes::SETACCESSCONTROL,         // 111
      _MajorOpcodeTraits( names.at( opcodes::SETACCESSCONTROL ),
                          &X11ProtocolParser::_parseRequest< SetAccessControl > ) },
    { opcodes::SETCLOSEDOWNMODE,         // 112
      _MajorOpcodeTraits( names.at( opcodes::SETCLOSEDOWNMODE ),
                          &X11ProtocolParser::_parseRequest< SetCloseDownMode > ) },
    { opcodes::KILLCLIENT,               // 113
      _MajorOpcodeTraits( names.at( opcodes::KILLCLIENT ),
                          &X11ProtocolParser::_parseRequest< KillClient > ) },
    { opcodes::ROTATEPROPERTIES,         // 114
      _MajorOpcodeTraits( names.at( opcodes::ROTATEPROPERTIES ),
                          &X11ProtocolParser::_parseRequest< RotateProperties > ) },
    { opcodes::FORCESCREENSAVER,         // 115
      _MajorOpcodeTraits( names.at( opcodes::FORCESCREENSAVER ),
                          &X11ProtocolParser::_parseRequest< ForceScreenSaver > ) },
    { opcodes::SETPOINTERMAPPING,        // 116
      _MajorOpcodeTraits( names.at( opcodes::SETPOINTERMAPPING ),
                          &X11ProtocolParser::_parseRequest< SetPointerMapping >,
                          &X11ProtocolParser::_parseReply< SetPointerMapping > ) },
    { opcodes::GETPOINTERMAPPING,        // 117
      _MajorOpcodeTraits( names.at( opcodes::GETPOINTERMAPPING ),
                          &X11ProtocolParser::_parseRequest< GetPointerMapping >,
                          &X11ProtocolParser::_parseReply< GetPointerMapping > ) },
    { opcodes::SETMODIFIERMAPPING,       // 118
      _MajorOpcodeTraits( names.at( opcodes::SETMODIFIERMAPPING ),
                          &X11ProtocolParser::_parseRequest< SetModifierMapping >,
                          &X11ProtocolParser::_parseReply< SetModifierMapping > ) },
    { opcodes::GETMODIFIERMAPPING,       // 119
      _MajorOpcodeTraits( names.at( opcodes::GETMODIFIERMAPPING ),
                          &X11ProtocolParser::_parseRequest< GetModifierMapping >,
                          &X11ProtocolParser::_parseReply< GetModifierMapping > ) },
    { opcodes::NOOPERATION,              // 127
      _MajorOpcodeTraits( names.at( opcodes::NOOPERATION ),
                          &X11ProtocolParser::_parseRequest< NoOperation > ) }
};
