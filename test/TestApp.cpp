#include "TestApp.h"

#include "poGUI.h"
#include "Helpers.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"

using namespace std;
using namespace boost;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	fs::path path;
	pathToFolder("xcode", &path);
	setCurrentPath(path/"test/resources");
	
	log("application started");
	log("path is %s", currentPath().c_str());
	
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 650, 500, 700, 700);
}

void cleanupApplication() {
	log("application ending");
}

void updateTb(poTextBox *tb, float f, int prop) {
	switch(prop) {
		case 1:
			tb->tracking(f);
			break;
		case 2:
			tb->leading(f);
			break;
	}
	tb->layout();
}

TestObj::TestObj() {
	addModifier(new poCamera2D());

//	poObject *obj = createShapesFromSVGfile("images/tester.svg");
//	addChild(obj);
	
//	poTextBox *tb = new poTextBox(600, 600);
//	tb->text("But I must explain to you how all this mistaken idea of denouncing pleasure and praising pain was born and I will give you a complete account of the system, and expound the actual teachings of the great explorer of the truth, the master-builder of human happiness. No one rejects, dislikes, or avoids pleasure itself, because it is pleasure, but because those who do not know how to pursue pleasure rationally encounter consequences that are extremely painful.")
//		.drawBounds(true)
//		.padding(10)
//		.tracking(1.f)
//		.leading(1.f)
//		.layout()
//		.position(50, 50);
//	addChild(tb);
//	
//	poGUISlider *slider = new poGUISlider("tracking", 1.f, 0.1f, 5.f, 0.01f, boost::bind(updateTb, tb, _1, 1));
//	addChild(slider);
//	
//	slider = new poGUISlider("leading", 1.f, 0.1f, 5.f, 0.01f, boost::bind(updateTb, tb, _1, 2));
//	slider->position(0,17);
//	addChild(slider);
	
	poShape2D *shape = new poOvalShape(300,300,100);
	shape->generateStroke(10,STROKE_PLACE_CENTER).strokeColor(poColor(1,0,0,.5)).position(350,350);
	addChild(shape);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
}

void TestObj::messageHandler(const std::string &msg, const poDictionary &dict) {
}



