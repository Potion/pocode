//
//  poMaskModifier.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poMask.h"
#include "Helpers.h"
#include "SimpleDrawing.h"
#include "poResourceStore.h"

// ============ poGeometryMask =============== //

poGeometryMask::poGeometryMask(poShape2D *shape, bool clearsStencil)
:	shape(shape)
,	clears_stencil(clearsStencil)
{}

poGeometryMask::~poGeometryMask() {
	if(shape)
		delete shape;
}

void poGeometryMask::setShape(poShape2D *s) {
	if(shape)
		delete shape;
	shape = s;
	enabled(shape != NULL);
}

bool poGeometryMask::pointInside(poPoint p) {
	
}

void poGeometryMask::doSetUp( poObject* obj ) {
	
    if(shape) {
		if(clears_stencil)
			glClear(GL_STENCIL_BUFFER_BIT);
        
		poMatrixStack::get()->pushModelview();
        applyObjTransform(shape);
        
        glPushAttrib(GL_STENCIL_BUFFER_BIT);
        glEnable(GL_STENCIL_TEST);
    
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glStencilFunc(GL_ALWAYS, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        drawPoints(GL_TRIANGLE_FAN, shape->getPoints());
        
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilFunc(GL_EQUAL, 1, 1);
        
        poMatrixStack::get()->popModelview();
	}
}

void poGeometryMask::doSetDown( poObject* obj ) {
	if(shape)
		glPopAttrib();
}


// ============ poImageMask =============== //

poImageMask::poImageMask(poImage *img)
:	image(img)
{}

poImageMask::poImageMask(const std::string &str)
{
	image = getImage(str);
}

bool poImageMask::pointInside(poPoint p) {
	
}

void poImageMask::doSetUp( poObject* obj ) {
	my_obj = obj;
	
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	
//	glClear(GL_STENCIL_BUFFER_BIT);
	
	glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ZERO);
	drawRect(image->texture());
	
	glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
}

void poImageMask::doSetDown( poObject* obj ) {
	glPopAttrib();
}




