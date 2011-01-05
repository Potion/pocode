#include "poCommon.h"
#include "poObjectModifier.h"

#ifndef _PO_TEXTURE_H_
#define _PO_TEXTURE_H_

#include "poImage.h"

class poObject;

enum	poBlendingMode { PO_NORMAL_BLENDING, PO_SCREEN_BLENDING, PO_MULTIPLY_BLENDING };

	

class poTexture : public poObjectModifier
{
public:
	poTexture();
	poTexture( char* imageFile, bool doCompression=false );
	poTexture(int w, int h, int c, const unsigned char* buff);
	~poTexture();

	virtual const char*			getClassName() { return "poTexture"; };
	virtual poObjectModifier*	duplicate() { return new poTexture(); };
	
	void	registerVariables();

	virtual void			init();
	virtual void			initWithObject( poObject* obj );
	
	virtual void setup(poObject* obj);
	virtual void setdown(poObject* obj);
	void draw(float x, float y, float scaler);

	void loadImage(int w, int h, int c, const unsigned char* buff);
	static GLenum determineFormat(int numComponents);

	GLenum format;

	int bindID;
	int width, height;
	int components;
	
	std::string				imageFileName;
	poTextureScaleOption	texturePlacement;
	poImage*				loadedImage;
	bool					doCompression;
	poBlendingMode			blendingMode;
};



#endif