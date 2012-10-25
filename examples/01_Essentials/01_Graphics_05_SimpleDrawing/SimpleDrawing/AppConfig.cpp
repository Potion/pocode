/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "SimpleDrawingApp.h"
#include "poApplication.h"
#include "poHelpers.h"

poObject *createObjectForID(uint uid) {
	return new SimpleDrawingApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "SimpleDrawing", 100, 100, 800, 600);
}

void cleanupApplication() {
}
