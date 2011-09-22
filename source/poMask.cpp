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
#include "poOpenGLState.h"
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

void poGeometryMask::set() {
    if(shape) {
		if(clears_stencil)
			glClear(GL_STENCIL_BUFFER_BIT);
        
		poOpenGLState::get()->matrix.pushModelview();
        applyObjTransform(shape);
    
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		
		state.func = GL_ALWAYS;
		state.func_ref = 1;
		state.func_mask = 1;
		state.op_fail = GL_KEEP;
		state.op_stencil_depth_fail = GL_KEEP;
		state.op_stencil_depth_pass = GL_REPLACE;
		poStencilState::set();
		
        drawPoints(GL_TRIANGLE_FAN, shape->getPoints());
        
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		state.func = GL_EQUAL;
		poStencilState::revert();
		poStencilState::set();
        
        poOpenGLState::get()->matrix.popModelview();
	}
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
	
	glEnable(GL_BLEND);
	
	glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ZERO);
	drawRect(image->texture());
	
	glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
}

void poImageMask::doSetDown( poObject* obj ) {
}




