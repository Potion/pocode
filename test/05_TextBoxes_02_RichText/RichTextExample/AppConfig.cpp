/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "RichTextExampleApp.h"

poObject *createObjectForID(uint uid) {
	return new RichTextExampleApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "RichTextExample", 100, 100, 1024, 768);
}

void cleanupApplication() {
}