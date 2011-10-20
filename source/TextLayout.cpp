//
//  TextLayout.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "TextLayout.h"

#include <pugixml.hpp>

#include <utf8.h>
#include <boost/tokenizer.hpp>

#include "poMath.h"
#include "Helpers.h"
#include "poDictionary.h"
#include "poResourceLoader.h"

#pragma mark - Layout Helper -

struct image_data {
	poImage image;
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
	img.image = poImage();
	img.align = PO_ALIGN_TOP_LEFT;
	
	img.image = getImage(node.attribute("src").value());
	if(img.image.isValid()) {
		
	}
	else {
		
	}
}

void parseText(const pugi::xml_node &node, parse_data *data) {
	using namespace pugi;
	
	if(!node)
		return;
	
	po::RangeDict range;
	
	if(node.type() == node_element) {
		range.range.start = utf8strlen(data->string.str());

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
{}

void po::TextLayout::layout() {
	prepareText();
	doLayout();
}

poRect po::TextLayout::textBounds() const {
	return text_bounds;
}

poDictionary po::TextLayout::textPropsAtIndex(int idx) {
	return parsedText.attributes(idx);
}

uint po::TextLayout::numLines() const {
	return lines.size();
}

uint po::TextLayout::numGlyphsForLine(uint line) const {
	return lines[line].glyphs.size();
}

po::TextLayoutLine &po::TextLayout::getLine(uint line) {
	return lines[line];
}

po::TextLayoutGlyph &po::TextLayout::getGlyphOnLine(uint glyph, uint line) {
	return lines[line].glyphs[glyph];
}

poRect po::TextLayout::boundsForLine(uint line) const {
	return lines[line].bbox;
}

poRect po::TextLayout::boundsForGlyphOnLine(uint glyphIdx, uint line) const {
	return lines[line].glyphs[glyphIdx].bbox;
}

void po::TextLayout::shiftLine(uint line, poPoint p) {
	for(int i=0; i<lines[line].glyphs.size(); i++) {
		poRect &r = lines[line].glyphs[i].bbox;
		r.x += p.x;
		r.y += p.y;
	}
}

void po::TextLayout::rotateLine(uint line, poPoint origin, float rot) {
	float rad = deg2rad(rot);
	float cs = cosf(rad);
	float ss = sinf(rad);
	for(int i=0; i<lines[line].glyphs.size(); i++) {
		poRect &r = lines[line].glyphs[i].bbox;
		r.x = (r.x - origin.x) * cs + origin.x;
		r.y = (r.y - origin.y) * ss + origin.y;
	}
}

void po::TextLayout::font(poFont f, const std::string &weight) {
	fonts[weight] = f;
}

poFont po::TextLayout::font(const std::string &weight) {
	return fonts[weight];
}

bool po::TextLayout::hasFont(const std::string &weight) {
	return fonts.find(weight) != fonts.end();
}

void po::TextLayout::addLine(const TextLayoutLine &line) {
	if(!lines.empty())
		text_bounds.include(line.bbox);
	else
		text_bounds = line.bbox;
	
	lines.push_back(line);
}

void po::TextLayout::replaceLine(int i, const TextLayoutLine &line) {
	lines[i] = line;
}

void po::TextLayout::recalculateTextBounds() {
	if(!lines.empty()) {
		text_bounds = lines[0].bbox;
		for(int i=1; i<numLines(); i++)
			text_bounds.include(lines[i].bbox);
	}
}

void po::TextLayout::prepareText() {
	// clean up
	lines.clear();
	text_bounds.set(0,0,0,0);

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
