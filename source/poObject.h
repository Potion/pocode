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


class poObject;
typedef std::vector<poObject*> poObjectVec;

class poObjectModifier;
typedef std::vector<poObjectModifier*> poObjectModifierVec;

class poWindow;

class poObject
:	public boost::noncopyable
{
	friend class poWindow;
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
	int				numChildren() const;

	template <typename T>
	T*				addChild(T* obj);
	template <typename T>
	T*				addChild(T* obj, int idx);

	int				getChildIndex(poObject* child);
	poObject*		getChild(int at_idx);
	poObject*		getChild(const std::string &with_name);
	poObject*		getLastChild();

	template <typename T>
	T*				getChildAs(int idx);
	template <typename T>
	T*				getChildAs(const std::string &name);
	template <typename T>
	T*				getLastChildAs();
	
	
	bool			removeChild(poObject* obj);
	bool			removeChild(int at_idx, bool and_delete=true);
	void			removeAllChildren(bool and_delete=true);

	// move it relative to its siblibings
	void			moveChildToFront(poObject* child);
	void			moveChildToBack(poObject* child);
	void			moveChildForward(poObject* child);
	void			moveChildBackward(poObject* child);
	
    // MODIFIERS
	template <typename T>
    T*				addModifier(T* mod);
	template <typename T>
	T*				getModifier(int idx);
	bool			removeModifier(int idx, bool and_delete=true);
	bool			removeModifier(poObjectModifier* mod, bool and_delete=true);
    void            removeAllModifiers(bool and_delete=true);
	int				numModifiers() const;
	
//	template <typename T>
//	T				getProperty(const std::string &prop, T def=T()) const;
//	template <typename T>
//	void			setProperty(const std::string &prop, T value);
//	poDictionary	getProperties() const;
//	void			setProperties(const poDictionary &dict);

    // assumes that all incoming points are in window-native coordinates (0,0 is in the upper left)
	// localize will convert global to local first
	// otherwise, point is assumed to be local
	virtual bool	pointInside(poPoint point, bool localize=false);
	bool			pointInside(float x, float y, float z=0.f, bool localize=false);
	// will convert a point from the coordinate system of obj into local coordinates
	poPoint			objectToLocal(poObject* obj, poPoint point) const;
	// be careful as these functions expect opengl-native coordinates
	poPoint			globalToLocal(poPoint point) const;
	poPoint			localToGlobal(poPoint point) const;

	// BOUNDING BOX
	poAlignment		alignment() const;
	virtual poObject& alignment(poAlignment align);
	
	// recursively compute the bounds of you and your children
	virtual poRect	calculateBounds();
	
	// OBJECT PROPERTIES
	poObject*		parent() const;
	bool			isInWindow() const;
	// this is the alpha with parent alpha pre-multiplied
	float			appliedAlpha() const;
    poMatrixSet     matrixSet() const;
	int				drawOrder() const;
	
	std::string		name;
	float			alpha;
	poPoint			scale;
	poPoint			position;
	float			rotation;
	poPoint			rotationAxis;
	poPoint			offset;
	poRect			bounds;
	bool			visible;
	bool			boundsAreFixed;
	poMatrixOrder	matrixOrder;

	poTween<poPoint>	position_tween;
	poTween<poPoint>	scale_tween;
	poTween<poPoint>	offset_tween;
	poTween<float>		alpha_tween;
	poTween<float>		rotation_tween;
	
	virtual void	stopAllTweens(bool recurse=false);

	// TREE TRAVERSAL
	void			_drawTree();
	void			_updateTree();
	// unchecked events that everyone should see
	void			_broadcastEvent(poEvent* event);

protected:
	// if you add new tweens make sure to update them here 
	virtual void	updateAllTweens();
	// this is the combination of all your parent's alphas with your own
	float			true_alpha;

private:
	void			pushObjectMatrix();
	void			popObjectMatrix();
	void			localizeEvent(poEvent*, poEvent*, poPoint);
	void			inWindow(bool b);
	
	poObjectVec             children;
    poObjectModifierVec     modifiers;
	poEventTable            events;
	
	poObject*		_parent;
	poAlignment		_alignment;

	poMatrixSet		matrices;
	int				draw_order;
	bool			in_window;
};

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
	obj->inWindow(in_window);
	children.insert(children.begin()+idx, obj);

	return obj;
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
T* poObject::addModifier(T* mod)
{
    modifiers.push_back( mod );
	return mod;
}
template <typename T>
T* poObject::getModifier(int idx) {
	return static_cast<T*>(modifiers[idx]);
}


//template <typename T>
//T poObject::getProperty(const std::string &prop, T def=T()) const {
//	
//}
//
//template <typename T>
//void poObject::setProperty(const std::string &prop, T value) {
//	
//}
