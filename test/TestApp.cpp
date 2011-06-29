#include "TestApp.h"

#include "poGUI.h"
#include "Helpers.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"
#include "poMaskModifier.h"

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

poShape2D *mask;

TestObj::TestObj() {
	addModifier(new poCamera2D());

	poObject *holder = new poObject();
	addChild(holder);
	
	using boost::format;
	for(int i=1; i<=4; i++) {
		poImage img((format("images/ducks/duck%d.png")%i).str());
		poRectShape *shape = new poRectShape(&img);
		shape->position(poPoint(100+i*100, 100+i*100));
		holder->addChild(shape);
	}
	
	mask = createShapesFromSVGfile("images/mask.svg").front();
	mask->alignment(PO_ALIGN_TOP_CENTER);
	addModifier(new poGeometryMask(mask));
}

void TestObj::draw() {
}

void TestObj::update() {
	mask->rotation(mask->rotation()+5);
}

void TestObj::eventHandler(poEvent *event) {
}

void TestObj::messageHandler(const std::string &msg, const poDictionary &dict) {
	
}



