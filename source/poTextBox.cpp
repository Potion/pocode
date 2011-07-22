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
,	atlas(NULL)
,	drawBounds(false)
,	button(NULL)
,	_layout(poPoint())
{
	defaultFonts();
}

poTextBox::poTextBox(int w, int h) 
:	poObject()
,	textColor(poColor::white)
,	atlas(NULL)
,	drawBounds(false)
,	button(NULL)
,	_layout(poPoint(w,h))
{
	defaultFonts();
	bounds = poRect(0,0,w,h);
}

void poTextBox::defaultFonts() {
	font(getFont("Lucida Grande", 20));
}

poTextBox::~poTextBox() {}

std::string poTextBox::text() const {return _layout.text();}
poTextBox &poTextBox::text(const std::string &str) {_layout.text(str); return *this; }

poAlignment poTextBox::textAlignment() const {return _layout.alignment();}
poTextBox &poTextBox::textAlignment(poAlignment al) {_layout.alignment(al); return *this;}

poRect poTextBox::textBounds() const {return _layout.textBounds();}

float poTextBox::leading() const {return _layout.leading();}
poTextBox &poTextBox::leading(float f) {_layout.leading(f); return *this;}
float poTextBox::tracking() const {return _layout.tracking();}
poTextBox & poTextBox::tracking(float f) {_layout.tracking(f); return *this;}
float poTextBox::paddingLeft() const {return _layout.paddingLeft();}
float poTextBox::paddingRight() const {return _layout.paddingRight();}
float poTextBox::paddingTop() const {return _layout.paddingTop();}
float poTextBox::paddingBottom() const {return _layout.paddingBottom();}
poTextBox & poTextBox::padding(float f) {_layout.padding(f); return *this;}
poTextBox & poTextBox::padding(float h, float v) {_layout.padding(h,v); return *this;}
poTextBox & poTextBox::padding(float l, float r, float t, float b) {_layout.padding(l,r,t,b); return *this;}

poTextBox &poTextBox::font(poFont *f, const std::string &name) {
	_layout.font(f,name);
	
	if(f && !atlas) 
		delete atlas;
	
	atlas = new poBitmapFontAtlas(f);
	
	return *this;
}
poFont const*poTextBox::font(const std::string &name) {return _layout.font(name);}

poTextBox &poTextBox::layout() {_layout.layout(); return *this;}

void poTextBox::draw() {
	if(button) {
		button->draw();
	}
	
    if(drawBounds) {
		applyColor(poColor::white);
		for(int i=0; i<_layout.numLines(); i++) {
			drawStroke(_layout.getLine(i).bounds);
		}
		
        applyColor(poColor::dk_grey);
        drawStroke(textBounds());
	
        applyColor(poColor::magenta);
        drawStroke(bounds);
		
		applyColor(poColor::red);
		drawRect(poRect(-offset-poPoint(5,5), poPoint(10,10)));
    }

	applyColor(poColor(textColor, appliedAlpha()));
	atlas->startDrawing(0);
	for(int i=0; i<_layout.numLines(); i++) {
		BOOST_FOREACH(layout_glyph const &glyph, _layout.getLine(i).glyphs) {
			atlas->cacheGlyph(glyph.glyph);
			atlas->drawUID(glyph.glyph, glyph.bbox.origin);
		}
	}
	atlas->stopDrawing();
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
