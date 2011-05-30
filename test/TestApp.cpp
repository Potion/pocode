#include "TestApp.h"

#include "poApplication.h"
#include "poCamera.h"
#include "poCommon.h"
#include "poTextBox.h"

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
	
	poResourceStore tmp;
	
	poTextBox *tb = new poTextBox();
	tb->text("pello gorld\nhopefully <u>this is</u> a new line?\nœ<u>∑´®†</u>¥¨")
		.font(poFontMap::REGULAR_FONT_KEY, tmp.add(new poFont("ScalaPro", FONT_REGULAR, 30)))
		.bounds(poRect(0,0,300,300))
		.position(100,100,0);
	tb->layout();
	addChild(tb);
}

void TestObj::update() {
    
}

void TestObj::eventHandler(poEvent *event) {
}


