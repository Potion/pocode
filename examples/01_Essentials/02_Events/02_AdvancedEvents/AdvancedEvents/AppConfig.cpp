/////////////////////////////////////////
//
// pocode Application Configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "AdvancedEventsApp.h"

poObject *createObjectForID(uint uid) {
	return new AdvancedEventsApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "AdvancedEvents", 100, 100, 800, 600);
}

void cleanupApplication() {
}
