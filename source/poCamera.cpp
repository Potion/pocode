//
//  poCamera.cpp
//  poFramework4
//
//  Created by Jared Schiffman on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poCamera.h"
#include "poApplication.h"
#include "poOpenGLState.h"
#include <glm/gtc/matrix_transform.hpp>

cameraType   poCamera::currentCameraType = PO_CAMERA_NONE;

// camera base class
poCamera::poCamera()
:	reset(true)
,	clearsBG(true)
,	backgroundColor(poColor::transparent)
,	isSizeFixed(false)
{}

poCamera::poCamera(poColor color) 
:	reset(true)
,	clearsBG(true)
,	backgroundColor(color)
,	isSizeFixed(false)
{}

poObjectModifier *poCamera::copy() {
	poCamera *cam = new poCamera();
	clone(cam);
	return cam;
}

void poCamera::clone(poCamera *cam) {
	cam->fixedSize = fixedSize;
	cam->reset = reset;
	cam->clearsBG = clearsBG;
	cam->backgroundColor = backgroundColor;
	cam->isSizeFixed = isSizeFixed;
}

void poCamera::doSetUp( poObject* obj ) {
	saveAndUpdateGLSettings();

	poMatrixStack *stack = &poOpenGLState::get()->matrix;
	
	if(isFixedSize())
		stack->pushViewport(poRect(poPoint(),fixedSize));
	else {
		stack->pushViewport(poRect(poPoint(),getWindowDimensions()));
	}

	if(clearsBG) {
        glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
	
	setProjection();
	setModelview();
    
    stack->setCameraMatrix(stack->getModelview());
}

void poCamera::doSetDown( poObject* obj ) {
	restoreGLSettings();
	
	poMatrixStack *stack = &poOpenGLState::get()->matrix;
	stack->popProjection();
	stack->popModelview();
	stack->popViewport();
}

void poCamera::setProjection() {
	poOpenGLState::get()->matrix.pushProjection(glm::mat4());
}

void poCamera::setModelview() {
	poOpenGLState::get()->matrix.pushModelview(glm::mat4());
}

void poCamera::saveAndUpdateGLSettings() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void poCamera::restoreGLSettings() {
}

bool poCamera::clearsBackground() const {
	return clearsBG;
}

poCamera* poCamera::setClearsBackground(bool b) {
	clearsBG = b; return this;
}

poColor poCamera::getBackgroundColor() const {
	return backgroundColor;
}

poCamera* poCamera::setBackgroundColor(poColor color) {
	backgroundColor = color;
	return this;
}

poCamera* poCamera::setBackgroundColor(float r, float g, float b, float a) {
	backgroundColor.set(r,g,b,a);
	return this;
}

bool poCamera::resetsModelview() const {
	return reset;
}

poCamera* poCamera::setResetsModelview(bool b) {
	reset = b;
	return this;
}

bool poCamera::isFixedSize() const {
	return isSizeFixed;
}

poCamera* poCamera::setFixedSize(bool b, poPoint p) {
	isSizeFixed=b;
	fixedSize=p;
	return this;
}


// window camera
poCamera2D::poCamera2D() {}

poCamera2D::poCamera2D(poColor clear)
:	poCamera(clear)
{}

poObjectModifier *poCamera2D::copy() {
	poCamera2D *cam = new poCamera2D();
	clone(cam);
	return cam;
}

void poCamera2D::clone(poCamera2D *cam) {
	poCamera::clone(cam);
}

void poCamera2D::setProjection() {
	poMatrixStack *stack = &poOpenGLState::get()->matrix;
	poRect viewp = stack->getViewport();
	stack->pushProjection(glm::ortho(viewp.x, viewp.width + viewp.x, viewp.height + viewp.y, viewp.y));
    
    poCamera::currentCameraType = PO_CAMERA_2D;
}

// orthographic camera
poOrthoCamera::poOrthoCamera()
:	x1(-1)
,	y1(-1)
,	x2(1)
,	y2(1)
,	nearClip(-1)
,	farClip(1)
{}

poOrthoCamera::poOrthoCamera(float w, float h, float n, float f)
:	x1(0)
,	y1(0)
,	x2(w)
,	y2(h)
,	nearClip(n)
,	farClip(f)
{}

poOrthoCamera::poOrthoCamera(float x1, float y1, float x2, float y2, float n, float f)
:	x1(x1)
,	y1(y1)
,	x2(x2)
,	y2(y2)
,	nearClip(n)
,	farClip(f)
{}

poObjectModifier *poOrthoCamera::copy() {
	poOrthoCamera *cam = new poOrthoCamera();
	clone(cam);
	return cam;
}

void poOrthoCamera::clone(poOrthoCamera *cam) {
	cam->x1 = x1;
	cam->y1 = y1;
	cam->x2 = x2;
	cam->y2 = y2;
	cam->nearClip = nearClip;
	cam->farClip = farClip;
	poCamera::clone(cam);
}

void poOrthoCamera::set(poRect r, float n, float f) {
	set(r.x, r.y, r.x + r.width, r.y + r.height, n, f);
}

void poOrthoCamera::set(float x1, float y1, float x2, float y2, float n, float f) {
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
	nearClip = n;
	farClip = f;
}

poRect poOrthoCamera::get() const {
	return poRect(x1, y1, x2-x1, y2-y1);
}

void poOrthoCamera::setProjection() {
	poOpenGLState::get()->matrix.pushProjection(glm::ortho(x1,x2,y2,y1,nearClip,farClip));
    poCamera::currentCameraType = PO_CAMERA_2D;
}



poPerspectiveCamera::poPerspectiveCamera(float fov, float nearClip, float farClip)
:	fov(fov)
,	nearClip(nearClip)
,	farClip(farClip)
{}

poObjectModifier *poPerspectiveCamera::copy() {
	poPerspectiveCamera *cam = new poPerspectiveCamera(fov, nearClip, farClip);
	clone(cam);
	return cam;
}

void poPerspectiveCamera::clone(poPerspectiveCamera *cam) {
	cam->lookAtPos = lookAtPos;
	cam->cameraPos = cameraPos;
	poCamera::clone(cam);
}

poPoint poPerspectiveCamera::lookAtPosition() const { 
	return lookAtPos;
}

poPerspectiveCamera *poPerspectiveCamera::lookAtPosition(poPoint p) {
	lookAtPos = p;
	return this;
}

void    poPerspectiveCamera::setupFor2DOnZPlane()
{
    float half_fov = deg2rad(fov/2.0); 
    float half_window_height = getWindowHeight() / 2.0;
    float new_z = half_window_height / tan(half_fov); 
    
    cameraPos.set( 0,0,-new_z );
    lookAtPos.set( 0,0,0 );
      
    nearClip = 0.10;
    farClip = new_z*10.0;
}
 

poPoint poPerspectiveCamera::cameraPosition() const  {
	return cameraPos;
}

poPerspectiveCamera* poPerspectiveCamera::cameraPosition(poPoint p) {
	cameraPos = p; return this;
}

void poPerspectiveCamera::doSetUp(poObject *obj) {
	poCamera::doSetUp(obj);
}

void poPerspectiveCamera::setProjection() {
	poMatrixStack *stack = &poOpenGLState::get()->matrix;
    float aspect = getWindowAspect();
	stack->pushProjection(glm::perspective(fov, aspect, nearClip, farClip));
    poCamera::currentCameraType = PO_CAMERA_3D;
}

void poPerspectiveCamera::setModelview() {
	using namespace glm;
	
	poMatrixStack *stack = &poOpenGLState::get()->matrix;
	
	vec3 eye(cameraPos.x, cameraPos.y, cameraPos.z);
	vec3 center(lookAtPos.x, lookAtPos.y, lookAtPos.z);
	vec3 up(0,1,0);
	stack->pushModelview(glm::lookAt(eye,center,up));
	stack->scale(poPoint(-1,-1,1));
}



