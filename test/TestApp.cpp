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
poFont *font = NULL;
poTextureAtlas *atlas = NULL;

TestObj::TestObj() {
    addModifier(new poCamera2D());
	font = new poFont("Lucida Grande", PO_FONT_REGULAR, 30);
	cout << "loaded " << *font << endl;
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void TestObj::draw() {
	if(atlas && atlas->hasUID(key)) {
		poTexture *tex = atlas->textureForPage(atlas->pageForUID(key));
		drawRect(atlas->sizeForUID(key),
				 atlas->coordsForUID(key),
				 tex);
		
		drawRect(poRect(poPoint(200,0), atlas->dimensions()), tex);
	}
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT) {
		if(!atlas)
			atlas = new poTextureAtlas(GL_ALPHA,1024,1024);
		
		key = event->keyChar;
		if(!atlas->hasUID(key)) {
			font->glyph(key);
			atlas->addImage(font->glyphImage(),key);
			atlas->layoutAtlas();
		}
	}
}
