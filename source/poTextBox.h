//
//  poTextBox.h
//  poFramework4
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poObject.h"
#include "poTextLayout.h"

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

	// actual bounds of the text
	poRect textBounds() const;
	
	// manage the fonts
	poFontMap &fontMap();
	poTextBox &font(const std::string &name, poFont *font);
	
	void layout();
	void draw();

private:
	void defaultFonts();
	
	std::string _text;
	poFontMap fonts;
	poBoxLayout _layout;
};

