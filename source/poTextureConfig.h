#pragma once



// all texture option should be set here
class poTextureConfig {
public:
	poTextureConfig();
	poTextureConfig(GLenum format);
	
	poTextureConfig &setFormat(GLenum f)			{format = f; return *this;};
	poTextureConfig &setInternalFormat(GLenum f)	{internalFormat = f; return *this;}
	poTextureConfig &setType(GLenum f)				{type = f; return *this;}
	poTextureConfig &setMinFilter(GLenum f)			{minFilter = f; return *this;}
	poTextureConfig &setMagFilter(GLenum f)			{magFilter = f; return *this;}
	poTextureConfig &setWrapS(GLenum f)				{wrapS = f; return *this;}
	poTextureConfig &setWrapT(GLenum f)				{wrapT = f; return *this;}
	
	GLenum format, internalFormat, type, minFilter, magFilter, wrapS, wrapT;
};
