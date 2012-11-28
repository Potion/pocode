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

#include <iostream>
#include <boost/foreach.hpp>
#include <glm/gtx/string_cast.hpp>

#include "poObject.h"

#include "poApplication.h"
#include "poHelpers.h"
#include "poWindow.h"
#include "poOpenGLState.h"
#include "poSimpleDrawing.h"
#include "poCamera.h"

static uint PO_OBJECT_UID = 0;

namespace po {
    Object::Object()
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

    Object::Object(const std::string &name)
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


    Object::Object(int width, int height, const std::string &name)
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

    Object::~Object() {
        EventCenter::get()->removeAllEvents(this);
        removeAllChildren(true);
        if(eventMemory)
            delete eventMemory;
    }
    
    
    //------------------------------------------------------------------------
    Object *Object::copy() {
        Object *obj = new Object();
        clone(obj);
        return obj;
    }
    
    
    //------------------------------------------------------------------------
    void Object::clone(Object *obj) {
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
        
        EventCenter::get()->copyEventsFromObject(this, obj);
    }
    
    
    //------------------------------------------------------------------------
    void Object::draw() {}
    
    
    //------------------------------------------------------------------------
    void Object::update() {}
    
    
    //------------------------------------------------------------------------
    void Object::eventHandler(Event *event) {}
    
    
    //------------------------------------------------------------------------
    void Object::messageHandler(const std::string &msg, const Dictionary& dict) {
        if(parent) {
            parent->messageHandler(msg,dict);
        }
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // =============================== Dimensions ========================================
    #pragma mark Dimensions
    
    //------------------------------------------------------------------------
    float Object::getWidth() {
        return getBounds().width;
    }
    
    
    //------------------------------------------------------------------------
    float Object::getScaledWidth() {
        return getBounds().width * scale.x;
    }
    
    
    //------------------------------------------------------------------------
    float Object::getHeight() {
           return getBounds().height;
    }
    
    
    //------------------------------------------------------------------------
    float Object::getScaledHeight() {
        return getBounds().height * scale.y;
    }
    
    
    //------------------------------------------------------------------------
    Point Object::getTransformedPoint( Point P ) {   
        // This assumes standard transformation order (PO_MATRIX_ORDER_TRS)
        // It should include alternate orders.
        P += offset;
        P.x *= scale.x;
        P.y *= scale.y;
        P = P.getRotate2D(rotation );
        P += position;
        return P;
    }
    
    
    //------------------------------------------------------------------------
    Rect Object::getFrame() {
        Rect rect = getBounds();
        
        Point topLeft = getTransformedPoint( rect.getTopLeft() );
        Point topRight = getTransformedPoint( rect.getTopRight() );
        Point bottomRight = getTransformedPoint( rect.getBottomRight() );
        Point bottomLeft = getTransformedPoint( rect.getBottomLeft() );
        
        Rect frame;
        frame.setPosition( bottomRight );
        frame.include( topLeft );
        frame.include( topRight );
        frame.include( bottomLeft );
        
        return frame;
    }
    
    
    //------------------------------------------------------------------------
    Rect Object::getBounds() {    
        Rect rect(0,0,0,0);
        
        // must initialize rect with first object
        if ( children.size() > 0 )
            rect = children[0]->getFrame(); 
        
        BOOST_FOREACH(Object* obj, children)
            if (obj->visible) 
                rect.include( obj->getFrame() );
        
        return rect;
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // =============================== Events ========================================
    #pragma mark Events
    
    //------------------------------------------------------------------------
    void Object::addEvent(int eventType, Object *sink, std::string message, const Dictionary& dict) {
        if(!sink)
            sink = this;
        EventCenter::get()->addEvent(eventType, this, sink, message, dict);
    }
    
    
    //------------------------------------------------------------------------
    void Object::removeAllEvents() {
        EventCenter::get()->removeAllEvents(this);
    }
    
    
    //------------------------------------------------------------------------
    void Object::removeAllEventsOfType(int eventType) {
        EventCenter::get()->removeAllEventsOfType(this, eventType);
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // =============================== Draw Tree ========================================
    #pragma mark Draw Tree
    
   //------------------------------------------------------------------------
    Object* Object::addChild(Object* obj) {
        if(obj->parent != NULL) {
            std::cout << "poCode::Child " << obj->name << "added in two place, removing from old..." << std::endl;
            obj->parent->removeChild(obj);
        }
        
        obj->parent = this;
        children.push_back(obj);
        return obj;
    }
    
    
    //------------------------------------------------------------------------
    Object* Object::addChild(Object* obj, int idx) {
        if(obj->parent != NULL)
            obj->parent->removeChild(obj);
        obj->parent = this;
        children.insert(children.begin()+idx, obj);
        return obj;
    }
    
    
    //------------------------------------------------------------------------
    Object* Object::addChildBefore(Object* obj, Object* before) {
        if(obj->parent != NULL)
            obj->parent->removeChild(obj);
        obj->parent = this;
        children.insert(children.begin()+getChildIndex(before), obj);
        return obj;
    }
    
    
    //------------------------------------------------------------------------
    Object*  Object::addChildAfter(Object* obj, Object* after) {
        if(obj->parent != NULL)
            obj->parent->removeChild(obj);
        obj->parent = this;
        children.insert(children.begin()+getChildIndex(after)+1, obj);
        return obj;
    }
    
    
    //------------------------------------------------------------------------
    bool Object::removeChild(Object* obj) {
        ObjectVec::iterator iter = std::find(children.begin(), children.end(), obj);
        bool found = iter != children.end();
        
        if(found) {
            (*iter)->parent = NULL;
            children.erase(iter);
        }
        
        return found;
    }
    
    
    //------------------------------------------------------------------------
    bool Object::removeChild(int idx, bool and_delete) {
        if(idx < 0 || idx >= children.size())
            return false;
        
        children[idx]->parent = NULL;
        
        if(and_delete)
            delete children[idx];
        
        children.erase(children.begin()+idx);
        
        return true;
    }
    
    
    //------------------------------------------------------------------------
    void Object::removeAllChildren(bool and_delete) {
        BOOST_FOREACH(Object* obj, children) {
            obj->parent = NULL;
            if(obj && and_delete)
                delete obj;
        }
        children.clear();
    }
    
    
    //------------------------------------------------------------------------
    Object* Object::getParent() const {
        return parent;
    }
    
    
    //------------------------------------------------------------------------
    int Object::getNumChildren() const {
        return (int)children.size();
    }
    
    
    //------------------------------------------------------------------------
    Object* Object::getChild(int idx) {
        if(idx < 0 || idx >= children.size())
            return NULL;
        return *(children.begin()+idx);
    }
    
    
    //------------------------------------------------------------------------
    int Object::getChildIndex(Object* obj) {
        ObjectVec::iterator iter = std::find(children.begin(), children.end(), obj);
        if(iter != children.end())
            return (int)std::distance(children.begin(), iter);
        return INVALID_INDEX;
    }
    
    
    //------------------------------------------------------------------------
    Object* Object::getChildWithUID(uint UID) {
        BOOST_FOREACH(Object *obj, children) {
            Object *resp = obj->getChildWithUID(UID);
            if(resp) return resp;
        }
        
        if(UID == getUID())
            return this;
        
        return NULL;
    }
    
    
    //------------------------------------------------------------------------
    Object* Object::getChild(const std::string &name) {
        ObjectVec::iterator iter = std::find_if(children.begin(), children.end(), boost::bind(&Object::name, _1) == name);
        if(iter != children.end())
            return *iter;
        return NULL;
    }
    
    
    //------------------------------------------------------------------------
    Object* Object::getLastChild() {
        if(children.empty())
            return NULL;
        return children.back();
    }
    
    
    //------------------------------------------------------------------------
    std::vector<Object*> Object::getChildren(const std::string &name) {
        std::vector<Object*> childrenP;
        BOOST_FOREACH(Object *obj, children)
            if(obj->name == name)
                childrenP.push_back(obj);
        return childrenP;
    }
    
    
    //------------------------------------------------------------------------
    void Object::moveChildToFront(Object* child) {
        if(removeChild(child))
            addChild(child);
    }
    
    
    //------------------------------------------------------------------------
    void Object::moveChildToBack(Object* child) {
        if(removeChild(child))
            addChild(child, 0);
    }
    
    
    //------------------------------------------------------------------------
    void Object::moveChildForward(Object* child) {
        int idx = getChildIndex(child);
        if(removeChild(child))
            addChild(child, std::min(idx+1, getNumChildren()));
    }
    
    
    //------------------------------------------------------------------------
    void Object::moveChildBackward(Object* child) {
        int idx = getChildIndex(child);
        if(removeChild(child))
            addChild(child, std::max(idx-1, 0));
    }
    
    
    //------------------------------------------------------------------------
    void Object::updateTree() {
        if(!visible)
            return;
        
        updateAllTweens();
        update();
        
        BOOST_FOREACH(Object* obj, children) {
            obj->updateTree();
        }
    }
    
    
    //------------------------------------------------------------------------
    void Object::drawTree() {
        if(!visible)
            return;
        
        // reset the drawing order for this object
        drawOrder = po::applicationCurrentWindow()->getNextDrawOrder();
        
        if(parent)	trueAlpha = parent->trueAlpha * alpha;
        else		trueAlpha = alpha;
        
        po::saveModelview();
        
        applyTransformation();
        
        po::saveModelview();
        
        // grab the matrices we need for everything
        matrices.camType = Camera::getCurrentCameraType();
        matrices.capture();
        
        // set up the modifiers ... cameras, etc
        BOOST_FOREACH(ObjectModifier* mod, modifiers) {
            mod->setUp( this );
        }
        
        draw();
        if(drawBounds)
            _drawBounds();
        
        // go back to uncentered for children
        po::restoreModelview();
        
        // draw the children
        BOOST_FOREACH(Object* obj, children) {
            obj->drawTree();
        }
        
        // then recenter around offset
        // some modifiers might need the objects complete transform
        po::translate(offset);
        
        // let modifiers clean up
        BOOST_FOREACH(ObjectModifier* mod, modifiers) {
            mod->setDown( this );
        }
        
        // and restore parent's matrix
        po::restoreModelview();
    }
    
    
    
    
    //------------------------------------------------------------------------
    void Object::_drawBounds() {
        po::setLineWidth(1);
        po::setColor(Color::red);
        po::drawStrokedRect(getBounds());
        po::drawFilledRect(Rect(-Point(2.5,2.5), Point(5,5)));
        po::setColor(Color::blue);
        po::drawFilledRect(Rect(-offset-Point(2.5,2.5), Point(5,5)));
    }
    
    //------------------------------------------------------------------------
    void Object::applyTransformation() {
        switch(matrixOrder) {
            case PO_MATRIX_ORDER_TRS:
                po::translate(position);
                po::rotate(rotation, rotationAxis);
                po::scale(scale);
                break;
                
            case PO_MATRIX_ORDER_RST:
                po::rotate(rotation, rotationAxis);
                po::scale(scale);
                po::translate(position);
                break;
        }
        
        po::translate(offset);
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // =============================== Modifiers ========================================
    #pragma mark Modifiers
    
    //------------------------------------------------------------------------
    ObjectModifier* Object::addModifier(ObjectModifier* mod) {
        modifiers.push_back(mod);
        return mod;
    }
    
    
    //------------------------------------------------------------------------
    ObjectModifier* Object::getModifier(int idx) {
        return modifiers[idx];
    }
    
    
    //------------------------------------------------------------------------
    std::vector<ObjectModifier*> const &Object::getModifiers() {
        return modifiers;
    }
    
    
    //------------------------------------------------------------------------
    bool Object::removeModifier(int idx, bool and_delete) {
        if(idx >= 0 && idx < modifiers.size()) {
            if(and_delete)
                delete modifiers[idx];
            modifiers.erase(modifiers.begin() + idx);
            return true;
        }
        return false;
    }
    
    
    //------------------------------------------------------------------------
    bool Object::removeModifier(ObjectModifier* mod, bool and_delete) {
        ObjectModifierVec::iterator found = std::find(modifiers.begin(), modifiers.end(), mod);
        if(found != modifiers.end()) {
            if(and_delete)
                delete *found;
            modifiers.erase(found);
            return true;
        }
        return false;
    }
    
    
    //------------------------------------------------------------------------
    void Object::removeAllModifiers(bool and_delete) {
        if(and_delete) {
            BOOST_FOREACH(ObjectModifier* mod, modifiers) {
                delete mod;
            }
        }
        modifiers.clear();
    }
    
    
    //------------------------------------------------------------------------
    int Object::getNumModifiers() const {
        return modifiers.size();
    }
    
    
    
    
    // ------------------------------------------------------------------------------------
    // =============================== Hit Testing ========================================
    #pragma mark Hit Testing
    
    //------------------------------------------------------------------------
    bool Object::pointInside(Point point, bool localize) {
        // if invisible, return false
        if(!visible)
            return false;
        
        // check pointInside for all children
        BOOST_FOREACH(Object* obj, children) {
            if ( obj->pointInside( point, localize ) )
                return true;
        }
        
        // if there are children, but no pointInside, return false
        if ( getNumChildren() > 0 )
            return false;
        
        // if there are no children, check bounds rect
        if(localize) {
            point.y = po::getWindowHeight() - point.y;
            point = globalToLocal(point);
        }
        return getBounds().contains(point);
    }
    
    
    //------------------------------------------------------------------------
    bool Object::pointInside(float x, float y, float z, bool localize) {
        return pointInside(Point(x,y,z),localize);
    }
    
    
    //------------------------------------------------------------------------
    Point Object::globalToLocal(Point point) const {
        return matrices.globalToLocal(point);
    }
    
    
    //------------------------------------------------------------------------
    Point Object::localToGlobal(Point point) const {
        return matrices.localToGlobal(point);
    }
    
    
    //------------------------------------------------------------------------
    Point Object::objectToLocal(Object* obj, Point point) const {
        point = obj->localToGlobal(point);
        return globalToLocal(point);
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // =============================== Alignment ========================================
    #pragma mark Alignment
    
    //------------------------------------------------------------------------
    Alignment Object::getAlignment() const {
        return alignment;
    }
    
    
    //------------------------------------------------------------------------
    Object& Object::setAlignment(Alignment align) {
        alignment = align;
        
        if ( alignment == PO_ALIGN_NONE )
            return *this;
        
        // first calculate bounds
        Rect bounds = getBounds();
        
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
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Getter Functions =================================
    #pragma mark Getter Functions
    
    uint			Object::getUID()            const   { return uid; }
    float			Object::getAppliedAlpha()   const   { return trueAlpha; }
    int				Object::getDrawOrder()      const   { return drawOrder; }
    MatrixSet&    Object::getMatrixSet()              { return matrices; }
    
    
    
    //------------------------------------------------------------------------
    bool Object::isVisible() {
        Object *thisParent = getParent();
        while(thisParent) {
            if(!thisParent->visible) return false;
            
            thisParent = thisParent->getParent();
        }
        
        return true;
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Memory Usage =====================================
    #pragma mark Memory Usage
    
    //------------------------------------------------------------------------
    int Object::getSizeInMemoryTree() {
        // get own size
        int S = getSizeInMemory();
        // get size of children
        BOOST_FOREACH(Object* obj, children)
            S += obj->getSizeInMemoryTree();
        // should also get ObjectModifier sizes
        return S;
    }
    
    
    //------------------------------------------------------------------------
    int Object::getSizeInMemory() {
        return sizeof(Object);
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Tweens ===========================================
    #pragma mark Tweens
    
    //------------------------------------------------------------------------
    void Object::updateAllTweens() {
        positionTween.update();
        scaleTween.update();
        offsetTween.update();
        alphaTween.update();
        rotationTween.update();
    }
    
    
    //------------------------------------------------------------------------
    void Object::stopAllTweens(bool recurse) {
        positionTween.stop();
        scaleTween.stop();
        offsetTween.stop();
        alphaTween.stop();
        rotationTween.stop();
        
        if(recurse) {
            BOOST_FOREACH(Object* obj, children)
                obj->stopAllTweens(true);
        }
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Reading/Saving =====================================
    #pragma mark Reading/Saving
    
    //------------------------------------------------------------------------
    void Object::read(XMLNode node) {
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
        matrixOrder = MatrixOrder(node.getChild("matrixOrder").getInnerInt());
        alignment = Alignment(node.getChild("alignment").getInnerInt());
        setAlignment(alignment);
    }
    
    
    //------------------------------------------------------------------------
    void Object::write(XMLNode &node) {
        node.setAttribute("type", "Object");

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
} /* End po namespace */