#include "poBitmapFont.h"
#include "poTexture.h"
#include "poImage.h"
#include "poTextBox.h"
#include "poShapePrimitives.h"

FT_Library* poBitmapFont::library = NULL;

poBitmapFont::poBitmapFont(const char* filename, int size, int faceidx, int charmap)
{
	info.fontName = (char*)malloc(strlen(filename)+1);
	strcpy(info.fontName, filename);
	info.fontSize = size;
	info.fontIndex = faceidx;
	info.charSet = charmap;

	if(library == NULL)
	{
		library = (FT_Library*)malloc(sizeof(FT_Library));
		FT_Init_FreeType(library);
	}
	
	if ( FT_New_Face(*library, filename, faceidx, &face) != 0)
	{
		printf("ERROR: poBitmapFont: Could not open font %s\n", filename );
		exit(0);
	}
	
	FT_Set_Pixel_Sizes(face, 0, size);
	
	ascender = face->size->metrics.ascender >> 6;
	descender = face->size->metrics.descender >> 6;
	height = face->size->metrics.height >> 6;

	selectCharMap(charmap);

	construct();
}

poBitmapFont::~poBitmapFont()
{
	FT_Done_Face(face);
}

void poBitmapFont::selectCharMap(int mapIdx)
{
//	FT_CharMap found = 0; 
	FT_CharMap charmap; 
	for ( int n = 0; n < face->num_charmaps; n++ ){ 
		charmap = face->charmaps[n];
		printf( "FONT: %s : platform_id: %d  encoding_id: %d\n", info.fontName, charmap->platform_id, charmap->encoding_id );
	}
	FT_Set_Charmap( face, face->charmaps[3] );
	//FT_Select_Charmap(face,  FT_ENCODING_APPLE_ROMAN );
}

void poBitmapFont::addAFMfile(const char* afmfile)
{
	FT_Attach_File(face, afmfile);
}

void poBitmapFont::drawChar(char C, float x, float y, float scale )
{
	unsigned char c = C;
	
	if(charmap[c].image == NULL)
	{
		//printf("WARNING: poBitmapFont->drawChar: character (%c, %i) out of bounds\n", c, c);
		return;
	}

	poBitmapGlyph& glyph = charmap[c];	
	float descender = glyph.metrics.height - glyph.metrics.horiBearingY;
	glyph.tex->draw( x+glyph.metrics.horiBearingX*scale,  y-descender*scale, scale);
}


void poBitmapFont::drawString(const char* str, float x, float y, float scale)
{
	int len = strlen(str);
	float penx = x;
	float kern = 0;
	for(int i=0; i<len; i++) 
	{
		char c = str[i];
		if(i > 0) 
			kern = getSpacing(str[i-1], c);
		drawChar(c, penx, y, scale);
		penx += getCharWidth(c) + kern;
	}
}


int poBitmapFont::getCharWidth(char C)
{
	unsigned char c = C;
	
	//	MAKING SPECIAL CASE FOR TAB
	if(c == '\t')
		return charmap[' '].metrics.horiAdvance * 4;
	else if(c == ' ')
		return charmap[' '].metrics.horiAdvance * 1.0;

	if(charmap[c].image == NULL)
	{
		//printf("WARNING: poBitmapFont->getCharWidth: character (%c, %i) out of bounds\n", c, c);
		return 0;
	}
	
	poBitmapGlyph glyph = charmap[c];
	return glyph.metrics.horiAdvance;
}

int poBitmapFont::getStringWidth(const char* str)
{
	if ( str==NULL )
		return 0;
	
	int len = strlen(str);
	float penx = 0;
	float kern = 0;
	for(int i=0; i<len; i++) 
	{
		char c = str[i];
		if(i > 0) 
			kern = getSpacing(str[i-1], c);
		penx += getCharWidth(c) + kern;
	}
	
	return penx;
}

int poBitmapFont::getSpacing(char Left, char Right)
{
	unsigned char left = Left;
	unsigned char right = Right;
		
	if(FT_HAS_KERNING(face))
	{
		int glyph1, glyph2;
		glyph1 = FT_Get_Char_Index(face, left);
		glyph2 = FT_Get_Char_Index(face, right);
		
		if( glyph1==0 || glyph2==0 )
			return 0;
		
		FT_Vector delta;
		FT_Get_Kerning(face, glyph1, glyph2, 0, &delta);
		int kern = delta.x >> 6;

		if ( kern > 0 )
			return kern * 1.5;
	}
	
	return poFont::getSpacing(left, right);
}

int poBitmapFont::getLeading()
{
	return height;
}

int poBitmapFont::getAscender() 
{
	return ascender;
}

int poBitmapFont::getDescender()
{
	return descender;
}

void poBitmapFont::construct()
{
	for(int i=9; i<=255; i++)
	{
		if ( FT_Load_Char(face, i, FT_LOAD_RENDER) != 0 ) {
			//printf("counldn't make %d %c\n", i, i);
			continue;
		}
		
		FT_Glyph_Metrics metrics = face->glyph->metrics;
		metrics.width = metrics.width >> 6;
		metrics.height = metrics.height >> 6;
		metrics.horiBearingX = metrics.horiBearingX >> 6;
		metrics.horiBearingY = metrics.horiBearingY >> 6;
		metrics.horiAdvance = metrics.horiAdvance >> 6;

		int pitch = face->glyph->bitmap.width;
		while(pitch%4) pitch++;

		char* buffer = (char*)malloc(pitch * face->glyph->bitmap.rows);
		memset( buffer, 0, pitch * face->glyph->bitmap.rows );
		
		for(int y=0; y<face->glyph->bitmap.rows; y++)
			for(int x=0; x<face->glyph->bitmap.width; x++)
				buffer[y*pitch+x] = face->glyph->bitmap.buffer[y * face->glyph->bitmap.pitch + x];
		
		poImage* image = new poImage(buffer, pitch, face->glyph->bitmap.rows, 1);
		free(buffer);

		poTexture* tex = image->getTexture();
		poBitmapGlyph glyph;
		glyph.image = image;
		glyph.tex = tex;
		glyph.metrics = metrics;
		charmap[i] = glyph;
	}
}

void poBitmapFont::layoutFont(float x, float y)
{
	float penx=0, peny=0;
	
	glColor3f( 0,0,0 );
	glRectf( x,y, x+500,y-1 );

	for(int i=32; i<=126; i++)
	{
		drawChar(i, penx, peny);
		penx += getCharWidth(i);
		
		if(penx > 500)
		{
			penx = 0;
			int leading = (ascender - descender) + 40;
			peny -= leading;
		}
	}
}

void poBitmapFont::remapChar(int from, int to)
{
	charmap[from] = charmap[to-1];
}


poObject* poBitmapFont::getCharMapObj()
{
	poObject* charMapObj = new poObject();
	
	// draw row
	for( int i=0; i<= 20*2; i++ )
	{
		poLineShape* L = new poLineShape( poPoint(i*40, 0), poPoint(i*40,600*2) );
		L->strokeColor = poColor::white;
		charMapObj->addChild( L );
	}
	// draw column
	for( int j=0; j<= 15*2; j++ )
	{
		poLineShape* L = new poLineShape( poPoint(0, j*40), poPoint(800*2,j*40) );
		L->strokeColor = poColor::white;
		charMapObj->addChild( L );
	}
	
	// add numbers
	for( int i=0; i< 20; i++ )
	{
		for( int j=0; j< 15; j++ )
		{
			poTextBox* B1 = new poTextBox( "", 100,100, poColor::white );
			char t [10];
			sprintf( t, "%d", j*20+i );
			B1->setText( t );
			B1->makeStaticTexture();
			B1->position.set( i*40, 40*15-j*40 );
			charMapObj->addChild( B1 );
		}
	}
	
	// add characters
	for( int i=0; i< 20*2; i++ )
	{
		for( int j=0; j< 15*2; j++ )
		{
			int N = j*20 + i;
			char t [ 10 ];
			sprintf( t, "%c", N );
			poTextBox* B2 = new poTextBox( "", 100,100, poColor::white );
			B2->setFont( this );
			B2->setText( t );
			B2->makeStaticTexture();
			B2->position.set( i*40+15, 40*2*15-j*40-15 );
			charMapObj->addChild( B2 );
		}
	}
	
	return charMapObj;
}