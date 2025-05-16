#include <vector>
#include <string_view>

// TBD these could be part of X11ProtocolParser

// TBD using const std::vector instead of constexpr std::array so that type is same for
//   all name arrays
namespace protocol {

namespace enum_names {

// common:
// BITGRAVITY
inline const
std::vector<std::string_view> bitgravity {
     "Forget",     //  0
     "NorthWest",  //  1
     "North",      //  2
     "NorthEast",  //  3
     "West",       //  4
     "Center",     //  5
     "East",       //  6
     "SouthWest",  //  7
     "South",      //  8
     "SouthEast",  //  9
     "Static"      // 10
};

// WINGRAVITY
inline const
std::vector<std::string_view> wingravity {
     "Unmap",      //  0
     "NorthWest",  //  1
     "North",      //  2
     "NorthEast",  //  3
     "West",       //  4
     "Center",     //  5
     "East",       //  6
     "SouthWest",  //  7
     "South",      //  8
     "SouthEast",  //  9
     "Static"      // 10
};

//BOOL
inline const
std::vector<std::string_view> bool_ {
     "False",  // 0
     "True"    // 1
};

// SETofEVENT
// SETofPOINTEREVENT
// SETofDEVICEEVENT
inline const
std::vector<std::string_view> set_of_event {
     "KeyPress",              // 1 <<  0
     "KeyRelease",            // 1 <<  1
     "ButtonPress",           // 1 <<  2
     "ButtonRelease",         // 1 <<  3
     "EnterWindow",           // 1 <<  4
     "LeaveWindow",           // 1 <<  5
     "PointerMotion",         // 1 <<  6
     "PointerMotionHint",     // 1 <<  7
     "Button1Motion",         // 1 <<  8
     "Button2Motion",         // 1 <<  9
     "Button3Motion",         // 1 << 10
     "Button4Motion",         // 1 << 11
     "Button5Motion",         // 1 << 12
     "ButtonMotion",          // 1 << 13
     "KeymapState",           // 1 << 14
     "Exposure",              // 1 << 15
     "VisibilityChange",      // 1 << 16
     "StructureNotify",       // 1 << 17
     "ResizeRedirect",        // 1 << 18
     "SubstructureNotify",    // 1 << 19
     "SubstructureRedirect",  // 1 << 20
     "FocusChange",           // 1 << 21
     "PropertyChange",        // 1 << 22
     "ColormapChange",        // 1 << 23
     "OwnerGrabButton"        // 1 << 24
};
// SETofEVENT
//     #xFE000000     unused but must be zero

// SETofPOINTEREVENT
//      encodings are the same as for SETofEVENT, except with
//      #xFFFF8003     unused but must be zero

// SETofDEVICEEVENT
//      encodings are the same as for SETofEVENT, except with
//      #xFFFFC0B0     unused but must be zero

// SETofKEYBUTMASK
inline const
std::vector<std::string_view> set_of_keybutmask {
     "Shift",    // 1 <<  0
     "Lock",     // 1 <<  1
     "Control",  // 1 <<  2
     "Mod1",     // 1 <<  3
     "Mod2",     // 1 <<  4
     "Mod3",     // 1 <<  5
     "Mod4",     // 1 <<  6
     "Mod5",     // 1 <<  7
     "Button1",  // 1 <<  8
     "Button2",  // 1 <<  9
     "Button3",  // 1 << 10
     "Button4",  // 1 << 11
     "Button5"   // 1 << 12
};
// SETofKEYBUTMASK
//   #xE000     unused but must be zero

// SETofKEYMASK
//      encodings are the same as for SETofKEYBUTMASK, except with
//      #xFF00          unused but must be zero

// TBD how to handle gap between 2 and 5?
// HOST used in 110 ListHosts
// HOST.family 1B
// Req 109 ChangeHosts.family 1B (up to 2)
inline const
std::vector<std::string_view> host_family {
    "Internet",           // 0
    "DECnet",             // 1
    "Chaos",              // 2
    "",
    "",
    "ServerInterpreted",  // 5
    "InternetV6"          // 6
};

// server connection success:
// image-byte-order 1B
inline const
std::vector<std::string_view> image_byte_order {
    "LSBFirst",  // 0
    "MSBFirst"   // 1
};

// bitmap-format-bit-order 1B
inline const
std::vector<std::string_view> bitmap_format_bit_order {
    "LeastSignificant",  // 0
    "MostSignificant"    // 1
};

// SCREEN.backing-stores 1B
inline const
std::vector<std::string_view> screen_backing_stores {
    "Never",       // 0
    "WhenMapped",  // 1
    "Always"       // 2
};

// VISUALTYPE.class 1B
inline const
std::vector<std::string_view> visualtype_class {
    "StaticGray",   // 0
    "GrayScale",    // 1
    "StaticColor",  // 2
    "PseudoColor",  // 3
    "TrueColor",    // 4
    "DirectColor"   // 5
};

// Requests

// Req 001 CreateWindow.class 2B
// Req 002 ChangeWindowAttributes.class 2B
// Req 003 GetWindowAttributes - Reply.class 2B (never 0)
inline const
std::vector<std::string_view> window_class {
    "CopyFromParent",  // 0
    "InputOutput",     // 1
    "InputOnly"        // 2
};

// Req 001 CreateWindow.visual VISUALID
// Req 002 ChangeWindowAttributes.visual VISUALID
// Req 001 CreateWindow.VALUE.border-pixmap PIXMAP
// Req 002 ChangeWindowAttributes.VALUE.border-pixmap PIXMAP
// Req 001 CreateWindow.VALUE.colormap COLORMAP
// Req 002 ChangeWindowAttributes.VALUE.colormap COLORMAP
inline const
std::vector<std::string_view> zero_copy_from_parent {
    "CopyFromParent"  // 0
};

// Req 001 CreateWindow.value-mask 4B BITMASK
// Req 002 ChangeWindowAttributes.value-mask 4B BITMASK
inline const
std::vector<std::string_view> window_attribute_value_mask {
    "background-pixmap",      // 1 <<  0
    "background-pixel",       // 1 <<  1
    "border-pixmap",          // 1 <<  2
    "border-pixel",           // 1 <<  3
    "bit-gravity",            // 1 <<  4
    "win-gravity",            // 1 <<  5
    "backing-store",          // 1 <<  6
    "backing-planes",         // 1 <<  7
    "backing-pixel",          // 1 <<  8
    "override-redirect",      // 1 <<  9
    "save-under",             // 1 << 10
    "event-mask",             // 1 << 11
    "do-not-propagate-mask",  // 1 << 12
    "colormap",               // 1 << 13
    "cursor"                  // 1 << 14
};

// Req 001 CreateWindow.VALUE.background-pixmap PIXMAP
// Req 002 ChangeWindowAttributes.VALUE.background-pixmap PIXMAP
inline const
std::vector<std::string_view> window_attribute_background_pixmap {
    "None",           // 0
    "ParentRelative"  // 1
};

// Req 001 CreateWindow.VALUE.backing-store 1B
// Req 002 ChangeWindowAttributes.VALUE.backing-store 1B
// Req 003 GetWindowAttributes - Reply.backing-store 1B
inline const
std::vector<std::string_view> window_attribute_backing_store {
    "NotUseful",   // 0
    "WhenMapped",  // 1
    "Always"       // 2
};

// Req 001 CreateWindow.VALUE.cursor CURSOR
// Req 002 ChangeWindowAttributes.VALUE.cursor CURSOR
// Req 003 GetWindowAttributes - Reply.colormap COLORMAP
// Req 015 QueryTree - Reply.parent WINDOW
// Req 016 InternAtom - Reply.atom ATOM
// Req 020 GetProperty - Reply.type ATOM
// Req 022 SetSelectionOwner.owner WINDOW
// Req 023 GetSelectionOwner - Reply.owner WINDOW
// Req 024 ConvertSelection.property ATOM
// Req 026 GrabPointer.confine-to WINDOW
// Req 026 GrabPointer.cursor CURSOR
// Req 028 GrabButton.confine-to WINDOW
// Req 028 GrabButton.cursor CURSOR
// Req 030 ChangeActivePointerGrab.cursor CURSOR
// Req 038 QueryPointer - Reply.child WINDOW
// Req 040 TranslateCoordinates - Reply.child WINDOW
// Req 041 WarpPointer.src-window WINDOW
// Req 041 WarpPointer.dst-window WINDOW
// Req 055 CreateGC.VALUE.clip-mask PIXMAP
// Req 056 ChangeGC.VALUE.clip-mask PIXMAP
// Req 073 GetImage - Reply.visual VISUALID
// Req 094 CreateGlyphCursor.mask-font FONT
// Evt  02 KeyPress.child WINDOW
// Evt  03 KeyRelease.child WINDOW
// Evt  04 ButtonPress.child WINDOW
// Evt  05 ButtonRelease.child WINDOW
// Evt  06 MotionNotify.child WINDOW
// Evt  07 EnterNotify.child WINDOW
// Evt  08 LeaveNotify.child WINDOW
// Evt  22 ConfigureNotify.above-sibling WINDOW
// Evt  23 ConfigureRequest.sibling WINDOW
// Evt  31 SelectionNotify.property ATOM
// Evt  32 ColormapNotify.colormap COLORMAP
inline const
std::vector<std::string_view> zero_none {
    "None"  // 0
};

// Req 003 GetWindowAttributes - Reply.map-state 1B
inline const
std::vector<std::string_view> window_attribute_map_state {
    "Unmapped",    // 0
    "Unviewable",  // 1
    "Viewable"     // 2
};

// Req 006 ChangeSaveSet.mode 1B
inline const
std::vector<std::string_view> save_set_mode {
    "Insert",  // 0
    "Delete"   // 1
};

// Req 012 ConfigureWindow.value-mask 2B BITMASK
// Evt  23 ConfigureRequest.value-mask 2B BITMASK
inline const
std::vector<std::string_view> window_value_mask {
    "x",             // 1 << 0
    "y",             // 1 << 1
    "width",         // 1 << 2
    "height",        // 1 << 3
    "border-width",  // 1 << 4
    "sibling",       // 1 << 5
    "stack-mode"     // 1 << 6
};

// Req 012 ConfigureWindow.VALUE.stack-mode 1B
// Evt  23 ConfigureRequest.stack-mode 1B
inline const
std::vector<std::string_view> window_value_stack_mode {
    "Above",     // 0
    "Below",     // 1
    "TopIf",     // 2
    "BottomIf",  // 3
    "Opposite"   // 4
};

// Req  013 CirculateWindow.direction 1B
inline const
std::vector<std::string_view> circulate_direction {
    "RaiseLowest",  // 0
    "LowerHighest"  // 1
};

// Req  018 ChangeProperty.mode 1B
inline const
std::vector<std::string_view> change_property_mode {
    "Replace",  // 0
    "Prepend",  // 1
    "Append"    // 2
};

// Req 020 GetProperty.type ATOM
inline const
std::vector<std::string_view> property_atom {
    "AnyPropertyType"  // 0
};

// Req 025 SendEvent.destination WINDOW
inline const
std::vector<std::string_view> event_destination {
    "PointerWindow",  // 0
    "InputFocus"      // 1
};

// Req 028 GrabButton.button BUTTON
inline const
std::vector<std::string_view> button {
    "AnyButton",  // 0
};

// Req 026 GrabPointer - Reply.status 1B
// Req 031 GrabKeyboard - Reply.status 1B
inline const
std::vector<std::string_view> grab_status {
    "Success",         // 0
    "AlreadyGrabbed",  // 1
    "InvalidTime",     // 2
    "NotViewable",     // 3
    "Frozen"           // 4
};

// Req 026 GrabPointer.pointer-mode 1B
// Req 026 GrabPointer.keyboard-mode 1B
// Req 028 GrabButton.pointer-mode 1B
// Req 028 GrabButton.keyboard-mode 1B
// Req 031 GrabKeyboard.pointer-mode 1B
// Req 031 GrabKeyboard.keyboard-mode 1B
// Req 033 GrabKey.pointer-mode 1B
// Req 033 GrabKey.keyboard-mode 1B
inline const
std::vector<std::string_view> input_mode {
    "Synchronous",  // 0
    "Asynchronous"  // 1
};

// Req 033 GrabKey.key KEYCODE
// Req 034 UngrabKey.key KEYCODE
inline const
std::vector<std::string_view> key {
    "AnyKey"  // 0
};

// TBD how does this work with SETofKEYMASK requiring OxFF00 being zero?
// Req 028 GrabButton.modifiers SETofKEYMASK
// Req 029 UngrabButton.modifiers SETofKEYMASK
// Req 033 GrabKey.modifiers SETofKEYMASK
// Req 034 UngrabKey.modifiers SETofKEYMASK
//   1 << 15       #x8000     AnyModifier

// Req 035 AllowEvents.mode 1B
inline const
std::vector<std::string_view> events_mode {
    "AsyncPointer",    // 0
    "SyncPointer",     // 1
    "ReplayPointer",   // 2
    "AsyncKeyboard",   // 3
    "SyncKeyboard",    // 4
    "ReplayKeyboard",  // 5
    "AsyncBoth",       // 6
    "SyncBoth"         // 7
};

// Req 022 SetSelectionOwner.time TIMESTAMP
// Req 024 ConvertSelection.time TIMESTAMP
// Req 026 GrabPointer.time TIMESTAMP
// Req 027 UngrabPointer.time TIMESTAMP
// Req 030 ChangeActivePointerGrab.time TIMESTAMP
// Req 031 GrabKeyboard.time TIMESTAMP
// Req 032 UngrabKeyboard.time TIMESTAMP
// Req 035 AllowEvents.time TIMESTAMP
// Req 039 GetMotionEvents.start TIMESTAMP
// Req 039 GetMotionEvents.stop TIMESTAMP
// Req 042 SetInputFocus.time TIMESTAMP
// Evt  30 SelectionRequest.time TIMESTAMP
// Evt  31 SelectionNotify.time TIMESTAMP
inline const
std::vector<std::string_view> time {
    "CurrentTime"  // 0
};

// Req 042 SetInputFocus.revert-to 1B
// Req 042 SetInputFocus.focus WINDOW (up to 1)
// Req 043 GetInputFocus - Reply.revert-to 1B
// Req 043 GetInputFocus - Reply.focus WINDOW (up to 1)
inline const
std::vector<std::string_view> input_focus {
    "None",         // 0
    "PointerRoot",  // 1
    "Parent"        // 2
};

// Req 047 QueryFont - Reply.draw-direction 1B
// Req 048 QueryTextExtents - Reply.draw-direction 1B
// Req 050 ListFontsWithInfo - Reply.draw-direction 1B
inline const
std::vector<std::string_view> draw_direction {
    "LeftToRight",  // 0
    "RightToLeft"   // 1
};

// Req 055 CreateGC.value-mask 4B BITMASK
// Req 056 ChangeGC.value-mask 4B BITMASK
// Req 057 CopyGC.value-mask 4B BITMASK
inline const
std::vector<std::string_view> gc_value_mask {
    "function",               // 1 <<  0
    "plane-mask",             // 1 <<  1
    "foreground",             // 1 <<  2
    "background",             // 1 <<  3
    "line-width",             // 1 <<  4
    "line-style",             // 1 <<  5
    "cap-style",              // 1 <<  6
    "join-style",             // 1 <<  7
    "fill-style",             // 1 <<  8
    "fill-rule",              // 1 <<  9
    "tile",                   // 1 << 10
    "stipple",                // 1 << 11
    "tile-stipple-x-origin",  // 1 << 12
    "tile-stipple-y-origin",  // 1 << 13
    "font",                   // 1 << 14
    "subwindow-mode",         // 1 << 15
    "graphics-exposures",     // 1 << 16
    "clip-x-origin",          // 1 << 17
    "clip-y-origin",          // 1 << 18
    "clip-mask",              // 1 << 19
    "dash-offset",            // 1 << 20
    "dashes",                 // 1 << 21
    "arc-mode"                // 1 << 22
};

// Req 055 CreateGC.VALUE.function 1B
// Req 056 ChangeGC.VALUE.function 1B
inline const
std::vector<std::string_view> gc_value_function {
    "Clear",         //  0
    "And",           //  1
    "AndReverse",    //  2
    "Copy",          //  3
    "AndInverted",   //  4
    "NoOp",          //  5
    "Xor",           //  6
    "Or",            //  7
    "Nor",           //  8
    "Equiv",         //  9
    "Invert",        // 10
    "OrReverse",     // 11
    "CopyInverted",  // 12
    "OrInverted",    // 13
    "Nand",          // 14
    "Set"            // 15
};

// Req 055 CreateGC.VALUE.line-style 1B
// Req 056 ChangeGC.VALUE.line-style 1B
inline const
std::vector<std::string_view> gc_value_line_style {
    "Solid",      // 0
    "OnOffDash",  // 1
    "DoubleDash"  // 2
};

// Req 055 CreateGC.VALUE.cap-style 1B
// Req 056 ChangeGC.VALUE.cap-style 1B
inline const
std::vector<std::string_view> gc_value_cap_style {
    "NotLast",    // 0
    "Butt",       // 1
    "Round",      // 2
    "Projecting"  // 3
};

// Req 055 CreateGC.VALUE.join-style 1B
// Req 056 ChangeGC.VALUE.join-style 1B
inline const
std::vector<std::string_view> gc_value_join_style {
    "Miter",  // 0
    "Round",  // 1
    "Bevel"   // 2
};

// Req 055 CreateGC.VALUE.fill-style 1B
// Req 056 ChangeGC.VALUE.fill-style 1B
inline const
std::vector<std::string_view> gc_value_fill_style {
    "Solid",          // 0
    "Tiled",          // 1
    "Stippled",       // 2
    "OpaqueStippled"  // 3
};

// Req 055 CreateGC.VALUE.fill-rule 1B
// Req 056 ChangeGC.VALUE.fill-rule 1B
inline const
std::vector<std::string_view> gc_value_fill_rule {
    "EvenOdd",  // 0
    "Winding"   // 1
};

// Req 055 CreateGC.VALUE.subwindow-mode 1B
// Req 056 ChangeGC.VALUE.subwindow-mode 1B
inline const
std::vector<std::string_view> gc_value_subwindow_mode {
    "ClipByChildren",   // 0
    "IncludeInferiors"  // 1
};

// Req 055 CreateGC.VALUE.arc-mode 1B
// Req 056 ChangeGC.VALUE.arc-mode 1B
inline const
std::vector<std::string_view> gc_value_arc_mode {
    "Chord",    // 0
    "PieSlice"  // 1
};

// Req 059 SetClipRectangles.ordering 1B
inline const
std::vector<std::string_view> clip_rect_ordering {
    "UnSorted",  // 0
    "YSorted",   // 1
    "YXSorted",  // 2
    "YXBanded"   // 3
};

// Req 069 FillPoly.shape 1B
inline const
std::vector<std::string_view> poly_shape {
    "Complex",    // 0
    "Nonconvex",  // 1
    "Convex"      // 2
};

// Req 064 PolyPoint.coordinate-mode 1B
// Req 065 PolyLine.coordinate-mode 1B
// Req 069 FillPoly.coordinate-mode 1B
inline const
std::vector<std::string_view> poly_coordinate_mode {
    "Origin",   // 0
    "Previous"  // 1
};

// Req 072 PutImage.format 1B
// Req 073 GetImage.format 1B (never 0)
inline const
std::vector<std::string_view> image_format {
    "Bitmap",    // 0
    "XYPixmap",  // 1
    "ZPixmap"    // 2
};

// Req 078 CreateColormap.alloc 1B
inline const
std::vector<std::string_view> colormap_alloc {
    "None",  // 0
    "All"    // 1
};

// TBD make unused a constant
// Req 089 StoreColors.COLORITEM.do-red/do-green/do-blue 1B
// Req 090 StoreNamedColor.do-red/do-green/do-blue 1B
inline const
std::vector<std::string_view> do_rgb_mask {
    "do-red",    // 1 << 0 (1 is True, 0 is False)
    "do-green",  // 1 << 1 (1 is True, 0 is False)
    "do-blue"    // 1 << 2 (1 is True, 0 is False)
};
//  #xF8     unused

// Req 097 QueryBestSize.class 1B
inline const
std::vector<std::string_view> size_class {
    "Cursor",  // 0
    "Tile",    // 1
    "Stipple"  // 2
};

// Req 102 ChangeKeyboardControl.value-mask 4B BITMASK
inline const
std::vector<std::string_view> keyctl_value_mask {
    "key-click-percent",  // 1 << 0
    "bell-percent",       // 1 << 1
    "bell-pitch",         // 1 << 2
    "bell-duration",      // 1 << 3
    "led",                // 1 << 4
    "led-mode",           // 1 << 5
    "key",                // 1 << 6
    "auto-repeat-mode"    // 1 << 7
};

// protocol::shared_enums::names::off_on_toggle
// Req 102 ChangeKeyboardControl.VALUE.led-mode 1B (up to 1)
// Req 102 ChangeKeyboardControl.VALUE.auto-repeat-mode 1B
// Req 103 GetKeyboardControl - Reply.global-auto-repeat 1B (up to 1)
inline const
std::vector<std::string_view> off_on {
    "Off",  // 0
    "On"    // 1
};

// Req 107 SetScreenSaver.prefer-blanking 1B
// Req 107 SetScreenSaver.allow-exposures 1B
// Req 108 GetScreenSaver - Reply.prefer-blanking 1B (up to 1)
// Req 108 GetScreenSaver - Reply.allow-exposures 1B (up to 1)
inline const
std::vector<std::string_view> screen_saver {
    "No",      // 0
    "Yes",     // 1
    "Default"  // 2
};

// Req 109 ChangeHosts.mode 1B
inline const
std::vector<std::string_view> host_change_mode {
    "Insert",  // 0
    "Delete"   // 1
};

// Req 110 ListHosts - Reply.mode 1B
inline const
std::vector<std::string_view> host_status_mode {
    "Disabled",  // 0
    "Enabled"    // 1
};

// Req 111 SetAccessControl.mode 1B
inline const
std::vector<std::string_view> access_mode {
    "Disable",  // 0
    "Enable"    // 1
};

// Req 112 SetCloseDownMode.mode 1B
inline const
std::vector<std::string_view> close_down_mode {
    "Destroy",          // 0
    "RetainPermanent",  // 1
    "RetainTemporary"   // 2
};

// Req 113 KillClient.resource CARD32
inline const
std::vector<std::string_view> client_resource {
    "AllTemporary"  // 0
};

// Req 115 ForceScreenSaver.mode 1B
inline const
std::vector<std::string_view> screen_saver_set_mode {
    "Reset",    // 0
    "Activate"  // 1
};

// Req 116 SetPointerMapping - Reply.status 1B (up to 1)
// Req 118 SetModifierMapping - Reply.status 1B
inline const
std::vector<std::string_view> mapping_status {
    "Success",  // 0
    "Busy",     // 1
    "Failed"    // 2
};

// Evt  06 MotionNotify.detail 1B
inline const
std::vector<std::string_view> motion_hint {
    "Normal",  // 0
    "Hint"     // 1
};

// Evt  07 EnterNotify.mode 1B (up to 2)
// Evt  08 LeaveNotify.mode 1B (up to 2)
// Evt  09 FocusIn.mode 1B
// Evt  10 FocusOut.mode 1B
inline const
std::vector<std::string_view> focus_mode {
    "Normal",       // 0
    "Grab",         // 1
    "Ungrab",       // 2
    "WhileGrabbed"  // 3
};

// TBD make unused constant
// Evt  07 EnterNotify.same-screen/focus 1B
// Evt  08 LeaveNotify.same-screen/focus 1B
inline const
std::vector<std::string_view> focus_same_screen_mask {
    "focus",       // 1 << 0 (1 is True, 0 is False)
    "same-screen"  // 1 << 1 (1 is True, 0 is False)
};
// #xFC     unused

// protocol::shared_enums::names::focus_detail
// Evt  07 EnterNotify.detail 1B (up to 4)
// Evt  08 LeaveNotify.detail 1B (up to 4)
// Evt  09 FocusIn.detail 1B
// Evt  10 FocusOut.detail 1B
inline const
std::vector<std::string_view> focus_detail {
    "Ancestor",          // 0
    "Virtual",           // 1
    "Inferior",          // 2
    "Nonlinear",         // 3
    "NonlinearVirtual",  // 4
    "Pointer",           // 5
    "PointerRoot",       // 6
    "None"               // 7
};

// Evt  15 VisibilityNotify.state 1B
inline const
std::vector<std::string_view> visibility_state {
    "Unobscured",         // 0
    "PartiallyObscured",  // 1
    "FullyObscured"       // 2
};

// Evt  26 CirculateNotify.place 1B
// Evt  27 CirculateRequest.place 1B
inline const
std::vector<std::string_view> circulate_place {
    "Top",    // 0
    "Bottom"  // 1
};

// Evt  28 PropertyNotify.state 1B
inline const
std::vector<std::string_view> property_state {
    "NewValue",  // 0
    "Deleted"    // 1
};

// Evt  32 ColormapNotify.state 1B
inline const
std::vector<std::string_view> colormap_state {
    "Uninstalled",  // 0
    "Installed"     // 1
};

// Evt  34 MappingNotify.request 1B
inline const
std::vector<std::string_view> mapping_state {
    "Modifier",  // 0
    "Keyboard",  // 1
    "Pointer"    // 2
};


}  // namespace enum_names

}  // namespace protocol
