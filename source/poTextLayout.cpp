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
//  TextLayout.cpp
//  pocode
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//


#include <pugixml.hpp>

#include <utf8.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

#include "poMath.h"
#include "poHelpers.h"
#include "poDictionary.h"
#include "poResourceStore.h"
#include <iostream>
using namespace std;

namespace po {
    
    // -----------------------------------------------------------------------------------
    // ================================ Utils ============================================
    #pragma mark - Utils -

    struct parse_data {
        parse_data(TextLayout *l)
        : layout(l)
        {}
        
        TextLayout *layout;
        po::AttributedString string;
    };
    
    
    //------------------------------------------------------------------------
    void parseText(const pugi::xml_node &node, parse_data *data) {
        using namespace std;
        using namespace pugi;
        
        if(!node)
            return;
        
        int start;
        Dictionary dict;
        
        if(node.type() == node_element) {
            start = po::utf8strlen(data->string.str());

            if(data->layout->hasFont(node.name())) {
                dict.set("font", data->layout->getFont(node.name()));
            }
            
            if(!strcmp("u", node.name())) {
                dict.set("u",true);
            }
            
    //		if(!strcmp("img", node.name())) {
    //			image_data img = parseImageNode(node);
    //		}

            xml_attribute attrib = node.first_attribute();
            while(attrib) {
                if(!strcmp(attrib.name(),"tracking"))
                    dict.set("tracking", attrib.as_float());
                else
                if(!strcmp(attrib.name(),"leading"))
                    dict.set("leading", attrib.as_float());
                else 
                if(!strcmp(attrib.name(),"color")) {
                    Color c;
                    if(c.set(attrib.value())) {
                        dict.set("color", c);
                    }
                }
                
                attrib = attrib.next_attribute();
            }
        }
        else 
        if(node.type() == node_pcdata) {
            data->string.append(node.value());
        }
        
        xml_node child = node.first_child();
        while(child) {
            parseText(child, data);
            child = child.next_sibling();
        }
        
        if(node.type() == node_element) {
            int end = po::utf8strlen(data->string.str());
            data->string.append(po::Range(start,end), dict);
        }
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: Text Layout ===============================
    #pragma mark - Text Layout -

    TextLayout::TextLayout()
    :	isRichText(false)
    ,	text("")
    ,	textSize(12)
    ,   useTextBounds(PO_USE_TEXTBOX_BOUNDS)
    {}

    void TextLayout::layout() {
        
        prepareText();
        doLayout();
        
    }
    
    
    //------------------------------------------------------------------------
    Rect TextLayout::getTextBounds() const {
        return textBounds;
    }
    
    
    //------------------------------------------------------------------------
    Dictionary TextLayout::getTextPropsAtIndex(int idx) {
        return parsedText.attributes(idx);
    }
    
    
    //------------------------------------------------------------------------
    uint TextLayout::getNumLines() const {
        return lines.size();
    }
    
    
    //------------------------------------------------------------------------
    uint TextLayout::getNumGlyphsForLine(uint line) const {
        return lines[line].glyphs.size();
    }
    
    
    //------------------------------------------------------------------------
    TextLayoutLine &TextLayout::getLine(uint line) {
        return lines[line];
    }
    
    
    //------------------------------------------------------------------------
    TextLayoutGlyph &TextLayout::getGlyphOnLine(uint glyph, uint line) {
        return lines[line].glyphs[glyph];
    }
    
    
    //------------------------------------------------------------------------
    Rect TextLayout::getBoundsForLine(uint line) const {
        return lines[line].bbox;
    }
    
    
    //------------------------------------------------------------------------
    Rect TextLayout::getBoundsForGlyphOnLine(uint glyphIdx, uint line) const {
        return lines[line].glyphs[glyphIdx].bbox;
    }
    
    
    //------------------------------------------------------------------------
    void TextLayout::shiftLine(uint line, Point p) {
        for(uint i=0; i<lines[line].glyphs.size(); i++) {
            Rect &r = lines[line].glyphs[i].bbox;
            r.x += p.x;
            r.y += p.y;
        }
    }
    
    
    //------------------------------------------------------------------------
    void TextLayout::rotateLine(uint line, Point origin, float rot) {
        float rad = deg2rad(rot);
        float cs = cosf(rad);
        float ss = sinf(rad);
        for(uint i=0; i<lines[line].glyphs.size(); i++) {
            Rect &r = lines[line].glyphs[i].bbox;
            r.x = (r.x - origin.x) * cs + origin.x;
            r.y = (r.y - origin.y) * ss + origin.y;
        }
    }
    
    
    //------------------------------------------------------------------------
    void TextLayout::setFont(Font *f, const std::string &weight) {
        fonts[weight] = f;
    }
    
    
    //------------------------------------------------------------------------
    Font* TextLayout::getFont(const std::string &weight) {
        if(fonts.find(weight) == fonts.end() || !hasFont(weight) )
            return NULL;
        
        return fonts[weight];
    }
    
    
    //------------------------------------------------------------------------
    bool TextLayout::hasFont(const std::string &weight) {
        return fonts.find(weight) != fonts.end();
    }
    
    
    //------------------------------------------------------------------------
    void TextLayout::addLine(const TextLayoutLine &line) {
        if(!lines.empty())
            textBounds.include(line.bbox);
        else
            textBounds = line.bbox;
        
        lines.push_back(line);
    }
    
    
    //------------------------------------------------------------------------
    void TextLayout::replaceLine(int i, const TextLayoutLine &line) {
        lines[i] = line;
    }
    
    
    //------------------------------------------------------------------------
    void TextLayout::recalculateTextBounds() {
        
        if ( useTextBounds == PO_USE_TEXTBOX_BOUNDS )
        {
            // use same as line bounds for now
            // could just be the box size
            if(!lines.empty()) {
                textBounds = lines[0].bbox;
                for(uint i=1; i<getNumLines(); i++)
                {
                    textBounds.include(lines[i].bbox);
                }
            }
        }
        else if ( useTextBounds == PO_USE_TEXT_LINE_BOUNDS )
        {
            if(!lines.empty()) {
                textBounds = lines[0].bbox;
                for(uint i=1; i<getNumLines(); i++)
                {
                    textBounds.include(lines[i].bbox);
                }
            }
        }
        else if ( useTextBounds == PO_USE_TEXT_CHARACTER_BOUNDS )
        {
            if( !lines.empty() && lines[0].glyphs.size() > 0 ) 
            {
                textBounds = lines[0].glyphs[0].bbox;
                for(uint i=0; i<getNumLines(); i++)
                {
                    BOOST_FOREACH(TextLayoutGlyph &glyph, lines[i].glyphs) 
                    {
                        textBounds.include( glyph.bbox );
                    }
                }
            } 
        }
    }
    
    
    //------------------------------------------------------------------------
    void TextLayout::prepareText() {
        // clean up
        lines.clear();
        textBounds.set(0,0,0,0);
        
        // take the user's word for it that this is rich text
        if(isRichText) {
            std::stringstream xml;
            xml << "<SPECIAL_HOLDER_TAG_PLEASE_IGNORE>" << text << "</SPECIAL_HOLDER_TAG_PLEASE_IGNORE>";
            
            using namespace pugi;

            xml_document doc;
            doc.load(xml, parse_ws_pcdata | parse_cdata, encoding_utf8);
            
            parse_data pd(this);
            parseText(doc.first_child(), &pd);
            
            parsedText = pd.string;
        }
        // otherwise blithly pretend there isn't any xml in there
        else {
            parsedText.str() = text;
        }
    }
} /* End po Namespace */