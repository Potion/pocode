//
//  poTextBox.h
//  poFramework4
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poObject.h"
#include "poFont.h"

class poTexture;

class poTextLayout {
public:
	poFontMap *fonts;
	poAlignment align;
	poRect bounds;

	void layout();
	
	poRect actual_bounds;
	poTexture *texture;
	
private:
	struct range {
		range(int,poFont*);
		int start, length;
		poFont *font;
	};
	struct helper {
		helper(int,poRect,poPoint,float);
		int glyph;
		poRect rect;
		poPoint bearing;
		float advance;
	};
	std::vector<range> ranges;
	std::vector<helper> glyphs;
};

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
	
	void layout();
	void draw();

private:
	void defaultFonts();
	
	std::string _text;
	poFontMap fonts;
	poColor color;
	poAlignment align;
};


