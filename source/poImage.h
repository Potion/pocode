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
//  Image.h
//  pocode
//
//  Created by Joshua Fisher on 4/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poShapeBasics2D.h"
#include "poRect.h"
#include "poEnums.h"
#include "poColor.h"
#include "poFileLocation.h"
#include "poThreadCenter.h"

#include <string>
#include <ostream>

struct FIBITMAP;

// CLASS NOTES
//
// Image is used to load and edit images. It can also be used to create images from scratch.
//
// You cannot construct a new Image directly. You can load a new image as follows:
//
//      Image* myImage = getImage("images/sample.jpg");
//
// Using getImage() ensures that the same image is not loaded and constructed multiple times.
//
// Image has no built-in methods for drawing. Instead, construct a RectShape as follows:
//
//      RectShape* S = new RectShape("images/sample.jpg");
//      addChild( S );
//
// This will load a Image, generate an OpenGL texture from the image, and attach the
// texture to the newly created RectShape. By default, the RectShape will be the
// native size of the image.
//

namespace po {
    class Image {
        friend std::ostream &operator<<(std::ostream &out, const Image &img);

    public:
        Image();
        Image(const FilePath &filePath);
        Image(const FilePath &filePath, uint numChannels);
        Image(uint w, uint h, uint numChannels, const ubyte *pixels);
        ~Image();
        
        static void getImageAsync(FilePath url, Object *notify);
        static void getImageAsync(poURL url, Object *notify, const FilePath &savePath=FilePath());

        Image*              copy();
        void				save(const FilePath &filePath);
        
        // IMAGE PROPERTIES
        bool				isValid() const;
        bool				hasAlpha() const;
        uint                getWidth() const;
        uint                getHeight() const;
        uint				getChannels() const;
        poPoint             getDimensions() const;
        uint                getPitch() const;
        uint                getStorageSize() const;
        bool                isScaled();
        
        // GET and SET PIXELS
        ubyte const*		getPixels() const;
        Color               getPixel(poPoint p) const;
        void                setPixel(poPoint p, Color c);
        void                setPixel(poPoint p, Color c, int stamp_width);
        
        // IMAGE OPERATIONS
        void                setNumChannels(uint numChannels);
        void                composite(Image *img, poPoint into, float blend);
        void                blur(int kernel_size, float sigma, int stepMultiplier=1);
        void                flip(poOrientation dir);
        void				fill(Color c);
        void                invert();
        void                clear();

        // IMAGE RESIZING
        void                resizeMaxDimension(float max_dim);
        void                resizeMinDimension(float min_dim);
        void                resizeWidth(float w);
        void                resizeHeight(float h);
        void                resize(float w, float h);
        
        // IMAGE URL, COULD BE "" IF NOTHING LOADED
        FilePath            getFilePath() const;
        
        static int          getTotalAllocatedImageMemorySize() { return totalAllocatedImageMemorySize; };
        
    private:
        void                load(const FilePath &filePath);
        void                load(const FilePath &filePath, uint numChannels);
        void                load(uint w, uint h, uint c, const ubyte *pix);
        
        FIBITMAP            *bitmap;
        bool                scaledBitmapFound;
        void                setFilePath(const FilePath &filePath);
        FilePath            filePath;
        
        static int          totalAllocatedImageMemorySize;
    };


    static const std::string ImageLoaderCompleteMessage    = "PO_IMAGE_LOADER_COMPLETE_MESSAGE";
    static const std::string ImageLoaderSuccessMessage     = "PO_IMAGE_LOADER_SUCCESS_MESSAGE";
    static const std::string ImageLoaderFailureMessage     = "PO_IMAGE_LOADER_FAILURE_MESSAGE";


    //------------------------------------------------------------------
    //ImageLoaderWorker
    class ImageLoaderWorker : public Worker {
    public:
        ImageLoaderWorker(FilePath filePath);
        ImageLoaderWorker(poURL url, const FilePath &savePath = FilePath("null"));
        virtual ~ImageLoaderWorker();
        
        void workerFunc();
    private:
        bool loadFromNetwork;
        poURL url;
        FilePath filePath;
    };
} /* End po Namespace */