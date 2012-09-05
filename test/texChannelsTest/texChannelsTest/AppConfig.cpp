/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "fontTestApp.h"

poObject *createObjectForID(uint uid) {
	return new fontTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "fontTest", 100, 100, 1024, 768);
}

void cleanupApplication() {
}