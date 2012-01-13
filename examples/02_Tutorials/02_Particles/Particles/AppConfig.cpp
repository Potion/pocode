//
//  AppConfig.cpp
//  Particles
//
//  Created by filippo on 1/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ParticlesApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poImageShape.h"
#include "Particle.h"

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
