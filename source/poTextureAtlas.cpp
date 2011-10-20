
#include "poTextureAtlas.h"
#include "SimpleDrawing.h"
#include "poImage.h"
#include "Helpers.h"
#include "poBasicRenderer.h"
#include "poOpenGLState.h"

poTextureAtlas::TextureAtlasImpl::TextureAtlasImpl(poTextureConfig c, uint w, uint h)
:	width(w)
,	height(h)
,	config(c)
,	packer(w,h,2)
{
	
}

poTextureAtlas::TextureAtlasImpl::~TextureAtlasImpl() {
	clearTextures();
	clearPages();
	clearImages();
}

void poTextureAtlas::TextureAtlasImpl::clearTextures() {
	textures.clear();
	coords.clear();
	uids.clear();
}

void poTextureAtlas::TextureAtlasImpl::clearPages() {
	pages.clear();
}

void poTextureAtlas::TextureAtlasImpl::clearImages() {
	images.clear();
	requested_ids.clear();
}

void poTextureAtlas::TextureAtlasImpl::layoutAtlas() {
	clearTextures();
	packer.reset();
	
	for(int i=0; i<images.size(); i++) {
		uids[requested_ids[i]] = packer.addRect(poRect(0,0,images[i].getWidth(),images[i].getHeight()));
	}
	packer.pack();
	
	for(int i=0; i<packer.numPages(); i++)
		pages.push_back(poImage(width,height,channelsForFormat(config.format),NULL));
	
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
		pages[pg].composite(images[i], pack_loc.getPosition(), 1.f);
	}
	
	textures.resize(pages.size());
	
	for(int i=0; i<pages.size(); i++) {
		poImage img = pages[i];
		textures[i] = poTexture(img,config);
	}
	
	clearPages();
}


poTextureAtlas::poTextureAtlas(GLenum f, uint w, uint h)
:	shared(new TextureAtlasImpl(poTextureConfig(f),w,h))
{}

poTextureAtlas::poTextureAtlas(poTextureConfig c, uint w, uint h)
:	shared(new TextureAtlasImpl(c,w,h))
{}

void poTextureAtlas::addImage(poImage img, uint request) {
	shared->requested_ids.push_back(request);

	poImage copy = img.copy();
	copy.setNumChannels(channelsForFormat(shared->config.format));
	shared->images.push_back(copy);
}

void poTextureAtlas::clearImages() {
	shared->clearImages();
}

void poTextureAtlas::layoutAtlas() {
	shared->layoutAtlas();
}

bool poTextureAtlas::hasUID(uint uid) {
	return shared->uids.find(uid) != shared->uids.end();
}

int poTextureAtlas::numPages() {
	return shared->textures.size();
}

poPoint poTextureAtlas::dimensions() const {
	return poPoint(shared->width, shared->height);
}

uint poTextureAtlas::pageForUID(uint uid) {
	return shared->coords[shared->uids[uid]].page;
}

poRect poTextureAtlas::coordsForUID(uint uid) {
	return shared->coords[shared->uids[uid]].coords;
}

poRect poTextureAtlas::sizeForUID(uint uid) {
	return poRect(poPoint(0,0), shared->coords[shared->uids[uid]].size);
}

poTexture poTextureAtlas::textureForPage(uint page) {
	return shared->textures[page];
}

poTexture poTextureAtlas::textureForUID(uint uid) {
	return shared->textures[pageForUID(uid)];
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
		po::drawRect(textureForPage(page), placement, coords);
	}
}

void poTextureAtlas::drawUID(uint uid, poPoint p) {
	drawUID(uid, poRect(p.x,p.y,1,1));
}

void poTextureAtlas::_debugDraw() {
	shared->packer.render();
}


