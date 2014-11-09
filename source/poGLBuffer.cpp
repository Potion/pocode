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

#include "poGLBuffer.h"

#ifdef POTION_IOS
	#define glMapBuffer glMapBufferOES
	#define glUnmapBuffer glUnmapBufferOES
#endif


namespace po {
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: GLBuffer Config ===========================
    #pragma mark - GLBufferConfig -
    
    GLBufferConfig::GLBufferConfig()
    :	target(GL_ARRAY_BUFFER)
    ,	usage(GL_STREAM_DRAW)
    {}

    GLBufferConfig::GLBufferConfig(GLenum target)
    :	target(target)
    ,	usage(GL_STREAM_DRAW)
    {}

    GLBufferConfig::GLBufferConfig(GLenum target, GLenum usage)
    :	target(GL_ARRAY_BUFFER)
    ,	usage(GL_STREAM_DRAW)
    {}
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: GLBuffer ==================================
    #pragma mark - GLBuffer -
    
    GLBuffer::GLBuffer(GLenum target, size_t size) {
        glGenBuffers(1, &uid);
        setConfig(GLBufferConfig(target));
        setSize(size);
    }

    GLBuffer::GLBuffer(GLBufferConfig config, size_t size) {
        glGenBuffers(1, &uid);
        setConfig(config);
        setSize(size);
    }

    GLBuffer::~GLBuffer() {
        glDeleteBuffers(1, &uid);
        uid = 0;
    }
    
    
    //------------------------------------------------------------------------
    void GLBuffer::bind() {
        glBindBuffer(config.target, uid);
    }
    
    
    //------------------------------------------------------------------------
    void GLBuffer::unbind() {
        glBindBuffer(config.target, 0);
    }
    
    
    //------------------------------------------------------------------------
    void GLBuffer::bufferData(size_t size, const void *data, GLenum usage) {
        glBufferData(config.target, size, data, config.usage);
    }
    
    
    //------------------------------------------------------------------------
    void GLBuffer::bufferSubData(size_t offset, size_t size, const void *data) {
        glBufferSubData(config.target, offset, size, data);
    }
    
    
    //------------------------------------------------------------------------
    void *GLBuffer::map(GLenum access) {
        return glMapBuffer(config.target, access);
    }
    
    
    //------------------------------------------------------------------------
    void GLBuffer::unmap() {
        glUnmapBuffer(config.target);
    }
    
    
    //------------------------------------------------------------------------
    bool GLBuffer::isValid() const {
        return uid > 0;
    }
    
    
    //------------------------------------------------------------------------
    uint GLBuffer::getUid() const {
        return uid;
    }
    
    
    //------------------------------------------------------------------------
    size_t GLBuffer::getSize() const {
        return size;
    }
    
    
    //------------------------------------------------------------------------
    GLBufferConfig GLBuffer::getConfig() const {
        return config;
    }
    
    
    //------------------------------------------------------------------------
    void GLBuffer::setSize(size_t sz) {
        size = sz;
        
        bind();
        glBufferData(config.target, sz, NULL, config.usage);
        unbind();
    }
    
    
    //------------------------------------------------------------------------
    void GLBuffer::setConfig(GLBufferConfig c) {
        config = c;
    }
    
    
    //------------------------------------------------------------------------
    void GLBuffer::setTarget(GLenum target) {
        config.target = target;
    }
    
    
    //------------------------------------------------------------------------
    void GLBuffer::setUsage(GLenum usage) {
        config.usage = usage;
    }
}