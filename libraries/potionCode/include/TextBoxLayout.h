//
//  TextBoxLayout.h
//  potionCode
//
//  Created by Joshua Fisher on 10/12/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "TextLayout.h"

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
		void realignText();
		
	protected:
		virtual void doLayout();
		
	private:
		struct word_layout {
			word_layout() 
			:	width(0.f){}
			float width;
			std::vector<TextLayoutGlyph> glyphs;
		};
		
		struct line_layout_props {
			line_layout_props() 
			:	max_line_height(0)
			,	max_drop(0)
			,	broke(true)
			,	glyph_count(0)
			,	last_space(0)
			,	leading(1){}
			float max_line_height;
			float max_drop;
			bool broke;
			int glyph_count;
			float last_space;
			float leading;
			word_layout word;
			TextLayoutLine line;
		};
		
		void addWordToLine(line_layout_props &props);
		void breakLine(line_layout_props &props);
	};

}