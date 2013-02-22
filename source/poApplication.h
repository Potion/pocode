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

#include "poWindow.h"

// FILE NOTES
//
// The functions declared in this file act as glue between the operating system and pocode.
// Their implemenations are platform specific.
//
// Most of the functions declared here are not implemented in poApplication.cpp because they are
// platform specific. All of pocode examples include implementations of these functions.
//
// The set of window-related functions at the bottom are defined in poApplication.cpp.


// These functions must be defined in the windowing system implementation. They are not in poApplication.cpp.
// On the Mac and iOS, implementations are in AppDelegate.mm.

namespace po {
    int			applicationNumberWindows();
    Window*	applicationCreateWindow(uint, WindowType, const char*, int, int, int, int);
    Window*	applicationGetWindow(int index=0);
    std::string	applicationGetResourceDirectory();
    std::string applicationGetSupportDirectory();
    Window*	applicationCurrentWindow();
    void		applicationMakeWindowCurrent(Window*);
    void		applicationMakeWindowFullscreen(Window*,bool);
    void		applicationMoveWindow(Window*,Point);
    void		applicationReshapeWindow(Window*,Rect);
    void        applicationQuit();

    // These three functions should be defined in your application. They are not defined in poApplication.cpp.

    // This function should return a Object, that is the root node of the scene graph.
    // The UID is the same UID passed in per window as each window is created.
    // This allows you to have multiple windows, each with their own scenge graph.
    // All pocode example projects implement this function.
    extern Object*    createObjectForID(uint uid);

    // This function is where windows should be created. All pocode example projects implement this function.
    extern void         setupApplication();

    // This function is called when the application quits. All pocode example projects implement this function.
    extern void         cleanupApplication();


    // These functions are defined in poApplication.cpp. The values returned are for the current top-most window.
    // All of these extract data from the Window class.
    float		getWindowWidth();
    float		getWindowHeight();
    Point		getWindowDimensions();
    float		getWindowAspect();
    Rect		getWindowFrame();
    Rect		getWindowBounds();
    Point		getWindowCenter();
    float		getWindowFramerate();
    int			getWindowFrameCount();
    float		getWindowLastFrameTime();
    float		getWindowLastFrameDuration();
    Point		getWindowMousePosition();
    Point		getWindowInvMousePosition();

    float       getScale();


    //Window + Mouse events only for desktop
    #if defined(POTION_MAC) || defined(POTION_WINDOWS) || defined(POTION_LINUX)
        void		setWindowMouseMoveEnabled(bool b);
        bool		getWindowMouseMoveEnabled();
        void        showCursor();
        void        hideCursor();
    #endif



    //Accelerometer events currently only exist in iOS app delegate.
    #ifdef POTION_IOS
    void setMultiTouchEnabled(bool isEnabled);
    void startAccelerometer(float frequency);
    void stopAccelerometer();

    Orientation   getOrientation();
    void            setAutoRotateOrientations(unsigned char orientations);
    #endif
}