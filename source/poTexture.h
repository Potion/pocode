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
//  Texture.h
//  pocode
//
//  Created by Joshua Fisher on 4/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poPoint.h"
#include "poColor.h"
#include "poRect.h"
#include "poEnums.h"
#include "poImage.h"

#include "poResourceStore.h"

namespace po {
    
    // This utlity class contains the settings for a Texture.
    class TextureConfig {
    public:
        TextureConfig();
        TextureConfig(GLenum format);
        
        TextureConfig &setFormat(GLenum f)              { format = f; return *this; }
        TextureConfig &setInternalFormat(GLenum f)      { internalFormat = f; return *this; }
        TextureConfig &setType(GLenum f)				{ type = f; return *this; }
        TextureConfig &setMinFilter(GLenum f)			{ minFilter = f; return *this; }
        TextureConfig &setMagFilter(GLenum f)			{ magFilter = f; return *this; }
        TextureConfig &setWrapS(GLenum f)				{ wrapS = f; return *this; }
        TextureConfig &setWrapT(GLenum f)				{ wrapT = f; return *this; }
        
        GLenum format, internalFormat, type, minFilter, magFilter, wrapS, wrapT;
    };

    // CLASS NOTES
    //
    // A Texture is a type of image that can be attached to poShape2D objects.
    //
    // It is rare that you will neeed to construct a Texture directly. Instead, Texture's
    // are usually derived from Image objects.
    //
    // Texture's are also used in frame buffer objects (FBO's) and in video display.
    //
    
    class Texture : public Resource {
    public:
        Texture();
        Texture(const FilePath &filePath, bool keepImage=false );
        Texture(Image* img);
        Texture(Image* img, const TextureConfig &config);
        Texture(uint width, uint height, const ubyte *pixels, const TextureConfig &config, uint stride=0);
        ~Texture();
        
        Texture*			copy();
        
        void				replace(Image* image);
        void				replace(const ubyte *pixels);
        
        bool                isValid() const;
        bool                isScaled() const;
        TextureConfig		getConfig() const;
        uint				getUid() const;
        uint				getWidth() const;
        uint				getHeight() const;
        uint				getChannels() const;
        uint				getBitsPerPixel() const;
        size_t				getSizeInBytes() const;
        Point				getDimensions() const;
        Rect				getBounds() const;
        
        void				setFormat(GLenum f);
        void				setInternalFormat(GLenum f);
        void				setMagFilter(GLenum f);
        void				setMinFilter(GLenum f);
        void				setType(GLenum f);
        void				setWrapS(GLenum f);
        void				setWrapT(GLenum f);
        
        bool                hasSourceImage() { return (sourceImage != NULL); };
        Color               getSourceImagePixel(Point p);
        Image*              getSourceImage() { return sourceImage; };
        
        static int          getTotalAllocatedTextureMemorySize() { return totalAllocatedTextureMemorySize; };
        bool				hasAlpha() { return channels > 0 && channels != 3; }
        
    private:
        void                load(Image* img);
        void                load(Image* img, const TextureConfig &config);
        void				load(uint width, uint height, int channels, const ubyte *pixels, uint stride=0);
        void				load(uint width, uint height, const ubyte *pixels, const TextureConfig &config);
        void				loadDummyImage();
        void                unload();
        void				configure();
        
        TextureConfig       config;
        uint				uid, width, height, channels;
        
        Image*              sourceImage;
        bool                sourceIsScaled;
        
        static int          totalAllocatedTextureMemorySize;
    };

    // figures out tex coords to fit texture in rect
    std::vector<Point> textureFit(Rect rect, Texture *tex, TextureFitOption fit, Alignment align);
    // these do the same but make coordinates for each point in points array
    // returns texture coordinates thru coords
    void textureFit(Rect rect, Texture *tex, TextureFitOption fit, Alignment align, std::vector<Point> &coords, const std::vector<Point> &points);
    uint channelsForFormat(GLenum format);
}/* End po namespace */
