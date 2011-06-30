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

class poWindow;

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
	poObject*		getLastChild();
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
	virtual bool	pointInside(poPoint point, bool localize=false);
	bool			pointInside(float x, float y, float z=0.f, bool localize=false);

	poPoint			globalToLocal(poPoint point) const;
	poPoint			localToGlobal(poPoint point) const;
	poPoint			objectToLocal(poObject* obj, poPoint point) const;

	// BOUNDING BOX
	poAlignment		alignment() const;
	virtual poObject& alignment(poAlignment align);
	// recursively compute the bounds of you and your children
	virtual poRect	calculateBounds();
	
	// OBJECT PROPERTIES
	poObject*		parent() const;
	bool			isInWindow() const;
	
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
	
	bool			visible() const;
	poObject&		visible(bool b);
	
	poMatrixOrder	matrixOrder() const;
	poObject&		matrixOrder(poMatrixOrder o);
    
    poMatrixSet     matrixSet() const;
	
	int				drawOrder() const;
	
	poTween<poPoint>	position_tween;
	poTween<poPoint>	scale_tween;
	poTween<poPoint>	offset_tween;
	poTween<float>		alpha_tween;
	poTween<float>		rotation_tween;

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
	void			inWindow(bool b);
	
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
	bool			in_window;
	bool			fixed_bounds;
	
	friend class poWindow;
};

template <typename T>
inline T *getChildAs(poObject *parent, int idx) {
	return static_cast<T*>(parent->getChild(idx));
}

template <typename T>
inline T *getChildAs(poObject *parent, const std::string &name) {
	return static_cast<T*>(parent->getChild(name));
}

