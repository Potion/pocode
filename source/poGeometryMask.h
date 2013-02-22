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


//#include "Image.h"
#include "poShape2D.h"

// CLASS NOTES
//
// GeometryMask implements a mask based upon a Shape2D object. A geometry mask is used as follows:
//
//      RectShape* objectToMask = new RectShape("myImage.jpg");
//      addChild( objectToMask );
//
//      poOvalShape* maskShape = new poOvalShape( 100,100 );
//      maskShape->position( objectToMask->bounds.width()/2, objectToMask->bounds.height()/2, 0 );
//
//      GeometryMask* geoMask = new GeometryMask( maskShape );
//      objectToMask->addModifer( geoMask );
//
// Note that the maskShape can have a position. It can also be scaled and rotated.
// The origin of the objectToMask serves as the origin of the maskShape.
// The maskShape should NOT be added to the scene graph.
//

namespace po {
    class GeometryMask : public ObjectModifier {
    public:
        GeometryMask(Shape2D *shape, bool clearsStencil=false, bool inverse=false);
        virtual			~GeometryMask();
        
        virtual			ObjectModifier *copy();
        
        void            setShape(Shape2D *shape);	
        // this isn't implemented yet
        bool            pointInside(Point p);
        
    protected:
        void			doSetUp(Object*);
        void			doSetDown(Object*);
        
    private:
        Shape2D*		shape;
        bool			clearsStencil;
        bool            inverse;
    };
}

/////////////////////////////////////////////////////
//
//	ImageMask is UNDER DEVELOPMENT
//	Check again in future releases of pocode
//
/////////////////////////////////////////////////////

/*
// CLASS NOTES
//
// ImageMask implements a mask based upon a Image object. A geometry mask is used as follows:
//
//      RectShape* objectToMask = new RectShape("myImage.jpg");
//      addChild( objectToMask );
//
//      ImageMask* imageMask = new ImageMask( getImage("myMask.png") );
//      objectToMask->addModifer( imageMask );
//
// The mask image should be a single channel grayscale image. The black areas in the mask
// is where the masked image will be visible.

class ImageMask :	public poBlendState
{
public:
	ImageMask(Image *img);
	ImageMask(const std::string &str);
	
	bool            pointInside(poPoint p);
	
protected:	
	virtual void    doSetUp( Object* obj );
	virtual void    doSetDown( Object* obj );
	
private:
	Object*       my_obj;
	Image*        image;
};
*/
