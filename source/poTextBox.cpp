//
//  poTextBox.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poTextBox.h"
#include "PlatformHelpers.h"
#include "poSimpleDrawing.h"

static const char *REGULAR_FONT = "text";
static const char *ITALIC_FONT = "em";
static const char *BOLD_FONT = "strong";

poTextBox::poTextBox()
:	text_("")
,	parsed_text("")
,	doc(NULL)
,	rendered(NULL)
{
	poFont *reg = new poFont("Helvetica", 0, 12);
	poFont *ita = new poFont("Helvetica", FONT_ITALIC, 12);
	poFont *bol = new poFont("Helvetica", FONT_BOLD, 12);
	
	setRegularFont(reg);
	setItalicFont(ita);
	setBoldFont(bol);
	
	delete reg;
	delete ita;
	delete bol;
}

poTextBox::~poTextBox() {
	if(doc != NULL)
		delete doc;
}

std::string poTextBox::text() const {return std::string(parsed_text.begin(), parsed_text.end());}

std::string poTextBox::rawText() const {return text_;}

poRect poTextBox::textBounds() const {return actual_bounds;}

bool poTextBox::textBounds(const std::string &sub, int start, uint *found, poRect *rect) const {
	*found = 0;
	*rect = poRect();
	return false;
}

void poTextBox::text(const std::string &str) {
	text_ = str;
	parsed_text = "";
}

void poTextBox::setRegularFont(poFont *font) {
	setFont(REGULAR_FONT, font);
}

void poTextBox::setItalicFont(poFont *font) {
	setFont(ITALIC_FONT, font);
}

void poTextBox::setBoldFont(poFont *font) {
	setFont(BOLD_FONT, font);
}

void poTextBox::setFont(const std::string &name, poFont *font) {
	if(fonts.find(name) != fonts.end())
		delete resources.remove(fonts[name]);
	
	fonts[name] = resources.add(font->copy());
}

void poTextBox::layout() {
	if(rendered)
		delete resources.remove(rendered);
	
	std::string xml = std::string("<text>") + text_ + "</text>";

	if(doc != NULL)
		delete doc;

	bool is_condensed = TiXmlBase::IsWhiteSpaceCondensed();
	TiXmlBase::SetCondenseWhiteSpace(false);
	
	doc = new TiXmlDocument();
	doc->Parse(xml.c_str(), NULL, TIXML_ENCODING_UTF8);

	TiXmlBase::SetCondenseWhiteSpace(is_condensed);

	parsed_text = "";
	renderTextToTexture(bounds, doc, fonts, &rendered, &parsed_text, &actual_bounds);
	resources.add(rendered);
}

void poTextBox::draw() {
	drawRect(poRect(0,0,bounds.width(),bounds.height()), rendered);
	drawStroke(actual_bounds);
}

