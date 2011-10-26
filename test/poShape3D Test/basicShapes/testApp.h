/*	Created by Jared Schiffman on 6/12/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poShape3DBasics.h"

class  testApp : public poObject {
public:
	 testApp();
	virtual ~ testApp();
    
    poShape3D*     myShape3D;
	
    virtual void update();
    virtual void eventHandler(poEvent *event);
};