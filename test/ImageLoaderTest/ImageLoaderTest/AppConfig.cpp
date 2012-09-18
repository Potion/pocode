/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "ImageLoaderTestApp.h"

poObject *createObjectForID(uint uid) {
	return new ImageLoaderTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "ImageLoaderTest", 100, 100, 1024, 768);
}

void cleanupApplication() {
}