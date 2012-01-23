/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "BasicTweensApp.h"

poObject *createObjectForID(uint uid) {
	return new BasicTweensApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "BasicTweens", 100, 100, 800, 600);
}

void cleanupApplication() {
}
