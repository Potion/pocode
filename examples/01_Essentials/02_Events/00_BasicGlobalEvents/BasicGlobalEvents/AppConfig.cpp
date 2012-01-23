/////////////////////////////////////////
//
// pocode Application Configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "BasicGlobalEventsApp.h"

poObject *createObjectForID(uint uid) {
	return new BasicGlobalEventsApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "BasicGlobalEvents", 100, 100, 800, 600);
}

void cleanupApplication() {
}