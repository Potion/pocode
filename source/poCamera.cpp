//
//  poCamera.cpp
//  poFramework4
//
//  Created by Jared Schiffman on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poCamera.h"
#include "poApplication.h"

// camera base class
poCamera::poCamera()
:	reset(true)
,	clears_background(true)
,	background_color(poColor::black)
{}

poCamera::poCamera(poColor color) 
:	reset(true)
,	clears_background(true)
,	background_color(color)
{}

void poCamera::setUp( poObject* obj ) {
	if(clears_background) {
        glClearColor(background_color.R, background_color.G, background_color.B, background_color.A);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	setProjection();
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	if(reset)
		glLoadIdentity();
	setModelview();

	saveAndUpdateGLSettings();
}

void poCamera::setDown( poObject* obj ) {
	restoreGLSettings();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

bool poCamera::clearsBackground() const {return clears_background;}
poCamera* poCamera::clearsBackground(bool b) {clears_background = b; return this;}

poColor poCamera::backgroundColor() const {return background_color;}
poCamera* poCamera::backgroundColor(poColor color) {background_color = color; return this;}
poCamera* poCamera::backgroundColor(float r, float g, float b, float a) {background_color.set(r,g,b,a); return this;}

bool poCamera::resetsModelview() const {return reset;}
poCamera* poCamera::resetsModelview(bool b) {reset = b; return this;}


// window camera
poCamera2D::poCamera2D() {}

poCamera2D::poCamera2D(poColor clear)
:	poCamera(clear)
{}

void poCamera2D::poCamera2D::setProjection() {
	gluOrtho2D(0, getWindowWidth(), getWindowHeight(), 0);
}

void poCamera2D::poCamera2D::saveAndUpdateGLSettings() {
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void poCamera2D::poCamera2D::restoreGLSettings() {
	glPopAttrib();
}


// orthographic camera
poOrthoCamera::poOrthoCamera(float w, float h, float n, float f)
:	x1(0)
,	y1(0)
,	x2(w)
,	y2(h)
,	near(n)
,	far(f)
{}

poOrthoCamera::poOrthoCamera(float x1, float y1, float x2, float y2, float n, float f)
:	x1(x1)
,	y1(y1)
,	x2(x2)
,	y2(y2)
,	near(n)
,	far(f)
{}

void poOrthoCamera::setProjection() {
	glOrtho(x1, x2, y2, y1, near, far);
}

poPerspectiveCamera::poPerspectiveCamera(float fov, float aspect, float near, float far)
:	fov(fov)
,	aspect(aspect)
,	near(near)
,	far(far)
{}

void poPerspectiveCamera::setUp(poObject *obj) {
	curr_pos = obj->position();
	poCamera::setUp(obj);
}

poPoint poPerspectiveCamera::lookAt() const {return look_at;}
poPerspectiveCamera *poPerspectiveCamera::lookAt(poPoint p) {look_at = p; return this;}
	
void poPerspectiveCamera::setProjection() {
	gluPerspective(fov, aspect, near, far);
}

void poPerspectiveCamera::setModelview() {
	gluLookAt(curr_pos.x, curr_pos.y, curr_pos.z, look_at.x, look_at.y, look_at.z, 0, 1, 0);
	glScalef(1,-1,1);
}


