//
//  poTextBox.h
//  poFramework4
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poObject.h"
#include "poFont.h"
#include "poTexture.h"

class poTextBox
:	public poObject 
{
public:
	poTextBox();
	poTextBox(int w, int h);
	virtual ~poTextBox();
	
	std::string text() const;
	// pass in text with embedded formatting
	poTextBox &text(const std::string &str);
	
	poColor textColor() const;
	poTextBox &textColor(poColor c);
	
	poAlignment textAlignment() const;
	poTextBox &textAlignment(poAlignment al);

	// actual bounds of the text
	poRect textBounds() const;
	
	// manage the fonts
	poTextBox &font(const std::string &name, poFont *font);
	poFont const*font(const std::string &name);
    
    // show bounds
    poTextBox&			drawBounds(bool b);
	bool				drawBounds() const;
	
	poTextBox &layout();
	void draw();

private:
	void defaultFonts();
	
	struct layout_glyph {
		layout_glyph() : glyph(0), bbox() {}
		uint glyph;
		poRect bbox;
	};
	struct layout_line {
		layout_line() : word_count(0) {}
		std::vector<layout_glyph> glyphs;
		poRect bounds;
		int word_count;
	};
	std::vector<layout_line> lines;
	
	void addGlyphsToLine(std::vector<layout_glyph> &glpyhs, poPoint size, layout_line &line);
	void breakLine(std::vector<layout_line> &lines, layout_line &line);
	void alignText();
	
	std::string _text;
	poColor color;
	poAlignment align;
	poRect text_bounds;
    bool    draw_bounds;

	poFont *_font;
	poBitmapFontAtlas *atlas;
};


