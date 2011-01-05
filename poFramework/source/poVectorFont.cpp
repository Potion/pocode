/*
 *  poVectorFont.cpp
 *  poTestApp
 *
 *  Created by Jared Schiffman on 7/21/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#include "poVectorFont.h"
#include <freetype/ftoutln.h>

FT_Library*		poVectorFont::library = NULL;
poShape2D*		letterShape = NULL;

int move_to(const FT_Vector *to, void *user);
int line_to(const FT_Vector *to, void *user);
int conic_to(const FT_Vector *control, const FT_Vector *to, void *user);
int cubic_to(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, void *user);

poVectorFont::poVectorFont( const char* filename, int size, int faceidx, int charmap )
{
	// general font loading and setup
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
	
	//FT_Set_Charmap(face, face->charmaps[0]);
	//FT_Set_Charmap(face, face->charmaps[1]);
	FT_Select_Charmap(face, FT_ENCODING_APPLE_ROMAN );
	
	
	// individual letter construction
	for( int i=0; i<256; i++ )
	{
 		letterShapeSet[i] = new poShape2D();
		letterShape = letterShapeSet[i];
		
		FT_Outline *outline;
		FT_Error    error;
		
		FT_UInt glyph_index = FT_Get_Char_Index(face, i);
		if( glyph_index == 0 )
			continue;
		
		
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if ( error != 0 )
			continue;
		
		assert(face->glyph->format == FT_GLYPH_FORMAT_OUTLINE);
		
		outline = &face->glyph->outline;
		//printf("n_contours = %d\n", outline->n_contours);
		//printf("n_points   = %d\n", outline->n_points);
		
		FT_Outline_Funcs outline_funcs = { move_to, line_to, conic_to, cubic_to, 0, 0 };
		FT_Outline_Decompose(outline, &outline_funcs, NULL);

		letterShape->doTessellation();
		letterShape->fillColor.set( 1,1,1 );
		letterShape->strokeWidth = 0;
		
		FT_Glyph_Metrics metrics = face->glyph->metrics;
		metrics.width = metrics.width >> 6;
		metrics.height = metrics.height >> 6;
		metrics.horiBearingX = metrics.horiBearingX >> 6;
		metrics.horiBearingY = metrics.horiBearingY >> 6;
		metrics.horiAdvance = metrics.horiAdvance >> 6;
		letterMetricsSet[i] = metrics;
	}
}


void	poVectorFont::draw()
{
	float pX = 0, pY = 0;
	for( int i=0; i<256; i++ )
	{
		glPushMatrix();
		glTranslatef( pX, pY, 0 );
		letterShapeSet[i]->draw();
		glPopMatrix();
		
		pX += 30;
		if ( pX > 470 )
		{
			pX = 0;
			pY -= 30;
		}
	}
}


void	poVectorFont::drawChar(char c, float x, float y, float scale )
{
	unsigned char C = c;
	
	glPushMatrix();
	float descender = letterMetricsSet[C].height - letterMetricsSet[C].horiBearingY;
	glTranslatef( x, y-descender*scale, 0);

	letterShapeSet[C]->draw();
	
	glPopMatrix();
}


int	poVectorFont::getCharWidth(char C)
{
	unsigned char c = C;
	
	//	MAKING SPECIAL CASE FOR TAB
	if(c == '\t')
		return letterMetricsSet[' '].horiAdvance * 4;
	else if(c == ' ')
		return letterMetricsSet[' '].horiAdvance * 1.0;
	
	if( letterShapeSet[c]->pointList.size()==0 )
	{
		printf("WARNING: poBitmapFont->getCharWidth: character (%c, %i) out of bounds\n", c, c);
		return 0;
	}
	
	return letterMetricsSet[c].horiAdvance;
}

int	poVectorFont::getSpacing(char Left, char Right)
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
		
		return kern * 1.5;
	}
	
	return 0;
}


// ============= C callbacks for outline evaluation ===================== //

#define DOUBLE_TO_16_16(d) ((FT_Fixed)((d) * 65536.0))
#define DOUBLE_FROM_26_6(t) ((double)(t) / 64.0)

void print4bytes(const char *bytes)
{
	int i;
	
	for (i = 0; i < 4; i++)
		printf("%02X ", bytes[i]);
	
	printf("\n");
}

int move_to(const FT_Vector *to, void *user)
{
	/*print4bytes((char *) &(to->x));
	print4bytes((char *) &(to->y));
	printf("move_to([%g, %g])\n",
		   DOUBLE_FROM_26_6(to->x), DOUBLE_FROM_26_6(to->y));*/
	
	letterShape->addPoint( DOUBLE_FROM_26_6(to->x), DOUBLE_FROM_26_6(to->y) );
	letterShape->pointList.back().mark = 100;		// begin new countour
	return 0;
}

int line_to(const FT_Vector *to, void *user)
{
	/*printf("line_to([%g, %g])\n",
		   DOUBLE_FROM_26_6(to->x), DOUBLE_FROM_26_6(to->y));*/

	letterShape->addPoint( DOUBLE_FROM_26_6(to->x), DOUBLE_FROM_26_6(to->y) );
	return 0;
}

int conic_to(const FT_Vector *control, const FT_Vector *to, void *user)
{
	/*printf("conic_to(c = [%g, %g], to = [%g, %g])\n",
		   DOUBLE_FROM_26_6(control->x), DOUBLE_FROM_26_6(control->y),
		   DOUBLE_FROM_26_6(to->x), DOUBLE_FROM_26_6(to->y));*/
	
	poPoint A = letterShape->pointList.back();
	poPoint B = poPoint(DOUBLE_FROM_26_6(control->x), DOUBLE_FROM_26_6(control->y));
	poPoint C = poPoint(DOUBLE_FROM_26_6(to->x), DOUBLE_FROM_26_6(to->y));
	for( float t=0; t<=1.0001; t+= 0.1 )
	{
		float invt = 1.0 - t;
		poPoint P;
		P.x = invt*invt*A.x + 2*t*invt*B.x + t*t*C.x;
		P.y = invt*invt*A.y + 2*t*invt*B.y + t*t*C.y;
		P.z = 0;
		letterShape->addPoint( P.x, P.y );
	}
	
	return 0;
}

int cubic_to(const FT_Vector *control1, const FT_Vector *control2,
			 const FT_Vector *to, void *user)
{
	/*printf("cubic_to(c1 = [%g, %g], c2 = [%g, %g], to = [%g, %g])\n",
		   DOUBLE_FROM_26_6(control1->x), DOUBLE_FROM_26_6(control1->y),
		   DOUBLE_FROM_26_6(control2->x), DOUBLE_FROM_26_6(control2->y),
		   DOUBLE_FROM_26_6(to->x), DOUBLE_FROM_26_6(to->y));*/
	
	poPoint A = letterShape->pointList.back();
	poPoint B = poPoint(DOUBLE_FROM_26_6(control1->x), DOUBLE_FROM_26_6(control1->y));
	poPoint C = poPoint(DOUBLE_FROM_26_6(control2->x), DOUBLE_FROM_26_6(control2->y));
	poPoint D = poPoint(DOUBLE_FROM_26_6(to->x), DOUBLE_FROM_26_6(to->y));
	for( float t=0; t<=1.0001; t+= 0.1 )
	{
		float invt = 1.0 - t;
		poPoint P;
		P.x = invt*invt*invt*A.x + 3*t*invt*invt*B.x + 3*t*t*invt*C.x + t*t*t*D.x;
		P.y = invt*invt*invt*A.y + 3*t*invt*invt*B.y + 3*t*t*invt*C.y + t*t*t*D.y;
		P.z = 0;
		letterShape->addPoint( P.x, P.y );
	}
	
	return 0;
}


