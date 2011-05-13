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
	virtual ~poTextBox();
	
	// formatting stripped out
	std::string text() const;
	// full text with formatting
	std::string rawText() const;
	// actual bounds of the text
	poRect textBounds() const;
	// rect for a substring starting at i
	bool textBounds(const std::string &sub, int start, uint *found, poRect *r) const;
	
	// pass in text with embedded formatting
	void text(const std::string &str);
	// manage the font
	void setRegularFont(poFont *font);
	void setItalicFont(poFont *font);
	void setBoldFont(poFont *font);
	// add an arbitrary named font
	void setFont(const std::string &name, poFont *font);
	
	void layout();
	void draw();

private:
	std::string text_;
	std::string parsed_text;
	poRect actual_bounds;
	poFontMap fonts;
	poResourceStore resources;
	TiXmlDocument *doc;
	poTexture *rendered;
};

