/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "BasicTextBoxesApp.h"

poObject *createObjectForID(uint uid) {
	return new BasicTextBoxesApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "BasicTextBoxes", 100, 100, 800, 600);
}

void cleanupApplication() {
}
