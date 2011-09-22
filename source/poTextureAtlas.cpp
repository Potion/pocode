#include "poTextureAtlas.h"
#include "SimpleDrawing.h"
#include "poImage.h"
#include "Loaders.h"
#include "BinPacker.h"
#include "Helpers.h"
#include "poBasicRenderer.h"

poTextureAtlas::poTextureAtlas(GLenum f, uint w, uint h)
:	width(w)
,	height(h)
,	config(f)
,	is_drawing(false)
{}

poTextureAtlas::poTextureAtlas(poTextureConfig c, uint w, uint h)
:	width(w)
,	height(h)
,	config(c)
,	is_drawing(false)
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
		
		poPoint origin(pack_loc.x / (float)width,
					   pack_loc.y / (float)height);
		poPoint size(pack_loc.width / (float)width,
					 pack_loc.height / (float)height);
		
		item.coords = poRect(origin, size);
		item.coords.y = 1.f - item.coords.y - item.coords.height;
		
		item.size = pack_loc.getSize();
		
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

void poTextureAtlas::startDrawing() {
	is_drawing = true;
	texState = poTextureState(textures[0]->uid);
}

void poTextureAtlas::drawUID(uint uid, poRect rect) {
	if(hasUID(uid)) {
		uint page = pageForUID(uid);
		poOpenGLState::get()->setTextureState(po::TextureState(textures[page]->uid));
		
		poRect size = sizeForUID(uid);
		poRect coords = coordsForUID(uid);
		
		rect.setPosition(rect.getPosition() + originAdjust());
		
		GLfloat quad[4*3] = { 
			rect.x, rect.y, 0, 
			rect.x, rect.y+(size.height*rect.height), 0, 
			rect.x+(size.width*rect.width), rect.y, 0, 
			rect.x+(size.width*rect.width), rect.y+(size.height*rect.height), 0 
		};
		
		GLfloat tcoords[4*2] = {
			coords.x, coords.y,
			coords.x, coords.y+coords.height,
			coords.x+coords.width, coords.y,
			coords.x+coords.width, coords.y+coords.height
		};

		poBasicRenderer::get()->setFromState();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, quad);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, tcoords);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}

void poTextureAtlas::drawUID(uint uid, poPoint p) {
	drawUID(uid, poRect(p.x,p.y,1,1));
}

void poTextureAtlas::stopDrawing() {
	is_drawing = false;
	texState.revert();
}

bool poTextureAtlas::isDrawing() {
	return is_drawing;
}




