//
//  poTextBox.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poTextBox.h"
#include "poSimpleDrawing.h"


poTextBox::poTextBox()
:	_text("")
,	color(poColor::white)
{
	defaultFonts();
}

poTextBox::poTextBox(int w, int h) 
:	_text("")
,	color(poColor::white)
{
	defaultFonts();
	bounds(poRect(0,0,w,h));
}

void poTextBox::defaultFonts() {
	poResourceStore tmp;
	fonts.font(poFontMap::REGULAR_FONT_KEY, tmp.add(new poFont("Helvetica", 0, 12)));
	fonts.font(poFontMap::ITALIC_FONT_KEY, tmp.add(new poFont("Helvetica", FONT_ITALIC, 12)));
	fonts.font(poFontMap::BOLD_FONT_KEY, tmp.add(new poFont("Helvetica", FONT_BOLD, 12)));
}

poTextBox::~poTextBox() {}

std::string poTextBox::text() const {return _text;}
poTextBox &poTextBox::text(const std::string &str) { _text = str; return *this; }

poColor poTextBox::textColor() const {return color;}
poTextBox &poTextBox::textColor(poColor c) {color = c; return *this;}

poRect poTextBox::textBounds() const {return _layout.actualBounds;}

poFontMap &poTextBox::fontMap() {
	return fonts;
}

poTextBox &poTextBox::font(const std::string &name, poFont *font) {
	fonts.font(name,font);
	return *this;
}

void poTextBox::layout() {
	_layout.fonts = &fonts;
	_layout.bounds = bounds();
	_layout.text = _text;
	_layout.render();
}

void poTextBox::draw() {
	if(_layout.rendered) {
		applyColor(color);
		drawRect(poRect(poPoint(0,0,0),bounds().size), _layout.rendered);
//		drawStroke(textBounds());
//		drawStroke(bounds());
//		drawRect(poRect(_layout.baseline, poPoint(5,5)));
	}
}

