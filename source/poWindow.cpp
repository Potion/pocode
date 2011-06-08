#include "poObject.h"
#include "poWindow.h"

#include "Helpers.h"
#include "poApplication.h"

poObject *objUnderMouse(poObject *obj, poPoint &mouse) {
	if(!obj->visible())
		return NULL;
	
	for(int i=obj->numChildren()-1; i>=0; i--) {
		poObject *response = objUnderMouse(obj->getChild(i),mouse);
		if(response != NULL)
			return response;
	}
	
	if(obj->pointInside(mouse, true))
		return obj;
	
	return NULL;
}

poWindow::poWindow(const char *title, void *handle, uint root_id, poRect bounds)
:	title_(title)
,	handle(handle)
,	root(NULL)
,	bounds(bounds)
,   mouse_receiver(NULL)
,   mouse_hover(NULL)
,   key_receiver(NULL)
,	fullscreen_(false)
,	closed_(false)
,	framecounter(0)
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

void poWindow::moveTo(poRect rect) {
	bounds = rect;
	applicationMoveWindow(this, rect);
}

void poWindow::fullscreen(bool b) {
	fullscreen_ = b;
	applicationMakeWindowFullscreen(this, b);
}

int poWindow::x() const {
	return bounds.origin.x;
}

std::string poWindow::title() const {
	return title_;
}

int poWindow::y() const {
	return bounds.origin.y;
}

int poWindow::width() const {
	return bounds.size.x;
}

int poWindow::height() const {
	return bounds.size.y;
}

float poWindow::framerate() const {
	return framerate_;
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

void poWindow::makeCurrent() {
	applicationMakeWindowCurrent(this);
}

void poWindow::update() {
	double now = getTime();
	
	framecounter++;
	if(now - last_mark >= 1.0) {
		last_mark = now;
		framerate_ = framecounter;
		framecounter = 0;
	}
	
	last_elapsed = now - last_frame;
	last_frame = now;
	
	if(root)
		root->_updateTree();
}

void poWindow::draw() {
	if(root)
		root->_drawTree();
}

void poWindow::mouseDown(int x, int y, int mod) {
	if(!root)
		return;
	
	poEvent event;
	event.position.set(x, height()-y, 0.f);
	event.modifiers = mod;
	
	event.type = PO_MOUSE_DOWN_EVENT;
	poEventCenter::get()->notify(event);

	event.type = PO_MOUSE_PRESS_EVENT;
	mouse_receiver = poEventCenter::get()->notify(event);
	
	if(mouse_receiver && 
	   (poEventCenter::get()->objectHasEvent(mouse_receiver, PO_KEY_PRESS_EVENT) ||
		poEventCenter::get()->objectHasEvent(mouse_receiver, PO_KEY_RELEASE_EVENT)))
	{
		key_receiver = mouse_receiver;
	}
	else {
		key_receiver = NULL;
	}
}

void poWindow::mouseUp(int x, int y, int mod) {
	if(!root)
		return;
	
	poEvent event;
	event.position.set(x, height()-y, 0.f);
	event.modifiers = mod;

	event.type = PO_MOUSE_UP_EVENT;
	poEventCenter::get()->notify(event);
	
	// something was previously clicked on
	if(mouse_receiver) {
		event.type = PO_MOUSE_RELEASE_EVENT;
		poEventCenter::get()->routeBySource(mouse_receiver, event);
		// we're done directly routing mouse events
		mouse_receiver = NULL;
	}
}

void poWindow::mouseMove(int x, int y, int mod) {
	if(!root)
		return;
	
	poEvent event;
	event.position.set(x, height()-y, 0.f);
	event.modifiers = mod;
	
	event.type = PO_MOUSE_MOVE_EVENT;
	poEventCenter::get()->notify(event);

	// figure out who's down there
	poPoint mouse = event.position;
	poObject *obj = objUnderMouse(root, mouse);
	
	// tell the previous hover they're off the hook
	if(mouse_hover && mouse_hover != obj) {
		event.type = PO_MOUSE_LEAVE_EVENT;
		poEventCenter::get()->routeBySource(mouse_hover, event);
		mouse_hover = NULL;
	}

	if(obj && obj != mouse_hover) {
		// and tell the new hover they're on
		event.type = PO_MOUSE_ENTER_EVENT;
		if(poEventCenter::get()->routeBySource(obj, event))
			mouse_hover = obj;
	}
}

void poWindow::mouseDrag(int x, int y, int mod) {
	if(!root)
		return;
	
	poEvent event;
	event.position.set(x, height()-y, 0.f);
	event.modifiers = mod;
	
	/*	TODO
		can't just run the move routine as you'd start dragging whatever you dragged over
		but without notifying for move event all drag targets have to have an area
		best thing is probably to figure out some auto-resizing thing, but that road leads to trouble
		for the moment treating drags as moves unless there's a drag target seems ok
	 */
	
	if(mouse_receiver) {
		event.type = PO_MOUSE_DRAG_EVENT;
		poEventCenter::get()->routeBySource(mouse_receiver, event);
	}
	else {
		event.type = PO_MOUSE_MOVE_EVENT;
		poEventCenter::get()->notify(event);
	}
}

void poWindow::mouseWheel(int x, int y, int mod, int num_steps) {
	if(!root)
		return;
	
}

void poWindow::keyDown(char key, int code, int mod) {
	if(!root)
		return;
	
	poEvent event;
	event.keyCode = key;
	event.keyChar = code;
	event.modifiers = mod;
	
	event.type = PO_KEY_DOWN_EVENT;
	poEventCenter::get()->notify(event);
	
	if(key_receiver) {
		event.type = PO_KEY_PRESS_EVENT;
		poEventCenter::get()->routeBySource(key_receiver, event);
	}
}

void poWindow::keyUp(char key, int code, int mod) {
	if(!root)
		return;
	
	poEvent event;
	event.keyCode = code;
	event.keyChar = key;
	event.modifiers = mod;
	
	event.type = PO_KEY_UP_EVENT;
	poEventCenter::get()->notify(event);

	if(key_receiver) {
		event.type = PO_KEY_RELEASE_EVENT;
		poEventCenter::get()->routeBySource(key_receiver, event);
	}
}

