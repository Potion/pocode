#include "poObject.h"
#include "poWindow.h"

#include "Helpers.h"
#include "poOpenGLState.h"
#include "poApplication.h"

void objUnderPoint(poObject *obj, poPoint &pnt, std::set<poObject*> &objsBeneath) {
	if(!(obj->visible && obj->alpha > 0.01))
		return; 
	
	for(int i=obj->numChildren()-1; i>=0; i--) {
		objUnderPoint(obj->getChild(i), pnt, objsBeneath);
	}

	if(obj->pointInside(pnt,true))
		objsBeneath.insert(obj);
}

poWindow::poWindow(const char *title, uint root_id, poRect b)
:	title_(title)
,	handle(NULL)
,	root(NULL)
,	root_id(root_id)
,	_bounds(b)
,   key_receiver(NULL)
,	fullscreen_(false)
,	closed_(false)
,	framecounter(0)
,	total_framecount(0)
,	last_mark(0.0)
,	framerate_(0.f)
{
}

poWindow::~poWindow() {
	makeCurrent();
	delete root;
}

void poWindow::moveTo(poPoint p) {
	applicationMoveWindow(this, p);
}

void poWindow::fullscreen(bool b) {
	fullscreen_ = b;
	applicationMakeWindowFullscreen(this, b);
}

int poWindow::x() const {
	return _bounds.x;
}

std::string poWindow::title() const {
	return title_;
}

int poWindow::y() const {
	return _bounds.y;
}

int poWindow::width() const {
	return _bounds.width;
}

poPoint poWindow::dimensions() const {
	return _bounds.getSize();
}

poRect poWindow::frame() const {
	return _bounds;
}

poRect poWindow::bounds() const {
	return poRect(poPoint(0,0), _bounds.getSize());
}

poPoint poWindow::centerPoint() const {
	return poPoint(_bounds.width/2.f, _bounds.height/2.f);
}

int poWindow::height() const {
	return _bounds.height;
}

float poWindow::framerate() const {
	return framerate_;
}

int poWindow::framecount() const {
	return total_framecount;
}

float poWindow::lastFrameElapsed() const {
	return last_elapsed;
}

float poWindow::lastFrameTime() const {
	return last_frame;
}

bool poWindow::isFullscreen() const {
	return fullscreen_;
}

poObject *poWindow::rootObject() {
	if(!root) {
		makeCurrent();
		poOpenGLState::get()->setBlend(po::BlendState::defaultBlending());
		root = createObjectForID(root_id);
	}
	return root;
}

poPoint poWindow::mousePosition() const {
	return mouse_pos;
}

void poWindow::makeCurrent() {
	applicationMakeWindowCurrent(this);
}

void poWindow::update() {
	double now = getTime();
	
	total_framecount++;
	framecounter++;
	if(now - last_mark >= 1.0) {
		last_mark = now;
		framerate_ = framecounter;
		framecounter = 0;
	}
	
	last_elapsed = now - last_frame;
	last_frame = now;
	
	processEvents();
	
	draw_order_counter = 0;
	
	rootObject()->_updateTree();
}

void poWindow::draw() {
    poEventCenter::get()->clearDrawOrderForObjectsWithEvents();
	rootObject()->_drawTree();
}

bool sortByDrawOrder(poObject *a, poObject *b) {
	return a->drawOrder() < b->drawOrder();
}

struct removeIfNoEvent{
	int type;
	removeIfNoEvent(int t) : type(t) {}
	bool operator()(poObject *obj) {
		return !poEventCenter::get()->objectHasEvent(obj,type);
	}
};

void sortedFilteredVectorFromSet(std::set<poObject*> &objs, std::vector<poObject*> &sorted, int type) {
	sorted.assign(objs.begin(), objs.end());
	sorted.erase(std::remove_if(sorted.begin(), sorted.end(), removeIfNoEvent(type)), sorted.end());
	std::sort(sorted.begin(), sorted.end(), sortByDrawOrder);
}

void poWindow::processEvents() {
   if(!handle) {
		received.clear();
		return;
	}
	
	// if there aren't any events, bail
	if(received.empty())
		return;
	
    // sort all event callbacks by draw order
    poEventCenter::get()->sortCallbacksByDrawOrder();
    
	while(!received.empty()) {
        //Get Event
		poEvent event = received.front();

		received.pop_front();
        
        //Check type, take appropriate action
        if(isMouseEvent(event.type)) {
            poEventCenter::get()->processMouseEvents(event);
        }
        
        if(isTouchEvent(event.type)) {
            poEventCenter::get()->processTouchEvents(event);
        }
        
        else if(isKeyEvent(event.type)) {
            poEventCenter::get()->processKeyEvents(event);
        }
        
        else if( event.type == PO_WINDOW_RESIZED_EVENT) {
                // everyone who cares gets resize
                // maybe this should be broadcast
            
                poEventCenter::get()->notifyAllListeners(event);
                //poEventCenter::get()->notify(event);
        }
        
        else {
            std::cout << "Unknown event type!" << std::endl;
        }
    }
}


void poWindow::processInteractionEvent(poEvent event) {
    //Get the appropriate point (mouse or touch)
    interactionPoint *pt;
    
    if(isTouchEvent(event.type)) {
        pt = getTouch(event.uid);
    } else {
        pt = &mouse;
    }
    
    //Get the Objects Beneath
    std::set<poObject*> objsBeneath;
    objUnderPoint(rootObject(), event.position, objsBeneath);
    
    //Find objects that the point entered and left
    std::set<poObject*> did_enter;
	std::set<poObject*> did_leave;
    setEnterLeave(objsBeneath, pt->prevObjsBeneath, did_enter, did_leave);
    
    //Filter for inside/outside events
    std::set<poObject*> filter;
    std::vector<poObject*> listeningObjs;
    
    
    // Event coming through should be an Everywhere event (from functions below)
    //so notify subscribers
    poEventCenter::get()->notify(event);
    
    
    //Dispatch Appropriate touch or mouse events
    switch (event.type) {
        case PO_MOUSE_DOWN_EVERYWHERE_EVENT:
        case PO_TOUCH_BEGAN_EVERYWHERE_EVENT:
            //Send events to objects beneath, set drag target, etc
            if(!objsBeneath.empty()) {
                event.type = isTouchEvent(event.type) ? PO_TOUCH_BEGAN_INSIDE_EVENT : PO_MOUSE_DOWN_INSIDE_EVENT;
                
                //Find listening objects
                sortedFilteredVectorFromSet(objsBeneath, listeningObjs, event.type);
                
                // if an object is under the interaction point (and listening) make them the drag target
                pt->dragTarget = listeningObjs.back();
                filter.insert(pt->dragTarget);
                
                // Filtered object gets inside event
                poEventCenter::get()->notifyFiltered(event, filter, false);
            }
            
            // Everything else gets outside event
            event.type = isTouchEvent(event.type) ? PO_TOUCH_BEGAN_OUTSIDE_EVENT : PO_MOUSE_DOWN_OUTSIDE_EVENT;
            poEventCenter::get()->notifyFiltered(event, filter, true);
            break;
            
        case PO_MOUSE_MOVE_EVENT:
        case PO_TOUCH_MOVED_EVERYWHERE_EVENT:
            // Notify ENTER/LEAVE Subscribers
            if(!objsBeneath.empty()) {
                event.type = isTouchEvent(event.type) ? PO_TOUCH_OVER_EVENT : PO_MOUSE_OVER_EVENT;
                poEventCenter::get()->notifyFiltered(event, objsBeneath, false);
            }
            
            //Notify Enter Objs
            if(!did_enter.empty()) {
                event.type = isTouchEvent(event.type) ? PO_TOUCH_ENTER_EVENT : PO_MOUSE_ENTER_EVENT;
                poEventCenter::get()->notifyFiltered(event, did_enter, false);
            }
            
            //Notify Leave objs
            if(!did_leave.empty()) {
                event.type = isTouchEvent(event.type) ? PO_TOUCH_LEAVE_EVENT : PO_MOUSE_LEAVE_EVENT;
                poEventCenter::get()->notifyFiltered(event, did_leave, false);
            }
            break;
            
        case PO_MOUSE_UP_EVERYWHERE_EVENT:
        case PO_TOUCH_ENDED_EVERYWHERE_EVENT:
            // Kill the drag target (no longer dragging)
            pt->dragTarget = NULL;
            
            // Filtered object gets inside event
            if(!objsBeneath.empty()) {
                event.type = isTouchEvent(event.type) ? PO_TOUCH_ENDED_INSIDE_EVENT : PO_MOUSE_UP_INSIDE_EVENT;
                
                sortedFilteredVectorFromSet(objsBeneath, listeningObjs, event.type);
                filter.insert(listeningObjs.back());
                poEventCenter::get()->notifyFiltered(event, filter, false);
            }
            
            //Everyone else gets outside event
            event.type = isTouchEvent(event.type) ? PO_TOUCH_ENDED_OUTSIDE_EVENT : PO_MOUSE_UP_OUTSIDE_EVENT;
            poEventCenter::get()->notifyFiltered(event, filter, true);
            break;
    }
}


void poWindow::setEnterLeave(std::set<poObject*> &objsBeneath, std::set<poObject*> &prevObjsBeneath, std::set<poObject*> &enter, std::set<poObject*> &leave) {
    enter.clear();
    leave.clear();
    
    // figure out who isn't in the list anymore
    std::set_difference(prevObjsBeneath.begin(), prevObjsBeneath.end(),
                        objsBeneath.begin(), objsBeneath.end(),
                        std::inserter(leave, leave.end()));
    
    // figure out who's new in the list
    std::set_difference(objsBeneath.begin(), objsBeneath.end(),
                        prevObjsBeneath.begin(), prevObjsBeneath.end(),
                        std::inserter(enter, enter.end()));
    
    // reset the list for next frame
    prevObjsBeneath.clear();
    prevObjsBeneath.insert(objsBeneath.begin(), objsBeneath.end());
}


void poWindow::processKeyEvent(poEvent event) {
    switch (event.type) {
        case PO_KEY_DOWN_EVENT:
            poEventCenter::get()->notify(event);
            break;
            
        case PO_KEY_UP_EVENT:
            poEventCenter::get()->notify(event);
            break;
            
        default:
            // just eat it
            break;
    }
}

void poWindow::mouseDown(int x, int y, int mod) {
	mouse_pos.set(x,y,1);
	
	poEvent event;
	event.position.set(x, y, 0.f);
	event.modifiers = mod;
	
	event.type = PO_MOUSE_DOWN_EVERYWHERE_EVENT;
	received.push_back(event);
}

void poWindow::mouseUp(int x, int y, int mod) {
	mouse_pos.set(x,y,1);
	
	poEvent event;
	event.position.set(x, y, 0.f);
	event.modifiers = mod;

	event.type = PO_MOUSE_UP_EVERYWHERE_EVENT;
	received.push_back(event);
}

void poWindow::mouseMove(int x, int y, int mod) {
	mouse_pos.set(x,y,1);
	
	poEvent event;
	event.position.set(x, y, 0.f);
	event.modifiers = mod;
	
	event.type = PO_MOUSE_MOVE_EVENT;
	received.push_back(event);
}

void poWindow::mouseDrag(int x, int y, int mod) {
	mouse_pos.set(x,y,1);
	
	poEvent event;
	event.position.set(x, y, 0.f);
	event.modifiers = mod;
	
	event.type = PO_MOUSE_DRAG_EVERYWHERE_EVENT;
	received.push_back(event);
}

void poWindow::mouseWheel(int x, int y, int mod, int num_steps) {
}

void poWindow::keyDown(int key, int code, int mod) {
	poEvent event;
	event.keyChar = key;
	event.keyCode = code;
	event.modifiers = mod;
	
	event.type = PO_KEY_DOWN_EVENT;
	received.push_back(event);
}

void poWindow::keyUp(int key, int code, int mod) {
	poEvent event;
	event.keyCode = code;
	event.keyChar = key;
	event.modifiers = mod;
	
	event.type = PO_KEY_UP_EVENT;
	received.push_back(event);
}


void poWindow::resized(int w, int h) {
	resized(_bounds.x, _bounds.y, w, h);
}


void poWindow::resized(int x, int y, int w, int h) {
	_bounds.set(x,y,w,h);

	poEvent event;
	event.type = PO_WINDOW_RESIZED_EVENT;
	received.push_back(event);
}


void poWindow::touchBegin(int x, int y, int uid, int tapCount )
{
    //Create an interactionPoint for this touch, with a unique id
    interactionPoint *t = new interactionPoint();
    t->uid = uid;
    t->bIsDead = false;
    
    //Begin tracking touch + give it a simple ID (0-100)
    trackTouch(t);
    
    //Fire Event
	poEvent event;
	event.position.set(x, y, 0.f);
	event.touchID = t->id;
    event.uid   = t->uid;
    event.tapCount = tapCount;
	
	event.type = PO_TOUCH_BEGAN_EVERYWHERE_EVENT;
	received.push_back(event);
}


void poWindow::touchMove(int x, int y, int uid, int tapCount )
{
    //Get the corresponding tracked object
    interactionPoint *t = getTouch(uid);
    
    //Send event
	poEvent event;
	event.position.set(x, y, 0.f);
	event.touchID   = t->id;
    event.uid       = t->uid;
    event.tapCount  = tapCount;
	
	event.type = PO_TOUCH_MOVED_EVERYWHERE_EVENT;
	received.push_back(event);
}


void poWindow::touchEnd(int x, int y, int uid, int tapCount )
{
    //Get the corresponding tracked object
    interactionPoint *t = getTouch(uid);
   
    //Send event
	poEvent event;
	event.position.set(x, y, 0.f);
	event.touchID   = t->id;
    event.uid       = t->uid;
    event.tapCount = tapCount;
	
	event.type = PO_TOUCH_ENDED_EVERYWHERE_EVENT;
    received.push_back(event);
    
    untrackTouch(uid);
}


void poWindow::touchCancelled(int x, int y, int id, int tapCount )
{
    untrackTouch(id);
    poEvent event;
	event.position.set(x, y, 0.f);
	event.touchID = id;
    event.tapCount = tapCount;
	
	event.type = PO_TOUCH_CANCELLED_EVENT;
	received.push_back(event);
}


void poWindow::trackTouch(interactionPoint *t) 
{
    int totalTouches = trackedTouches.size();
        
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
    t->id = trackedTouches.size();
    trackedTouches.push_back(t);
}


interactionPoint *poWindow::getTouch(int uid) 
{
    for(int i=0;i<trackedTouches.size(); i++) {
        if(trackedTouches[i]->uid == uid) {
            return trackedTouches[i];
        }
    }
}


void poWindow::untrackTouch(int uid) 
{
    interactionPoint *t = getTouch(uid);
    t->bIsDead = true;
}

void *poWindow::getWindowHandle() {
	return handle;
}

void poWindow::setWindowHandle(void *handle) {
	this->handle = handle;
}

int poWindow::nextDrawOrder() {
	return draw_order_counter++;
}
