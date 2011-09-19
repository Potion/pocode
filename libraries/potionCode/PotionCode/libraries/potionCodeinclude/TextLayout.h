//
//  TextLayout.h
//  poFramework4
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "poRect.h"
#include "poFont.h"
#include "AttributedString.h"

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

class TextLayout {
public:
	TextLayout();
	
	std::string text() const;
	void text(const std::string &str);
	
	// style corresponds to the tag used to display it
	// ''=regular, 'b'=bold, 'i'=italic, etc
	// you can set any font equal to any tag then use that tag in your text box
	void font(poFont *f, const std::string &style="");
	poFont *const font(const std::string &style="");
	bool hasFont(const std::string &style="");
	
	virtual void layout();
	
	uint numLines() const;
	poRect  boundsForLine(uint line_num) const;
	int     numLettersForLine(uint line_num);
    poRect& boundsForLetterOnLine( uint letterIndex, uint lineIndex );
    
	bool richText() const;
	void richText(bool b);
	
	layout_line getLine(uint line_num) const;
	poDictionary dictionaryForIndex(int idx);

	poRect textBounds() const;

protected:
	virtual void doLayout() = 0;
	po::AttributedString &parsedText();
	void addLine(const layout_line &line);
	void replaceLine(int i, const layout_line &line);
	void recalculateTextBounds();
	
private:
	// 1. strip html
	// 2. store ranges
	// 3. build attributed str
	void prepareText();
	
	poFontMap fonts;
	std::string _text;
	po::AttributedString _parsed;
	std::vector<layout_line> lines;
	poRect text_bounds;
	bool rich;
};

class TextBoxLayout : public TextLayout {
public:
	TextBoxLayout(poPoint s);
	
	poPoint size() const;
	void size(poPoint s);

	float tracking() const;
	void tracking(float f);
	
	float leading() const;
	void leading(float f);
	
	float paddingLeft() const;
	float paddingRight() const;
	float paddingTop() const;
	float paddingBottom() const;
	void padding(float f);
	void padding(float h, float v);
	void padding(float l, float r, float t, float b);

	int tabWidth() const;
	void tabWidth(int tw);
	
	poAlignment alignment() const;
	void alignment(poAlignment a);
	
protected:
	virtual void doLayout();
	
private:
	struct word_layout {
		word_layout() : width(0.f) {}
		float width;
		std::vector<layout_glyph> glyphs;
	};
	
	struct line_layout_props {
		line_layout_props() : max_line_height(0), max_drop(0), broke(true), glyph_count(0), last_space(0), leading(1) {}
		float max_line_height;
		float max_drop;
		bool broke;
		int glyph_count;
		float last_space;
		float leading;
		word_layout word;
		layout_line line;
	};

	void addWordToLine(line_layout_props &props);
	void breakLine(line_layout_props &props);
	void alignText();

	int tab_width;
	poPoint _size;
	float _tracking;
	float _leading;
	float _padding[4];
	poAlignment _alignment;
};