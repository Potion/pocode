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
//  TextBoxLayout.h
//  pocode
//
//  Created by Joshua Fisher on 10/12/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once


#include "poTextLayout.h"

namespace po {
	
	class TextBoxLayout : public TextLayout {
	public:
		int tabWidth;
		poPoint size;
		float tracking;
		float leading;
		poAlignment alignment;
		
		enum {
			PADDING_LEFT,
			PADDING_RIGHT,
			PADDING_TOP,
			PADDING_BOTTOM
		};
		float padding[4];
		
		TextBoxLayout(poPoint s);
        
		void                realignText();
        
        virtual int         getSizeInMemory();
        
	protected:
		virtual void doLayout();
		
	private:
		struct wordLayout {
			wordLayout() 
			:	width(0.f){}
			float width;
			std::vector<TextLayoutGlyph> glyphs;
		};
		
		struct lineLayoutProps {
			lineLayoutProps() 
			:	maxLineHeight(0)
			,	maxDrop(0)
			,	broke(true)
			,	glyphCount(0)
			,	lastSpace(0)
			,	leading(1){}
			float maxLineHeight;
			float maxDrop;
			bool broke;
			int glyphCount;
			float lastSpace;
			float leading;
			wordLayout word;
			TextLayoutLine line;
		};
		
		void addWordToLine(lineLayoutProps &props);
		void breakLine(lineLayoutProps &props);
	};

}