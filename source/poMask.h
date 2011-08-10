#pragma once

#include "poObjectModifier.h"

#include "poImage.h"
#include "poShape2D.h"

#include "poObjectModifier.h"


#pragma mark - poMask
class poMask : public poObjectModifier {
	virtual bool pointInside(poPoint p) {return false;}
};


#pragma mark - poGeometryMask
class poGeometryMask : public poMask
{
public:
	// you are responsible for the object lifetime of this shape
	poGeometryMask(poShape2D *shape, bool clearsStencil=false);
	virtual ~poGeometryMask();
	
	void setShape(poShape2D *shape);
	
	bool pointInside(poPoint p);
	
protected:
	virtual void doSetUp( poObject* obj );
	virtual void doSetDown( poObject* obj );
	
private:
	poShape2D *shape;
	bool clears_stencil;
};


#pragma mark - poImageMask
class poImageMask :	public poObjectModifier
{
public:
	poImageMask(poImage *img);
	poImageMask(const std::string &str);
	
	bool pointInside(poPoint p);
	
protected:	
	virtual void doSetUp( poObject* obj );
	virtual void doSetDown( poObject* obj );
	
private:
	poObject *my_obj;
	poImage *image;
};

