//
//  poMaskModifier.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poGeometryMask.h"
#include "Helpers.h"
#include "SimpleDrawing.h"
#include "poOpenGLState.h"

// ============ poGeometryMask =============== //

poGeometryMask::poGeometryMask(poShape2D *shape, bool clearsStencil)
:	shape(shape)
,	clears_stencil(clearsStencil)
{}

poGeometryMask::~poGeometryMask() {
	if(shape) {
		delete shape;
		shape = NULL;
	}
}

poObjectModifier *poGeometryMask::copy() {
	poGeometryMask *obj = new poGeometryMask((poShape2D*)shape->copy(), clears_stencil);
	return obj;
}

void poGeometryMask::setShape(poShape2D *s) {
	if(shape)
		delete shape;
	shape = s;
	enabled(shape != NULL);
}

bool poGeometryMask::pointInside(poPoint p) {
	if(shape)
		return shape->pointInside(p);
}

void poGeometryMask::doSetUp(poObject *obj) {
    if(shape) {
		if(clears_stencil)
			glClear(GL_STENCIL_BUFFER_BIT);

        poOpenGLState *ogl = poOpenGLState::get();
		
		ogl->pushStencilState();

		po::StencilState state;
		state.enabled = true;
		state.func = GL_ALWAYS;
		state.func_ref = 1;
		state.func_mask = 1;
		state.op_fail = GL_KEEP;
		state.op_stencil_depth_fail = GL_KEEP;
		state.op_stencil_depth_pass = GL_REPLACE;
		ogl->setStencil(state);
		
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		ogl->matrix.pushModelview();
        applyObjTransform(shape);
		po::drawPoints(shape->getPoints(), GL_TRIANGLE_FAN);
		ogl->matrix.popModelview();
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		state.func = GL_EQUAL;
		ogl->setStencil(state);
	}
}

void poGeometryMask::doSetDown(poObject *obj) {
	if(shape != NULL) {
		poOpenGLState *ogl = poOpenGLState::get();
		ogl->popStencilState();
	}
}

/*
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

	state.enabled = true;
	state.separate = true;
	state.source_factor = GL_ONE;
	state.dest_factor = GL_ZERO;
	state.source_alpha_factor = GL_SRC_COLOR;
	state.dest_alpha_factor = GL_ZERO;
	save();
	
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
	po::drawRect(image->texture());
	
	restore();
	state.separate = false;
	state.source_factor = GL_DST_ALPHA;
	state.dest_factor = GL_ONE_MINUS_DST_ALPHA;
	save();
	
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void poImageMask::doSetDown( poObject* obj ) {
	restore();
}
*/



