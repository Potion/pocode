//
//  TextBoxLayout.cpp
//  potionCode
//
//  Created by Joshua Fisher on 10/12/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poTextBoxLayout.h"

#include <utf8.h>
#include "poMath.h"

po::TextBoxLayout::TextBoxLayout(poPoint s)
:	size(s)
,	tracking(1.f)
,	leading(1.f)
,	alignment(PO_ALIGN_LEFT)
,	tabWidth(4)
,	padding()
{
}

void po::TextBoxLayout::doLayout() {
	using namespace po;
	using namespace std;
	using namespace boost;
	
	AttributedString str = parsedText;
	
	if(str.empty())
		return;
	
	// will add this in to the top and bottom when a glyph excedes the default line height
	float line_padding_fudge = 2;
	
	TextLayoutGlyph dummy_glyph;
	dummy_glyph.glyph = ' ';
	dummy_glyph.bbox = poRect();
	
	lineLayoutProps props;
	
	poFont *fnt = getFont();
	float spacer = 0;
	
	uint lastCodepoint = 0;
	
	std::string::const_iterator ch = str.begin();
	while(ch != str.end()) {
		// if we broke to a new line last pass reset the line height, etc
		if(props.broke) {
			fnt = getFont();
			fnt->setPointSize(textSize);
			fnt->setGlyph(' ');
			
			spacer = fnt->getGlyphAdvance().x;
			
			props.maxLineHeight = fnt->getLineHeight();
			props.maxDrop = fnt->getAscender();
			props.leading = leading;
			
			props.broke = false;
		}
		
		// keep track of local changes to tracking
		// could be overidden in rich text
		float tracking_tmp = tracking;
		
		// maybe the font changed
		bool font_changed = false;
		
		// if we're parsing the fanciness
		if(isRichText) {
//			// get the dictionary for this position
//			poDictionary dict = parsedText.attributes(props.glyphCount);
//			// keep track of how many glyphs we have total
//			props.glyphCount++;
//			
//			// check if the font has changed
//			if(dict.has("font")) {
//				// there's one in hte dictionary
//				poFont* tmp = (poFont*)dict.getPtr("font");
//				int tmpSize = dict.has("fontSize") ? dict.getInt("fontSize") : textSize;
//				
//				// and it isn't the same as last time
//				if(tmp != fnt && fnt->getPointSize() != tmpSize) {
//					fnt = tmp;
//					fnt->setPointSize(tmpSize);
//					font_changed = true;
//				}
//			}
//			else {
//				// there's nothing in the dictionary
//				if(fnt != getFont()) {
//					// and the current font isn't the default one
//					fnt = getFont();
//					fnt->setPointSize(textSize);
//					font_changed = true;
//				}
//			}
//			
//			// do what we need to do when the font switches
//			if(font_changed) {
//				fnt->setGlyph(' ');
//				spacer = fnt->getGlyphAdvance().x;
//			}
//			
//			if(dict.has("tracking")) {
//				tracking_tmp = dict.getDouble("tracking");
//			}
//			
//			if(dict.has("leading")) {
//				props.leading = dict.getDouble("leading");
//			}
		}
		
		// got to the next codepoint, could be é or § or some other unicode bs, er ... non-english glyph
		uint codepoint = utf8::next(ch, str.end());
		
		poPoint kern;
		if(lastCodepoint != 0 && fnt->hasKerning() && !font_changed) {
			kern = fnt->kernGlyphs(lastCodepoint, codepoint);
		}
		
		// handle whitespace specially
		// any whitespace indicates a new word
		if(::iswspace(codepoint)) {
			switch(codepoint) {
				case ' ':
					props.lastSpace = spacer;
					break;
					
				case '\n':
					props.broke = true;
					break;
					
				case '\t':
				{
					float s = spacer * tabWidth;
					float w = props.line.bbox.width + props.word.width;
					props.lastSpace = (floor(w/s) + 1) * s - w;
					break;
				}
			}
			
			// make the line knows about the space
			props.lastSpace *= tracking_tmp;
			// put a dummy glyph in there for indexing the properties dictionary
			// we have to keep glyphs and codepoints at 1:1
			props.word.glyphs.push_back(dummy_glyph);
			// dump the word into the line
			addWordToLine(props);
			
			// if the whitespace indicated a linebreak, then do so
			if(props.broke)
				breakLine(props);
			
			continue;
		}
		
		// change glyph
		fnt->setGlyph(codepoint);
		
		// store all the info we need to render
		TextLayoutGlyph glyph;
		glyph.glyph = codepoint;
		glyph.bbox = fnt->getGlyphBounds();
        glyph.bbox.setPosition(glyph.bbox.getPosition() + poPoint(props.word.width, 0) + fnt->getGlyphBearing() + kern);
		props.word.glyphs.push_back(glyph);
		
		// see if we need to update our line height
		props.maxDrop = std::max(props.maxDrop, -fnt->getGlyphBearing().y + line_padding_fudge);
		props.maxLineHeight = std::max(props.maxLineHeight, props.maxDrop + fnt->getGlyphDescender() + line_padding_fudge);
		
		// update the pen x position
		props.word.width += (fnt->getGlyphAdvance().x + kern.x) * tracking_tmp;
		
		// check if we've gone over
		if(props.line.bbox.width + props.word.width > (size.x-padding[PADDING_LEFT]-padding[PADDING_RIGHT]) && props.line.wordCount >= 1) {
			breakLine(props);
		}
	}
	
	// just in case, make sure the current glyphs get onto the last line
	addWordToLine(props);
	breakLine(props);
}

void po::TextBoxLayout::addWordToLine(lineLayoutProps &props) {
	if(!props.word.glyphs.empty()) {
		props.line.wordCount++;
		for(int i=0; i<props.word.glyphs.size(); i++) {
			TextLayoutGlyph &glyph = props.word.glyphs[i];
            glyph.bbox.setPosition(glyph.bbox.getPosition() + props.line.bbox.getSize());
			props.line.glyphs.push_back(glyph);
		}
	}
	
	props.line.bbox.width += props.word.width + props.lastSpace;
	props.word = wordLayout();
}

void po::TextBoxLayout::breakLine(lineLayoutProps &props) {
	if(!props.line.glyphs.empty()) {
		// save the start pos
		float start_y = props.line.bbox.y;
		
		for(int i=0; i<props.line.glyphs.size(); i++) {
			TextLayoutGlyph &glyph = props.line.glyphs[i];
			// move the glyph down to the baseline + the start position
			glyph.bbox.y += props.maxDrop + start_y;
		}
		
		// words all end with a space, so take off the space for it
		props.line.bbox.width -= props.lastSpace;
		// and bump the line height
		props.line.bbox.height = props.maxLineHeight;
		// save the line
		addLine(props.line);
		
		// and set up hte next line
		props.line = TextLayoutLine();
		props.line.bbox.y = start_y + props.maxLineHeight * props.leading;
	}
	
	props.broke = true;
}

void po::TextBoxLayout::realignText() {
	
    poRect bounds = getTextBounds();
    
	for(int i=0; i<lines.size(); i++) {
		TextLayoutLine line = lines[i];
		
		poPoint glyphOffset(0.f, 0.f);
		        
		switch(alignment) {
			case PO_ALIGN_TOP_LEFT:
                glyphOffset.x = 0;
                glyphOffset.y = 0;
                break;
			case PO_ALIGN_CENTER_LEFT:
                glyphOffset.x = 0;
                glyphOffset.y = (size.y - bounds.height)/2;
                break;
			case PO_ALIGN_BOTTOM_LEFT:
                glyphOffset.x = 0;
                glyphOffset.y = (size.y - bounds.height);
				break;
                
			case PO_ALIGN_TOP_CENTER:
                glyphOffset.x = (size.x - line.bbox.width)/2;
                glyphOffset.y = 0;
                break;
			case PO_ALIGN_CENTER_CENTER:
                glyphOffset.x = (size.x - line.bbox.width)/2;
                glyphOffset.y = (size.y - bounds.height)/2;
                break;
			case PO_ALIGN_BOTTOM_CENTER:
				glyphOffset.x = (size.x - line.bbox.width)/2;
				glyphOffset.y = (size.y - bounds.height);
                break;
                
			case PO_ALIGN_TOP_RIGHT:
                glyphOffset.x = (size.x - line.bbox.width); 
                glyphOffset.y = 0;
                break;
			case PO_ALIGN_CENTER_RIGHT:
                glyphOffset.x = (size.x - line.bbox.width); 
                glyphOffset.y = (size.y - bounds.height)/2;
                break;
			case PO_ALIGN_BOTTOM_RIGHT:
				glyphOffset.x = (size.x - line.bbox.width); 
                glyphOffset.y = (size.y - bounds.height);
				break;
		}
		
		glyphOffset.x += padding[PADDING_LEFT];
		glyphOffset.y += padding[PADDING_BOTTOM];
		
		line.bbox.setPosition(round(line.bbox.getPosition() + glyphOffset));
		
		BOOST_FOREACH(TextLayoutGlyph &glyph, line.glyphs) {
			glyph.bbox.setPosition(round(glyph.bbox.getPosition() + glyphOffset));
		}
		
		replaceLine(i, line);
	}
	
	recalculateTextBounds();
}
