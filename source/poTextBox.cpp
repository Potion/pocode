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
,	cacheToTexture(false)
{}

poTextBox::poTextBox(int w) 
:	poObject()
,	textColor(poColor::white)
,	_layout(poPoint(w,0))
,	fit_height_to_bounds(true)
,	text_align(PO_ALIGN_TOP_LEFT)
,	cacheToTexture(false)
{
	setWidth(w);
}

poTextBox::poTextBox(int w, int h) 
:	poObject()
,	textColor(poColor::white)
,	_layout(poPoint(w,h))
,	fit_height_to_bounds(false)
,	text_align(PO_ALIGN_TOP_LEFT)
,	cacheToTexture(false)
{
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
	tb->cacheToTexture = cacheToTexture;
	tb->text_align = text_align;
	tb->_layout = _layout;
	tb->cached = cached;
	poObject::clone(tb);
}

poTextBox::~poTextBox() {}

std::string poTextBox::getText() const {return _layout.text;}
void poTextBox::setText(const std::string &str) {_layout.text = str;}

uint poTextBox::getTextSize() const {return _layout.textSize;}
void poTextBox::setTextSize(uint ptSize) {_layout.textSize = ptSize;}

poAlignment poTextBox::getTextAlignment() const {return text_align;}
void poTextBox::setTextAlignment(poAlignment align) {text_align = align;}

poRect poTextBox::getTextBounds() const {return _layout.textBounds();}
void poTextBox::reshape(int w, int h) {
	setSize(w,h);
	_layout.size = poPoint(getWidth(), getHeight());
}

bool poTextBox::isRichText() const {return _layout.isRichText;}
void poTextBox::setRichText(bool b) {_layout.isRichText = b;}

void poTextBox::reshape(poPoint p){
    reshape(p.x,p.y);
}

uint poTextBox::getNumLines() const {
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

bool poTextBox::getCacheToTexture() const {
	return cacheToTexture;
}

void poTextBox::setCacheToTexture(bool b) {
    if(cacheToTexture != b) {
        cacheToTexture = b;
        
        if(b) {
            //Create cached
            if(!layoutDone) {
                layout();
            } else {
                generateCachedTexture();
            }
        } else {
            //Erase cached
            cached = poTexture();
        }
    }
}

float poTextBox::getLeading() const {
	return _layout.leading;}

void poTextBox::setLeading(float f) {
	_layout.leading = f; }

float poTextBox::getTracking() const {
	return _layout.tracking;}

void poTextBox::setTracking(float f) {
	_layout.tracking = f; }

float poTextBox::getPaddingLeft() const {
	return _layout.padding[po::TextBoxLayout::PADDING_LEFT];
}

float poTextBox::getPaddingRight() const {
	return _layout.padding[po::TextBoxLayout::PADDING_RIGHT];
}

float poTextBox::getPaddingTop() const {
	return _layout.padding[po::TextBoxLayout::PADDING_TOP];
}

float poTextBox::getPaddingBottom() const {
	return _layout.padding[po::TextBoxLayout::PADDING_BOTTOM];
}

void poTextBox::setPadding(float f) {
	_layout.padding[0] = _layout.padding[1] = _layout.padding[2] = _layout.padding[3] = f; 
}

void poTextBox::setPadding(float h, float v) {
	_layout.padding[0] = _layout.padding[1] = h;
	_layout.padding[2] = _layout.padding[3] = v;
}

void poTextBox::setPadding(float l, float r, float t, float b) {
	_layout.padding[0] = l;
	_layout.padding[1] = r;
	_layout.padding[2] = t;
	_layout.padding[3] = b;
}

void poTextBox::setTabWidth(int tw) {
	_layout.tabWidth = tw;
}

int poTextBox::getTabWidth() const {
	return _layout.tabWidth;
}

void poTextBox::setFont(poFont f, const std::string &name) {
	_layout.font(f,name);
}

poFont poTextBox::setFont(const std::string &name) {
	return _layout.font(name);
}

void poTextBox::layout() {
	_layout.layout();
	
	if(fit_height_to_bounds)
		setHeight(getTextBounds().height);

	setAlignment(getAlignment());
	_layout.alignment = text_align;
	_layout.realignText();
	
	if(cacheToTexture) {
        generateCachedTexture();
	}
    
    layoutDone = true;
}


void poTextBox::generateCachedTexture() {
    if(cached.isValid()) {
        cached = poTexture();
    }
    
    poRect bounds = getBounds();
    bounds.include(getTextBounds());
    
    poFBO *fbo = new poFBO(bounds.width, bounds.height, poFBOConfig());
    fbo->setUp(this);
    
    // http://stackoverflow.com/questions/2171085/opengl-blending-with-previous-contents-of-framebuffer
    po::BlendState blend;
    blend.enabled = true;
    blend.blendFunc(GL_SRC_COLOR, GL_ZERO, GL_ONE, GL_ONE);
    
    poOpenGLState *ogl = poOpenGLState::get();
    ogl->pushBlendState();
    ogl->setBlend(blend);
    
    poBitmapFont bmp = getBitmapFont(setFont(), _layout.textSize);
    
    po::setColor(poColor::white);
    for(int i=0; i<getNumLines(); i++) {
        BOOST_FOREACH(po::TextLayoutGlyph const &glyph, _layout.lines[i].glyphs) {
            bmp.drawGlyph( glyph.glyph, glyph.bbox.getPosition() ); 
        }
    }
    
    ogl->popBlendState();
    
    fbo->setDown(this);
    cached = fbo->colorTexture();
    delete fbo;
}


void poTextBox::draw() {
	if(cached.isValid()) {
		po::BlendState blend = po::BlendState::preMultipliedBlending();
		
		poOpenGLState* ogl = poOpenGLState::get();
		ogl->pushBlendState();
		ogl->setBlend(blend);
		
		po::setColor(textColor, getAppliedAlpha());
		po::drawRect(cached);
		
		ogl->popBlendState();
		return;
	}
	
	poBitmapFont regFont = getBitmapFont(setFont(), _layout.textSize);
	poBitmapFont bitmapFont = regFont;
	
    if ( _layout.isRichText ) {
		int count = 0;
		for(int i=0; i<getNumLines(); i++) {
			BOOST_FOREACH(po::TextLayoutGlyph const &glyph, _layout.getLine(i).glyphs) {
				po::setColor(poColor(textColor, getAppliedAlpha()));
				
				poDictionary dict = _layout.textPropsAtIndex(count);
				count++;
				
				// see if the user wants anything special
				if(dict.has("color"))
					po::setColor(poColor(dict.getColor("color"), getAppliedAlpha()));
				
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
		po::setColor( poColor(textColor, getAppliedAlpha()) );
		
        for(int i=0; i<getNumLines(); i++) {
            BOOST_FOREACH(po::TextLayoutGlyph const &glyph, _layout.lines[i].glyphs) {
                bitmapFont.drawGlyph( glyph.glyph, glyph.bbox.getPosition() ); 
            }
        }
    }
}


void poTextBox::_drawBounds() {
    for(int i=0; i<getNumLines(); i++) {
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
        po::drawStroke(getTextBounds());
    }
    
    po::setColor(poColor::dk_grey, .8f);
    po::drawStroke(getBounds());
    
    po::setColor(poColor::red);
    po::drawRect(poRect(-offset-poPoint(5,5), poPoint(10,10)));
}




