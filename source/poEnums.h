#pragma once



// FILE NOTES
//
// This file contains enumerated values used throughough potionCode.
//


#pragma mark - Window Enums

// PO_WINDW_TYPE
// Used by applicationCreateWindow() prior to application launch
enum poWindowType {
	WINDOW_TYPE_NORMAL=0,
	WINDOW_TYPE_BORDERLESS,
	WINDOW_TYPE_FULLSCREEN
};


// poOrientation
#pragma mark - General Enums
enum poOrientation {
    PO_VERTICAL_UP = 0,
    PO_VERTICAL_DOWN = 1<<1,
    PO_HORIZONTAL_LEFT = 1<<2,
    PO_HORIZONTAL_RIGHT = 1<<3,
    
	PO_VERTICAL=PO_VERTICAL_UP | PO_VERTICAL_DOWN,
	PO_HORIZONTAL=PO_HORIZONTAL_LEFT | PO_HORIZONTAL_RIGHT
};

// poAlignment
// Used by all poObjects as arguments to the alignment() method.
// Also used by poTextBoxes in the textAlignment() method.
enum poAlignment {
	PO_ALIGN_TOP_LEFT=0,
	PO_ALIGN_TOP_CENTER,
	PO_ALIGN_TOP_RIGHT,
	PO_ALIGN_CENTER_LEFT,
	PO_ALIGN_CENTER_CENTER,
	PO_ALIGN_CENTER_RIGHT,
	PO_ALIGN_BOTTOM_LEFT,
	PO_ALIGN_BOTTOM_CENTER,
	PO_ALIGN_BOTTOM_RIGHT,
	
	// for code clarity in cases where only one direction matters 
	PO_ALIGN_LEFT	= PO_ALIGN_TOP_LEFT,
	PO_ALIGN_RIGHT	= PO_ALIGN_TOP_RIGHT,
	PO_ALIGN_CENTER	= PO_ALIGN_CENTER_CENTER,
	PO_ALIGN_TOP	= PO_ALIGN_TOP_LEFT,
	PO_ALIGN_BOTTOM	= PO_ALIGN_BOTTOM_LEFT,
	
	PO_ALIGN_NUM_OPTIONS
};

#pragma mark - Matrix enums

// poMatrixOrder
// Used by poObjects to determine the order of the matrix transormations on every frame.
enum poMatrixOrder
{
	// standard order
	PO_MATRIX_ORDER_TRS,
	// orbit order
	PO_MATRIX_ORDER_RST
};

#pragma mark - Event enums

// PO_EVENT TYPES
// This is a list of all poEvent types. These are used in addEvent() and in the eventHandker implementations.
enum {
    PO_TOUCH_BEGAN_INSIDE_EVENT = 0,
    PO_TOUCH_BEGAN_OUTSIDE_EVENT,
    PO_TOUCH_BEGAN_EVERYWHERE_EVENT,
    
    PO_TOUCH_MOVED_INSIDE_EVENT,
    PO_TOUCH_MOVED_OUTSIDE_EVENT,
    PO_TOUCH_MOVED_EVERYWHERE_EVENT,
    
    PO_TOUCH_ENDED_INSIDE_EVENT,
    PO_TOUCH_ENDED_OUTSIDE_EVENT,
    PO_TOUCH_ENDED_EVERYWHERE_EVENT,
    
    PO_TOUCH_CANCELLED_EVENT,
    
	PO_TOUCH_ENTER_EVENT,
	PO_TOUCH_OVER_EVENT,
	PO_TOUCH_LEAVE_EVENT,
    
    PO_MOTION_BEGAN_EVENT,
    PO_MOTION_ENDED_EVENT,
    PO_MOTION_CANCELLED_EVENT,
	
	PO_KEY_UP_EVENT,
	PO_KEY_DOWN_EVENT,
	
	PO_KEY_PRESS_EVENT,
	PO_KEY_RELEASE_EVENT,
	
	PO_MOUSE_UP_INSIDE_EVENT,
	PO_MOUSE_UP_OUTSIDE_EVENT,
	PO_MOUSE_UP_EVERYWHERE_EVENT,
	
	PO_MOUSE_DOWN_INSIDE_EVENT,
	PO_MOUSE_DOWN_OUTSIDE_EVENT,
	PO_MOUSE_DOWN_EVERYWHERE_EVENT,
	
	PO_MOUSE_MOVE_EVENT,
	
	PO_MOUSE_DRAG_EVENT,
	PO_MOUSE_DRAG_EVERYWHERE_EVENT,
	
	PO_MOUSE_ENTER_EVENT,
	PO_MOUSE_OVER_EVENT,
	PO_MOUSE_LEAVE_EVENT,
	
	PO_WINDOW_RESIZED_EVENT,
	
	PO_LAST_EVENT
};


// PO_EVENT SPECIAL KEYS
enum {
	// key event mask
	PO_KEY_CAPS		= 1 << 16,
	PO_KEY_SHIFT	= 1 << 17,
	PO_KEY_CTRL		= 1 << 18,
	PO_KEY_ALT		= 1 << 19,
	PO_KEY_META		= 1 << 20,
	// specific key 
	PO_NUMERIC_KEY_MASK = 1 << 21,
	PO_FUNCTION_KEY_MASK = 1 << 23,
};


// PO_EVENT MOUSE BUTTON LEFT and RIGHT
enum {
	// mouse event mask
	PO_MOUSE_LEFT	= 1,
	PO_MOUSE_MIDDLE	= 1 << 1,
	PO_MOUSE_RIGHT	= 1 << 2,
};

enum {
	// also need to check the modifier flags
	// for arrows, numeric mask and function mask should both be set
	// these numbers are the keyCode
	PO_UP_ARROW = 126,
	PO_DOWN_ARROW = 125,
	PO_LEFT_ARROW = 123,
	PO_RIGHT_ARROW = 124,
	
	PO_RETURN_KEY = 36,
	
	PO_F1 = 122,
	PO_F2 = 120,
	PO_F3 = 99,
	PO_F4 = 118,
	PO_F5 = 96,
	PO_F6 = 97,
	PO_F7 = 98,
	PO_F8 = 100
};


#pragma mark - Image Enums

// PO TEXTURE FITTING OPTIONS
enum poTextureFitOption {
	PO_TEX_FIT_NONE=0,
	PO_TEX_FIT_EXACT,
	PO_TEX_FIT_H,
	PO_TEX_FIT_V,
	PO_TEX_FIT_INSIDE,
	
	PO_TEX_FIT_NUM_OPTIONS
};



#pragma mark - Stroke Enums

// PO_SHAPE_2D STROKE OPTIONS
enum poStrokePlacementOption {
	PO_STROKE_INSIDE=0,
	PO_STROKE_OUTSIDE,
	PO_STROKE_CENTER
};

enum poStrokePlacementProperty {
	PO_STROKE_PLACE_INSIDE=0,
	PO_STROKE_PLACE_OUTSIDE,
	PO_STROKE_PLACE_CENTER
};

enum poStrokeCapProperty {
	PO_STROKE_CAP_BUTT=0,
	PO_STROKE_CAP_ROUND,
	PO_STROKE_CAP_SQUARE
};

enum poStrokeJoinProperty {
	PO_STROKE_JOIN_MITRE=0,
	PO_STROKE_JOIN_BEVEL,
	PO_STROKE_JOIN_ROUND
};


#pragma mark - Tween Enums

// PO_TWEEN REPEAT OPTIONS
enum poTweenRepeat {
	PO_TWEEN_REPEAT_NONE,
	PO_TWEEN_REPEAT_REGULAR,
	PO_TWEEN_REPEAT_PINGPONG
};

