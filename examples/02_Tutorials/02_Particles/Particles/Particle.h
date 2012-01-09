//
//  Particle.h
//  Particle
//
//  Created by Potion on 12/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#ifndef Particle_h
#define Particle_h

#include "poObject.h"
#include "poShapeBasics2D.h"


class Particle : public poObject {
public:
	Particle();
	
	virtual void update();
	virtual void eventHandler(poEvent* event);
	
	poShape2D* shape;
	poPoint	velocity;
	float	rotationSpeed;
//	float	defaultScale;
};


#endif
