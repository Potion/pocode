#include "poObject.h"
#include "application/poWindow.h"
#include "application/poApplication.h"

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
	event.x = x;
	event.y = getHeight() - y;
	event.modifiers = mod;
}

void poWindow::mouseUp(int x, int y, int mod) {
	poEvent event;
	event.x = x;
	event.y = getHeight() - y;
	event.modifiers = mod;
}

void poWindow::mouseMove(int x, int y, int mod) {
	poEvent event;
	event.x = x;
	event.y = getHeight() - y;
	event.modifiers = mod;
}

void poWindow::mouseDrag(int x, int y, int mod) {
	poEvent event;
	event.x = x;
	event.y = getHeight() - y;
	event.modifiers = mod;
}

void poWindow::mouseWheel(int x, int y, int mod, int num_steps) {
}

void poWindow::keyDown(char key, int code, int mod) {
	poEvent event;
	event.keyCode = key;
	event.keyChar = code;
	event.modifiers = mod;
}

void poWindow::keyUp(char key, int code, int mod) {
	poEvent event;
	event.keyCode = code;
	event.keyChar = key;
	event.modifiers = mod;
}

