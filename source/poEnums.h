#pragma once


#pragma mark - Window Enums

enum poWindowType {
	WINDOW_TYPE_NORMAL=0,
	WINDOW_TYPE_BORDERLESS,
	WINDOW_TYPE_FULLSCREEN
};



#pragma mark - General Enums
enum poOrientation {
	PO_VERTICAL=0,
	PO_HORIZONTAL
};

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

enum poMatrixOrder
{
	// standard order
	PO_MATRIX_ORDER_TRS,
	// orbit order
	PO_MATRIX_ORDER_RST
};



#pragma mark - Event enums
enum {
	PO_TOUCH_BEGAN_EVENT = 0,
	PO_TOUCH_ENDED_EVENT,
	PO_TOUCH_MOVED_EVENT,
	
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

// the image bit depth
enum ImageBitDepth {
	IMAGE_8 = 8,
	IMAGE_16 = 16,
	IMAGE_24 = 24,
	IMAGE_32 = 32,
	
	NUM_IMAGE_DEPTHS
};

// use these to fit textures in a shape
enum poTextureFitOption {
	PO_TEX_FIT_NONE=0,
	PO_TEX_FIT_EXACT,
	PO_TEX_FIT_H,
	PO_TEX_FIT_V,
	PO_TEX_FIT_INSIDE,
	
	PO_TEX_FIT_NUM_OPTIONS
};



#pragma mark - Stroke Enums

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

enum poTweenRepeat {
	PO_TWEEN_REPEAT_NONE,
	PO_TWEEN_REPEAT_REGULAR,
	PO_TWEEN_REPEAT_PINGPONG
};

