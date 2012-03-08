/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "poMessageCenterExampleApp.h"

poObject *createObjectForID(uint uid) {
	return new poMessageCenterExampleApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "poMessageCenterExample", 100, 100, 1024, 768);
}

void cleanupApplication() {
}