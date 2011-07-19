#include "poApplication.h"
#include "poWindow.h"

float getWindowWidth() {
	poWindow *window = applicationCurrentWindow();
	return window->width();
}

float getWindowHeight() {
	poWindow *window = applicationCurrentWindow();
	return window->height();
}

poRect getWindowFrame() {
	poWindow *window = applicationCurrentWindow();
	return window->frame();
}

poRect getWindowBounds() {
	poWindow *window = applicationCurrentWindow();
	return window->bounds();
}

poPoint getWindowCenter() {
	poWindow *window = applicationCurrentWindow();
	return window->centerPoint();
}

float getWindowFramerate() {
	poWindow *window = applicationCurrentWindow();
	return window->framerate();
}

float getWindowLastFrameTime() {
	poWindow *window = applicationCurrentWindow();
	return window->lastFrameTime();
}

float getWindowLastFrameDuration() {
	poWindow *window = applicationCurrentWindow();
	return window->lastFrameElapsed();
}
