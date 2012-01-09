//
//  Coin.cpp
//  MoneyMaking
//
//  Created by filippo on 11/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Coin.h"

Coin::Coin( coinType type ) {
	
	std::string imgUrl;
	
	if( type == CENT ) {
		imgUrl = "cent.png";
		value = 0.01;
	}
	else if( type == HALF_DIME ) {
		imgUrl = "half-dime.png";
		value = 0.05;
	}
	else if( type == DIME ) {
		imgUrl = "dime.png";
		value = 0.10;
	}
	else if( type == QUARTER ) {
		imgUrl = "quarter.png";
		value = 0.25;
	}
	else if( type == HALF_DOLLAR ) {
		imgUrl = "half-dollar.png";
		value = 0.50;
	}
	else if( type == DOLLAR ) {
		imgUrl = "dollar.png";
		value = 1.00;
	}
	
	float x = poRand(50, 250);
	float y = poRand(getWindowHeight() - 100, getWindowHeight() - 300);
	
	printf("position %f %f\n", x, y);
	
	coinImage = new poImageShape(imgUrl, true);
	coinImage->setAlphaTest(true);
	coinImage->scale.set(0.5, 0.5, 1);
	coinImage->setAlignment(PO_ALIGN_CENTER_CENTER);
	coinImage->position.set(x, y, 0);
	coinImage->positionTween.setTweenFunction(PO_TWEEN_EXPO_OUT_FUNC).setDuration(1);
	coinImage->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	coinImage->addEvent(PO_MOUSE_DRAG_INSIDE_EVENT, this);
	addChild( coinImage );
	
	isDragged = false;
	
	addEvent(PO_MOUSE_UP_EVENT, this);
}

Coin::~Coin() {
}

void Coin::eventHandler( poEvent* event ) {
	
	if( event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
		
//		dragOffset = event->localPosition;
		dragOffset.set( event->localPosition.x, event->localPosition.y, 0);
		dragOffset.x += coinImage->offset.x;
		dragOffset.y += coinImage->offset.y;
		dragOffset.x *= coinImage->scale.x;
		dragOffset.y *= coinImage->scale.y;
		printf("%f\n", event->localPosition.z);
	}
	else if( event->type == PO_MOUSE_DRAG_INSIDE_EVENT ) {
		
		isDragged = true;
		coinImage->position = event->globalPosition - dragOffset;
	}
	else if( event->type == PO_MOUSE_UP_EVENT && isDragged ) {
		
		isDragged = false;
		
		poRect droppingArea(480, 105, 255, 400);
		
		if( droppingArea.contains( coinImage->position ) ) {
			
			poPoint droppedPosition(coinImage->position.x, getWindowHeight() - 250, 0);
			coinImage->positionTween.set( droppedPosition ).setNotification(this, "drop done").start();
		
			poDictionary Dict;
			Dict.set("value", value);
			
			getParent()->messageHandler("coin dropped", Dict);
		}
	}
}

void Coin::messageHandler(const std::string &msg, const poDictionary& dict) {
	
	if( msg == "drop done" ) {
		
		coinImage->visible = false;
	}
}

