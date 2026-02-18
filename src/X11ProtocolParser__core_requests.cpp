#include <unordered_map>

#include <cstdint>

#include "X11ProtocolParser.hpp"

#include "protocol/requests.hpp"


namespace rq = protocol::requests;

const std::unordered_map< uint8_t, X11ProtocolParser::_MajorOpcodeTraits >
X11ProtocolParser::_core_requests {
    { rq::opcodes::CREATEWINDOW,             //   1
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CREATEWINDOW ),
                          &X11ProtocolParser::_parseRequest< rq::CreateWindow > ) },
    { rq::opcodes::CHANGEWINDOWATTRIBUTES,   //   2
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CHANGEWINDOWATTRIBUTES ),
                          &X11ProtocolParser::_parseRequest< rq::ChangeWindowAttributes > ) },
    { rq::opcodes::GETWINDOWATTRIBUTES,      //   3
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETWINDOWATTRIBUTES ),
                          &X11ProtocolParser::_parseRequest< rq::GetWindowAttributes >,
                          &X11ProtocolParser::_parseReply< rq::GetWindowAttributes > ) },
    { rq::opcodes::DESTROYWINDOW,            //   4
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::DESTROYWINDOW ),
                          &X11ProtocolParser::_parseRequest< rq::DestroyWindow > ) },
    { rq::opcodes::DESTROYSUBWINDOWS,        //   5
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::DESTROYSUBWINDOWS ),
                          &X11ProtocolParser::_parseRequest< rq::DestroySubwindows > ) },
    { rq::opcodes::CHANGESAVESET,            //   6
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CHANGESAVESET ),
                          &X11ProtocolParser::_parseRequest< rq::ChangeSaveSet > ) },
    { rq::opcodes::REPARENTWINDOW,           //   7
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::REPARENTWINDOW ),
                          &X11ProtocolParser::_parseRequest< rq::ReparentWindow > ) },
    { rq::opcodes::MAPWINDOW,                //   8
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::MAPWINDOW ),
                          &X11ProtocolParser::_parseRequest< rq::MapWindow > ) },
    { rq::opcodes::MAPSUBWINDOWS,            //   9
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::MAPSUBWINDOWS ),
                          &X11ProtocolParser::_parseRequest< rq::MapSubwindows > ) },
    { rq::opcodes::UNMAPWINDOW,              //  10
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::UNMAPWINDOW ),
                          &X11ProtocolParser::_parseRequest< rq::UnmapWindow > ) },
    { rq::opcodes::UNMAPSUBWINDOWS,          //  11
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::UNMAPSUBWINDOWS ),
                          &X11ProtocolParser::_parseRequest< rq::UnmapSubwindows > ) },
    { rq::opcodes::CONFIGUREWINDOW,          //  12
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CONFIGUREWINDOW ),
                          &X11ProtocolParser::_parseRequest< rq::ConfigureWindow > ) },
    { rq::opcodes::CIRCULATEWINDOW,          //  13
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CIRCULATEWINDOW ),
                          &X11ProtocolParser::_parseRequest< rq::CirculateWindow > ) },
    { rq::opcodes::GETGEOMETRY,              //  14
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETGEOMETRY ),
                          &X11ProtocolParser::_parseRequest< rq::GetGeometry >,
                          &X11ProtocolParser::_parseReply< rq::GetGeometry > ) },
    { rq::opcodes::QUERYTREE,                //  15
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::QUERYTREE ),
                          &X11ProtocolParser::_parseRequest< rq::QueryTree >,
                          &X11ProtocolParser::_parseReply< rq::QueryTree > ) },
    { rq::opcodes::INTERNATOM,               //  16
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::INTERNATOM ),
                          &X11ProtocolParser::_parseRequest< rq::InternAtom >,
                          &X11ProtocolParser::_parseReply< rq::InternAtom > ) },
    { rq::opcodes::GETATOMNAME,              //  17
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETATOMNAME ),
                          &X11ProtocolParser::_parseRequest< rq::GetAtomName >,
                          &X11ProtocolParser::_parseReply< rq::GetAtomName > ) },
    { rq::opcodes::CHANGEPROPERTY,           //  18
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CHANGEPROPERTY ),
                          &X11ProtocolParser::_parseRequest< rq::ChangeProperty > ) },
    { rq::opcodes::DELETEPROPERTY,           //  19
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::DELETEPROPERTY ),
                          &X11ProtocolParser::_parseRequest< rq::DeleteProperty > ) },
    { rq::opcodes::GETPROPERTY,              //  20
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETPROPERTY ),
                          &X11ProtocolParser::_parseRequest< rq::GetProperty >,
                          &X11ProtocolParser::_parseReply< rq::GetProperty > ) },
    { rq::opcodes::LISTPROPERTIES,           //  21
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::LISTPROPERTIES ),
                          &X11ProtocolParser::_parseRequest< rq::ListProperties >,
                          &X11ProtocolParser::_parseReply< rq::ListProperties > ) },
    { rq::opcodes::SETSELECTIONOWNER,        //  22
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::SETSELECTIONOWNER ),
                          &X11ProtocolParser::_parseRequest< rq::SetSelectionOwner > ) },
    { rq::opcodes::GETSELECTIONOWNER,        //  23
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETSELECTIONOWNER ),
                          &X11ProtocolParser::_parseRequest< rq::GetSelectionOwner >,
                          &X11ProtocolParser::_parseReply< rq::GetSelectionOwner > ) },
    { rq::opcodes::CONVERTSELECTION,         //  24
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CONVERTSELECTION ),
                          &X11ProtocolParser::_parseRequest< rq::ConvertSelection > ) },
    { rq::opcodes::SENDEVENT,                //  25
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::SENDEVENT ),
                          &X11ProtocolParser::_parseRequest< rq::SendEvent > ) },
    { rq::opcodes::GRABPOINTER,              //  26
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GRABPOINTER ),
                          &X11ProtocolParser::_parseRequest< rq::GrabPointer >,
                          &X11ProtocolParser::_parseReply< rq::GrabPointer > ) },
    { rq::opcodes::UNGRABPOINTER,            //  27
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::UNGRABPOINTER ),
                          &X11ProtocolParser::_parseRequest< rq::UngrabPointer > ) },
    { rq::opcodes::GRABBUTTON,               //  28
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GRABBUTTON ),
                          &X11ProtocolParser::_parseRequest< rq::GrabButton > ) },
    { rq::opcodes::UNGRABBUTTON,             //  29
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::UNGRABBUTTON ),
                          &X11ProtocolParser::_parseRequest< rq::UngrabButton > ) },
    { rq::opcodes::CHANGEACTIVEPOINTERGRAB,  //  30
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CHANGEACTIVEPOINTERGRAB ),
                          &X11ProtocolParser::_parseRequest< rq::ChangeActivePointerGrab > ) },
    { rq::opcodes::GRABKEYBOARD,             //  31
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GRABKEYBOARD ),
                          &X11ProtocolParser::_parseRequest< rq::GrabKeyboard >,
                          &X11ProtocolParser::_parseReply< rq::GrabKeyboard > ) },
    { rq::opcodes::UNGRABKEYBOARD,           //  32
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::UNGRABKEYBOARD ),
                          &X11ProtocolParser::_parseRequest< rq::UngrabKeyboard > ) },
    { rq::opcodes::GRABKEY,                  //  33
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GRABKEY ),
                          &X11ProtocolParser::_parseRequest< rq::GrabKey > ) },
    { rq::opcodes::UNGRABKEY,                //  34
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::UNGRABKEY ),
                          &X11ProtocolParser::_parseRequest< rq::UngrabKey > ) },
    { rq::opcodes::ALLOWEVENTS,              //  35
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::ALLOWEVENTS ),
                          &X11ProtocolParser::_parseRequest< rq::AllowEvents > ) },
    { rq::opcodes::GRABSERVER,               //  36
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GRABSERVER ),
                          &X11ProtocolParser::_parseRequest< rq::GrabServer > ) },
    { rq::opcodes::UNGRABSERVER,             //  37
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::UNGRABSERVER ),
                          &X11ProtocolParser::_parseRequest< rq::UngrabServer > ) },
    { rq::opcodes::QUERYPOINTER,             //  38
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::QUERYPOINTER ),
                          &X11ProtocolParser::_parseRequest< rq::QueryPointer >,
                          &X11ProtocolParser::_parseReply< rq::QueryPointer > ) },
    { rq::opcodes::GETMOTIONEVENTS,          //  39
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETMOTIONEVENTS ),
                          &X11ProtocolParser::_parseRequest< rq::GetMotionEvents >,
                          &X11ProtocolParser::_parseReply< rq::GetMotionEvents > ) },
    { rq::opcodes::TRANSLATECOORDINATES,     //  40
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::TRANSLATECOORDINATES ),
                          &X11ProtocolParser::_parseRequest< rq::TranslateCoordinates >,
                          &X11ProtocolParser::_parseReply< rq::TranslateCoordinates > ) },
    { rq::opcodes::WARPPOINTER,              //  41
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::WARPPOINTER ),
                          &X11ProtocolParser::_parseRequest< rq::WarpPointer > ) },
    { rq::opcodes::SETINPUTFOCUS,            //  42
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::SETINPUTFOCUS ),
                          &X11ProtocolParser::_parseRequest< rq::SetInputFocus > ) },
    { rq::opcodes::GETINPUTFOCUS,            //  43
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETINPUTFOCUS ),
                          &X11ProtocolParser::_parseRequest< rq::GetInputFocus >,
                          &X11ProtocolParser::_parseReply< rq::GetInputFocus > ) },
    { rq::opcodes::QUERYKEYMAP,              //  44
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::QUERYKEYMAP ),
                          &X11ProtocolParser::_parseRequest< rq::QueryKeymap >,
                          &X11ProtocolParser::_parseReply< rq::QueryKeymap > ) },
    { rq::opcodes::OPENFONT,                 //  45
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::OPENFONT ),
                          &X11ProtocolParser::_parseRequest< rq::OpenFont > ) },
    { rq::opcodes::CLOSEFONT,                //  46
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CLOSEFONT ),
                          &X11ProtocolParser::_parseRequest< rq::CloseFont > ) },
    { rq::opcodes::QUERYFONT,                //  47
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::QUERYFONT ),
                          &X11ProtocolParser::_parseRequest< rq::QueryFont >,
                          &X11ProtocolParser::_parseReply< rq::QueryFont > ) },
    { rq::opcodes::QUERYTEXTEXTENTS,         //  48
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::QUERYTEXTEXTENTS ),
                          &X11ProtocolParser::_parseRequest< rq::QueryTextExtents >,
                          &X11ProtocolParser::_parseReply< rq::QueryTextExtents > ) },
    { rq::opcodes::LISTFONTS,                //  49
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::LISTFONTS ),
                          &X11ProtocolParser::_parseRequest< rq::ListFonts >,
                          &X11ProtocolParser::_parseReply< rq::ListFonts > ) },
    { rq::opcodes::LISTFONTSWITHINFO,        //  50
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::LISTFONTSWITHINFO ),
                          &X11ProtocolParser::_parseRequest< rq::ListFontsWithInfo >,
                          &X11ProtocolParser::_parseReply< rq::ListFontsWithInfo > ) },
    { rq::opcodes::SETFONTPATH,              //  51
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::SETFONTPATH ),
                          &X11ProtocolParser::_parseRequest< rq::SetFontPath > ) },
    { rq::opcodes::GETFONTPATH,              //  52
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETFONTPATH ),
                          &X11ProtocolParser::_parseRequest< rq::GetFontPath >,
                          &X11ProtocolParser::_parseReply< rq::GetFontPath > ) },
    { rq::opcodes::CREATEPIXMAP,             //  53
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CREATEPIXMAP ),
                          &X11ProtocolParser::_parseRequest< rq::CreatePixmap > ) },
    { rq::opcodes::FREEPIXMAP,               //  54
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::FREEPIXMAP ),
                          &X11ProtocolParser::_parseRequest< rq::FreePixmap > ) },
    { rq::opcodes::CREATEGC,                 //  55
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CREATEGC ),
                          &X11ProtocolParser::_parseRequest< rq::CreateGC > ) },
    { rq::opcodes::CHANGEGC,                 //  56
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CHANGEGC ),
                          &X11ProtocolParser::_parseRequest< rq::ChangeGC > ) },
    { rq::opcodes::COPYGC,                   //  57
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::COPYGC ),
                          &X11ProtocolParser::_parseRequest< rq::CopyGC > ) },
    { rq::opcodes::SETDASHES,                //  58
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::SETDASHES ),
                          &X11ProtocolParser::_parseRequest< rq::SetDashes > ) },
    { rq::opcodes::SETCLIPRECTANGLES,        //  59
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::SETCLIPRECTANGLES ),
                          &X11ProtocolParser::_parseRequest< rq::SetClipRectangles > ) },
    { rq::opcodes::FREEGC,                   //  60
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::FREEGC ),
                          &X11ProtocolParser::_parseRequest< rq::FreeGC > ) },
    { rq::opcodes::CLEARAREA,                //  61
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CLEARAREA ),
                          &X11ProtocolParser::_parseRequest< rq::ClearArea > ) },
    { rq::opcodes::COPYAREA,                 //  62
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::COPYAREA ),
                          &X11ProtocolParser::_parseRequest< rq::CopyArea > ) },
    { rq::opcodes::COPYPLANE,                //  63
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::COPYPLANE ),
                          &X11ProtocolParser::_parseRequest< rq::CopyPlane > ) },
    { rq::opcodes::POLYPOINT,                //  64
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::POLYPOINT ),
                          &X11ProtocolParser::_parseRequest< rq::PolyPoint > ) },
    { rq::opcodes::POLYLINE,                 //  65
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::POLYLINE ),
                          &X11ProtocolParser::_parseRequest< rq::PolyLine > ) },
    { rq::opcodes::POLYSEGMENT,              //  66
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::POLYSEGMENT ),
                          &X11ProtocolParser::_parseRequest< rq::PolySegment > ) },
    { rq::opcodes::POLYRECTANGLE,            //  67
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::POLYRECTANGLE ),
                          &X11ProtocolParser::_parseRequest< rq::PolyRectangle > ) },
    { rq::opcodes::POLYARC,                  //  68
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::POLYARC ),
                          &X11ProtocolParser::_parseRequest< rq::PolyArc > ) },
    { rq::opcodes::FILLPOLY,                 //  69
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::FILLPOLY ),
                          &X11ProtocolParser::_parseRequest< rq::FillPoly > ) },
    { rq::opcodes::POLYFILLRECTANGLE,        //  70
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::POLYFILLRECTANGLE ),
                          &X11ProtocolParser::_parseRequest< rq::PolyFillRectangle > ) },
    { rq::opcodes::POLYFILLARC,              //  71
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::POLYFILLARC ),
                          &X11ProtocolParser::_parseRequest< rq::PolyFillArc > ) },
    { rq::opcodes::PUTIMAGE,                 //  72
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::PUTIMAGE ),
                          &X11ProtocolParser::_parseRequest< rq::PutImage > ) },
    { rq::opcodes::GETIMAGE,                 //  73
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETIMAGE ),
                          &X11ProtocolParser::_parseRequest< rq::GetImage >,
                          &X11ProtocolParser::_parseReply< rq::GetImage > ) },
    { rq::opcodes::POLYTEXT8,                //  74
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::POLYTEXT8 ),
                          &X11ProtocolParser::_parseRequest< rq::PolyText8 > ) },
    { rq::opcodes::POLYTEXT16,               //  75
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::POLYTEXT16 ),
                          &X11ProtocolParser::_parseRequest< rq::PolyText16 > ) },
    { rq::opcodes::IMAGETEXT8,               //  76
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::IMAGETEXT8 ),
                          &X11ProtocolParser::_parseRequest< rq::ImageText8 > ) },
    { rq::opcodes::IMAGETEXT16,              //  77
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::IMAGETEXT16 ),
                          &X11ProtocolParser::_parseRequest< rq::ImageText16 > ) },
    { rq::opcodes::CREATECOLORMAP,           //  78
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CREATECOLORMAP ),
                          &X11ProtocolParser::_parseRequest< rq::CreateColormap > ) },
    { rq::opcodes::FREECOLORMAP,             //  79
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::FREECOLORMAP ),
                          &X11ProtocolParser::_parseRequest< rq::FreeColormap > ) },
    { rq::opcodes::COPYCOLORMAPANDFREE,      //  80
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::COPYCOLORMAPANDFREE ),
                          &X11ProtocolParser::_parseRequest< rq::CopyColormapAndFree > ) },
    { rq::opcodes::INSTALLCOLORMAP,          //  81
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::INSTALLCOLORMAP ),
                          &X11ProtocolParser::_parseRequest< rq::InstallColormap > ) },
    { rq::opcodes::UNINSTALLCOLORMAP,        //  82
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::UNINSTALLCOLORMAP ),
                          &X11ProtocolParser::_parseRequest< rq::UninstallColormap > ) },
    { rq::opcodes::LISTINSTALLEDCOLORMAPS,   //  83
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::LISTINSTALLEDCOLORMAPS ),
                          &X11ProtocolParser::_parseRequest< rq::ListInstalledColormaps >,
                          &X11ProtocolParser::_parseReply< rq::ListInstalledColormaps > ) },
    { rq::opcodes::ALLOCCOLOR,               //  84
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::ALLOCCOLOR ),
                          &X11ProtocolParser::_parseRequest< rq::AllocColor >,
                          &X11ProtocolParser::_parseReply< rq::AllocColor > ) },
    { rq::opcodes::ALLOCNAMEDCOLOR,          //  85
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::ALLOCNAMEDCOLOR ),
                          &X11ProtocolParser::_parseRequest< rq::AllocNamedColor >,
                          &X11ProtocolParser::_parseReply< rq::AllocNamedColor > ) },
    { rq::opcodes::ALLOCCOLORCELLS,          //  86
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::ALLOCCOLORCELLS ),
                          &X11ProtocolParser::_parseRequest< rq::AllocColorCells >,
                          &X11ProtocolParser::_parseReply< rq::AllocColorCells > ) },
    { rq::opcodes::ALLOCCOLORPLANES,         //  87
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::ALLOCCOLORPLANES ),
                          &X11ProtocolParser::_parseRequest< rq::AllocColorPlanes >,
                          &X11ProtocolParser::_parseReply< rq::AllocColorPlanes > ) },
    { rq::opcodes::FREECOLORS,               //  88
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::FREECOLORS ),
                          &X11ProtocolParser::_parseRequest< rq::FreeColors > ) },
    { rq::opcodes::STORECOLORS,              //  89
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::STORECOLORS ),
                          &X11ProtocolParser::_parseRequest< rq::StoreColors > ) },
    { rq::opcodes::STORENAMEDCOLOR,          //  90
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::STORENAMEDCOLOR ),
                          &X11ProtocolParser::_parseRequest< rq::StoreNamedColor > ) },
    { rq::opcodes::QUERYCOLORS,              //  91
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::QUERYCOLORS ),
                          &X11ProtocolParser::_parseRequest< rq::QueryColors >,
                          &X11ProtocolParser::_parseReply< rq::QueryColors > ) },
    { rq::opcodes::LOOKUPCOLOR,              //  92
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::LOOKUPCOLOR ),
                          &X11ProtocolParser::_parseRequest< rq::LookupColor >,
                          &X11ProtocolParser::_parseReply< rq::LookupColor > ) },
    { rq::opcodes::CREATECURSOR,             //  93
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CREATECURSOR ),
                          &X11ProtocolParser::_parseRequest< rq::CreateCursor > ) },
    { rq::opcodes::CREATEGLYPHCURSOR,        //  94
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CREATEGLYPHCURSOR ),
                          &X11ProtocolParser::_parseRequest< rq::CreateGlyphCursor > ) },
    { rq::opcodes::FREECURSOR,               //  95
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::FREECURSOR ),
                          &X11ProtocolParser::_parseRequest< rq::FreeCursor > ) },
    { rq::opcodes::RECOLORCURSOR,            //  96
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::RECOLORCURSOR ),
                          &X11ProtocolParser::_parseRequest< rq::RecolorCursor > ) },
    { rq::opcodes::QUERYBESTSIZE,            //  97
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::QUERYBESTSIZE ),
                          &X11ProtocolParser::_parseRequest< rq::QueryBestSize >,
                          &X11ProtocolParser::_parseReply< rq::QueryBestSize > ) },
    { rq::opcodes::QUERYEXTENSION,           //  98
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::QUERYEXTENSION ),
                          &X11ProtocolParser::_parseRequest< rq::QueryExtension >,
                          &X11ProtocolParser::_parseReply< rq::QueryExtension > ) },
    { rq::opcodes::LISTEXTENSIONS,           //  99
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::LISTEXTENSIONS ),
                          &X11ProtocolParser::_parseRequest< rq::ListExtensions >,
                          &X11ProtocolParser::_parseReply< rq::ListExtensions > ) },
    { rq::opcodes::CHANGEKEYBOARDMAPPING,    // 100
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CHANGEKEYBOARDMAPPING ),
                          &X11ProtocolParser::_parseRequest< rq::ChangeKeyboardMapping > ) },
    { rq::opcodes::GETKEYBOARDMAPPING,       // 101
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETKEYBOARDMAPPING ),
                          &X11ProtocolParser::_parseRequest< rq::GetKeyboardMapping >,
                          &X11ProtocolParser::_parseReply< rq::GetKeyboardMapping > ) },
    { rq::opcodes::CHANGEKEYBOARDCONTROL,    // 102
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CHANGEKEYBOARDCONTROL ),
                          &X11ProtocolParser::_parseRequest< rq::ChangeKeyboardControl > ) },
    { rq::opcodes::GETKEYBOARDCONTROL,       // 103
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETKEYBOARDCONTROL ),
                          &X11ProtocolParser::_parseRequest< rq::GetKeyboardControl >,
                          &X11ProtocolParser::_parseReply< rq::GetKeyboardControl > ) },
    { rq::opcodes::BELL,                     // 104
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::BELL ),
                          &X11ProtocolParser::_parseRequest< rq::Bell > ) },
    { rq::opcodes::CHANGEPOINTERCONTROL,     // 105
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CHANGEPOINTERCONTROL ),
                          &X11ProtocolParser::_parseRequest< rq::ChangePointerControl > ) },
    { rq::opcodes::GETPOINTERCONTROL,        // 106
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETPOINTERCONTROL ),
                          &X11ProtocolParser::_parseRequest< rq::GetPointerControl >,
                          &X11ProtocolParser::_parseReply< rq::GetPointerControl > ) },
    { rq::opcodes::SETSCREENSAVER,           // 107
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::SETSCREENSAVER ),
                          &X11ProtocolParser::_parseRequest< rq::SetScreenSaver > ) },
    { rq::opcodes::GETSCREENSAVER,           // 108
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETSCREENSAVER ),
                          &X11ProtocolParser::_parseRequest< rq::GetScreenSaver >,
                          &X11ProtocolParser::_parseReply< rq::GetScreenSaver > ) },
    { rq::opcodes::CHANGEHOSTS,              // 109
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::CHANGEHOSTS ),
                          &X11ProtocolParser::_parseRequest< rq::ChangeHosts > ) },
    { rq::opcodes::LISTHOSTS,                // 110
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::LISTHOSTS ),
                          &X11ProtocolParser::_parseRequest< rq::ListHosts >,
                          &X11ProtocolParser::_parseReply< rq::ListHosts > ) },
    { rq::opcodes::SETACCESSCONTROL,         // 111
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::SETACCESSCONTROL ),
                          &X11ProtocolParser::_parseRequest< rq::SetAccessControl > ) },
    { rq::opcodes::SETCLOSEDOWNMODE,         // 112
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::SETCLOSEDOWNMODE ),
                          &X11ProtocolParser::_parseRequest< rq::SetCloseDownMode > ) },
    { rq::opcodes::KILLCLIENT,               // 113
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::KILLCLIENT ),
                          &X11ProtocolParser::_parseRequest< rq::KillClient > ) },
    { rq::opcodes::ROTATEPROPERTIES,         // 114
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::ROTATEPROPERTIES ),
                          &X11ProtocolParser::_parseRequest< rq::RotateProperties > ) },
    { rq::opcodes::FORCESCREENSAVER,         // 115
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::FORCESCREENSAVER ),
                          &X11ProtocolParser::_parseRequest< rq::ForceScreenSaver > ) },
    { rq::opcodes::SETPOINTERMAPPING,        // 116
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::SETPOINTERMAPPING ),
                          &X11ProtocolParser::_parseRequest< rq::SetPointerMapping >,
                          &X11ProtocolParser::_parseReply< rq::SetPointerMapping > ) },
    { rq::opcodes::GETPOINTERMAPPING,        // 117
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETPOINTERMAPPING ),
                          &X11ProtocolParser::_parseRequest< rq::GetPointerMapping >,
                          &X11ProtocolParser::_parseReply< rq::GetPointerMapping > ) },
    { rq::opcodes::SETMODIFIERMAPPING,       // 118
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::SETMODIFIERMAPPING ),
                          &X11ProtocolParser::_parseRequest< rq::SetModifierMapping >,
                          &X11ProtocolParser::_parseReply< rq::SetModifierMapping > ) },
    { rq::opcodes::GETMODIFIERMAPPING,       // 119
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::GETMODIFIERMAPPING ),
                          &X11ProtocolParser::_parseRequest< rq::GetModifierMapping >,
                          &X11ProtocolParser::_parseReply< rq::GetModifierMapping > ) },
    { rq::opcodes::NOOPERATION,              // 127
      _MajorOpcodeTraits( rq::names.at( rq::opcodes::NOOPERATION ),
                          &X11ProtocolParser::_parseRequest< rq::NoOperation > ) }
};
