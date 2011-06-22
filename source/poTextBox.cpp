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
#include "poShapeBasics2D.h"
#include <boost/tokenizer.hpp>
#include <utf8.h>

#include <float.h>

poTextBox::poTextBox()
:	_text("")
,	color(poColor::white)
,	_font(NULL)
,	atlas(NULL)
,	draw_bounds(false)
,	align(PO_ALIGN_TOP_LEFT)
,	button(NULL)
{
	defaultFonts();
}

poTextBox::poTextBox(int w, int h) 
:	_text("")
,	color(poColor::white)
,	_font(NULL)
,	atlas(NULL)
,	draw_bounds(false)
,	align(PO_ALIGN_TOP_LEFT)
,	button(NULL)
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
poTextBox &poTextBox::textAlignment(poAlignment al) {align = al; return *this;}

poRect poTextBox::textBounds() const {return text_bounds;}

poTextBox   &poTextBox::drawBounds(bool b) {draw_bounds = b; return *this;}
bool		poTextBox::drawBounds() const {return draw_bounds;}

poTextBox &poTextBox::font(const std::string &name, poFont *f) {
//	if(fonts.find(name) != fonts.end())
//		delete fonts[name];
//	fonts[name] = font;
	if(_font != NULL) {
		delete _font;
	}
	_font = f->copy();
	atlas = BitmapFontCache().atlasForFont(_font);
	
	return *this;
}

poFont const*poTextBox::font(const std::string &name) {
	if(_font)
		return _font;
}

void poTextBox::addGlyphsToLine(std::vector<layout_glyph> &glyphs, poPoint size, layout_line &line) {
	BOOST_FOREACH(layout_glyph &glyph, glyphs) {
		glyph.bbox.origin = glyph.bbox.origin + poPoint(line.bounds.size.x, line.bounds.origin.y);
		line.glyphs.push_back(glyph);
	}

	line.bounds.size.x += size.x;
	line.bounds.size.y = std::max(line.bounds.size.y, size.y);
	line.word_count += 1;
	glyphs.clear();
}

void poTextBox::breakLine(vector<layout_line> &lines, layout_line &line) {
	lines.push_back(line);

	line = layout_line();
	line.bounds.origin.y = lines.size() * _font->lineHeight();
}

poTextBox &poTextBox::layout() {
	lines.clear();
	text_bounds.set(0,0,0,0);
	
	if(_text.empty())
		return *this;
	
	layout_line line;
	
	_font->glyph(' ');
	float spacer = _font->glyphAdvance().x;

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
			
			_font->glyph(codepoint);

			poPoint kern(0.f, 0.f);
//			if(ch != word->begin() && _font->hasKerning())
//				kern = _font->kernGlyphs(*prev, codepoint);

			layout_glyph glyph;
			glyph.glyph = codepoint;
			glyph.bbox = _font->glyphBounds();
			glyph.bbox.origin += poPoint(size.x, 0) + _font->glyphBearing() + kern;
			glyphs.push_back(glyph);

			size.x += _font->glyphAdvance().x + kern.x;
			size.y = std::max(glyph.bbox.origin.y + glyph.bbox.size.y, size.y);
			
			if(size.x + line.bounds.size.x > bounds().width() && line.word_count >= 1) {
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
	
	return *this;
}

void poTextBox::alignText() {
	poRect frame = bounds();

	BOOST_FOREACH(layout_line &line, lines) {
		poPoint glyphOffset(0.f, 0.f);

		switch(align) {
			case PO_ALIGN_TOP_LEFT:
				break;
			case PO_ALIGN_TOP_CENTER:
				glyphOffset.x = (frame.width() - line.bounds.size.x)/2;
				break;
			case PO_ALIGN_TOP_RIGHT:
				glyphOffset.x = (frame.width() - line.bounds.size.x); 
				break;
			case PO_ALIGN_CENTER_LEFT:
				glyphOffset.y = (frame.height() - text_bounds.height())/2;
				break;
			case PO_ALIGN_CENTER_CENTER:
				glyphOffset.x = (frame.width() - line.bounds.size.x)/2;
				glyphOffset.y = (frame.height() - text_bounds.height())/2;
				break;
			case PO_ALIGN_CENTER_RIGHT:
				glyphOffset.x = (frame.width() - line.bounds.size.x); 
				glyphOffset.y = (frame.height() - text_bounds.height())/2;
				break;
			case PO_ALIGN_BOTTOM_LEFT:
				glyphOffset.y = (frame.height() - text_bounds.height());
				break;
			case PO_ALIGN_BOTTOM_CENTER:
				glyphOffset.x = (frame.width() - line.bounds.size.x)/2; 
				glyphOffset.y = (frame.height() - text_bounds.height());
				break;
			case PO_ALIGN_BOTTOM_RIGHT:
				glyphOffset.x = (frame.width() - line.bounds.size.x); 
				glyphOffset.y = (frame.height() - text_bounds.height());
				break;
		}
		
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

void poTextBox::draw() {
//	applyColor(poColor::yellow);
//	BOOST_FOREACH(layout_line &line, lines) {
//		drawRect(poRect(poPoint(0,line.ypos),poPoint(line.width,_font->lineHeight())));
//	}
//
	if(button) {
		button->draw();
	}
	
    if(draw_bounds) {
		applyColor(poColor::white);
		BOOST_FOREACH(layout_line &line, lines) {
			drawStroke(line.bounds);
		}
		
        applyColor(poColor::dk_grey);
        drawStroke(textBounds());
	
        applyColor(poColor::magenta);
        drawStroke(bounds());
    }

	applyColor(textColor());
	atlas->startDrawing(0);
	BOOST_FOREACH(layout_line &line, lines) {
		BOOST_FOREACH(layout_glyph &glyph, line.glyphs) {
			atlas->cacheGlyph(glyph.glyph);
			atlas->drawUID(glyph.glyph, glyph.bbox.origin);
		}
	}
	atlas->stopDrawing();
}

poTextBox &poTextBox::buttonize(poColor fill, poColor stroke, float strokeWidth, float rad) {
	if(button)
		delete button;

	button = new poRectShape(bounds().width(), bounds().height(), rad);
	button->fillColor(fill);
	button->generateStroke(strokeWidth);
	button->strokeColor(stroke);
	
	return *this;
}
							 
poTextBox &poTextBox::debuttonize() {
	delete button;
	button = NULL;
	return *this;
}
bool poTextBox::isButtonized() const {return button != NULL;}
poColor poTextBox::buttonFill() const {return button->fillColor();}
poColor poTextBox::buttonStroke() const {return button->strokeColor();}
float poTextBox::buttonStrokeWidth() const {return button->strokeWidth();}
