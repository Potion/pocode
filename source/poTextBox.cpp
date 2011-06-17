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
,	draw_bounds(false)
{
	defaultFonts();
}

poTextBox::poTextBox(int w, int h) 
:	_text("")
,	color(poColor::white)
,	_font(NULL)
,	atlas(NULL)
,	draw_bounds(false)
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

poTextBox   &poTextBox::drawBounds(bool b) {draw_bounds = b;}
bool		poTextBox::drawBounds() const {return draw_bounds;}

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

void poTextBox::addGlyphsToLine(std::vector<layout_glyph> &glyphs, float &width, layout_line &line) {
	BOOST_FOREACH(layout_glyph &glyph, glyphs) {
		glyph.bbox.origin += poPoint(line.width, line.ypos);
		line.glyphs.push_back(glyph);
	}
	
	line.width += width;
	line.word_count += 1;
	glyphs.clear();
	
	width = 0;
}

void poTextBox::breakLine(float &widest, vector<layout_line> &lines, layout_line &line) {
	widest = max(widest, line.width);
	lines.push_back(line);
	
	line = layout_line();
	line.ypos = lines.size() * _font->lineHeight();
}

poTextBox &poTextBox::layout() {
	lines.clear();
	layout_line line;
	
	float widest_line = 0.f;
	_font->glyph(' ');
	float spacer = _font->glyphAdvance().x;
	
	tokenizer< char_separator<char> > tok(text(), char_separator<char>(" "));
	for(tokenizer< char_separator<char> >::iterator word=tok.begin(); word!=tok.end(); ++word) {
		float w = 0;
		vector<layout_glyph> glyphs;
		
		string::const_iterator ch=word->begin();
		while(ch != word->end()) {
			uint codepoint = utf8::next(ch, word->end());
			if(codepoint == '\n') {
				addGlyphsToLine(glyphs, w, line);
				breakLine(widest_line, lines, line);
				continue;
			}
			else if(codepoint == '\t') {
				w += spacer * 4;
				continue;
			}
			
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
				line.width -= spacer;
				breakLine(widest_line, lines, line);
			}
		}
		
		w += spacer;
		addGlyphsToLine(glyphs, w, line);
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
    if(draw_bounds) {
        applyColor(poColor::dk_grey);
        drawStroke(textBounds());
	
        applyColor(poColor::magenta);
        drawStroke(bounds());
    }

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

