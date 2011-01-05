/*
 *  poTextBox.cpp
 *  potionApp
 *
 *  Created by Jared Schiffman on 9/14/07.
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 *
 */

#include "poTextBox.h"
#include "poBitmapFont.h"
#include "poTexture.h"
#include <boost/regex.hpp>

poTextBox::poTextBox()
:	poObject()
{
	construct("", 100, 100);
}

poTextBox::poTextBox(int width, int height)
:	poObject()
{
	construct("", width, height);
}

poTextBox::poTextBox(const char* text, int width, int height)
:	poObject()
{
	construct(text, width, height);
}

poTextBox::poTextBox(const char* text, int width, int height, const poColor& fontColor )
:	poObject()
{
	construct(text, width, height);
	setFontColor( fontColor );
}

poTextBox::poTextBox(const char* text, int width, int height, const poColor& fontColor, const poColor& fillColor, int strokeWidth, const poColor& strokeColor )
:	poObject()
{
	construct(text, width, height);
	setFontColor( fontColor );
	this->enableFill = true;
	this->fillColor = fillColor;
	this->strokeWidth = strokeWidth;
	this->strokeColor = strokeColor;
	italicFont = NULL;
	boldFont = NULL;
}
	
poTextBox::~poTextBox()
{
	free(cleanedText);
	
	if ( staticTexture != NULL )
		delete staticTexture;
	if ( staticImage != NULL )
		delete staticImage;
}

poObject* poTextBox::duplicate(bool copyChildren, poObject* copyInto)
{
	poTextBox* obj = (poTextBox*)copyInto;
	if(obj == NULL)
		obj = new poTextBox();
		
	obj->width = width;
	obj->height = height;
	obj->setAlign(hAlign, vAlign);
	obj->setOrigin(hOrigin, vOrigin);
	obj->setFont(font);
	obj->setText(text.c_str());
	obj->setFontColor(fontColor);
	obj->setFontScale(fontScale);
	obj->setFontTracking(fontTracking);
	obj->enableFill = enableFill;
	obj->fillColor = fillColor;
	obj->strokeColor = strokeColor;
	obj->strokeWidth = strokeWidth;
	
	poObject::duplicate(copyChildren, obj);
	return obj;
}

void poTextBox::init()
{
	poObject::init();

	poFont* newFont = poFontStore::get(fontInfo.fontName, fontInfo.fontSize, fontInfo.fontIndex, fontInfo.charSet);
	setFont(newFont);
	setFontColor(fontColor);
	setFontTracking(fontTracking);
	setFontScale(fontScale);
	setText(text.c_str());
	
	doLayout();
}

void poTextBox::construct(const char* txt, int width, int height)
{
	text = "";
	cleanedText = NULL;
	
	this->width = width;
	this->height = height;
	boundingBox.left = 0;
	boundingBox.right = width;
	boundingBox.bottom = -height;
	boundingBox.top = 0;
	
	hAlign = PO_ALIGN_LEFT;
	vAlign = PO_ALIGN_TOP;
	hOrigin = PO_ALIGN_LEFT;
	vOrigin = PO_ALIGN_TOP;
	
	font = getDefaultFont();
	fontLeading = font->getLeading();
	fontInfo = font->info;
	fontTracking = 1.0f;
	fontScale = 1.0f;
	fontColor = poColor::black;
		
	enableFill = false;
	strokeWidth = 0;
	fillColor = poColor::white;
	strokeColor = poColor::black;
	
	staticTexture = NULL;
	staticImage = NULL;
	
	setText( txt );

	setName(getClassName());
	registry.addSeparator( getClassName() );
	registry.add("text", &text);
	registry.add("fontLeading", &fontLeading);
	registry.add("width", &this->width);
	registry.add("height", &this->height);
	registry.add("hAlign", (int*)&hAlign);
	registry.add("vAlign", (int*)&vAlign);
	registry.add("hOrigin", (int*)&hOrigin);
	registry.add("vOrigin", (int*)&vOrigin);
	registry.add("fontColor", &fontColor);
	registry.add("fontTracking", &fontTracking);
	registry.add("fontScale", &fontScale);
	registry.add("font", &fontInfo);
	registry.add("enableFill", &enableFill );
	registry.add("fillColor", &fillColor );
	registry.add("strokeColor", &strokeColor );
	registry.add("strokeWidth", &strokeWidth );
}


void poTextBox::draw()
{
	drawFrame();
	
	glAlphaFunc(GL_GREATER,0.01);
	glEnable(GL_ALPHA_TEST);
	
	
	if ( staticTexture != NULL )
	{		// special static texture drawing method
		glColor4f( fontColor.R, fontColor.G, fontColor.B, fontColor.A*masterAlphaControl);
		staticTexture->draw( boundingBox.left, boundingBox.top - staticTexture->height, 1 );
	}
	else	// normal drawing methord
	{
		for(int i=0; i<letterList.size(); i++)
		{
			poTBLetter& letter = letterList[i];
			glColor4f(letter.color.R, letter.color.G, letter.color.B, letter.color.A*masterAlphaControl);
			letter.font->drawChar(letter.c, letter.position.x, letter.position.y, letter.scale );
		}
	}
	
	glDisable(GL_ALPHA_TEST);
	
	if ( poObject::doDebugDraw )
	{
		debugDraw();
		poObject::debugDraw();
	}
}


void	poTextBox::drawFrame()
{
	if ( enableFill && fillColor.A > 0.01 )
	{
		glColor4f(fillColor.R, fillColor.G, fillColor.B, fillColor.A*masterAlphaControl);	
		glRectf(boundingBox.left, boundingBox.bottom, boundingBox.right, boundingBox.top );
	}
	
	if ( strokeWidth != 0 && strokeColor.A > 0.01 )
	{
		glLineWidth( strokeWidth );
		glColor4f(strokeColor.R, strokeColor.G, strokeColor.B, strokeColor.A*masterAlphaControl);
		glBegin(GL_LINE_LOOP);
		glVertex2f(boundingBox.left, boundingBox.bottom);
		glVertex2f(boundingBox.right, boundingBox.bottom);
		glVertex2f(boundingBox.right, boundingBox.top);
		glVertex2f(boundingBox.left, boundingBox.top);
		glEnd();
		glLineWidth( 1 );
	}
}


void poTextBox::debugDraw()
{
	glColor4f( 0,1,0,0.5 );
	glBegin(GL_LINE_LOOP);
	glVertex2f(boundingBox.left, boundingBox.bottom);
	glVertex2f(boundingBox.right, boundingBox.bottom);
	glVertex2f(boundingBox.right, boundingBox.top);
	glVertex2f(boundingBox.left, boundingBox.top);
	glEnd();
	
	glColor3f( 0,1,0 );
	glBegin(GL_LINE_LOOP);
	glVertex2f(actualTextBounds.left, actualTextBounds.bottom);
	glVertex2f(actualTextBounds.right, actualTextBounds.bottom);
	glVertex2f(actualTextBounds.right, actualTextBounds.top);
	glVertex2f(actualTextBounds.left, actualTextBounds.top);
	glEnd();

	poObject::debugDraw();
}

void poTextBox::update()
{}

void poTextBox::setAlign(poHoriOrientation hAlign, poVertOrientation vAlign)
{
	this->hAlign = hAlign;
	this->vAlign = vAlign;
}

void poTextBox::setOrigin(poHoriOrientation ho, poVertOrientation vo)
{
	this->hOrigin = ho;
	this->vOrigin = vo;
}

void poTextBox::postFormat()
{	
	if( italicFont ){
		bool	italicOn = false;
		for( int i=0; i<letterList.size(); i++ )
		{
			if ( detectAndRemoveSequence( "#i#", i ) )
				italicOn = !italicOn;
			if ( detectAndRemoveSequence( "#/i#", i ) )
				italicOn = false;
			
			if ( italicOn )
					letterList[i].font = italicFont;
		}
	}
	
	if( boldFont ){
		bool	boldOn = false;
		for( int i=0; i<letterList.size(); i++ )
		{
			if ( detectAndRemoveSequence( "#b#", i ) )
				boldOn = !boldOn;
			if ( detectAndRemoveSequence( "#/b#", i ) )
				boldOn = false;
			
			if ( boldOn )
					letterList[i].font = boldFont;
		}
	}
	
}

bool	poTextBox::detectAndRemoveSequence( const char* sequence, int N )
{
	int length = strlen(sequence);
	
	// can't fit at end
	if ( N+length >= letterList.size() )
		return false;
	
	// check for sequence
	for( int i=0; i<length; i++ )
	{
		if ( letterList[N+i].c != sequence[i] )
			return false;
	}
	
	letterList.erase( letterList.begin()+N, letterList.begin()+N+length );
	return true;
}

void	poTextBox::makeStaticTexture( bool buildImage )
{	
	// check that we're using bitmap fonts
	poBitmapFont* fontCheckFont = dynamic_cast<poBitmapFont*>(font);
	if ( fontCheckFont == NULL )
	{
		printf("ERROR: poTextBox::makeStaticTexture cannot be used with non poBitmapFonts\n");
		exit(0);
	}
	
	// doLayout() just in case
	doLayout();
	
	// determine full texture size
	poBoundingBox bBox = getActualBounds();
	int texW = bBox.right - bBox.left;
	int texH = bBox.top - bBox.bottom;
	
	if ( texW < boundingBox.right - boundingBox.left )
		texW = boundingBox.right - boundingBox.left;

	// make power of two textur size
	/*int potTexW = 2;
	int potTexH = 2;
	while( potTexW < texW )
		potTexW *= 2;
	while( potTexH < texH )
		potTexH *= 2;
	texW = potTexW;
	texH = potTexH;*/
	
	// determine textOrigin
	poPoint textOrigin;
	if(vOrigin == PO_ALIGN_TOP)
		textOrigin.y = 0;
	else if(vOrigin == PO_ALIGN_BOTTOM)
		textOrigin.y = (boundingBox.top-boundingBox.bottom);
	else if(vOrigin == PO_ALIGN_VERT_CENTER)
		textOrigin.y = (boundingBox.top-boundingBox.bottom)/2;
	
	if( hOrigin == PO_ALIGN_LEFT)
		textOrigin.x = 0;
	else if(hOrigin == PO_ALIGN_RIGHT)
		textOrigin.x = (boundingBox.right-boundingBox.left);
	else if(hOrigin == PO_ALIGN_HORI_CENTER)
		textOrigin.x = (boundingBox.right-boundingBox.left)/2;
		
	// allocate and clear image buffer
	/*printf("makeStaticTexture %d %d ", texW, texH  );
	for( int i=0; i<letterList.size() && i < 30; i++ )
		printf("%c", letterList[i].c);
	printf("\n");*/

	if( !staticImage ){
		staticImage = new poImage( texW, texH, 1 );
	}
	
	// place each letter
	for(int n=0; n<letterList.size(); n++)
	{
		poTBLetter& letter = letterList[n];
		
		unsigned char c = letter.c;
		//poBitmapGlyph& glyph = ((poBitmapFont*)font)->charmap[c];
		poBitmapGlyph& glyph = ((poBitmapFont*)(letter.font))->charmap[c];
		poImage* image = glyph.image;
		if(image == NULL)
			continue;
		
		float descender = glyph.metrics.height - glyph.metrics.horiBearingY;
		float transX = letter.position.x + glyph.metrics.horiBearingX;
		float transY = -1*letter.position.y + descender - image->height;
		transX += textOrigin.x;
		transY += textOrigin.y;
		
		// pixel by pixel for each letter
		for( int i=0; i<image->width; i++ )
		{
			for( int j=0; j<image->height; j++ )
			{
				int tx = transX + i;
				int ty = transY + j;
				if ( tx<0 || tx>=texW || ty<0 || ty>=texH )
					continue;
				//buff[ ty*texW+tx ] |= image->getSubPixel(i,j,0);
				staticImage->setPixel( tx, ty, 0, image->getSubPixel(i,j,0) );
			}
		}
	}

	// make the texture
	if ( staticTexture == NULL )
	{
		staticTexture = new poTexture();
		staticImage->loadIntoTexture( staticTexture );
	}
	else
		staticImage->loadIntoTexture( staticTexture );
	
	if( !buildImage && staticImage ){
		delete staticImage;
	}	
}

void poTextBox::updateStaticTexture()
{
	if( staticImage && staticTexture )
		staticImage->loadIntoTexture( staticTexture );
}

bool poTextBox::pointInside(float x, float y)
{
	if( !visible || matrixSet.isZero() )
		return false;
	
    poPoint P;
    P.set( x, y, 0 );
	globalToLocal(P.x, P.y, P.z);

	// test point inside bounding box
    if ( P.x >= boundingBox.left && P.x <= boundingBox.right && P.y <= boundingBox.top && P.y >= boundingBox.bottom )
		return true;
    
    return false;

}

void poTextBox::setText(const char* txt)
{
	text = txt;

	// clean input
	cleanedText = (char*)realloc( cleanedText, text.size()+1);
	strcpy(cleanedText, text.c_str());
	cleanedText = cleanInput(cleanedText);

	// initialzes letter list
	letterList.clear();
	int len = strlen(cleanedText);
	letterList.resize(len);
	for(int i=0; i<len; i++)
	{
		poTBLetter letter;
		letter.c = cleanedText[i];
		letter.font = font;
		letter.color = fontColor;
		letter.scale = fontScale;
		letter.tracking = fontTracking;
		letterList[i] = letter;
	}
}

void poTextBox::appendText( const char* txt )
{
	setText(std::string(text + txt).c_str());
}

void normalizeRange(poTBLetterVector txt, int& s, int& e)
{
	int textLen = txt.size();
	
	if(s == -1)
		s = 0;
	
	if(e == -1 || e > textLen)
		e = textLen;
	
	if(e < s) 
	{
		int tmp = s;
		s = e;
		e = tmp;
	}
}


void poTextBox::resetFont()
{
	for(int i=0; i<letterList.size(); i++) 
	{
		letterList[i].font = font;
		letterList[i].color = fontColor;
		letterList[i].tracking = fontTracking;
		letterList[i].scale = fontScale;
	}
}

void poTextBox::setFont(poFont* aFont, int startIdx, int endIdx)
{
	normalizeRange(letterList,startIdx, endIdx);
	
	if((endIdx-startIdx) == letterList.size()) 
	{
		font = aFont;
		fontLeading = font->getLeading();
		
		fontInfo = font->info;
	}
	
	for(int i=startIdx; i<endIdx; i++)
		letterList[i].font = aFont;
}

void poTextBox::setFontColor(const poColor& color, int startIdx, int endIdx)
{	
	normalizeRange(letterList, startIdx, endIdx);
	
	if((endIdx-startIdx) == letterList.size()) 
		fontColor = color;
		
	for(int i=startIdx; i<endIdx; i++)
		letterList[i].color = color;
}

void poTextBox::setFontTracking(float track, int startIdx, int endIdx)
{
	normalizeRange(letterList, startIdx, endIdx);
	
	if((endIdx-startIdx) == letterList.size()) 
		fontTracking = track;
		
	for(int i=startIdx; i<endIdx; i++)
		letterList[i].tracking = track;
}

void poTextBox::setFontScale(float scale, int startIdx, int endIdx)
{
	normalizeRange(letterList, startIdx, endIdx);

	if((endIdx-startIdx) == letterList.size())
		fontScale = scale;
		
	for(int i=startIdx; i<endIdx; i++)
		letterList[i].scale = scale;
}

void poTextBox::calculateBounds()
{
	// bounding box already set
	
	// poObject::calculateRelativeBounds( relativeObj, bbox );
}


void poTextBox::calculateRelativeBounds( poObject* relativeObj, poBoundingBox& bbox )
{
	poPoint pointList[4];
	pointList[0].set( boundingBox.left, boundingBox.top );
	pointList[1].set( boundingBox.left, boundingBox.bottom );
	pointList[2].set( boundingBox.right, boundingBox.bottom );
	pointList[3].set( boundingBox.right, boundingBox.top );

	for(int i=0; i<4; i++)
	{
		poPoint P = pointList[i];
		if ( getParent()==NULL )
			return;
		localToGlobal( P );
		relativeObj->globalToLocal( P );
		
		if( P.x > bbox.right )
			bbox.right = P.x;
		if( P.x < bbox.left )
			bbox.left = P.x;
		if( P.y > bbox.top )
			 bbox.top = P.y;
		if( P.y < bbox.bottom )
			bbox.bottom = P.y;
	}

	poObject::calculateRelativeBounds( relativeObj, bbox );
}


void poTextBox::reshape( float w, float h )
{
	width = w;
	height = h;
}

void poTextBox::setPadding(float p)
{
	setPadding(p, p, p, p);
}

void poTextBox::setPadding(float h, float v)
{
	setPadding(h, h, v, v);
}

void poTextBox::setPadding(float l, float r, float t, float b)
{
	padding.left = l;
	padding.right = r;
	padding.top = t;
	padding.bottom = b;
}

poFont* poTextBox::getDefaultFont()
{
#ifdef WIN32
	return poFontStore::get("C:\\WINDOWS\\Fonts\\arial.ttf", 12, 0, 0);
#else
	return poFontStore::get("/System/Library/Fonts/Geneva.dfont", 12, 0, 0);
#endif
}

char* poTextBox::cleanInput(char* txt)
{
	using namespace std;
	using namespace boost;

	const char* expressions = "(<br>)|(<tab>)|(&amp;)|(&quot;)";
	const char* replace = "(?1\n)(?2\t)(?3&)(?4\")";

	string strText(txt);
	string strReplaced = regex_replace(strText, regex(expressions), replace, boost::match_default|boost::format_all);

	txt = (char*)realloc(txt, strReplaced.size()+1);
	memcpy( txt, strReplaced.c_str(), strReplaced.size());
	txt[ strReplaced.size() ] = 0; //force a NULL terminator
	return txt;
}

void poTextBox::doLayout()
{	
	// set up bounding box
	boundingBox.left = 0;
	boundingBox.right = width;
	boundingBox.bottom = -height;
	boundingBox.top = 0;
	
	// setup for layout pass
	//	poFont* font = poFontStore::get(regFont, fontSize);
	poBoundingBox innerMargin = boundingBox - padding;

	// set text positions
	int lastSpace = 0;
	int currentLineStart=0, currentLine=0, currentLineWidth=0;
	float penx = innerMargin.left;
	float peny = -font->getAscender() - padding.top;

	int len = letterList.size();
	for(int i=0; i<len; i++)
	{
		poTBLetter* letter = &letterList[i];
		poFont* font = letter->font;
		
		// sometimes we need this, sometimes we don't (ie \n)
		int pixelIncrease = 0;
		
		// processing characters happens in a few steps
		// if its a new line ... start a new line
		if(letterList[i].c == '\n')
		{
			lastSpace = i+1;
		} 
		// if its a tab, indent the current cursor
		else if(letterList[i].c == '\t')
		{
			pixelIncrease = font->getCharWidth(' ') * 4 * letter->scale;
			penx += pixelIncrease * letter->tracking;
			lastSpace = i;
		}
		// else its any old character
		else
		{
			// determine kerning, if any
			int kern = 0;
			if(i > 0) kern = font->getSpacing(letterList[i-1].c, letterList[i].c) * letter->scale;
			if (kern != 0)
				printf("kern %d %c:%c\n", kern, letterList[i-1].c,letterList[i].c );

			// position the current letter based on the pen position

			letter->position.set(penx+kern, peny);
			pixelIncrease = font->getCharWidth(letterList[i].c) * letter->scale + kern;
			penx += pixelIncrease * letter->tracking;
		}
		// bump the current line width
		currentLineWidth += pixelIncrease;
		
		bool needLineBreak = (penx > innerMargin.right && lastSpace != 0) || i == len - 1 || letterList[i].c == '\n';

		// we've exceded the bounds so move the current word down
		// unless we're the first work, in which case leave it alone
		if(needLineBreak)
		{
			penx = innerMargin.left;
			peny -= fontLeading*letter->scale;

			// later we'll need to know how many pixels, if any, we shaved off the current line
			int shavingOff = 0;

			// if we're still processing the first word we can't move it down
			// otherwise its ok to move the current word down to the next line
			if(lastSpace != 0 && i != len-1)
			{
				// first look back to the last space to see if there's a hyphen between there and the end
				// if there is and the left portion + the hyphen is in the bounds, split the word there
				// otherwise move everything down
				for(int j=i; j>=lastSpace; j--)
				{
					int charPixelsRemoved = font->getCharWidth(letterList[j].c) * letter->scale;
					shavingOff += charPixelsRemoved;
					/*
					if(letterList[j].c == '-')
					{
						if(letterList[j].position.x < innerMargin.right)
						{
							lastSpace = j;
							break;
						}
					}
					*/
				}
				// truncate the current line width by the length of the letters you took off
				currentLineWidth -= shavingOff;

				// backtrack to move the current word down
				// this inner loop is a repeat of what happend at the start of the outer loop
				// only the loop is defined by the last space to the end
				for(int j=lastSpace+1; j<=i; j++)
				{
					int backKern = 0;
					if(j > lastSpace+1) backKern = font->getSpacing(letterList[j-1].c, letterList[j].c) * letter->scale;

					poTBLetter* backLetter = &letterList[j];
					backLetter->position.set(penx+backKern, peny);
					//backLetter->color.A = 0.f;
					penx += font->getCharWidth(letterList[j].c) * letter->tracking * letter->scale;
				}
				
				if(letterList[lastSpace].c == ' ')
					shavingOff -= font->getCharWidth(' ') * letter->scale;
			}

			// do horizontal positioning of this line
			int lineOffset = 0;
			if(hAlign == PO_ALIGN_RIGHT)
				lineOffset = innerMargin.right - currentLineWidth;
			else if(hAlign == PO_ALIGN_HORI_CENTER)
				lineOffset = (innerMargin.right - currentLineWidth) / 2;

			// move all characters on this line into horizontal alignment
			int currentLineEnd = (i == len-1) ? i : lastSpace;
			for(int j=currentLineStart; j<=currentLineEnd; j++)
				letterList[j].position.x += lineOffset;

			// and reset the currentLineStart
			currentLineStart = currentLineEnd;
				
			// make sure we reset properly
			currentLine++;

			lastSpace = 0;
			currentLineWidth = shavingOff;
		}
		
		if(letterList[i].c == ' ') 
		{
			lastSpace = i;
		}
	}
	
	
	// go back and calculate the actual text bounds
	actualTextBounds.top = innerMargin.top;
	// sometimes leading will be manually set so don't use font leading
	actualTextBounds.bottom = fontLeading * fontScale * currentLine * -1;
	actualTextBounds.left = 10000000;
	actualTextBounds.right = -10000000;
	poTBLetterVector::iterator letterIter = letterList.begin();
	while(letterIter != letterList.end())
	{
		if(letterIter->position.x < actualTextBounds.left)
			actualTextBounds.left = letterIter->position.x;

		int charWidth = letterIter->font->getCharWidth(letterIter->c) * letterIter->scale;
		if(letterIter->position.x+charWidth > actualTextBounds.right && !isspace(letterIter->c))
			actualTextBounds.right = letterIter->position.x + charWidth;

		letterIter++;
	}
	
	
	// lets do our vertical positioning
	int yoff = 0;
	if(vAlign == PO_ALIGN_BOTTOM)
		yoff = innerMargin.bottom - actualTextBounds.bottom;
	else if(vAlign == PO_ALIGN_VERT_CENTER)
		yoff = (innerMargin.bottom - actualTextBounds.bottom) / 2;
		
	letterIter = letterList.begin();
	while(letterIter != letterList.end())
	{
		letterIter->position.y += yoff;
		letterIter++;
	}
	
	actualTextBounds.top += yoff;
	actualTextBounds.bottom += yoff;
	
	actualTextBounds.top += padding.top;
	actualTextBounds.bottom -= padding.bottom*2;
	actualTextBounds.left -= 0;
	actualTextBounds.right += padding.left + padding.right;
	
	// we're pretty much set so lets move everything into position according to our "origin"
	poPoint newTextOrigin;
	if(vOrigin == PO_ALIGN_TOP)
		newTextOrigin.y = 0;
	else if(vOrigin == PO_ALIGN_BOTTOM)
		newTextOrigin.y = (boundingBox.bottom*-1);
	else if(vOrigin == PO_ALIGN_VERT_CENTER)
		newTextOrigin.y = (boundingBox.top + boundingBox.bottom*-1) / 2;
	
	if(hOrigin == PO_ALIGN_LEFT)
		newTextOrigin.x = 0;
	else if(hOrigin == PO_ALIGN_RIGHT)
		newTextOrigin.x = -boundingBox.right;
	else if(hOrigin == PO_ALIGN_HORI_CENTER)
		newTextOrigin.x = -(boundingBox.left + boundingBox.right) / 2;
	
	letterIter = letterList.begin();
	while(letterIter != letterList.end())
	{
		letterIter->position += newTextOrigin;
		letterIter++;
	}
	
	boundingBox.left += newTextOrigin.x;
	boundingBox.right += newTextOrigin.x;
	actualTextBounds.left += newTextOrigin.x;
	actualTextBounds.right += newTextOrigin.x;
	boundingBox.top += newTextOrigin.y;
	boundingBox.bottom += newTextOrigin.y;
	actualTextBounds.top += newTextOrigin.y;
	actualTextBounds.bottom += newTextOrigin.y;
}

