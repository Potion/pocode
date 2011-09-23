/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poTween.h"
#include "poEvent.h"
#include "poDictionary.h"
#include "poMatrixSet.h"
#include "poMatrixStack.h"
#include "poEnums.h"
#include "poPoint.h"
#include "poColor.h"
#include "poRect.h"
#include "poObjectModifier.h"
#include "poXML.h"

class poObject;
typedef std::vector<poObject*> poObjectVec;

class poObjectModifier;
typedef std::vector<poObjectModifier*> poObjectModifierVec;

class poWindow;

/*! \class poObject
 \brief poObject is the central object in the potionCode library. All potionCode graphic objects(including poShape2D, poRectShape and poTextBox) are subclasses of poObject. <a href="http://www.potiondesign.com">Pocode wiki</a>.
 
 All poObjects and poObject subclasses:

 + Have position, scale, rotation, offset and tranparency (alpha) properties.<br>
 + Have a list of children that are also poObjects (this forms the scene graph).<br>
 + Can register for and receive events, including mouse, keyboard and touch events.<br>
 + Have a set of tweens for basic animation (animation may also be done in the "update" method).<br>
 + Have a list of poObjectModifiers that can modify the object and/or OpenGl state.<br>
*/



class poObject : public boost::noncopyable
{
	friend class poWindow;
public:
    //!
	/** @name OBJECT CREATION AND DESTRUCTION */
    //@{
                        poObject();
                        poObject(const std::string &name);
	virtual             ~poObject();
    //@}
    
    
    /*! Most poObject subclasses should implement all four of these methods.
     The only method you should call explictly is messageHandler. The other three are called automatically
     if the object is added to the scene graph. 
    */
    /** @name WORK-HORSE METHODS */
    //@{
    //!"draw" is only necessary if the class does its own OpenGL drawing, instead of using potionCode shapes.
    virtual void        draw();
    //!Draw the bounds if drawBounds is set.
    virtual void        _drawBounds();
    //!"update" is called once per frame. It should be used for implementing animation.
	virtual void        update();
    //!"eventHandler" is called when the object receives an event. Events are registered using "addEvent".<br>
	virtual void        eventHandler(poEvent *event);
    //!"messageHander" is a general utility method used for passing messages between any two poObjects.<br>
	virtual void        messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    //@}

    
    /*! All events are registed using "addEvent". See "poEnums.h" for a list of all eventTypes.*/
	/** @name EVENTS */
    //@{
	int                 addEvent(int eventType, poObject *sink, std::string message="", const poDictionary& dict=poDictionary());
	void                removeEvent(int event_id);
    //@}

    /*! Width/height should never be set directly!*/
	/** @name SETTERS/GETTERS */
    //@{
    float       getWidth();
    void        setWidth(float width);
    
    float       getHeight();
    void        setHeight(float height);
    
    void        setSize(float width, float height);
    void        setSize(poPoint size);
    
    poRect              getBounds();
    poRect              getFrame();
    //@}
    
    

    /*! The scene graph is a tree structure composed of poObjects and subclasses of poObject.
     A potionCode app is itself a poObject and is also the root of the tree.
     The most recently added objects appear on top of previously added objects.
     */
	/** @name SCENE GRAPH COMPOSITION*/
    //@{
    int                 numChildren() const;
	template <typename T>
	T*                  addChild(T* obj);
	template <typename T>
	T*                  addChild(T* obj, int idx); 
    template <typename T, typename TT>
	T*                  addChildBefore(T* obj, TT* before);
    template <typename T, typename TT>
	T*                  addChildAfter(T* obj, TT* after);
	int                 getChildIndex(poObject* child);
	poObject*           getChild(int at_idx);
	poObject*           getChildWithUID(uint uid);
	poObject*           getChild(const std::string &with_name);
	poObject*           getLastChild();
	std::vector<poObject*>  getChildren(const std::string &with_name);
	template <typename T> 
    T*                  getChildAs(int idx);
	template <typename T>
	T*                  getChildAs(const std::string &name);
	template <typename T>
	T*                  getLastChildAs();
	template <typename T>
	std::vector<T*>     getChildrenAs(const std::string &name);
	bool                removeChild(poObject* obj);
	bool                removeChild(int at_idx, bool and_delete=true);
	void                removeAllChildren(bool and_delete=true);
    //@}
    
    
	/** @name SCENE GRAPH ORDERING*/
    //@{
	void                moveChildToFront(poObject* child);
	void                moveChildToBack(poObject* child);
	void                moveChildForward(poObject* child);
	void                moveChildBackward(poObject* child);
	//@}
    
    
    /*! poObject modifiers attach to a poObject and may modify it's properties.
     poObjectModifiers have two virtual methods, doSetUp and doSetDown
     that are called, respectively, before and afer a poObject is drawn.
     */
    /** @name OBJECT MODIFIER OPERATIONS*/
    //@{
    template <typename T>
    T*                  addModifier(T* mod);
	template <typename T>
	T*                  getModifier(int idx);
	template <typename T>
	std::vector<T*>		getModifiers();
	bool                removeModifier(int idx, bool and_delete=true);
	bool                removeModifier(poObjectModifier* mod, bool and_delete=true);
    void                removeAllModifiers(bool and_delete=true);
	int                 numModifiers() const;
    //@}

    
    /*!pointInside assumes points are in window-native coordinates (0,0 is in the upper left).<br>
    globalToLocal and localToGlobal are useful in hitTesting and inter-object coordination.*/
    /** @name HIT TESTING & COORDINATE TRANSFORMATIONS*/
    //@{
	virtual bool        pointInside(poPoint point, bool localize=false);
	bool                pointInside(float x, float y, float z=0.f, bool localize=false);
	poPoint             objectToLocal(poObject* obj, poPoint point) const;
	poPoint             globalToLocal(poPoint point) const;
	poPoint             localToGlobal(poPoint point) const;
    //@}
    
    
    /** @name OBJECT ALIGNMENT & BOUNDS CALCULATION*/
    //@{
	poAlignment         alignment() const;
	virtual poObject&   alignment(poAlignment align);
	//@}
    
    
    /** @name TWEEN MANAGEMENT*/
    //@{    
    virtual void        stopAllTweens(bool recurse=false);
    //@}
    
    
    /*! Parent object should pass the child's poXMLNode node into the child.*/
    /** @name OBJECT SERIALIZATION */
    //@{
	virtual void        read(poXMLNode node);
	virtual void        write(poXMLNode &node);
	//@}
    
	/** @name SCENE GRAPH TRAVERSAL (DO NOT CALL) */
    //@{
	void                _drawTree();
	void                _updateTree();
	void                _broadcastEvent(poEvent* event);
    //@}

    
    /*! These properties may be set directly either inside or outside the class.
    Changes to these properties require no additional computation.*/
	/** @name OBJECT PROPERTIES (DIRECTLY ACCESSIBLE) */
    //@{
    std::string         name;
	float               alpha;
	poPoint             scale;
	poPoint             position;
	float               rotation;
	poPoint             rotationAxis;
	poPoint             offset;
	bool                visible;
	bool                bFixedWidth;
    bool                bFixedHeight;
    int                 drawBounds;
	poMatrixOrder       matrixOrder;
    //@}
    
    
    /* All poObjects have tween operators that may be enabled and disabled independently.
    By default, all tweens are disabled. See poTween.h for more about tweens.*/
 	/** @name OBJECT TWEENS (DIRECTLY ACCESSIBLE) */
    //@{   
	poTween<poPoint>	position_tween;
	poTween<poPoint>	scale_tween;
	poTween<poPoint>	offset_tween;
	poTween<float>		alpha_tween;
	poTween<float>		rotation_tween;
    //@}
    
    
    /** @name OBJECT PROPERTIES (PROTECTED) */
    //@{  
	poObject*           parent() const;
	uint                uid() const;
    //!alpha with parent alpha pre-multiplied
	float               appliedAlpha() const; 	
    poMatrixSet         matrixSet() const;
	int                 drawOrder() const;
	
    static const int    INVALID_INDEX = -1;
    //@}
    
    
protected:
    
    /** @name PROTECTED PROPERTIES*/
    //@{
	virtual void        updateAllTweens();//new tween types should be updated here
    float               true_alpha; 	//combination of all your parent's alphas with your own
    //@}     

private:
    
    float width, height;
    /*! These matrix operators not only push and pop the matrix, but also maintain
     the alpha stack and data required by the event handling system.*/
    /** @name COORDINATE SYSTEM OPERATIONS*/
    //@{
	void                pushObjectMatrix();
	void                popObjectMatrix();
	void                localizeEvent(poEvent*, poEvent*, poPoint);
	//@}
    
    /*! "children" is a vector of poObjects. It is the basis for the poObject scene graph/tree structure.
     Every poObject that has been added as child has a parent. Be careful not to access the parent
     in a poObject constructor, since it will not have one yet.*/
    /** @name CHILDREN, PARENT AND MODIFIERS*/
	//@{
    poObjectVec         children;
    poObject*           _parent;
    poObjectModifierVec modifiers;
	//@}
    
    /** @name PRIVATE PROPERTIES*/
    //@{
	uint                _uid;
	poAlignment         _alignment;

	poMatrixSet         matrices;
	int                 draw_order;
    //@}
};

/*! THESE METHODS MUST BE PLACED HERE INSTEAD OF THE CPP FILES, BECAUSE THEY ARE TEMPLATED.*/

template <typename T>
T* poObject::addChild(T* obj) {
	return addChild(obj, children.size());
}

template <typename T>
T* poObject::addChild(T* obj, int idx) {
	if(obj->_parent) {
		obj->_parent->removeChild(obj);
	}
	
	obj->_parent = this;
	children.insert(children.begin()+idx, obj);

	return obj;
}

template <typename T, typename TT>
T* poObject::addChildBefore(T* obj, TT* before) {
	int idx = getChildIndex(before);
    idx = (idx <= 0) ? 0 : idx - 1;
    return addChild(obj, idx);
}

template <typename T, typename TT>
T* poObject::addChildAfter(T* obj, TT* after) {
	int idx = getChildIndex(after);
    return addChild(obj, idx);
}

template <typename T>
T* poObject::getChildAs(int idx) {
	return static_cast<T*>(getChild(idx));
}
template <typename T>
T* poObject::getChildAs(const std::string &name) {
	return static_cast<T*>(getChild(name));
}
template <typename T>
T* poObject::getLastChildAs() {
	return static_cast<T*>(getLastChild());
}

template <typename T>
std::vector<T*> poObject::getChildrenAs(const std::string &name) {
    std::vector<T*> childrenT;
	BOOST_FOREACH(poObject *obj, getChildren(name))
        childrenT.push_back(static_cast<T*>(obj));
	return childrenT;
}

template <typename T>
T* poObject::addModifier(T* mod)
{
    modifiers.push_back( mod );
	return mod;
}
template <typename T>
T* poObject::getModifier(int idx) {
	return static_cast<T*>(modifiers[idx]);
}

template <typename T>
std::vector<T*> poObject::getModifiers() {
	std::vector<T*> mods;
	BOOST_FOREACH(poObjectModifier *mod, modifiers) {
		T* modT = dynamic_cast<T*>(mod);
		if(modT)
			mods.push_back(modT);
	}
	return mods;
}
