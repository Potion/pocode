
#include "poTextureAtlas.h"
#include "poSimpleDrawing.h"
#include "poImage.h"
#include "poHelpers.h"
#include "poBasicRenderer.h"
#include "poOpenGLState.h"

poTextureAtlas::TextureAtlasImpl::TextureAtlasImpl(poTextureConfig config, uint w, uint h) 
:	width(w)
,	height(h)
,	config(config)
,	packer(w,h)
{}

poTextureAtlas::TextureAtlasImpl::~TextureAtlasImpl()
{}

void poTextureAtlas::TextureAtlasImpl::layoutAtlas() {
	packer.reset();
	
	for(int i=0; i<images.size(); i++) {
		uids[requestedIDs[i]] = packer.addRect(poRect(0,0,images[i].getWidth(),images[i].getHeight()));
	}
	packer.pack();
	
	for(int i=0; i<packer.getNumPages(); i++)
		pages.push_back(poImage(width,height,channelsForFormat(config.format),NULL));
	
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
		pages[pg].composite(images[i], pack_loc.getPosition(), 1.f);
	}
	
	textures.resize(pages.size());
	
	for(int i=0; i<pages.size(); i++) {
		poImage img = pages[i];
		textures[i] = poTexture(img,config);
	}
}

poTextureAtlas::poTextureAtlas(GLenum f, uint w, uint h)
:	shared(new TextureAtlasImpl(poTextureConfig(f), w, h))
{}

poTextureAtlas::poTextureAtlas(poTextureConfig c, uint w, uint h)
:	shared(new TextureAtlasImpl(c, w, h))
{}

poTextureAtlas::~poTextureAtlas() {
	clearTextures();
	clearPages();
	clearImages();
}

void poTextureAtlas::addImage(poImage img, uint request) {
	shared->requestedIDs.push_back(request);

	poImage copy = img.copy();
	copy.setNumChannels(channelsForFormat(shared->config.format));
	shared->images.push_back(copy);
}

bool poTextureAtlas::hasUID(uint uid) {
	return shared->uids.find(uid) != shared->uids.end();
}

int poTextureAtlas::getNumPages() {
	return shared->textures.size();
}

poPoint poTextureAtlas::getDimensions() const {
	return poPoint(shared->width, shared->height);
}

uint poTextureAtlas::getPageForUID(uint uid) {
	return shared->coords[shared->uids[uid]].page;
}

poRect poTextureAtlas::getCoordsForUID(uint uid) {
	return shared->coords[shared->uids[uid]].coords;
}

poRect poTextureAtlas::getSizeForUID(uint uid) {
	return poRect(poPoint(0,0), shared->coords[shared->uids[uid]].size);
}

poTexture poTextureAtlas::getTextureForPage(uint page) {
	return shared->textures[page];
}

poTexture poTextureAtlas::getTextureForUID(uint uid) {
	return shared->textures[getPageForUID(uid)];
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
	shared->packer.render();
}

void poTextureAtlas::clearTextures() {
	shared->textures.clear();
	shared->coords.clear();
	shared->uids.clear();
}

void poTextureAtlas::clearPages() {
	shared->pages.clear();
}

void poTextureAtlas::clearImages() {
	shared->images.clear();
	shared->requestedIDs.clear();
}

void poTextureAtlas::layoutAtlas() {
	clearTextures();
	shared->layoutAtlas();
	// save some space
	clearPages();
}

