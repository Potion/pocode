/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "ControlPanelApp.h"
#include "poHelpers.h"

poObject *createObjectForID(uint uid) {
	return new ControlPanelApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "ControlPanel", 100, 100, 800, 600);
}

void cleanupApplication() {
}