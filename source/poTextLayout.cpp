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

#include "poTextLayout.h"

#include <pugixml.hpp>

#include <utf8.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

#include "poMath.h"
#include "poHelpers.h"
#include "poDictionary.h"
#include "poResourceStore.h"

#pragma mark - Layout Helper -

struct image_data {
	poImage *image;
	poAlignment align;
};

struct parse_data {
	parse_data(po::TextLayout *layout) : layout(layout) {}
	
	po::TextLayout *layout;
	po::AttributedString string;
	
	std::vector<image_data> images;
};

image_data parseImageNode(const pugi::xml_node &node) {
	image_data img;
	img.image = NULL;
	img.align = PO_ALIGN_TOP_LEFT;
	
//	img.image = getImage(node.attribute("src").value());
	if(img.image != NULL && img.image->isValid()) {
		
	}
	else {
		
	}

	return img;
}

void parseText(const pugi::xml_node &node, parse_data *data) {
	using namespace pugi;
	
	if(!node)
		return;
	
	po::RangeDict range;
	
	if(node.type() == node_element) {
		range.range.start = utf8strlen(data->string.str());

		// if(data->layout->hasFont(node.name())) {
		// 	range.dict.setPtr("font", data->layout->font(node.name()));
		// }
		
		if(!strcmp("u", node.name())) {
			range.dict.set("u",true);
		}
		
		if(!strcmp("img", node.name())) {
			image_data img = parseImageNode(node);
		}

		xml_attribute attrib = node.first_attribute();
		while(attrib) {
			if(!strcmp(attrib.name(),"tracking"))
				range.dict.set("tracking", attrib.as_float());
			else
			if(!strcmp(attrib.name(),"leading"))
				range.dict.set("leading", attrib.as_float());
			else 
			if(!strcmp(attrib.name(),"color")) {
				poColor c;
				if(c.set(attrib.value())) {
					range.dict.set("color", c);
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
		range.range.end = utf8strlen(data->string.str());
		data->string.append(range);
	}
}

#pragma mark - TextLayout -
po::TextLayout::TextLayout()
:	isRichText(false)
,	text("")
,	textSize(12)
,   useTextBounds(PO_USE_TEXTBOX_BOUNDS)
{}

void po::TextLayout::layout() {
    
	prepareText();
	doLayout();
    
}

poRect po::TextLayout::getTextBounds() const {
	return textBounds;
}

poDictionary po::TextLayout::getTextPropsAtIndex(int idx) {
	return parsedText.attributes(idx);
}

uint po::TextLayout::getNumLines() const {
	return lines.size();
}

uint po::TextLayout::getNumGlyphsForLine(uint line) const {
	return lines[line].glyphs.size();
}

po::TextLayoutLine &po::TextLayout::getLine(uint line) {
	return lines[line];
}

po::TextLayoutGlyph &po::TextLayout::getGlyphOnLine(uint glyph, uint line) {
	return lines[line].glyphs[glyph];
}

poRect po::TextLayout::getBoundsForLine(uint line) const {
	return lines[line].bbox;
}

poRect po::TextLayout::getBoundsForGlyphOnLine(uint glyphIdx, uint line) const {
	return lines[line].glyphs[glyphIdx].bbox;
}

void po::TextLayout::shiftLine(uint line, poPoint p) {
	for(uint i=0; i<lines[line].glyphs.size(); i++) {
		poRect &r = lines[line].glyphs[i].bbox;
		r.x += p.x;
		r.y += p.y;
	}
}

void po::TextLayout::rotateLine(uint line, poPoint origin, float rot) {
	float rad = deg2rad(rot);
	float cs = cosf(rad);
	float ss = sinf(rad);
	for(uint i=0; i<lines[line].glyphs.size(); i++) {
		poRect &r = lines[line].glyphs[i].bbox;
		r.x = (r.x - origin.x) * cs + origin.x;
		r.y = (r.y - origin.y) * ss + origin.y;
	}
}

void po::TextLayout::setFont(poFont *f, const std::string &weight) {
	fonts[weight] = f;
}

poFont* po::TextLayout::getFont(const std::string &weight) {
    
    if(fonts.find(weight) == fonts.end() || !hasFont(weight) ) {
        
        
        std::string path;
        
        #ifdef POTION_MAC
            path = "/System/Library/Fonts/Helvetica.dfont";      
            
        
        #elif defined POTION_WIN32        
            wchar_t p[1024];
            GetSystemWindowsDirectory( p,1024 );
            
            char ch[260];
            char defChar = " ";
            
            WideCharToMultiByte( CP_ACP,0,p,-1,ch,260,&DefChar, NULL );
            string path(ch);
            path = path+"\\Fonts\\arial.ttf"        
        #endif
        
        poFont *defaultFont = poGetFont(path,"Regular",12); 
        return defaultFont;
        
    }
    
	return fonts[weight];
}

bool po::TextLayout::hasFont(const std::string &weight) {
	return fonts.find(weight) != fonts.end();
}

void po::TextLayout::addLine(const TextLayoutLine &line) {
	if(!lines.empty())
		textBounds.include(line.bbox);
	else
		textBounds = line.bbox;
	
	lines.push_back(line);
}

void po::TextLayout::replaceLine(int i, const TextLayoutLine &line) {
	lines[i] = line;
}

void po::TextLayout::recalculateTextBounds() {
    
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



void po::TextLayout::prepareText() {
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
