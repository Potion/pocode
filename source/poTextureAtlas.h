#pragma once

#include "poRect.h"
#include "poPoint.h"
#include "poTexture.h"

class poTextureAtlas 
{
public:
	poTextureAtlas(GLenum f, uint w, uint h);
	virtual ~poTextureAtlas();
	
	// move the list of images to pack
	void clearImages();
	// pass in the id you want to have associated with the image
	void addImage(poImage *img, uint requested_id);
	// will redo all the bin packing with the current set of images
	void layoutAtlas();
	
	bool hasUID(uint uid);
	int numPages();
	poPoint dimensions() const;
	
	uint pageForUID(uint uid);
	poRect coordsForUID(uint uid);
	poRect sizeForUID(uint uid);
	poTexture *textureForPage(uint pg);
	
	// start drawing sets up the texture state
	void startDrawing(uint unit=0);
	// draws will shift the texture as needed,
	// tho user can look at the pages for what it wants to draw and
	// organize it so there are minimal texture switches
	// size should be between 0..1, will scale
	void drawUID(uint uid, poRect r);
	void drawUID(uint uid, poPoint p);
	// reset the texture state to what it was
	void stopDrawing();
	
private:
	void clearPages();
	void bindPage(uint page);
	
	struct ImageLookup {
		uint page;
		poRect coords;
		poPoint size;
	};
	
	int width, height;
	std::vector<ImageLookup> coords;
	// the items to be atlased
	std::vector<poImage*> images;
	// users control the ids
	std::vector<uint> requested_ids;
	// the pages of the atlas
	std::vector<poImage*> pages;
	// the textures of the atlas
	std::vector<poTexture*> textures;
	std::map<uint,uint> uids;
	// the is the format we want
	GLenum format;
	// store the last bound page so we can avoid rebinding
	int bound_page, unit;
};
