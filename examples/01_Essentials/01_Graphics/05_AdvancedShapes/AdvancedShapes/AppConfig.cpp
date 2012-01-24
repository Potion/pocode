/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "AdvancedShapesApp.h"

poObject *createObjectForID(uint uid) {
	return new AdvancedShapesApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "AdvancedShapes", 100, 100, 800, 600);
}

void cleanupApplication() {
}