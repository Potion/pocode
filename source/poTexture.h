//
//  poTexture.h
//  poFramework4
//
//  Created by Joshua Fisher on 4/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poImage.h"

class poTexture {
public:
	poTexture();
	poTexture(poImage img);
	poTexture(const poTexture &tex);
	poTexture &operator=(const poTexture &tex);

	uint uid() const;
	uint width() const;
	uint height() const;
	float s() const;
	float t() const;
	GLenum format() const;
	GLenum internalFormat() const;
	GLenum type() const;
	GLenum minFilter() const;
	GLenum magFilter() const;
	GLenum wrapS() const;
	GLenum wrapT() const;
	
	void bind(uint unit=0);
	void unbind(uint unit=0);
	
	void st(float s, float t);
	
private:
	struct texture_impl {
		texture_impl(GLenum format, GLenum internal_format, GLenum type, 
					 GLenum min, GLenum mag, GLenum ws, GLenum wt,
					 int w, int h, ubyte const*pixels);
		~texture_impl();
		
		GLuint uid;
		float s, t;
		int width, height;
		GLenum format, internal_format, type;
		GLenum min_filter, mag_filter;
		GLenum wrap_s, wrap_t;
	};
	boost::shared_ptr<texture_impl> reference;
};


