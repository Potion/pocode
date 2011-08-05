#include "poTextureAtlas.h"
#include "SimpleDrawing.h"
#include "poImage.h"
#include "Loaders.h"
#include "BinPacker.h"
#include "Helpers.h"

poTextureAtlas::poTextureAtlas(GLenum f, uint w, uint h)
:	width(w)
,	height(h)
,	config(f)
,	bound_page(-1)
{}

poTextureAtlas::poTextureAtlas(poTextureConfig c, uint w, uint h)
:	width(w)
,	height(h)
,	config(c)
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
	
	ImageBitDepth bpp = bitDepthForFormat(config.format);
	copy->changeBpp(bpp);
	
	requested_ids.push_back(request);
	
	return;
}

void poTextureAtlas::layoutAtlas() {
	clearPages();
	
	BinPacker pack(width,height);
	
	for(int i=0; i<images.size(); i++) {
		uids[requested_ids[i]] = pack.addRect(poRect(0,0,images[i]->width(),images[i]->height()));
	}
	pack.pack();
	
	poImageLoader loader;
	for(int i=0; i<pack.numPages(); i++) {
		ImageBitDepth depth = bitDepthForFormat(config.format);
		pages.push_back(loader.load(poImageSpec("",width,height,depth,NULL)));
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
		textures[i] = img->texture(config);
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
		
		rect.origin += originAdjust();
		
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
	drawUID(uid, poRect(p.x,p.y,1,1));
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

bool poTextureAtlas::bindInfo(int *bp, int *bu) {
	if(bound_page >= 0) {
		*bp = bound_page;
		*bu = unit;
		return true;
	}
	return false;
}




