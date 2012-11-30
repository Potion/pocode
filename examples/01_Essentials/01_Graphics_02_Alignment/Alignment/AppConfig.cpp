/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "AlignmentApp.h"
#include "poHelpers.h"

poObject *createObjectForID(uint uid) {
	return new AlignmentApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Alignment", 100, 100, 800, 600);
}

void cleanupApplication() {
}