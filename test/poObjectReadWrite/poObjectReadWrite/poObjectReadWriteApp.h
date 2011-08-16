/*	Created by Joshua Fisher on 8/15/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class poObjectReadWriteApp : public poObject {
public:
	poObjectReadWriteApp();
	virtual void eventHandler(poEvent *event);
};