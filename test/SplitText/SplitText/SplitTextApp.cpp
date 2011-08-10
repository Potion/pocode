#include "SplitTextApp.h"

#include "poCamera.h"
#include "poApplication.h"
#include "poResourceStore.h"
#include "poBitmapFontAtlas.h"

#include "poTextBox.h"
#include "SimpleDrawing.h"

poObject *createObjectForID(uint uid) {
	return new SplitTextApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "SplitText", 100, 100, 800, 800);
}

void cleanupApplication() {
}

SplitTextApp::SplitTextApp() 
:	layout(poPoint(400,0))
{
	addModifier(new poCamera2D());
	
	layout.font(getFont("Helvetica",20));
	layout.text("Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?");
	layout.alignment(PO_ALIGN_CENTER);
	layout.layout();

	addEvent(PO_KEY_DOWN_EVENT, this);
	restartDynamicEntrance();
}

void SplitTextApp::draw() {
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
			bbox.origin.x += 200 + offsets[i];
			bbox.origin.y += val;
			
			atlas->drawUID(glyph.glyph, bbox.origin);
		}
	}
	atlas->stopDrawing();
}

void SplitTextApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT) {
		if(event->keyChar == ' ') {
			restartDynamicEntrance();
		}
	}
}

void SplitTextApp::restartDynamicEntrance() {
	offsets.clear();
	tweens.clear();
	
	for(int i=0; i<layout.numLines(); i++) {
		poTween<float> tween(800.f);
		tween
			.set(200.f)
			.setDelay(i*.1f)
			.setDuration(1.f)
			.setTweenFunction(quadInOutFunc)
			.start();
		
		tweens.push_back(tween);
		
		offsets.push_back(poRand(-20,20));
	}
}



