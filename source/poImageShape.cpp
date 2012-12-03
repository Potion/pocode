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
#include "poOpenGLState.h"

namespace po {
    ImageShape::ImageShape()
    : tex(NULL)
    {};

    ImageShape::ImageShape(std::string url, bool shouldLoadAsync)
    : tex(NULL)
    {
        
    };

    ImageShape::ImageShape(Image *image)
    : tex(new Texture(image, true))
    {};


    //ImageShape::ImageShape()
    //:	tex(NULL)
    //,	alphaTest(false)
    //,   imageScale(1.0)
    //{}
    //
    //ImageShape::ImageShape(Image *i, bool keepImage)
    //:	tex(new Texture(i, keepImage))
    //,	alphaTest(i->hasAlpha())
    //,   imageScale(1.0)
    //{
    //}
    //
    //ImageShape::ImageShape(const std::string &str, bool keepImage) 
    //:	tex(NULL)
    //,	alphaTest(false)
    //,   imageScale(1.0)
    //{
    //    tex = poGetTexture( str, keepImage );
    //	alphaTest = (keepImage && tex->getSourceImage() && tex->getSourceImage()->hasAlpha());
    //}
    //
    //ImageShape::~ImageShape() {
    //    if(!poResourceStore::get()->resourceIsCached(tex))
    //        delete tex;
    //}
    //
    //void ImageShape::draw() {
    //    po::setColor(poColor::white, getAppliedAlpha());
    //    po::drawTexturedRect(tex, 0,0, tex->getWidth()*imageScale, tex->getHeight()*imageScale);
    //}
    //
    //bool ImageShape::doesAlphaTest() const {
    //	return alphaTest;
    //}
    //
    //void ImageShape::setAlphaTest(bool b) {
    //	alphaTest = b;
    //}
    //
    //Image *ImageShape::getImage() const {
    //	return tex->getSourceImage();
    //}
    //
    //void ImageShape::setImage(Image* i) {
    //	if(tex) {
    //        
    //		delete tex;
    //		
    ////		img = i;
    //		tex = new Texture(i);
    //	}
    //}
    //
    //bool ImageShape::pointInside(poPoint p, bool localize)
    //{	
    //    if(!visible || !tex->getSourceImage() || !tex)
    //		return false;
    //	
    //    // DO POINT INSIDE TEST FOR 2D
    //    if ( poCamera::getCurrentCameraType() == po::CAMERA_2D )
    //    {
    //        if(localize) {
    //            p.y = getWindowHeight() - p.y;
    //            p = globalToLocal(p);
    //        }
    //        
    //        if(alphaTest) {
    //            // flip y value, since Image y coordinates are reversed
    //            p.y = tex->getSourceImage()->getHeight()*imageScale - p.y;
    //            p /= imageScale;
    //            poColor pix = tex->getSourceImagePixel(p);
    //            return pix.A > 0.f;
    //        }
    //        
    //        poRect r(0,0,tex->getWidth()*imageScale,tex->getHeight()*imageScale);
    //        return r.contains(p.x, p.y);
    //    }
    //     
    //    // DO POINT INSIDE TEST FOR 3D
    //    if ( poCamera::getCurrentCameraType() == po::CAMERA_3D )
    //    {
    //        if(localize) {
    //            p.y = getWindowHeight() - p.y;
    //        }
    //        
    //        /*if(alphaTest) {
    //            // flip y value, since Image y coordinates are reversed
    //            p.y = tex->getSourceImage()->getHeight()*imageScale - p.y;
    //            p /= imageScale;
    //            poColor pix = tex->getSourceImagePixel(p);
    //            return pix.A > 0.f;
    //        }*/
    //
    //        poRect r(0,0,tex->getWidth()*imageScale,tex->getHeight()*imageScale);
    //        return pointInRect3D( p, getMatrixSet(), r );
    //    }
    //    
    //    return false;
    //}
    //
    //poRect  ImageShape::getBounds()
    //{
    //    poRect rect(0,0,tex->getWidth()*imageScale,tex->getHeight()*imageScale);
    //    return rect;
    //}
    //
}
