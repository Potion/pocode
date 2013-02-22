/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "BezierApp.h"

BezierApp* app;
poObject *createObjectForID(uint uid) {
    app = new BezierApp();
	return app;
}

void setupApplication() {
    lookUpAndSetPath("resources");
    applicationCreateWindow(0, WINDOW_TYPE_FULLSCREEN, "MSI_Car", 0, 0, 1024, 768);
}

void cleanupApplication() {
}