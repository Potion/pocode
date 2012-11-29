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
#include "poApplication.h"
#include "poTextureAtlas.h"
#include "poSimpleDrawing.h"
#include "poImage.h"
#include "poHelpers.h"
#include "poOpenGLState.h"

#include <boost/foreach.hpp>

namespace po {
    TextureAtlas::TextureAtlas(GLenum f, uint w, uint h)
    :	width(w)
    ,	height(h)
    ,	config(f)
    ,	packer(w,h,4)
    {}

    TextureAtlas::TextureAtlas(TextureConfig c, uint w, uint h)
    :	width(w)
    ,	height(h)
    ,	config(c)
    ,	packer(w,h,4)
    {}

    TextureAtlas::~TextureAtlas() {
        clearTextures();
        clearPages();
        clearImages();
    }
    
    
    //------------------------------------------------------------------------
    void TextureAtlas::addImage(Image* img, uint request) {
        requestedIDs.push_back(request);

        Image* copy = img->copy();
        copy->setNumChannels(channelsForFormat(config.format));
        images.push_back(copy);
    }
    
    
    //------------------------------------------------------------------------
    bool TextureAtlas::hasUID(uint uid) {
        return uids.find(uid) != uids.end();
    }
    
    
    //------------------------------------------------------------------------
    int TextureAtlas::getNumPages() {
        return textures.size();
    }
    
    
    //------------------------------------------------------------------------
    Point TextureAtlas::getDimensions() const {
        return Point(width, height);
    }
    
    
    //------------------------------------------------------------------------
    uint TextureAtlas::getPageForUID(uint uid) {
        return coords[uids[uid]].page;
    }
    
    
    //------------------------------------------------------------------------
    Rect TextureAtlas::getCoordsForUID(uint uid) {
        return coords[uids[uid]].coords;
    }
    
    
    //------------------------------------------------------------------------
    Rect TextureAtlas::getSizeForUID(uint uid) {
        return Rect(Point(0,0), coords[uids[uid]].size);
    }
    
    
    //------------------------------------------------------------------------
    Texture* TextureAtlas::getTextureForPage(uint page) {
        return textures[page];
    }
    
    
    //------------------------------------------------------------------------
    Texture* TextureAtlas::getTextureForUID(uint uid) {
        return textures[getPageForUID(uid)];
    }
    
    
    //------------------------------------------------------------------------
    void TextureAtlas::drawUID(uint uid, Rect rect, float scale) {
        if(hasUID(uid)) {
            uint page = getPageForUID(uid);
            
            Rect size = getSizeForUID(uid);
            Rect placement(rect.x, 
                             rect.y,
                             rect.width  * size.width,
                             rect.height * size.height);
            
            placement.width    /= scale;
            placement.height   /= scale;
            
            Rect coords = getCoordsForUID(uid);
            po::drawTexturedRect(getTextureForPage(page), placement, coords);
        }
    }
    
    
    //------------------------------------------------------------------------
    void TextureAtlas::drawUID(uint uid, Point p, float scale) {
        drawUID(uid, Rect(p.x,p.y,1,1), scale);
    }
    
    
    //------------------------------------------------------------------------
    void TextureAtlas::debugDraw() {
        packer.render();
    }
    
    
    //------------------------------------------------------------------------
    void TextureAtlas::clearTextures() {
        BOOST_FOREACH(Texture *tex, textures)
            delete tex;
        textures.clear();
        coords.clear();
        uids.clear();
    }
    
    
    //------------------------------------------------------------------------
    void TextureAtlas::clearPages() {
        BOOST_FOREACH(Image *img, pages)
            delete img;
        pages.clear();
    }
    
    
    //------------------------------------------------------------------------
    void TextureAtlas::clearImages() {
        BOOST_FOREACH(Image *img, images)
            delete img;
        images.clear();
        requestedIDs.clear();
    }
        
        
    //------------------------------------------------------------------------
    void TextureAtlas::layoutAtlas() {
        clearTextures();

        packer.reset();
        
        for(uint i=0; i<images.size(); i++) {
            uids[requestedIDs[i]] = packer.addRect(Rect(0,0,images[i]->getWidth(),images[i]->getHeight()));
        }
        packer.pack();
        
        for(int i=0; i<packer.getNumPages(); i++)
            pages.push_back(new Image(width,height,channelsForFormat(config.format),NULL));
        
        coords.resize(images.size());
        
        for(uint i=0; i<images.size(); i++) {
            uint pg;
            Rect pack_loc = packer.packPosition(i, &pg);
            
            uint uid = requestedIDs[i];
            
            ImageLookup item;
            item.page = pg;
            
            Point size(pack_loc.width / (float)width,
                         pack_loc.height / (float)height);
            
            Point origin(pack_loc.x / (float)width,
                           pack_loc.y / (float)height);
            
            item.coords = Rect(origin, size);
            item.coords.y = 1.f - item.coords.y - item.coords.height;
            
            item.size = pack_loc.getSize();
            
            coords[i] = item;
            pages[pg]->composite(images[i], pack_loc.getPosition(), 1.f);
        }
        
        textures.resize(pages.size());
        
        for(uint i=0; i<pages.size(); i++) {
            Image* img = pages[i];
            textures[i] = new Texture(img,config);
        }

        clearPages();
    }
} /*End po Namespace*/