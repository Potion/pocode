/*
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 */

#pragma once

#include "poCommon.h"

class	poObject;
class	poModifier;
typedef std::vector<poModifier*> poModifierVec;

class poModifier {
public:
	poModifier();
	virtual ~poModifier();
	
	virtual void	setup(poObject*) { };
	virtual void	setdown(poObject*) { };
	
	std::string		typeName;
	bool			enabled;
};

