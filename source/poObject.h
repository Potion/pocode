/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poTween.h"
#include "poEvent.h"
#include "poDictionary.h"
#include "poMatrixSet.h"
#include "poEnums.h"
#include "poPoint.h"
#include "poColor.h"
#include "poRect.h"

#define kAddedToWindow "added to window message"
#define kRemovedFromWindow "removed from window message"

class poModifier;
class poWindow;

typedef std::vector<poModifier*> poModifierVec;

class poObject;
typedef std::vector<poObject*> poObjectVec;

class poObject
:	public boost::noncopyable
{
public:
	static const int INVALID_INDEX = -1;
	
	// OBJECT CREATION AND DESTRUCTION
					poObject();
					poObject(const std::string &name);
	virtual			~poObject();
	
    // will get called once the opengl context exists
    virtual void    setup();
	
	// WORK-HORSES
	virtual void	draw();
	virtual void	update();
	virtual bool	eventHandler(poEvent* event);
	virtual void	messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());

	// pass null to remove from a window
	void			setWindow(poWindow* win);

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
	poWindow*		window;

	poPointTween	position_tween;
	poPointTween	scale_tween;
	poFloatTween	alpha_tween;
	poFloatTween	rotation_tween;

	// TREE TRAVERSAL
	void			_drawTree();
	void			_updateTree();
	// unchecked events that everyone should see
	void			_broadcastEvent(poEvent* event);
	// targeted events for a specific object
	bool			_processEvent(poEvent* event);

protected:
	static float	master_alpha;

	// if you add new tweens make sure to update them here 
	virtual void	updateAllTweens();

private:
	void			pushObjectMatrix();
	void			popObjectMatrix();
	void			localizeEvent(poEvent*, poEvent*, poPoint);
	
	poObjectVec		children;
	poModifierVec	modifiers;
	poEventTable	events;
	
	poMatrixSet		matrices;
	
	static std::stack<float> alpha_stack;
};
