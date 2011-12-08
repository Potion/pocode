/////////////////////////////////////////
//
// pocCode Application Configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "DictionariesApp.h"

poObject *createObjectForID(uint uid) {
	return new DictionariesApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Dictionaries", 100, 100, 800, 600);
}

void cleanupApplication() {
}
