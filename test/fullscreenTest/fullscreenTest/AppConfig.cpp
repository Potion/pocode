/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "fullscreenTestApp.h"

poObject *createObjectForID(uint uid) {
	return new fullscreenTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_FULLSCREEN, "fullscreenTest", 100, 100, 1024, 768);
}

void cleanupApplication() {
}