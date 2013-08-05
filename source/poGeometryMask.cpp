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

//
//  poMaskModifier.cpp
//  pocode
//
//  Created by Joshua Fisher on 6/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poGeometryMask.h"
#include "poHelpers.h"
#include "poSimpleDrawing.h"
#include "poOpenGLState.h"

namespace po {
    // -----------------------------------------------------------------------------------
    // ================================ Class: Geometry Mask ============================
    #pragma mark - Geometry Mask -

    GeometryMask::GeometryMask(Shape2D *shape, bool clearsStencil, bool inverse)
    :	shape(shape)
    ,	clearsStencil(clearsStencil)
    ,   inverse(inverse)
    {}

    GeometryMask::~GeometryMask() {
        if(shape) {
            delete shape;
            shape = NULL;
        }
    }


    //------------------------------------------------------------------------
    ObjectModifier *GeometryMask::copy() {
        GeometryMask *obj = new GeometryMask((Shape2D*)shape->copy(), clearsStencil);
        return obj;
    }


    //------------------------------------------------------------------------
    void GeometryMask::setShape(Shape2D *s) {
        if(shape)
            delete shape;
        shape = s;
        setEnabled(shape != NULL);
    }
    
    
    //------------------------------------------------------------------------
    bool GeometryMask::pointInside(Point p) {
        if(shape) {
			Rect r = shape->getBounds();
			r.setPosition(shape->position);
            return r.contains(p);
		}
        return false;
    }
    
    
    //------------------------------------------------------------------------
    void GeometryMask::doSetUp(Object *obj) {
        if(shape) {
            po::setupStencilMask(clearsStencil);
            po::saveModelview();
    //		po::saveModelviewThenIdentity();
            shape->applyTransformation();
            po::drawPoints(shape->getPoints(), GL_TRIANGLE_FAN);
            po::restoreModelview();
            po::useStencilMask(inverse);
        }
    }
    
    
    //------------------------------------------------------------------------
    void GeometryMask::doSetDown(Object *obj) {
        if(shape) {
            po::disableStencil();
        }
    }
} /*End po namespace */

/*
// ============ ImageMask =============== //

ImageMask::ImageMask(Image *img)
:	image(img)
{}

ImageMask::ImageMask(const std::string &str)
{
	image = getImage(str);
}

bool ImageMask::pointInside(Point p) {
	
}

void ImageMask::doSetUp( Object* obj ) {
	my_obj = obj;

	state.enabled = true;
	state.separate = true;
	state.source_factor = GL_ONE;
	state.dest_factor = GL_ZERO;
	state.source_alpha_factor = GL_SRC_COLOR;
	state.dest_alpha_factor = GL_ZERO;
	save();
	
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
	po::drawRect(image->texture());
	
	restore();
	state.separate = false;
	state.source_factor = GL_DST_ALPHA;
	state.dest_factor = GL_ONE_MINUS_DST_ALPHA;
	save();
	
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void ImageMask::doSetDown( Object* obj ) {
	restore();
}
*/



