
#include "poTextureAtlas.h"
#include "poSimpleDrawing.h"
#include "poImage.h"
#include "poHelpers.h"
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
	clearTextures();
	clearPages();
	clearImages();
}

void poTextureAtlas::addImage(poImage* img, uint request) {
	requestedIDs.push_back(request);

	poImage* copy = img->copy();
	copy->setNumChannels(channelsForFormat(config.format));
	images.push_back(copy);
}

bool poTextureAtlas::hasUID(uint uid) {
	return uids.find(uid) != uids.end();
}

int poTextureAtlas::getNumPages() {
	return textures.size();
}

poPoint poTextureAtlas::getDimensions() const {
	return poPoint(width, height);
}

uint poTextureAtlas::getPageForUID(uint uid) {
	return coords[uids[uid]].page;
}

poRect poTextureAtlas::getCoordsForUID(uint uid) {
	return coords[uids[uid]].coords;
}

poRect poTextureAtlas::getSizeForUID(uint uid) {
	return poRect(poPoint(0,0), coords[uids[uid]].size);
}

poTexture* poTextureAtlas::getTextureForPage(uint page) {
	return textures[page];
}

poTexture* poTextureAtlas::getTextureForUID(uint uid) {
	return textures[getPageForUID(uid)];
}

void poTextureAtlas::drawUID(uint uid, poRect rect) {
	if(hasUID(uid)) {
		uint page = getPageForUID(uid);
		
		poRect size = getSizeForUID(uid);
		poRect placement(rect.x, 
						 rect.y,
						 rect.width * size.width,
						 rect.height * size.height);
		poRect coords = getCoordsForUID(uid);
		po::drawTexturedRect(getTextureForPage(page), placement, coords);
	}
}

void poTextureAtlas::drawUID(uint uid, poPoint p) {
	drawUID(uid, poRect(p.x,p.y,1,1));
}

void poTextureAtlas::debugDraw() {
	packer.render();
}

void poTextureAtlas::clearTextures() {
	BOOST_FOREACH(poTexture *tex, textures)
		delete tex;
	textures.clear();
	coords.clear();
	uids.clear();
}

void poTextureAtlas::clearPages() {
	BOOST_FOREACH(poImage *img, pages)
		delete img;
	pages.clear();
}

void poTextureAtlas::clearImages() {
	BOOST_FOREACH(poImage *img, images)
		delete img;
	images.clear();
	requestedIDs.clear();
}

void poTextureAtlas::layoutAtlas() {
	clearTextures();

	packer.reset();
	
	for(int i=0; i<images.size(); i++) {
		uids[requestedIDs[i]] = packer.addRect(poRect(0,0,images[i]->getWidth(),images[i]->getHeight()));
	}
	packer.pack();
	
	for(int i=0; i<packer.getNumPages(); i++)
		pages.push_back(new poImage(width,height,channelsForFormat(config.format),NULL));
	
	coords.resize(images.size());
	
	for(int i=0; i<images.size(); i++) {
		uint pg;
		poRect pack_loc = packer.packPosition(i, &pg);
		
		uint uid = requestedIDs[i];
		
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
	clearImages();
    
	textures.resize(pages.size());
	
	for(int i=0; i<pages.size(); i++) {
		poImage* img = pages[i];
		textures[i] = new poTexture(img,config);
	}

	clearPages();
}

