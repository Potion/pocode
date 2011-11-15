/*	Created by filippo on 11/14/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class BasicShapesApp : public poObject {
public:
	BasicShapesApp();
	virtual ~BasicShapesApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poShape2D*      customShape;
    poRectShape*    rectShape;
    poRectShape*    rectShapeWithImage;
    poOvalShape*    ovalShape;
};