//
//  poTextBox.h
//  poFramework4
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poObject.h"
#include "poTexture.h"
#include "TextLayout.h"

#include "poFont.h"
#include "poBitmapFontAtlas.h"

static const std::string PO_TEXT_REGULAR = "";
static const std::string PO_TEXT_ITALIC = "i";
static const std::string PO_TEXT_BOLD = "b";
static const std::string PO_TEXT_BOLD_ITALIC = "bi";

enum {
	PO_TEXT_BOX_STROKE_BOUNDS = 1,
	PO_TEXT_BOX_STROKE_TEXT_BOUNDS = 1<<1,
	PO_TEXT_BOX_STROKE_LINE = 1<<2,
	PO_TEXT_BOX_STROKE_GLYPH = 1<<3,
	
	PO_TEXT_BOX_STROKE_ALL = PO_TEXT_BOX_STROKE_TEXT_BOUNDS | PO_TEXT_BOX_STROKE_LINE | PO_TEXT_BOX_STROKE_GLYPH
};

class poTextBox
:	public poObject 
{
public:
	poTextBox();
	poTextBox(int w);
	poTextBox(int w, int h);
	virtual ~poTextBox();
	
	std::string text() const;
	// pass in text with embedded formatting
	poTextBox *text(const std::string &str);
	
	poAlignment textAlignment() const;
	void textAlignment(poAlignment al);

	// actual bounds of the text
	poRect textBounds() const;
	
	void reshape(int w, int h);
	void reshape(poPoint p);

	bool richText() const;
	void richText(bool b);

	uint numLines() const;
	poRect boundsForLine(uint num) const;
	
	// overide with color property in rich text mode
	poColor textColor;
	
	// you can use the PO_TEXT_BOX_STROKE_* to control debugging
	// true and false are still ok, but it'll only give minimal info
    int		drawBounds;

	// these can be overridden inline with a span
	// eg <span leading='.9' tracking='1.1'>leaded and tracked differently</span>
	// there can be only 1 leading for a line tho, so be careful as it'll the one it has when the line breaks
	float leading() const;
	void leading(float f);
	float tracking() const;
	void tracking(float f);
	
	float paddingLeft() const;
	float paddingRight() const;
	float paddingTop() const;
	float paddingBottom() const;
	void padding(float f);
	void padding(float h, float v);
	void padding(float l, float r, float t, float b);
	
	void tabWidth(int tw);
	int tabWidth() const;
	
	// font name correspondes to html tag embedded in the text
	// eg <i>italic</i> <u>underlined</u> <b>bold</b> <b><i>bold italic</b></i>
	// <span>regular, when you want to override color, leading or tracking</span>
	void font(poFont *font, const std::string &name=PO_TEXT_REGULAR);
	poFont *font(const std::string &name=PO_TEXT_REGULAR);
    
	poTextBox *layout();
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
	
	bool fit_height_to_bounds;
	poAlignment text_align;
	TextBoxLayout _layout;
	poShape2D *button;
};


