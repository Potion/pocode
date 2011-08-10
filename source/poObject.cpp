/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "poWindow.h"
#include "poApplication.h"

poObject::poObject() 
:	_parent(NULL)
,	name("")
,	alpha(1.f)
,	scale(1.f, 1.f, 1.f)
,	position(0.f, 0.f, 0.f)
,	rotation(0.f)
,	rotationAxis(0.f, 0.f, 1.f)
,	offset(0.f, 0.f, 0.f)
,	bounds(0.f, 0.f, 0.f, 0.f)
,	_alignment(PO_ALIGN_TOP_LEFT)
,	visible(true)
,	events(PO_LAST_EVENT)
,	matrixOrder(PO_MATRIX_ORDER_TRS)
,	draw_order(0)
,	position_tween(&position)
,	scale_tween(&scale)
,	offset_tween(&offset)
,	alpha_tween(&alpha)
,	rotation_tween(&rotation)
,	true_alpha(1.f)
,	in_window(false)
{}

poObject::poObject(const std::string &name)
:	_parent(NULL)
,	name(name)
,	alpha(1.f)
,	scale(1.f, 1.f, 1.f)
,	position(0.f, 0.f, 0.f)
,	rotation(0.f)
,	rotationAxis(0.f, 0.f, 1.f)
,	offset(0.f, 0.f, 0.f)
,	bounds(0.f, 0.f, 0.f, 0.f)
,	_alignment(PO_ALIGN_TOP_LEFT)
,	visible(true)
,	events(PO_LAST_EVENT)
,	matrixOrder(PO_MATRIX_ORDER_TRS)
,	draw_order(0)
,	position_tween(&position)
,	scale_tween(&scale)
,	offset_tween(&offset)
,	alpha_tween(&alpha)
,	rotation_tween(&rotation)
,	true_alpha(1.f)
,	in_window(false)
{}

poObject::~poObject() {
	removeAllChildren(true);
}

void poObject::draw() {}
void poObject::update() {}
void poObject::eventHandler(poEvent *event) {}
void poObject::messageHandler(const std::string &msg, const poDictionary& dict) {
	if(_parent) {
		_parent->messageHandler(msg,dict);
	}
}

int poObject::addEvent(int eventType, poObject *sink, std::string message, const poDictionary& dict) {
	if(!sink)
		sink = this;
	return poEventCenter::get()->registerForEvent(eventType, this, sink, message, dict);
}

void poObject::removeEvent(int event_id) {
	poEventCenter::get()->removeEvent(event_id);
}

bool poObject::removeChild(poObject* obj) {
	poObjectVec::iterator iter = std::find(children.begin(), children.end(), obj);
	bool found = iter != children.end();
	
	if(found) {
		(*iter)->_parent = NULL;
		(*iter)->inWindow(false);
		children.erase(iter);
	}
	
	return found;
}

bool poObject::removeChild(int idx, bool and_delete) {
	if(idx < 0 || idx >= children.size())
		return false;

	children[idx]->_parent = NULL;
	children[idx]->inWindow(false);

	if(and_delete)
		delete children[idx];

	children.erase(children.begin()+idx);
	
	return true;
}

void poObject::removeAllChildren(bool and_delete) {
	BOOST_FOREACH(poObject* obj, children) {
		obj->_parent = NULL;
		obj->inWindow(false);
		if(and_delete)
			delete obj;
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

poObject* poObject::getLastChild() {
	if(children.empty())
		return NULL;
	return children.back();
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

bool poObject::removeModifier(int idx, bool and_delete) {
	if(idx >= 0 && idx < modifiers.size()) {
		if(and_delete)
			delete modifiers[idx];
		modifiers.erase(modifiers.begin() + idx);
	}
}

bool poObject::removeModifier(poObjectModifier* mod, bool and_delete) {
	poObjectModifierVec::iterator found = std::find(modifiers.begin(), modifiers.end(), mod);
	if(found != modifiers.end()) {
		if(and_delete)
			delete *found;
		modifiers.erase(found);
		return true;
	}
	return false;
}

void poObject::removeAllModifiers(bool and_delete)
{
	if(and_delete) {
		BOOST_FOREACH(poObjectModifier* mod, modifiers) {
			delete mod;
		}
	}
    modifiers.clear();
}

int poObject::numModifiers() const {
	return modifiers.size();
}

bool poObject::pointInside(poPoint point, bool localize) {
	if(!visible)
		return false;
	
	if(localize) {
		point.y = getWindowHeight() - point.y;
		point = globalToLocal(point);
	}
	
	return bounds.contains(point);
}

bool poObject::pointInside(float x, float y, float z, bool localize) {
	return pointInside(poPoint(x,y,z),localize);
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

poAlignment poObject::alignment() const {return _alignment;}
poObject& poObject::alignment(poAlignment align) {
	_alignment = align; 
	
	// first calculate bounds
	poRect frame = calculateBounds();
	
	// then set offset based upon bounds and alignment
	switch(align) {
		case PO_ALIGN_TOP_LEFT:
			offset.set(0,0,0); break;
		case PO_ALIGN_TOP_CENTER:
			offset.set(-frame.width()/2.f,0,0); break;
		case PO_ALIGN_TOP_RIGHT:
			offset.set(-frame.width(),0,0); break;
		case PO_ALIGN_CENTER_LEFT:
			offset.set(0,-frame.height()/2.f,0); break;
		case PO_ALIGN_CENTER_CENTER:
			offset.set(-frame.width()/2.f,-frame.height()/2.f,0); break;
		case PO_ALIGN_CENTER_RIGHT:
			offset.set(-frame.width(),-frame.height()/2.f,0); break;
		case PO_ALIGN_BOTTOM_LEFT:
			offset.set(0,-frame.height(),0); break;
		case PO_ALIGN_BOTTOM_CENTER:
			offset.set(-frame.width()/2.f,-frame.height(),0); break;
		case PO_ALIGN_BOTTOM_RIGHT:
			offset.set(-frame.width(),-frame.height(),0); break;
	}
	
	offset = offset-frame.origin;
	
	return *this;
}

poRect poObject::calculateBounds() {
	poRect rect = bounds;
	BOOST_FOREACH(poObject* obj, children) {
		poRect obj_b = obj->calculateBounds();
		rect.include(objectToLocal(obj, obj_b.bottomRight()));
		rect.include(objectToLocal(obj, obj_b.topLeft()));
	}
	return rect;
}

poObject*		poObject::parent() const {return _parent;}

bool			poObject::isInWindow() const {return in_window;}
void			poObject::inWindow(bool b) {
	in_window = b;
	BOOST_FOREACH(poObject *obj, children) {
		obj->inWindow(b);
	}
}

float			poObject::appliedAlpha() const{return true_alpha;}
poMatrixSet     poObject::matrixSet() const {return matrices;}
int				poObject::drawOrder() const {return draw_order;}

void poObject::_drawTree() {
	if(!visible)
		return;
	
	pushObjectMatrix();

    BOOST_FOREACH(poObjectModifier* mod, modifiers) {
        mod->setUp( this );
    }
    
	draw();
	
	BOOST_FOREACH(poObject* obj, children) {
		obj->_drawTree();
	}
    
    BOOST_FOREACH(poObjectModifier* mod, modifiers) {
        mod->setDown( this );
    }
	
	popObjectMatrix();
}

void poObject::_updateTree() {
	if(!visible)
		return;
	
	draw_order = applicationCurrentWindow()->nextDrawOrder();
	
	updateAllTweens();
	update();
	
	BOOST_FOREACH(poObject* obj, children) {
		obj->_updateTree();
	}
}

void poObject::_broadcastEvent(poEvent* event) {
	if(!visible)
		return;
	// make sure its even a valid event
	if(event->type < 0 || event->type >= PO_LAST_EVENT)
		return;

	// send it to all our children
	BOOST_REVERSE_FOREACH(poObject* obj, children) {
		obj->_broadcastEvent(event);
	}

	// localize the point only once
	poPoint local_point = globalToLocal(event->position);
	
	// handle every event like this we have
	BOOST_FOREACH(poEvent *e, events[event->type]) {
		localizeEvent(e, event, local_point);
		eventHandler(e);
	}
}

void poObject::stopAllTweens(bool recurse) {
	position_tween.stop();
	scale_tween.stop();
	offset_tween.stop();
	alpha_tween.stop();
	rotation_tween.stop();
	
	if(recurse) {
		BOOST_FOREACH(poObject* obj, children)
			obj->stopAllTweens(true);
	}
}

void poObject::updateAllTweens() {
	position_tween.update();
	scale_tween.update();
	offset_tween.update();
	alpha_tween.update();
	rotation_tween.update();
}

void poObject::pushObjectMatrix() {
	glPushMatrix();

	if(_parent) {
		true_alpha = _parent->true_alpha * alpha;
	}
	else {
		true_alpha = alpha;
	}
	
	poPoint trans = round(position);
	draw_order = applicationCurrentWindow()->nextDrawOrder();
	
	// now move depending on the matrix order
	switch(matrixOrder) {
		case PO_MATRIX_ORDER_TRS:
			glTranslatef(trans.x, trans.y, trans.z);
			glRotatef(rotation, rotationAxis.x, rotationAxis.y, rotationAxis.z);
			glScalef(scale.x, scale.y, scale.z);
			break;
			
		case PO_MATRIX_ORDER_RST:
			glRotatef(rotation, rotationAxis.x, rotationAxis.y, rotationAxis.z);
			glScalef(scale.x, scale.y, scale.z);
			glTranslatef(trans.x, trans.y, trans.z);
			break;
	}

	poPoint off = round(offset);
	glTranslatef(off.x, off.y, off.z);
	matrices.capture();
}

void poObject::popObjectMatrix() {

	// and reset gl
	glPopMatrix();
}

void poObject::localizeEvent(poEvent *local_event, poEvent *global_event, poPoint localized_pt) {
	local_event->position = global_event->position;
	local_event->local_position = localized_pt;
	local_event->keyChar = global_event->keyChar;
	local_event->keyCode = global_event->keyCode;
	local_event->modifiers = global_event->modifiers;
	local_event->uid = global_event->uid;
	local_event->timestamp = global_event->timestamp;
	local_event->previous_position = global_event->previous_position;
	local_event->touchID = global_event->touchID;
	// don't touch the message or the dictionary
}
