/*
 *  poObjectModifier.h
 *  poFramework3
 *
 *  Created by Joshua Fisher on 3/25/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#include <vector>

#ifndef _PO_OBJECT_MODIFIER_H_
#define _PO_OBJECT_MODIFIER_H_

#include "poSerializable.h"

class	poObject;
class	poObjectModifier;
typedef std::vector<poObjectModifier*> poObjectModifierVector;

class poObjectModifier : public poSerializable
{
public:
	poObjectModifier(){ enabled = true; };
	virtual ~poObjectModifier() { };
	
	virtual void				init();
	virtual void				initWithObject( poObject* obj ) { };
	virtual const char*			getClassName() { return "poObjectModifier"; };
	virtual poObjectModifier*	duplicate() { return new poObjectModifier(); };
	
	virtual void setup(poObject*) { };
	virtual void setdown(poObject*) { };
	
	virtual poXMLnode*		write();
	virtual void			read(poXMLnode* node);
	
	static poObjectModifier*		generateModifier( char* className );
	static poObjectModifierVector	registeredModifierSet;
	
	bool							enabled;
};


#endif
