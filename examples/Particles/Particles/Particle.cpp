//
//  Particle.cpp
//  Particles
//
//  Created by filippo on 12/1/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Particle.h"


Particle::Particle() {
	
	float size = poRand(0.3, 1);
	float posX = poRand(50, 750);
	float posY = poRand(150, 550);
	float velX = poRand(-1, 1);
	float velY = poRand(-1, 1);
	
	if(velX >= 0) poMapf(0, 1, velX, 0.2, 1);
	else poMapf(-1, 0, velX, -1, -0.2);
	
	if(velY >= 0) poMapf(0, 1, velY, 0.2, 1);
	else poMapf(-1, 0, velY, -1, -0.2);
	
	shape = new poStarShape( 50, 50, 5, 10 );
	shape->fillColor = poColor::orange;
	shape->alpha = 0.75;
	shape->scale.set(size, size, 1);
	shape->rotationTween.setTweenFunction(PO_TWEEN_QUAD_OUT_FUNC);
	shape->rotationTween.setDuration(4);
	shape->setAlignment(PO_ALIGN_CENTER_CENTER);
	shape->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	addChild(shape);
	
	position.set(posX, posY, 0);
	velocity.set(velX, velY, 0);
	
	rotationSpeed = poRand( -1.0, 1.0 );
}

void Particle::update() {
	
	position += velocity;
	
	if( position.x > 800-50 || position.x < 0+50 ) velocity.x *= -1;
	if( position.y > 600-50 || position.y < 100+50 ) velocity.y *= -1;
	
	rotation += rotationSpeed;
	
}

void Particle::eventHandler(poEvent *event) {
	
	if( event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
		
		if( shape->rotationTween.isRunning() )
			return;
		
		shape->rotationTween.set( 360 );
		shape->rotationTween.setNotification(this, "rotation done");
		shape->rotationTween.start();
		
		poDictionary dict;
		dict.set("position", position);
		getParent()->messageHandler("particle clicked", dict);
	}
}

void Particle::messageHandler(const std::string &msg, const poDictionary& dict) {
	
	if(msg == "rotation done") {
		
		shape->rotation = 0;
		shape->rotationTween.set( 360 );
		shape->rotationTween.setNotification(NULL);
	}
}