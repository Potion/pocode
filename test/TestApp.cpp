#include "TestApp.h"

#include "Helpers.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"

using namespace std;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 100, 100, 1050, 600);
}

void cleanupApplication() {
}

uint key = 0;
poBitmapFontAtlas *font;

TestObj::TestObj() {
    addModifier(new poCamera2D());
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void TestObj::draw() {
	if(!font) {
		poResourceStore tmp;
		font = new poBitmapFontAtlas(tmp.add(new poFont("Helvetica",35)));
	}
	
	font->startDrawing(0);
	font->drawUID(key, poPoint(0,0));
	font->stopDrawing();
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT) {
		key = event->keyChar;
		font->cacheGlyph(key);
	}
}
