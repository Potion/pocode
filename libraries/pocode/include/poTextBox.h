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
//  TextBox.h
//  pocode
//
//  Created by Joshua Fisher on 5/5/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once


#include "poObject.h"
#include "poTextBoxLayout.h"

#include "poFont.h"
#include "poBitmapFont.h"

namespace po {
    // for caching
    class FBO;

    static const std::string PO_TEXT_REGULAR = "";
    static const std::string PO_TEXT_ITALIC = "i";
    static const std::string PO_TEXT_BOLD = "b";
    static const std::string PO_TEXT_BOLD_ITALIC = "bi";



    // CLASS NOTES
    //
    // TextBox derives directly from Object. It is one of the most commonly used objects in pocode.
    // 
    // A simple TextBox may be constructed as follows:
    // 
    //      TextBox* TB = new TextBox( 200,300 );
    //      TB->setText( "hello world!" );
    //      TB->setFont( getFont("Courier", 20) );
    //      TB->setTextAlignment( po::ALIGN_TOP_CENTER );
    //      TB->textColor = Color::black;
    //      TB->doLayout();
    //
    // You MUST call the doLayout() method in order to see any changes in your TextBox take effect.
    //
    // The text that is passed into the setText() method may be formatted using basic HTML tags as follows:
    //
    //      TB->setText( "This is in <b size='10'>bold</b>.This is in <i>italic</i>." );
    //
    // You will also need to specify the bold and italic fonts explicitly using the font method.
    // The richText setting must be on to see the effects on the text.


    class TextBox : public Object {
    public:
        TextBox();
        TextBox(int w);
        TextBox(int w, int h);
        virtual ~TextBox();
        
        virtual Object* copy();
        
        // LAYOUT
        // You MUST call layout after making any changes to TextBox. Otherwise, you will not see
        // the changes in the text box.
        void				doLayout();
		
		// For convenient formatting of a textbox, this function adds end lines to prevent widows
		void				removeWidows();


        // TEXTBOX PROPERTIES
        std::string         getText() const;
        void				setText(const std::string &str);
        
        uint				getTextSize() const;
        void				setTextSize(uint ptSize);
        
        // The font methods tell the textBox which fonts to use. If a single font is provided as follows:
        //        TB->setFont( getFont("Courier", 20) );
        // This font will be used for all text, and acts as the PO_TEXT_REGULAR font.
        // Bold and italic fonts may set as follows:
        //        TB->setFont( getFont("Courier Bold"), PO_TEXT_BOLD );
        //        TB->setFont( getFont("Courier Italic"), PO_TEXT_ITALIC );
        // These fonts will be used where the <b> and <i> HTML tags appear in the text.
        void                setFont(Font *font, const std::string &name=PO_TEXT_REGULAR);
        Font*				getFont(const std::string &name=PO_TEXT_REGULAR);
        
        // textAlignment may be set using one of the Alignment enums found in "poEnums.h".
        // Commonly used settings are po::ALIGN_TOP_LEFT, po::ALIGN_TOP_CENTER and po::ALIGN_CENTER_CENTER.
        Alignment           getTextAlignment() const;
        void                setTextAlignment(Alignment align);
        
        // This is the textColor for all of the text. To set the text color by word or by letter,
        // use the color property in rich text mode.
        Color             textColor;

        // The padding determine the amount of space between the bounds of the text box
        // and the actual text. It may be set for all 4 sides at once, or per side.
        void                setPadding(float f);
        void                setPadding(float h, float v);
        void                setPadding(float l, float r, float t, float b);
        float               getPaddingLeft() const;
        float               getPaddingRight() const;
        float               getPaddingTop() const;
        float               getPaddingBottom() const;
        
        // The tabWidth determines the width of a tab in spaces. A setting of 5 means the tab
        // will be 5 spaces wide.
        void                setTabWidth(int tw);
        int                 getTabWidth() const;
		
    /////////////////////////////////////////////////////
    //
    //	TextBox rich text is UNDER DEVELOPMENT
    //	Check again in future releases of pocode
    //
    /////////////////////////////////////////////////////
        
        // The richText mode enables specifc text settings within the text. For example, setting
        // the color of a single world or line of text. By default, richText mode is off.
        bool                isRichText() const;
        void                setRichText(bool b);

        // Leading is the spacing between lines of text. Tracking is the spacing between characters.
        // These properties can be overridden inline with a span as follows:
        //      <span leading='.9' tracking='1.1'>leaded and tracked differently</span>
        // There can be only 1 leading setting for a single line of text.
        float               getLeading() const;
        void                setLeading(float f);
        float               getTracking() const;
        void                setTracking(float f);
        
        // TEXTBOX BOUNDS
        // The "reshape" methods set the official bounds of the text box that are used by the doLayout() method.
        // The "textBounds" methods returns the actual bounds of the text after is has been layed out.
        // So, if the text does not fill the whole textbox, the bounds will differ.
        void                reshape(int w, int h);
        void                reshape(Point p);
        void				reshape(Rect r);
        void                useTextBoundsAsBounds( bool B );
        void                useAutoAdjustHeight( bool B );
        Rect                getTextBounds() const;
        virtual Rect        getBounds();
        
        virtual bool        pointInside(Point p, bool localize);
        
        // LINES OF TEXT
        uint                getNumLines() const;
        uint                getNumWordsForLine(uint lineNum);
        Rect                boundsForLine(uint num) const;
        int                 numLettersForLine( int lineIndex );
        Rect                getBoundsForLetterOnLine( int letterIndex, int lineIndex );
        void                setBoundsForLetterOnLine( int letterIndex, int lineIndex, Rect newBounds );
        
        // MOVE LINES OF TEXT
        void				setLineOffset(uint lineNum, Point offset);
        
        // CACHE TEXT
        bool				getCacheToTexture() const;
        void				setCacheToTexture(bool b);
        
        // DEBUGGING
        // The bounds of a textBox can be shown by setting drawBounds to 1. Setting it to 0 will hide the bounds.
        // You can use the po::TEXT_BOX_STROKE_XXXXX settings in the enum defined at the top of this file.
        virtual void		_drawBounds();
        
        // DRAWING
        // The draw() method is called automatically if a text box is added to the scene graph.
        // You should not need to call the draw() method yourself.
        void                draw();

        // MEMORY MANAGEMENT
        virtual int         getSizeInMemory();
        
        // FILL AND STROKE
        Color               fillColor;
        bool                fillEnabled;
        Color               strokeColor;
        int                 strokeWidth;
            
    protected:
        void				clone(TextBox *tb);
        
    private:
        void                generateCachedTexture();
        
        //Render the text, set to false for rendering into an FBO (scaled later)
        void                renderText(bool bWithScaling = true);

        bool                useTextBounds;
        bool                autoAdjustHeight;
        bool				cacheToTexture;
        bool                layoutDone;
        Alignment           textAlignment;
        TextBoxLayout       layout;
        Texture*			cached;
    };
} /* End po namespace */