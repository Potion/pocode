/*
 *  poObject.h
 *  potionApp
 *
 *  Created by Jared Schiffman on 2/27/97. 
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 *
 */

#include "poCommon.h"

#include "poSerializable.h"
#include "poResourceStore.h"
#include "poObjectProperties.h"
#include "poObjectModifier.h"
#include "poEvent.h"


#ifndef _PO_OBJECT_H_
#define _PO_OBJECT_H_

class poObject;
typedef std::vector<poObject*> poObjectVector;
typedef std::list<poObject*> poObjectList;
typedef std::map<std::string, poObject*> poObjectMap;

class poObject : public poSerializable
{
public:
							poObject();
							poObject(const char* n);
	virtual					~poObject();
	
	// REQUIRED SUBCLASS METHODS
	virtual poObject*		duplicate(bool copyChildren=true, poObject* copyInto=NULL);
	virtual const char*		getClassName() { return "poObject"; };
	
							poObject(const poObject& rhs);
	poObject&				operator=(const poObject& rhs);
	
	virtual void			init();
	virtual void			draw();
	virtual void			debugDraw();
	virtual void			update() {}

	// PROPERTY ACCESS
	const char*				getName() const { return name.c_str(); }
	void					setName(const char* n);
	poObjectProperties		getProperties();
	void					setProperties( const poObjectProperties& p );
	
	// ALL THINGS DISPLAY LIST
	inline int				numChildren() const { return children.size(); }
	virtual void			addChild(poObject* child);
	bool					insertChild(poObject* child, int index);
	poObject*				removeChild(poObject* child);
	poObject*				removeChildAt(int index);
	poObject*				getChild(int index);
	poObject*				getChildByName(const char* name);
	int						getChildIndex(poObject* child);
	poObject*				getParent() {return parent;}
	
	// STACK ON MORE MODIFIERS
	void					addModifier(poObjectModifier* mod, int idx=0);
	void					removeModifier(int idx);
	void					removeModifier(poObjectModifier* mod);
	poObjectModifier*		getModifier( char* modType );

	// DEAL WITH OUR BOUNDS
	virtual void			calculateBounds();
	virtual void			calculateRelativeBounds( poObject* relativeObj, poBoundingBox& bbox );
	poBoundingBox			getBounds() { return boundingBox; }; 
	inline float			getWidth() const				{ return boundingBox.right - boundingBox.left; }
	inline float			getHeight() const				{ return boundingBox.top - boundingBox.bottom; }
	virtual void			setOrigin(poHoriOrientation ho, poVertOrientation vo);

	// DEAL WITH THE WORLD
	virtual bool			pointInside(float x, float y);
	void					globalToLocal(poPoint& p) const;
	void					globalToLocal(float &x, float &y, float &z) const;
	void					localToGlobal(poPoint& p) const;
	void					localToGlobal(float &x, float &y, float &z) const;

	// EVENT HANDLING JOY
	poEvent*				addEvent(int type, poObject* handler);
	poEvent*				addEvent(int type, poObject* handler, const char* message, const poDictionary& dict=poDictionary());
	bool					hasEvents(int type);
	bool					hasEvent(int type, poObject* handler);
	void					removeEvent(int type, poObject* handler);
	void					dumpEventTable();
	virtual bool			eventHandler(poEvent* event) {return false;}
	virtual void			messageHandler(const char* message, const poDictionary& data = poDictionary() ) { if( parent != NULL ) parent->messageHandler(message,data); }
	
	// SERIALIZATION
	virtual poXMLnode*		write();
	virtual void			read(poXMLnode* node);
	static poObject*		generateObject( char* className );
	
	poPoint					position;
	float					alpha;
	poPoint					scale;

	float					rotation;
	poPoint					rotationAxis;

	bool					visible;
	bool					transformChildren;
    
	poMatrixOrder			matrixOrder;
	poObjectModifierVector	modifiers;
	
	bool					mouseCurrentlyInside;

//protected:

	// RECURSIVE (TREE) FUNCTIONS
	virtual void			initTree();
	virtual void			drawTree();
	virtual void			updateTree();
	virtual bool			processEventTree(poEvent* event);
	virtual bool			processEvent(poEvent* event);


	poBoundingBox			boundingBox;
    poMatrixSet				matrixSet;

	void					pushObjectMatrix();
	void					popObjectMatrix();

	poObject*				parent;

	static poObjectVector	registeredClassSet;
	static float			masterAlphaControl;
	static int				masterAlphaStackHeight;
	static float			masterAlphaStack[256];
	static bool				doDebugDraw;
	static bool				masterDisableMatrixCapture;
	
	void					construct();

	std::string				name;

	poObjectVector			children;
	poEventTable			events;
	

};

#endif


