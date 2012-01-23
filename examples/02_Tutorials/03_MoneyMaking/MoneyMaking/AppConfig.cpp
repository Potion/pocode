/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////

#include "poApplication.h"
#include "MoneyMakingApp.h"

poObject *createObjectForID(uint uid) {
	return new MoneyMakingApp();
}

void setupApplication() {
	poRandSeed();
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MoneyMaking", 100, 100, 800, 600);
}

void cleanupApplication() {
}