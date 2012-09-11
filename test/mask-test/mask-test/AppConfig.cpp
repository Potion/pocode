/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "mask_testApp.h"

poObject *createObjectForID(uint uid) {
	return new mask_testApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "mask_test", 100, 100, 1024, 768);
}

void cleanupApplication() {
}