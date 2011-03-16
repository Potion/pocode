#include "poObject.h"
#include "application/poWindow.h"

#ifdef _WIN32
#include "application/windows/Win32Impl.cpp"
#elif defined __APPLE__
#include "application/cocoa/CocoaWindowImpl.cpp"
#endif

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
	root->setWindow(this);
	impl = new WINDOW_IMPL_TYPE(this, type, title, x, y, w, h);
	impl->initialize();
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
	root->setWindow(NULL);

	root = obj;
	root->setWindow(this);
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
	event.x = x;
	event.y = getHeight() - y;
	event.modifiers = mod;
	
	event.type = PO_MOUSE_DOWN_EVENT;
	root->_broadcastEvent(&event);
	
	// reset the key receiver
	key_receiver = NULL;
	// figure out if anyone is there
	poObject *obj = root->objectUnderMouse(event.x, event.y);
	if(obj) {
		event.type = PO_MOUSE_PRESS_EVENT;
		// send the event directy to the object
		if(obj->_processEvent(&event)) {
			mouse_receiver = obj;
			// and set the key handler if it takes key events
			if(obj->hasEvents(PO_KEY_PRESS_EVENT) ||
			   obj->hasEvents(PO_KEY_RELEASE_EVENT))
			{
				key_receiver = obj;
			}
		}
	}
}

void poWindow::mouseUp(int x, int y, int mod) {
	poEvent event;
	event.x = x;
	event.y = getHeight() - y;
	event.modifiers = mod;
    
	event.type = PO_MOUSE_UP_EVENT;
	root->_broadcastEvent(&event);
	
	if(mouse_receiver != NULL) {
		event.type = PO_MOUSE_RELEASE_EVENT;
		mouse_receiver->_processEvent(&event);
		mouse_receiver = NULL;
	}
}

void poWindow::mouseMove(int x, int y, int mod) {
	poEvent event;
	event.x = x;
	event.y = getHeight() - y;
	event.modifiers = mod;
	
	event.type = PO_MOUSE_MOVE_EVENT;
	root->_broadcastEvent(&event);
	
	poObject *obj = root->objectUnderMouse(event.x, event.y);
	if(obj) {
		// we're changing hover targets
		if(obj != mouse_hover) {
			// if there was a previous hover target
			if(mouse_hover != NULL) {
				// let them know the mouse is leaving them
				event.type = PO_MOUSE_LEAVE_EVENT;
				mouse_hover->_processEvent(&event);
			}
			// and tell the new hover target
			event.type = PO_MOUSE_ENTER_EVENT;
			if(obj->_processEvent(&event))
                mouse_hover = obj;
		}
	}
	else if(mouse_hover != NULL) {
		// there's nothing under the mouse but we still have a hover target
		event.type = PO_MOUSE_LEAVE_EVENT;
		mouse_hover->_processEvent(&event);
		mouse_hover = NULL;
	}
}

void poWindow::mouseDrag(int x, int y, int mod) {
	poEvent event;
	event.x = x;
	event.y = getHeight() - y;
	event.modifiers = mod;
    
	if(mouse_receiver != NULL) {
		event.type = PO_MOUSE_DRAG_EVENT;
		mouse_receiver->_processEvent(&event);
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
	root->_broadcastEvent(&event);
	
	if(key_receiver != NULL) {
		event.type = PO_KEY_PRESS_EVENT;
		key_receiver->_processEvent(&event);
	}
}

void poWindow::keyUp(char key, int code, int mod) {
	poEvent event;
	event.keyCode = code;
	event.keyChar = key;
	event.modifiers = mod;
    
	event.type = PO_KEY_UP_EVENT;
	root->_broadcastEvent(&event);
	
	if(key_receiver != NULL) {
		event.type = PO_KEY_RELEASE_EVENT;
		key_receiver->_processEvent(&event);
	}
}

