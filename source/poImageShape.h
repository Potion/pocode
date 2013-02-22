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
#include "poShapeBasics2D.h"
#include "poImage.h"
#include "poTexture.h"

// CLASS NOTES
//
// ImageShape is used for simple image loading and manipulation.
// it is basically a more specific version of RectShape
// that has image-centric methods for convenience
//

namespace po {
    class ImageShape : public Object {
    public:
        ImageShape();
        ImageShape(float width, float height);
        ImageShape(const FilePath filePath, bool async = false, float width=0, float height=0);
        ImageShape(const URL url, bool async = false, float width=0, float height=0);
        
        ~ImageShape();
        
        //Load from FilePath
        void load(const FilePath filePath, bool shouldAutoFit = true);
        void loadAsync(const FilePath filePath, bool shouldAutoFit = true);
        
        //Load from URL
        void load(const URL url, bool shouldAutoFit = true);
        void loadAsync(const URL url, bool shouldAutoFit = true);
        
    private:
        po::RectShape *imageRect;
        Texture *tex;
        
    //	ImageShape();
    //	ImageShape(Image *img, bool keepImage=false);
    //	ImageShape(const std::string &str, bool keepImage=false);
    //	virtual ~ImageShape();
    //    
    //	virtual void draw();
    //	
    //	bool doesAlphaTest() const;
    //	void setAlphaTest(bool b);
    //	
    //	Image *getImage() const;
    //	void setImage(Image* img);
    //    
    //	bool                    pointInside(poPoint p, bool localize=false);
    //	virtual Rect          getBounds();
    //private:
    //	Texture*  tex;
    //	bool        alphaTest;
    };
} /* End po Namespace */