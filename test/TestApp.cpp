#include "TestApp.h"
#include "poSimpleDrawing.h"
#include "poShape2D.h"
#include "poImage.h"
#include "poFont.h"

void setupApplication() {
	FreeImage_Initialise();

	// make the application
	poApplication *app = poApplication::get();
	
	// make the window and add a root
	poWindow *win = new poWindow(WINDOW_TYPE_NORMAL, "MEH", 100, 100, 800, 600);
	win->setRootObject(new TestObj());
	
	// add the window
	app->addWindow(win);
}

void cleanupApplication() {
	// delete all the windows
	poApplication::get()->quit();
}

TestObj::TestObj() {
	glClearColor(0,0,0,1);
	
	poFont *f1 = new poFont("fonts/ScalaPro.otf", 100);
	poShape2D *glyph = f1->getGlyphOutline("£");
	glyph->fillDrawStyle(GL_LINE_STRIP);
	glyph->fillColor(poColor(.3,.3,.3,1));
	glyph->position.set(100,100,0);
	addChild(glyph);
	
	resources.add(f1);
}

void TestObj::update() {
}

void TestObj::draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, getWindowWidth(), getWindowHeight(), 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TestObj::eventHandler(poEvent *event) {
}
