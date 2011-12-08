/////////////////////////////////////////
//
// poCode Application Configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "AdvancedTextBoxesApp.h"

poObject *createObjectForID(uint uid) {
	return new AdvancedTextBoxesApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "AdvancedTextBoxes", 100, 100, 800, 600);
}

void cleanupApplication() {
}
