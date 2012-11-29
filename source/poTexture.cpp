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
//  Texture.cpp
//  pocode
//
//  Created by Joshua Fisher on 4/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poTexture.h"
#include "poImage.h"
#include "poHelpers.h"
#include "poOpenGLState.h"
#include "poGLBuffer.h"
#include "poApplication.h"

namespace po {
    //------------------------------------------------------------------------
    GLenum formatForChannels(uint channels) {
        switch(channels) {
            case 1:
                return GL_LUMINANCE;
            case 2:
                return GL_LUMINANCE_ALPHA;
            case 3:
                return GL_RGB;
            case 4:
                return GL_RGBA;
            default:
                return 0;
        }
        return GL_LUMINANCE;
    }
    
    
    //------------------------------------------------------------------------
    uint channelsForFormat(GLenum format) {
        switch(format) {
            #ifndef OPENGL_ES
            case GL_COLOR_INDEX:
            case GL_RED:
            case GL_GREEN:
            case GL_BLUE:
            #endif
            case GL_ALPHA:
            case GL_LUMINANCE:
                return 1;
            case GL_LUMINANCE_ALPHA:
                return 2;
            case GL_RGB:
            #ifndef OPENGL_ES
            case GL_BGR:
            #endif
                return 3;
            case GL_RGBA:
            case GL_BGRA:
                return 4;
        }
        return 1;
    }
    
    
    //------------------------------------------------------------------------
    uint bitsPerChannelForType(GLenum type) {
        switch(type) {
            case GL_BYTE:
            case GL_UNSIGNED_BYTE:
                
            #ifndef OPENGL_ES
            case GL_BITMAP:
            case GL_UNSIGNED_BYTE_3_3_2:
            case GL_UNSIGNED_BYTE_2_3_3_REV:
            #endif
                return 8;
            case GL_SHORT:
            case GL_UNSIGNED_SHORT:
            case GL_UNSIGNED_SHORT_5_6_5:
            case GL_UNSIGNED_SHORT_4_4_4_4:
            case GL_UNSIGNED_SHORT_4_4_4_4_REV:
            case GL_UNSIGNED_SHORT_5_5_5_1:
            case GL_UNSIGNED_SHORT_1_5_5_5_REV:
            #ifndef OPENGL_ES
            case GL_UNSIGNED_SHORT_5_6_5_REV:
            #endif
                return 16;
            case GL_INT:
            case GL_UNSIGNED_INT:
            #ifndef OPENGL_ES
            case GL_UNSIGNED_INT_8_8_8_8:
            case GL_UNSIGNED_INT_8_8_8_8_REV:
            case GL_UNSIGNED_INT_10_10_10_2:
            case GL_UNSIGNED_INT_2_10_10_10_REV:
            #endif
                return 32;
            case GL_FLOAT:
                return 32;
        }

        return 0;
    }
    
    
    //------------------------------------------------------------------------
    uint bppForFormatAndType(GLenum format, GLenum type) {
        return channelsForFormat(format) * bitsPerChannelForType(type);
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: Texture Config ============================
    #pragma mark - Texture Config -
    
    int Texture::totalAllocatedTextureMemorySize = 0;
    
    TextureConfig::TextureConfig()
    :	format(GL_RGB)
    ,	internalFormat(GL_RGB)
    ,	type(GL_UNSIGNED_BYTE)
    ,	minFilter(GL_LINEAR)
    ,	magFilter(GL_LINEAR)
        #if defined(OPENGL_ES)
    ,	wrapS(GL_CLAMP_TO_EDGE)
    ,	wrapT(GL_CLAMP_TO_EDGE)
        #else
    ,	wrapS(GL_CLAMP_TO_BORDER)
    ,	wrapT(GL_CLAMP_TO_BORDER)
        #endif
    {}

    TextureConfig::TextureConfig(GLenum format)
    :	format(format)
    ,	internalFormat(format)
    ,	type(GL_UNSIGNED_BYTE)
    ,	minFilter(GL_LINEAR)
    ,	magFilter(GL_LINEAR)
        #if defined(OPENGL_ES)
    ,	wrapS(GL_CLAMP_TO_EDGE)
    ,	wrapT(GL_CLAMP_TO_EDGE)
        #else
    ,	wrapS(GL_CLAMP_TO_BORDER)
    ,	wrapT(GL_CLAMP_TO_BORDER)
        #endif
    {}
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: Texture ===================================
    #pragma mark - Texture -
    
    Texture::Texture()
    :	uid(0), width(0), height(0), channels(0), config(), sourceImage(NULL), sourceIsScaled(false)
    {}

    Texture::Texture(const FilePath &filePath, bool keepImage )
    :	uid(0), width(0), height(0), channels(0), config(), sourceImage(NULL), sourceIsScaled(false)
    {
        Image* img = new Image(filePath);
        load(img);
        
        if ( keepImage )
            sourceImage = img;
        else
            delete img;
    }

    Texture::Texture(Image* img) 
    :	uid(0), width(0), height(0), channels(0), config(), sourceImage(img), sourceIsScaled(false)
    {
        load(img);
    }

    Texture::Texture(Image* img, const TextureConfig &config)
    :	uid(0), width(0), height(0), channels(0), config(), sourceImage(img), sourceIsScaled(false)
    {
        load(img, config);
    }

    Texture::Texture(uint width, uint height, const ubyte *pixels, const TextureConfig &config)
    :	uid(0), width(0), height(0), channels(0), config(), sourceImage(NULL), sourceIsScaled(false)
    {
        load(width, height, pixels, config);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &uid);
        uid = 0;
        
        totalAllocatedTextureMemorySize -= width*height*channels;
        
        // should delete sourceImage in some cases
    }
    
    
    //------------------------------------------------------------------------
    Texture* Texture::copy() {
        #if defined(OPENGL_ES)
        #warning Texture::copy not implemented on iOS\n\
        #warning call poFBO::copyColorTexture to copy out of an FBO\n
            return NULL;
        #else
            po::saveTextureState();
        
            glBindTexture(GL_TEXTURE_2D, uid);
            
            #ifndef OPENGL_ES
            GLBuffer buffer(GL_PIXEL_PACK_BUFFER, getSizeInBytes());
            glBindBuffer(GL_PIXEL_PACK_BUFFER, buffer.getUid());
            glGetTexImage(GL_TEXTURE_2D, 0, config.format, config.type, NULL);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
            #endif
            
            Texture *tex = new Texture(width,height,NULL,config);
            glBindTexture(GL_TEXTURE_2D, tex->getUid());
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer.getUid());
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, config.format, config.type, NULL);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        
            po::restoreTextureState();
            return tex;
        #endif
    }
    
    
    //------------------------------------------------------------------------
    void Texture::replace(Image* image) {
        replace(image->getPixels());
    }
    
    
    //------------------------------------------------------------------------
    void Texture::replace(const ubyte *pixels) {
        po::saveTextureState();
        
        glBindTexture(GL_TEXTURE_2D, uid);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, config.format, GL_UNSIGNED_BYTE, pixels);
        
        po::restoreTextureState();
    }
    
    
    //------------------------------------------------------------------------
    bool Texture::isValid() const {
        return uid > 0;
    }
    
    
    //------------------------------------------------------------------------
    bool Texture::isScaled() const {
        return sourceIsScaled;
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Getters ==========================================
    #pragma mark Getters
    
    //------------------------------------------------------------------------
    TextureConfig Texture::getConfig() const {
        return config;
    }
    
    
    //------------------------------------------------------------------------
    uint Texture::getUid() const {
        return uid;
    }
    
    
    //------------------------------------------------------------------------
    uint Texture::getWidth() const {
        return isScaled() ? width/po::getScale() : width;
    }
    
    
    //------------------------------------------------------------------------
    uint Texture::getHeight() const {
        return isScaled() ? height/po::getScale() : height;
    }
    
    
    //------------------------------------------------------------------------
    uint Texture::getChannels() const {
        return channels;
    }
    
    
    //------------------------------------------------------------------------
    uint Texture::getBitsPerPixel() const {
        return bppForFormatAndType(config.format, config.type);
    }
    
    
    //------------------------------------------------------------------------
    size_t Texture::getSizeInBytes() const {
        return (getWidth() * po::getScale()) * (getHeight() * po::getScale()) * getBitsPerPixel();
    }
    
    
    //------------------------------------------------------------------------
    Point Texture::getDimensions() const {
        return Point(width, height, 0);
    }
    
    
    //------------------------------------------------------------------------
    Rect Texture::getBounds() const {
        return Rect(0,0,width,height);
    }
    
    
    //------------------------------------------------------------------------
    Color Texture::getSourceImagePixel(Point p) {
        if ( sourceImage == NULL )
            return Color(1,1,1,1);
        else
            return sourceImage->getPixel(p);
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Configuration ====================================
    #pragma mark Configuration
    
    //------------------------------------------------------------------------
    void Texture::setFormat(GLenum f) {
        config.setFormat(f);
        configure();
    }
    
    
    //------------------------------------------------------------------------
    void Texture::setInternalFormat(GLenum f) {
        config.setInternalFormat(f);
        configure();
    }
    
    
    //------------------------------------------------------------------------
    void Texture::setMagFilter(GLenum f) {
        config.setMagFilter(f);
        configure();
    }
    
    
    //------------------------------------------------------------------------
    void Texture::setMinFilter(GLenum f){
        config.setMinFilter(f);
        configure();
    }
    
    
    //------------------------------------------------------------------------
    void Texture::setType(GLenum f) {
        config.setType(f);
        configure();
    }
    
    
    //------------------------------------------------------------------------
    void Texture::setWrapS(GLenum f) {
        config.setWrapS(f);
        configure();
    }
    
    
    //------------------------------------------------------------------------
    void Texture::setWrapT(GLenum f) {
        config.setWrapT(f);
        configure();
    }
    
    
    //------------------------------------------------------------------------
    void Texture::configure(){
        po::saveTextureState();
        
        glBindTexture(GL_TEXTURE_2D, uid);
        
        // set the filters we want
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrapT);
        
    #ifndef OPENGL_ES
        float trans[] = {0.f, 0.f, 0.f, 0.f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, trans);
    #endif
        
        po::restoreTextureState();
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Loading ==========================================
    #pragma mark Loading
    
    //------------------------------------------------------------------------
    void Texture::load(Image* img) {
        if(!(img && img->isValid())) {
            loadDummyImage();
            return;
        }
        
        sourceIsScaled = img->isScaled();
        load(img, TextureConfig(formatForChannels(img->getChannels())));
    }
    
    
    //------------------------------------------------------------------------
    void Texture::load(Image* img, const TextureConfig &config) {
        if(!(img && img->isValid())) {
            loadDummyImage();
            return;
        }
        
        sourceIsScaled = img->isScaled();
        load(img->getWidth(), img->getHeight(), img->getPixels(), config);
    }
    
    
    //------------------------------------------------------------------------
    void Texture::load(uint width, uint height, int channels, const ubyte *pixels) {
        load(width, height, pixels, TextureConfig(formatForChannels(channels)));
    }
    
    
    //------------------------------------------------------------------------
    void Texture::load(uint w, uint h, const ubyte *p, const TextureConfig &c) {
        
        totalAllocatedTextureMemorySize -= width*height*channels;
        totalAllocatedTextureMemorySize += w*h*channelsForFormat(c.format);
        
        width       = w;
        height      = h;
        channels    = channelsForFormat(c.format);
        config = c;
        
        po::saveTextureState();
        
        glGenTextures(1, &uid);
        glBindTexture(GL_TEXTURE_2D, uid);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        
        // set the filters we want
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrapT);
        
    #ifndef OPENGL_ES
        float trans[] = {0.f, 0.f, 0.f, 0.f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, trans);
    #endif
        
        // i'm assuming you're replacing the whole texture anyway
        glTexImage2D(GL_TEXTURE_2D, 
                     0, 
                     config.internalFormat, 
                     width, 
                     height, 
                     0, 
                     config.format, 
                     config.type, 
                     p);
        
        po::restoreTextureState();
    }
    
    
    //------------------------------------------------------------------------
    void Texture::loadDummyImage() {
        static GLuint dummy = 0;
        if(!dummy) {
            ubyte *pix = new ubyte[20*20*3]();
            for(int y=0; y<20; y++) {
                for(int x=0; x<20; x++) {
                    if((y == 0 || x == 0 || y == 19 || x == 19) ||
                       (x == y || x == 19 - y))
                    {
                        pix[(y*20+x)*3] = 0xFF;
                    }
                }
            }
            
            po::saveTextureState();
            
            glGenTextures(1, &dummy);
            glBindTexture(GL_TEXTURE_2D, dummy);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            
            #ifndef OPENGL_ES
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            #else
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            #endif
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 20, 20, 0, GL_RGB, GL_UNSIGNED_BYTE, pix);
            delete [] pix;
            
            po::restoreTextureState();
        }
        
        uid = dummy;
        width = height = 20;
        channels = 3;
        config = TextureConfig(GL_RGB).setMinFilter(GL_NEAREST).setMagFilter(GL_NEAREST).setWrapS(GL_TEXTURE_WRAP_S).setWrapT(GL_TEXTURE_WRAP_T);
    }

    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Texture Fit ======================================
    #pragma mark Texture Fit
    
    void textureFitExact(Rect rect, Texture *tex, Alignment align, std::vector<Point> &coords, const std::vector<Point> &points);
    void textureFitNone(Rect rect, Texture *tex, Alignment align, std::vector<Point> &coords, const std::vector<Point> &points);
    void textureFitHorizontal(Rect rect, Texture *tex, Alignment align, std::vector<Point> &coords, const std::vector<Point> &points);
    void textureFitVertical(Rect rect, Texture *tex, Alignment align, std::vector<Point> &coords, const std::vector<Point> &points);
    
    
    //------------------------------------------------------------------------
    std::vector<Point> textureFit(Rect rect, Texture *tex, TextureFitOption fit, Alignment align) {
        std::vector<Point> coords(4);
        std::vector<Point> points = rect.getCorners();
        textureFit(rect, tex, fit, align, coords, points);
        return coords;
    }
    
    
    //------------------------------------------------------------------------
    void textureFit(Rect rect, Texture *tex, TextureFitOption fit, Alignment align, std::vector<Point> &coords, const std::vector<Point> &points) {
        switch(fit) {
            case PO_TEX_FIT_NONE:
                textureFitNone(rect, tex, align, coords, points);
                break;
                
            case PO_TEX_FIT_EXACT:
                textureFitExact(rect, tex, align, coords, points);
                break;
                
            case PO_TEX_FIT_H:
                textureFitHorizontal(rect, tex, align, coords, points);
                break;
                
            case PO_TEX_FIT_V:
                textureFitVertical(rect, tex, align, coords, points);
                break;
                
            case PO_TEX_FIT_INSIDE: 
            {
                float new_h = ((float)rect.width) * ((float)tex->getHeight()) / ((float)tex->getWidth());
                if(new_h > rect.height)
                    textureFitVertical(rect, tex, align, coords, points);
                else
                    textureFitHorizontal(rect, tex, align, coords, points);
                break;
            }
            default:
                ;
        }
    }
    
    
    //------------------------------------------------------------------------
    void textureFitExact(Rect rect, Texture *tex, Alignment align, std::vector<Point> &coords, const std::vector<Point> &points) {
        for(uint i=0; i<points.size(); i++) {
            float s = (points[i].x-rect.x) / rect.width;
            float t = (points[i].y-rect.y) / rect.height;
            coords[i].set(s,1.f-t,0.f);
        }
    }
    
    
    //------------------------------------------------------------------------
    void textureFitNone(Rect rect, Texture *tex, Alignment align, std::vector<Point> &coords, const std::vector<Point> &points) {
        Point max(FLT_MIN, FLT_MIN);
        
        for(uint i=0; i<points.size(); i++) {
            float s = (points[i].x-rect.x) / tex->getWidth();
            float t = (points[i].y-rect.y) / tex->getHeight();
            
            max.x = std::max(s, max.x);
            max.y = std::max(t, max.y);
            
            coords[i].set(s,t,0.f);
        }
        
        Point offset = po::alignInRect(max, Rect(0,0,1,1), align);
        
        for(uint i=0; i<coords.size(); i++) {
            coords[i].y = max.y - coords[i].y;
            coords[i] -= offset;
        }
    }
    
    
    //------------------------------------------------------------------------
    void textureFitHorizontal(Rect rect, Texture *tex, Alignment align, std::vector<Point> &coords, const std::vector<Point> &points) {
        float new_w = rect.width;
        float new_h = new_w / (tex->getWidth() / (float)tex->getHeight());
        
        Point max(FLT_MIN, FLT_MIN);
        
        for(uint i=0; i<points.size(); i++) {
            float s = (points[i].x-rect.x) / rect.width;
            float t = (points[i].y-rect.y) / new_h;
            
            max.x = std::max(s, max.x);
            max.y = std::max(t, max.y);
            
            coords[i].set(s,t,0.f);
        }
        
        Point offset = po::alignInRect(max, Rect(0,0,1,1), align);
        
        for(uint i=0; i<coords.size(); i++) {
            coords[i].y = max.y - coords[i].y;
            coords[i] -= offset;
        }
    }
    
    
    //------------------------------------------------------------------------
    void textureFitVertical(Rect rect, Texture *tex, Alignment align, std::vector<Point> &coords, const std::vector<Point> &points) {
        float new_h = rect.height;
        float new_w = new_h / (tex->getHeight() / (float)tex->getWidth());
        
        Point max(FLT_MIN, FLT_MIN);
        
        for(uint i=0; i<points.size(); i++) {
            float s = (points[i].x-rect.x) / new_w;
            float t = (points[i].y-rect.y) / rect.height;
            
            max.x = std::max(s, max.x);
            max.y = std::max(t, max.y);
            
            coords[i].set(s,t,0.f);
        }
        
        Point offset = po::alignInRect(max, Rect(0,0,1,1), align);
        
        for(uint i=0; i<coords.size(); i++) {
            coords[i].y = max.y - coords[i].y;
            coords[i] -= offset;
        }
    }
}/*End po namespace*/