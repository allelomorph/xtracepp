#include <vector>
#include <string_view>


// TBD these could be part of X11ProtocolParser

// TBD using const std::vector instead of constexpr std::array so that type is same for
//   all name arrays
namespace protocol {

namespace enum_names {

// common:
// BITGRAVITY
extern const std::vector<std::string_view> bitgravity;

// WINGRAVITY
extern const std::vector<std::string_view> wingravity;

//BOOL
extern const std::vector<std::string_view> bool_;

// SETofEVENT
// SETofPOINTEREVENT
// SETofDEVICEEVENT
extern const std::vector<std::string_view> set_of_event;
// SETofEVENT
//     #xFE000000     unused but must be zero

// SETofPOINTEREVENT
//      encodings are the same as for SETofEVENT, except with
//      #xFFFF8003     unused but must be zero

// SETofDEVICEEVENT
//      encodings are the same as for SETofEVENT, except with
//      #xFFFFC0B0     unused but must be zero

// SETofKEYBUTMASK
extern const std::vector<std::string_view> set_of_keybutmask;
// SETofKEYBUTMASK
//   #xE000     unused but must be zero

// SETofKEYMASK
//      encodings are the same as for SETofKEYBUTMASK, except with
//      #xFF00          unused but must be zero

// TBD how to handle gap between 2 and 5?
// HOST used in 110 ListHosts
// HOST.family 1B
// Req 109 ChangeHosts.family 1B (up to 2)
extern const std::vector<std::string_view> host_family;

// server connection success:
// image-byte-order 1B
extern const std::vector<std::string_view> image_byte_order;

// bitmap-format-bit-order 1B
extern const std::vector<std::string_view> bitmap_format_bit_order;

// SCREEN.backing-stores 1B
extern const std::vector<std::string_view> screen_backing_stores;

// VISUALTYPE.class 1B
extern const std::vector<std::string_view> visualtype_class;

// Requests

// Req 001 CreateWindow.class 2B
// Req 002 ChangeWindowAttributes.class 2B
// Req 003 GetWindowAttributes - Reply.class 2B (never 0)
extern const std::vector<std::string_view> window_class;

// Req 001 CreateWindow.visual VISUALID
// Req 002 ChangeWindowAttributes.visual VISUALID
// Req 001 CreateWindow.VALUE.border-pixmap PIXMAP
// Req 002 ChangeWindowAttributes.VALUE.border-pixmap PIXMAP
// Req 001 CreateWindow.VALUE.colormap COLORMAP
// Req 002 ChangeWindowAttributes.VALUE.colormap COLORMAP
extern const std::vector<std::string_view> zero_copy_from_parent;

// Req 001 CreateWindow.value-mask 4B BITMASK
// Req 002 ChangeWindowAttributes.value-mask 4B BITMASK
extern const std::vector<std::string_view> window_attribute_value_mask;

// Req 001 CreateWindow.VALUE.background-pixmap PIXMAP
// Req 002 ChangeWindowAttributes.VALUE.background-pixmap PIXMAP
extern const std::vector<std::string_view> window_attribute_background_pixmap;

// Req 001 CreateWindow.VALUE.backing-store 1B
// Req 002 ChangeWindowAttributes.VALUE.backing-store 1B
// Req 003 GetWindowAttributes - Reply.backing-store 1B
extern const std::vector<std::string_view> window_attribute_backing_store;

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
extern const std::vector<std::string_view> zero_none;

// Req 003 GetWindowAttributes - Reply.map-state 1B
extern const std::vector<std::string_view> window_attribute_map_state;

// Req 006 ChangeSaveSet.mode 1B
extern const std::vector<std::string_view> save_set_mode;

// Req 012 ConfigureWindow.value-mask 2B BITMASK
// Evt  23 ConfigureRequest.value-mask 2B BITMASK
extern const std::vector<std::string_view> window_value_mask;

// Req 012 ConfigureWindow.VALUE.stack-mode 1B
// Evt  23 ConfigureRequest.stack-mode 1B
extern const std::vector<std::string_view> window_value_stack_mode;

// Req  013 CirculateWindow.direction 1B
extern const std::vector<std::string_view> circulate_direction;

// Req  018 ChangeProperty.mode 1B
extern const std::vector<std::string_view> change_property_mode;

// Req 020 GetProperty.type ATOM
extern const std::vector<std::string_view> property_atom;

// Req 025 SendEvent.destination WINDOW
extern const std::vector<std::string_view> event_destination;

// Req 028 GrabButton.button BUTTON
extern const std::vector<std::string_view> button;

// Req 026 GrabPointer - Reply.status 1B
// Req 031 GrabKeyboard - Reply.status 1B
extern const std::vector<std::string_view> grab_status;

// Req 026 GrabPointer.pointer-mode 1B
// Req 026 GrabPointer.keyboard-mode 1B
// Req 028 GrabButton.pointer-mode 1B
// Req 028 GrabButton.keyboard-mode 1B
// Req 031 GrabKeyboard.pointer-mode 1B
// Req 031 GrabKeyboard.keyboard-mode 1B
// Req 033 GrabKey.pointer-mode 1B
// Req 033 GrabKey.keyboard-mode 1B
extern const std::vector<std::string_view> input_mode;

// Req 033 GrabKey.key KEYCODE
// Req 034 UngrabKey.key KEYCODE
extern const std::vector<std::string_view> key;

// TBD how does this work with SETofKEYMASK requiring OxFF00 being zero?
// Req 028 GrabButton.modifiers SETofKEYMASK
// Req 029 UngrabButton.modifiers SETofKEYMASK
// Req 033 GrabKey.modifiers SETofKEYMASK
// Req 034 UngrabKey.modifiers SETofKEYMASK
//          #x8000     AnyModifier

// Req 035 AllowEvents.mode 1B
extern const std::vector<std::string_view> events_mode;

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
extern const std::vector<std::string_view> time;

// Req 042 SetInputFocus.revert-to 1B
// Req 042 SetInputFocus.focus WINDOW (up to 1)
// Req 043 GetInputFocus - Reply.revert-to 1B
// Req 043 GetInputFocus - Reply.focus WINDOW (up to 1)
extern const std::vector<std::string_view> input_focus;

// Req 047 QueryFont - Reply.draw-direction 1B
// Req 048 QueryTextExtents - Reply.draw-direction 1B
// Req 050 ListFontsWithInfo - Reply.draw-direction 1B
extern const std::vector<std::string_view> draw_direction;

// Req 055 CreateGC.value-mask 4B BITMASK
// Req 056 ChangeGC.value-mask 4B BITMASK
extern const std::vector<std::string_view> gc_value_mask;

// protocol::shared_enums::names::gc_value_function
// Req 055 CreateGC.VALUE.function 1B
// Req 056 ChangeGC.VALUE.function 1B
extern const std::vector<std::string_view> gc_value_function;

// Req 055 CreateGC.VALUE.line-style 1B
// Req 056 ChangeGC.VALUE.line-style 1B
extern const std::vector<std::string_view> gc_value_line_style;

// Req 055 CreateGC.VALUE.cap-style 1B
// Req 056 ChangeGC.VALUE.cap-style 1B
extern const std::vector<std::string_view> gc_value_cap_style;

// Req 055 CreateGC.VALUE.join-style 1B
// Req 056 ChangeGC.VALUE.join-style 1B
extern const std::vector<std::string_view> gc_value_join_style;

// Req 055 CreateGC.VALUE.fill-style 1B
// Req 056 ChangeGC.VALUE.fill-style 1B
extern const std::vector<std::string_view> gc_value_fill_style;

// Req 055 CreateGC.VALUE.fill-rule 1B
// Req 056 ChangeGC.VALUE.fill-rule 1B
extern const std::vector<std::string_view> gc_value_fill_rule;

// Req 055 CreateGC.VALUE.subwindow-mode 1B
// Req 056 ChangeGC.VALUE.subwindow-mode 1B
extern const std::vector<std::string_view> gc_value_subwindow_mode;

// Req 055 CreateGC.VALUE.arc-mode 1B
// Req 056 ChangeGC.VALUE.arc-mode 1B
extern const std::vector<std::string_view> gc_value_arc_mode;

// Req 059 SetClipRectangles.ordering 1B
extern const std::vector<std::string_view> clip_rect_ordering;

// Req 069 FillPoly.shape 1B
extern const std::vector<std::string_view> poly_shape;

// Req 064 PolyPoint.coordinate-mode 1B
// Req 065 PolyLine.coordinate-mode 1B
// Req 069 FillPoly.coordinate-mode 1B
extern const std::vector<std::string_view> poly_coordinate_mode;

// Req 072 PutImage.format 1B
// Req 073 GetImage.format 1B (never 0)
extern const std::vector<std::string_view> image_format;

// Req 078 CreateColormap.alloc 1B
extern const std::vector<std::string_view> colormap_alloc;

// TBD make unused a extern constant
// Req 089 StoreColors.COLORITEM.do-red/do-green/do-blue 1B
// Req 090 StoreNamedColor.do-red/do-green/do-blue 1B
extern const std::vector<std::string_view> do_rgb_mask;
//  #xF8     unused

// Req 097 QueryBestSize.class 1B
extern const std::vector<std::string_view> size_class;

// Req 102 ChangeKeyboardControl.value-mask 4B BITMASK
extern const std::vector<std::string_view> keyctl_value_mask;

// protocol::shared_enums::names::off_on_toggle
// Req 102 ChangeKeyboardControl.VALUE.led-mode 1B (up to 1)
// Req 102 ChangeKeyboardControl.VALUE.auto-repeat-mode 1B
// Req 103 GetKeyboardControl - Reply.global-auto-repeat 1B (up to 1)
extern const std::vector<std::string_view> off_on;

// Req 107 SetScreenSaver.prefer-blanking 1B
// Req 107 SetScreenSaver.allow-exposures 1B
// Req 108 GetScreenSaver - Reply.prefer-blanking 1B (up to 1)
// Req 108 GetScreenSaver - Reply.allow-exposures 1B (up to 1)
extern const std::vector<std::string_view> screen_saver;

// Req 109 ChangeHosts.mode 1B
extern const std::vector<std::string_view> host_change_mode;

// Req 110 ListHosts - Reply.mode 1B
extern const std::vector<std::string_view> host_status_mode;

// Req 111 SetAccessControl.mode 1B
extern const std::vector<std::string_view> access_mode;

// Req 112 SetCloseDownMode.mode 1B
extern const std::vector<std::string_view> close_down_mode;

// Req 113 KillClient.resource CARD32
extern const std::vector<std::string_view> client_resource;

// Req 115 ForceScreenSaver.mode 1B
extern const std::vector<std::string_view> screen_saver_set_mode;

// Req 116 SetPointerMapping - Reply.status 1B (up to 1)
// Req 118 SetModifierMapping - Reply.status 1B
extern const std::vector<std::string_view> mapping_status;

// Evt  06 MotionNotify.detail 1B
extern const std::vector<std::string_view> motion_hint;

// Evt  07 EnterNotify.mode 1B (up to 2)
// Evt  08 LeaveNotify.mode 1B (up to 2)
// Evt  09 FocusIn.mode 1B
// Evt  10 FocusOut.mode 1B
extern const std::vector<std::string_view> focus_mode;

// TBD make unused extern constant
// Evt  07 EnterNotify.same-screen/focus 1B
// Evt  08 LeaveNotify.same-screen/focus 1B
extern const std::vector<std::string_view> focus_same_screen_mask;
// #xFC     unused

// Evt  07 EnterNotify.detail 1B (up to 4)
// Evt  08 LeaveNotify.detail 1B (up to 4)
// Evt  09 FocusIn.detail 1B
// Evt  10 FocusOut.detail 1B
extern const std::vector<std::string_view> focus_detail;

// Evt  15 VisibilityNotify.state 1B
extern const std::vector<std::string_view> visibility_state;

// Evt  26 CirculateNotify.place 1B
// Evt  27 CirculateRequest.place 1B
extern const std::vector<std::string_view> circulate_place;

// Evt  28 PropertyNotify.state 1B
extern const std::vector<std::string_view> property_state;

// Evt  32 ColormapNotify.state 1B
extern const std::vector<std::string_view> colormap_state;

// Evt  34 MappingNotify.request 1B
extern const std::vector<std::string_view> mapping_state;


}  // namespace enum_names

}  // namespace protocol
