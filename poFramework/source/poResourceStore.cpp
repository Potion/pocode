#include "poResourceStore.h"
#include "poImage.h"
#include "poBitmapFont.h"
#include "poVectorFont.h"

std::map<const char*, poFont*, name_match> poFontStore::fonts;
std::map<const char*, poImage*, name_match> poImageStore::images;
std::map<const char*, poTexture*, name_match> poTextureStore::textures;

//poFont* firstFont = NULL;

poFont* poFontStore::get(const char* name, int size, int faceidx, int charmap, bool getVectorFont )
{
	/*if ( firstFont )
		return firstFont;*/
	
	char* lookup = (char*)malloc(strlen(name) + 30);
	sprintf(lookup, "%s_%d_%d", name, size, faceidx);
	
	std::map<const char*, poFont*, name_match>::iterator found = fonts.find(lookup);
	if(found != fonts.end())
	{
		free(lookup);
		return found->second;
	}
	
	// we didn't find one so build it
	poFont* font;
	if ( getVectorFont )
		font = new poVectorFont(name, size, faceidx, charmap);
	else
		font = new poBitmapFont(name, size, faceidx, charmap);
	fonts[lookup] = font;
	//firstFont = font;
	return font;
}

poImage* poImageStore::get(const char* name)
{
	std::map<const char*, poImage*, name_match>::iterator found = images.find(name);
		
	if(found != images.end())
	{
		return found->second;
	}
	
	// we didn't find one so build it
	char* copiedName = (char*) malloc( strlen(name)+1 );
	strcpy( copiedName, name );
		
	poImage* image = new poImage(copiedName);
	images[copiedName] = image;
	return image;
}

void poImageStore::clearAllImages()
{
	std::map<const char*, poImage*, name_match>::iterator itr;

	for( itr = images.begin(); itr != images.end(); itr++){
		poImage *Im = itr->second;
		delete Im;
	}

	images.clear();
}

int poTextureStoreMemoryCount = 0;


poTexture* poTextureStore::get(const char* name, bool doCompression)
{
	std::map<const char*, poTexture*, name_match>::iterator found = textures.find(name);
	
	if(found != textures.end())
	{
		//printf("found texture: %s\n", name );
		return found->second;
	}
	
	// we didn't find one so build it
	char* copiedName = (char*) malloc( strlen(name)+1 );
	strcpy( copiedName, name );
	
	poTexture* tex = new poTexture(copiedName, doCompression);
	textures[copiedName] = tex;
	
	poTextureStoreMemoryCount += tex->width*tex->height*tex->components;
	printf("poTextureStoreMemoryCount: %d\n", poTextureStoreMemoryCount/1024);
	return tex;
}


void poTextureStore::clearAllTextures()
{
	std::map<const char*, poTexture*, name_match>::iterator itr;
	
	for( itr = textures.begin(); itr != textures.end(); itr++){
		poTexture *tex = itr->second;
		delete tex;
	}
	
	textures.clear();
}

