#pragma once

#include "poWindow.h"

// close all windows and exit the application
void		applicationQuit();

// defined in the windowing system implementation
int			applicationNumberWindows();
poWindow*	applicationCreateWindow(uint, poWindowType, const char*, int, int, int, int);
poWindow*	applicationGetWindow(int index);
poWindow*	applicationCurrentWindow();
void		applicationMakeWindowCurrent(poWindow*);
void		applicationMakeWindowFullscreen(poWindow*,bool);
void		applicationMoveWindow(poWindow*,poPoint);
void		applicationReshapeWindow(poWindow*,poRect);

// define to generate base objects for a window after the gl context is created
extern poObject* createObjectForID(uint uid);
// configure the number of windows in an application
extern void		setupApplication();
// delete any global storage associated with the app
extern void		cleanupApplication();

// information about the current windowfloat		
float		getWindowWidth();
float		getWindowHeight();
poRect		getWindowFrame();
poRect		getWindowBounds();
poPoint		getWindowCenter();
float		getWindowFramerate();
int			getWindowFrameCount();
float		getWindowLastFrameTime();
float		getWindowLastFrameDuration();
poPoint		getWindowMousePosition();

