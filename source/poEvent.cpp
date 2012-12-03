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

#include "poEvent.h"
#include "poObject.h"
#include "poApplication.h"

namespace po {

    // -----------------------------------------------------------------------------------
    // ================================ Event Type Checking ==============================
    #pragma mark Event Type Checking

    bool isNumericMask(Event* e)	{ return (e->modifiers & po::NUMERIC_KEY_MASK) > 0; }
    bool isFunctionMask(Event* e)	{ return (e->modifiers & po::FUNCTION_KEY_MASK) > 0; }
    bool isArrowMask(Event* e)      { return isNumericMask(e) && isFunctionMask(e); }
    bool isMetaMask(Event* e)		{ return (e->modifiers & po::KEY_META) > 0; }
    bool isAltMask(Event* e)		{ return (e->modifiers & po::KEY_ALT) > 0; }
    bool isShiftMask(Event* e)      { return (e->modifiers & po::KEY_SHIFT) > 0; }
    bool isCtrlMask(Event* e)		{ return (e->modifiers & po::KEY_CTRL) > 0; }
    bool isLeftMouse(Event* e)      { return (e->modifiers & po::MOUSE_LEFT) > 0; }
    bool isRightMouse(Event* e)     { return (e->modifiers & po::MOUSE_RIGHT) > 0; }
    bool isCenterMouse(Event* e)	{ return (e->modifiers & po::MOUSE_MIDDLE) > 0; }
    
    
    //------------------------------------------------------------------------
    bool isMouseEvent(int type) {
        return (type == po::MOUSE_UP_EVENT			||
                type == po::MOUSE_DOWN_EVENT			||
                type == po::MOUSE_DOWN_INSIDE_EVENT	||
                type == po::MOUSE_MOVE_EVENT			||
                type ==	po::MOUSE_DRAG_EVENT			||
                type ==	po::MOUSE_DRAG_INSIDE_EVENT	||
                type == po::MOUSE_ENTER_EVENT		||
                type == po::MOUSE_LEAVE_EVENT		||
                type == po::MOUSE_OVER_EVENT);
    }
    
    
    //------------------------------------------------------------------------
    bool isKeyEvent(int type) {
        return (type == po::KEY_UP_EVENT				||
                type == po::KEY_DOWN_EVENT);
    }
    
    
    //------------------------------------------------------------------------
    bool isTouchEvent(int type) {
        return (type == po::TOUCH_BEGAN_EVENT		||
                type == po::TOUCH_BEGAN_INSIDE_EVENT ||
                type == po::TOUCH_BEGAN_OUTSIDE_EVENT||
                type == po::TOUCH_MOVED_EVENT		||
                type == po::TOUCH_ENDED_EVENT		||
                type == po::TOUCH_ENDED_INSIDE_EVENT	||
                type == po::TOUCH_ENDED_OUTSIDE_EVENT||
                type == po::TOUCH_CANCELLED_EVENT);
    }
    
    
    //------------------------------------------------------------------------
    bool isMotionEvent(int type) {
        return (type == po::ACCELEROMETER_EVENT      ||
                type == po::GYROSCOPE_EVENT          ||
                type == po::ROTATION_EVENT);
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: Event =====================================
    #pragma mark - Event -

    //------------------------------------------------------------------------
    Event::Event()
    :	type(po::LAST_EVENT)
    ,	source(NULL)
    ,	keyCode(0)
    ,	keyChar(0)
    ,	timestamp(0.0)
    ,	modifiers(0)
    ,   touchID(-1)
    ,   uniqueID(-1)
    {}
    
    
    //------------------------------------------------------------------------
    Event::Event(int t, Object* f, std::string m, const Dictionary& d)
    :	type(t)
    ,	source(f)
    ,	keyCode(0)
    ,	keyChar(0)
    ,	timestamp(0.0)
    ,	modifiers(0)
    ,   message(m)
    ,	dictionary(d)
    ,   touchID(-1)
    ,   uniqueID(-1)
    {}

}/* End po namespace */