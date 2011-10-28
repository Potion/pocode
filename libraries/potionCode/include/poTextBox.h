//
//  poTextBox.h
//  poFramework4
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poObject.h"
#include "TextBoxLayout.h"

#include "poFont.h"
#include "poBitmapFont.h"

// for caching
class poFBO;

static const std::string PO_TEXT_REGULAR = "";
static const std::string PO_TEXT_ITALIC = "i";
static const std::string PO_TEXT_BOLD = "b";
static const std::string PO_TEXT_BOLD_ITALIC = "bi";

enum {
	PO_TEXT_BOX_STROKE_BOUNDS = 1,
	PO_TEXT_BOX_STROKE_TEXT_BOUNDS = 1<<1,
	PO_TEXT_BOX_STROKE_LINES = 1<<2,
	PO_TEXT_BOX_STROKE_GLYPHS = 1<<3,
	PO_TEXT_BOX_STROKE_ALL = PO_TEXT_BOX_STROKE_BOUNDS | PO_TEXT_BOX_STROKE_TEXT_BOUNDS | PO_TEXT_BOX_STROKE_LINES | PO_TEXT_BOX_STROKE_GLYPHS
};

// CLASS NOTES
//
// poTextBox derives directly from poObject. It is one of the most commonly used objects in potionCode.
// 
// A simple poTextBox may be constructed as follows:
// 
//      poTextBox* TB = new poTextBox( 200,300 );
//      TB->text( "hello world!" );
//      TB->font( getFont("Courier", 20) );
//      TB->textAlignment( PO_ALIGN_TOP_CENTER );
//      TB->textColor = poColor::black;
//      TB->layout();
//
// You MUST call the layout() method in order to see any changes in your poTextBox take effect.
//
// The text that is passed into the text() method may be formatted using basic HTML tags as follows:
//
//      TB-text( "This is in <b size='10'>bold</b>.This is in <i>italic</i>." );
//
// You will also need to specify the bold and italic fonts explicitly using the font method.
// The richText setting must be on to see the effects on the text.


class poTextBox
:	public poObject 
{
public:
	poTextBox();
	poTextBox(int w);
	poTextBox(int w, int h);
	virtual ~poTextBox();
	
	virtual poObject* copy();
	
    // LAYOUT
    // You MUST call layout after making any changes to poTextBox. Otherwise, you will not see
    // the changes in the text box.
    void				layout();

    // TEXTBOX PROPERTIES
	std::string         text() const;
	void				text(const std::string &str);
	
	uint				textSize() const;
	void				textSize(uint ptSize);
	
    // The font methods tell the textBox which fonts to use. If a single font is provided as follows:
    //        TB->font( getFont("Courier", 20) );
    // This font will be used for all text, and acts as the PO_TEXT_REGULAR font.
    // Bold and italic fonts may set as follows:
    //        TB->font( getFont("Courier Bold"), PO_TEXT_BOLD );
    //        TB->font( getFont("Courier Italic"), PO_TEXT_ITALIC );
	// These fonts will be used where the <b> and <i> HTML tags appear in the text.
	void                font(poFont font, const std::string &name=PO_TEXT_REGULAR);
	poFont				font(const std::string &name=PO_TEXT_REGULAR);
    
    // textAlignment may be set using one of the poAlignment enums found in "poEnums.h".
    // Commonly used settings are PO_ALIGN_TOP_LEFT, PO_ALIGN_TOP_CENTER and PO_ALIGN_CENTER_CENTER.
	poAlignment         textAlignment() const;
	void                textAlignment(poAlignment al);
	
    // This is the textColor for all of the text. To set the text color by word or by letter,
    // use the color property in rich text mode.
	poColor             textColor;

    // The padding determine the amount of space between the bounds of the text box
    // and the actual text. It may be set for all 4 sides at once, or per side.
    void                padding(float f);
	void                padding(float h, float v);
	void                padding(float l, float r, float t, float b);
    float               paddingLeft() const;
	float               paddingRight() const;
	float               paddingTop() const;
	float               paddingBottom() const;
	
    // The tabWidth determines the width of a tab in spaces. A setting of 5 means the tab
    // will be 5 spaces wide.
	void                tabWidth(int tw);
	int                 tabWidth() const;

    // The richText mode enables specifc text settings within the text. For example, setting
    // the color of a single world or line of text. By default, richText mode is off.
    bool                richText() const;
	void                richText(bool b);

    // Leading is the spacing between lines of text. Tracking is the spacing between characters.
    // These properties can be overridden inline with a span as follows:
	//      <span leading='.9' tracking='1.1'>leaded and tracked differently</span>
	// There can be only 1 leading setting for a single line of text.
	float               leading() const;
	void                leading(float f);
	float               tracking() const;
	void                tracking(float f);
    
    // TEXTBOX BOUNDS
    // The "reshape" methods set the official bounds of the text box that are used by the layout() method.
    // The "textBounds" methods returns the actual bounds of the text after is has been layed out.
    // So, if the text does not fill the whole textbox, the bounds will differ.
    void                reshape(int w, int h);
	void                reshape(poPoint p);
	poRect              textBounds() const;
	
    // LINES OF TEXT
	uint                numLines() const;
	poRect              boundsForLine(uint num) const;
    int                 numLettersForLine( int lineIndex );
    poRect              getBoundsForLetterOnLine( int letterIndex, int lineIndex );
    void                setBoundsForLetterOnLine( int letterIndex, int lineIndex, poRect newBounds );
	
	bool				getCacheToTexture() const;
	void				setCacheToTexture(bool b);
	
    // DEBUGGING
    // The bounds of a textBox can be shown by setting drawBounds to 1. Setting it to 0 will hide the bounds.
	// You can use the PO_TEXT_BOX_STROKE_XXXXX settings in the enum defined at the top of this file.
    virtual void	_drawBounds();
	
    // DRAWING
    // The draw() method is called automatically if a text box is added to the scene graph.
    // You should not need to call the draw() method yourself.
	void                draw();

protected:
	void				clone(poTextBox *tb);
	
private:
    void                generateCachedTexture();
    
	bool                fit_height_to_bounds;
	bool				cacheToTexture;
    bool                layoutDone;
	poAlignment         text_align;
	po::TextBoxLayout   _layout;
	poTexture			cached;
};


