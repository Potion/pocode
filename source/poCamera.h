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

class poCamera : public poObjectModifier
{
public:
	poCamera();
	poCamera(poColor clear);
	
	void setUp( poObject* obj );
    void setDown( poObject* obj );

	bool clearsBackground() const;
	poCamera *clearsBackground(bool b);
	
	poColor backgroundColor() const;
	poCamera *backgroundColor(poColor color);
	poCamera *backgroundColor(float r, float g, float b, float a=1.f);
	
	bool resetsModelview() const;
	poCamera *resetsModelview(bool b);
	
protected:
	virtual void setProjection() = 0;
	virtual void setModelview() {}
	virtual void saveAndUpdateGLSettings() {}
	virtual void restoreGLSettings() {}
	
private:
    bool		clears_background;
    poColor		background_color;
	bool		reset;
};

class poCamera2D : public poCamera
{
public:
	poCamera2D();
    poCamera2D(poColor clear);
	
protected:
	virtual void setProjection();
	virtual void saveAndUpdateGLSettings();
	virtual void restoreGLSettings();
};

class poOrthoCamera : public poCamera
{
public:
    poOrthoCamera(float w, float h, float n, float f);
	poOrthoCamera(float x1, float y1, float x2, float y2, float n, float f);

protected:
	virtual void setProjection();

private:
	float x1, y1, x2, y2, near, far;
};

class poPerspectiveCamera : public poCamera
{
public:
	poPerspectiveCamera(float fov, float aspect, float near, float far);

	virtual void setUp(poObject* obj);
	
	poPoint lookAt() const;
	poPerspectiveCamera *lookAt(poPoint p);
	
protected:
	virtual void setProjection();
	virtual void setModelview();

private:
	float fov, aspect, near, far;
	poPoint look_at, curr_pos;
};



