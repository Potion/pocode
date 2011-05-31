#include "TestApp.h"

#include "poApplication.h"
#include "poCamera.h"
#include "poCommon.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 1024, 768);
}

void cleanupApplication() {
	// make sure whatever global props you changed get written out
	poCommon::get()->write("common.xml");
}


TestObj::TestObj() {
    addModifier(new poCamera2D(poColor::black));

//	poFontMap fonts;
//	fonts.font(poFontMap::REGULAR_FONT_KEY, new poFont("fonts/ScalaPro.otf",45.f));
//	
//	poCurveLayout layout;
//	layout.text = "hello world on a curve";
//	layout.fonts = &fonts;
//	layout.render();
//	
//	poRectShape *rect = new poRectShape(layout.rendered->copy());
//	rect->position(100,100);
//	addChild(rect);
	
	poTextBox *tb = new poTextBox(300,300);
	tb->text("hello world").layout();
	addChild(tb);
}

void TestObj::update() {
    
}

void TestObj::eventHandler(poEvent *event) {
}


