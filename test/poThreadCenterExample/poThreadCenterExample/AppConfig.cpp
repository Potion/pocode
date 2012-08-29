/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "poThreadCenterExampleApp.h"

poObject *createObjectForID(uint uid) {
	return new poThreadCenterExampleApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "poThreadCenterExample", 100, 100, 1024, 768);
}

void cleanupApplication() {
}