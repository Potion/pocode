/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "VideoPlayerApp.h"

poObject *createObjectForID(uint uid) {
	return new VideoPlayerApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "VideoPlayer", 100, 100, 1024, 768);
}

void cleanupApplication() {
}