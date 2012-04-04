/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "poHelpers.h"
#include "poObject.h"
#include "poWindow.h"
#include "poApplication.h"

#include "poCamera.h"

#include "poOpenGLState.h"
#include "poMatrixStack.h"

#include "poSimpleDrawing.h"

#include <iostream>
#include <boost/foreach.hpp>

static uint PO_OBJECT_UID = 0;

poObject::poObject() 
:	parent(NULL)
,	uid(PO_OBJECT_UID++)
,	name("")
,	alpha(1.f)
,	scale(1.f, 1.f, 1.f)
,	position(0.f, 0.f, 0.f)
,	rotation(0.f)
,	rotationAxis(0.f, 0.f, 1.f)
,	offset(0.f, 0.f, 0.f)
,	alignment(PO_ALIGN_NONE)
,	visible(true)
,	matrixOrder(PO_MATRIX_ORDER_TRS)
,	drawOrder(-1)
,	positionTween(&position)
,	scaleTween(&scale)
,	offsetTween(&offset)
,	alphaTween(&alpha)
,	rotationTween(&rotation)
,	trueAlpha(1.f)
,	drawBounds(false)
,	eventMemory(NULL)
{}

poObject::poObject(const std::string &name)
:	parent(NULL)
,	uid(PO_OBJECT_UID++)
,	name(name)
,	alpha(1.f)
,	scale(1.f, 1.f, 1.f)
,	position(0.f, 0.f, 0.f)
,	rotation(0.f)
,	rotationAxis(0.f, 0.f, 1.f)
,	offset(0.f, 0.f, 0.f)
,	alignment(PO_ALIGN_NONE)
,	visible(true)
,	matrixOrder(PO_MATRIX_ORDER_TRS)
,	drawOrder(-1)
,	positionTween(&position)
,	scaleTween(&scale)
,	offsetTween(&offset)
,	alphaTween(&alpha)
,	rotationTween(&rotation)
,	trueAlpha(1.f)
,	drawBounds(false)
,	eventMemory(NULL)
{}


poObject::poObject(int width, int height, const std::string &name)
:	parent(NULL)
,	uid(PO_OBJECT_UID++)
,	name(name)
,	alpha(1.f)
,	scale(1.f, 1.f, 1.f)
,	position(0.f, 0.f, 0.f)
,	rotation(0.f)
,	rotationAxis(0.f, 0.f, 1.f)
,	offset(0.f, 0.f, 0.f)
,	alignment(PO_ALIGN_NONE)
,	visible(true)
,	matrixOrder(PO_MATRIX_ORDER_TRS)
,	drawOrder(-1)
,	positionTween(&position)
,	scaleTween(&scale)
,	offsetTween(&offset)
,	alphaTween(&alpha)
,	rotationTween(&rotation)
,	trueAlpha(1.f)
,	drawBounds(false)
,	eventMemory(NULL)
{
}

poObject::~poObject() {
    poEventCenter::get()->removeAllEvents(this);
	removeAllChildren(true);
	if(eventMemory)
		delete eventMemory;
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
	if(parent) {
		parent->messageHandler(msg,dict);
	}
}

float poObject::getWidth() {
    return getBounds().width;
}

float poObject::getScaledWidth() {
    return getBounds().width * scale.x;
}

float poObject::getHeight() {
       return getBounds().height;
}

float poObject::getScaledHeight() {
    return getBounds().height * scale.y;
}


poPoint poObject::getTransformedPoint( poPoint P )
{   
    // This assumes standard transformation order (PO_MATRIX_ORDER_TRS)
    // It should include alternate orders.
    P += offset;
    P.x *= scale.x;
    P.y *= scale.y;
    P = P.getRotate2D(rotation );
    P += position;
    return P;
}
 
poRect poObject::getFrame() {
    poRect rect = getBounds();
    
    poPoint topLeft = getTransformedPoint( rect.getTopLeft() );
    poPoint topRight = getTransformedPoint( rect.getTopRight() );
    poPoint bottomRight = getTransformedPoint( rect.getBottomRight() );
    poPoint bottomLeft = getTransformedPoint( rect.getBottomLeft() );
    
    poRect frame;
    frame.setPosition( bottomRight );
    frame.include( topLeft );
    frame.include( topRight );
    frame.include( bottomLeft );
    
	return frame;
}


poRect poObject::getBounds()
{    
    poRect rect(0,0,0,0);
    
    // must initialize rect with first object
    if ( children.size() > 0 )
        rect = children[0]->getFrame(); 
    
	BOOST_FOREACH(poObject* obj, children)
        if (obj->visible) 
            rect.include( obj->getFrame() );
    
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
		(*iter)->parent = NULL;
		children.erase(iter);
	}
	
	return found;
}

bool poObject::removeChild(int idx, bool and_delete) {
	if(idx < 0 || idx >= children.size())
		return false;

	children[idx]->parent = NULL;

	if(and_delete)
		delete children[idx];

	children.erase(children.begin()+idx);
	
	return true;
}

void poObject::removeAllChildren(bool and_delete) {
	BOOST_FOREACH(poObject* obj, children) {
		obj->parent = NULL;
		if(obj && and_delete)
			delete obj;
	}
	children.clear();
}

int poObject::getNumChildren() const {
	return (int)children.size();
}

poObject* poObject::addChild(poObject* obj) {
	if(obj->parent != NULL) {
        std::cout << "poCode::Child " << obj->name << "added in two place, removing from old..." << std::endl;
		obj->parent->removeChild(obj);
    }
    
	obj->parent = this;
	children.push_back(obj);
    return obj;
}

poObject* poObject::addChild(poObject* obj, int idx) {
	if(obj->parent != NULL)
		obj->parent->removeChild(obj);
	obj->parent = this;
	children.insert(children.begin()+idx, obj);
    return obj;
}

poObject* poObject::addChildBefore(poObject* obj, poObject* before) {
	if(obj->parent != NULL)
		obj->parent->removeChild(obj);
	obj->parent = this;
	children.insert(children.begin()+getChildIndex(before), obj);
    return obj;
}

poObject*  poObject::addChildAfter(poObject* obj, poObject* after) {
	if(obj->parent != NULL)
		obj->parent->removeChild(obj);
	obj->parent = this;
	children.insert(children.begin()+getChildIndex(after)+1, obj);
    return obj;
}

poObject* poObject::getChild(int idx) {
	if(idx < 0 || idx >= children.size())
		return NULL;
	return *(children.begin()+idx);
}

int poObject::getChildIndex(poObject* obj) {
	poObjectVec::iterator iter = std::find(children.begin(), children.end(), obj);
	if(iter != children.end())
		return (int)std::distance(children.begin(), iter);
	return INVALID_INDEX;
}

poObject* poObject::getChildWithUID(uint UID) {
	BOOST_FOREACH(poObject *obj, children) {
		poObject *resp = obj->getChildWithUID(UID);
		if(resp) return resp;
	}
	
	if(UID == getUID())
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
	addChild(child, std::min(idx+1, getNumChildren()));
}

void poObject::moveChildBackward(poObject* child) {
	int idx = getChildIndex(child);
	removeChild(child);
	addChild(child, std::max(idx-1, 0));
}

poObjectModifier* poObject::addModifier(poObjectModifier* mod) {
	modifiers.push_back(mod);
	return mod;
}

poObjectModifier* poObject::getModifier(int idx) {
	return modifiers[idx];
}

std::vector<poObjectModifier*> const &poObject::getModifiers() {
	return modifiers;
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

void poObject::removeAllModifiers(bool and_delete) {
	if(and_delete) {
		BOOST_FOREACH(poObjectModifier* mod, modifiers) {
			delete mod;
		}
	}
    modifiers.clear();
}

int poObject::getNumModifiers() const {
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
    if ( getNumChildren() > 0 )
        return false;
    
    // if there are no children, check bounds rect
    if(localize) {
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

poAlignment poObject::getAlignment() const {return alignment;}

poObject& poObject::setAlignment(poAlignment align) {
	alignment = align;
	
    if ( alignment == PO_ALIGN_NONE )
        return *this;
    
	// first calculate bounds
	poRect bounds = getBounds();
	
	float offsetZ = offset.z;
	
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
	offset.z = offsetZ;
	
	return *this;
}

poObject*		poObject::getParent() const {return parent;}
uint			poObject::getUID() const {return uid;}

float			poObject::getAppliedAlpha() const {return trueAlpha;}
poMatrixSet&    poObject::getMatrixSet()  {return matrices;}
int				poObject::getDrawOrder() const {return drawOrder;}

int             poObject::getSizeInMemoryTree() {
    // get own size
    int S = getSizeInMemory();
    // get size of children
    BOOST_FOREACH(poObject* obj, children)
        S += obj->getSizeInMemoryTree();
    // should also get poObjectModifier sizes
    return S;
}

int             poObject::getSizeInMemory() {
    return sizeof(poObject);
}


bool poObject::isVisible() {
    poObject *thisParent = getParent();
    while(thisParent) {
        if(!thisParent->visible) return false;
           
        thisParent = thisParent->getParent();
    }
           
    return true;
}


void poObject::drawTree() {
	if(!visible)
		return;
	
	// reset the drawing order for this object
	drawOrder = applicationCurrentWindow()->getNextDrawOrder();
	
	if(parent)	trueAlpha = parent->trueAlpha * alpha;
	else		trueAlpha = alpha;
    
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
    matrices.camType = poCamera::getCurrentCameraType();
	matrices.capture();

	// set up the modifiers ... cameras, etc
    BOOST_FOREACH(poObjectModifier* mod, modifiers) {
        mod->setUp( this );
    }
	
	draw();
    if(drawBounds) 
		_drawBounds();
    
	// go back to centered around the origin
	stack->translate(-offset);
	
	// draw the children
	BOOST_FOREACH(poObject* obj, children) {
		obj->drawTree();
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

void poObject::updateTree() {
	if(!visible)
		return;
	
	updateAllTweens();
	update();
	
	BOOST_FOREACH(poObject* obj, children) {
		obj->updateTree();
	}
}

void poObject::_drawBounds() {
	po::setStrokeWidth(1);
    po::setColor(poColor::red);
    po::drawStrokedRect(getBounds());
    po::drawFilledRect(poRect(-poPoint(2.5,2.5), poPoint(5,5)));
    po::setColor(poColor::blue);
    po::drawFilledRect(poRect(-offset-poPoint(2.5,2.5), poPoint(5,5)));
}

void poObject::stopAllTweens(bool recurse) {
	positionTween.stop();
	scaleTween.stop();
	offsetTween.stop();
	alphaTween.stop();
	rotationTween.stop();
	
	if(recurse) {
		BOOST_FOREACH(poObject* obj, children)
			obj->stopAllTweens(true);
	}
}

void poObject::read(poXMLNode node) {
	uid = (uint)node.getChild("uid").getInnerInt();
	name = node.getChild("name").getInnerString();
	position.fromString(node.getChild("position").getInnerString());
	//width   = node.getChild("width").getInnerFloat();
	//height  = node.getChild("height").getInnerFloat();
	scale.fromString(node.getChild("scale").getInnerString());
	alpha = node.getChild("alpha").getInnerFloat();
	rotation = node.getChild("rotation").getInnerFloat();
	rotationAxis.fromString(node.getChild("rotationAxis").getInnerString());
	offset.fromString(node.getChild("offset").getInnerString());
	visible = (bool)node.getChild("visible").getInnerInt();
	matrixOrder = poMatrixOrder(node.getChild("matrixOrder").getInnerInt());
	alignment = poAlignment(node.getChild("alignment").getInnerInt());
	setAlignment(alignment);
}

void poObject::write(poXMLNode &node) {
	node.setAttribute("type", "poObject");

	node.addChild("uid").setInnerInt(getUID());
	node.addChild("name").setInnerString(name);
	node.addChild("position").setInnerString(position.toString());
	//node.addChild("width").setInnerFloat(width);
    //node.addChild("height").setInnerFloat(height);
	node.addChild("scale").setInnerString(scale.toString());
	node.addChild("alpha").setInnerFloat(alpha);
	node.addChild("rotation").setInnerFloat(rotation);
	node.addChild("rotationAxis").setInnerString(rotationAxis.toString());
	node.addChild("offset").setInnerString(offset.toString());
	node.addChild("visible").setInnerInt(visible);
	node.addChild("matrixOrder").setInnerInt(matrixOrder);
	node.addChild("alignment").setInnerInt(alignment);
}

void poObject::updateAllTweens() {
	positionTween.update();
	scaleTween.update();
	offsetTween.update();
	alphaTween.update();
	rotationTween.update();
}

void poObject::clone(poObject *obj) {
	obj->alpha = alpha;
	obj->scale = scale;
	obj->position = position;
	obj->rotation = rotation;
	obj->rotationAxis = rotationAxis;
	obj->offset = offset;
	obj->visible = visible;
	obj->drawBounds = drawBounds;
	obj->matrixOrder = matrixOrder;
	obj->positionTween = positionTween;
	obj->scaleTween = scaleTween;
	obj->offsetTween = offsetTween;
	obj->alphaTween = alphaTween;
	obj->rotationTween = rotationTween;
	obj->alignment = alignment;
	
	for(int i=0; i<getNumChildren(); i++) {
		obj->children.push_back(children[i]->copy());
	}
	
	for(int i=0; i<getNumModifiers(); i++) {
		obj->modifiers.push_back(modifiers[i]->copy());
	}
	
	poEventCenter::get()->copyEventsFromObject(this, obj);
}


