/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "poNetworkLoaderApp.h"

poObject *createObjectForID(uint uid) {
	return new poNetworkLoaderApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "poNetworkLoader", 100, 100, 1024, 768);
}

void cleanupApplication() {
}