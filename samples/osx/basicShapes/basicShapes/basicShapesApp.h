/*	Created by Jared Schiffman on 6/12/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class basicShapesApp : public poObject {
public:
	basicShapesApp();
	virtual ~basicShapesApp();

	void draw();
	
    poShape2D*      customShape;
    poRectShape*    rectShape;
    poRectShape*    rectShapeWithImage;
    poOvalShape*    ovalShape;
};