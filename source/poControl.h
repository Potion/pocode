/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

//
//  cUIElements.h
//  cUIElements
//
//  Created by Tamar Ziv on 11/21/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poImageShape.h"
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
    
    poSliderF( string _ID, float init, float _min = 0, float _max = 1, string _valName = "" ,poObject* _listener = NULL );
    virtual void eventHandler(poEvent *event);

    poRectShape* back;
    poRectShape* sliderShape;
    poRectShape* sliderKnob;
    poTextBox*   shapeData;
    poTextBox*   shapeLabel;
    string valName;
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
    string valName;

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

    poRectShape* toggleShape;
    poTextBox*   shapeLabel;

};

class poRadio : public poControl {
public:
    
    poRadio( string _ID, int init, vector<string> names ,poObject* _listener = NULL );
    virtual void eventHandler(poEvent *event);
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    vector <poToggleBox*> buttons;  
    poTextBox*   shapeLabel;
    
};


class poInputTextBox : public poControl {
public:
    
    poInputTextBox( string _ID, string init, poObject* _listener = NULL );
    virtual void eventHandler(poEvent *event);

    poRectShape* back;
    poRectShape* textShape;
    poTextBox*   shapeLabel;
    poTextBox*   shapeData;
    poTextBox*   inputBox;
    
};

class poColorSlider : public poControl {
public:
    
    poColorSlider( string _ID, poColor init, bool _RGB, poObject* _listener = NULL );
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    virtual void eventHandler(poEvent *event);
    poTexture*      calcTex ( poColor input );

    poRectShape*        colorBox;
    vector <poSliderF*> sliders;
    poPoint             min,max;
    poTextBox*          shapeLabel;
    bool                RGB;
};


class poKnob : public poControl {
public:
    
    poKnob( string _ID, float init, float _min = 0, float _max = 1, poObject* _listener = NULL );
    virtual void eventHandler(poEvent *event);
    
    poRectShape* back;
    poOvalShape* sliderShape;
    poRectShape* sliderKnob;
    poTextBox*   shapeData;
    poTextBox*   shapeLabel;
    float min,max;
    poPoint dragOffset;
};


class poMessage : public poControl {
public:
    
    poMessage( string _ID, poObject* _listener = NULL );
    virtual void eventHandler(poEvent *event);
    
    poRectShape* back;
    poTextBox*   shapeLabel;
};
