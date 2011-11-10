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
#include "poAttributedString.h"

namespace po {
		
	struct TextLayoutGlyph {
		TextLayoutGlyph() : glyph(0), bbox() {}
		uint glyph;
		poRect bbox;
	};

	struct TextLayoutLine {
		TextLayoutLine() : wordCount(0) {}

		uint getNumGlyphs() const { return glyphs.size(); }
		TextLayoutGlyph &getGlyph(uint g) { return glyphs[g]; }
		
		std::vector<TextLayoutGlyph> glyphs;
		poRect bbox;
		int wordCount;
	};

	class TextLayout {
	public:
		std::string text;
		uint textSize;
		bool isRichText;
		AttributedString parsedText;
		std::vector<TextLayoutLine> lines;
		
				TextLayout();
		void	layout();
		poRect	getTextBounds() const;
		poDictionary getTextPropsAtIndex(int idx);

		uint	getNumLines() const;
		uint	getNumGlyphsForLine(uint line) const;
		
		TextLayoutLine &getLine(uint line);
		TextLayoutGlyph &getGlyphOnLine(uint glyph, uint line);
		poRect	getBoundsForLine(uint line) const;
		poRect	getBoundsForGlyphOnLine(uint glyphIdx, uint line) const;
		
		void	shiftLine(uint line, poPoint p);
		void	rotateLine(uint line, poPoint origin, float rot);
		
		// style corresponds to the tag used to display it
		// ''=regular, 'b'=bold, 'i'=italic, etc
		// you can set any font equal to any tag then use that tag in your text box
<<<<<<< HEAD
		void	font(poFont* f, const std::string &style="");
		poFont*	font(const std::string &style="");
=======
		void	setFont(poFont f, const std::string &style="");
		poFont	getFont(const std::string &style="");
>>>>>>> 7affbf66b041e8753c99c3ceaca61cc0b1724ddd
		bool	hasFont(const std::string &style="");

	protected:
		virtual void doLayout() = 0;
		void	addLine(const TextLayoutLine &line);
		void	replaceLine(int i, const TextLayoutLine &line);
		void	recalculateTextBounds();
		
	private:
		// 1. strip html
		// 2. store ranges
		// 3. build attributed str
		void	prepareText();
		
<<<<<<< HEAD
		std::map<std::string, poFont*> fonts;
		poRect text_bounds;
=======
		std::map<std::string, poFont> fonts;
		poRect textBounds;
>>>>>>> 7affbf66b041e8753c99c3ceaca61cc0b1724ddd
	};
	
}
