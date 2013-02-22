/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once
#pragma warning (disable:4068)

// FILE NOTES
//
// This file contains enumerated values used throughough pocode.
//

namespace po {
    #pragma mark - Window Enums

    // WINDW_TYPE
    // Used by applicationCreateWindow() prior to application launch
    enum WindowType {
        WINDOW_TYPE_NORMAL=0,
        WINDOW_TYPE_BORDERLESS,
        WINDOW_TYPE_FULLSCREEN
    };


    #pragma mark - Camera Enums
    enum CameraType { 
        CAMERA_NONE, 
        CAMERA_2D, 
        CAMERA_3D 
    };

    // Orientation
    #pragma mark - General Enums
    enum Orientation {
        ORIENTATION_VERTICAL_UP         = 0x01,
        ORIENTATION_VERTICAL_DOWN       = 0x02,
        ORIENTATION_HORIZONTAL_LEFT     = 0x04,
        ORIENTATION_HORIZONTAL_RIGHT    = 0x08,
        
        ORIENTATION_UNKNOWN = 0x16,
        
        ORIENTATION_VERTICAL            = ORIENTATION_VERTICAL_UP | ORIENTATION_VERTICAL_DOWN,
        ORIENTATION_HORIZONTAL          = ORIENTATION_HORIZONTAL_LEFT | ORIENTATION_HORIZONTAL_RIGHT
    };

    // Alignment
    // Used by all poObjects as arguments to the setAlignment() method.
    // Also used by poTextBoxes in the setTextAlignment() method.
    enum Alignment {
        ALIGN_NONE=0,
        ALIGN_TOP_LEFT,
        ALIGN_TOP_CENTER,
        ALIGN_TOP_RIGHT,
        ALIGN_CENTER_LEFT,
        ALIGN_CENTER_CENTER,
        ALIGN_CENTER_RIGHT,
        ALIGN_BOTTOM_LEFT,
        ALIGN_BOTTOM_CENTER,
        ALIGN_BOTTOM_RIGHT,
        
        // for code clarity in cases where only one direction matters 
        ALIGN_LEFT	= ALIGN_TOP_LEFT,
        ALIGN_RIGHT	= ALIGN_TOP_RIGHT,
        ALIGN_CENTER	= ALIGN_CENTER_CENTER,
        ALIGN_TOP	= ALIGN_TOP_LEFT,
        ALIGN_BOTTOM	= ALIGN_BOTTOM_LEFT,
        
        ALIGN_NUM_OPTIONS
    };

    #pragma mark - Matrix enums

    // poMatrixOrder
    // Used by poObjects to determine the order of the matrix transormations on every frame.
    enum MatrixOrder {
        // standard order
        MATRIX_ORDER_TRS,
        // orbit order
        MATRIX_ORDER_RST
    };


    #pragma mark - Image Enums

    // PO TEXTURE FITTING OPTIONS
    enum TextureFitOption {
        TEX_FIT_NONE=0,
        TEX_FIT_EXACT,
        TEX_FIT_H,
        TEX_FIT_V,
        TEX_FIT_INSIDE,
        
        TEX_FIT_NUM_OPTIONS
    };



    #pragma mark - Stroke Enums

    // SHAPE_2D STROKE OPTIONS
    enum StrokePlacementOption {
        STROKE_INSIDE=0,
        STROKE_OUTSIDE,
        STROKE_CENTER
    };

    enum StrokePlacementProperty {
        STROKE_PLACE_INSIDE=0,
        STROKE_PLACE_OUTSIDE,
        STROKE_PLACE_CENTER
    };

    enum StrokeCapProperty {
        STROKE_CAP_BUTT=0,
        STROKE_CAP_ROUND,
        STROKE_CAP_SQUARE
    };

    enum StrokeJoinProperty {
        STROKE_JOIN_MITRE=0,
        STROKE_JOIN_BEVEL,
        STROKE_JOIN_ROUND
    };


    #pragma mark - Tween Enums

    // TWEEN REPEAT OPTIONS
    enum  TweenRepeat {
        TWEEN_REPEAT_NONE,
        TWEEN_REPEAT_REGULAR,
        TWEEN_REPEAT_PINGPONG
    };


    #pragma mark - TextBox Enums
    enum {
        TEXT_BOX_STROKE_BOUNDS = 1,
        TEXT_BOX_STROKE_TEXT_BOUNDS = 1<<1,
        TEXT_BOX_STROKE_LINES = 1<<2,
        TEXT_BOX_STROKE_GLYPHS = 1<<3,
        TEXT_BOX_STROKE_ALL = TEXT_BOX_STROKE_BOUNDS | TEXT_BOX_STROKE_TEXT_BOUNDS | TEXT_BOX_STROKE_LINES | TEXT_BOX_STROKE_GLYPHS
    };
    
    // DICTIONARY TYPES
    //These are valid values that can be passed to a poDictionary
    
    enum DictionaryType {
        INT_T=0,
        FLOAT_T,
        STRING_T,
        POINT_T,
        COLOR_T,
        VOID_PTR_T,
        DICTIONARY_T
    };

    // EVENT TYPES
    // This is a list of all poEvent types. These are used in addEvent() and in the eventHandker implementations.
    #pragma mark - EventType Enums
    enum {
        MOUSE_DOWN_EVENT,
        MOUSE_DOWN_INSIDE_EVENT,
        MOUSE_UP_EVENT,
        MOUSE_MOVE_EVENT,
        MOUSE_DRAG_EVENT,
        MOUSE_DRAG_INSIDE_EVENT,
        MOUSE_ENTER_EVENT,
        MOUSE_LEAVE_EVENT,
        MOUSE_OVER_EVENT,
        
        KEY_DOWN_EVENT,
        KEY_UP_EVENT,
        
        TOUCH_BEGAN_EVENT,
        TOUCH_BEGAN_INSIDE_EVENT,
        TOUCH_BEGAN_OUTSIDE_EVENT,
        TOUCH_MOVED_EVENT,
        TOUCH_ENDED_EVENT,
        TOUCH_ENDED_INSIDE_EVENT,
        TOUCH_ENDED_OUTSIDE_EVENT,
        TOUCH_CANCELLED_EVENT,
        
        ACCELEROMETER_EVENT,
        GYROSCOPE_EVENT,
        ROTATION_EVENT,
        
        LAST_EVENT
    };
    
    // EVENT SPECIAL KEYS
    enum {
        // key event mask
        KEY_CAPS		= 1 << 16,
        KEY_SHIFT	= 1 << 17,
        KEY_CTRL		= 1 << 18,
        KEY_ALT		= 1 << 19,
        KEY_META		= 1 << 20,
        // specific key
        NUMERIC_KEY_MASK = 1 << 21,
        FUNCTION_KEY_MASK = 1 << 23,
    };
    
    
    // EVENT MOUSE BUTTON LEFT and RIGHT
    enum {
        // mouse event mask
        MOUSE_LEFT	= 1,
        MOUSE_MIDDLE	= 1 << 1,
        MOUSE_RIGHT	= 1 << 2,
    };
    
    enum {
        // also need to check the modifier flags
        // for arrows, numeric mask and function mask should both be set
        // these numbers are the keyCode
        UP_ARROW = 126,
        DOWN_ARROW = 125,
        LEFT_ARROW = 123,
        RIGHT_ARROW = 124,
        
        RETURN_KEY = 36,
        
        F1 = 122,
        F2 = 120,
        F3 = 99,
        F4 = 118,
        F5 = 96,
        F6 = 97,
        F7 = 98,
        F8 = 100
    };
}
