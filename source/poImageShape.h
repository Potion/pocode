#pragma once


/*	Created by Joshua Fisher on 11/18/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "poApplication.h"

class poImageShape : public poObject
{
public:
	poImageShape();
	poImageShape(poImage *img, bool keepImage=false);
	poImageShape(const std::string &str, bool keepImage=false);
	virtual ~poImageShape();
	
    void    reshapeToWidth( float W );
    void    reshapeToHeight( float H );
    void    reshapeToWidthAndHeight( float W, float H );
    
	virtual void draw();
	
	bool doesAlphaTest() const;
	void setAlphaTest(bool b);
	
	poImage *getImage() const;
	void setImage(poImage* img);
	
    
	bool                    pointInside(poPoint p, bool localize=false);
	virtual poRect          getBounds();
    
private:
	poTexture*  tex;
	bool        alphaTest;
    float       imageScale;
};