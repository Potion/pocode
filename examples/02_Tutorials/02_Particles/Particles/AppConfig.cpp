/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////

#include "ParticlesApp.h"
#include "poApplication.h"

poObject *createObjectForID(uint uid) {
	return new ParticlesApp();
}

void setupApplication() {
	poRandSeed();
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Particles", 100, 100, 800, 600);
}

void cleanupApplication() {
}
