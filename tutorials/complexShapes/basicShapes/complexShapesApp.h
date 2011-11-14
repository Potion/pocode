/*	Created by Jared Schiffman on 6/12/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"


class complexShapesApp : public poObject {
public:
	complexShapesApp();
	virtual ~complexShapesApp();
    
    void eventHandler(poEvent *event);
    std::vector <poShape2D*> SVGShapes;
};