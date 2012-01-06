//
//  Particle.cpp
//  Particle
//
//  Created by Potion on 12/1/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "Particle.h"

Particle::Particle() {
	
	float defaultScale = poRand(0.4, 1);
	
	shape = new poStarShape( 50, 50, 5, 10 );
	shape->fillColor = poColor::orange;
	shape->alpha = 0.75;
	shape->setAlignment(PO_ALIGN_CENTER_CENTER);
	shape->scale.set(defaultScale, defaultScale, 1);
	shape->scaleTween.setTweenFunction(PO_TWEEN_QUAD_OUT_FUNC);
	shape->scaleTween.setRepeat(PO_TWEEN_REPEAT_PINGPONG, 1);
	shape->scaleTween.setDuration(1);
	shape->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, "particle clicked");
	addChild(shape);
	
	float posX = poRand(100, 700);
	float posY = poRand(200, 500);
	
	position.set(posX, posY, 0);
	
	float velX = poRand(-0.6, 0.6);
	float velY = poRand(-0.6, 0.6);
	
	velocity.set(velX, velY, 0);
	
	if(velocity.getLength() < 0.2) {
		velocity *= (0.2 / velocity.getLength());
	}
	
	rotationSpeed = poRand( -1.0, 1.0 );
}

void Particle::update() {
	
	position += velocity;
	
	if( position.x > 750 || position.x < 50 ) velocity.x *= -1;
	if( position.y > 550 || position.y < 150 ) velocity.y *= -1;
	
	rotation += rotationSpeed;
}

void Particle::eventHandler(poEvent *event) {
	
	if( event->message == "particle clicked" ) {
		
		if( shape->scaleTween.isRunning() )
			return;
		
		shape->scaleTween.set( poPoint(2, 2, 1) );
		shape->scaleTween.start();
	}
	
}
