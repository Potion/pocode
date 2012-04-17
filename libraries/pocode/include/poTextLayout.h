/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  TextLayout.h
//  pocode
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once


#include "poRect.h"
#include "poFont.h"
#include "poAttributedString.h"


enum {
	PO_USE_TEXTBOX_BOUNDS = 0,
    PO_USE_TEXT_LINE_BOUNDS = 1,
	PO_USE_TEXT_CHARACTER_BOUNDS = 1<<1,
};


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
        
        virtual int getSizeInMemory() { return ( sizeof(TextLayoutLine)+(glyphs.capacity()*sizeof(TextLayoutGlyph)) ); };
        
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

        void    setUseTextBounds( char B ) { useTextBounds = B; };
        
		void	shiftLine(uint line, poPoint p);
		void	rotateLine(uint line, poPoint origin, float rot);
		
		// style corresponds to the tag used to display it
		// ''=regular, 'b'=bold, 'i'=italic, etc
		// you can set any font equal to any tag then use that tag in your text box
		void	setFont(poFont *f, const std::string &style="");
		poFont*	getFont(const std::string &style="");
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
		
		std::map<std::string, poFont*> fonts;
		poRect  textBounds;
        char    useTextBounds;
	};
	
}
