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
//  Image.cpp
//  pocode
//
//  Created by Joshua Fisher on 4/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poImage.h"
#include "poHelpers.h"
#include "poURLLoader.h"
#include "poApplication.h"
#include <FreeImage.h>

// -----------------------------------------------------------------------------------
// ========================== Static Async Loading Functions =========================


//------------------------------------------------------------------
void po::Image::getImageAsync(FilePath filePath, Object *notify) {
    ThreadCenter::addItem(new ImageLoaderWorker(filePath), notify);
}


//------------------------------------------------------------------
void po::Image::getImageAsync(URL url, Object *notify, const FilePath &savePath) {
    ThreadCenter::addItem(new ImageLoaderWorker(url, savePath), notify);
}

namespace po {
    
    static void loadFreeImageIfNeeded() {
        static bool free_image_loaded = false;
        if(!free_image_loaded) {
            FreeImage_Initialise();
            free_image_loaded = true;
        }
    }
    
    int Image::totalAllocatedImageMemorySize = 0;
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: Image =========================
    #pragma mark - Image -

    Image::Image()
    :	bitmap(NULL)
    ,   scaledBitmapFound(false)
    {}

    Image::Image(const FilePath &filePath)
    :	bitmap(NULL)
    ,   scaledBitmapFound(false)
    {
        load(filePath);
    }

    Image::Image(const FilePath &filePath, uint c)
    :	bitmap(NULL)
    ,   scaledBitmapFound(false)
    {
        load(filePath);
    }

    Image::Image(uint w, uint h, uint c, const ubyte *p) 
    :	bitmap(NULL)
    ,   scaledBitmapFound(false)
    {
        load(w, h, c, p);
    }

    Image::~Image() {
        totalAllocatedImageMemorySize -= FreeImage_GetDIBSize(bitmap);
        FreeImage_Unload(bitmap);
        bitmap = NULL;
    }
    
    
    //------------------------------------------------------------------
    Image* Image::copy() {
        Image* image = new Image;
        if(isValid()) {
            image->bitmap = FreeImage_Clone(bitmap);
            image->filePath = filePath;
        }
        return image;
    }
    
    
    //------------------------------------------------------------------
    void Image::save(FilePath &filePath) {
        if(isValid()) {
            FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filePath.toString().c_str());
            FreeImage_Save(fif, bitmap, filePath.toString().c_str());
        }
    }
    
    
    //------------------------------------------------------------------
    bool Image::isValid() const {
        return bitmap != NULL;
    }
    
    
    //------------------------------------------------------------------
    bool Image::hasAlpha() const {
        return FreeImage_GetColorType(bitmap) == FIC_RGBALPHA;
    }
    
    
    //------------------------------------------------------------------
    bool Image::isScaled() {
        return scaledBitmapFound;
    }
    
    
    //------------------------------------------------------------------
    uint Image::getWidth() const {
        return FreeImage_GetWidth(bitmap);
    }
    
    
    //------------------------------------------------------------------
    uint Image::getHeight() const {
        return FreeImage_GetHeight(bitmap);
    }
    
    
    //------------------------------------------------------------------
    uint Image::getChannels() const {
        return FreeImage_GetBPP(bitmap) / 8;
    }
    
    
    //------------------------------------------------------------------
    Point Image::getDimensions() const {
        return Point(getWidth(), getHeight());
    }
    
    
    //------------------------------------------------------------------
    uint Image::getPitch() const {
        return FreeImage_GetPitch(bitmap);
    }
    
    
    //------------------------------------------------------------------
    uint Image::getStorageSize() const {
        return getPitch() * getHeight();
    }
    
    
    //------------------------------------------------------------------
    ubyte const*Image::getPixels() const {
        return FreeImage_GetBits(bitmap);
    }
    
    
    //------------------------------------------------------------------
    Color Image::getPixel(Point p) const {
            
        if(!isValid() || p.x < 0 || p.y < 0 || p.x >= getWidth() || p.y >=getHeight())
            return Color();

        uint x = p.x;
        uint y = p.y;
        
        BYTE* bits = FreeImage_GetScanLine(bitmap, y);
        Color ret;
        
        switch(getChannels()) {
            case 1:
                ret.set255(bits[x], bits[x], bits[x], 255);
                break;
            case 2:
                ret.set255(bits[x*2], bits[x*2], bits[x*2], bits[x*2+1]);
                break;
            case 3:
                ret.set255(bits[x*3+0], bits[x*3+1], bits[x*3+2], 255);
                break;
            case 4:
                ret.set255(bits[x*4+0], bits[x*4+1], bits[x*4+2], bits[x*4+3]);
                break;
            default:
                ;
        }
        
        return ret;
    }
    
    
    //------------------------------------------------------------------
    void Image::setPixel(Point p, Color c) {
        if(p.x < 0 || p.y < 0 || p.x >= getWidth() || p.y >=getHeight())
            return;
        
        uint x = p.x;
        uint y = p.y;
        
        BYTE *bits = FreeImage_GetScanLine(bitmap, y);
        switch(getChannels()) {
            case 1:
                bits[x] = (((0.21*c.R) + (0.71*c.G) + (0.07*c.B)) * c.A) * 255;
                break;
            case 2:
                bits[x*2] = ((0.21*c.R) + (0.71*c.G) + (0.07*c.B)) * 255;
                bits[x*2+1] = c.A * 255;
                break;
            case 3:
                bits[x*3] = c.R * c.A * 255;
                bits[x*3+1] = c.G * c.A * 255;
                bits[x*3+2] = c.B * c.A * 255;
                break;
            case 4:
                bits[x*4] = c.R * 255;
                bits[x*4+1] = c.G * 255;
                bits[x*4+2] = c.B * 255;
                bits[x*4+3] = c.A * 255;
                break;
        }
    }
    
    
    //------------------------------------------------------------------
    void Image::setPixel(Point p, Color c, int stamp_width) {
        for(int y=-stamp_width/2; y<stamp_width/2; y++) {
            for(int x=-stamp_width/2; x<stamp_width/2; x++) {
                setPixel(p + Point(x,y), c);
            }
        }
    }
    
    
    //------------------------------------------------------------------
    void Image::setNumChannels(uint c) {
        if(isValid() && FreeImage_GetBPP(bitmap) != c*8) {
            FIBITMAP *dst = NULL;
            switch(c) {
                case 1:	
                    dst = FreeImage_ConvertTo8Bits(bitmap); 
                    break;
                case 2:	
                    dst = FreeImage_ConvertTo16Bits565(bitmap); 
                    break;
                case 3:	
                    dst = FreeImage_ConvertTo24Bits(bitmap); 
                    break;
                case 4:	
                    dst = FreeImage_ConvertTo32Bits(bitmap); 
                    break;
            }
            FreeImage_Unload(bitmap);
            bitmap = dst;
        }
    }
    
    
    //------------------------------------------------------------------
    void Image::composite(Image *img, Point into, float blend) {
        FreeImage_Paste(bitmap, img->bitmap, into.x, into.y, blend*256);
    }
    
    
    //------------------------------------------------------------------
    Color operator*(const Color& c, float f) {
        Color response(c);
        response.R *= f;
        response.G *= f;
        response.B *= f;
        response.A *= f;
        return response;
    }
    
    
    //------------------------------------------------------------------
    Color operator/(const Color& c, float f) {
        Color response(c);
        response.R /= f;
        response.G /= f;
        response.B /= f;
        response.A /= f;
        return response;
    }
    
    
    //------------------------------------------------------------------
    Color operator+(const Color &a, const Color &b) {
        Color response(a);
        response.R = a.R + b.R;
        response.G = a.G + b.G;
        response.B = a.B + b.B;
        response.A = a.A + b.A;
        return response;
    }
    
    
    //------------------------------------------------------------------
    void Image::blur(int kernel_size, float sig, int stepMultiplier) {
        float e = 2.718281828f;
        int hk = kernel_size / 2;
        
        float *kernel = new float[kernel_size];
        for(int i=0; i<kernel_size; i++) {
            float contrib = powf(i - hk, 2) / (2 * sig*sig);
            kernel[i] = powf(e, -contrib);
        }
        
        Image* tmp = copy();
        int width = getWidth();
        int height = getHeight();
        
        for(int y=0; y<height; y++) {
            for(int x=0; x<width; x++) {
                Color sum;
                float k_tot = 0;
                
                for(int i=-hk; i<hk; i++) {
                    int stp = i * stepMultiplier;
                    if(x+stp < 0 || x+stp >= width)
                        continue;
                    
                    float k = kernel[i+hk];
                    k_tot += k;
                    
                    Color c = getPixel(Point(x+stp,y));
                    
                    sum = sum + (c*k);
                }
                
                tmp->setPixel(Point(x,y), sum/k_tot);
            }
        }
        
        for(int y=0; y<height; y++) {
            for(int x=0; x<width; x++) {
                Color sum;
                float k_tot = 0;
                
                for(int i=-hk; i<hk; i++) {
                    int stp = i * stepMultiplier;
                    if(y+stp < 0 || y+stp >= height)
                        continue;
                    
                    float k = kernel[i+hk];
                    k_tot += k;
                    
                    Color c = tmp->getPixel(Point(x,y+stp));
                    
                    sum = sum + (c*k);
                }
                
                setPixel(Point(x,y), sum/k_tot);
            }
        }
        
        delete tmp;
        delete [] kernel;
    }
    
    
    //------------------------------------------------------------------
    void Image::flip(Orientation dir) {
        if(dir == po::ORIENTATION_VERTICAL)
            FreeImage_FlipVertical(bitmap);
        else
            FreeImage_FlipHorizontal(bitmap);
    }
    
    
    //------------------------------------------------------------------
    void Image::invert() {
        FreeImage_Invert(bitmap);
    }
    
    
    //------------------------------------------------------------------
    void Image::fill(Color c) {
        unsigned char color[4] = {c.R*255, c.G*255, c.B*255, c.A*255};
        FreeImage_FillBackground(bitmap, color);
    }
    
    
    //------------------------------------------------------------------
    void Image::resizeMaxDimension(float max_dim) {
        int newW = getWidth();
        int newH = getHeight();
        
        if(newW > newH)
            resizeWidth(max_dim);
        else
            resizeHeight(max_dim);
    }
    
    
    //------------------------------------------------------------------
    void Image::resizeMinDimension(float min_dim) {
        int newW = getWidth();
        int newH = getHeight();
        
        if(newW < newH)
            resizeWidth(min_dim);
        else
            resizeHeight(min_dim);
    }
    
    
    //------------------------------------------------------------------
    void Image::resizeWidth(float w) {
        float aspect = getHeight() / (float)getWidth();
        resize(w, w * aspect);
    }
    
    
    //------------------------------------------------------------------
    void Image::resizeHeight(float h) {
        float aspect = getWidth() / (float)getHeight();
        resize(h * aspect, h);
    }
    
    
    //------------------------------------------------------------------
    void Image::resize(float w, float h) {
        FIBITMAP *img = FreeImage_Rescale(bitmap, w, h, FILTER_CATMULLROM);

        FreeImage_Unload(bitmap);
        bitmap = img;
    }
    
    
    //------------------------------------------------------------------
    FilePath Image::getFilePath() const {
        return filePath;
    }
    
    
    //------------------------------------------------------------------
    void Image::clear() {
        ubyte color[] = {0,0,0,0};
        FreeImage_FillBackground(bitmap, &color[0]);
    }
    
    
    //------------------------------------------------------------------
    FIBITMAP *loadDIB(const FilePath &filePath) {
        // figure out if the file exists
        if(!filePath.exists()) {
            printf("Image: image file not found (%s)\n", filePath.toString().c_str());
            return NULL;
        }
        
        loadFreeImageIfNeeded();

        FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
        fif = FreeImage_GetFileType(filePath.toString().c_str());
    //	if(fif == FIF_UNKNOWN)
    //		fif = FreeImage_GetFIFFromFilename(url.c_str());
        if(fif == FIF_UNKNOWN) {
            printf("Image: image isn't a supported file type (%s)\n", filePath.toString().c_str());
            return NULL;
        }
        
        FIBITMAP *dib = NULL;
        if(FreeImage_FIFSupportsReading(fif))
            dib = FreeImage_Load(fif, filePath.toString().c_str());
        if(!dib) {
            printf("Image: image file not found (%s)\n", filePath.toString().c_str());
            return NULL;
        }

        unsigned bpp = FreeImage_GetBPP(dib);
        if(bpp == 24 || bpp == 32) {
            // there has got to be a more efficient way of doing this
            FIBITMAP *red = FreeImage_GetChannel(dib,FICC_RED);
            FIBITMAP *blue = FreeImage_GetChannel(dib,FICC_BLUE);
            
            FreeImage_SetChannel(dib,red,FICC_BLUE);
            FreeImage_SetChannel(dib,blue,FICC_RED);
            
            FreeImage_Unload(red);
            FreeImage_Unload(blue);
        }
        
        return dib;
    }
    
    
    //------------------------------------------------------------------
    void Image::load(const FilePath &filePath) {
        this->setFilePath(filePath);
        
        FIBITMAP *bmp = loadDIB(this->filePath);
        if(bmp) {
            bitmap      = bmp;
            totalAllocatedImageMemorySize += FreeImage_GetDIBSize(bitmap);
        }
    }
    
    
    //------------------------------------------------------------------
    void Image::load(const FilePath &filePath, uint c) {
        this->setFilePath(filePath);
        
        FIBITMAP *bmp = loadDIB(this->filePath);
        if(bmp) {
            bitmap = bmp;
            this->setFilePath(filePath);
            setNumChannels(c);
            totalAllocatedImageMemorySize += FreeImage_GetDIBSize(bitmap);
        }
    }
    
    
    //------------------------------------------------------------------
    void Image::load(uint w, uint h, uint c, const ubyte *pix) {
        if(bitmap)
        {
            totalAllocatedImageMemorySize -= FreeImage_GetDIBSize(bitmap);
            FreeImage_Unload(bitmap);
        }
        
        if(pix != NULL)
            bitmap = FreeImage_ConvertFromRawBits(const_cast<ubyte*>(pix), w, h, w*c, c*8, 0,0,0);
        else {
            bitmap = FreeImage_Allocate(w, h, c*8);
            unsigned char black[] = {0,0,0,0};
            FreeImage_FillBackground(bitmap, black, FI_COLOR_IS_RGBA_COLOR);
        }
        
        totalAllocatedImageMemorySize += FreeImage_GetDIBSize(bitmap);
    }
    
    
    //------------------------------------------------------------------
    void Image::setFilePath(const FilePath &filePath) {
        //See if we need a different scale
        if(po::getScale() == 1.0f) {
            this->filePath = filePath;
            scaledBitmapFound = false;
        } else {
            FilePath scaledPath = filePath.getScaled(po::getScale());
            
            if(scaledPath.toString() != filePath.toString()) {
                this->filePath = scaledPath;
                scaledBitmapFound = true;
            } else {
                this->filePath = filePath;
                scaledBitmapFound = false;
            }
        }
    }
    
    
    //------------------------------------------------------------------
    std::ostream &operator<<(std::ostream &out, const Image *img) {
        return out << "image('" << img->getFilePath().toString() << "')";
    }

    //void writeImageToCHeader(const std::string &str, Image *img) {
    //	std::string filename = str + "Image.cpp";
    //	FILE *f = fopen(filename.c_str(), "w+");
    //	fprintf(f,
    //			"\n#include \"%sImage.h\"\n\n"
    //			"static const uint width = %d;\n"
    //			"static const uint height = %d;\n"
    //			"static const uint format = %d;\n"
    //			"static const ubyte bytes[] = {\n",
    //			str.c_str(), img->width(), img->height(), (int)img->bpp());
    //	for(int h=0; h<img->height(); h++) {
    //		fprintf(f,"\t");
    //		for(int w=0; w<img->width(); w++) {
    //			Color color = img->getPixel(Point(w,h));
    //			fprintf(f,"0x%X,0x%X,0x%X,", uint(color.R*255), uint(color.G*255), uint(color.B*255));
    //		}
    //		if(h == img->height()-1)
    //			fseek(f, -1, SEEK_CUR);
    //		fprintf(f,"\n");
    //	}
    //	fprintf(f,
    //			"};\n"
    //			"Image *get%sImage() {\n"
    //			"	static Image *img = NULL;\n"
    //			"	if(!img) {\n"
    //			"		img = new Image(width,height,(ImageBitDepth)format,&bytes[0]);\n"
    //			"	}\n"
    //			"	return img->copy();\n"
    //			"}\n\n",
    //			str.c_str());
    //	fclose(f);
    //	
    //	filename = str + "Image.h";
    //	f = fopen(filename.c_str(), "w");
    //	fprintf(f,
    //			"\n#pragma once\n\n"
    //			"#include \"Image.h\"\n\n"
    //			"Image *get%sImage();\n",
    //			str.c_str());
    //	fclose(f);
    //}
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: ImageLoaderWorker =========================
    #pragma mark - ImageLoaderWorker -

    ImageLoaderWorker::ImageLoaderWorker(FilePath filePath)
    : loadFromNetwork(false)
    , filePath(filePath)
    {}

    ImageLoaderWorker::ImageLoaderWorker(URL url, const FilePath &savePath)
    : url(url)
    , loadFromNetwork(true)
    , filePath(savePath)
    {}

    ImageLoaderWorker::~ImageLoaderWorker() {
    }
    
    
    //------------------------------------------------------------------
    void ImageLoaderWorker::workerFunc() {
        //First check if we need to pull this image
        //from a remote url
        //If so, do it!
        if(loadFromNetwork) {
            filePath = URLLoader::getFile(url, filePath);
        }
        
        //Try to load the image (should be local now regardless)
        std::string status;
        
        //Load image, check for error
        Image*    image = new Image(filePath);
        if(!image->isValid()) {
            delete image; image = NULL;
            status = ImageLoaderFailureMessage;
        } else {
            status = ImageLoaderSuccessMessage;
        }
        
        workerMessage = ImageLoaderCompleteMessage;
        
        //Set Dictionary
        dict.set("status", status);
        dict.set("image", image);
        dict.set("url", url.toString());
        dict.set("filePath", filePath.toString());
    }
} /* End po namespace */