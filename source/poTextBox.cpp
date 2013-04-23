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
//  TextBox.cpp
//  pocode
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//


#include "poFBO.h"
#include "poTextBox.h"
#include "poSimpleDrawing.h"
#include "poOpenGLState.h"
#include "poShapeBasics2D.h"
#include "poResourceStore.h"
#include "poApplication.h"

#include <float.h>
#include <boost/foreach.hpp>

namespace po {
    TextBox::TextBox()
    :	Object()
    ,	textColor(Color::white)
    ,	layout(Point())
    ,	useTextBounds(false)
    ,   autoAdjustHeight(false)
    ,	textAlignment(po::ALIGN_TOP_LEFT)
    ,	cacheToTexture(false)
    ,	cached(NULL)
    ,   fillEnabled(false)
    ,   strokeWidth(0)
    {
        setFont(Font::defaultFont());
    }

    TextBox::TextBox(int w) 
    :	Object()
    ,	textColor(Color::white)
    ,	layout(Point(w,0))
    ,	useTextBounds(false)
    ,   autoAdjustHeight(true)
    ,	textAlignment(po::ALIGN_TOP_LEFT)
    ,	cacheToTexture(false)
    ,	cached(NULL)
    ,   fillEnabled(false)
    ,   strokeWidth(0)
    {
        layout.size = Point( w,0 );
        setFont(Font::defaultFont());
    }

    TextBox::TextBox(int w, int h) 
    :	Object()
    ,	textColor(Color::white)
    ,	layout(Point(w,h))
    ,	useTextBounds(false)
    ,   autoAdjustHeight(false)
    ,	textAlignment(po::ALIGN_TOP_LEFT)
    ,	cacheToTexture(false)
    ,	cached(NULL)
    ,   fillEnabled(false)
    ,   strokeWidth(0)
    {
        layout.size = Point( w,h );
        setFont(Font::defaultFont());
    }
    
    TextBox::~TextBox() {
        if(cached) delete cached; cached = NULL;
    }
    
    
    //------------------------------------------------------------------------
    Object* TextBox::copy() {
        TextBox *tb = new TextBox();
        clone(tb);
        return tb;
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::clone(TextBox *tb) {
        tb->textColor = textColor;
        tb->useTextBounds = useTextBounds;
        tb->cacheToTexture = cacheToTexture;
        tb->layout = layout;
        tb->cached = cached;
        Object::clone(tb);
    }
    
    
    
    
    // ------------------------------------------------------------------------------------
    // =============================== Text/Fonts ==============================================
    #pragma mark Text/Fonts
    
    //------------------------------------------------------------------------
    void TextBox::setText(const std::string &str) {
        layout.text = str;
    }
    
    
    //------------------------------------------------------------------------
    std::string TextBox::getText() const {
        return layout.text;
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setTextSize(uint ptSize) {
        layout.textSize = ptSize;
    }
    
    
    //------------------------------------------------------------------------
    uint TextBox::getTextSize() const {
        return layout.textSize;
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setRichText(bool b) {
        layout.isRichText = b;
    }
    
    
    //------------------------------------------------------------------------
    bool TextBox::isRichText() const {
        return layout.isRichText;
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setFont(Font *f, const std::string &name) {
        layout.setFont(f,name);
    }
    
    
    //------------------------------------------------------------------------
    Font* TextBox::getFont(const std::string &name) {
        return layout.getFont(name);
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setTextAlignment(Alignment align) {
        textAlignment = align;
    }
    
    
    //------------------------------------------------------------------------
    Alignment TextBox::getTextAlignment() const {
        return textAlignment;
    }
    
    
    //------------------------------------------------------------------------
    Rect TextBox::getTextBounds() const {
        return layout.getTextBounds();
    }
    
    
    
    
    // ------------------------------------------------------------------------------------
    // =============================== Dimensions/Hit Testing =============================
    #pragma mark Dimensions/Hit Testing
    
    //------------------------------------------------------------------------
    Rect TextBox::getBounds() {
        if ( useTextBounds ) {
            Rect R = getTextBounds();
            R.x -= getPaddingLeft();
            R.width += getPaddingLeft() + getPaddingRight();
            R.y -= getPaddingTop();
            R.height += getPaddingTop() + getPaddingBottom();
            return R;
        }
        else
        {
            if (autoAdjustHeight)
                return Rect( 0, 0, layout.size.x,  getTextBounds().height+getPaddingTop()+getPaddingBottom() );
            else
                return Rect( Point(0,0), layout.size );
        }
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::reshape(int w, int h) {
        layout.size = Point(w,h);
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::reshape(Rect r) {
        layout.size = r.getSize();
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::reshape(Point p) {
        reshape(p.x,p.y);
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::useTextBoundsAsBounds( bool B ) {
        useTextBounds = B; layout.setUseTextBounds(B);
    };
    
    
    //------------------------------------------------------------------------
    void TextBox::useAutoAdjustHeight( bool B ) {
        autoAdjustHeight = B;
    };
    
    //------------------------------------------------------------------------
    bool TextBox::pointInside(Point p, bool localize) {
        if(!visible)
            return false;
        
        if(localize) {
            //p.y = po::getWindowHeight() - p.y;
            p = globalToLocal(p);
        }
        
        Rect bounds = getBounds();
        
        // DO POINT INSIDE TEST FOR 2D
        if ( Camera::getCurrentCameraType() == po::CAMERA_2D ) {
            return bounds.contains(p.x, p.y);
        }
        else if(Camera::getCurrentCameraType() == po::CAMERA_3D) {
            return pointInRect3D( p, getMatrixSet(), bounds );
        }
        
        return false;
    }
    
    
    
    
    // ------------------------------------------------------------------------------------
    // =============================== Lines ==============================================
    #pragma mark Lines
    
    //------------------------------------------------------------------------
    uint TextBox::getNumLines() const {
        return layout.getNumLines();
    }
    
    
    //------------------------------------------------------------------------
    uint TextBox::getNumWordsForLine(uint lineNum) {
        return layout.getLine(lineNum).wordCount;
    }
    
    
    //------------------------------------------------------------------------
    Rect TextBox::boundsForLine(uint num) const {
        return layout.getBoundsForLine(num);
    }
    
    
    //------------------------------------------------------------------------
    int TextBox::numLettersForLine( int lineIndex ) {
        return layout.getNumGlyphsForLine(lineIndex);
    }
    
    
    //------------------------------------------------------------------------
    Rect TextBox::getBoundsForLetterOnLine( int letterIndex, int lineIndex ) {
        return layout.getBoundsForGlyphOnLine( letterIndex, lineIndex );
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setBoundsForLetterOnLine( int letterIndex, int lineIndex, Rect newBounds ) {
        layout.lines[lineIndex].glyphs[letterIndex].bbox = newBounds;
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setLineOffset(uint lineNum, Point offset) {    
        layout.shiftLine(lineNum, offset);
    }
    
    
    
    
    // ------------------------------------------------------------------------------------
    // =============================== Leading/Tracking/Tabs ===================================
    #pragma mark Leading/Tracking/Tabs
    
    //------------------------------------------------------------------------
    float TextBox::getLeading() const {
        return layout.leading;
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setLeading(float f) {
        layout.leading = f;
    }
    
    
    //------------------------------------------------------------------------
    float TextBox::getTracking() const {
        return layout.tracking;
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setTracking(float f) {
        layout.tracking = f;
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setTabWidth(int tw) {
        layout.tabWidth = tw;
    }
    
    
    //------------------------------------------------------------------------
    int TextBox::getTabWidth() const {
        return layout.tabWidth;
    }
    
    
    
    
    // ------------------------------------------------------------------------------------
    // =============================== Padding ============================================
    #pragma mark Padding
    
    //------------------------------------------------------------------------
    float TextBox::getPaddingLeft() const {
        return layout.padding[po::TextBoxLayout::PADDING_LEFT];
    }
    
    
    //------------------------------------------------------------------------
    float TextBox::getPaddingRight() const {
        return layout.padding[po::TextBoxLayout::PADDING_RIGHT];
    }
    
    
    //------------------------------------------------------------------------
    float TextBox::getPaddingTop() const {
        return layout.padding[po::TextBoxLayout::PADDING_TOP];
    }
    
    
    //------------------------------------------------------------------------
    float TextBox::getPaddingBottom() const {
        return layout.padding[po::TextBoxLayout::PADDING_BOTTOM];
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setPadding(float f) {
        layout.padding[0] = layout.padding[1] = layout.padding[2] = layout.padding[3] = f;
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setPadding(float h, float v) {
        layout.padding[0] = layout.padding[1] = h;
        layout.padding[2] = layout.padding[3] = v;
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setPadding(float l, float r, float t, float b) {
        layout.padding[0] = l;
        layout.padding[1] = r;
        layout.padding[2] = t;
        layout.padding[3] = b;
    }
    
    
    
    
    // ------------------------------------------------------------------------------------
    // =============================== Layout =============================================
    #pragma mark Layout
    
    //------------------------------------------------------------------------
    void TextBox::doLayout() {
        layout.layout();
        
        layout.alignment = getTextAlignment();
        layout.realignText();
        setAlignment(getAlignment());
        
        if(cacheToTexture) {
            generateCachedTexture();
        }
        
        layoutDone = true;
    }
    
    
    
    
    // ------------------------------------------------------------------------------------
    // =============================== Cache to Texture ===================================
    #pragma mark Cache to Texture
    
    //------------------------------------------------------------------------
    bool TextBox::getCacheToTexture() const {
        return cacheToTexture;
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::setCacheToTexture(bool b) {
        cacheToTexture = b;
        if(!cacheToTexture && cached) delete cached; cached = NULL;
    }
    
    //------------------------------------------------------------------------
    void TextBox::generateCachedTexture() {
        if(cached) {
            delete cached;
            cached = NULL;
        }
        
        Rect bounds = getBounds();
        bounds.include(getTextBounds());
        
        FBO *fbo = new FBO(bounds.width * po::getScale(), bounds.height * po::getScale(), FBOConfig());
        fbo->setUp(this);
        
        // http://stackoverflow.com/questions/2171085/opengl-blending-with-previous-contents-of-framebuffer
        po::saveBlendState();
        po::enableBlendWithFunc(GL_SRC_COLOR, GL_ZERO, GL_ONE, GL_ONE);
        
        BitmapFont *bmp = getBitmapFont(getFont(), layout.textSize);
        
        po::setColor(Color::white);
        for(uint i=0; i<getNumLines(); i++) {
            BOOST_FOREACH(po::TextLayoutGlyph const &glyph, layout.lines[i].glyphs) {
                bmp->drawGlyph( glyph.glyph, glyph.bbox.getPosition(), false);
            }
        }
        
        po::restoreBlendState();
        
        fbo->setDown(this);
        cached = fbo->getColorTextureCopy();
        delete fbo;
    }
    
    
    
    
    // ------------------------------------------------------------------------------------
    // =============================== Drawing ========================================
    #pragma mark Drawing
    
    
    //------------------------------------------------------------------------
    void TextBox::draw() {
        using namespace std;
        if(cached && cached->isValid()) {
            po::saveBlendState();
            po::enableAlphaBlending();
            po::setColor(textColor, getAppliedAlpha());
            po::drawTexturedRect(cached, Rect(0,0,cached->getWidth()/po::getScale(), cached->getHeight()/po::getScale()));
            po::restoreBlendState();
            return;
        }
        
        // draw fill and stroke
        if (fillEnabled) {
            po::setColor( fillColor, getAppliedAlpha() );
            //po::drawFilledRect( 0, 0, layout.size.x, layout.size.y );
            po::drawFilledRect( getBounds() );
        }
        
        if (strokeWidth > 0) {
            glLineWidth( strokeWidth );
            po::setColor( strokeColor, getAppliedAlpha() );
            po::drawStrokedRect( 0, 0, layout.size.x, layout.size.y );
        }
        
        BitmapFont *bitmapFont = getBitmapFont(getFont(), layout.textSize);
        
        if (layout.isRichText) {
            int count = 0;
            for(int i=0; i<getNumLines(); i++) {
                BOOST_FOREACH(po::TextLayoutGlyph const &glyph, layout.getLine(i).glyphs) {
                    po::setColor(Color(textColor, getAppliedAlpha()));
                    
                    Dictionary dict = layout.getTextPropsAtIndex(count);
                    count++;
                    
                    // see if the user wants anything special
                    if(dict.has("color"))
                        po::setColor(Color(dict.getColor("color"), getAppliedAlpha()));
                    
                    // a new font, perhaps?
                    if(dict.has("font")) {
                        Font *theFont = (Font*)dict.getPtr("font");
                        int fontSize = dict.has("fontSize") ? dict.getInt("fontSize") : layout.textSize;
                        BitmapFont* newFont = getBitmapFont(theFont, fontSize);
                        
                        if(newFont != bitmapFont) {
                            bitmapFont = newFont;
                        }
                    }
                    else if(bitmapFont->getFont() != layout.getFont()) {
                        bitmapFont = getBitmapFont(layout.getFont(), layout.textSize);
                    }
                    
                    // very well, now draw it
                    bitmapFont->drawGlyph( glyph.glyph, glyph.bbox.getPosition() ); 
                }
            }
        }
        else {
            po::setColor( Color(textColor, getAppliedAlpha()) );
            
            for(uint i=0; i<getNumLines(); i++) {
                BOOST_FOREACH(po::TextLayoutGlyph const &glyph, layout.lines[i].glyphs) {
                    bitmapFont->drawGlyph( glyph.glyph, glyph.bbox.getPosition() ); 
                }
            }
        }
    }
    
    
    //------------------------------------------------------------------------
    void TextBox::_drawBounds() {
        for(uint i=0; i<getNumLines(); i++) {
            if(drawBounds & po::TEXT_BOX_STROKE_GLYPHS) {
                po::setColor(Color::ltGrey, .5f);
                BOOST_FOREACH(po::TextLayoutGlyph const &glyph, layout.getLine(i).glyphs) {
                    po::drawStrokedRect(glyph.bbox);
                }
            }
            
            if(drawBounds & po::TEXT_BOX_STROKE_LINES) {
                po::setColor(Color::white, .6f);
                po::drawStrokedRect(boundsForLine(i));
            }
        }
        
        if(drawBounds & po::TEXT_BOX_STROKE_TEXT_BOUNDS) {
            po::setColor(Color::grey, .7f);
            po::drawStrokedRect(getTextBounds());
        }
        
        Object::_drawBounds();
        
        /*po::setColor(Color::dkGrey, .8f);
         po::drawStrokedRect(getBounds());
         
         po::setColor(Color::red);
         po::drawFilledRect(Rect(-offset-Point(5,5), Point(10,10)));*/
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Memory Usage =====================================
    #pragma mark Memory Usage
    
    //------------------------------------------------------------------------
    int TextBox::getSizeInMemory() {
        int S = sizeof(TextBox);
        
        S += layout.getSizeInMemory();
        
        //if ( cached )
        //    S += cached->getSizeInMemory();
        
        return S;
    }
}/* End po namespace */
