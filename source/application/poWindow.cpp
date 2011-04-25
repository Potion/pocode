#include "poObject.h"
#include "application/poWindow.h"
#include "application/poApplication.h"

#ifdef _WIN32
#include "application/windows/Win32Impl.cpp"
#elif defined __APPLE__
#include "application/cocoa/CocoaWindowImpl.cpp"
#endif


poObject *objUnderMouse(poObject *obj, poPoint &mouse) {
	if(!obj->enabled)
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

poWindow::poWindow(poObject *root,
				   poWindowType type,
				   const std::string &title,
				   int x, int y, int w, int h)
:	impl(NULL)
,	root(root)
,   mouse_receiver(NULL)
,   mouse_hover(NULL)
,   key_receiver(NULL)
{
	impl = new WINDOW_IMPL_TYPE(this, type, title, x, y, w, h);
	impl->initialize();
	
	poApplication::get()->currentWindow = this;
	root->setup();
}

poWindow::~poWindow() {
	cleanup();
	delete impl;
	delete root;
}

void poWindow::setSize(int w, int h) {
	impl->width = w;
	impl->height = h;
}

void poWindow::setPosition(int x, int y) {
	impl->xpos = x;
	impl->ypos = y;
}

void poWindow::moveTo(int x, int y, int w, int h) {
	impl->moveTo(x, y, w, h);
}

void poWindow::setFullscreen(bool b) {
	impl->setFullscreen(b);
}

void poWindow::toggleFullscreen() {
	setFullscreen(!impl->is_fullscreen);
}

void poWindow::setDragging(bool b) {
	impl->is_dragging = b;
}

void poWindow::setRootObject(poObject *obj) {
	root = obj;
}

std::string poWindow::getTitle() {
	return impl->title;
}

int poWindow::getWidth() {
	return impl->width;
}

int poWindow::getHeight() {
	return impl->height;
}

int poWindow::getX() {
	return impl->xpos;
}

int poWindow::getY() {
	return impl->ypos;
}

bool poWindow::isFullscreen() {
	return impl->is_fullscreen;
}

bool poWindow::isDragging() {
	return impl->is_dragging;
}

int poWindow::getId() {
	return impl->id;
}

poObject *poWindow::getRootObject() {
	return root;
}

void poWindow::setup() {
}

void poWindow::cleanup() {
}

void poWindow::update() {
	root->_updateTree();
}

void poWindow::draw() {
	root->_drawTree();
}

void poWindow::mouseDown(int x, int y, int mod) {
	poEvent event;
	event.position.set(x, getHeight()-y, 0.f);
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
	poEvent event;
	event.position.set(x, getHeight()-y, 0.f);
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
	poEvent event;
	event.position.set(x, getHeight()-y, 0.f);
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
	poEvent event;
	event.position.set(x, getHeight()-y, 0.f);
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
}

void poWindow::keyDown(char key, int code, int mod) {
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

