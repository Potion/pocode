/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  poCamera.cpp
//  pocode
//
//  Created by Jared Schiffman on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poCamera.h"
#include "poApplication.h"
#include "poOpenGLState.h"
//#include <glm/gtc/matrix_transform.hpp>

poCameraType   poCamera::currentCameraType = PO_CAMERA_NONE;

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

//	if(isFixedSize())
//		po::setViewport(0, 0, fixedSize.x, fixedSize.y);
//	else {
//		poPoint win = getWindowDimensions() * poGetScale();
//		po::setViewport(0, 0, win.x, win.y);
//	}
	
	po::setCamera(po::modelview());
	
    glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
    
    GLenum clear = 0;
    if(clearsBG)
        clear |= GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
    if(glIsEnabled(GL_DEPTH_TEST))
       clear |= GL_DEPTH_BUFFER_BIT;
    
    if(clear != 0)
        glClear(clear);
//       
//	if(clearsBG) {
//        glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
//        if ( glIsEnabled( GL_DEPTH_TEST ) )
//            glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//        else
//            glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
//    }
	
	po::saveProjection();
	
	setProjection();
	setModelview();
}

void poCamera::doSetDown( poObject* obj ) {
	restoreGLSettings();
	po::restoreProjection();
}

void poCamera::setProjection() {
}

void poCamera::setModelview() {
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
	float scale = po::getScale();
	
	if(isFixedSize())
		po::setOrthoProjection(0, fixedSize.x / scale, fixedSize.y / scale, 0);
	else {
		float w = po::getWindowWidth();
		float h = po::getWindowHeight();
		po::setOrthoProjection(0, w / scale, h / scale, 0);
	}
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
	po::setOrthoProjection(x1, x2, y2, y1, nearClip, farClip);
    poCamera::currentCameraType = PO_CAMERA_2D;
}


// perspective camera
poPerspectiveCamera::poPerspectiveCamera(float fov, float nearClip, float farClip)
:	fov(fov)
,	nearClip(nearClip)
,	farClip(farClip)
,   doDepthTesting(true)
,   doBackfaceCulling(true)
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

void poPerspectiveCamera::setupFor2DOnZPlane()
{
    float half_fov = deg2rad(fov/2.0); 
    float half_window_height = po::getWindowHeight() / 2.0;
    float new_z = half_window_height / tan(half_fov); 
    
    cameraPos.set( 0,0,-new_z );
    lookAtPos.set( 0,0,0 );
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
    float aspect = po::getWindowAspect();
	po::setPerpsective(fov, aspect, nearClip, farClip);
    poCamera::currentCameraType = PO_CAMERA_3D;
}

void poPerspectiveCamera::setModelview() {
	po::lookAt(cameraPos, lookAtPos, poPoint(0,1,0));
	po::scale(poPoint(-1,-1,1));
}

void poPerspectiveCamera::saveAndUpdateGLSettings()
{
	if(doDepthTesting) glEnable(GL_DEPTH_TEST);
	if(doBackfaceCulling) glEnable(GL_CULL_FACE);
}


