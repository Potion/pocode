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

poPoint getWindowDimensions() {
	poWindow *window = applicationCurrentWindow();
	return window->dimensions();
}

float getWindowAspect() {
	poWindow *window = applicationCurrentWindow();
	return window->width() / (float)window->height();
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

int getWindowFrameCount() {
	poWindow *window = applicationCurrentWindow();
	return window->framecount();
}

float getWindowLastFrameTime() {
	poWindow *window = applicationCurrentWindow();
	return window->lastFrameTime();
}

float getWindowLastFrameDuration() {
	poWindow *window = applicationCurrentWindow();
	return window->lastFrameElapsed();
}

poPoint getWindowMousePosition() {
	poWindow *window = applicationCurrentWindow();
	return window->mousePosition();
}

poPoint getWindowInvMousePosition() {
	poWindow *window = applicationCurrentWindow();
	poPoint response = window->mousePosition();
	response.y = window->bounds().height() - response.y;
	return response;
}