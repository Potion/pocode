//
//  poTextBox.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

using namespace std;

#include "poFBO.h"
#include "poTextBox.h"
#include "SimpleDrawing.h"
#include "poOpenGLState.h"
#include "poShapeBasics2D.h"
#include "poResourceLoader.h"

#include <float.h>

poTextBox::poTextBox()
:	poObject()
,	textColor(poColor::white)
,	_layout(poPoint())
,	fit_height_to_bounds(true)
,	text_align(PO_ALIGN_TOP_LEFT)
,	cache_to_texture(true)
{
	defaultFonts();
}

poTextBox::poTextBox(int w) 
:	poObject()
,	textColor(poColor::white)
,	_layout(poPoint(w,0))
,	fit_height_to_bounds(true)
,	text_align(PO_ALIGN_TOP_LEFT)
,	cache_to_texture(true)
{
	defaultFonts();
	setWidth(w);
}

poTextBox::poTextBox(int w, int h) 
:	poObject()
,	textColor(poColor::white)
,	_layout(poPoint(w,h))
,	fit_height_to_bounds(false)
,	text_align(PO_ALIGN_TOP_LEFT)
,	cache_to_texture(true)
{
	defaultFonts();
	setWidth(w);
    setHeight(h);
}

poObject* poTextBox::copy() {
	poTextBox *tb = new poTextBox();
	clone(tb);
	return tb;
}

void poTextBox::clone(poTextBox *tb) {
	tb->textColor = textColor;
	tb->fit_height_to_bounds = fit_height_to_bounds;
	tb->cache_to_texture = cache_to_texture;
	tb->text_align = text_align;
	tb->_layout = _layout;
	tb->cached = cached;
	poObject::clone(tb);
}

void poTextBox::defaultFonts() {
#ifndef POTION_IOS
	font(getFont("Lucida Grande", "Regular"));
#endif
}

poTextBox::~poTextBox() {}

std::string poTextBox::text() const {return _layout.text;}
void poTextBox::text(const std::string &str) {_layout.text = str;}

uint poTextBox::textSize() const {return _layout.textSize;}
void poTextBox::textSize(uint ptSize) {_layout.textSize = ptSize;}

poAlignment poTextBox::textAlignment() const {return text_align;}
void poTextBox::textAlignment(poAlignment al) {text_align = al;}

poRect poTextBox::textBounds() const {return _layout.textBounds();}
void poTextBox::reshape(int w, int h) {
	setSize(w,h);
	_layout.size = poPoint(getWidth(), getHeight());
}

bool poTextBox::richText() const {return _layout.isRichText;}
void poTextBox::richText(bool b) {_layout.isRichText = b;}

void poTextBox::reshape(poPoint p){
    reshape(p.x,p.y);
}

uint poTextBox::numLines() const {
	return _layout.numLines();
}

poRect poTextBox::boundsForLine(uint num) const {
	return _layout.boundsForLine(num);
}

int     poTextBox::numLettersForLine( int lineIndex )
{ 
    return _layout.numGlyphsForLine(lineIndex);
}

poRect  poTextBox::getBoundsForLetterOnLine( int letterIndex, int lineIndex )
{
    return _layout.boundsForGlyphOnLine( letterIndex, lineIndex );
}

void    poTextBox::setBoundsForLetterOnLine( int letterIndex, int lineIndex, poRect newBounds )
{
	_layout.lines[lineIndex].glyphs[letterIndex].bbox = newBounds;
}

bool poTextBox::cacheToTexture() const {
	return cache_to_texture;
}

void poTextBox::cacheToTexture(bool b) {
	cache_to_texture = b;
}

float poTextBox::leading() const {
	return _layout.leading;}

void poTextBox::leading(float f) {
	_layout.leading = f; }

float poTextBox::tracking() const {
	return _layout.tracking;}

void poTextBox::tracking(float f) {
	_layout.tracking = f; }

float poTextBox::paddingLeft() const {
	return _layout.padding[po::TextBoxLayout::PADDING_LEFT];
}

float poTextBox::paddingRight() const {
	return _layout.padding[po::TextBoxLayout::PADDING_RIGHT];
}

float poTextBox::paddingTop() const {
	return _layout.padding[po::TextBoxLayout::PADDING_TOP];
}

float poTextBox::paddingBottom() const {
	return _layout.padding[po::TextBoxLayout::PADDING_BOTTOM];
}

void poTextBox::padding(float f) {
	_layout.padding[0] = _layout.padding[1] = _layout.padding[2] = _layout.padding[3] = f; 
}

void poTextBox::padding(float h, float v) {
	_layout.padding[0] = _layout.padding[1] = h;
	_layout.padding[2] = _layout.padding[3] = v;
}

void poTextBox::padding(float l, float r, float t, float b) {
	_layout.padding[0] = l;
	_layout.padding[1] = r;
	_layout.padding[2] = t;
	_layout.padding[3] = b;
}

void poTextBox::tabWidth(int tw) {
	_layout.tabWidth = tw;
}

int poTextBox::tabWidth() const {
	return _layout.tabWidth;
}

void poTextBox::font(poFont f, const std::string &name) {
	_layout.font(f,name);
}

poFont poTextBox::font(const std::string &name) {
	return _layout.font(name);
}

void poTextBox::layout() {
	if(cached.isValid()) {
		cached = poTexture();
	}
	
	_layout.layout();
	
	if(fit_height_to_bounds)
		setHeight(textBounds().height);

	alignment(alignment());
	_layout.alignment = text_align;
	_layout.realignText();
	
	if(cache_to_texture) {
		poRect bounds = getBounds();
		bounds.include(textBounds());
		
		poFBO *fbo = new poFBO(bounds.width, bounds.height, poFBOConfig());
		fbo->setUp(this);
		
		// http://stackoverflow.com/questions/2171085/opengl-blending-with-previous-contents-of-framebuffer
		po::BlendState blend;
		blend.blendFunc(GL_SRC_COLOR, GL_ZERO, GL_ONE, GL_ONE);
		
		poOpenGLState *ogl = poOpenGLState::get();
		ogl->pushBlendState();
		ogl->setBlend(blend);

		poBitmapFont bmp = getBitmapFont(font(), _layout.textSize);

		po::setColor(poColor::white);
		for(int i=0; i<numLines(); i++) {
            BOOST_FOREACH(po::TextLayoutGlyph const &glyph, _layout.lines[i].glyphs) {
                bmp.drawGlyph( glyph.glyph, glyph.bbox.getPosition() ); 
            }
        }
		
		ogl->popBlendState();
		
		fbo->setDown(this);
		cached = fbo->colorTexture();
		delete fbo;
	}
}

void poTextBox::draw() {
	if(cached.isValid()) {
		po::setColor(textColor, appliedAlpha());
		po::drawRect(cached);
		return;
	}
	
	poBitmapFont regFont = getBitmapFont(font(), _layout.textSize);
	poBitmapFont bitmapFont = regFont;
	
    if ( _layout.isRichText ) {
		int count = 0;
		for(int i=0; i<numLines(); i++) {
			BOOST_FOREACH(po::TextLayoutGlyph const &glyph, _layout.getLine(i).glyphs) {
				po::setColor(poColor(textColor, appliedAlpha()));
				
				poDictionary dict = _layout.textPropsAtIndex(count);
				count++;
				
				// see if the user wants anything special
				if(dict.has("color"))
					po::setColor(poColor(dict.getColor("color"), appliedAlpha()));
				
				// a new font, perhaps?
				if(dict.has("font")) {
					poFont theFont = dict.getFont("font");
					int fontSize = dict.has("fontSize") ? dict.getInt("fontSize") : _layout.textSize;
					poBitmapFont newFont = getBitmapFont(theFont, fontSize);
					
					if(newFont != bitmapFont) {
						bitmapFont = newFont;
					}
				}
				else if(bitmapFont != regFont) {
					bitmapFont = regFont;
				}
				
				// very well, now draw it
				bitmapFont.drawGlyph( glyph.glyph, glyph.bbox.getPosition() ); 
			}
		}
    }
    else {
		po::setColor( poColor(textColor, appliedAlpha()) );
		
        for(int i=0; i<numLines(); i++) {
            BOOST_FOREACH(po::TextLayoutGlyph const &glyph, _layout.lines[i].glyphs) {
                bitmapFont.drawGlyph( glyph.glyph, glyph.bbox.getPosition() ); 
            }
        }
    }
}


void poTextBox::_drawBounds() {
    for(int i=0; i<numLines(); i++) {
        if(drawBounds & PO_TEXT_BOX_STROKE_GLYPHS) {
            po::setColor(poColor::lt_grey, .5f);
            BOOST_FOREACH(po::TextLayoutGlyph const &glyph, _layout.getLine(i).glyphs) {
                po::drawStroke(glyph.bbox);
            }
        }
        
        if(drawBounds & PO_TEXT_BOX_STROKE_LINES) {
            po::setColor(poColor::white, .6f);
            po::drawStroke(boundsForLine(i));
        }
    }
    
    if(drawBounds & PO_TEXT_BOX_STROKE_TEXT_BOUNDS) {
        po::setColor(poColor::grey, .7f);
        po::drawStroke(textBounds());
    }
    
    po::setColor(poColor::dk_grey, .8f);
    po::drawStroke(getBounds());
    
    po::setColor(poColor::red);
    po::drawRect(poRect(-offset-poPoint(5,5), poPoint(10,10)));
}




