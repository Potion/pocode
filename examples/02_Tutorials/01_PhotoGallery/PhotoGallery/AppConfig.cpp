/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////

#include "PhotoGalleryApp.h"
#include "poApplication.h"

poObject *createObjectForID(uint uid) {
	return new PhotoGalleryApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "PhotoGallery", 100, 100, 800, 600);
}

void cleanupApplication() {
}
