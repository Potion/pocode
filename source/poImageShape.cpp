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

#include "poImageShape.h"
#include "poSimpleDrawing.h"
#include "poCamera.h"

poImageShape::poImageShape()
:	tex(NULL)
,	alphaTest(false)
,   imageScale(1.0)
{}

poImageShape::poImageShape(poImage *i, bool keepImage)
:	tex(new poTexture(i, keepImage))
,	alphaTest(i->hasAlpha())
,   imageScale(1.0)
{
}

poImageShape::poImageShape(const std::string &str, bool keepImage) 
:	tex(NULL)
,	alphaTest(false)
,   imageScale(1.0)
{
    tex = poGetTexture( str, keepImage );
	alphaTest = (keepImage && tex->getSourceImage() && tex->getSourceImage()->hasAlpha());
}

poImageShape::~poImageShape() {
    if(!poResourceStore::get()->resourceIsCached(tex))
        delete tex;
}

void    poImageShape::reshapeToWidth( float W )
{
    imageScale = W / (float) tex->getWidth();
}

void    poImageShape::reshapeToHeight( float H )
{
    imageScale = H / (float) tex->getHeight();
}

void    poImageShape::reshapeToWidthAndHeight( float W, float H )
{
    imageScale = W / (float) tex->getWidth();
    if ( tex->getHeight()*imageScale > H )
        imageScale = H / (float) tex->getHeight();
}

void poImageShape::draw() {
    po::setColor(poColor::white, getAppliedAlpha());
    po::drawTexturedRect(tex, 0,0, tex->getWidth()*imageScale, tex->getHeight()*imageScale);
}

bool poImageShape::doesAlphaTest() const {
	return alphaTest;
}

void poImageShape::setAlphaTest(bool b) {
	alphaTest = b;
}

poImage *poImageShape::getImage() const {
	return tex->getSourceImage();
}

void poImageShape::setImage(poImage* i) {
	if(tex->getSourceImage()) {
		delete tex;
		
//		img = i;
		tex = new poTexture(i);
	}
}

bool poImageShape::pointInside(poPoint p, bool localize)
{	
    if(!visible)
		return false;
	
    // DO POINT INSIDE TEST FOR 2D
    if ( poCamera::getCurrentCameraType() == PO_CAMERA_2D )
    {
        if(localize) {
            p.y = getWindowHeight() - p.y;
            p = globalToLocal(p);
        }
        
        if(alphaTest) {
            // flip y value, since poImage y coordinates are reversed
            p.y = tex->getSourceImage()->getHeight()*imageScale - p.y;
            p /= imageScale;
            poColor pix = tex->getSourceImagePixel(p);
            return pix.A > 0.f;
        }
        
        poRect r(0,0,tex->getWidth()*imageScale,tex->getHeight()*imageScale);
        return r.contains(p.x, p.y);
    }
     
    // DO POINT INSIDE TEST FOR 3D
    if ( poCamera::getCurrentCameraType() == PO_CAMERA_3D )
    {
        if(localize) {
            p.y = getWindowHeight() - p.y;
        }
        
        /*if(alphaTest) {
            // flip y value, since poImage y coordinates are reversed
            p.y = tex->getSourceImage()->getHeight()*imageScale - p.y;
            p /= imageScale;
            poColor pix = tex->getSourceImagePixel(p);
            return pix.A > 0.f;
        }*/

        poRect r(0,0,tex->getWidth()*imageScale,tex->getHeight()*imageScale);
        return pointInRect3D( p, getMatrixSet(), r );
    }
    
    return false;
}

poRect  poImageShape::getBounds()
{
    poRect rect(0,0,tex->getWidth()*imageScale,tex->getHeight()*imageScale);
    return rect;
}

