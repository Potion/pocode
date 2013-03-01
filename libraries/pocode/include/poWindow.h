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

#include "poRect.h"
#include "poPoint.h"
#include "poEvent.h"

#include <set>
#include <deque>

class poObject;

// CLASS NOTES
//
// This class implements a platform-independent window. The poWindow holds the root poObject
// of the scene graph, and is responsible for the continuous draw/update loop. It also
// receives events (mouse, keyboard, touch, etc) from the operating system and turns them into
// poEvents, which are processed and propagated to poObjects that have registed for them.
// pocode can support multiple windows. Each window may contains its own scene graph,
// or may share a common scene graph.

typedef struct {
    int id;
    int uid;
    std::set<poObject *> prevObjsBeneath;
    poObject *dragTarget;
    
    bool bIsDead;
} interactionPoint;

extern const std::string WindowResizedNotification;

class poWindow {
public:
	poWindow(const char *title, uint rootID, poRect bounds, float scale=1.0f);
	~poWindow();

    // WINDOW MANAGEMENT
	void            moveTo(poPoint p);
	void            setFullscreen(bool b);
	void            makeCurrent();
    
    void            *getWindowHandle();
	void            setWindowHandle(void *handle);

    // WINDOW UPDATE AND DRAW
	void            update();
	void            draw();
    
    // WINDOW PROPERTIES
	std::string     getTitle() const;
	float			getX() const;
	float           getY() const;
	float           getWidth() const;
	float			getHeight() const;
	poPoint         getDimensions() const;
    float           getScale() const;
	poRect          getFrame() const;
	poRect          getBounds() const;
	poPoint         getCenterPoint() const;
	float           getFramerate() const;
	int             getFramecount() const;
	float           getLastFrameElapsed() const;
	float           getLastFrameTime() const;
	bool            isFullscreen() const;
	bool            wasClosed() const;
    poPoint         getMousePosition() const;
	poObject*       getRootObject();
	bool			isMouseMoveEnabled() const;
	void			setMouseMoveEnabled(bool b);
    
    

    // EVENT RECEIVING METHODS
    // These methods are called by the platform-specific windowing system. On the Mac and iOS
    // these event receiving methods are called from within poOpenGLView.mm.
    // Do not call these methods directly.
	void            mouseDown(int x, int y, int mod);
	void            mouseUp(int x, int y, int mod);
	void            mouseMove(int x, int y, int mod);
	void            mouseDrag(int x, int y, int mod);
	void            mouseWheel(int x, int y, int mod, int num_steps);
	void            keyDown(int key, int code, int mod);
	void            keyUp(int key, int code, int mod);
	void            resized(int w, int h);
	void            resized(int x, int y, int w, int h);
	
	void            touchBegin(int x, int y, int uid, int tapCount );
	void            touchMove(int x, int y, int uid, int tapCount );
	void            touchEnd(int x, int y, int uid, int tapCount );
    void            touchCancelled(int x, int y, int uid, int tapCount );
    
    void            accelerometerEvent(double x, double y, double z);
    void            gyroscopeEvent(double x, double y, double z);
    void            rotationEvent();
	
	// DRAW ORDER COUNTER
    // The draw order of every poObject in the scene graph is set based upon the drawOrderCounter.
    // The draw order is used by the event system to determine which objects are drawn on top of others.
	int             getNextDrawOrder();
	
	void			setBlockAllEvent(bool b);

private:
    
    // THE SCENE GRAPH ROOT NODE
	poObject        *root;
	uint			rootID;
    
    // EVENT PROCESSING
	// Incoming events are stored in the "received" event queue. This event queue is processed and then cleared
    // once per frame by the processEvents method. Queuing the events allows for thread-safe operation.
	void                    processEvents();
    
    std::deque<poEvent>     received;
	
    // WINDOW PROPERTIES (PRIVATE)
	bool            closed;
	poRect          bounds;
    float           scale;
	bool            fullscreen;
	void            *handle;
	std::string     title;
	
    // FRAME COUNTING and FRAME RATE
	float           lastMark, lastFrame;
	int             framecounter, totalFramecount;
	float           framerate, lastElapsed;
	
	// GLOBAL MOUSE POSITION
	poPoint         mousePos;
	bool			mouseMoveEnabled;
    
    // TOUCH EVENTS
    std::vector<interactionPoint *> trackedTouches;
    void trackTouch(interactionPoint *t);
    interactionPoint * getTouch(int uid);
    void untrackTouch(int uid);
	
    // DRAW ORDER COUNTER
    int drawOrderCounter;
	
	bool block_all_events;
};


