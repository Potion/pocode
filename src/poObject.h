/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poCommon.h"
#include "poTween.h"

class poModifier;

typedef std::vector<poModifier*> poModifierVec;

class poObject;
typedef std::vector<poObject*> poObjectVec;

enum poObjectProperty {
	PO_OBJECT_PROP_POSITION = 0,
	PO_OBJECT_PROP_SCALE,
	PO_OBJECT_PROP_ROTATION,
	PO_OBJECT_PROP_ROTATION_AXIS,
	PO_OBJECT_PROP_ALPHA,
	
	PO_OBJECT_PROP_NUM_OPTIONS
};

class poObject
:	public boost::noncopyable
{
public:
	static const int INVALID_INDEX = -1;
	
	// OBJECT CREATION AND DESTRUCTION
					poObject();
					poObject(const std::string &name);
	virtual			~poObject();
	
	// WORK-HORSES
	virtual void	draw();
	virtual void	update();
	virtual bool	eventHandler(poEvent* event);
	virtual void	messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());

	// DISPLAY LIST
	void			addChild(poObject* obj);
	void			addChild(poObject* obj, int idx);
	bool			removeChild(poObject* obj);
	bool			removeChild(int at_idx, bool and_delete=true);
	void			removeAllChildren(bool and_delete=false);
	int				numChildren() const;
	poObject*		getChild(int at_idx);
	poObject*		getChild(const std::string &with_name);
	int				getChildIndex(poObject* child);
	// move it relative to its siblibings
	void			moveChildToFront(poObject* child);
	void			moveChildToBack(poObject* child);
	void			moveChildForward(poObject* child);
	void			moveChildBackward(poObject* child);
	
	// OBJECT MODIFIERS
	void			addModifier(poModifier* mod, int idx=-1);
	bool			removeModifier(poModifier* mod);
	bool			removeModifier(int idx);
	void			removeAllModifiers(bool and_delete=false);
	int				numModifiers() const;
	poModifier*		getModifierAtIndex(int idx);
	poModifier*		getFirstModifierOfTypeName(const std::string &typeName);
	poModifierVec	getModifiersByTypeName(const std::string &typeName);
	
	// WORLD COORDINATES
	bool			pointInside(poPoint point);
	bool			pointInside(float x, float y, float z=0.f);
	poPoint			globalToLocal(poPoint point) const;
	poPoint			localToGlobal(poPoint point) const;
	poPoint			objectToLocal(poObject* obj, poPoint point) const;

	// BOUNDING BOX
	virtual void	setAlignment(poAlignment align);
	virtual poRect	calculateBounds(bool include_children=false);
	
	// OH, THE EVENTS
	poEvent*		addEvent(int type, poObject* handler=NULL, const std::string &msg="", const poDictionary& dict=poDictionary());
	// check if there are any events of this type
	bool			hasEvents(int type);
	// check if there is an event with a specific receiver of this type
	bool			hasEvent(int type, poObject* handler);
	// remove all events of this type
	void			removeEvents(int type);
	// remove any events of this type with this receiver
	void			removeEvent(int type, poObject* handler);
	// just pull them all and delete them
	void			removeAllEvents();
	// figure out if the mouse is over something
	poObject*		objectUnderMouse(float x, float y);

	void tween(poObjectProperty prop, poTweenMajorType major, poTweenMinorType minor, 
			   float end_value, float duration, float delay=0.f, poTweenRepeat repeat=PO_TWEEN_REPEAT_NONE, int repeat_count=0, poObject *obj=NULL, const std::string &msg="");
	void tween(poObjectProperty prop, poTweenMajorType major, poTweenMinorType minor, 
			   poPoint end_value, float duration, float delay=0.f, poTweenRepeat repeat=PO_TWEEN_REPEAT_NONE, int repeat_count=0, poObject *obj=NULL, const std::string &msg="");
	bool hasTweenForProperty(poObjectProperty prop);
	void cancelTweenForProperty(poObjectProperty prop);
	
	// OBJECT PROPERTIES
	poObject*		parent;
	std::string		name;
	float			alpha;
	poPoint			scale;
	poPoint			position;
	float			rotation;
	poPoint			rotation_axis;
	poPoint			offset;
	poRect			bounds;
	poAlignment		align;
	bool			enabled;
	poMatrixOrder	matrix_order;

	// TREE TRAVERSAL
	void			_drawTree();
	void			_updateTree();
	// unchecked events that everyone should see
	void			_broadcastEvent(poEvent* event);
	// targeted events for a specific object
	bool			_processEvent(poEvent* event);

protected:
	static float	master_alpha;
	// add property addresses thru these in subclasses
	virtual void	addrForProperty(poObjectProperty prop, poPoint **addr);
	virtual void	addrForProperty(poObjectProperty prop, float **addr);

private:
	void			pushObjectMatrix();
	void			popObjectMatrix();
	void			localizeEvent(poEvent*, poEvent*, poPoint);
	
	poObjectVec		children;
	poModifierVec	modifiers;
	poEventTable	events;
	
	poMatrixSet		matrices;

	std::vector<poTween> tweens;
	std::vector< std::vector<int> > tweens_for_prop;
	
	static std::stack<float> alpha_stack;
};
