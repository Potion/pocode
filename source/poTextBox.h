//
//  poTextBox.h
//  poFramework4
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

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
		layout_line() : width(0), ypos(0), word_count(0) {}
		std::vector<layout_glyph> glyphs;
		float width, ypos;
		int word_count;
	};
	std::vector<layout_line> lines;
	
	std::string _text;
	poColor color;
	poAlignment align;
	poRect text_bounds;

	poFont *_font;
	poBitmapFontAtlas *atlas;
};


