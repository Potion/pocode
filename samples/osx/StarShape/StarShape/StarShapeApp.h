/*	Created by Jonathan Bobrow on 7/25/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class StarShapeApp : public poObject {
public:
	StarShapeApp();
	virtual ~StarShapeApp();
    virtual void draw();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());

    int numPoints;
    float radius, depth;
    poStarShape* s; 
};