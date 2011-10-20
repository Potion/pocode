#pragma once

#include "poRect.h"
#include "poPoint.h"
#include "poTexture.h"
#include "poResource.h"
#include "BinPacker.h"

class poTextureAtlas : public poResource
{
public:
	poTextureAtlas(GLenum f, uint w, uint h);
	poTextureAtlas(poTextureConfig config, uint w, uint h);
	
    // pass in the id you want to have associated with the image
	void            addImage(poImage img, uint requested_id);
	// remove all images that are current in there
	void            clearImages();

	// will redo all the bin packing with the current set of images
	void            layoutAtlas();
	
	bool            hasUID(uint uid);
	int             numPages();
	poPoint         dimensions() const;
	
	uint            pageForUID(uint uid);
	poRect          coordsForUID(uint uid);
	poRect          sizeForUID(uint uid);
	poTexture		textureForPage(uint pg);
	poTexture		textureForUID(uint uid);
    
	// draws will shift the texture as needed,
	// tho user can look at the pages for what it wants to draw and
	// organize it so there are minimal texture switches
	// size should be between 0..1, will scale
	void            drawUID(uint uid, poRect r);
	// this one will draw the subtex at its native size
	void            drawUID(uint uid, poPoint p);
	
	
	// dump the pack to screen for debugging
	void			_debugDraw();
	
private:
	struct ImageLookup {
		uint page;
		poRect coords;
		poPoint size;
	};
	
	struct TextureAtlasImpl {
		TextureAtlasImpl(poTextureConfig c, uint w, uint h);
		~TextureAtlasImpl();
		
		void clearImages();
		void clearPages();
		void clearTextures();
		
		void layoutAtlas();
		
		int width, height;
		std::vector<ImageLookup> coords;
		
		// the items to be atlased
		std::vector<poImage> images;
		
		// users control the ids
		std::vector<uint> requested_ids;
		
		// the pages of the atlas
		std::vector<poImage> pages;
		
		// the textures of the atlas
		std::vector<poTexture> textures;
		std::map<uint,uint> uids;
		
		// this is the configuration we want for the atlas
		poTextureConfig config;
		
		BinPacker packer;
	};
	boost::shared_ptr<TextureAtlasImpl> shared;
	
};
