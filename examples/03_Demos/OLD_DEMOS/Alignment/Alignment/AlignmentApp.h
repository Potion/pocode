/*	Created by Jonathan Bobrow on 6/10/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class AlignmentApp : public poObject {
public:
	AlignmentApp();
	virtual ~AlignmentApp();
    virtual void eventHandler(poEvent* event);
};