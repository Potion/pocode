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


#pragma mark - Layout Helper -

struct parse_data {
	parse_data(TextLayout *layout) : layout(layout) {}
	
	TextLayout *layout;
	po::AttributedString string;
	std::stack<po::RangeDict> dict;
};

void parseText(const pugi::xml_node &node, parse_data *data) {
	using namespace pugi;
	
	if(!node)
		return;
	
	if(node.type() == node_element) {
		po::RangeDict range;
		range.range.start = utf8strlen(data->string.str());
		
		if(data->layout->hasFont(node.name())) {
			range.dict.setPtr("font", data->layout->font(node.name()));
		}
		else if(!strcmp("u", node.name())) {
			range.dict.setBool("u",true);
		}
		
		xml_attribute attrib = node.first_attribute();
		while(attrib) {
			if(!strcmp(attrib.name(),"tracking")) {
				range.dict.setFloat("tracking", attrib.as_float());
			}
			else 
			if(!strcmp(attrib.name(),"color")) {
				std::istringstream val_str(attrib.value());
				
				poColor c;
				val_str >> c;
				range.dict.setColor("color", c);
			}
			
			attrib = attrib.next_attribute();
		}
		
		data->dict.push(range);
	}
	else 
	if(node.type() == node_pcdata) {
		data->string.append(node.value());
	}
		
	printf("%d: '%s' '%s' '%s'\n-----------\n", node.type(), node.name(), node.value(), node.child_value());
	
	xml_node child = node.first_child();
	while(child) {
		parseText(child, data);
		child = child.next_sibling();
	}
	
	
}

/*
struct StripAndIndex : public TiXmlVisitor {
	StripAndIndex(po::AttributedString &str, TextLayout *layout) 
	:	stripped(str)
	,	layout(layout) 
	{}
	
	po::AttributedString &stripped;
	TextLayout *layout;

	std::stack<po::RangeDict> dicts;
	
	virtual bool VisitEnter(const TiXmlElement &ele, const TiXmlAttribute* attribs) {
		po::RangeDict range;
		range.range.start = utf8strlen(stripped.str());

		if(layout->hasFont(ele.ValueStr())) {
			range.dict.setPtr("font", layout->font(ele.ValueStr()));
		}
		else if(ele.ValueStr() == "u") {
			range.dict.setBool("u", true);
		}
		
		while(attribs) {
			if(attribs->NameTStr() == "tracking") {
				range.dict.setFloat("tracking", attribs->DoubleValue());
			}
			else if(attribs->NameTStr() == "color") {
				std::istringstream val_str(attribs->Value());

				poColor c;
				val_str >> c;
				
				std::cout << c << std::endl;
				
				range.dict.setColor("color", c);
			}
			
			attribs = attribs->Next();
		};

		dicts.push(range);
		
		return true;
	}
	virtual bool VisitExit(const TiXmlElement &ele) {
		po::RangeDict range = dicts.top();
		dicts.pop();

		range.range.end = utf8strlen(stripped.str());
		stripped.append(range);
		
		printf("%s\n", ele.Value());
		return true;
	}
	virtual bool Visit(const TiXmlText &ele) {
		printf("%s", ele.Value());
		stripped.append(ele.ValueStr());
		return true;
	}
};
*/
#pragma mark - TextLayout -
std::string TextLayout::text() const {return _text;}
void TextLayout::text(const std::string &str) {_text = str;}
void TextLayout::font(poFont *f, const std::string &weight) {fonts[weight] = f;}
poFont *const TextLayout::font(const std::string &weight) {return fonts[weight];}
bool TextLayout::hasFont(const std::string &weight) {return fonts.find(weight) != fonts.end();}

uint TextLayout::numLines() const {return lines.size();}
poRect TextLayout::boundsForLine(uint line_num) const {return lines[line_num].bounds;}
layout_line TextLayout::getLine(uint line_num) const {return lines[line_num];}

poRect TextLayout::textBounds() const {return text_bounds;}

void TextLayout::prepareText() {
	// clean up
	lines.clear();
	text_bounds.set(0,0,0,0);

	std::stringstream xml;
	xml << "<span>" << text() << "</span>";
	
	using namespace pugi;

	xml_document doc;
	doc.load(xml, parse_ws_pcdata | parse_cdata, encoding_utf8);
	
	parse_data pd(this);
	parseText(doc.first_child(), &pd);
	
	_parsed = pd.string;
}

void TextLayout::layout() {
	prepareText();
	doLayout();
}

po::AttributedString &TextLayout::parsedText() {return _parsed;}

void TextLayout::addLine(const layout_line &line) {
	if(!lines.empty())
		text_bounds.include(line.bounds);
	else
		text_bounds = line.bounds;
	
	lines.push_back(line);
}

void TextLayout::replaceLine(int i, const layout_line &line) {
	lines[i] = line;
}

void TextLayout::recalculateTextBounds() {
	text_bounds = lines[0].bounds;
	for(int i=1; i<numLines(); i++)
		text_bounds.include(lines[i].bounds);
}

#pragma mark - TextBoxLayout -
TextBoxLayout::TextBoxLayout(poPoint s)
:	_size(s), _tracking(1.f), _leading(-1.f), _alignment(PO_ALIGN_TOP_LEFT)
{
	padding(0.f);
}

void TextBoxLayout::doLayout() {
	using namespace po;
	using namespace std;
	using namespace boost;
	
	AttributedString str = parsedText();
	
	if(str.empty())
		return;

	layout_line line;
	
	poFont *fnt = font();

	bool has_leading = leading() >= 0.f;
	if(!has_leading)
		leading(fnt->lineHeight());
	
	fnt->glyph(' ');

	float spacer = fnt->glyphAdvance().x * tracking();

	poPoint size(0,0);
	vector<layout_glyph> glyphs;
	
	std::string::const_iterator ch = str.begin();
	while(ch != str.end()) {
		uint codepoint = utf8::next(ch, str.end());
		
		if(codepoint == ' ') {
			size.x += spacer;
			addGlyphsToLine(glyphs, size, line);
			continue;
		}
		if(codepoint == '\n') {
			addGlyphsToLine(glyphs, size, line);
			breakLine(line);
			continue;
		}
		else if(codepoint == '\t') {
			size.x += spacer * 4;
			continue;
		}
		
		fnt->glyph(codepoint);
		
		poPoint kern(0,0);
		if(fnt->hasKerning() && !glyphs.empty()) {
			kern = fnt->kernGlyphs(glyphs.back().glyph, codepoint);
		}
		
		layout_glyph glyph;
		glyph.glyph = codepoint;
		glyph.bbox = fnt->glyphBounds();
		glyph.bbox.origin += poPoint(size.x-kern.x, 0) + fnt->glyphBearing();
		glyphs.push_back(glyph);
		
		size.x += (fnt->glyphAdvance().x - kern.x) * tracking();
		size.y = std::max(glyph.bbox.origin.y + glyph.bbox.size.y, size.y);
		
		if(size.x + line.bounds.size.x > (_size.x-paddingLeft()-paddingRight()) && line.word_count >= 1) {
			line.bounds.size.x -= spacer;
			breakLine(line);
		}
	}

	addGlyphsToLine(glyphs, size, line);
	breakLine(line);
	alignText();
	
	if(!has_leading)
		leading(-1);
}

poPoint TextBoxLayout::size() const {return _size;}
void TextBoxLayout::size(poPoint s) {_size = s;}
float TextBoxLayout::tracking() const {return _tracking;}
void TextBoxLayout::tracking(float f) {_tracking = f;}
float TextBoxLayout::leading() const {return _leading;}
void TextBoxLayout::leading(float f) {_leading = f;}
float TextBoxLayout::paddingLeft() const {return _padding[0];}
float TextBoxLayout::paddingRight() const {return _padding[1];}
float TextBoxLayout::paddingTop() const {return _padding[2];}
float TextBoxLayout::paddingBottom() const {return _padding[3];}
void TextBoxLayout::padding(float f) {_padding[0] = _padding[1] = _padding[2] = _padding[3] = f;}
void TextBoxLayout::padding(float h, float v) {_padding[0] = _padding[1] = h; _padding[2] = _padding[3] = v;}
void TextBoxLayout::padding(float l, float r, float t, float b) {_padding[0] = l; _padding[1] = r; _padding[2] = t; _padding[3] = b;}
poAlignment TextBoxLayout::alignment() const {return _alignment;}
void TextBoxLayout::alignment(poAlignment a) {_alignment = a;}

void TextBoxLayout::addGlyphsToLine(std::vector<layout_glyph> &glyphs, poPoint &size, layout_line &line) {
	BOOST_FOREACH(layout_glyph &glyph, glyphs) {
		glyph.bbox.origin = glyph.bbox.origin + poPoint(line.bounds.size.x, line.bounds.origin.y);
		line.glyphs.push_back(glyph);
	}
	
	line.bounds.size.x += size.x;
	line.bounds.size.y = std::max(line.bounds.size.y, size.y);
	line.word_count += 1;
	
	glyphs.clear();
	size.set(0,0,0);
}

void TextBoxLayout::breakLine(layout_line &line) {
	addLine(line);

	line = layout_line();
	line.bounds.origin.y = numLines() * leading();
}

void TextBoxLayout::alignText() {
	poRect text_bounds = textBounds();

	for(int i=0; i<numLines(); i++) {
		layout_line line = getLine(i);
		
		poPoint glyphOffset(0.f, 0.f);
		
		switch(_alignment) {
			case PO_ALIGN_TOP_LEFT:
				break;
			case PO_ALIGN_TOP_CENTER:
				glyphOffset.x = (_size.x - line.bounds.size.x)/2;
				break;
			case PO_ALIGN_TOP_RIGHT:
				glyphOffset.x = (_size.x - line.bounds.size.x); 
				break;
			case PO_ALIGN_CENTER_LEFT:
				glyphOffset.y = (_size.y - text_bounds.height())/2;
				break;
			case PO_ALIGN_CENTER_CENTER:
				glyphOffset.x = (_size.x - line.bounds.size.x)/2;
				glyphOffset.y = (_size.y - text_bounds.height())/2;
				break;
			case PO_ALIGN_CENTER_RIGHT:
				glyphOffset.x = (_size.x - line.bounds.size.x); 
				glyphOffset.y = (_size.y - text_bounds.height())/2;
				break;
			case PO_ALIGN_BOTTOM_LEFT:
				glyphOffset.y = (_size.y - text_bounds.height());
				break;
			case PO_ALIGN_BOTTOM_CENTER:
				glyphOffset.x = (_size.x - line.bounds.size.x)/2; 
				glyphOffset.y = (_size.y - text_bounds.height());
				break;
			case PO_ALIGN_BOTTOM_RIGHT:
				glyphOffset.x = (_size.x - line.bounds.size.x); 
				glyphOffset.y = (_size.y - text_bounds.height());
				break;
		}
		
		glyphOffset.x += paddingLeft();
		glyphOffset.y += paddingBottom();
		
		line.bounds.origin += glyphOffset;
		
		BOOST_FOREACH(layout_glyph &glyph, line.glyphs) {
			glyph.bbox.origin = round(glyph.bbox.origin + glyphOffset - poPoint(GLYPH_PADDING,GLYPH_PADDING));
		}
		
		replaceLine(i, line);
	}
	
	recalculateTextBounds();
}


