#include "poCommon.h"

#ifndef _PO_RESOURCE_STORE_H_
#define _PO_RESOURCE_STORE_H_

#include "poFont.h"
#include "poImage.h"
#include "poTexture.h"

struct name_match {bool operator() (const char* a, const char* b) const {return strcmp(a, b) < 0;} };

class poFontStore
{
public:
	static poFont* get(const char* name, int size, int faceidx=0, int charmap=0, bool getVectorFont=false );
	static std::map<const char*, poFont*, name_match> fonts;
};

class poImageStore
{
public:
	static poImage* get(const char* name);
	static void clearAllImages();

	static std::map<const char*, poImage*, name_match> images;
};

class poTextureStore
{
public:
	static poTexture* get(const char* name, bool doCompression=false);
	static void clearAllTextures();
	
	static std::map<const char*, poTexture*, name_match> textures;
};

#endif