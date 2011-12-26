
//  Copyright 2011 Potion Design. All rights reserved.


#pragma once

#include "common.h"

#include "poTween.h"
#include "poEventCenter.h"
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

// poObject is the central object in the potionCode library. All potionCode graphic objects
// (including poShape2D, poRectShape and poTextBox) are subclasses poObject.
// 
// All poObjects and poObject subclasses:
//
// + Have position, scale, rotation, offset and tranparency (alpha) properties.
// + Have a list of children that are also poObjects (this forms the scene graph).
// + Can register for and receive events, including mouse, keyboard and touch events.
// + Have a set of tweens for basic animation (animation may also be done in the "update" method).
// + Have a list of poObjectModifiers that can modify the object and/or OpenGl state.



class poObject {
	friend class poWindow;
public:
	// OBJECT CREATION AND DESTRUCTION
    poObject();
    poObject(const std::string &name);
    poObject(int width, int height, const std::string &name="");
	virtual ~poObject();
	
	virtual poObject*	copy();
	
    // Most poObject subclasses should implement all four of these methods.
    // The only method you should call explictly is messageHandler. 
    // The other three are called automatically if the object is added to the scene graph. 
     
    /// WORK-HORSE METHODS
    // "draw" is only necessary if the class does its own OpenGL drawing, instead of using potionCode shapes.
    virtual void        draw();
    
    // Draw the bounds if drawBounds is set.
    virtual void        _drawBounds();
    
    // "update" is called once per frame. It should be used for implementing animation.
	virtual void        update();
    
    // "eventHandler" is called when the object receives an event. Events are registered using "addEvent".
	virtual void        eventHandler(poEvent *event);
    
    // "messageHander" is a general utility method used for passing messages between any two poObjects.
	virtual void        messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());

    //  All events are registed using "addEvent". See "poEnums.h" for a list of all eventTypes.
	// EVENTS
    
	void				addEvent(int eventType, poObject *sink, std::string message="", const poDictionary& dict=poDictionary());
    void				removeAllEvents();
	void				removeAllEventsOfType(int eventType);

    // OBJECT DIMENSIONS
    // The bounds are relative local to the object's coordinate system. The frame is relative
    // to the parents coordinate system. 
    
    float				getWidth();
    float				getHeight();
    virtual poRect      getBounds();
    poRect				getFrame();
    poPoint             getTransformedPoint( poPoint P );

    // The scene graph is a tree structure composed of poObjects and subclasses of poObject.
    // A potionCode app is itself a poObject and is also the root of the tree.
    // The most recently added objects appear on top of previously added objects.
    
	// SCENE GRAPH COMPOSITION
    int                 getNumChildren() const;
	poObject*			addChild(poObject* obj);
	poObject*			addChild(poObject* obj, int idx); 
	poObject*			addChildBefore(poObject* obj, poObject* before);
	poObject*			addChildAfter(poObject* obj, poObject* after);
	int                 getChildIndex(poObject* child);
	poObject*           getChild(int at_idx);
	poObject*           getChildWithUID(uint uid);
	poObject*           getChild(const std::string &with_name);
	poObject*           getLastChild();
	std::vector<poObject*> getChildren(const std::string &with_name);
	bool                removeChild(poObject* obj);
	bool                removeChild(int at_idx, bool and_delete=true);
	void                removeAllChildren(bool and_delete=true);
    
	// SCENE GRAPH ORDERING
	void                moveChildToFront(poObject* child);
	void                moveChildToBack(poObject* child);
	void                moveChildForward(poObject* child);
	void                moveChildBackward(poObject* child);
    
    
	// poObject modifiers attach to a poObject and may modify it's properties.
	// poObjectModifiers have two virtual methods, doSetUp and doSetDown
	// that are called, respectively, before and afer a poObject is drawn.

    // OBJECT MODIFIER OPERATIONS
    poObjectModifier*	addModifier(poObjectModifier* mod);
	poObjectModifier*	getModifier(int idx);
	std::vector<poObjectModifier*> const &getModifiers();
	bool                removeModifier(int idx, bool and_delete=true);
	bool                removeModifier(poObjectModifier* mod, bool and_delete=true);
    void                removeAllModifiers(bool and_delete=true);
	int                 getNumModifiers() const;
	
    // pointInside assumes points are in window-native coordinates (0,0 is in the upper left).
    // globalToLocal and localToGlobal are useful in hitTesting and inter-object coordination.
    
    // HIT TESTING & COORDINATE TRANSFORMATIONS
	virtual bool        pointInside(poPoint point, bool localize=false);
	bool                pointInside(float x, float y, float z=0.f, bool localize=false);
	poPoint             objectToLocal(poObject* obj, poPoint point) const;
	poPoint             globalToLocal(poPoint point) const;
	poPoint             localToGlobal(poPoint point) const;
    
    // OBJECT ALIGNMENT & BOUNDS CALCULATION
	poAlignment         getAlignment() const;
	virtual poObject&   setAlignment(poAlignment align);
	
    // TWEEN MANAGEMENT
    virtual void        stopAllTweens(bool recurse=false);
    
    //  Parent object should pass the child's poXMLNode node into the child.
    // OBJECT SERIALIZATION
	virtual void        read(poXMLNode node);
	virtual void        write(poXMLNode &node);
	
    
	// SCENE GRAPH TRAVERSAL (DO NOT CALL)
	virtual void        drawTree();
	virtual void        updateTree();
    
    // These properties may be set directly either inside or outside the class.
    // Changes to these properties require no additional computation.
	
    // OBJECT PROPERTIES (DIRECTLY ACCESSIBLE)
    std::string         name;
	float               alpha;
	poPoint             scale;
	poPoint             position;
	float               rotation;
	poPoint             rotationAxis;
	poPoint             offset;
	bool                visible;
    int                 drawBounds;
	poMatrixOrder       matrixOrder;
    
    
    // All poObjects have tween operators that may be enabled and disabled independently.
    // By default, all tweens are disabled. See poTween.h for more about tweens.
 	
    // OBJECT TWEENS (DIRECTLY ACCESSIBLE)
	poTween<poPoint>	positionTween;
	poTween<poPoint>	scaleTween;
	poTween<poPoint>	offsetTween;
	poTween<float>		alphaTween;
	poTween<float>		rotationTween;

    
	poObject*           getParent() const;
	uint                getUID() const;
    //!alpha with parent alpha pre-multiplied
	float               getAppliedAlpha() const;
    poMatrixSet&        getMatrixSet();
	int                 getDrawOrder() const;
    
    //Recursively check parents for definitive visibility
    bool                isVisible(); 
    
    // MEMORY MANAGEMENT
    int                 getSizeInMemoryTree();
    virtual int         getSizeInMemory();
	
    static const int    INVALID_INDEX = -1;
    
    poEventMemory       *eventMemory;
	int                 drawOrder;
    
protected:
    // PROTECTED PROPERTIES
    // new tween types should be updated within updateAllTweens
	virtual void        updateAllTweens();
    float               trueAlpha;
	void				clone(poObject* obj);
	
private:
    
    // PRIVATE PROPERTIES
    
    // "children" is a vector of poObjects. It is the basis for the poObject scene graph/tree structure.
    // Every poObject that has been added as child has a parent. Be careful not to access the parent
    // in a poObject constructor, since it will not have one yet.
    // CHILDREN, PARENT AND MODIFIERS
	
    poObjectVec         children;
    poObject*           parent;
    poObjectModifierVec modifiers;
    
	uint                uid;
	poAlignment         alignment;

	poMatrixSet         matrices;
};
