
#include "poTextureAtlas.h"
#include "SimpleDrawing.h"
#include "poImage.h"
#include "Helpers.h"
#include "poBasicRenderer.h"
#include "poOpenGLState.h"

poTextureAtlas::poTextureAtlas(GLenum f, uint w, uint h)
:	width(w)
,	height(h)
,	config(f)
,	packer(w,h,2)
{}

poTextureAtlas::poTextureAtlas(poTextureConfig c, uint w, uint h)
:	width(w)
,	height(h)
,	config(c)
,	packer(w,h,2)
{}

poTextureAtlas::~poTextureAtlas() {
	clearImages();
	clearPages();
}

void poTextureAtlas::addImage(poImage *img, uint request) {
	requested_ids.push_back(request);

	poImage *copy = img->copy();
	copy->changeChannelCount(channelsForFormat(config.format));
	images.push_back(copy);
}

void poTextureAtlas::layoutAtlas() {
	clearTextures();
	packer.reset();
	
	for(int i=0; i<images.size(); i++) {
		uids[requested_ids[i]] = packer.addRect(poRect(0,0,images[i]->width(),images[i]->height()));
	}
	packer.pack();
	
	for(int i=0; i<packer.numPages(); i++)
		pages.push_back(new poImage(width,height,channelsForFormat(config.format),NULL));
	
	coords.resize(images.size());
	
	for(int i=0; i<images.size(); i++) {
		uint pg;
		poRect pack_loc = packer.packPosition(i, &pg);
		
		uint uid = requested_ids[i];
		
		ImageLookup item;
		item.page = pg;
		
		poPoint size(pack_loc.width / (float)width,
					 pack_loc.height / (float)height);

		poPoint origin(pack_loc.x / (float)width,
					   pack_loc.y / (float)height);
		
		item.coords = poRect(origin, size);
		item.coords.y = 1.f - item.coords.y - item.coords.height;
		
		item.size = pack_loc.getSize();
		
		coords[i] = item;
		pages[pg]->composite(images[i], pack_loc.getPosition(), 1.f);
	}
	
	textures.resize(pages.size());
	
	for(int i=0; i<pages.size(); i++) {
		poImage *img = pages[i];
		textures[i] = new poTexture(img,config);
	}
	
	clearPages();
}

bool poTextureAtlas::hasUID(uint uid) {
	return uids.find(uid) != uids.end();
}

int poTextureAtlas::numPages() {
	return textures.size();
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

poTexture *poTextureAtlas::textureForUID(uint uid) {
	return textures[pageForUID(uid)];
}

void poTextureAtlas::clearTextures() {
	for(int i=0; i<textures.size(); i++) {
		delete textures[i];
	}
	textures.clear();
	coords.clear();
	uids.clear();
}

void poTextureAtlas::clearPages() {
	for(int i=0; i<pages.size(); i++)
		delete pages[i];
	pages.clear();
}

void poTextureAtlas::clearImages() {
	BOOST_FOREACH(poImage *img, images) {
		delete img;
	}
	images.clear();
	requested_ids.clear();
}

void poTextureAtlas::drawUID(uint uid, poRect rect) {
	if(hasUID(uid)) {
		uint page = pageForUID(uid);
		
		poRect size = sizeForUID(uid);
		poRect placement(rect.x, 
						 rect.y,
						 rect.width * size.width,
						 rect.height * size.height);
		poRect coords = coordsForUID(uid);
		po::drawRect(placement, coords, textureForPage(page));
	}
}

void poTextureAtlas::drawUID(uint uid, poPoint p) {
	drawUID(uid, poRect(p.x,p.y,1,1));
}

void poTextureAtlas::_debugDraw() {
	packer.render();
}


