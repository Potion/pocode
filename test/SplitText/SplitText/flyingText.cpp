#include "flyingText.h"
#include "poResourceStore.h"
#include "poBitmapFontAtlas.h"
#include "poTextBox.h"
#include "SimpleDrawing.h"


//extend to accept alignment and easing_function
flyingText::flyingText( std::string text, int width, float start, float end, float del, float dur, float shift)
:	
layout(poPoint(width,0))
{
	start_y = start;
	end_y = end;
	delay = del;
	duration = dur;
	h_shift = shift;
	
	layout.font(getFont("Helvetica",20));
	layout.text(text);
	layout.alignment(PO_ALIGN_CENTER);
	layout.layout();

	restartDynamicEntrance();
}

flyingText::~flyingText() 
{
}

// TODO: add opacity tween

void flyingText::draw() 
{
	BOOST_FOREACH(poTween<float> &tween, tweens) {
		tween.update();
	}
	
	poBitmapFontAtlas *atlas = getBitmapFont(layout.font());
	
	atlas->startDrawing(0);
	for(int i=0; i<layout.numLines(); i++) {
		float val = tweens[i].getValue();
		
		layout_line line = layout.getLine(i);
		
		for(int j=0; j<line.glyphs.size(); j++) {
			layout_glyph glyph = line.glyphs[j];
			
			poRect bbox = glyph.bbox;
			bbox.origin.x += offsets[i];
			bbox.origin.y += val;
			
			atlas->drawUID(glyph.glyph, bbox.origin);
		}
	}
	atlas->stopDrawing();
}

void flyingText::eventHandler(poEvent *event)
{
}

void flyingText::restartDynamicEntrance()
{
	offsets.clear();
	tweens.clear();
	
	for(int i=0; i<layout.numLines(); i++) {
		poTween<float> tween(start_y);
		tween
		.set(end_y)
		.setDelay(i*delay)
		.setDuration(duration)
		.setTweenFunction(expoOutFunc)
		.start();
		
		tweens.push_back(tween);
		
		offsets.push_back(poRand(-h_shift,h_shift));
	}
}