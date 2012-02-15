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
//  UIWindow.h
//  controlUI
//
//  Created by Tamar Ziv on 1/30/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "poObject.h"
#include "poShapeBasics2d.h"
#include "poControl.h"
#include "poTextBox.h"
#include "poDictionary.h"
#include <set>
#include "poLayout.h"

using namespace std;

static const std::string CUI_WINDOW_EVENT = "ui panel value changed";

class poControlPanel : public poObject {

public:
    poControlPanel( string label, string _filename = "settings.xml" );
    
	virtual void update();
    virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    
    void addControl( poObject* obj );
    void addSliderF( string _ID, float min=0, float max=1, poObject* obj=NULL );
    void addKnob( string _ID, float min=0, float max=1, poObject* obj=NULL );
    void addSliderI( string _ID, int min=0, int max=255, poObject* obj=NULL );
    void addInputTextBox( string _ID, poObject* obj=NULL );
    void addPointSlider( string _ID, poPoint min=poPoint(0,0), poPoint max=poPoint(1,1) ,poObject* obj=NULL );
    void addColorSlider( string _ID, bool RGB=true ,poObject* obj=NULL );
    void addRadio( string _ID , vector<string> names, poObject* obj=NULL );
    void addMessage( string _ID , poObject* obj=NULL );
    void addToggle( string _ID, poObject* obj=NULL );
    void autoResize();
    void saveSettings();
    void readSettings();
    
    bool                getBool( string s);
	int					getInt( string s);
	float				getFloat( string s);
	string              getString( string s);
	poPoint				getPoint( string s);
	poColor				getColor( string s);

    poObject*    container;
	poLayout*	 containerLayout;
    poRectShape* bar;
    poRectShape* save;
    poRectShape* box;
    poRectShape* hide;
    poTextBox*   hideText;
    string       label;
    string       filename;
    
    poPoint      pos;
    
    int winWidth, winHeight;
    bool isDragged;
    poPoint dragOffset;
    float r,g,b,a;
    
    poDictionary settings;
	
	bool isResized;
};

