/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "ParentsChildrenApp.h"
#include "poHelpers.h"

poObject *createObjectForID(uint uid) {
	return new ParentsChildrenApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "ParentsChildren", 100, 100, 800, 600);
}

void cleanupApplication() {
}
