/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "poWindow.h"
#include "poApplication.h"

#include "poOpenGLState.h"
#include "poMatrixStack.h"

static uint PO_OBJECT_UID = 0;

poObject::poObject() 
:	_parent(NULL)
,	_uid(PO_OBJECT_UID++)
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
,	_uid(PO_OBJECT_UID++)
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

poObject* poObject::getChildWithUID(uint UID) {
	BOOST_FOREACH(poObject *obj, children) {
		poObject *resp = obj->getChildWithUID(UID);
		if(resp) return resp;
	}
	
	if(UID == uid())
		return this;
	
	return NULL;
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

std::vector<poObject*> poObject::getChildren(const std::string &name) {
    std::vector<poObject*> childrenP;
	BOOST_FOREACH(poObject *obj, children)
        if(obj->name == name)
            childrenP.push_back(obj);
	return childrenP;
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
		return true;
	}
	return false;
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
    
    // if invisible, return false
	if(!visible)
		return false;
	
	// check pointInside for all children
    BOOST_FOREACH(poObject* obj, children) {
		if ( obj->pointInside( point, localize ) )
            return true;
	}
    
    // if there are children, but no pointInside, return false
    if ( numChildren() > 0 )
        return false;
    
    // if there are no children, check bounds rect
    if(localize) 
    {
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
			offset.set(-frame.width/2.f,0,0); break;
		case PO_ALIGN_TOP_RIGHT:
			offset.set(-frame.width,0,0); break;
		case PO_ALIGN_CENTER_LEFT:
			offset.set(0,-frame.height/2.f,0); break;
		case PO_ALIGN_CENTER_CENTER:
			offset.set(-frame.width/2.f,-frame.height/2.f,0); break;
		case PO_ALIGN_CENTER_RIGHT:
			offset.set(-frame.width,-frame.height/2.f,0); break;
		case PO_ALIGN_BOTTOM_LEFT:
			offset.set(0,-frame.height,0); break;
		case PO_ALIGN_BOTTOM_CENTER:
			offset.set(-frame.width/2.f,-frame.height,0); break;
		case PO_ALIGN_BOTTOM_RIGHT:
			offset.set(-frame.width,-frame.height,0); break;
	}
	
	offset = offset-frame.getPosition();
	
	return *this;
}

poRect poObject::calculateBounds() {
	poRect rect = bounds;
	BOOST_FOREACH(poObject* obj, children) {
		poRect obj_b = obj->calculateBounds();
		rect.include(objectToLocal(obj, obj_b.getBottomRight()));
		rect.include(objectToLocal(obj, obj_b.getTopLeft()));
	}
	return rect;
}

poObject*		poObject::parent() const {return _parent;}
uint			poObject::uid() const {return _uid;}

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
	BOOST_FOREACH(poEvent *e, poEventCenter::get()->eventsForObject(this,event->type)) {
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

void poObject::read(poXMLNode node) {
	_uid = (uint)node.getChild("uid").innerInt();
	name = node.getChild("name").innerString();
	position.set(node.getChild("position").innerString());
	scale.set(node.getChild("scale").innerString());
	alpha = node.getChild("alpha").innerFloat();
	rotation = node.getChild("rotation").innerFloat();
	rotationAxis.set(node.getChild("rotationAxis").innerString());
	offset.set(node.getChild("offset").innerString());
	bounds.set(node.getChild("bounds").innerString());
	visible = node.getChild("visible").innerInt();
	boundsAreFixed = node.getChild("boundsAreFixed").innerInt();
	matrixOrder = poMatrixOrder(node.getChild("matrixOrder").innerInt());
	_alignment = poAlignment(node.getChild("alignment").innerInt());
	alignment(_alignment);
}

void poObject::write(poXMLNode &node) {
	node.addAttribute("type", "poObject");

	node.addChild("uid").setInnerInt(uid());
	node.addChild("name").setInnerString(name);
	node.addChild("position").setInnerString(position.toString());
	node.addChild("scale").setInnerString(scale.toString());
	node.addChild("alpha").setInnerFloat(alpha);
	node.addChild("rotation").setInnerFloat(rotation);
	node.addChild("rotationAxis").setInnerString(rotationAxis.toString());
	node.addChild("offset").setInnerString(offset.toString());
	node.addChild("bounds").setInnerString(bounds.toString());
	node.addChild("visible").setInnerInt(visible);
	node.addChild("boundsAreFixed").setInnerInt(boundsAreFixed);
	node.addChild("matrixOrder").setInnerInt(matrixOrder);
	node.addChild("alignment").setInnerInt(_alignment);
}

void poObject::updateAllTweens() {
	position_tween.update();
	scale_tween.update();
	offset_tween.update();
	alpha_tween.update();
	rotation_tween.update();
}

void poObject::pushObjectMatrix() {
	if(_parent) {
		true_alpha = _parent->true_alpha * alpha;
	}
	else {
		true_alpha = alpha;
	}
	
	poPoint trans = round(position);
	draw_order = applicationCurrentWindow()->nextDrawOrder();
	
	poMatrixStack *stack = &poOpenGLState::get()->matrix;
	stack->pushModelview();
	
	// now move depending on the matrix order
	switch(matrixOrder) {
		case PO_MATRIX_ORDER_TRS:
			stack->translate(trans);
			stack->rotate(rotation, rotationAxis);
			stack->scale(scale);
			break;
			
		case PO_MATRIX_ORDER_RST:
			stack->rotate(rotation, rotationAxis);
			stack->scale(scale);
			stack->translate(trans);
			break;
	}

	stack->translate(round(offset));
	

	matrices.capture();
}

void poObject::popObjectMatrix() {
	poOpenGLState::get()->matrix.popModelview();
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
