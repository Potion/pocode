#include "poObjectReadWriteApp.h"

#include "Helpers.h"
#include "poCamera.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"
#include "poResourceStore.h"

#include <boost/assign/list_of.hpp>

poObject *createObjectForID(uint uid) {
	return new poObjectReadWriteApp();
}

void setupApplication() {
	lookUpAndSetPathNextTo("poObjectReadWrite");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "poObjectReadWrite", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

poObjectReadWriteApp::poObjectReadWriteApp() {
	addModifier(new poCamera2D());
	
	poShape2D *obj = addChild(new poRectShape(getImage("imgres.jpeg")->texture()));
	poXMLDocument doc("square.xml");
	obj->read(doc.rootNode());

	
//	obj->generateStroke(10);
//	obj->alignment(PO_ALIGN_CENTER_CENTER);
//	obj->position = getWindowCenter();
//	obj->fillColor = poColor::blue;
//	obj->strokeColor = poColor::orange;
//	obj->drawBounds = true;
//	
//	poXMLDocument doc;
//	poXMLNode root = doc.rootNode();
//	obj->write(root);
//	
//	doc.write("square.xml");
}

void poObjectReadWriteApp::eventHandler(poEvent *event) {
	
}
