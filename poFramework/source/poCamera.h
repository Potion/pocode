#ifndef __poCamera_h__
#define __poCamera_h__

#include "poObjectModifier.h"

class poCamera : public poObjectModifier
{
public:
	poCamera();
	
	virtual const char*			getClassName() { return "poCamera"; };
	virtual poObjectModifier*	duplicate() { return new poCamera(); };
	
	virtual void setup(poObject*) {}
	virtual void setdown(poObject*) {}
	
	float	rotation;
	poPoint rotationAxis;
	poPoint position;
	float	scale;
	poPoint	scale2D;

protected:
};

class poCamera2D : public poCamera
{
public:
	virtual const char*			getClassName() { return "poCamera2D"; };
	virtual poObjectModifier*	duplicate() { return new poCamera2D(); };
	
	virtual void setup(poObject*);
	virtual void setdown(poObject*);
};

class poCamera3D : public poCamera
{
public:
	poCamera3D();
	poCamera3D(float fov, float nearPlane, float farPlane);
	
	virtual const char*			getClassName() { return "poCamera3D"; };
	virtual poObjectModifier*	duplicate() { return new poCamera3D(); };
	
	virtual void setup(poObject*);
	virtual void setdown(poObject*);
	
	void lookat(float x, float y, float z);
	void lookat(const poPoint& p);
	
	float fov, nearPlane, farPlane;
	poPoint up;
	poPoint forward;
	poPoint position;
	poPoint lookAtPoint;
};

#endif