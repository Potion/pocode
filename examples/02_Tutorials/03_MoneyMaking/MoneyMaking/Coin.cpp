/////////////////////////////////////////
//
// pocode : Money Making
//
/////////////////////////////////////////

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
	
	coinImage = new poImageShape(imgUrl, true);
	coinImage->setAlignment(PO_ALIGN_CENTER_CENTER);
	coinImage->positionTween.setTweenFunction(PO_TWEEN_EXPO_OUT_FUNC);
	coinImage->positionTween.setDuration(1);
	coinImage->setAlphaTest(true);
	coinImage->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	coinImage->addEvent(PO_MOUSE_DRAG_INSIDE_EVENT, this);
	addChild( coinImage );
	
	isDragged = false;
	
	addEvent(PO_MOUSE_UP_EVENT, this);
}

void Coin::eventHandler( poEvent* event ) {
	
	if( event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
		
		isDragged = true;
		dragOffset = event->localPosition;
		dragOffset += coinImage->offset;
	}
	else if( event->type == PO_MOUSE_DRAG_INSIDE_EVENT ) {
		
		coinImage->position = event->globalPosition - dragOffset;
	}
	else if( event->type == PO_MOUSE_UP_EVENT && isDragged ) {
		
		isDragged = false;
		
		if( dropArea.contains( coinImage->position ) ) {
			
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

