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
//	addModifier(new poCamera2D());
	addModifier(new poOrthoCamera(0,getWindowHeight(),getWindowWidth(),0,-1,1));
	
	addEvent(PO_MOUSE_DOWN_EVENT, this);
	addEvent(PO_MOUSE_MOVE_EVENT, this);
	addEvent(PO_MOUSE_UP_EVENT, this);
	
	poRectShape *shp;
	addChild((shp = new poRectShape(100,100))); shp->position(100,100).addEvent(PO_MOUSE_PRESS_EVENT,this);
	addChild((shp = new poRectShape(100,100))); shp->position(300,500).addEvent(PO_MOUSE_PRESS_EVENT,this);
	

//	addEvent(PO_KEY_DOWN_EVENT, this);
//	tex = new poTexture("images/grid_landscape.png");
//	poRectShape *shape = new poRectShape(200,300);
//	shape->placeTexture(tex, fit, opt).drawBounds(true);
//	addChild(shape);
	
//	poTextBox *tb = new poTextBox(400,200);
//	tb->text("Hello <b color='1.0 0.3 0.5'>world</b>\nAnd to you too!").layout().drawBounds(true).position(50,50);
//	addChild(tb);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	
	if(event->type == PO_MOUSE_MOVE_EVENT) {
		for(int i=0; i<numChildren(); i++) {
			poShape2D *shp = getChildAs<poShape2D>(this, i);
			shp->fillColor(shp->pointInside(event->position,true) ? poColor::red : poColor::white);
		}
	}
	else if(event->type == PO_MOUSE_DOWN_EVENT) {
		for(int i=0; i<numChildren(); i++) {
			poShape2D *shp = getChildAs<poShape2D>(this, i);
			shp->fillColor(shp->pointInside(event->position,true) ? poColor::orange : poColor::white);
		}
	}
	else if(event->type == PO_MOUSE_UP_EVENT) {
		for(int i=0; i<numChildren(); i++) {
			poShape2D *shp = getChildAs<poShape2D>(this, i);
			shp->fillColor(poColor::white);
		}
	}
	else if(event->type == PO_MOUSE_PRESS_EVENT) {
		((poShape2D*)event->source)->fillColor(poColor::green);
	}
	
	return;
	
	
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



