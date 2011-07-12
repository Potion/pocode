#include "TestApp.h"

#include "poGUI.h"
#include "Helpers.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"
#include "poMaskModifier.h"
#include "poXML.h"

using namespace std;
using namespace boost;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	fs::path path;
	pathToFolder("xcode", &path);
	setCurrentPath(path/"test/resources");
	
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 650, 500, 700, 700);
}

void cleanupApplication() {
}

poTextureFitOption fit = PO_TEX_FIT_NONE;
poAlignment opt = PO_ALIGN_TOP_LEFT;
poTexture *tex = NULL;

TestObj::TestObj() {
	addModifier(new poCamera2D());
	addEvent(PO_KEY_DOWN_EVENT, this);
	
	tex = new poTexture("images/grid_landscape.png");
	
	poRectShape *shape = new poRectShape(200,300);
	shape->placeTexture(tex, fit, opt).drawBounds(true);
	addChild(shape);
	
//	poTextBox *tb = new poTextBox(400,200);
//	tb->text("Hello <b color='1.0 0.3 0.5'>world</b>\nAnd to you too!").layout().drawBounds(true).position(50,50);
//	addChild(tb);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	if(event->keyCode == PO_DOWN_ARROW) {
		fit = poTextureFitOption(fit + 1);
		if(fit == PO_TEX_FIT_MIN)
			fit = PO_TEX_FIT_NONE;
		getChildAs<poShape2D>(this,0)->placeTexture(tex,fit,opt);
	}
	
	if(event->keyCode == PO_UP_ARROW) {
		if(fit == PO_TEX_FIT_NONE)
			fit = PO_TEX_FIT_MIN;
		fit = poTextureFitOption(fit - 1);
		getChildAs<poShape2D>(this,0)->placeTexture(tex,fit,opt);
	}
	
	if(event->keyCode == PO_RIGHT_ARROW) {
		opt = poAlignment(opt + 1);
		if(opt == PO_ALIGN_NUM_OPTIONS)
			opt = PO_ALIGN_BOTTOM_LEFT;
		getChildAs<poShape2D>(this,0)->placeTexture(tex,fit,opt);
	}
	
	if(event->keyCode == PO_LEFT_ARROW) {
		if(opt == PO_ALIGN_TOP_LEFT)
			opt = PO_ALIGN_BOTTOM_RIGHT;
		opt = poAlignment(opt - 1);
		getChildAs<poShape2D>(this,0)->placeTexture(tex,fit,opt);
	}
}

void TestObj::messageHandler(const std::string &msg, const poDictionary &dict) {
}



