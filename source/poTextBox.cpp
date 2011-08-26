//
//  poTextBox.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

using namespace std;

#include "poTextBox.h"
#include "SimpleDrawing.h"
#include "poShapeBasics2D.h"
#include "poResourceStore.h"

#include <float.h>

poTextBox::poTextBox()
:	poObject()
,	textColor(poColor::white)
,	drawBounds(false)
,	button(NULL)
,	_layout(poPoint())
,	fit_height_to_bounds(true)
,	text_align(PO_ALIGN_TOP_LEFT)
{
	defaultFonts();
}

poTextBox::poTextBox(int w) 
:	poObject()
,	textColor(poColor::white)
,	drawBounds(false)
,	button(NULL)
,	_layout(poPoint(w,0))
,	fit_height_to_bounds(true)
,	text_align(PO_ALIGN_TOP_LEFT)
{
	defaultFonts();
	bounds = poRect(0,0,w,0);
}

poTextBox::poTextBox(int w, int h) 
:	poObject()
,	textColor(poColor::white)
,	drawBounds(false)
,	button(NULL)
,	_layout(poPoint(w,h))
,	fit_height_to_bounds(false)
,	text_align(PO_ALIGN_TOP_LEFT)
{
	defaultFonts();
	bounds = poRect(0,0,w,h);
}

void poTextBox::defaultFonts() {
	font(getFont("Lucida Grande", 20));
}

poTextBox::~poTextBox() {}

std::string poTextBox::text() const {return _layout.text();}
poTextBox *poTextBox::text(const std::string &str) {_layout.text(str); return this; }

poAlignment poTextBox::textAlignment() const {return text_align;}
void poTextBox::textAlignment(poAlignment al) {text_align = al;}

poRect poTextBox::textBounds() const {return _layout.textBounds();}
void poTextBox::reshape(int w, int h) {
	bounds.size.set(w,h,0);
	_layout.size(bounds.size);
}

bool poTextBox::richText() const {return _layout.richText();}
void poTextBox::richText(bool b) {_layout.richText(b);}

void poTextBox::reshape(poPoint p){
    reshape(p.x,p.y);
}

uint poTextBox::numLines() const {return _layout.numLines();}
poRect poTextBox::boundsForLine(uint num) const {return _layout.boundsForLine(num);}

int     poTextBox::numLettersForLine( int lineIndex )
{ 
    return _layout.numLettersForLine( lineIndex );
}

poRect  poTextBox::getBoundsForLetterOnLine( int letterIndex, int lineIndex )
{
    return _layout.boundsForLetterOnLine( letterIndex, lineIndex );
}

void    poTextBox::setBoundsForLetterOnLine( int letterIndex, int lineIndex, poRect newBounds )
{
    _layout.boundsForLetterOnLine( letterIndex, lineIndex ) = newBounds;
}


float poTextBox::leading() const {return _layout.leading();}
void poTextBox::leading(float f) {_layout.leading(f); }
float poTextBox::tracking() const {return _layout.tracking();}
void poTextBox::tracking(float f) {_layout.tracking(f); }
float poTextBox::paddingLeft() const {return _layout.paddingLeft();}
float poTextBox::paddingRight() const {return _layout.paddingRight();}
float poTextBox::paddingTop() const {return _layout.paddingTop();}
float poTextBox::paddingBottom() const {return _layout.paddingBottom();}
void poTextBox::padding(float f) {_layout.padding(f); }
void poTextBox::padding(float h, float v) {_layout.padding(h,v); }
void poTextBox::padding(float l, float r, float t, float b) {_layout.padding(l,r,t,b); }

void poTextBox::tabWidth(int tw) {_layout.tabWidth(tw);}
int poTextBox::tabWidth() const {return _layout.tabWidth();}

void poTextBox::font(poFont *f, const std::string &name) {
	_layout.font(f,name);
}
poFont *poTextBox::font(const std::string &name) {return _layout.font(name);}

poTextBox *poTextBox::layout() {
	_layout.layout();
	
	if(fit_height_to_bounds)
		bounds.size.y = textBounds().size.y;

	alignment(alignment());
	_layout.alignment(text_align);
	
	return this;
}

void poTextBox::draw() {
	if(button) {
		button->draw();
	}
	
    if(drawBounds) {
		for(int i=0; i<numLines(); i++) {
			
			if(drawBounds & PO_TEXT_BOX_STROKE_GLYPH) {
				applyColor(poColor::lt_grey, .5f);
				BOOST_FOREACH(layout_glyph const &glyph, _layout.getLine(i).glyphs) {
					drawStroke(glyph.bbox);
				}
			}
			
			if(drawBounds & PO_TEXT_BOX_STROKE_LINE) {
				applyColor(poColor::white, .6f);
				drawStroke(boundsForLine(i));
			}
		}
		
		if(drawBounds & PO_TEXT_BOX_STROKE_TEXT_BOUNDS) {
			applyColor(poColor::grey, .7f);
			drawStroke(textBounds());
		}
		
        applyColor(poColor::dk_grey, .8f);
        drawStroke(bounds);
		
		applyColor(poColor::red);
		drawRect(poRect(-offset-poPoint(5,5), poPoint(10,10)));
    }

	poBitmapFontAtlas *reg = getBitmapFont(this->font());
	poBitmapFontAtlas *atlas = reg;
	
	glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT);
	
	int count = 0;
	glDisable(GL_MULTISAMPLE);
	atlas->startDrawing(0);
	for(int i=0; i<numLines(); i++) {
		BOOST_FOREACH(layout_glyph const &glyph, _layout.getLine(i).glyphs) {
			applyColor(poColor(textColor, appliedAlpha()));
			
			if(_layout.richText()) {
				poDictionary dict = _layout.dictionaryForIndex(count);
				count++;

				if(dict.has("color"))
					applyColor(poColor(dict.getColor("color"), appliedAlpha()));

				if(dict.has("font")) {
					poBitmapFontAtlas *a = getBitmapFont(dict.getPtr<poFont>("font"));
					if(a != atlas) {
						atlas = a;
						atlas->startDrawing(0);
					}
				}
				else if(atlas != reg) {
					atlas = reg;
					atlas->startDrawing(0);
				}
			}
			
			atlas->cacheGlyph(glyph.glyph);
			atlas->drawUID(glyph.glyph, glyph.bbox.origin);
		}
	}
	atlas->stopDrawing();
	glEnable(GL_MULTISAMPLE);
	
	glPopAttrib();
}

poTextBox &poTextBox::buttonize(poColor fill, poColor stroke, float strokeWidth, float rad) {
	if(button)
		delete button;

	button = new poRectShape(bounds.width(), bounds.height(), rad);
	button->fillColor = fill;
	button->strokeColor = stroke;
	button->generateStroke(strokeWidth);
	
	return *this;
}
							 
poTextBox &poTextBox::debuttonize() {
	delete button;
	button = NULL;
	return *this;
}
bool poTextBox::isButtonized() const {return button != NULL;}
poColor poTextBox::buttonFill() const {return button->fillColor;}
poColor poTextBox::buttonStroke() const {return button->strokeColor;}
float poTextBox::buttonStrokeWidth() const {return button->strokeWidth();}
