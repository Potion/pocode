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
#include "poResourceStore.h"


#pragma mark - Layout Helper -

struct image_data {
	poImage *image;
	poAlignment align;
};

struct parse_data {
	parse_data(TextLayout *layout) : layout(layout) {}
	
	TextLayout *layout;
	po::AttributedString string;
	
	std::vector<image_data> images;
};

image_data parseImageNode(const pugi::xml_node &node) {
	image_data img;
	img.image = NULL;
	img.align = PO_ALIGN_TOP_LEFT;
	
	img.image = getImage(node.attribute("src").value());
	if(img.image) {
		
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

		if(data->layout->hasFont(node.name())) {
			range.dict.setPtr("font", data->layout->font(node.name()));
		}
		
		if(!strcmp("u", node.name())) {
			range.dict.setBool("u",true);
		}
		
		if(!strcmp("img", node.name())) {
			image_data img = parseImageNode(node);
		}

		xml_attribute attrib = node.first_attribute();
		while(attrib) {
			if(!strcmp(attrib.name(),"tracking"))
				range.dict.setFloat("tracking", attrib.as_float());
			else
			if(!strcmp(attrib.name(),"leading"))
				range.dict.setFloat("leading", attrib.as_float());
			else 
			if(!strcmp(attrib.name(),"color")) {
				poColor c;
				if(c.set(attrib.value())) {
					range.dict.setColor("color", c);
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
TextLayout::TextLayout()
:	rich(false)
{}

std::string TextLayout::text() const {return _text;}
void TextLayout::text(const std::string &str) {_text = str;}
void TextLayout::font(poFont *f, const std::string &weight) {fonts[weight] = f;}
poFont *const TextLayout::font(const std::string &weight) {return fonts[weight];}
bool TextLayout::hasFont(const std::string &weight) {return fonts.find(weight) != fonts.end();}

uint TextLayout::numLines() const {return lines.size();}
poRect TextLayout::boundsForLine(uint line_num) const {return lines[line_num].bounds;}
layout_line TextLayout::getLine(uint line_num) const {return lines[line_num];}

bool TextLayout::richText() const {return rich;}
void TextLayout::richText(bool b) {rich = b;}

poDictionary TextLayout::dictionaryForIndex(int idx) {
	return _parsed.attributes(idx);
}

poRect TextLayout::textBounds() const {return text_bounds;}

void TextLayout::prepareText() {
	// clean up
	lines.clear();
	text_bounds.set(0,0,0,0);

	std::stringstream xml;
	xml << "<SPECIAL_HOLDER_TAG_PLEASE_IGNORE>" << text() << "</SPECIAL_HOLDER_TAG_PLEASE_IGNORE>";
	
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
	if(!lines.empty()) {
		text_bounds = lines[0].bounds;
		for(int i=1; i<numLines(); i++)
			text_bounds.include(lines[i].bounds);
	}
}

#pragma mark - TextBoxLayout -
TextBoxLayout::TextBoxLayout(poPoint s)
:	_size(s), _tracking(1.f), _leading(1.f), _alignment(PO_ALIGN_LEFT), tab_width(4)
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

	// will add this in to the top and bottom when a glyph excedes the default line height
	float line_padding_fudge = 2;

	layout_glyph dummy_glyph;
	dummy_glyph.glyph = ' ';
	dummy_glyph.bbox = poRect();

	line_layout_props props;
	
	poFont *fnt = NULL;
	float spacer = 0;
	
	std::string::const_iterator ch = str.begin();
	while(ch != str.end()) {
		// if we broke to a new line last pass reset the line height, etc
		if(props.broke) {
			fnt = font();
			fnt->glyph(' ');

			props.max_line_height = fnt->lineHeight();
			props.max_drop = fnt->ascender();
			props.leading = leading();
			
			props.broke = false;
			
			spacer = fnt->glyphAdvance().x;
		}

		// keep track of local changes to tracking
		// could be overidden in rich text
		float tracking_tmp = tracking();
		
		// if we're parsing the fanciness
		if(richText()) {
			// get the dictionary for this position
			poDictionary dict = parsedText().attributes(props.glyph_count);
			// keep track of how many glyphs we have total
			props.glyph_count++;

			bool font_changed = false;
			// check if the font has changed
			if(dict.has("font")) {
				// there's one in hte dictionary
				poFont *tmp = dict.getPtr<poFont>("font");
				// and it isn't the same as last time
				if(tmp != fnt) {
					fnt = tmp;
					font_changed = true;
				}
			}
			else {
				// there's nothing in the dictionary
				if(fnt != font()) {
					// and the current font isn't the default one
					fnt = font();
					font_changed = true;
				}
			}
						
			// do what we need to do when the font switches
			if(font_changed) {
				fnt->glyph(' ');
				spacer = fnt->glyphAdvance().x;
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
		
		// handle whitespace specially
		// any whitespace indicates a new word
		if(::iswspace(codepoint)) {
			switch(codepoint) {
				case ' ':
					props.last_space = spacer;
					break;
					
				case '\n':
					props.broke = true;
					break;
					
				case '\t':
				{
					float s = spacer * tab_width;
					float w = props.line.bounds.width() + props.word.width;
					props.last_space = (floor(w/s) + 1) * s - w;
					break;
				}
			}
			
			// make the line knows about the space
			props.last_space *= tracking_tmp;
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
		fnt->glyph(codepoint);
		
		// store all the info we need to render
		layout_glyph glyph;
		glyph.glyph = codepoint;
		glyph.bbox = fnt->glyphBounds();
		glyph.bbox.origin += poPoint(props.word.width, 0) + fnt->glyphBearing();
		props.word.glyphs.push_back(glyph);
		
		// see if we need to update our line height
		props.max_drop = std::max(props.max_drop, -fnt->glyphBearing().y + line_padding_fudge);
		props.max_line_height = std::max(props.max_line_height, props.max_drop + fnt->glyphDescender() + line_padding_fudge);

		// update the pen x position
		props.word.width += fnt->glyphAdvance().x * tracking_tmp;

		// check if we've gone over
		if(props.line.bounds.size.x + props.word.width > (_size.x-paddingLeft()-paddingRight()) && props.line.word_count >= 1) {
			breakLine(props);
		}
	}

	// just in case, make sure the current glyphs get onto the last line
	addWordToLine(props);
	breakLine(props);
}

void TextBoxLayout::addWordToLine(line_layout_props &props) {
	if(!props.word.glyphs.empty()) {
		props.line.word_count++;
		for(int i=0; i<props.word.glyphs.size(); i++) {
			layout_glyph &glyph = props.word.glyphs[i];
			glyph.bbox.origin += props.line.bounds.size;
			props.line.glyphs.push_back(glyph);
		}
	}
	
	props.line.bounds.size.x += props.word.width + props.last_space;
	
	props.word = word_layout();
}

void TextBoxLayout::breakLine(line_layout_props &props) {
	if(!props.line.glyphs.empty()) {
		// save the start pos
		float start_y = props.line.bounds.origin.y;
		
		for(int i=0; i<props.line.glyphs.size(); i++) {
			layout_glyph &glyph = props.line.glyphs[i];
			// move the glyph down to the baseline + the start position
			glyph.bbox.origin.y += props.max_drop + start_y;
		}
		
		// words all end with a space, so take off the space for it
		props.line.bounds.size.x -= props.last_space;
		// and bump the line height
		props.line.bounds.size.y = props.max_line_height;
		// save the line
		addLine(props.line);
		
		// and set up hte next line
		props.line = layout_line();
		props.line.bounds.origin.y = start_y + props.max_line_height * props.leading;
	}
	
	props.broke = true;
}

void TextBoxLayout::alignText() {
	poRect text_bounds = textBounds();

	for(int i=0; i<numLines(); i++) {
		layout_line line = getLine(i);
		
		poPoint glyphOffset(0.f, 0.f);
		
		switch(_alignment) {
			case PO_ALIGN_TOP_LEFT:
			case PO_ALIGN_CENTER_LEFT:
			case PO_ALIGN_BOTTOM_LEFT:
				break;
			case PO_ALIGN_TOP_CENTER:
			case PO_ALIGN_CENTER_CENTER:
			case PO_ALIGN_BOTTOM_CENTER:
				glyphOffset.x = (_size.x - line.bounds.size.x)/2;
				break;
			case PO_ALIGN_TOP_RIGHT:
			case PO_ALIGN_CENTER_RIGHT:
			case PO_ALIGN_BOTTOM_RIGHT:
				glyphOffset.x = (_size.x - line.bounds.size.x); 
				break;
		}
		
		glyphOffset.x += paddingLeft();
		glyphOffset.y += paddingBottom();
		
		line.bounds.origin = round(line.bounds.origin + glyphOffset);
		
		BOOST_FOREACH(layout_glyph &glyph, line.glyphs) {
			glyph.bbox.origin = round(glyph.bbox.origin + glyphOffset);
		}
		
		replaceLine(i, line);
	}
	
	recalculateTextBounds();
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
void TextBoxLayout::alignment(poAlignment a) {_alignment = a; alignText();}
int TextBoxLayout::tabWidth() const {return tab_width;}
void TextBoxLayout::tabWidth(int tw) {tab_width = tw;}

