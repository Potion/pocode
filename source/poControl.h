//
//  cUIElements.h
//  cUIElements
//
//  Created by Tamar Ziv on 11/21/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef cUIElements_h
#define cUIElements_h

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poTextBox.h"

#define TEXT_SIZE 12
#define SLIDER_HEIGHT 15
#define SLIDER_WIDTH 150
#define SPACER 5

using namespace std;

class poControl : public poObject {
public:
    
    poControl( string _ID, poObject* _listener = NULL );
    string ID;    
    poObject* listener;
    
    float       valF;
    int         valI;
    bool        valB;
    poPoint     valP;
    poColor     valC;
    string      valS;
};

class poSliderF : public poControl {
public:
    
    poSliderF( string _ID, float init, float _min = 0, float _max = 1, poObject* _listener = NULL );
    virtual void eventHandler(poEvent *event);
    
    poRectShape* sliderShape;
    poRectShape* sliderKnob;
    poTextBox*   shapeData;
    poTextBox*   shapeLabel;
    float min,max;
    poPoint dragOffset;
};

class poSliderI : public poControl {
public:
    
    poSliderI( string _ID, int init, int _min = 0, int _max = 255, poObject* _listener = NULL );
    virtual void eventHandler(poEvent *event);
    
    poRectShape* sliderShape;
    poRectShape* sliderKnob;
    poTextBox*   shapeData;
    poTextBox*   shapeLabel;
    int min,max;
    poPoint dragOffset;    
};

class poPointSlider : public poControl {
public:
    
    poPointSlider( string _ID, poPoint init, poPoint _min = poPoint(0,0), poPoint _max = poPoint(1,1), poObject* _listener = NULL );
    virtual void eventHandler(poEvent *event);
    
    poRectShape* sliderShape;
    poRectShape* sliderPoint;
    poTextBox*   shapeData;
    poTextBox*   shapeLabel;
    poPoint min,max;
    
};


class poToggleBox : public poControl {
public:
    
    poToggleBox( string _ID, bool init, poObject* _listener = NULL );
    virtual void eventHandler(poEvent *event);

    poRectShape* parent;
    poRectShape* toggleShape;
    poTextBox*   shapeLabel;

};

class poInputTextBox : public poControl {
public:
    
    poInputTextBox( string _ID, string init, poObject* _listener = NULL );
    virtual void eventHandler(poEvent *event);

    poRectShape* back;
    poRectShape* parent;
    poRectShape* textShape;
    poTextBox*   shapeLabel;
    poTextBox*   shapeData;
    poTextBox*   inputBox;
    
};






#endif
