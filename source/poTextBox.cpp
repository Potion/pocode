
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
#include "poSimpleDrawing.h"
#include "poOpenGLState.h"
#include "poShapeBasics2D.h"
#include "poResourceStore.h"

#include <float.h>

poTextBox::poTextBox()
:	poObject()
,	textColor(poColor::white)
,	layout(poPoint())
,	fitHeightToBounds(true)
,	textAlignment(PO_ALIGN_TOP_LEFT)
,	cacheToTexture(false)
,	cached(NULL)
{}

poTextBox::poTextBox(int w) 
:	poObject()
,	textColor(poColor::white)
,	layout(poPoint(w,0))
,	fitHeightToBounds(true)
,	textAlignment(PO_ALIGN_TOP_LEFT)
,	cacheToTexture(false)
,	cached(NULL)
{
	setWidth(w);
}

poTextBox::poTextBox(int w, int h) 
:	poObject()
,	textColor(poColor::white)
,	layout(poPoint(w,h))
,	fitHeightToBounds(false)
,	textAlignment(PO_ALIGN_TOP_LEFT)
,	cacheToTexture(false)
,	cached(NULL)
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
	tb->fitHeightToBounds = fitHeightToBounds;
	tb->cacheToTexture = cacheToTexture;
	tb->layout = layout;
	tb->cached = cached;
	poObject::clone(tb);
	
}

poTextBox::~poTextBox() {
}

std::string poTextBox::getText() const {
	return layout.text;
}

void poTextBox::setText(const std::string &str) {
	layout.text = str;
}

uint poTextBox::getTextSize() const {
	return layout.textSize;
}

void poTextBox::setTextSize(uint ptSize) {
	layout.textSize = ptSize;
}

poAlignment poTextBox::getTextAlignment() const {
	return textAlignment;
}

void poTextBox::setTextAlignment(poAlignment align) {
	textAlignment = align;
}

poRect poTextBox::getTextBounds() const {
	return layout.getTextBounds();
}

void poTextBox::reshape(int w, int h) {
	setSize(w,h);
	layout.size = poPoint(getWidth(), getHeight());
}

bool poTextBox::isRichText() const {
	return layout.isRichText;
}

void poTextBox::setRichText(bool b) {
	layout.isRichText = b;
}

void poTextBox::reshape(poPoint p) {
    reshape(p.x,p.y);
}

uint poTextBox::getNumLines() const {
	return layout.getNumLines();
}

poRect poTextBox::boundsForLine(uint num) const {
	return layout.getBoundsForLine(num);
}

int     poTextBox::numLettersForLine( int lineIndex ) {
    return layout.getNumGlyphsForLine(lineIndex);
}

poRect  poTextBox::getBoundsForLetterOnLine( int letterIndex, int lineIndex ) {
    return layout.getBoundsForGlyphOnLine( letterIndex, lineIndex );
}

void    poTextBox::setBoundsForLetterOnLine( int letterIndex, int lineIndex, poRect newBounds ) {
	layout.lines[lineIndex].glyphs[letterIndex].bbox = newBounds;
}

bool poTextBox::getCacheToTexture() const {
	return cacheToTexture;
}

void poTextBox::setCacheToTexture(bool b) {
	cacheToTexture = b;
}

float poTextBox::getLeading() const {
	return layout.leading;}

void poTextBox::setLeading(float f) {
	layout.leading = f; }

float poTextBox::getTracking() const {
	return layout.tracking;}

void poTextBox::setTracking(float f) {
	layout.tracking = f; }

float poTextBox::getPaddingLeft() const {
	return layout.padding[po::TextBoxLayout::PADDING_LEFT];
}

float poTextBox::getPaddingRight() const {
	return layout.padding[po::TextBoxLayout::PADDING_RIGHT];
}

float poTextBox::getPaddingTop() const {
	return layout.padding[po::TextBoxLayout::PADDING_TOP];
}

float poTextBox::getPaddingBottom() const {
	return layout.padding[po::TextBoxLayout::PADDING_BOTTOM];
}

void poTextBox::setPadding(float f) {
	layout.padding[0] = layout.padding[1] = layout.padding[2] = layout.padding[3] = f; 
}

void poTextBox::setPadding(float h, float v) {
	layout.padding[0] = layout.padding[1] = h;
	layout.padding[2] = layout.padding[3] = v;
}

void poTextBox::setPadding(float l, float r, float t, float b) {
	layout.padding[0] = l;
	layout.padding[1] = r;
	layout.padding[2] = t;
	layout.padding[3] = b;
}

void poTextBox::setTabWidth(int tw) {
	layout.tabWidth = tw;
}

int poTextBox::getTabWidth() const {
	return layout.tabWidth;
}

void poTextBox::setFont(poFont *f, const std::string &name) {
	layout.setFont(f,name);
}

poFont* poTextBox::getFont(const std::string &name) {
	return layout.getFont(name);
}

void poTextBox::doLayout() {
	layout.layout();
	
	if(fitHeightToBounds)
		setHeight(getTextBounds().height);

	setAlignment(getAlignment());
	setTextAlignment(getTextAlignment());
	layout.realignText();
	
	if(cacheToTexture) {
        generateCachedTexture();
	}
    
    layoutDone = true;
}


void poTextBox::generateCachedTexture() {
	if(cached) {
		delete cached;
		cached = NULL;
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
    
    poBitmapFont *bmp = poGetBitmapFont(getFont(), layout.textSize);
    
    po::setColor(poColor::white);
    for(int i=0; i<getNumLines(); i++) {
        BOOST_FOREACH(po::TextLayoutGlyph const &glyph, layout.lines[i].glyphs) {
            bmp->drawGlyph( glyph.glyph, glyph.bbox.getPosition() ); 
        }
    }
    
    ogl->popBlendState();
    
    fbo->setDown(this);
//	cached = fbo->getColorTexture()->copy();
    cached = fbo->getColorTextureCopy();
    delete fbo;
}


void poTextBox::draw() {
	if(cached && cached->isValid()) {
		po::BlendState blend = po::BlendState::preMultipliedBlending();
		
		poOpenGLState* ogl = poOpenGLState::get();
		ogl->pushBlendState();
		ogl->setBlend(blend);
		
		po::setColor(textColor, getAppliedAlpha());
		po::drawTexturedRect(cached);
		
		ogl->popBlendState();
		return;
	}
	
	poBitmapFont *bitmapFont = poGetBitmapFont(getFont(), layout.textSize);
	
    if ( layout.isRichText ) {
//		int count = 0;
//		for(int i=0; i<getNumLines(); i++) {
//			BOOST_FOREACH(po::TextLayoutGlyph const &glyph, layout.getLine(i).glyphs) {
//				po::setColor(poColor(textColor, getAppliedAlpha()));
//				
//				poDictionary dict = layout.getTextPropsAtIndex(count);
//				count++;
//				
//				// see if the user wants anything special
//				if(dict.has("color"))
//					po::setColor(poColor(dict.getColor("color"), getAppliedAlpha()));
//				
//				// a new font, perhaps?
//				if(dict.has("font")) {
//					poFont theFont = (poFont*)dict.getPtr("font");
//					int fontSize = dict.has("fontSize") ? dict.getInt("fontSize") : layout.textSize;
//					poBitmapFont* newFont = getBitmapFont(theFont, fontSize);
//					
//					if(newFont != bitmapFont) {
//						bitmapFont = newFont;
//					}
//				}
//				else if(bitmapFont != regFont) {
//					bitmapFont = regFont;
//				}
//				
//				// very well, now draw it
//				bitmapFont->drawGlyph( glyph.glyph, glyph.bbox.getPosition() ); 
//			}
//		}
    }
    else {
		po::setColor( poColor(textColor, getAppliedAlpha()) );
		
        for(int i=0; i<getNumLines(); i++) {
            BOOST_FOREACH(po::TextLayoutGlyph const &glyph, layout.lines[i].glyphs) {
                bitmapFont->drawGlyph( glyph.glyph, glyph.bbox.getPosition() ); 
            }
        }
    }
}


void poTextBox::_drawBounds() {
    for(int i=0; i<getNumLines(); i++) {
        if(drawBounds & PO_TEXT_BOX_STROKE_GLYPHS) {
            po::setColor(poColor::ltGrey, .5f);
            BOOST_FOREACH(po::TextLayoutGlyph const &glyph, layout.getLine(i).glyphs) {
                po::drawStrokedRect(glyph.bbox);
            }
        }
        
        if(drawBounds & PO_TEXT_BOX_STROKE_LINES) {
            po::setColor(poColor::white, .6f);
            po::drawStrokedRect(boundsForLine(i));
        }
    }
    
    if(drawBounds & PO_TEXT_BOX_STROKE_TEXT_BOUNDS) {
        po::setColor(poColor::grey, .7f);
        po::drawStrokedRect(getTextBounds());
    }
    
    po::setColor(poColor::dkGrey, .8f);
    po::drawStrokedRect(getBounds());
    
    po::setColor(poColor::red);
    po::drawFilledRect(poRect(-offset-poPoint(5,5), poPoint(10,10)));
}




