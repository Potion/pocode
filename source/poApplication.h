#pragma once

#include "poWindow.h"


// FILE NOTES
//
// The functions declared in this file act as glue between the operating system and potionCode.
// Their implemenations are platform specific.
//
// Most of the functions declared here are not implemented in poApplication.cpp because they are
// platform specific. All of potionCode examples include implementations of these functions.
//
// The set of window-related functions at the bottom are defined in poApplication.cpp.


// These functions must be defined in the windowing system implementation. They are not in poApplication.cpp.
// On the Mac and iOS, implementations are in AppDelegate.mm.
int			applicationNumberWindows();
poWindow*	applicationCreateWindow(uint, poWindowType, const char*, int, int, int, int);
poWindow*	applicationGetWindow(int index);
std::string applicationGetSupportDirectory();
poWindow*	applicationCurrentWindow();
void		applicationMakeWindowCurrent(poWindow*);
void		applicationMakeWindowFullscreen(poWindow*,bool);
void		applicationMoveWindow(poWindow*,poPoint);
void		applicationReshapeWindow(poWindow*,poRect);
void        applicationQuit();

// These three functions should be defined in your application. They are not defined in poApplication.cpp.

// This function should return a poObject, that is the root node of the scene graph.
// The UID is the same UID passed in per window as each window is created.
// This allows you to have multiple windows, each with their own scenge graph.
// All potionCode example projects implement this function.
extern poObject*    createObjectForID(uint uid);

// This function is where windows should be created. All potionCode example projects implement this function.
extern void         setupApplication();

// This function is called when the application quits. All potionCode example projects implement this function.
extern void         cleanupApplication();




// These functions are defined in poApplication.cpp. The values returned are for the current top-most window.
// All of these extract data from the poWindow class.
float		getWindowWidth();
float		getWindowHeight();
poPoint		getWindowDimensions();
float		getWindowAspect();
poRect		getWindowFrame();
poRect		getWindowBounds();
poPoint		getWindowCenter();
float		getWindowFramerate();
int			getWindowFrameCount();
float		getWindowLastFrameTime();
float		getWindowLastFrameDuration();
poPoint		getWindowMousePosition();
poPoint		getWindowInvMousePosition();



