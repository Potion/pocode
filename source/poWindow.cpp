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

#include "poObject.h"
#include "poWindow.h"

#include "poThreadCenter.h"
#include "poMessageCenter.h"

#include "poHelpers.h"
#include "poOpenGLState.h"
#include "poApplication.h"

#include "poTexture.h"
#include "poImage.h"

namespace po {
    void objUnderPoint(Object *obj, Point &pnt, std::set<Object*> &objsBeneath) {
        if(!(obj->visible && obj->alpha > 0.01))
            return;
        
        for(int i=obj->getNumChildren()-1; i>=0; i--) {
            objUnderPoint(obj->getChild(i), pnt, objsBeneath);
        }

        if(obj->pointInside(pnt,true))
            objsBeneath.insert(obj);
    }

    
    Window::Window(const char *title, uint rootID, Rect b, float s)
    :	title(title)
    ,	handle(NULL)
    ,	root(NULL)
    ,	rootID(rootID)
    ,	bounds(b)
    ,   scale(s)
    //,   key_receiver(NULL)
    ,	fullscreen(false)
    ,	closed(false)
    ,	framecounter(0)
    ,	totalFramecount(0)
    ,	lastMark(0.0)
    ,	framerate(0.f)
    ,   useAlphaForEvents(true)
    ,	mouseMoveEnabled(true)
    ,	block_all_events(false)
    {
    }

    Window::~Window() {
        makeCurrent();
        delete root;
    }
    
    
    //------------------------------------------------------------------------
    void Window::update() {
        float now = po::getElapsedTime();
        
        totalFramecount++;
        framecounter++;
        if(now - lastMark >= 1.0) {
            lastMark = now;
            framerate = (float)framecounter;
            framecounter = 0;
        }
        
        lastElapsed = now - lastFrame;
        lastFrame = now;
        
        // handle events
        if(handle && !received.empty()) {
            EventCenter::get()->processEvents(received);
        }
        received.clear();
        
        //Update internal classes
        MessageCenter::update();
        ThreadCenter::update();
        
        
        // tell everyone who cares they should update
        updateSignal();
        
        // update the objects
        getRootObject()->updateTree();
    }
    
    
    //------------------------------------------------------------------------
    SigConn Window::addUpdate(const boost::function<void()> &func) {
        return updateSignal.connect(func);
    }
    
    
    //------------------------------------------------------------------------
    void Window::setBlockAllEvent(bool b) {
        block_all_events = b;
    }
    
    
    //------------------------------------------------------------------------
    void Window::draw() {
        drawOrderCounter = 0;
        EventCenter::get()->negateDrawOrderForObjectWithEvents();
        getRootObject()->drawTree();
    }
    
    
    //------------------------------------------------------------------------
    std::string Window::getTitle() const {
        return title;
    }
    
    
    //------------------------------------------------------------------------
    Object *Window::getRootObject() {
        if(!root) {
            makeCurrent();
            po::initGraphics();
            po::enableAlphaBlending();
            po::setViewport(Rect(0,0,bounds.width,bounds.height));
            root = po::createObjectForID(rootID);
        }
        return root;
    }
	
    //------------------------------------------------------------------------
    void Window::makeCurrent() {
        po::applicationMakeWindowCurrent(this);
    }
    
    
    //------------------------------------------------------------------------
    void *Window::getWindowHandle() {
        return handle;
    }
    
    
    //------------------------------------------------------------------------
    void Window::setWindowHandle(void *handle) {
        this->handle = handle;
    }
    
    
    //------------------------------------------------------------------------
    int Window::getNextDrawOrder() {
        return drawOrderCounter++;
    }
    
    
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Position =========================================
    #pragma mark Position
    
    //------------------------------------------------------------------------
    float Window::getX() const {
        return bounds.x;
    }
    
    
    //------------------------------------------------------------------------
    float Window::getY() const {
        return bounds.y;
    }
    
    //------------------------------------------------------------------------
    void Window::moveTo(Point p) {
        po::applicationMoveWindow(this, p);
    }
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Dimensions =======================================
    #pragma mark Dimensions
    
    
    //------------------------------------------------------------------------
    void Window::setFullscreen(bool b) {
        fullscreen = b;
        po::applicationMakeWindowFullscreen(this, b);
    }
    
    
    //------------------------------------------------------------------------
    bool Window::isFullscreen() const {
        return fullscreen;
    }
    
    
    //------------------------------------------------------------------------
    Point Window::getDimensions() const {
        return bounds.getSize();
    }
    
    
    //------------------------------------------------------------------------
    Rect Window::getFrame() const {
        return bounds;
    }
    
    
    //------------------------------------------------------------------------
    float Window::getWidth() const {
        return bounds.width;
    }
    
    
    //------------------------------------------------------------------------
    float Window::getHeight() const {
        return bounds.height;
    }
    
    
    //------------------------------------------------------------------------
    Rect Window::getBounds() const {
        return Rect(Point(0,0), bounds.getSize());
    }
    
    
    //------------------------------------------------------------------------
    Point Window::getCenterPoint() const {
        return Point(bounds.width/2.f, bounds.height/2.f);
    }
    
    
    //------------------------------------------------------------------------
    //For Retina Displays, scale of content (ie @2x stuff), not window
    float Window::getScale() const {
        return scale;
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Framerate ========================================
    #pragma mark Framerate
    
    //------------------------------------------------------------------------
    float Window::getFramerate() const {
        return framerate;
    }
    
    
    //------------------------------------------------------------------------
    int Window::getFramecount() const {
        return totalFramecount;
    }
    
    
    //------------------------------------------------------------------------
    float Window::getLastFrameElapsed() const {
        return lastElapsed;
    }
    
    
    //------------------------------------------------------------------------
    float Window::getLastFrameTime() const {
        return lastFrame;
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Window Events ====================================
    #pragma mark Window Events
    
    //------------------------------------------------------------------------
    void Window::resized(int x, int y, int w, int h) {
        po::setViewport(0, 0, w, h);
        bounds.set(x,y,w,h);
        
        MessageCenter::broadcastMessage(po::WindowResizedNotification);
    }
    
    
    //------------------------------------------------------------------------
    void Window::resized(int w, int h) {
        resized(bounds.x, bounds.y, w, h);
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Events =====================================
    #pragma mark Events
    
    void Window::setUseAlphaForEvents(bool useAlpha) {
        useAlphaForEvents = useAlpha;
    }
    
    bool Window::getUseAlphaForEvents() {
        return useAlphaForEvents;
    }
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Mouse Events =====================================
    #pragma mark Mouse Events
    
    //------------------------------------------------------------------------
    Point Window::getMousePosition() const {
        return mousePos;
    }
    
    
    //------------------------------------------------------------------------
    bool Window::isMouseMoveEnabled() const {
        return mouseMoveEnabled;
    }
    
    
    //------------------------------------------------------------------------
    void Window::setMouseMoveEnabled(bool b) {
        mouseMoveEnabled = b;
    }
    
    
    //------------------------------------------------------------------------
    void Window::mouseDown(int x, int y, int mod) {
        mousePos.set(x,y,1.f);
        
        Event event;
        event.globalPosition.set(x, y, 0.f);
        event.modifiers = mod;
        
        event.type = po::MOUSE_DOWN_EVENT;
        received.push_back(event);
    }
    
    
    //------------------------------------------------------------------------
    void Window::mouseUp(int x, int y, int mod) {
        mousePos.set(x,y,1);
        
        Event event;
        event.globalPosition.set(x, y, 0.f);
        event.modifiers = mod;
        
        event.type = po::MOUSE_UP_EVENT;
        received.push_back(event);
    }
    
    
    //------------------------------------------------------------------------
    void Window::mouseMove(int x, int y, int mod) {
        if(!mouseMoveEnabled)
            return;
        
        mousePos.set(x,y,1);
        
        Event event;
        event.globalPosition.set(x, y, 0.f);
        event.modifiers = mod;
        
        event.type = po::MOUSE_MOVE_EVENT;
        received.push_back(event);
    }
    
    
    //------------------------------------------------------------------------
    void Window::mouseDrag(int x, int y, int mod) {
        mousePos.set(x,y,1);
        
        Event event;
        event.globalPosition.set(x, y, 0.f);
        event.modifiers = mod;
        
        event.type = po::MOUSE_DRAG_EVENT;
        received.push_back(event);
    }
    
    
    //------------------------------------------------------------------------
    void Window::mouseWheel(int x, int y, int mod, int num_steps) {
    }    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Key Events =======================================
    #pragma mark Key Events
    
    //------------------------------------------------------------------------
    void Window::keyDown(int key, int code, int mod) {
        Event event;
        event.keyChar = key;
        event.keyCode = code;
        event.modifiers = mod;
        
        event.type = po::KEY_DOWN_EVENT;
        received.push_back(event);
    }
    
    
    //------------------------------------------------------------------------
    void Window::keyUp(int key, int code, int mod) {
        Event event;
        event.keyCode = code;
        event.keyChar = key;
        event.modifiers = mod;
        
        event.type = po::KEY_UP_EVENT;
        received.push_back(event);
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Touch Events =======================================
    #pragma mark Touch Events
    
    //------------------------------------------------------------------------
    void Window::touchBegin(int x, int y, int uid, int tapCount ) {
        //Create an interactionPoint for this touch, with a unique id
        interactionPoint *t = new interactionPoint();
        t->uid = uid;
        t->bIsDead = false;
        
        //Begin tracking touch + give it a simple ID (0-100)
        trackTouch(t);
            
        //Fire Event
        Event event;
        event.globalPosition.set(x, y, 0.f);
        event.touchID   = t->id;
        event.uniqueID  = uid;
        event.tapCount  = tapCount;
        
        event.type = po::TOUCH_BEGAN_EVENT;
        received.push_back(event);
    }
    
    
    //------------------------------------------------------------------------
    void Window::touchMove(int x, int y, int uid, int tapCount ) {
        //Get the corresponding tracked object
        interactionPoint *t = getTouch(uid);
        
        if(t != NULL) {
            //Send event
            Event event;
            event.globalPosition.set(x, y, 0.f);
            event.touchID   = t->id;
            event.uniqueID  = uid;
            event.tapCount  = tapCount;
            
            event.type = po::TOUCH_MOVED_EVENT;
            received.push_back(event);
        }
    }
    
    
    //------------------------------------------------------------------------
    void Window::touchEnd(int x, int y, int uid, int tapCount) {
        //Get the corresponding tracked object
        interactionPoint *t = getTouch(uid);
       
        if(t != NULL) {
            //Send event
            Event event;
            event.globalPosition.set(x, y, 0.f);
            event.touchID   = t->id;
            event.uniqueID  = uid;
            event.tapCount  = tapCount;
            
            event.type = po::TOUCH_ENDED_EVENT;
            received.push_back(event);
            
            untrackTouch(uid);
        }
    }
    
    
    //------------------------------------------------------------------------
    void Window::touchCancelled(int x, int y, int uid, int tapCount) {
        untrackTouch(uid);
        
        Event event;
        event.globalPosition.set(x, y, 0.f);
        event.touchID = uid;
        event.tapCount = tapCount;
        
        event.type = po::TOUCH_CANCELLED_EVENT;
        received.push_back(event);
        
        untrackTouch(uid);
    }
    
    
    //------------------------------------------------------------------------
    void Window::trackTouch(interactionPoint *t) {
        size_t totalTouches = trackedTouches.size();
            
        //See if there are any empty slots
        for(int i=0; i<totalTouches; i++) {
            if(trackedTouches[i]->bIsDead) {
                //Delete old touch
                delete trackedTouches[i];
                
                //Set id
                t->id = i;
            
                //Track in this spot
                trackedTouches[i] = t;
                return;
            }
        }
        
        //If the touch wasn't found, add it
        t->id = (int)trackedTouches.size();
        trackedTouches.push_back(t);
    }
    
    
    //------------------------------------------------------------------------
    interactionPoint *Window::getTouch(int uid) {
        for(uint i=0;i<trackedTouches.size(); i++) {
            if(trackedTouches[i]->uid == uid) {
                return trackedTouches[i];
            }
        }
        return NULL;
    }
    
    
    //------------------------------------------------------------------------
    void Window::untrackTouch(int uid) {
        interactionPoint *t = getTouch(uid);
        t->bIsDead = true;
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Motion Events ====================================
    #pragma mark Motion Events
    
    //------------------------------------------------------------------------
    void Window::accelerometerEvent(double x, double y, double z) {
        Event event;
        event.motion.set(x, y, z);
        event.type = po::ACCELEROMETER_EVENT;
        received.push_back(event);
    }
    
    
    //------------------------------------------------------------------------
    void Window::gyroscopeEvent(double x, double y, double z) {
        Event event;
        event.motion.set(x, y, z);
        event.type = po::GYROSCOPE_EVENT;
        received.push_back(event);
    }
    
    
    //------------------------------------------------------------------------
    void Window::rotationEvent() {
        Event event;
        event.type = po::ROTATION_EVENT;
        received.push_back(event);
    }
} /* End po Namespace */
