//
//  poTexture.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 4/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poTexture.h"
#include "BinPacker.h"
#include "FileNotFoundImage.h"

bool formatsForBitDepth(ImageBitDepth bpp, GLenum *format, GLenum *internal_format, GLenum *type) {
	switch(bpp) {
		case IMAGE_8:
			*format = GL_LUMINANCE;
			*internal_format = GL_LUMINANCE;
			*type = GL_UNSIGNED_BYTE;
			return true;
			
		case IMAGE_16:
			*format = GL_LUMINANCE_ALPHA;
			*internal_format = 2;
			*type = GL_UNSIGNED_BYTE;
			return true;
			
		case IMAGE_24:
			*format = GL_BGR;
			*internal_format = 3;
			*type = GL_UNSIGNED_BYTE;
			return true;
			
		case IMAGE_32:
			*format = GL_BGRA;
			*internal_format = 4;
			*type = GL_UNSIGNED_BYTE;
			return true;
	}
	
	printf("poTexture: unsupported image bit depth (%d)\n", bpp);
	return false;
}

size_t bytesForPixelFormat(GLenum format) {
	switch(format) {
		case GL_BGRA:
		case GL_RGBA:	return 4; break;
			
		case GL_BGR:
		case GL_RGB:	return 3; break;
		
		case GL_LUMINANCE_ALPHA:	return 2; break;
			
		case GL_ALPHA:
		case GL_LUMINANCE: return 1; break;
			
		default:		return 0;
	}
}

poTexture::poTexture() {}

poTexture::poTexture(poImage *img) {
	if(img && img->isValid())
		load(img);
	else
		loadNotFound();
}

poTexture::poTexture(const std::string &str) {
	poImage img(str);
	if(img.isValid())
		load(&img);
	else
		loadNotFound();
}

poTexture::poTexture(GLenum format, uint width, uint height, uint mem, ubyte const*pixels) {
	load(format, format, GL_UNSIGNED_BYTE, 
		 GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP,
		 width, height, mem, pixels);
	pushToCard();
	
	createRefCounter();
	incrRefCount();
}

poTexture::poTexture(GLenum format, GLenum internal_format, GLenum type,
					 uint width, uint height, uint mem, ubyte const*pixels)
{
	load(format, internal_format, type,
		 GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP,
		 width, height, mem, pixels);
	pushToCard();
	
	createRefCounter();
	incrRefCount();
}

poTexture::~poTexture() {
	decrRefCount();
	if(refCount() <= 0) {
		if(isOnCard())
			pullFromCard();
		if(storingPixels())
			deleteLocalMemory();
		delete ref_count;
	}
}

poTexture *poTexture::copy() {
	poTexture *tex = new poTexture();
	tex->load(_format, _internal_format, _type, 
			  _min_filter, _mag_filter, _wrap_s, _wrap_t,
			  _width, _height, 0, NULL);
	tex->st(_s, _t);
	tex->_uid = _uid;
	tex->_pixels = _pixels;
	tex->_mem_size = _mem_size;
	
	tex->ref_count = ref_count;
	incrRefCount();
	
	return tex;
}

void poTexture::replace(ubyte const*pix) {
	if(storingPixels())
		memcpy(_pixels, pix, _mem_size);

	glBindTexture(GL_TEXTURE_2D, uid());
	
	// TODO optimize this with pbuffer transfer
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width(), height(), format(), GL_UNSIGNED_BYTE, pix);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool poTexture::opaqueAtPoint(poPoint p) const {
	poColor color = colorAtPoint(p);
	return color.A > 0;
}

poColor poTexture::colorAtPoint(poPoint p) const {
	poColor response(0,0,0,0);

	if(p.x < 0 || p.y < 0 || p.x >= width() || p.y >= height()) {
		return response;
	}
	
	uint bpp = bytesForPixelFormat(format());
	if(bpp == 4 || bpp == 2) {
		if(storingPixels()) {
			GLubyte *pix = _pixels + int(p.y * width() + p.x) * bpp;
			switch(bpp) {
				case 1: response.set255(pix[0], pix[0], pix[0], 255);		break;
				case 2: response.set255(pix[0], pix[0], pix[0], pix[1]);	break;
				case 3: response.set255(pix[2], pix[1], pix[0], 255);		break;
				case 4: response.set255(pix[2], pix[1], pix[0], pix[3]);	break;
			}
			
			return response;
		}
	}
	
	return response;
}

uint poTexture::uid() const			{return _uid;}
float poTexture::width() const		{return _width;}
float poTexture::height() const		{return _height;}
float poTexture::s() const			{return _s;}
float poTexture::t() const			{return _t;}
GLenum poTexture::format() const	{return _format;}
GLenum poTexture::internalFormat() const {return _internal_format;}
GLenum poTexture::type() const		{return _type;}
GLenum poTexture::minFilter() const {return _min_filter;}
GLenum poTexture::magFilter() const {return _mag_filter;}
GLenum poTexture::wrapS() const		{return _wrap_s;}
GLenum poTexture::wrapT() const		{return _wrap_t;}

bool poTexture::isOnCard() const {return _uid != 0;}
bool poTexture::storingPixels() const {return _pixels != NULL;}
void poTexture::deleteLocalMemory() {
	delete [] _pixels; 
	_pixels = NULL; 
	_mem_size = 0;
}

void poTexture::pushToCard() {
	if(storingPixels() && !isOnCard()) {
		glGenTextures(1, &_uid);
		glBindTexture(GL_TEXTURE_2D, _uid);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _mag_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap_t);
		glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, _width, _height, 0, _format, _type, _pixels);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void poTexture::pullFromCard() {
	if(isOnCard()) {
		glDeleteTextures(1, &_uid);
		_uid = 0;
	}
}

void poTexture::bind(uint unit) {
	glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(GL_TEXTURE_2D, _uid);
}

void poTexture::unbind(uint unit) {
	glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void poTexture::st(float s, float t) {
	_s = s;
	_t = t;
}

void poTexture::createRefCounter() {
	ref_count = new uint[1];
	*ref_count = 0;
}

void poTexture::incrRefCount() {
	(*ref_count)++;
}

void poTexture::decrRefCount() {
	(*ref_count)--;
}

uint poTexture::refCount() {
	return *ref_count;
}

void poTexture::load(poImage *img) {
	GLenum f, i, t; 
	formatsForBitDepth(img->bpp(), &f, &i, &t);
	
	load(f, i, t,
		 GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP,
		 img->width(), img->height(), img->storageSize(), img->pixels());
	pushToCard();
	
	createRefCounter();
	incrRefCount();
}

void poTexture::loadNotFound() {
//	poResourceStore tmp;
//	load(tmp.add(getFileNotFoundImage()));
	
	static poImage *img = NULL;
	if(!img) {
		ubyte *pix = new ubyte[10*10*3]();
		for(int i=0; i<10*10; i++) {
			pix[i*3+2] = 255;
			pix[i*3+1] = 0;
			pix[i*3] = 0;
		}
		img = new poImage(10,10,IMAGE_24,pix);
	}
	load(img);
}

void poTexture::load(GLenum format, GLenum internal_format, GLenum type, 
					 GLenum min, GLenum mag, GLenum ws, GLenum wt,
					 uint w, uint h, uint mem, ubyte const*pixels)
{
	_uid = 0;
	_s = _t = 1.f;
	_format = format;
	_internal_format = internal_format;
	_type = type;
	_min_filter = min;
	_mag_filter = mag;
	_wrap_s = ws;
	_wrap_t = wt;
	_width = w;
	_height = h;
	_mem_size = mem;
	_pixels = new ubyte[_mem_size]();
	if(pixels)
		memcpy(_pixels, pixels, _mem_size);
}





poTextureAtlas::poTextureAtlas(GLenum f, uint w, uint h)
:	width(w)
,	height(h)
,	format(f)
,	bound_page(-1)
{}

poTextureAtlas::~poTextureAtlas() {
	clearImages();
	clearPages();
}

void poTextureAtlas::clearImages() {
	BOOST_FOREACH(poImage *img, images) {
		delete img;
	}
	images.clear();
	requested_ids.clear();
}

void poTextureAtlas::addImage(poImage *img, uint request) {
	poImage *copy = img->copy();
	images.push_back(copy);
	
	ImageBitDepth bpp = (ImageBitDepth)(bytesForPixelFormat(format)*8);
	copy->changeBpp(bpp);
	
	requested_ids.push_back(request);
	
	return images.size() - 1;
}

void poTextureAtlas::layoutAtlas() {
	clearPages();
	
	BinPacker pack(width,height);
	
	for(int i=0; i<images.size(); i++) {
		uids[requested_ids[i]] = pack.addRect(poRect(0,0,images[i]->width(),images[i]->height()));
	}
	pack.pack();
	
	for(int i=0; i<pack.numPages(); i++) {
		ImageBitDepth depth = (ImageBitDepth)(bytesForPixelFormat(format)*8);
		pages.push_back(new poImage(width,height,depth,NULL));
	}
	
	coords.resize(images.size());
	
	for(int i=0; i<images.size(); i++) {
		uint pg;
		poRect pack_loc = pack.packPosition(i, &pg);
		
		ImageLookup item;
		item.page = pg;
		
		poPoint origin(pack_loc.origin.x / (float)width,
					   pack_loc.origin.y / (float)height);
		poPoint size(pack_loc.size.x / (float)width,
					 pack_loc.size.y / (float)height);
		
		item.coords = poRect(origin, size);
		item.coords.origin.y = 1.f - item.coords.origin.y - item.coords.size.y;
		
		item.size = pack_loc.size;
		
		coords[i] = item;
		pages[pg]->composite(images[i], pack_loc);
	}
	
	textures.resize(pages.size());
	
	for(int i=0; i<pages.size(); i++) {
		poImage *img = pages[i];
		textures[i] = new poTexture(format, img->width(), img->height(), img->storageSize(), img->pixels());
	}
}

bool poTextureAtlas::hasUID(uint uid) {
	return uids.find(uid) != uids.end();
}

int poTextureAtlas::numPages() {
	return pages.size();
}

poPoint poTextureAtlas::dimensions() const {
	return poPoint(width, height);
}

uint poTextureAtlas::pageForUID(uint uid) {
	return coords[uids[uid]].page;
}

poRect poTextureAtlas::coordsForUID(uint uid) {
	return coords[uids[uid]].coords;
}

poRect poTextureAtlas::sizeForUID(uint uid) {
	return poRect(poPoint(0,0), coords[uids[uid]].size);
}

poTexture *poTextureAtlas::textureForPage(uint page) {
	return textures[page];
}

void poTextureAtlas::clearPages() {
	for(int i=0; i<pages.size(); i++) {
		delete pages[i];
		delete textures[i];
	}
	pages.clear();
	textures.clear();
	coords.clear();
	uids.clear();
}

void poTextureAtlas::startDrawing(uint unit) {
	this->unit = unit;
	if(bound_page >= 0)
		stopDrawing();
	
	glPushAttrib(GL_TEXTURE_BIT);
	glPushClientAttrib(GL_TEXTURE_BIT);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glClientActiveTexture(GL_TEXTURE0+unit);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glPushAttrib(GL_TEXTURE_BIT);
	glEnable(GL_TEXTURE_2D);
}

void poTextureAtlas::bindPage(uint page) {
	if(page != bound_page) {
		textures[page]->bind(unit);
		bound_page = page;
	}
}

void poTextureAtlas::drawUID(uint uid, poRect rect) {
	if(hasUID(uid)) {
		uint page = pageForUID(uid);
		bindPage(page);

		poRect size = sizeForUID(uid);
		poRect coords = coordsForUID(uid);
		
		GLfloat quad[4*3] = { 
			rect.origin.x, rect.origin.y, 0, 
			rect.origin.x, rect.origin.y+(size.size.y*rect.size.y), 0, 
			rect.origin.x+(size.size.x*rect.size.x), rect.origin.y, 0, 
			rect.origin.x+(size.size.x*rect.size.x), rect.origin.y+(size.size.y*rect.size.y), 0 
		};
		
		GLfloat tcoords[4*2] = {
			coords.origin.x, coords.origin.y,
			coords.origin.x, coords.origin.y+coords.size.y,
			coords.origin.x+coords.size.x, coords.origin.y,
			coords.origin.x+coords.size.x, coords.origin.y+coords.size.y
		};

		glVertexPointer(3, GL_FLOAT, 0, quad);
		glTexCoordPointer(2, GL_FLOAT, 0, tcoords);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}

void poTextureAtlas::drawUID(uint uid, poPoint p) {
	drawUID(uid, poRect(p,poPoint(1,1)));
}

void poTextureAtlas::stopDrawing() {
	if(bound_page >=0) {
		textures[bound_page]->unbind(unit);
		bound_page = -1;
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glPopClientAttrib();
	glPopAttrib();
}


