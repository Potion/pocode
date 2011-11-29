/*	Created by Jared Schiffman on 11/27/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poShapeBasics3D.h"
#include "poImageShape.h"

class pointInside3DTestApp : public poObject {
public:
	pointInside3DTestApp();
	virtual ~pointInside3DTestApp();
	
    virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());

    poShape2D*      myShape;
    poImageShape*   imageShape;
    poShape3D*      shape3D;
};