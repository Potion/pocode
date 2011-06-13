//
//  poTextBox.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poTextBox.h"
#include "poResource.h"
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
	font(poFontMap::REGULAR_FONT_KEY, tmp.add(new poFont("Lucida Grande", PO_FONT_REGULAR)));
	font(poFontMap::ITALIC_FONT_KEY, tmp.add(new poFont("Lucida Grande", PO_FONT_ITALIC)));
	font(poFontMap::BOLD_FONT_KEY, tmp.add(new poFont("Lucida Grande", PO_FONT_BOLD)));
}

poTextBox::~poTextBox() {}

std::string poTextBox::text() const {return _text;}
poTextBox &poTextBox::text(const std::string &str) { _text = str; return *this; }

poColor poTextBox::textColor() const {return color;}
poTextBox &poTextBox::textColor(poColor c) {color = c; return *this;}

poAlignment poTextBox::textAlignment() const {return align;}
poTextBox &poTextBox::textAlignment(poAlignment al) {align = al;}

poRect poTextBox::textBounds() const {}

poTextBox &poTextBox::font(const std::string &name, poFont *font) {
	fonts.font(name,font);
	return *this;
}

void poTextBox::layout() {}
void poTextBox::draw() {}

