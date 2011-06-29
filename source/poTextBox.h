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
	
	// these properties can be overridden inline 
	// eg <span leading=13 tracking=.2>text will be leaded and tracked</span>
	float leading() const;
	poTextBox &leading(float f);
	float tracking() const;
	poTextBox &tracking(float f);
	float paddingLeft() const;
	float paddingRight() const;
	float paddingTop() const;
	float paddingBottom() const;
	poTextBox &padding(float f);
	poTextBox &padding(float h, float v);
	poTextBox &padding(float l, float r, float t, float b);
	
	// font name correspondes to html tag embedded in the text
	// eg <i>italic</i> <u>underlined</u> <b>bold</b> <b><i>bold italic</b></i>
	// <span>regular, when you want to override color, leading or tracking</span>
	poTextBox &font(const std::string &name, poFont *font);
	poTextBox &font(const std::string &name, const poFont &font);
	poFont const*font(const std::string &name);
    
    // show bounds
    poTextBox&			drawBounds(bool b);
	bool				drawBounds() const;
	
	poTextBox &layout();
	void draw();
	
	// make a quick dumb button ... 
	poTextBox &buttonize(poColor fill, poColor stroke, float strokeWidth, float rad=0.f);
	poTextBox &debuttonize();
	bool isButtonized() const;
	poColor buttonFill() const;
	poColor buttonStroke() const;
	float buttonStrokeWidth() const;

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
	float _leading, _tracking, _padding[4];
	
	poShape2D *button;
};


