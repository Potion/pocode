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
//  TextBoxLayout.cpp
//  pocode
//
//  Created by Joshua Fisher on 10/12/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poTextBoxLayout.h"

#include "poMath.h"

#include <utf8.h>
#include <boost/foreach.hpp>

namespace po {

    TextBoxLayout::TextBoxLayout(Point s)
    :	size(s)
    ,	tracking(1.f)
    ,	leading(1.f)
    ,	alignment(po::ALIGN_LEFT)
    ,	tabWidth(4)
    ,	padding()
    {}
    
    
    //------------------------------------------------------------------------
    void TextBoxLayout::doLayout() {
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
        dummy_glyph.bbox = Rect();
        
        lineLayoutProps props;
        
        Font *fnt = getFont();
        
        fnt->setPointSize(textSize);
        fnt->cacheGlyphMetrics();       // caches the first time font is used (presently, only works for single poFont size)
        float spacer = 0;
        
        uint lastCodepoint = 0;
        
        std::string::const_iterator ch = str.begin();
        while(ch != str.end()) {
            // if we broke to a new line last pass reset the line height, etc
            if(props.broke) {
                fnt = getFont();
                fnt->setPointSize(textSize);      // this must be set before caching
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
                // get the dictionary for this position
                Dictionary dict = parsedText.attributes(props.glyphCount);
                // keep track of how many glyphs we have total
                props.glyphCount++;
                
                // check if the font has changed
                if(dict.has("font")) {
                    // there's one in hte dictionary
                    Font* tmp = (Font*)dict.getPtr("font");
                    int tmpSize = dict.has("fontSize") ? dict.getInt("fontSize") : textSize;
                    
                    // and it isn't the same as last time
                    if(tmp != fnt || fnt->getPointSize() != tmpSize) {
                        fnt = tmp;
                        fnt->setPointSize(tmpSize);
                        font_changed = true;
                    }
                }
                else {
                    // there's nothing in the dictionary
                    if(fnt != getFont()) {
                        // and the current font isn't the default one
                        fnt = getFont();
                        fnt->setPointSize(textSize);
                        font_changed = true;
                        std::cout << "Changed" << std::endl;
                    }
                }
                
                // do what we need to do when the font switches
                if(font_changed) {
                    fnt->setGlyph(' ');
                    spacer = fnt->getGlyphAdvance().x;
                }
                
                if(dict.has("tracking")) {
                    tracking_tmp = dict.getFloat("tracking");
                }
                
                if(dict.has("leading")) {
                    props.leading = dict.getFloat("leading");
                }
            }
            
            // got to the next codepoint, could be é or § or some other unicode bs, er ... non-english glyph
            uint codepoint = utf8::next(ch, str.end());
            
            Point kern;
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
                        props.lastSpace = (std::floor(w/s) + 1) * s - w;
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
            glyph.bbox.setPosition(glyph.bbox.getPosition() + Point(props.word.width, 0) + fnt->getGlyphBearing() + kern);
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
    
    
    //------------------------------------------------------------------------
    void TextBoxLayout::addWordToLine(lineLayoutProps &props) {
        if(!props.word.glyphs.empty()) {
            props.line.wordCount++;
            for(uint i=0; i<props.word.glyphs.size(); i++) {
                TextLayoutGlyph &glyph = props.word.glyphs[i];
                glyph.bbox.setPosition(glyph.bbox.getPosition() + props.line.bbox.getSize());
                props.line.glyphs.push_back(glyph);
            }
        }
        
        props.line.bbox.width += props.word.width + props.lastSpace;
        props.word = wordLayout();
    }
    
    
    //------------------------------------------------------------------------
    void TextBoxLayout::breakLine(lineLayoutProps &props) {
        if(!props.line.glyphs.empty()) {
            // save the start pos
            float start_y = props.line.bbox.y;
            
            for(uint i=0; i<props.line.glyphs.size(); i++) {
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
            props.line.bbox.y = start_y + round(props.maxLineHeight * props.leading); //Round lineheight * leading to push to whole pixel, otherwise blurry type
        }
        
        props.broke = true;
    }
    
    
    //------------------------------------------------------------------------
    void TextBoxLayout::realignText() {
        
        Rect bounds = getTextBounds();
        
        for(uint i=0; i<lines.size(); i++) {
            TextLayoutLine line = lines[i];
            
            Point glyphOffset(0.f, 0.f);
                    
            switch(alignment) {
                case po::ALIGN_TOP_LEFT:
                    glyphOffset.x = 0;
                    glyphOffset.y = 0;
                    break;
                case po::ALIGN_CENTER_LEFT:
                    glyphOffset.x = 0;
                    glyphOffset.y = (size.y - bounds.height)/2;
                    break;
                case po::ALIGN_BOTTOM_LEFT:
                    glyphOffset.x = 0;
                    glyphOffset.y = (size.y - bounds.height);
                    break;
                    
                case po::ALIGN_TOP_CENTER:
                    glyphOffset.x = (size.x - line.bbox.width)/2;
                    glyphOffset.y = 0;
                    break;
                case po::ALIGN_CENTER_CENTER:
                    glyphOffset.x = (size.x - line.bbox.width)/2;
                    glyphOffset.y = (size.y - bounds.height)/2;
                    break;
                case po::ALIGN_BOTTOM_CENTER:
                    glyphOffset.x = (size.x - line.bbox.width)/2;
                    glyphOffset.y = (size.y - bounds.height);
                    break;
                    
                case po::ALIGN_TOP_RIGHT:
                    glyphOffset.x = (size.x - line.bbox.width); 
                    glyphOffset.y = 0;
                    break;
                case po::ALIGN_CENTER_RIGHT:
                    glyphOffset.x = (size.x - line.bbox.width); 
                    glyphOffset.y = (size.y - bounds.height)/2;
                    break;
                case po::ALIGN_BOTTOM_RIGHT:
                    glyphOffset.x = (size.x - line.bbox.width); 
                    glyphOffset.y = (size.y - bounds.height);
                    break;
            }
            
            glyphOffset.x += padding[PADDING_LEFT];
            glyphOffset.y += padding[PADDING_BOTTOM];
            
            line.bbox.setPosition(line.bbox.getPosition() + glyphOffset);
            
            BOOST_FOREACH(TextLayoutGlyph &glyph, line.glyphs) {
                glyph.bbox.setPosition(glyph.bbox.getPosition() + glyphOffset);
            }
            
            replaceLine(i, line);
        }
        
        recalculateTextBounds();
    }
    
    
    //------------------------------------------------------------------------
    int TextBoxLayout::getSizeInMemory() {
        int S = sizeof( po::TextBoxLayout );
        
        S += text.capacity() * sizeof(char);
        
        // should include AttributedString parsedText;
        // should include std::map<std::string, poFont*> fonts;
        
        for( int i=0; i<lines.size(); i++ )
            S += lines[i].getSizeInMemory();
        
        return S;
    }
}