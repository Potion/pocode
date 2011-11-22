#pragma once

/*	Created by Joshua Fisher on 11/18/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class poImageShape : public poObject
{
public:
	poImageShape();
	poImageShape(poImage *img);
	poImageShape(const std::string &str);
	virtual ~poImageShape();
	
	virtual void draw();
	
	bool doesAlphaTest() const;
	void setAlphaTest(bool b);
	
	poImage *getImage() const;
	void setImage(poImage* img);
	
	bool pointInside(poPoint p, bool localize=false);
	
private:
	poImage *img;
	poTexture *tex;
	bool alphaTest;
};