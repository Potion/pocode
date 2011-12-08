/////////////////////////////////////////
//
// poCode Application Configuration
//
/////////////////////////////////////////


#include "UpdateApp.h"
#include "poApplication.h"

poObject *createObjectForID(uint uid) {
	return new UpdateApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Update", 100, 100, 800, 600);
}

void cleanupApplication() {
}
