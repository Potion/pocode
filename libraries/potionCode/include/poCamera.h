//
//  poCamera.h
//  poFramework4
//
//  Created by Jared Schiffman on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poObject.h"
#include "poColor.h"


// CLASS NOTES
//
// poCamera dervies from poObjectModifier. It is the parent class of poCamera2D,
// poCameraOrtho and poPerspectiveCamera.
//
// poCamera is responsible for clearing the background on every frame and for
// the color of the background.
//
// The virtual setProjection() method is the method responsible for setting
// the OpenGL projection matrix.
//

class poCamera : public poObjectModifier
{
public:
	poCamera();
	poCamera(poColor clear);
	virtual poCamera *copy() = 0;
	
    // BACKGROUND CLEARING and BACKGROUND COLOR
	bool                clearsBackground() const;
	poCamera*           clearsBackground(bool b);
	poColor             backgroundColor() const;
	poCamera*           backgroundColor(poColor color);
	poCamera*           backgroundColor(float r, float g, float b, float a=1.f);
	
    // RESET MODEL VIEW
    // Setting this to true means the model view matrix will be reset by the camera.
    // Setting this to false means the model view matrix will be inherited.
	bool                resetsModelview() const;
	poCamera*           resetsModelview(bool b);
	
    // FIXED VIEW SIZE
	bool                fixedSize() const;
	poCamera*           fixedSize(bool b, poPoint p=poPoint());
	
protected:
    // SETUP and SETDOWN
    // The camera projection matrix is setup (pushed) by doSetUp 
    // and setdown (popped) by doSetDown.
	virtual void        doSetUp( poObject* obj );
    virtual void        doSetDown( poObject* obj );

	virtual void        setProjection();
	virtual void        setModelview();
	virtual void        saveAndUpdateGLSettings();
	virtual void        restoreGLSettings();
	
	poPoint             fixed_size;
	bool                reset;

private:
    bool                clears_background;
    poColor             background_color;
	bool                is_fixed_size;
};

// CLASS NOTES
//
// poCamera2D implements a simple two-dimensional camera. All objects viewed by this camera
// should have Z values equal to 0.

class poCamera2D : public poCamera
{
public:
	poCamera2D();
    poCamera2D(poColor clear);
	virtual poCamera *copy();

protected:
	virtual void        setProjection();
};


// CLASS NOTES
//
// poOrthoCamera implements an orthographic camera. This is a 3D camera in which
// all perspective lines are parallel, as in SimCity.
//
// By default the "near" and "far" clipping planes are set to -1 and 1. These can
// be set to greater values in the expanded constructor or via the set() method.
//

class poOrthoCamera : public poCamera
{
public:
	poOrthoCamera();
    poOrthoCamera(float w, float h, float n, float f);
	poOrthoCamera(float x1, float y1, float x2, float y2, float n, float f);
	virtual poCamera *copy();

	void                set(poRect r, float n=-1, float f=1);
	void                set(float x1, float y1, float x2, float y2, float n, float f);
	poRect              get() const;
	
protected:
	virtual void        setProjection();

private:
	float               x1, y1, x2, y2, near, far;
};


// CLASS NOTES
//
// poPerspectiveCamera implements a traditional 3D camera. The camera's view is defined
// two points: the cameraPosition and the lookAtPosition. By default, these are
// both set to (0,0,0). At least one of these MUST be set in order for the camera
// to see anything. Assuming the objects being viewed are situated near the origin
// of the space, the camera may be set up as follows:
//
//      Cam = new poPerspectiveCamera(90, 1, 1000);
//      Cam->cameraPosition( poPoint(0,0,-100 ) );
//      Cam->lookAtPosition( poPoint(0,0,0 ) );
//      addModifier( Cam );
//
// Notice that the Z value of the cameraPosition is set to -100. This effectively pulls
// camera back from the scene so it can see the objects at the origin.

class poPerspectiveCamera : public poCamera
{
public:
	poPerspectiveCamera(float fov, float near, float far);
	virtual poCamera *copy();

    // CAMERA POSITION and LOOKAT POSITION
    poPoint                 cameraPosition() const;
    poPerspectiveCamera*    cameraPosition(poPoint p);
    
	poPoint                 lookAtPosition() const;
	poPerspectiveCamera*    lookAtPosition(poPoint p);
	
protected:
	virtual void            doSetUp(poObject* obj);
	virtual void            setProjection();
	virtual void            setModelview();

private:
	float                   fov, near, far;
	poPoint                 look_at_pos, camera_pos;
};



