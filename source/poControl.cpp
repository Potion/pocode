//
//  cUIElements.cpp
//  cUIElements
//
//  Created by Tamar Ziv on 11/21/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "poControl.h"
#include "poHelpers.h"
#include "poApplication.h"

poControl::poControl( string _ID, poObject* _listener )
{
    ID = _ID;
    listener = _listener;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

poSliderF::poSliderF( string _ID, float init, float _min, float _max, poObject* _listener ) : poControl( _ID, _listener )
{
    
    valF = init;
    
    char valString [256];
    sprintf( valString, "%.2lf", valF );
    
    this->name = ID;
    min = _min;
    max = _max;
    
    addEvent( PO_MOUSE_DOWN_EVENT, this );
    
    poRectShape* back = new poRectShape( SLIDER_WIDTH+SLIDER_HEIGHT, SLIDER_HEIGHT);
    back->fillColor = poColor(1,1,1,0.2);
    addChild( back );
    
    sliderShape = new poRectShape( SLIDER_WIDTH,SLIDER_HEIGHT+SPACER );
    sliderShape->fillColor = poColor(0,0,0,0);
    sliderKnob = new poRectShape( SLIDER_HEIGHT,SLIDER_HEIGHT );
    sliderKnob->fillColor = poColor(1,1,1,0.2);
    sliderKnob->position.x = SLIDER_WIDTH * valF/max; 
    sliderKnob->addEvent( PO_MOUSE_DRAG_INSIDE_EVENT, this );
    sliderKnob->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this );
    
    addChild( sliderShape );
    sliderShape->addChild( sliderKnob );
    
    shapeData = new poTextBox( SLIDER_HEIGHT,SLIDER_HEIGHT );
	shapeData->textColor = poColor::white;
    shapeData->position.set(0,0,0);
	shapeData->setFont( poGetFont ("Courier") );
    shapeData->setTextSize(12);
    shapeData->setText(valString);
	shapeData->doLayout();  
    addChild( shapeData );
    
    shapeLabel = new poTextBox( SLIDER_HEIGHT,SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( SLIDER_WIDTH+SPACER*4,0,0);
	shapeLabel->setFont( poGetFont ("Courier") );
    shapeLabel->setTextSize(12);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();  
    addChild( shapeLabel );
    
}

void poSliderF::eventHandler(poEvent *event) {
    
    
    if (event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
        
        dragOffset = event->localPosition;
    }

    if (event->type == PO_MOUSE_DRAG_INSIDE_EVENT ) {
        
        poPoint sOffset = sliderShape->localToGlobal( poPoint(0,0) );
        
        float sliderWidth = sliderShape->getWidth();
        float knobWidth = sliderKnob->getWidth();
        float xMin = 0;
        float xMax = sliderWidth - knobWidth/2;
        float posX = event->globalPosition.x - dragOffset.x - sOffset.x;
        
        posX = clamp( xMin, xMax, posX);
        valF =  sqrt( pow(posX/xMax,2) ); 
        
        float mappedVal = poMapf(0.0f,1, valF, min, max );;        
        
        sliderKnob->position.x = sliderWidth*valF; 
        poDictionary D;
        
        char valString [256];
        sprintf( valString, "%.2lf", mappedVal );
        shapeData->setText( valString );
        shapeData->doLayout();  
        
        D.set("value", mappedVal);
        valF = mappedVal;
        
        if ( listener == NULL ) 
        {
//            getParent()->messageHandler( ID, D);
        }
            
        else
            listener->messageHandler( ID, D);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////

poSliderI::poSliderI( string _ID, int init, int _min, int _max, poObject* _listener ) : poControl( _ID, _listener )
{
    
    valI = init;
    valF = (float)init;
    
    char valString [256];
    sprintf( valString, "%i", valI );
    
    this->name = ID;
    min = _min;
    max = _max;
    
    addEvent( PO_MOUSE_DOWN_EVENT, this );
    poRectShape* back = new poRectShape( SLIDER_WIDTH+SLIDER_HEIGHT, SLIDER_HEIGHT);
    back->fillColor = poColor(1,1,1,0.2);
    addChild( back );
    
    sliderShape = new poRectShape( SLIDER_WIDTH,SLIDER_HEIGHT+SPACER );
    sliderShape->fillColor = poColor(0,0,0,0);
    sliderKnob = new poRectShape( SLIDER_HEIGHT,SLIDER_HEIGHT );
    sliderKnob->fillColor = poColor(1,1,1,0.2);
    sliderKnob->position.x = SLIDER_WIDTH * valF/max; 
    sliderKnob->addEvent( PO_MOUSE_DRAG_INSIDE_EVENT, this );
    sliderKnob->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this );
    
    addChild( sliderShape );
    sliderShape->addChild( sliderKnob );
    
    shapeData = new poTextBox( SLIDER_HEIGHT,SLIDER_HEIGHT );
	shapeData->textColor = poColor::white;
    shapeData->position.set(0,0,0);
	shapeData->setFont( poGetFont ("Courier") );
    shapeData->setTextSize(12);
    shapeData->setText(valString);
	shapeData->doLayout();  
    addChild( shapeData );
    
    shapeLabel = new poTextBox( SLIDER_HEIGHT,SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( SLIDER_WIDTH+SPACER*4,0,0);
	shapeLabel->setFont( poGetFont ("Courier") );
    shapeLabel->setTextSize(12);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();  
    addChild( shapeLabel );
    
}

void poSliderI::eventHandler(poEvent *event) {
    
    
    if (event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
        
        dragOffset = event->localPosition;
    }
    
    
    if (event->type == PO_MOUSE_DRAG_INSIDE_EVENT ) {
        
        poPoint sOffset = sliderShape->localToGlobal( poPoint(0,0) );
        float sliderWidth = sliderShape->getWidth();
        float knobWidth = sliderKnob->getWidth();
        float xMin = 0;
        float xMax = sliderWidth - knobWidth/2;
        float posX = event->globalPosition.x - dragOffset.x - sOffset.x;
        
        posX = clamp( xMin, xMax, posX);
        
        valF =  sqrt( pow(posX/xMax,2) ); 
        float mappedVal = poMapf(0.0f,1, valF, min, max );;        
        
        sliderKnob->position.x = sliderWidth*valF; 
        poDictionary D;
        
        char valString [256];
        sprintf( valString, "%i", (int)mappedVal );
        shapeData->setText( valString );
        shapeData->doLayout();  
        
        D.set("value", (int)mappedVal );
        valI = (int)mappedVal;

        if ( listener == NULL )
        {
//            getParent()->messageHandler( ID, D);
        }
        else
            listener->messageHandler( ID, D);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////

poPointSlider::poPointSlider( string _ID, poPoint init, poPoint _min, poPoint _max, poObject* _listener ) : poControl( _ID, _listener )
{
    
    valP = init;
    
    char valString [256];
    sprintf( valString, "%.2lf,%.2lf", valP.x,valP.y );
    
    this->name = ID;
    
    min = _min;
    max = _max;
    
    addEvent( PO_MOUSE_DOWN_EVENT, this );
    
    poRectShape* back = new poRectShape( SLIDER_WIDTH+SLIDER_HEIGHT, SLIDER_WIDTH );
    back->fillColor = poColor(1,1,1,0.2);
    addChild( back );
    
    poRectShape* space = new poRectShape( SLIDER_WIDTH,SLIDER_WIDTH+SPACER);
    space->fillColor = poColor(0,0,0,0);
    addChild( space );
    
    sliderShape = new poRectShape( SLIDER_WIDTH, SLIDER_WIDTH - SLIDER_HEIGHT );
    sliderShape->position = poPoint (SLIDER_HEIGHT/2,SLIDER_HEIGHT/2);
    sliderShape->fillColor = poColor(0,0,0,0);
    sliderPoint = new poRectShape( SLIDER_HEIGHT,SLIDER_HEIGHT );
    sliderPoint->position.x = valP.x/max.x * SLIDER_WIDTH; 
    sliderPoint->position.y = valP.y/max.y * SLIDER_WIDTH; 
    sliderPoint->fillColor = poColor(1,0,0,0.5);
    sliderPoint->setAlignment(PO_ALIGN_CENTER_CENTER);
    sliderPoint->addEvent( PO_MOUSE_DRAG_INSIDE_EVENT, this );
    sliderPoint->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this );
    
    addChild( sliderShape );
    sliderShape->addChild( sliderPoint );
    
    shapeData = new poTextBox( SLIDER_HEIGHT,SLIDER_HEIGHT );
	shapeData->textColor = poColor::white;
    shapeData->position.set(0,0,0);
	shapeData->setFont( poGetFont ("Courier") );
    shapeData->setTextSize(12);
    shapeData->setText( valString );
	shapeData->doLayout();  
    addChild( shapeData );
    
    shapeLabel = new poTextBox( SLIDER_HEIGHT,SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( SLIDER_WIDTH+SPACER*4,0,0);
	shapeLabel->setFont( poGetFont ("Courier") );
    shapeLabel->setTextSize(12);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();  
    addChild( shapeLabel );
    
    }

void poPointSlider::eventHandler(poEvent *event) {
    
    
    if (event->type == PO_MOUSE_DRAG_INSIDE_EVENT ) 
    {
        poPoint P = event->globalPosition;
        P.y = getWindowHeight() - P.y;
        poPoint pos = sliderShape->globalToLocal( P );
        sliderPoint->position = pos; 
              
        float sliderWidth = sliderShape->getWidth();
        float boxWidth = sliderPoint->getWidth();
        float xMin = 0;
        float xMax = sliderWidth+boxWidth/2;

        float sliderHeight = sliderShape->getHeight();
        float yMin = 0;
        float yMax = sliderHeight+boxWidth/2;
 
        pos.x = clamp( xMin, xMax, pos.x);
        pos.y = clamp( yMin, yMax, pos.y);

        valP.x =  sqrt( pow( pos.x/xMax,2 ) ); 
        valP.y =  sqrt( pow( pos.y/yMax,2 ) ); 

        sliderPoint->position.x = sliderWidth*valP.x; 
        sliderPoint->position.y = sliderHeight*valP.y;
        
        poPoint mappedVal;
        mappedVal.x = poMapf(0.0f,1, valP.x, min.x, max.x);
        mappedVal.y = poMapf(0.0f,1, valP.y, min.y, max.y);        
                
        char valString [256];
        sprintf( valString, "%.2lf,%.2lf", mappedVal.x, mappedVal.y );
        shapeData->setText( valString );
        shapeData->doLayout();  
        
        poDictionary D;
        D.set("value", mappedVal);
        valP = mappedVal;
        
        if ( listener == NULL ) 
        {
//            getParent()->messageHandler( ID, D );
        }
        else
            listener->messageHandler( ID, D );
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////

poToggleBox::poToggleBox( string _ID, bool init, poObject* _listener ) : poControl( _ID, _listener )
{
    
    valB = init;
    this->name = ID;
    
    addEvent( PO_MOUSE_DOWN_EVENT, this );
    
    poRectShape* back = new poRectShape( SLIDER_HEIGHT,SLIDER_HEIGHT);
    back->fillColor = poColor::dkGrey;
    addChild( back );
    
    poRectShape* space = new poRectShape( SLIDER_HEIGHT,SLIDER_HEIGHT+SPACER);
    space->fillColor = poColor(0,0,0,0);
    addChild( space );
    
    toggleShape = new poRectShape( SLIDER_HEIGHT,SLIDER_HEIGHT );
    toggleShape->fillColor = poColor((int)valB,0,0,(int)valB);
    toggleShape->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this );
    addChild( toggleShape );
    
    shapeLabel = new poTextBox( SLIDER_HEIGHT,SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( SLIDER_HEIGHT+SPACER*2,0,0);
	shapeLabel->setFont( poGetFont ("Courier") );
    shapeLabel->setTextSize(12);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();  
    addChild( shapeLabel );
    
}

void poToggleBox::eventHandler(poEvent *event) {
 
    if (event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) 
    {
        valB = !valB;
        toggleShape->fillColor = poColor((int)valB,0,0,(int)valB);
        
        poDictionary D;
        D.set("value", valB);

        if ( listener == NULL )
        {
//            getParent()->messageHandler( ID, D);

        }
        else
        {
            listener->messageHandler( ID, D);
//            cout << valB << endl;

        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////

poInputTextBox::poInputTextBox( string _ID, string init, poObject* _listener ) : poControl( _ID, _listener )
{
    
    this->name = ID;
    string temp;
    temp = init;
//    cout << temp << endl;
    temp.erase(temp.find_last_not_of("'")+1);
    temp.erase(0,temp.find_first_not_of("'"));
    valS = temp;
        
    back = new poRectShape( SLIDER_WIDTH+SLIDER_HEIGHT, SLIDER_WIDTH );
    back->fillColor = poColor(1,1,1,0.2);
    addChild( back );
    
    poRectShape* space = new poRectShape( SLIDER_WIDTH,SLIDER_WIDTH+SPACER);
    space->fillColor = poColor(0,0,0,0);
    addChild( space );
    
    textShape = new poRectShape( SLIDER_WIDTH,SLIDER_WIDTH );
    textShape->fillColor = poColor(1,1,1,0);
    textShape->addEvent( PO_KEY_DOWN_EVENT, this );
    addChild( textShape );
    
    shapeData = new poTextBox( SLIDER_WIDTH,SLIDER_WIDTH );
	shapeData->textColor = poColor::white;
    shapeData->position.set(0,0,0);
	shapeData->setFont( poGetFont ("Courier") );
    shapeData->setTextSize(12);
    shapeData->setText( valS );
	shapeData->doLayout();  

    textShape->addChild( shapeData );
    
    shapeLabel = new poTextBox( SLIDER_HEIGHT,SLIDER_HEIGHT );
	shapeLabel->textColor = poColor::white;
    shapeLabel->position.set( SLIDER_WIDTH+SPACER*4,0,0);
	shapeLabel->setFont( poGetFont ("Courier") );
    shapeLabel->setTextSize(12);
    shapeLabel->setText( ID );
	shapeLabel->doLayout();  
    addChild( shapeLabel );
    
}

void poInputTextBox::eventHandler(poEvent *event) {
    
    if (event->type == PO_KEY_DOWN_EVENT ) 
    {
//        cout << (int)event->keyChar << endl;
        string S;
        if ( event->keyChar != 127 && event->keyChar != 13 ) {
            S = event->keyChar;
            valS = valS+S;
//            cout << "typing " << valS << endl;
        } 

        if ( event->keyChar == 127 && valS.length() > 0 ) {
            valS = valS.erase( valS.length()-1 );
        } 
        
        if ( event->keyChar == 13 ) {
            valS = valS + "\n";
        } 
        
        shapeData->setText( valS );
        shapeData->doLayout();
        shapeData->reshape( textShape->getWidth(), shapeData->getTextBounds().height );
        textShape->reshape( textShape->getWidth(), shapeData->getTextBounds().height );
        back->reshape( back->getWidth(), shapeData->getTextBounds().height );

        poDictionary D;
        D.set("value", valS);
//        cout << shapeData->getTextBounds().height << endl;
        
        if ( listener == NULL )
        {
            //            getParent()->messageHandler( ID, D);
        }
        else
            listener->messageHandler( ID, D);
    }
}