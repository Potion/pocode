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
}

void ParticlesApp::eventHandler(poEvent *event) {
}

void ParticlesApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
