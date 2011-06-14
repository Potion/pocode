//
//  poTextBox.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

using namespace std;
using namespace boost;

#include "poTextBox.h"
#include "poResource.h"
#include "poSimpleDrawing.h"
#include <boost/tokenizer.hpp>
#include <utf8.h>

poTextBox::poTextBox()
:	_text("")
,	color(poColor::white)
,	_font(NULL)
,	atlas(NULL)
{
	defaultFonts();
}

poTextBox::poTextBox(int w, int h) 
:	_text("")
,	color(poColor::white)
,	_font(NULL)
,	atlas(NULL)
{
	defaultFonts();
	bounds(poRect(0,0,w,h));
}

void poTextBox::defaultFonts() {
//	poResourceStore tmp;
//	font(keyForFontTrait(PO_FONT_REGULAR), tmp.add(new poFont("Lucida Grande", PO_FONT_REGULAR)));
//	font(keyForFontTrait(PO_FONT_ITALIC), tmp.add(new poFont("Lucida Grande", PO_FONT_ITALIC)));
//	font(keyForFontTrait(PO_FONT_BOLD), tmp.add(new poFont("Lucida Grande", PO_FONT_BOLD)));
	_font = new poFont("Lucida Grande", 20);
	atlas = new poBitmapFontAtlas(_font);
}

poTextBox::~poTextBox() {}

std::string poTextBox::text() const {return _text;}
poTextBox &poTextBox::text(const std::string &str) { _text = str; return *this; }

poColor poTextBox::textColor() const {return color;}
poTextBox &poTextBox::textColor(poColor c) {color = c; return *this;}

poAlignment poTextBox::textAlignment() const {return align;}
poTextBox &poTextBox::textAlignment(poAlignment al) {align = al;}

poRect poTextBox::textBounds() const {return text_bounds;}

poTextBox &poTextBox::font(const std::string &name, poFont *f) {
//	if(fonts.find(name) != fonts.end())
//		delete fonts[name];
//	fonts[name] = font;
	if(_font != NULL) {
		delete _font;
		delete atlas;
	}
	_font = f->copy();
	atlas = new poBitmapFontAtlas(_font);
	
	return *this;
}

poFont const*poTextBox::font(const std::string &name) {
	if(_font)
		return _font;
}

poTextBox &poTextBox::layout() {
	lines.clear();
	layout_line line;
	
	float widest_line = 0.f;
	_font->glyph(' ');
	float spacer = _font->glyphAdvance().x;
	
	tokenizer<> tok(text());
	for(tokenizer<>::iterator word=tok.begin(); word!=tok.end(); ++word) {
		float w = 0;
		vector<layout_glyph> glyphs;
		
		string::const_iterator ch=word->begin();
		while(ch != word->end()) {
			uint codepoint = utf8::next(ch, word->end());
			_font->glyph(codepoint);

			poPoint kern(0.f, 0.f);
//			if(ch != word->begin() && _font->hasKerning())
//				kern = _font->kernGlyphs(*prev, codepoint);

			layout_glyph glyph;
			glyph.glyph = codepoint;
			glyph.bbox = _font->glyphBounds();
			glyph.bbox.origin += poPoint(w, 0) + _font->glyphBearing() + kern;
			
			glyphs.push_back(glyph);

			w += _font->glyphAdvance().x + kern.x;
			
			if(w + line.width > bounds().width() && line.word_count >= 1) {
				widest_line = max(widest_line, line.width-=spacer);
				lines.push_back(line);
				
				line = layout_line();
				line.ypos = lines.size() * _font->lineHeight();
			}
		}
		
		BOOST_FOREACH(layout_glyph &glyph, glyphs) {
			glyph.bbox.origin += poPoint(line.width, line.ypos);
			line.glyphs.push_back(glyph);
		}
		
		line.width += w + spacer;
		line.word_count += 1;
		glyphs.clear();
	}
	
	widest_line = max(widest_line, line.width-=spacer);
	lines.push_back(line);
	text_bounds.set(0,0,widest_line,line.ypos+_font->lineHeight());
}

void poTextBox::draw() {
//	applyColor(poColor::yellow);
//	BOOST_FOREACH(layout_line &line, lines) {
//		drawRect(poRect(poPoint(0,line.ypos),poPoint(line.width,_font->lineHeight())));
//	}
//
//	applyColor(poColor::green);
//	drawStroke(textBounds());
//	
//	applyColor(poColor::red);
//	drawStroke(bounds());

	applyColor(poColor::white);
	atlas->startDrawing(0);
	BOOST_FOREACH(layout_line &line, lines) {
		BOOST_FOREACH(layout_glyph &glyph, line.glyphs) {
			atlas->cacheGlyph(glyph.glyph);
			atlas->drawUID(glyph.glyph, glyph.bbox.origin);
		}
	}
	atlas->stopDrawing();
}

