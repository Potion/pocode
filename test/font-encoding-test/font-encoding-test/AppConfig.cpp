/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "font_encoding_testApp.h"

poObject *createObjectForID(uint uid) {
	return new font_encoding_testApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "font_encoding_test", 100, 100, 1024, 768);
}

void cleanupApplication() {
}