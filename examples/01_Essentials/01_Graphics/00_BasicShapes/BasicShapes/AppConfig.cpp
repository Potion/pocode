/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "BasicShapesApp.h"

poObject *createObjectForID(uint uid) {
	return new BasicShapesApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "BasicShapes", 100, 100, 800, 600);
}

void cleanupApplication() {
}
