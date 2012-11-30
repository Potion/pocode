/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "XMLApp.h"
#include "poHelpers.h"

poObject *createObjectForID(uint uid) {
	return new XMLApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "XML", 100, 100, 800, 600);
}

void cleanupApplication() {
}
