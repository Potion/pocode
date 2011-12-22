/*	Created by Jonathan Bobrow on 12/22/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "perlin.h"
#include "poShapeBasics2D.h"

class PerlinNoiseApp : public poObject {
public:
	PerlinNoiseApp();
	virtual ~PerlinNoiseApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* r;
	poImage* img;
	Perlin* p;
};