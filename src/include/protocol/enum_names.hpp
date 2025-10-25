#ifndef PROTOCOL_ENUM_NAMES_HPP
#define PROTOCOL_ENUM_NAMES_HPP


#include <vector>
#include <string_view>

namespace protocol {

// enum names shared accross serveral protocol value fields - if a set of names
//   are unique to a field, they are stored with that encoding

namespace enum_names {

////// Connection Setup

// HOST.family 1B
// Req 109 ChangeHosts.family 1B (max 2)
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

// ConnInitiation::Header::byte_order 1B
// ConnAcceptance::Encoding::image_byte_order 1B
inline const
std::vector<std::string_view> byte_order {
    "LSBFirst",  // 0
    "MSBFirst"   // 1
};

////// Requests, Events

// Req 001 CreateWindow.class 2B
// Req 002 ChangeWindowAttributes.class 2B
// Req 003 GetWindowAttributes - Reply.class 2B (min 1)
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

// Req 006 ChangeSaveSet.mode 1B
// Req 109 ChangeHosts.mode 1B
inline const
std::vector<std::string_view> change_mode {
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

// Req 028 GrabButton.button BUTTON
// Req 029 UngrabButton.button BUTTON
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
// Req 042 SetInputFocus.focus WINDOW (max 1)
// Req 043 GetInputFocus - Reply.revert-to 1B
// Req 043 GetInputFocus - Reply.focus WINDOW (max 1)
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

// Req 089 StoreColors.COLORITEM.do-red/do-green/do-blue 1B
// Req 090 StoreNamedColor.do-red/do-green/do-blue 1B
inline const
std::vector<std::string_view> do_rgb_mask {
    "do-red",    // 1 << 0  // 0 False 1 True
    "do-green",  // 1 << 1  // 0 False 1 True
    "do-blue"    // 1 << 2  // 0 False 1 True
};

// Req 102 ChangeKeyboardControl.VALUE.led-mode 1B (max 1)
// Req 102 ChangeKeyboardControl.VALUE.auto-repeat-mode 1B
// Req 103 GetKeyboardControl - Reply.global-auto-repeat 1B (max 1)
inline const
std::vector<std::string_view> off_on {
    "Off",     // 0
    "On",      // 1
    "Default"  // 2
};

// Req 107 SetScreenSaver.prefer-blanking 1B
// Req 107 SetScreenSaver.allow-exposures 1B
// Req 108 GetScreenSaver - Reply.prefer-blanking 1B (max 1)
// Req 108 GetScreenSaver - Reply.allow-exposures 1B (max 1)
inline const
std::vector<std::string_view> no_yes {
    "No",      // 0
    "Yes",     // 1
    "Default"  // 2
};

// Req 116 SetPointerMapping - Reply.status 1B (max 1)
// Req 118 SetModifierMapping - Reply.status 1B
inline const
std::vector<std::string_view> mapping_status {
    "Success",  // 0
    "Busy",     // 1
    "Failed"    // 2
};

// Evt  07 EnterNotify.mode 1B (max 2)
// Evt  08 LeaveNotify.mode 1B (max 2)
// Evt  09 FocusIn.mode 1B
// Evt  10 FocusOut.mode 1B
inline const
std::vector<std::string_view> focus_mode {
    "Normal",       // 0
    "Grab",         // 1
    "Ungrab",       // 2
    "WhileGrabbed"  // 3
};

// Evt  07 EnterNotify.same-screen/focus 1B
// Evt  08 LeaveNotify.same-screen/focus 1B
inline const
std::vector<std::string_view> focus_same_screen_mask {
    "focus",       // 1 << 0  // 0 False 1 True
    "same-screen"  // 1 << 1  // 0 False 1 True
};

// Evt  07 EnterNotify.detail 1B (max 4)
// Evt  08 LeaveNotify.detail 1B (max 4)
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

// Evt  26 CirculateNotify.place 1B
// Evt  27 CirculateRequest.place 1B
inline const
std::vector<std::string_view> circulate_place {
    "Top",    // 0
    "Bottom"  // 1
};

}  // namespace enum_names

}  // namespace protocol


#endif  // PROTOCOL_ENUM_NAMES_HPP
