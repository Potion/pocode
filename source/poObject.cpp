/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "Helpers.h"
#include "poObject.h"
#include "poWindow.h"
#include "poApplication.h"

#include "poOpenGLState.h"
#include "poMatrixStack.h"

#include "SimpleDrawing.h"

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
,	width(0.0f)
,   height(0.0f)
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
,   bFixedWidth(false)
,   bFixedHeight(false)
,	drawBounds(false)
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
,	width(0.0f)
,   height(0.0f)
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
,   bFixedWidth(false)
,   bFixedHeight(false)
,	drawBounds(false)
{}

poObject::~poObject() {
    poEventCenter::get()->removeAllEvents(this);
	removeAllChildren(true);
}

poObject *poObject::copy() {
	poObject *obj = new poObject();
	clone(obj);
	return obj;
}

void poObject::draw() {}

void poObject::update() {}
void poObject::eventHandler(poEvent *event) {}
void poObject::messageHandler(const std::string &msg, const poDictionary& dict) {
	if(_parent) {
		_parent->messageHandler(msg,dict);
	}
}

float poObject::getWidth() {
    if(!bFixedWidth) {
        poRect b = getBounds();
        width = b.width; 
    }
    
    return width;
}


void poObject::setWidth(float width) {
    this->width = width;
    bFixedWidth = true;
}


float poObject::getHeight() {
    if(!bFixedHeight) {
        poRect b = getBounds();
        height = b.height; 
    }
    
    return height;
}

void poObject::setHeight(float height) {
    this->height = height;
    bFixedHeight = true;
}


void poObject::setSize(float width, float height) {
    setWidth(width);
    setHeight(height);
}

void poObject::setSize(poPoint size) {
    setSize(size.x, size.y);
}

poRect poObject::getFrame() {
    poRect rect = getBounds();
    
    poRect frame;
    frame.setPosition(parent()->objectToLocal(this, rect.getBottomRight()));
    frame.include(parent()->objectToLocal(this, rect.getTopLeft()));
    
	return frame;
}

poRect poObject::getBounds() {
    poRect rect(0, 0, width, height);
	BOOST_FOREACH(poObject* obj, children) {
        if (obj->visible) {
            poRect obj_b = obj->getBounds();
            rect.include(objectToLocal(obj, obj_b.getBottomRight()));
            rect.include(objectToLocal(obj, obj_b.getTopLeft()));
        }
	}
	rect.setPosition(rect.getPosition() + offset);
	return rect;
}
       
void poObject::addEvent(int eventType, poObject *sink, std::string message, const poDictionary& dict) {
	if(!sink)
		sink = this;
	poEventCenter::get()->addEvent(eventType, this, sink, message, dict);
}

void poObject::removeAllEvents() {
	poEventCenter::get()->removeAllEvents(this);
}

void poObject::removeAllEventsOfType(int eventType) {
	poEventCenter::get()->removeAllEventsOfType(this, eventType);
}

bool poObject::removeChild(poObject* obj) {
	poObjectVec::iterator iter = std::find(children.begin(), children.end(), obj);
	bool found = iter != children.end();
	
	if(found) {
		(*iter)->_parent = NULL;
		children.erase(iter);
	}
	
	return found;
}

bool poObject::removeChild(int idx, bool and_delete) {
	if(idx < 0 || idx >= children.size())
		return false;

	children[idx]->_parent = NULL;

	if(and_delete)
		delete children[idx];

	children.erase(children.begin()+idx);
	
	return true;
}

void poObject::removeAllChildren(bool and_delete) {
	BOOST_FOREACH(poObject* obj, children) {
		obj->_parent = NULL;
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
	addChild(child);
}

void poObject::moveChildToBack(poObject* child) {
	removeChild(child);
	addChild(child, 0);
}

void poObject::moveChildForward(poObject* child) {
	int idx = getChildIndex(child);
	removeChild(child);
	addChild(child, std::max(idx, numChildren()));
}

void poObject::moveChildBackward(poObject* child) {
	int idx = getChildIndex(child);
	removeChild(child);
	addChild(child, std::min(idx-1, 0));
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
    return getBounds().contains(point);
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
	poRect bounds = getBounds();
	
	// then set offset based upon bounds and alignment
	switch(align) {
		case PO_ALIGN_TOP_LEFT:
			offset.set(0,0,0); break;
		case PO_ALIGN_TOP_CENTER:
			offset.set(-bounds.width/2.f,0,0); break;
		case PO_ALIGN_TOP_RIGHT:
			offset.set(-bounds.width,0,0); break;
		case PO_ALIGN_CENTER_LEFT:
			offset.set(0,-bounds.height/2.f,0); break;
		case PO_ALIGN_CENTER_CENTER:
			offset.set(-bounds.width/2.f,-bounds.height/2.f,0); break;
		case PO_ALIGN_CENTER_RIGHT:
			offset.set(-bounds.width,-bounds.height/2.f,0); break;
		case PO_ALIGN_BOTTOM_LEFT:
			offset.set(0,-bounds.height,0); break;
		case PO_ALIGN_BOTTOM_CENTER:
			offset.set(-bounds.width/2.f,-bounds.height,0); break;
		case PO_ALIGN_BOTTOM_RIGHT:
			offset.set(-bounds.width,-bounds.height,0); break;
	}
	
	offset = offset-bounds.getPosition();
	
	return *this;
}

poObject*		poObject::parent() const {return _parent;}
uint			poObject::uid() const {return _uid;}

float			poObject::appliedAlpha() const{return true_alpha;}
poMatrixSet     poObject::matrixSet() const {return matrices;}
int				poObject::drawOrder() const {return draw_order;}


void poObject::_drawTree() {
	if(!visible)
		return;
	
	// reset the drawing order for this object
	draw_order = applicationCurrentWindow()->nextDrawOrder();
	
	if(_parent)	true_alpha = _parent->true_alpha * alpha;
	else		true_alpha = alpha;

	poMatrixStack *stack = &poOpenGLState::get()->matrix;
	stack->pushModelview();
	
	switch(matrixOrder) {
		case PO_MATRIX_ORDER_TRS:
			stack->translate(position);
			stack->rotate(rotation, rotationAxis);
			stack->scale(scale);
			break;
			
		case PO_MATRIX_ORDER_RST:
			stack->rotate(rotation, rotationAxis);
			stack->scale(scale);
			stack->translate(position);
			break;
	}
	
	// center our object around its offset
	stack->translate(offset);
	
	// grab the matrices we need for everything
	matrices.dirty = true;
	matrices.capture();

	// set up the modifiers ... cameras, etc
    BOOST_FOREACH(poObjectModifier* mod, modifiers) {
        mod->setUp( this );
    }
	
	draw();

	// go back to centered around the origin
	stack->translate(-offset);
	
	if(drawBounds) 
		_drawBounds();
	
	// draw the children
	BOOST_FOREACH(poObject* obj, children) {
		obj->_drawTree();
	}
	
	// then recenter around offset
	// some modifiers might need the objects complete transform
	stack->translate(offset);

	// let modifiers clean up
    BOOST_FOREACH(poObjectModifier* mod, modifiers) {
        mod->setDown( this );
    }
	
	// and restore parent's matrix
	stack->popModelview();
}

void poObject::_updateTree() {
	if(!visible)
		return;
	
	updateAllTweens();
	update();
	
	BOOST_FOREACH(poObject* obj, children) {
		obj->_updateTree();
	}
}

void poObject::_drawBounds() {    
    po::setColor(poColor::red);
    po::drawStroke(getBounds());
    po::drawRect(poRect(-poPoint(2.5,2.5), poPoint(5,5)));
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
	_uid = (uint)node.getChild("uid").getInnerInt();
	name = node.getChild("name").getInnerString();
	position.set(node.getChild("position").getInnerString());
	width   = node.getChild("width").getInnerFloat();
	height  = node.getChild("height").getInnerFloat();
	scale.set(node.getChild("scale").getInnerString());
	alpha = node.getChild("alpha").getInnerFloat();
	rotation = node.getChild("rotation").getInnerFloat();
	rotationAxis.set(node.getChild("rotationAxis").getInnerString());
	offset.set(node.getChild("offset").getInnerString());
	visible = node.getChild("visible").getInnerInt();
	bFixedWidth = node.getChild("bFixedWidth").getInnerInt();
	bFixedHeight = node.getChild("bFixedHeight").getInnerInt();
	matrixOrder = poMatrixOrder(node.getChild("matrixOrder").getInnerInt());
	_alignment = poAlignment(node.getChild("alignment").getInnerInt());
	alignment(_alignment);
}

void poObject::write(poXMLNode &node) {
	node.addAttribute("type", "poObject");

	node.addChild("uid").setInnerInt(uid());
	node.addChild("name").setInnerString(name);
	node.addChild("position").setInnerString(position.toString());
	node.addChild("width").setInnerFloat(width);
    node.addChild("height").setInnerFloat(height);
	node.addChild("scale").setInnerString(scale.toString());
	node.addChild("alpha").setInnerFloat(alpha);
	node.addChild("rotation").setInnerFloat(rotation);
	node.addChild("rotationAxis").setInnerString(rotationAxis.toString());
	node.addChild("offset").setInnerString(offset.toString());
	node.addChild("visible").setInnerInt(visible);
	node.addChild("bFixedWidth").setInnerInt(bFixedWidth);
	node.addChild("bFixedHeight").setInnerInt(bFixedHeight);
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

void poObject::clone(poObject *obj) {
	obj->alpha = alpha;
	obj->scale = scale;
	obj->position = position;
	obj->rotation = rotation;
	obj->rotationAxis = rotationAxis;
	obj->offset = offset;
	obj->visible = visible;
	obj->bFixedWidth = bFixedWidth;
	obj->bFixedHeight = bFixedHeight;
	obj->drawBounds = drawBounds;
	obj->matrixOrder = matrixOrder;
	obj->position_tween = position_tween;
	obj->scale_tween = scale_tween;
	obj->offset_tween = offset_tween;
	obj->alpha_tween = alpha_tween;
	obj->rotation_tween = rotation_tween;
	obj->width = width;
	obj->height = height;
	obj->_alignment = _alignment;
	
	for(int i=0; i<numChildren(); i++) {
		obj->children.push_back(children[i]->copy());
	}
	
	for(int i=0; i<numModifiers(); i++) {
		obj->modifiers.push_back(modifiers[i]->copy());
	}
	
	poEventCenter::get()->copyEventsFromObject(this, obj);
}


