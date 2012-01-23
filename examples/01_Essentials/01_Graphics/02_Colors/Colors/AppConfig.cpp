/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "ColorsApp.h"

poObject *createObjectForID(uint uid) {
	return new ColorsApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Colors", 100, 100, 800, 600);
}

void cleanupApplication() {
}
