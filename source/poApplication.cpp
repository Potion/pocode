#include "poApplication.h"
#include "poWindow.h"

float getWindowWidth() {
	poWindow *window = applicationCurrentWindow();
	return window->getWidth();
}

float getWindowHeight() {
	poWindow *window = applicationCurrentWindow();
	return window->getHeight();
}

poPoint getWindowDimensions() {
	poWindow *window = applicationCurrentWindow();
	return window->getDimensions();
}

float getWindowAspect() {
	poWindow *window = applicationCurrentWindow();
	return window->getWidth() / (float)window->getHeight();
}

poRect getWindowFrame() {
	poWindow *window = applicationCurrentWindow();
	return window->getFrame();
}

poRect getWindowBounds() {
	poWindow *window = applicationCurrentWindow();
	return window->getBounds();
}


poPoint getWindowCenter() {
	poWindow *window = applicationCurrentWindow();
	return window->getCenterPoint();
}

float getWindowFramerate() {
	poWindow *window = applicationCurrentWindow();
	return window->getFramerate();
}

int getWindowFrameCount() {
	poWindow *window = applicationCurrentWindow();
	return window->getFramecount();
}

float getWindowLastFrameTime() {
	poWindow *window = applicationCurrentWindow();
	return window->getLastFrameTime();
}

float getWindowLastFrameDuration() {
	poWindow *window = applicationCurrentWindow();
	return window->getLastFrameElapsed();
}

poPoint getWindowMousePosition() {
	poWindow *window = applicationCurrentWindow();
	return window->getMousePosition();
}

poPoint getWindowInvMousePosition() {
	poWindow *window = applicationCurrentWindow();
	poPoint response = window->getMousePosition();
	response.y = window->getBounds().height - response.y;
	return response;
}