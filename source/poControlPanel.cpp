//
//  UIWindow.cpp
//  controlUI
//
//  Created by Tamar Ziv on 1/30/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "poControlPanel.h"

#define MARGIN 10

poControlPanel::poControlPanel( string _label, string _filename )
{
    
    label = _label;
    
    readSettings();
    
    if ( settings.has(label) )
        pos = settings.getPoint(label);
    else
        pos = poPoint(0,0,0);
    
    float padding = 10;
    filename = _filename;    
    
    addEvent(PO_MOUSE_UP_EVENT, this);
	
    bar = new poRectShape(240,20);
    bar->position = pos;
    bar->fillColor = poColor( 1,1,1,.2 );
    bar->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	bar->addEvent(PO_MOUSE_DRAG_INSIDE_EVENT, this);
    addChild( bar );

    save = new poRectShape(50,20);
    save->fillColor = poColor( 1,1,1,.2 );
    save->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
    save->position = poPoint( bar->getWidth()-save->getWidth() ,0 );
    bar->addChild( save );
    
    hide = new poRectShape(50,20);
    hide->fillColor = poColor( 1,1,1,.2 );
    hide->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
    hide->position = poPoint( bar->getWidth()-(save->getWidth()*2 + 2) ,0 );
    bar->addChild( hide );
    
    container = new poObject();
    container->position = poPoint( 0,bar->getBounds().height + padding/10 );
    bar->addChild( container );
    
    box = new poRectShape( bar->getWidth(),400 );
    box->fillColor = poColor( 1,1,1,.2 );
    bar->addChild( box );
    
    float sWidth = container->getBounds().width - padding;
    
    poTextBox* shapeData = new poTextBox( bar->getWidth(), bar->getHeight() );
	shapeData->textColor = poColor::white;
    shapeData->position.set( padding,3,0 );
	shapeData->setFont( poGetFont ("Courier") );
    shapeData->setTextSize(12);
    shapeData->setText( label );
	shapeData->doLayout();  
    bar->addChild( shapeData );
    
    poTextBox* saveText = new poTextBox( save->getWidth(), save->getHeight() );
	saveText->textColor = poColor::white;
    saveText->position.set( padding,3,0 );
	saveText->setFont( poGetFont ("Courier") );
    saveText->setTextSize(12);
    saveText->setText( "save" );
	saveText->doLayout();  
    save->addChild( saveText );
    
    hideText = new poTextBox( save->getWidth(), save->getHeight() );
	hideText->textColor = poColor::white;
    hideText->position.set( padding,3,0 );
	hideText->setFont( poGetFont ("Courier") );
    hideText->setTextSize(12);
    hideText->setText( "hide" );
	hideText->doLayout();  
    hide->addChild( hideText );
    
    bar->moveChildToBack(box);
}


void poControlPanel::addKnob( string _ID, float min, float max, poObject* obj  )
{
    string prop = _ID;
    float propVal; 
    
    if ( settings.has( prop )) 
    {
        propVal = settings.getFloat(prop);
    } else 
    {
        propVal = 0.0f;
        settings.set( prop, propVal );
    }
    poKnob* K = new poKnob( _ID, propVal, min, max, obj );
    K->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( K );
}

void poControlPanel::addSliderF( string _ID, float min, float max, poObject* obj  )
{
    string prop = _ID;
    float propVal; 
    
    if ( settings.has( prop )) 
    {
        propVal = settings.getFloat(prop);
    } else 
    {
        propVal = 0.0f;
        settings.set( prop, propVal );
    }
    poSliderF* S = new poSliderF( _ID, propVal, min, max, "" , obj );
    S->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( S );
}



void poControlPanel::addSliderI( string _ID, int min, int max, poObject* obj  )
{
    
    string prop = _ID;
    int propVal;
    
    if ( settings.has(prop) ) {
        propVal = settings.getInt(prop);
    } else {
        propVal = 0;
        settings.set( prop, propVal );
    }
    
//    cout << "val is set to: " << propVal << endl;
    
    poSliderI* S = new poSliderI( _ID, propVal, min, max, obj );
    S->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( S );
}

void poControlPanel::addToggle( string _ID, poObject* obj )
{
    
    string prop = _ID;
    bool propVal;
    
    if ( settings.has( prop)) {
        propVal = settings.getBool(prop);
    } else 
    {
        propVal = true;
        settings.set( prop, propVal );
    }
    
    poToggleBox* T = new poToggleBox( _ID, propVal, obj );
    T->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( T );
    
}

void poControlPanel::addMessage( string _ID, poObject* obj )
{
    string prop = _ID;    
    poMessage* M = new poMessage( _ID, obj );
    M->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( M );
}

void poControlPanel::addPointSlider( string _ID, poPoint min, poPoint max ,poObject* obj )
{
    
    string prop = _ID;
    poPoint propVal;
    
    if ( settings.has( prop)) {
        propVal = settings.getPoint(prop);
    } else 
    {
        propVal = poPoint(0,0,0);
        settings.set( prop, propVal );
    }
    
    poPointSlider* P = new poPointSlider( _ID, propVal, min, max, obj );
    P->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( P );
}

void poControlPanel::addColorSlider( string _ID, bool RGB ,poObject* obj )
{
    
    string prop = _ID;
    poColor propVal;
    
    if ( settings.has(prop) ) 
    {
        propVal = settings.getColor(prop);
    } 
    else 
    {
        propVal = poColor(0,0,0,0);
        settings.set( prop, propVal );
    }
    
    poColorSlider* P = new poColorSlider( _ID, propVal, RGB, obj );
    P->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( P );

}

void poControlPanel::addRadio( string _ID, vector<string> names, poObject* obj )
{
    string prop = _ID;
    int propVal;
    
    if ( settings.has(prop) ) {
        propVal = settings.getInt(prop);
    } 
    else
    {
        propVal = 0;
        settings.set( prop,propVal );
    }
    
    poRadio* R = new poRadio( _ID, propVal, names, obj );
    R->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( R );
}


void poControlPanel::addInputTextBox( string _ID,poObject* obj )
{
    
    string prop = _ID;
    string propVal;
    
    if ( settings.has( prop )) {
                
        string temp = settings.getString( prop );
        temp = temp.erase(temp.find_last_not_of("'")+1);
        temp = temp.erase(0,temp.find_first_not_of("'"));
        propVal = temp;

    } else 
    {
        propVal = "something";
        settings.set( prop, propVal );
    }
    
    poInputTextBox* T = new poInputTextBox( _ID, propVal, obj );
    T->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( T );
}

void poControlPanel::autoResize()
{
    if ( box->getHeight() > 300) {
        box->reshape( box->getWidth(),(container->getHeight()+50) );
    }
}

void poControlPanel::eventHandler(poEvent *event) 
{
    if ( event->source == bar ) 
    {
        if ( event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
            isDragged = true;
            dragOffset = event->localPosition;
            dragOffset += bar->offset;
        }
        
        else if ( event->type == PO_MOUSE_DRAG_INSIDE_EVENT ) {
            bar->position = event->globalPosition - dragOffset;
            settings.set( label , bar->position );
        }
    }
    
    if ( event->source == save ) 
    {
        if ( event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
            saveSettings();
            save->fillColor = poColor( 1,0,0,.2 );
        }
    }
	
    if ( event->source == hide ) 
    {
        if ( event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
            box->visible = ! box->visible;
            container->visible = ! container->visible;
            hide->fillColor = poColor( 1,0,0,.2 );
            
            if (!container->visible) {
                hideText->setText( "show" );
                hideText->doLayout(); 
            } else {
                
                hideText->setText( "hide" );
                hideText->doLayout(); 
            }
        }
    }
	
	if ( event->type == PO_MOUSE_UP_EVENT ) {
		if(isDragged) isDragged = false;
		if(save->fillColor != poColor( 1,1,1,.2 )) save->fillColor = poColor( 1,1,1,.2 );
		if(hide->fillColor != poColor( 1,1,1,.2 )) hide->fillColor = poColor( 1,1,1,.2 );
	}
}

void poControlPanel::saveSettings() {    
    
    settings.write( label+".xml" );
}

void poControlPanel::readSettings() 
{    
    FILE* p = fopen((label+".xml").c_str() , "r");
    if(p) settings.read( label+".xml" );
}
bool poControlPanel::getBool( string s ) {    
    poControl* C = (poControl*) container->getChild(s);
//    cout << "prop name is: " << s << endl;
//    cout << "container children is: " << container->getNumChildren() << endl;

    if ( C != NULL) 
    {
        settings.set( s, C->valB );
        return C->valB;
    }
}

int poControlPanel::getInt( string s ) {
    poControl* C = (poControl*) container->getChild(s);
    if ( C != NULL) 
    {
        settings.set( s, C->valI );
        return C->valI;
    }
}

float poControlPanel::getFloat( string s ) {
    poControl* C = (poControl*) container->getChild(s);
    if ( C != NULL) 
    {
        settings.set( s, C->valF );
        return C->valF;
    }
}

string poControlPanel::getString( string s ) {
    poControl* C = (poControl*) container->getChild(s);
    if ( C != NULL) 
    {
        
        string temp = C->valS;
        temp = temp.erase(temp.find_last_not_of("'")+1);
        temp = temp.erase(0,temp.find_first_not_of("'"));
        settings.set( s, temp );
//        cout << temp << endl;
        return temp;
    }
}

poPoint poControlPanel::getPoint( string s ) {
	poControl* C = (poControl*) container->getChild(s);
    if ( C != NULL) 
    {
        settings.set( s, C->valP );
        return C->valP;
    }
}

poColor poControlPanel::getColor( string s) {
    poControl* C = (poControl*) container->getChild(s);
    if ( C != NULL) 
    {
        settings.set( s, C->valC );
        return C->valC;
    }
}





