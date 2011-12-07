//
//  Particle.h
//  Particles
//
//  Created by filippo on 12/1/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Particles_Header_h
#define Particles_Header_h

#include "poObject.h"
#include "poShapeBasics2D.h"


class Particle : public poObject {
public:
	Particle();
	
	virtual void update();
	virtual void eventHandler(poEvent* event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poShape2D* shape;
	poPoint	velocity;
	float	rotationSpeed;
	float	defaultScale;
};


#endif
