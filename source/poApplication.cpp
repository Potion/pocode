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

#include "poApplication.h"
#include "poWindow.h"
#include "poImage.h"
#include "poTexture.h"

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

void setWindowMouseMoveEnabled(bool b) {
	poWindow *window = applicationCurrentWindow();
	window->setMouseMoveEnabled(b);
}
bool getWindowMouseMoveEnabled() {
	poWindow *window = applicationCurrentWindow();
	return window->isMouseMoveEnabled();
}


