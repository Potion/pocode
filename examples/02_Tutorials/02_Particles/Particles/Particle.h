/////////////////////////////////////////
//
// pocode : Particles
//
/////////////////////////////////////////

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
};


#endif
