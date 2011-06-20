#pragma once

#include "poEnums.h"
#include "poRect.h"

class poObject;
class poWindow;

// close all windows and exit the application
void		applicationQuit();

// manage windows
int			applicationNumberWindows();
poWindow*	applicationCreateWindow(uint, poWindowType, const char*, int, int, int, int);
poWindow*	applicationGetWindow(int index);
poWindow*	applicationCurrentWindow();
void		applicationMoveWindow(poWindow*, poRect);
void		applicationMakeWindowCurrent(poWindow*);
void		applicationMakeWindowFullscreen(poWindow*,bool);

float		getWindowWidth();
float		getWindowHeight();
poRect		getWindowBounds();
float		getWindowFramerate();
float		getWindowLastFrameTime();
float		getWindowLastFrameDuration();

// define to generate base objects for a window after the gl context is created
extern poObject* createObjectForID(uint uid);
// configure the number of windows in an application
extern void		setupApplication();
// delete any global storage associated with the app
extern void		cleanupApplication();
