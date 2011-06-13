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
#include "poObjectModifier.h"

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
	int				addEvent(int eventType, poObject *sink, std::string message="", const poDictionary& dict=poDictionary());
	int				addEvent(int eventType, poObject *sink, std::string message);
	int				addEvent(int eventType, poObject *sink, const poDictionary &dict);
	void			removeEvent(int event_id);
	
	// DISPLAY LIST
	void			addChild(poObject* obj);
	void			addChild(poObject* obj, int idx);
	bool			removeChild(poObject* obj);
	bool			removeChild(int at_idx, bool and_delete=true);
	void			removeAllChildren(bool and_delete=true);
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
	bool			removeModifier(poObjectModifier* mod);
    void            removeAllModifiers(bool and_delete=true);
    
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
	poObject*		parent();
	
	std::string		name() const;
	poObject&		name(const std::string &str);
	
	float			alpha() const;
	poObject&		alpha(float f);
	
	poPoint			scale() const;
	poObject&		scale(poPoint pt);
	poObject&		scale(float x, float y, float z=1.f);
	
	poPoint			position() const;
	poObject&		position(poPoint p);
	poObject&		position(float x, float y, float z=0.f);
	
	float			rotation() const;
	poObject&		rotation(float f);
	
	poPoint			rotationAxis() const;
	poObject&		rotationAxis(poPoint p);
	poObject&		rotationAxis(float x, float y, float z);
	
	poPoint			offset() const;
	poObject&		offset(poPoint p);
	poObject&		offset(float x, float y, float z);
	
	poRect			bounds() const;
	poObject&		bounds(poRect r);
	
	poAlignment		alignment() const;
	
	bool			visible() const;
	poObject&		visible(bool b);
	
	poMatrixOrder	matrixOrder() const;
	poObject&		matrixOrder(poMatrixOrder o);
    
    poMatrixSet     matrixSet() const;
	
	int				drawOrder() const;
	
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
	
	poObject*		_parent;
	std::string		_name;
	float			_alpha;
	poPoint			_scale;
	poPoint			_position;
	float			_rotation;
	poPoint			_rotation_axis;
	poPoint			_offset;
	poRect			_bounds;
	poAlignment		_alignment;
	bool			_visible;
	poMatrixOrder	_matrix_order;
	int				_draw_order;
};

template <typename T>
inline T *getChildAs(poObject *parent, int idx) {
	return static_cast<T*>(parent->getChild(idx));
}

template <typename T>
inline T *getChildAs(poObject *parent, const std::string &name) {
	return static_cast<T*>(parent->getChild(name));
}

