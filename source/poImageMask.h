#pragma once

#include "poImage.h"
#include "poObjectModifier.h"

class poImageMask :	public poObjectModifier
{
public:
	poImageMask(poImage *img);
	poImageMask(const std::string &str);
	
protected:	
	virtual void doSetUp( poObject* obj );
	virtual void doSetDown( poObject* obj );
	
private:
	poImage *image;
};

