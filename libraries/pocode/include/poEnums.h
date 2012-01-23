#pragma once
#pragma warning (disable:4068)

// FILE NOTES
//
// This file contains enumerated values used throughough pocode.
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
// Used by all poObjects as arguments to the setAlignment() method.
// Also used by poTextBoxes in the setTextAlignment() method.
enum poAlignment {
    PO_ALIGN_NONE=0,
	PO_ALIGN_TOP_LEFT,
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

