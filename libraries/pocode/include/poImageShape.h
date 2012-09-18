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

#pragma once


/*	Created by Joshua Fisher on 11/18/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "poApplication.h"
#include "poImage.h"
#include "poTexture.h"

// CLASS NOTES
//
// poImageShape is used for simple image loading and manipulation.
// it is basically a more specific version of poRectShape
// that has image-centric methods for convenience
//


class poImageShape : public poObject {
public:
    poImageShape();
    poImageShape(std::string url, bool loadAsync = false);
    poImageShape(poImage* image);
    
    poImage *getImage();
    
    void setImage(std::string url);
    void setImage(poImage *image);
    void setImageAsync(std::string url); //Load image asynchronously
    
    void clearImageData(); //Deletes the image data from the texture, lower memory cost
    
    void setShouldClearImage(bool shouldClearImage);
    bool getShouldClearImage();
    
private:
    bool shouldKeepImage;
    poTexture *tex;
    
//	poImageShape();
//	poImageShape(poImage *img, bool keepImage=false);
//	poImageShape(const std::string &str, bool keepImage=false);
//	virtual ~poImageShape();
//    
//	virtual void draw();
//	
//	bool doesAlphaTest() const;
//	void setAlphaTest(bool b);
//	
//	poImage *getImage() const;
//	void setImage(poImage* img);
//    
//	bool                    pointInside(poPoint p, bool localize=false);
//	virtual poRect          getBounds();
//private:
//	poTexture*  tex;
//	bool        alphaTest;
};