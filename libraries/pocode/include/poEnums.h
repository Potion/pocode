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

    // PO_WINDW_TYPE
    // Used by applicationCreateWindow() prior to application launch
    enum WindowType {
        WINDOW_TYPE_NORMAL=0,
        WINDOW_TYPE_BORDERLESS,
        WINDOW_TYPE_FULLSCREEN
    };


    #pragma mark - Camera Enums
    enum CameraType { 
        PO_CAMERA_NONE, 
        PO_CAMERA_2D, 
        PO_CAMERA_3D 
    };

    // Orientation
    #pragma mark - General Enums
    enum Orientation {
        PO_VERTICAL_UP      = 0x01,
        PO_VERTICAL_DOWN    = 0x02,
        PO_HORIZONTAL_LEFT  = 0x04,
        PO_HORIZONTAL_RIGHT = 0x08,
        
        PO_UNKNOWN_ORIENTATION = 0x16,
        
        PO_VERTICAL=PO_VERTICAL_UP | PO_VERTICAL_DOWN,
        PO_HORIZONTAL=PO_HORIZONTAL_LEFT | PO_HORIZONTAL_RIGHT
    };

    // Alignment
    // Used by all poObjects as arguments to the setAlignment() method.
    // Also used by poTextBoxes in the setTextAlignment() method.
    enum Alignment {
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
    enum MatrixOrder {
        // standard order
        PO_MATRIX_ORDER_TRS,
        // orbit order
        PO_MATRIX_ORDER_RST
    };


    #pragma mark - Image Enums

    // PO TEXTURE FITTING OPTIONS
    enum TextureFitOption {
        PO_TEX_FIT_NONE=0,
        PO_TEX_FIT_EXACT,
        PO_TEX_FIT_H,
        PO_TEX_FIT_V,
        PO_TEX_FIT_INSIDE,
        
        PO_TEX_FIT_NUM_OPTIONS
    };



    #pragma mark - Stroke Enums

    // PO_SHAPE_2D STROKE OPTIONS
    enum StrokePlacementOption {
        PO_STROKE_INSIDE=0,
        PO_STROKE_OUTSIDE,
        PO_STROKE_CENTER
    };

    enum StrokePlacementProperty {
        PO_STROKE_PLACE_INSIDE=0,
        PO_STROKE_PLACE_OUTSIDE,
        PO_STROKE_PLACE_CENTER
    };

    enum StrokeCapProperty {
        PO_STROKE_CAP_BUTT=0,
        PO_STROKE_CAP_ROUND,
        PO_STROKE_CAP_SQUARE
    };

    enum StrokeJoinProperty {
        PO_STROKE_JOIN_MITRE=0,
        PO_STROKE_JOIN_BEVEL,
        PO_STROKE_JOIN_ROUND
    };


    #pragma mark - Tween Enums

    // PO_TWEEN REPEAT OPTIONS
    enum  TweenRepeat {
        PO_TWEEN_REPEAT_NONE,
        PO_TWEEN_REPEAT_REGULAR,
        PO_TWEEN_REPEAT_PINGPONG
    };


    #pragma mark - TextBox Enums
    enum {
        PO_TEXT_BOX_STROKE_BOUNDS = 1,
        PO_TEXT_BOX_STROKE_TEXT_BOUNDS = 1<<1,
        PO_TEXT_BOX_STROKE_LINES = 1<<2,
        PO_TEXT_BOX_STROKE_GLYPHS = 1<<3,
        PO_TEXT_BOX_STROKE_ALL = PO_TEXT_BOX_STROKE_BOUNDS | PO_TEXT_BOX_STROKE_TEXT_BOUNDS | PO_TEXT_BOX_STROKE_LINES | PO_TEXT_BOX_STROKE_GLYPHS
    };
    
    // PO_DICTIONARY TYPES
    //These are valid values that can be passed to a poDictionary
    
    enum DictionaryType {
        PO_INT_T=0,
        PO_FLOAT_T,
        PO_STRING_T,
        PO_POINT_T,
        PO_COLOR_T,
        PO_VOID_PTR_T,
        PO_DICTIONARY_T
    };

    // PO_EVENT TYPES
    // This is a list of all poEvent types. These are used in addEvent() and in the eventHandker implementations.
    #pragma mark - EventType Enums
    enum {
        PO_MOUSE_DOWN_EVENT,
        PO_MOUSE_DOWN_INSIDE_EVENT,
        PO_MOUSE_UP_EVENT,
        PO_MOUSE_MOVE_EVENT,
        PO_MOUSE_DRAG_EVENT,
        PO_MOUSE_DRAG_INSIDE_EVENT,
        PO_MOUSE_ENTER_EVENT,
        PO_MOUSE_LEAVE_EVENT,
        PO_MOUSE_OVER_EVENT,
        
        PO_KEY_DOWN_EVENT,
        PO_KEY_UP_EVENT,
        
        PO_TOUCH_BEGAN_EVENT,
        PO_TOUCH_BEGAN_INSIDE_EVENT,
        PO_TOUCH_BEGAN_OUTSIDE_EVENT,
        PO_TOUCH_MOVED_EVENT,
        PO_TOUCH_ENDED_EVENT,
        PO_TOUCH_ENDED_INSIDE_EVENT,
        PO_TOUCH_ENDED_OUTSIDE_EVENT,
        PO_TOUCH_CANCELLED_EVENT,
        
        PO_ACCELEROMETER_EVENT,
        PO_GYROSCOPE_EVENT,
        PO_ROTATION_EVENT,
        
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
}
