/*	Created by Joshua Fisher on 8/15/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "poDictionary.h"

class Dictionary_TestApp : public poObject {
public:
	Dictionary_TestApp();
	virtual void eventHandler(poEvent *event);

	void reload();
	
	poDictionary dict;
};