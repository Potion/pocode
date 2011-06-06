#include "TestApp.h"

#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"

using namespace std;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 100, 100, 800, 800);
}

void cleanupApplication() {
}

poTexture *tex = NULL;
bool inverted = false;

TestObj::TestObj() {
    addModifier(new poCamera2D(poColor::black,false));
	
	tex = new poTexture("images/alfred_e_neuman.jpg");
	poRectShape *rect = new poRectShape(tex);
	addChild(rect);
	
	poImage *img = new poImage("images/alfred_e_neuman copy.jpg");
	tex->replace(img->pixels());
	delete img;
}

void TestObj::eventHandler(poEvent *event) {
}
