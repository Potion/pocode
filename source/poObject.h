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
	
	// WORLD COORDINATES
	bool			pointInside(poPoint point);
	bool			pointInside(float x, float y, float z=0.f);
	poPoint			globalToLocal(poPoint point) const;
	poPoint			localToGlobal(poPoint point) const;
	poPoint			objectToLocal(poObject* obj, poPoint point) const;

	// BOUNDING BOX
	virtual void	setAlignment(poAlignment align);
	virtual poRect	calculateBounds(bool include_children=false);
	
	// will return whatever child is directly under the mouse
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
	static float	master_alpha;

	// if you add new tweens make sure to update them here 
	virtual void	updateAllTweens();

private:
	void			pushObjectMatrix();
	void			popObjectMatrix();
	void			localizeEvent(poEvent*, poEvent*, poPoint);
	
	poObjectVec		children;
	poEventTable	events;
	
	poMatrixSet		matrices;
	
	static std::stack<float> alpha_stack;
};
