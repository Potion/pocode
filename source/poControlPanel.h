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

#pragma once

using namespace std;

static const std::string CUI_WINDOW_EVENT = "ui panel value changed";

class poControlPanel : public poObject {

public:
    poControlPanel( string label, string _filename = "settings.xml" );
    
    virtual void eventHandler(poEvent *event);
    
    void addControl( poObject* obj );
    void addSliderF( string _ID, float min=0, float max=1, poObject* obj=NULL );
    void addSliderI( string _ID, int min=0, int max=255, poObject* obj=NULL );
    void addToggle( string _ID, poObject* obj=NULL );
    void addInputTextBox( string _ID, poObject* obj=NULL );
    void addPointSlider( string _ID, poPoint min=poPoint(0,0), poPoint max=poPoint(1,1) ,poObject* obj=NULL );
    void autoResize();
    
    
    void                addListener( poObject* obj );
    void                saveSettings();
    void                readSettings();
    bool                getBool( string s);
	int					getInt( string s);
	float				getFloat( string s);
	string              getString( string s);
	poPoint				getPoint( string s);
	poColor				getColor( string s);

    
    poObject*    container;
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

};


