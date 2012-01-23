/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "PointsApp.h"
#include "poApplication.h"

poObject *createObjectForID(uint uid) {
	return new PointsApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Points", 100, 100, 800, 600);
}

void cleanupApplication() {
}
