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

class poObject;
typedef std::vector<poObject*> poObjectVec;

class poObjectModifier;
typedef std::vector<poObjectModifier*> poObjectModifierVec;


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
	virtual void	eventHandler(poEvent *event);
	virtual void	messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());

	// events
	int				addEvent(int eventType, poObject *sink, std::string message, const poDictionary& dict=poDictionary());
	void			removeEvent(int event_id);
	
	// useful for stuff like cameras, fbos, masks, etc
	virtual void	preDraw();
	virtual void	postDraw();
	
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
	
    // MODIFIERS
    void            addModifier(poObjectModifier* mod);
    void            removeAllModifiers();
    
	// localize will convert global to local first
	// otherwise, point is assumed to be local
	bool			pointInside(poPoint point, bool localize=false);
	bool			pointInside(float x, float y, float z=0.f, bool localize=false);

	poPoint			globalToLocal(poPoint point) const;
	poPoint			localToGlobal(poPoint point) const;
	poPoint			objectToLocal(poObject* obj, poPoint point) const;

	// BOUNDING BOX
	virtual void	setAlignment(poAlignment align);
	virtual poRect	calculateBounds(bool include_children=false);
	
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
	bool			visible;
	poMatrixOrder	matrix_order;
	int				draw_order;
	
	poPointTween	position_tween;
	poPointTween	scale_tween;
	poFloatTween	alpha_tween;
	poFloatTween	rotation_tween;

	// TREE TRAVERSAL
	void			_drawTree();
	void			_updateTree();
	// unchecked events that everyone should see
	void			_broadcastEvent(poEvent* event);

protected:
	// if you add new tweens make sure to update them here 
	virtual void	updateAllTweens();
	float			true_alpha;

private:
	void			pushObjectMatrix();
	void			popObjectMatrix();
	void			localizeEvent(poEvent*, poEvent*, poPoint);
	
	poObjectVec             children;
    poObjectModifierVec     modifiers;
	poEventTable            events;
	
	poMatrixSet		matrices;
};

template <typename T>
inline T *getChildAs(poObject *parent, int idx) {
	return static_cast<T*>(parent->getChild(idx));
}

template <typename T>
inline T *getChildAs(poObject *parent, const std::string &name) {
	return static_cast<T*>(parent->getChild(name));
}


class poObjectModifier
{
public:
    poObjectModifier() { };
    
    virtual void setUp( poObject* obj ) { };
    virtual void setDown( poObject* obj ) { };
};

