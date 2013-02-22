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

#include "poRect.h"
#include "poPoint.h"
#include "poTexture.h"
#include "poBinPacker.h"

namespace po {
    class TextureAtlas {
    public:
        TextureAtlas(GLenum f, uint w, uint h);
        TextureAtlas(TextureConfig config, uint w, uint h);
        ~TextureAtlas();
        
        // pass in the id you want to have associated with the image
        void            addImage(Image *img, uint requested_id);
        // remove all images that are current in there
        void            clearImages();

        // will redo all the bin packing with the current set of images
        void            layoutAtlas();
        
        bool            hasUID(uint uid);
        int             getNumPages();
        Point         getDimensions() const;
        
        uint            getPageForUID(uint uid);
        Rect          getCoordsForUID(uint uid);
        Rect          getSizeForUID(uint uid);
        Texture*		getTextureForPage(uint pg);
        Texture*		getTextureForUID(uint uid);
        
        // draws will shift the texture as needed,
        // tho user can look at the pages for what it wants to draw and
        // organize it so there are minimal texture switches
        // size should be between 0..1, will scale
        void            drawUID(uint uid, Rect r, float scale);
        // this one will draw the subtex at its native size
        void            drawUID(uint uid, Point p, float scale);
        
        
        // dump the pack to screen for debugging
        void			debugDraw();
        
    private:
        struct ImageLookup {
            uint page;
            Rect coords;
            Point size;
        };
        
        void clearPages();
        void clearTextures();
        
        int width, height;
        std::vector<ImageLookup> coords;
        
        // the items to be atlased
        std::vector<Image*> images;
        
        // users control the ids
        std::vector<uint> requestedIDs;
        
        // the pages of the atlas
        std::vector<Image*> pages;
        
        // the textures of the atlas
        std::vector<Texture*> textures;
        std::map<uint,uint> uids;
        
        // this is the configuration we want for the atlas
        TextureConfig config;
        
        BinPacker packer;
    };
}/* End po Namespace */