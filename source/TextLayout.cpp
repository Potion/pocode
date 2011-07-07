//
//  TextLayout.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "TextLayout.h"
#include "poMath.h"
#include <boost/tokenizer.hpp>
#include <utf8.h>

std::string TextLayout::text() const {return _text;}
void TextLayout::text(const std::string &str) {_text = str;}
void TextLayout::font(poFont *f, const std::string &weight) {font(*f,weight);}
void TextLayout::font(const poFont &f, const std::string &weight) {
	if(fonts.find(weight) != fonts.end())
		delete fonts[weight];
	fonts[weight] = f.copy();
}
poFont *const TextLayout::font(const std::string &weight) {return fonts[weight];}
uint TextLayout::numLines() const {return lines.size();}
layout_line TextLayout::getLine(uint i) const {return lines[i];}




TextBoxLayout::TextBoxLayout(poPoint s)
:	_size(s), _tracking(1.f), _leading(1.f), _alignment(PO_ALIGN_TOP_LEFT)
{
	padding(0.f);
}

void TextBoxLayout::layout() {
	lines.clear();
	text_bounds.set(0,0,0,0);
	
	if(_text.empty())
		return;
	
	layout_line line;
	
	poFont *font = fonts[PO_FONT_REGULAR];
	font->glyph(' ');

	float spacer = font->glyphAdvance().x * tracking();
	
	using namespace boost;
	using namespace std;
	
	tokenizer< char_separator<char> > tok(text(), char_separator<char>(" "));
	for(tokenizer< char_separator<char> >::iterator word=tok.begin(); word!=tok.end(); ++word) {
		poPoint size(0,0);
		vector<layout_glyph> glyphs;
		
		string::const_iterator ch=word->begin();
		while(ch != word->end()) {
			uint codepoint = utf8::next(ch, word->end());
			if(codepoint == '\n') {
				addGlyphsToLine(glyphs, size, line);
				text_bounds.include(line.bounds.origin + line.bounds.size);
				breakLine(lines, line);
				size.set(0,0,0);
				continue;
			}
			else if(codepoint == '\t') {
				size.x += spacer * 4;
				continue;
			}
			
			font->glyph(codepoint);
			
			layout_glyph glyph;
			glyph.glyph = codepoint;
			glyph.bbox = font->glyphBounds();
			glyph.bbox.origin += poPoint(size.x, 0) + font->glyphBearing();
			glyphs.push_back(glyph);
			
			size.x += font->glyphAdvance().x*tracking();
			size.y = std::max(glyph.bbox.origin.y + glyph.bbox.size.y, size.y);
			
			if(size.x + line.bounds.size.x > (_size.x-paddingLeft()-paddingRight()) && line.word_count >= 1) {
				line.bounds.size.x -= spacer;
				text_bounds.include(line.bounds.origin + line.bounds.size);
				breakLine(lines, line);
			}
		}
		
		size.x += spacer;
		addGlyphsToLine(glyphs, size, line);
	}
	
	line.bounds.size.x -= spacer;
	text_bounds.include(line.bounds.origin + line.bounds.size);
	lines.push_back(line);
	alignText();
}

poRect TextBoxLayout::textBounds() const {return text_bounds;}
poPoint TextBoxLayout::size() const {return _size;}
void TextBoxLayout::size(poPoint s) {_size = s;}
float TextBoxLayout::tracking() const {return _tracking;}
void TextBoxLayout::tracking(float f) {_tracking = f;}
float TextBoxLayout::leading() const {return _leading;}
void TextBoxLayout::leading(float f) {_leading = f;}
float TextBoxLayout::paddingLeft() const {return _padding[0];}
float TextBoxLayout::paddingRight() const {return _padding[1];}
float TextBoxLayout::paddingTop() const {return _padding[2];}
float TextBoxLayout::paddingBottom() const {return _padding[3];}
void TextBoxLayout::padding(float f) {_padding[0] = _padding[1] = _padding[2] = _padding[3] = f;}
void TextBoxLayout::padding(float h, float v) {_padding[0] = _padding[1] = h; _padding[2] = _padding[3] = v;}
void TextBoxLayout::padding(float l, float r, float t, float b) {_padding[0] = l; _padding[1] = r; _padding[2] = t; _padding[3] = b;}
poAlignment TextBoxLayout::alignment() const {return _alignment;}
void TextBoxLayout::alignment(poAlignment a) {_alignment = a;}

void TextBoxLayout::addGlyphsToLine(std::vector<layout_glyph> &glyphs, poPoint size, layout_line &line) {
	BOOST_FOREACH(layout_glyph &glyph, glyphs) {
		glyph.bbox.origin = glyph.bbox.origin + poPoint(line.bounds.size.x, line.bounds.origin.y);
		line.glyphs.push_back(glyph);
	}
	
	line.bounds.size.x += size.x;
	line.bounds.size.y = std::max(line.bounds.size.y, size.y);
	line.word_count += 1;
	glyphs.clear();
}

void TextBoxLayout::breakLine(std::vector<layout_line> &lines, layout_line &line) {
	lines.push_back(line);
	
	line = layout_line();
	line.bounds.origin.y = lines.size() * fonts[PO_FONT_REGULAR]->lineHeight() * leading();
}

void TextBoxLayout::alignText() {
	BOOST_FOREACH(layout_line &line, lines) {
		poPoint glyphOffset(0.f, 0.f);
		
		switch(_alignment) {
			case PO_ALIGN_TOP_LEFT:
				break;
			case PO_ALIGN_TOP_CENTER:
				glyphOffset.x = (_size.x - line.bounds.size.x)/2;
				break;
			case PO_ALIGN_TOP_RIGHT:
				glyphOffset.x = (_size.x - line.bounds.size.x); 
				break;
			case PO_ALIGN_CENTER_LEFT:
				glyphOffset.y = (_size.y - text_bounds.height())/2;
				break;
			case PO_ALIGN_CENTER_CENTER:
				glyphOffset.x = (_size.x - line.bounds.size.x)/2;
				glyphOffset.y = (_size.y - text_bounds.height())/2;
				break;
			case PO_ALIGN_CENTER_RIGHT:
				glyphOffset.x = (_size.x - line.bounds.size.x); 
				glyphOffset.y = (_size.y - text_bounds.height())/2;
				break;
			case PO_ALIGN_BOTTOM_LEFT:
				glyphOffset.y = (_size.y - text_bounds.height());
				break;
			case PO_ALIGN_BOTTOM_CENTER:
				glyphOffset.x = (_size.x - line.bounds.size.x)/2; 
				glyphOffset.y = (_size.y - text_bounds.height());
				break;
			case PO_ALIGN_BOTTOM_RIGHT:
				glyphOffset.x = (_size.x - line.bounds.size.x); 
				glyphOffset.y = (_size.y - text_bounds.height());
				break;
		}
		
		glyphOffset.x += paddingLeft();
		glyphOffset.y += paddingBottom();
		line.bounds.origin += glyphOffset;
		
		BOOST_FOREACH(layout_glyph &glyph, line.glyphs) {
			glyph.bbox.origin = round(glyph.bbox.origin + glyphOffset);
		}
	}
	
	text_bounds = lines[0].bounds;
	for(int i=1; i<lines.size(); i++) {
		text_bounds.include(lines[i].bounds);
	}
}


