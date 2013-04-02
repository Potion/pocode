/////////////////////////////////////////
//
// pocode : Particles
//
/////////////////////////////////////////

#include "ParticlesApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poImageShape.h"
#include "Particle.h"

extern void dbgPrintf(const char* fmt, ...);

ParticlesApp::ParticlesApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	for(int i=0; i < 200; i++) {
		Particle* P = new Particle();
		addChild(P);
	}
}

ParticlesApp::~ParticlesApp() {
}

void ParticlesApp::update() {

	static int frameCount = 0;
	static float lastFrameMark = poGetElapsedTime();

	float now = poGetElapsedTime();
	if(now - lastFrameMark > 1.f) {
		dbgPrintf("fps: %d\n", frameCount);
		lastFrameMark = now;
		frameCount = 0;
	}
	frameCount += 1;
}

void ParticlesApp::eventHandler(poEvent *event) {
}

void ParticlesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
