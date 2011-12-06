#include "FlipCard.h"

FlipCard::FlipCard()
{
    toggled = false;
    side = 0;
    
    rect = new poRectShape("test.png");					//image from Ghostly International - Gold Panda --- Companion
    rect->scale = poPoint(.5,.5,1);
	rect->setAlignment(PO_ALIGN_CENTER_CENTER);
    rect->rotationAxis = poPoint(1,0,0);
	addChild(rect);
	
	//	press
	//	' ' for flip on current axis
	//	'x' for x-rotation
	//	'y' for y-rotation 
	//	'z' for z-rotation
	addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
}

FlipCard::~FlipCard() 
{
}

void FlipCard::update() 
{
    if( (int)rect->rotation%180 > 90 && !toggled)
        toggleTexture();
    
}

void FlipCard::flip() {
	
	float flip_duration = 1.f;
    
	// flip rotation
	rect->rotationTween
    .set(rect->rotation+180)
    .setDuration(flip_duration)
    .setTweenFunction(PO_TWEEN_QUAD_INOUT_FUNC)
    .start();
	
	// move back for rotation
	rect->positionTween
    .set(poPoint(0, 0, rect->getHeight()*5/8.f))
    .setDuration(flip_duration/2.f)
    .setTweenFunction(PO_TWEEN_SINUS_INOUT_FUNC)
    .setRepeat(PO_TWEEN_REPEAT_PINGPONG,1)
    .start();	
	
	// dim color when rotating
	rect->fillColorTween
    .set(poColor(.5f,.5f,.5f,1.f))
    .setDuration(flip_duration/2.f)
    .setTweenFunction(PO_TWEEN_SINUS_INOUT_FUNC)
    .setRepeat(PO_TWEEN_REPEAT_PINGPONG,1)
    .start();	
}

void FlipCard::toggleTexture() {
    
    side++;
    
    if(side%2 == 0){
        rect->placeTexture(poGetTexture("test.png"));
    }
    else{
        rect->placeTexture(poGetTexture("test_2.png"));        
    }
    
    toggled = true;
}

void FlipCard::eventHandler(poEvent *event) {
	
    if( event->type == PO_MOUSE_DOWN_INSIDE_EVENT && !rect->positionTween.isRunning()){
        toggled = false;
        flip();
    }
    
    else if( event->type == PO_KEY_DOWN_EVENT ){
        
        switch( event->keyChar ){
               
            case 'x':	rect->rotationAxis = poPoint(1,0,0); break;
            case 'y':	rect->rotationAxis = poPoint(0,1,0); break;
            case 'z':	rect->rotationAxis = poPoint(0,0,1); break;
            default:	break;
        }
    }
}

void FlipCard::messageHandler(const std::string &msg, const poDictionary& dict) 
{
}

