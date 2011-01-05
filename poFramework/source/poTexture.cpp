#include "poTexture.h"
#include "poFBO.h"
#include "poResourceStore.h"
#include "poShape2D.h"
#include "poShape3D.h"
#include "poShapePrimitives.h"


#ifdef WIN32
#define TEXTURE_BIND_METHOD GL_TEXTURE_2D
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_COMPRESSED_RGB 0x84ED
#define GL_COMPRESSED_RGBA 0x84EE

#else
#define TEXTURE_BIND_METHOD GL_TEXTURE_RECTANGLE_EXT
#endif

poTexture::poTexture()
:	bindID(-1), width(0), height(0), components(0)
{
	imageFileName = "";
	texturePlacement = PO_TEX_OBJ_RESHAPE;
	loadedImage = NULL;
	doCompression = false;
	blendingMode = PO_NORMAL_BLENDING;
	registerVariables();
}

poTexture::poTexture( char* imageFile, bool doCompression )
:	bindID(-1), width(0), height(0), components(0)
{
	imageFileName = imageFile;
	texturePlacement = PO_TEX_OBJ_RESHAPE;
	this->doCompression = doCompression;
	loadedImage = poImageStore::get( imageFile );
	if ( loadedImage->loadSuccess )
		loadedImage->loadIntoTexture( this );
	blendingMode = PO_NORMAL_BLENDING;
	registerVariables();
}

int textureMemoryCount = 0;
int compressedTextureMemoryCount = 0;
int	rawTextureMemoryCount = 0;

poTexture::poTexture(int w, int h, int c, const unsigned char* buff)
:	bindID(-1), width(0), height(0), components(0)
{
	loadImage(w, h, c, buff);
	imageFileName = "";
	loadedImage = NULL;
	texturePlacement = PO_TEX_OBJ_RESHAPE;
	doCompression = false;
	blendingMode = PO_NORMAL_BLENDING;
	registerVariables();
}

void	poTexture::registerVariables()
{
	registry.addSeparator( getClassName() );
	registry.add("fileName", &imageFileName);
	registry.add("placement", (int*) &texturePlacement);
}

void	poTexture::init()
{
	if ( imageFileName != "" )
	{
		poImage* Im = poImageStore::get( imageFileName.c_str() );
		Im->loadIntoTexture( this );
	}
}

void	poTexture::initWithObject( poObject* obj )
{
	// try as poShape2D
	poShape2D* shape2D = dynamic_cast<poShape2D*>(obj);
	if ( shape2D )
	{
		shape2D->placeTexture( this, texturePlacement );
		
		if ( width <= 1 || height <= 1 )
		{
			shape2D->fillColor = poColor::red;		// error placement
			printf("ERROR: poTexture::initWithObject, 0 sized image !!!!!!!!!!\n");
			return;
		}
		else if ( texturePlacement == PO_TEX_OBJ_RESHAPE )
		{
			if ( strcmp(obj->getClassName(), "poRectShape")==0 )
			{
				poRectShape* R = (poRectShape*) shape2D;
				R->reshape( width, height );
				R->placeTexture( this, PO_TEX_NO_FIT );
			}
			if ( strcmp(obj->getClassName(), "poOvalShape")==0 )
			{
				poOvalShape* R = (poOvalShape*) shape2D;
				R->reshape( width, height );
				R->placeTexture( this, PO_TEX_NO_FIT );
			}
		}
	}
	
	// try as poShape3D
	poShape3D* shape3D = dynamic_cast<poShape3D*>(obj);
	if ( shape3D )
	{
		shape3D->placeTexture( this, texturePlacement );
	}
	
}


poTexture::~poTexture()
{
	if ( bindID != -1 )
		glDeleteTextures( 1, (GLuint*) &bindID );
}

void poTexture::draw(float x, float y, float scale)
{
#ifdef WIN32
	glEnable( GL_TEXTURE_2D );
	glBindTexture(TEXTURE_BIND_METHOD, bindID);	
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);			glVertex2f(x, y+height*scale);
	glTexCoord2f(1, 0);			glVertex2f(x+width*scale, y+height*scale);
	glTexCoord2f(1, 1);			glVertex2f(x+width*scale, y);
	glTexCoord2f(0, 1);			glVertex2f(x, y);
	glEnd();
	glBindTexture(TEXTURE_BIND_METHOD, 0);
	glDisable( GL_TEXTURE_2D );
#else
	glEnable( TEXTURE_BIND_METHOD );
	glBindTexture(TEXTURE_BIND_METHOD, bindID);	
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);				glVertex2f(x, y+height*scale);
	glTexCoord2f(width, 0);			glVertex2f(x+width*scale, y+height*scale);
	glTexCoord2f(width, height);	glVertex2f(x+width*scale, y);
	glTexCoord2f(0, height);		glVertex2f(x, y);
	glEnd();
	glBindTexture(TEXTURE_BIND_METHOD, 0);
	glDisable( TEXTURE_BIND_METHOD );
#endif
}

GLenum poTexture::determineFormat(int numComponents)
{
	GLenum rightFormat;

	switch(numComponents)
	{
	case 1: rightFormat = GL_ALPHA; break;
	case 2: rightFormat = GL_LUMINANCE_ALPHA; break;
	case 3: rightFormat = GL_RGB; break;
	case 4: rightFormat = GL_RGBA; break;
	}
	
	return rightFormat;
}

void poTexture::setup(poObject* obj) {
	glEnable( TEXTURE_BIND_METHOD );
	glBindTexture(TEXTURE_BIND_METHOD, bindID);	
	
	// set special blending mode
	if ( blendingMode == PO_NORMAL_BLENDING )
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	else if ( blendingMode == PO_SCREEN_BLENDING )
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	else if ( blendingMode == PO_MULTIPLY_BLENDING )
		glBlendFunc( GL_ZERO, GL_SRC_COLOR );
}

void poTexture::setdown(poObject* obj) {
	glBindTexture(TEXTURE_BIND_METHOD, 0);
	glDisable( TEXTURE_BIND_METHOD );
	
	// return to normal blending mode
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void poTexture::loadImage(int w, int h, int c, const unsigned char* buff)
{
	width = w;
	height = h;
	components = c;

	if ( bindID == -1 )
	{
		textureMemoryCount += w*h*c;
		if ( doCompression )
			compressedTextureMemoryCount += w*h*c;
		else
			rawTextureMemoryCount += w*h*c;
	}
	//printf("textureMemoryCount %d r:%d c:%d kb\n", textureMemoryCount/1024, rawTextureMemoryCount/1024, compressedTextureMemoryCount/1024  );
	
	format = determineFormat(components);

	// gl assumes all images are packed in 4 byte pixels
	// must set this to allow for 3 byte and 4 byte alignment
	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glEnable(TEXTURE_BIND_METHOD);
	if ( bindID == -1 )
		glGenTextures(1, (GLuint*)&bindID);
	glBindTexture(TEXTURE_BIND_METHOD, bindID);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	// printf("TEXTURE_BIND_METHOD %d   format %d   width %d   height %d   channels %d   buff %d\n", TEXTURE_BIND_METHOD, format, width, height, c, buff );

	// compress image
	if( doCompression )
	{
		printf( "image compression\n" );
		if( format == GL_RGB){
			glTexImage2D(TEXTURE_BIND_METHOD, 0, GL_COMPRESSED_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, buff);
		}else if( format == GL_RGBA ){
			glTexImage2D(TEXTURE_BIND_METHOD, 0, GL_COMPRESSED_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, buff);
		}else{
			glTexImage2D(TEXTURE_BIND_METHOD, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buff);
		}
	}else
		glTexImage2D(TEXTURE_BIND_METHOD, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buff);


	glBindTexture(TEXTURE_BIND_METHOD, 0);
	glDisable( TEXTURE_BIND_METHOD );
}