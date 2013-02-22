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

namespace po {

    //------------------------------------------------------------------------
    float getWindowWidth() {
        Window *window = applicationCurrentWindow();
        return window->getWidth();
    }
    
    
    //------------------------------------------------------------------------
    float getWindowHeight() {
        Window *window = applicationCurrentWindow();
        return window->getHeight();
    }
    
    
    //------------------------------------------------------------------------
    Point getWindowDimensions() {
        Window *window = applicationCurrentWindow();
        return window->getDimensions();
    }
    
    
    //------------------------------------------------------------------------
    float getWindowAspect() {
        Window *window = applicationCurrentWindow();
        return window->getWidth() / (float)window->getHeight();
    }
    
    
    //------------------------------------------------------------------------
    Rect getWindowFrame() {
        Window *window = applicationCurrentWindow();
        return window->getFrame();
    }
    
    
    //------------------------------------------------------------------------
    Rect getWindowBounds() {
        Window *window = applicationCurrentWindow();
        return window->getBounds();
    }
    
    
    //------------------------------------------------------------------------
    float getScale() {
        Window *window = applicationCurrentWindow();
        return window->getScale();
    }
    
    
    //------------------------------------------------------------------------
    Point getWindowCenter() {
        Window *window = applicationCurrentWindow();
        return window->getCenterPoint();
    }
    
    
    //------------------------------------------------------------------------
    float getWindowFramerate() {
        Window *window = applicationCurrentWindow();
        return window->getFramerate();
    }
    
    
    //------------------------------------------------------------------------
    int getWindowFrameCount() {
        Window *window = applicationCurrentWindow();
        return window->getFramecount();
    }
    
    
    //------------------------------------------------------------------------
    float getWindowLastFrameTime() {
        Window *window = applicationCurrentWindow();
        return window->getLastFrameTime();
    }
    
    
    //------------------------------------------------------------------------
    float getWindowLastFrameDuration() {
        Window *window = applicationCurrentWindow();
        return window->getLastFrameElapsed();
    }
    
    
    //------------------------------------------------------------------------
    Point getWindowMousePosition() {
        Window *window = applicationCurrentWindow();
        return window->getMousePosition();
    }
    
    
    //------------------------------------------------------------------------
    Point getWindowInvMousePosition() {
        Window *window = applicationCurrentWindow();
        Point response = window->getMousePosition();
        response.y = window->getBounds().height - response.y;
        return response;
    }
    
    
    //------------------------------------------------------------------------
    void setWindowMouseMoveEnabled(bool b) {
        Window *window = applicationCurrentWindow();
        window->setMouseMoveEnabled(b);
    }
    
    
    //------------------------------------------------------------------------
    bool getWindowMouseMoveEnabled() {
        Window *window = applicationCurrentWindow();
        return window->isMouseMoveEnabled();
    }
} /* End po namespace */