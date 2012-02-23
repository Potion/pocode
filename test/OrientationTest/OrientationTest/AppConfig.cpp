/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "OrientationTestApp.h"

poObject *createObjectForID(uint uid) {
	return new OrientationTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "OrientationTest", 100, 100, 1024, 768);
}

void cleanupApplication() {
}