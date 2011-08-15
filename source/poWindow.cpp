#include "poObject.h"
#include "poWindow.h"

#include "Helpers.h"
#include "poApplication.h"

void objUnderMouse(poObject *obj, poPoint &mouse, std::set<poObject*> &hovers) {
	if(!(obj->isInWindow() && obj->visible && obj->alpha > 0.01))
		return; 
	
	for(int i=obj->numChildren()-1; i>=0; i--) {
		objUnderMouse(obj->getChild(i), mouse, hovers);
	}

	if(obj->pointInside(mouse,true))
		hovers.insert(obj);
}

poWindow::poWindow(const char *title, uint root_id, poRect b)
:	title_(title)
,	handle(NULL)
,	root(NULL)
,	_bounds(b)
,   drag_target(NULL)
,   key_receiver(NULL)
,	fullscreen_(false)
,	closed_(false)
,	framecounter(0)
,	total_framecount(0)
,	last_mark(0.0)
,	framerate_(0.f)
{
	makeCurrent();
	root = createObjectForID(root_id);
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
	return _bounds.origin.x;
}

std::string poWindow::title() const {
	return title_;
}

int poWindow::y() const {
	return _bounds.origin.y;
}

int poWindow::width() const {
	return _bounds.size.x;
}

poRect poWindow::frame() const {
	return _bounds;
}

poRect poWindow::bounds() const {
	return poRect(poPoint(0,0), _bounds.size);
}

poPoint poWindow::centerPoint() const {
	return poPoint(_bounds.width()/2.f, _bounds.height()/2.f);
}

int poWindow::height() const {
	return _bounds.size.y;
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

poObject *poWindow::rootObject() const {
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
	
	if(root)
		root->_updateTree();
}

void poWindow::draw() {
	if(root)
		root->_drawTree();
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
	if(!root) {
		received.clear();
		return;
	}
	
	// if there aren't any events, bail
	if(received.empty())
		return;
	
	// store the events
	poEventCenter *center = poEventCenter::get();

	// figure out who's down there
	std::set<poObject*> hovers;
	objUnderMouse(root, mouse_pos, hovers);
	
	std::set<poObject*> did_enter;
	std::set<poObject*> did_leave;
	
	// figure out who isn't in the list anymore
	std::set_difference(mouse_hovers.begin(), mouse_hovers.end(),
						hovers.begin(), hovers.end(),
						std::inserter(did_leave, did_leave.end()));
	// figure out who's new in the list
	std::set_difference(hovers.begin(), hovers.end(),
						mouse_hovers.begin(), mouse_hovers.end(),
						std::inserter(did_enter, did_enter.end()));

	// reset the list for next frame
	mouse_hovers.clear();
	mouse_hovers.insert(hovers.begin(), hovers.end());
	
	std::set<poObject*> filter;

	while(!received.empty()) {
		poEvent event = received.front();
		received.pop_front();
		
		switch(event.type) {
			case PO_MOUSE_DOWN_EVERYWHERE_EVENT:
				// handle mouse down everywhere
				center->notify(event);

				if(!hovers.empty()) {
					std::vector<poObject*> over;
					sortedFilteredVectorFromSet(hovers, over, PO_MOUSE_DOWN_INSIDE_EVENT);
					
					// if someone is under the mouse make them the drag target
					drag_target = over.back();
					filter.insert(drag_target);
					
					// tell the one guy if he got clicked on
					event.type = PO_MOUSE_DOWN_INSIDE_EVENT;
					center->notifyFiltered(event, filter, false);
				}

				// and tell everyone else they didn't
				event.type = PO_MOUSE_DOWN_OUTSIDE_EVENT;
				center->notifyFiltered(event, filter, true);

				break;
				
			case PO_MOUSE_UP_EVERYWHERE_EVENT:
				// handle mouse up
				poEventCenter::get()->notify(event);
				
				// kill the drag target because the mouse is up
				drag_target = NULL;
				
				if(!hovers.empty()) {
					std::vector<poObject*> over;
					sortedFilteredVectorFromSet(hovers, over, PO_MOUSE_UP_INSIDE_EVENT);

					filter.insert(over.back());

					// do the bounds checked versions
					event.type = PO_MOUSE_UP_INSIDE_EVENT;
					center->notifyFiltered(event, filter, false);

					event.type = PO_MOUSE_UP_OUTSIDE_EVENT;
					center->notifyFiltered(event, filter, true);
				}
				
				break;
				
			case PO_MOUSE_MOVE_EVENT:
				// push through the mouse move
				poEventCenter::get()->notify(event);
				
				if(!hovers.empty()) {
					event.type = PO_MOUSE_OVER_EVENT;
					center->notifyFiltered(event, filter, false);
				}
				
				if(!did_enter.empty()) {
					event.type = PO_MOUSE_ENTER_EVENT;
					center->notifyFiltered(event, did_enter, false);
				}
				
				if(!did_leave.empty()) {
					event.type = PO_MOUSE_LEAVE_EVENT;
					center->notifyFiltered(event, did_leave, false);
				}
				
				break; 

			case PO_MOUSE_DRAG_EVERYWHERE_EVENT:
                poEventCenter::get()->notify(event);
				
				if(drag_target) {
					std::set<poObject*> drag_filter;
					drag_filter.insert(drag_target);
					
					event.type = PO_MOUSE_DRAG_EVENT;
					center->notifyFiltered(event, drag_filter, false);
				}
				
				break;
				
			case PO_KEY_DOWN_EVENT:
				center->notify(event);
				break;
				
			case PO_KEY_UP_EVENT:
				center->notify(event);
				break;

			case PO_WINDOW_RESIZED_EVENT:
				// everyone who cares gets resize
				// maybe this should be broadcast
				center->notify(event);
				break;
				
			case PO_TOUCH_BEGAN_EVENT:
			{
				// handle mouse down everywhere
				center->notify(event);
				
				// figure out who's down there
				std::set<poObject*> touch_hovers;
				objUnderMouse(root, event.position, touch_hovers);
				
				if(!touch_hovers.empty()) 
				{
					// find top-most clicked on object
					// tell the one guy if he got clicked on
					
					poObject* topObject = NULL;
					int topDrawOrder = 0;
					
					// look for top object with event.type = PO_TOUCH_INSIDE_EVENT;
					BOOST_FOREACH(poObject *obj, touch_hovers) 
					{
						if ( ! poEventCenter::get()->objectHasEvent(obj,PO_TOUCH_INSIDE_EVENT) )
							continue;
						
						if ( obj->drawOrder() > topDrawOrder )
						{
							topDrawOrder = obj->drawOrder();
							topObject = obj;
						}
					}
					if ( topObject != NULL )
					{
						event.type = PO_TOUCH_INSIDE_EVENT;
						center->routeBySource( topObject, event );
					}
				}
				
				printf("\n");
				break;
			}
			case PO_TOUCH_MOVED_EVENT:
			{
				// handle mouse down everywhere
				center->notify(event);
				
				// figure out who's down there
				std::set<poObject*> touch_hovers;
				objUnderMouse(root, event.position, touch_hovers);
				
				if(!touch_hovers.empty()) {
		
					poObject* topObject = NULL;
					int topDrawOrder = 0;
					
					event.type = PO_TOUCH_OVER_EVENT;
					// look for top object with event.type = PO_TOUCH_INSIDE_EVENT;
					BOOST_FOREACH(poObject *obj, touch_hovers) 
					{
						if ( ! poEventCenter::get()->objectHasEvent(obj,PO_TOUCH_OVER_EVENT) )
							continue;
						
						if ( obj->drawOrder() > topDrawOrder )
						{
							topDrawOrder = obj->drawOrder();
							topObject = obj;
						}
					}
					if ( topObject != NULL )
					{
						center->routeBySource( topObject, event );
					}
					
					// tell the one guy if he got clicked on
					/*event.type = PO_TOUCH_OVER_EVENT;
					BOOST_FOREACH(poObject *obj, touch_hovers) {
						if ( ! poEventCenter::get()->objectHasEvent(obj,PO_TOUCH_OVER_EVENT) )
							continue;  
						center->routeBySource( obj, event );
					}*/
				}
				break;
			}
				
			default:
				// just eat it
				break;
		}
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
	if(!root)
		return;
	
	poEvent event;
	event.keyCode = code;
	event.keyChar = key;
	event.modifiers = mod;
	
	event.type = PO_KEY_UP_EVENT;
	received.push_back(event);
}

void poWindow::resized(int w, int h) {
	resized(_bounds.origin.x, _bounds.origin.y, w, h);
}

void poWindow::resized(int x, int y, int w, int h) {
	_bounds.set(x,y,w,h);

	poEvent event;
	event.type = PO_WINDOW_RESIZED_EVENT;
	received.push_back(event);
}

void poWindow::touchBegin(int x, int y, int ID )
{
	poEvent event;
	event.position.set(x, y, 0.f);
	event.touchID = ID;
	
	event.type = PO_TOUCH_BEGAN_EVENT;
	received.push_back(event);
}

void poWindow::touchMove(int x, int y, int ID )
{
	poEvent event;
	event.position.set(x, y, 0.f);
	event.touchID = ID;
	
	event.type = PO_TOUCH_MOVED_EVENT;
	received.push_back(event);
}

void poWindow::touchEnd(int x, int y, int ID )
{
	poEvent event;
	event.position.set(x, y, 0.f);
	event.touchID = ID;
	
	event.type = PO_TOUCH_ENDED_EVENT;
	received.push_back(event);
}

void *poWindow::getWindowHandle() {
	return handle;
}

void poWindow::setWindowHandle(void *handle) {
	this->handle = handle;
	if(root)
		root->inWindow(handle != NULL);
}

int poWindow::nextDrawOrder() {
	return draw_order_counter++;
}


