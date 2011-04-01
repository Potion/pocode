/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

std::stack<float>	poObject::alpha_stack;
float				poObject::master_alpha = 1.f;

poObject::poObject() 
:	parent(NULL)
,	name("")
,	alpha(1.f)
,	scale(1.f, 1.f, 1.f)
,	position(0.f, 0.f, 0.f)
,	rotation(0.f)
,	rotation_axis(0.f, 0.f, 1.f)
,	offset(0.f, 0.f, 0.f)
,	bounds(0.f, 0.f, 0.f, 0.f)
,	align(PO_ALIGN_TOP_LEFT)
,	enabled(true)
,	events(PO_LAST_EVENT)
,	matrix_order(PO_MATRIX_ORDER_TRS)
,	position_tween(&position)
,	scale_tween(&scale)
,	alpha_tween(&alpha)
,	rotation_tween(&rotation)
{}

poObject::poObject(const std::string &name)
:	parent(NULL)
,	name(name)
,	alpha(1.f)
,	scale(1.f, 1.f, 1.f)
,	position(0.f, 0.f, 0.f)
,	rotation(0.f)
,	rotation_axis(0.f, 0.f, 1.f)
,	offset(0.f, 0.f, 0.f)
,	bounds(0.f, 0.f, 0.f, 0.f)
,	align(PO_ALIGN_TOP_LEFT)
,	enabled(true)
,	events(PO_LAST_EVENT)
,	matrix_order(PO_MATRIX_ORDER_TRS)
,	position_tween(&position)
,	scale_tween(&scale)
,	alpha_tween(&alpha)
,	rotation_tween(&rotation)
{}

poObject::~poObject() {
	removeAllChildren(true);
}

void poObject::setup() {}
void poObject::draw() {}
void poObject::update() {}
bool poObject::eventHandler(poEvent* event) {return false;}
void poObject::messageHandler(const std::string &msg, const poDictionary& dict) {}
void poObject::preDraw() {}
void poObject::postDraw() {}

void poObject::addChild(poObject* obj) {
	children.push_back(obj);
}

void poObject::addChild(poObject *obj, int idx) {
	children.insert(children.begin()+idx, obj);
}

bool poObject::removeChild(poObject* obj) {
	poObjectVec::iterator iter = std::find(children.begin(), children.end(), obj);
	bool found = iter != children.end();
	children.erase(iter);
	return found;
}

bool poObject::removeChild(int idx, bool and_delete) {
	if(idx < 0 || idx >= children.size())
		return false;
	if(and_delete)
		delete children[idx];
	children.erase(children.begin()+idx);
	return true;
}

void poObject::removeAllChildren(bool and_delete) {
	if(and_delete) {
		BOOST_FOREACH(poObject* obj, children) {
			delete obj;
		}
	}
	children.clear();
}

int poObject::numChildren() const {
	return (int)children.size();
}

poObject* poObject::getChild(int idx) {
	if(idx < 0 || idx >= children.size())
		return NULL;
	return *(children.begin()+idx);
}

poObject* poObject::getChild(const std::string &name) {
	poObjectVec::iterator iter = std::find_if(children.begin(), children.end(), boost::bind(&poObject::name, _1) == name);
	if(iter != children.end())
		return *iter;
	return NULL;
}

int poObject::getChildIndex(poObject* obj) {
	poObjectVec::iterator iter = std::find(children.begin(), children.end(), obj);
	if(iter != children.end())
		return (int)std::distance(children.begin(), iter);
	return INVALID_INDEX;
}

void poObject::moveChildToFront(poObject* child) {
	removeChild(child);
	addChild(child, 0);
}

void poObject::moveChildToBack(poObject* child) {
	removeChild(child);
	addChild(child);
}

void poObject::moveChildForward(poObject* child) {
	int idx = getChildIndex(child);
	removeChild(child);
	addChild(child, std::max(idx-1, 0));
}

void poObject::moveChildBackward(poObject* child) {
	int idx = getChildIndex(child);
	removeChild(child);
	addChild(child, idx+1);
}

bool poObject::pointInside(poPoint point) {
	return pointInside(point.x, point.y, point.z);
}

bool poObject::pointInside(float x, float y, float z) {
	if(!enabled)
		return false;
	
	return bounds.contains(poPoint(x,y));
}

poPoint poObject::globalToLocal(poPoint point) const {
	return matrices.globalToLocal(point);
}

poPoint poObject::localToGlobal(poPoint point) const {
	return matrices.localToGlobal(point);
}

poPoint poObject::objectToLocal(poObject* obj, poPoint point) const {
	point = obj->localToGlobal(point);
	return globalToLocal(point);
}

void poObject::setAlignment(poAlignment align) {
	this->align = align;
}

poRect poObject::calculateBounds(bool include_children) {
	bounds = poRect(0,0,0,0);
	
	if(include_children) {
		BOOST_FOREACH(poObject* obj, children) {
			obj->calculateBounds(include_children);
			bounds.include(objectToLocal(obj, obj->bounds.bottomLeft()));
			bounds.include(objectToLocal(obj, obj->bounds.bottomRight()));
			bounds.include(objectToLocal(obj, obj->bounds.topLeft()));
			bounds.include(objectToLocal(obj, obj->bounds.topRight()));
		}
	}
	
	return bounds;
}

poObject* poObject::objectUnderMouse(float x, float y) {
	if(!enabled)
		return NULL;
	
	BOOST_REVERSE_FOREACH(poObject* obj, children) {
		poObject *response = obj->objectUnderMouse(x,y);
		if(response != NULL)
			return response;
	}
	
	poPoint localized = globalToLocal(poPoint(x,y));
	if(pointInside(localized))
		return this;
	
	return NULL;
}

void poObject::_drawTree() {
	if(!enabled)
		return;
	
	pushObjectMatrix();

	preDraw();
	
	draw();
	
	BOOST_FOREACH(poObject* obj, children) {
		obj->_drawTree();
	}

	postDraw();
	
	popObjectMatrix();
}

void poObject::_updateTree() {
	if(!enabled)
		return;
	
	updateAllTweens();
	update();
	
	BOOST_FOREACH(poObject* obj, children) {
		obj->_updateTree();
	}
}

void poObject::_broadcastEvent(poEvent* event) {
	if(!enabled)
		return;
	// make sure its even a valid event
	if(event->type < 0 || event->type >= PO_LAST_EVENT)
		return;

	// send it to all our children
	BOOST_REVERSE_FOREACH(poObject* obj, children) {
		obj->_broadcastEvent(event);
	}

	// localize the point only once
	poPoint local_point = globalToLocal(poPoint(event->x, event->y));
	
	// handle every event like this we have
	BOOST_FOREACH(poEvent *e, events[event->type]) {
		localizeEvent(e, event, local_point);
		eventHandler(e);
	}
}

void poObject::updateAllTweens() {
	position_tween.update();
	scale_tween.update();
	alpha_tween.update();
	rotation_tween.update();
}

void poObject::pushObjectMatrix() {
	glPushMatrix();
	
	// modify the alpha stack
	alpha_stack.push(master_alpha);
	master_alpha *= alpha;
	
	// now move depending on the matrix order
	switch(matrix_order) {
		case PO_MATRIX_ORDER_TRS:
			glTranslatef(position.x, position.y, position.z);
			glRotatef(rotation, rotation_axis.x, rotation_axis.y, rotation_axis.z);
			glScalef(scale.x, scale.y, scale.z);
			break;
			
		case PO_MATRIX_ORDER_RST:
			glRotatef(rotation, rotation_axis.x, rotation_axis.y, rotation_axis.z);
			glScalef(scale.x, scale.y, scale.z);
			glTranslatef(position.x, position.y, position.z);
			break;
	}
	
	// translate to the offset
	glTranslatef(offset.x, offset.y, offset.z);

	matrices.capture();
}

void poObject::popObjectMatrix() {
	// rewind the alpha stack
	master_alpha = alpha_stack.top();
	alpha_stack.pop();

	// and reset gl
	glPopMatrix();
}

void poObject::localizeEvent(poEvent *local_event, poEvent *global_event, poPoint localized_pt) {
	local_event->x = global_event->x;
	local_event->y = global_event->y;
	local_event->localX = localized_pt.x;
	local_event->localY = localized_pt.y;
	local_event->keyChar = global_event->keyChar;
	local_event->keyCode = global_event->keyCode;
	local_event->modifiers = global_event->modifiers;
	local_event->uid = global_event->uid;
	local_event->timestamp = global_event->timestamp;
	local_event->prevX = global_event->prevX;
	local_event->prevY = global_event->prevY;
	// don't touch the message or the dictionary
}
