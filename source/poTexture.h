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
//  poTexture.h
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

// This utlity class contains the settings for a poTexture.
class poTextureConfig {
public:
	poTextureConfig();
	poTextureConfig(GLenum format);
	
	poTextureConfig &setFormat(GLenum f)			{ format = f; return *this; }
	poTextureConfig &setInternalFormat(GLenum f)	{ internalFormat = f; return *this; }
	poTextureConfig &setType(GLenum f)				{ type = f; return *this; }
	poTextureConfig &setMinFilter(GLenum f)			{ minFilter = f; return *this; }
	poTextureConfig &setMagFilter(GLenum f)			{ magFilter = f; return *this; }
	poTextureConfig &setWrapS(GLenum f)				{ wrapS = f; return *this; }
	poTextureConfig &setWrapT(GLenum f)				{ wrapT = f; return *this; }
	
	GLenum format, internalFormat, type, minFilter, magFilter, wrapS, wrapT;
};


// CLASS NOTES
//
// A poTexture is a type of image that can be attached to poShape2D objects.
//
// It is rare that you will neeed to construct a poTexture directly. Instead, poTexture's
// are usually derived from poImage objects.
//
// poTexture's are also used in frame buffer objects (FBO's) and in video display.
//

class poTexture : public poResource {
public:
	poTexture();
	poTexture(const poFilePath &filePath, bool keepImage=false );
	poTexture(poImage* img);
	poTexture(poImage* img, const poTextureConfig &config);
	poTexture(uint width, uint height, const ubyte *pixels, const poTextureConfig &config, uint stride=0);
	~poTexture();
	
	poTexture*			copy();
    
	void				replace(poImage* image);
	void				replace(const ubyte *pixels);
	
	bool                isValid() const;
    bool                isScaled() const;
	poTextureConfig		getConfig() const;
	uint				getUid() const;
	uint				getWidth() const;
	uint				getHeight() const;
	uint				getChannels() const;
	uint				getBitsPerPixel() const;
	size_t				getSizeInBytes() const;
	poPoint				getDimensions() const;
	poRect				getBounds() const;
	
	void				setFormat(GLenum f);
	void				setInternalFormat(GLenum f);
	void				setMagFilter(GLenum f);
	void				setMinFilter(GLenum f);
	void				setType(GLenum f);
	void				setWrapS(GLenum f);
	void				setWrapT(GLenum f);
    
    bool                hasSourceImage() { return (sourceImage != NULL); };
	poColor             getSourceImagePixel(poPoint p);
    poImage*            getSourceImage() { return sourceImage; };
    
    static int          getTotalAllocatedTextureMemorySize() { return totalAllocatedTextureMemorySize; };
	bool				hasAlpha() { return channels > 0 && channels != 3; }
    
private:
	void                load(poImage* img);
	void                load(poImage* img, const poTextureConfig &config);
	void				load(uint width, uint height, int channels, const ubyte *pixels, uint stride=0);
	void				load(uint width, uint height, const ubyte *pixels, const poTextureConfig &config, uint stride=0);
	void				loadDummyImage();
	void                unload();
	void				configure();
	
	poTextureConfig     config;
	uint				uid, width, height, channels;
	
    poImage*            sourceImage;
    bool                sourceIsScaled;
    
    static int          totalAllocatedTextureMemorySize;
};


// figures out tex coords to fit texture in rect
std::vector<poPoint> textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align);
// these do the same but make coordinates for each point in points array
// returns texture coordinates thru coords
void textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);
uint channelsForFormat(GLenum format);

