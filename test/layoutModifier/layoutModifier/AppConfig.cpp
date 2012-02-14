/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "layoutModifierApp.h"

poObject *createObjectForID(uint uid) {
	return new layoutModifierApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "layoutModifier", 100, 100, 1400, 900);
}

void cleanupApplication() {
}