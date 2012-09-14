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

#include <iostream>
#include "poControlPanel.h"

#define MARGIN 10
#define SPACING 5

poControlPanel::poControlPanel( string _label, poColor _color, int textSize ) {
    label = _label;
    
    readSettings();
    
    if ( settings.has(label) )
        pos = settings.getPoint(label);
    else
        pos = poPoint(0,0,0);
    
    bool showHide;
    if ( settings.has("showHide") )
        showHide = settings.getInt("showHide");
    else
        showHide = true;
    
    float padding = 10;
	
	panelColor = _color;
    poControl::controlTextSize = textSize;
    
    addEvent(PO_MOUSE_UP_EVENT, this);
	
    bar = new poRectShape(240,20);
    bar->position = pos;
    bar->fillColor = panelColor;
    bar->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	bar->addEvent(PO_MOUSE_DRAG_INSIDE_EVENT, this);
    addChild( bar );

    save = new poRectShape(50,20);
    save->fillColor = panelColor;
    save->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
    save->position = poPoint( bar->getWidth()-save->getWidth() ,0 );
    bar->addChild( save );
    
    hide = new poRectShape(50,20);
    hide->fillColor = panelColor;
    hide->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
    hide->position = poPoint( bar->getWidth()-(save->getWidth()*2 + 2) ,0 );
    bar->addChild( hide );
	
    container = new poObject();
    container->position = poPoint( MARGIN, bar->getBounds().height + MARGIN );
    container->visible = showHide;
    bar->addChild( container );
	
	containerLayout = new poLayout(PO_VERTICAL_DOWN);
	containerLayout->setSpacing(MARGIN);
	containerLayout->setRefreshEveryFrame(false);
	container->addModifier(containerLayout);
    
    box = new poRectShape( bar->getWidth(),400 );
    box->fillColor = panelColor;
    box->visible = showHide;
    bar->addChild( box );
    
    float sWidth = container->getBounds().width - padding;
    
    poTextBox* shapeData = new poTextBox( bar->getWidth(), bar->getHeight() );
	shapeData->textColor = poColor::white;
    shapeData->position.set( padding,3,0 );
    shapeData->setTextSize(12);
    shapeData->setText( label );
	shapeData->doLayout();  
    bar->addChild( shapeData );
    
    poTextBox* saveText = new poTextBox( save->getWidth(), save->getHeight() );
	saveText->textColor = poColor::white;
    saveText->position.set( padding,3,0 );
    saveText->setTextSize(12);
    saveText->setText( "save" );
	saveText->doLayout();  
    save->addChild( saveText );
    
    string status;
    if ( showHide )
        status = "hide";
    else
        status = "show";
    
    hideText = new poTextBox( save->getWidth(), save->getHeight() );
	hideText->textColor = poColor::white;
    hideText->position.set( padding,3,0 );
    hideText->setTextSize(12);
    hideText->setText( status );
	hideText->doLayout();
    hide->addChild( hideText );
    
    bar->moveChildToBack(box);
	
	isResized = false;
	isDragged = false;
}


void poControlPanel::addKnob( string _ID, float min, float max, poObject* obj ) {
    string prop = _ID;
    float propVal; 
    
    if ( settings.has( prop )) {
        propVal = settings.getFloat(prop);
    }
	else {
        propVal = 0.0f;
        settings.set( prop, propVal );
    }
    poKnob* K = new poKnob( _ID, propVal, min, max, obj );
//    K->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( K );
}

void poControlPanel::addSliderF( string _ID, float min, float max, poObject* obj  ) {
    string prop = _ID;
    float propVal; 
    
    if ( settings.has( prop )) {
        propVal = settings.getFloat(prop);
    }
	else {
        propVal = 0.0f;
        settings.set( prop, propVal );
    }
    poSliderF* S = new poSliderF( _ID, propVal, min, max, "" , obj );
//    S->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( S );
}

void poControlPanel::addSliderI( string _ID, int min, int max, poObject* obj  ) {
    string prop = _ID;
    int propVal;
    
    if ( settings.has(prop) ) {
        propVal = settings.getInt(prop);
    }
	else {
        propVal = 0;
        settings.set( prop, propVal );
    }
    
//    cout << "val is set to: " << propVal << endl;
    
    poSliderI* S = new poSliderI( _ID, propVal, min, max, obj );
//    S->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( S );
}

void poControlPanel::addToggle( string _ID, poObject* obj ) {
    string prop = _ID;
    bool propVal;
    
    if ( settings.has( prop)) {
        propVal = settings.getBool(prop);
    }
	else {
        propVal = true;
        settings.set( prop, propVal );
    }
    
    poToggleBox* T = new poToggleBox( _ID, propVal, obj );
//    T->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( T );
}

void poControlPanel::addButton( string _ID, poObject* obj ) {
    string prop = _ID;
    poButton* M = new poButton( _ID, obj );
//    M->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( M );
}

void poControlPanel::addPointSlider( string _ID, poPoint min, poPoint max ,poObject* obj ) {
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
//    P->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( P );
}

void poControlPanel::addColorSlider( string _ID, bool RGB ,poObject* obj ) {
    string prop = _ID;
    poColor propVal;
    
    if ( settings.has(prop) ) {
        propVal = settings.getColor(prop);
    }
    else {
        propVal = poColor(0,0,0,0);
        settings.set( prop, propVal );
    }
    
    poColorSlider* P = new poColorSlider( _ID, propVal, RGB, obj );
//    P->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( P );

}

void poControlPanel::addRadio( string _ID, vector<string> names, poObject* obj ) {
    string prop = _ID;
    int propVal;
    
    if ( settings.has(prop) ) {
        propVal = settings.getInt(prop);
    } 
    else {
        propVal = 0;
        settings.set( prop,propVal );
    }
    
    poRadio* R = new poRadio( _ID, propVal, names, obj );
//    R->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( R );
}


void poControlPanel::addInputTextBox( string _ID,poObject* obj ) {
    string prop = _ID;
    string propVal;
    
    if ( settings.has( prop ) ) {
        string temp = settings.getString( prop );
        temp = temp.erase(temp.find_last_not_of("'")+1);
        temp = temp.erase(0,temp.find_first_not_of("'"));
        propVal = temp;

    }
	else {
        propVal = "something";
        settings.set( prop, propVal );
    }
    
    poInputTextBox* T = new poInputTextBox( _ID, propVal, obj );
//    T->position = poPoint( MARGIN,container->getHeight()+MARGIN );
    container->addChild( T );
}

void poControlPanel::update() {
	if(!isResized) autoResize();
}

void poControlPanel::autoResize()
{
	containerLayout->refresh();
	
    if ( container->getBounds().height > 300) {
        box->reshape( box->getWidth(),(container->position.y + container->getHeight() + MARGIN) );
    }
}

void poControlPanel::eventHandler(poEvent *event) {
    if ( event->source == save ) {
        if ( event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
            saveSettings();
            save->fillColor = poColor( 1,0,0,.2 );
        }
    }
	else if ( event->source == hide ) {
        if ( event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
            box->visible = ! box->visible;
            container->visible = ! container->visible;
            hide->fillColor = poColor( 1,0,0,.2 );
            settings.set( "showHide" , container->visible );
            
            if (!container->visible) {
                hideText->setText( "show" );
                hideText->doLayout(); 
            } else {
                
                hideText->setText( "hide" );
                hideText->doLayout(); 
            }
        }
    }
	else if ( event->source == bar ) {
		if ( event->type == PO_MOUSE_DOWN_INSIDE_EVENT ) {
			isDragged = true;
			dragOffset = event->localPosition;
			dragOffset += bar->offset;
		}
		
		else if ( event->type == PO_MOUSE_DRAG_INSIDE_EVENT && isDragged ) {
			poRect winBounds(0,0,po::getWindowWidth(),po::getWindowHeight());
			if(!winBounds.contains(event->globalPosition))
				return;
			bar->position = event->globalPosition - dragOffset;
			settings.set( label , bar->position );
		}
	}
	
	if ( event->type == PO_MOUSE_UP_EVENT ) {
		if(isDragged) isDragged = false;
		if(save->fillColor != poColor( 1,1,1,.2 )) save->fillColor = poColor( 1,1,1,.2 );
		if(hide->fillColor != poColor( 1,1,1,.2 )) hide->fillColor = poColor( 1,1,1,.2 );
	}
}

void poControlPanel::messageHandler(const std::string &msg, const poDictionary& dict) {
	
	if( msg == "update_settings" ) {
		if( dict.has("control") ) {
			poControl* C = (poControl*) dict.getPtr("control");
			if ( C != NULL) {
				bool isChild = false;
				poObject* child = (poObject*) dict.getPtr("control");
				for(int i=0; i < container->getNumChildren(); i++) {
					if(child == container->getChild(i))
						isChild = true;
				}
				if(!isChild) return;
				
				std::string valueType = dict.getString("valueType");
				if( valueType == "bool" ) {
					settings.set( C->ID, C->valB );
				}
				else if( valueType == "int" ) {
					settings.set( C->ID, C->valI );
				}
				else if( valueType == "float" ) {
					settings.set( C->ID, C->valF );
				}
				else if( valueType == "string" ) {
					string temp = C->valS;
					temp = temp.erase(temp.find_last_not_of("'")+1);
					temp = temp.erase(0,temp.find_first_not_of("'"));
					settings.set( C->ID, temp );
				}
				else if( valueType == "point" ) {
					settings.set( C->ID, C->valP );
				}
				else if( valueType == "color" ) {
					settings.set( C->ID, C->valC );
				}
					
				autoResize();
			}
		}
	}
}

void poControlPanel::saveSettings() {
    settings.write( label+".xml" );
}

void poControlPanel::readSettings() {    
    FILE* p = fopen((label+".xml").c_str() , "r");
    if(p) settings.read( label+".xml" );
}

bool poControlPanel::getBool( string s ) {
    poControl* C = (poControl*) container->getChild(s);
    if(C == NULL) {
        std::cout << "poControl '" << s << "' does not exist, can't get its value..." << std::endl;
        return 1;
    }
    else if( C->type != PO_CONTROL_TYPE_BOOL )
        std::cout << "poControl '" << s << "' does not store a boolean..." << std::endl;
    
    return C->valB;
}

int poControlPanel::getInt( string s ) {
    poControl* C = (poControl*) container->getChild(s);
    if(C == NULL) {
        std::cout << "poControl '" << s << "' does not exist, can't get its value..." << std::endl;
        return 0;
    }
    else if( C->type != PO_CONTROL_TYPE_INT )
        std::cout << "poControl '" << s << "' does not store an integer..." << std::endl;
    
    return C->valI;
}

float poControlPanel::getFloat( string s ) {
    poControl* C = (poControl*) container->getChild(s);
    if(C == NULL) {
        std::cout << "poControl '" << s << "' does not exist, can't get its value..." << std::endl;
        return 0.f;
    }
    else if( C->type != PO_CONTROL_TYPE_FLOAT )
        std::cout << "poControl '" << s << "' does not store a float..." << std::endl;
    
    return C->valF;
}

string poControlPanel::getString( string s ) {
    poControl* C = (poControl*) container->getChild(s);
    if(C == NULL) {
        std::cout << "poControl '" << s << "' does not exist, can't get its value..." << std::endl;
        return "";
    }
    else if( C->type != PO_CONTROL_TYPE_TEXT )
        std::cout << "poControl '" << s << "' does not store a string..." << std::endl;
    
    return C->valS;
}

poPoint poControlPanel::getPoint( string s ) {
    poControl* C = (poControl*) container->getChild(s);
    if(C == NULL) {
        std::cout << "poControl '" << s << "' does not exist, can't get its value..." << std::endl;
        return poPoint(0,0,0);
    }
    else if( C->type != PO_CONTROL_TYPE_POINT )
        std::cout << "poControl '" << s << "' does not store a poPoint..." << std::endl;
    
    return C->valP;
}

poColor poControlPanel::getColor( string s ) {
    poControl* C = (poControl*) container->getChild(s);
    if(C == NULL) {
        std::cout << "poControl '" << s << "' does not exist, can't get its value..." << std::endl;
        return poColor(0,0,0,0);
    }
    else if( C->type != PO_CONTROL_TYPE_COLOR )
        std::cout << "poControl '" << s << "' does not store a poColor..." << std::endl;
    
    return C->valC;
}

void poControlPanel::setString( string s, string setString ) {
    poControl* controlChild = (poControl*) container->getChild(s);
    if(controlChild == NULL) {
        std::cout << "poControl '" << s << "' does not exist, can't set its value..." << std::endl;
        return;
    }
    else if( controlChild->type != PO_CONTROL_TYPE_TEXT ) {
        std::cout << "poControl '" << s << "' is not a text input, can't setString..." << std::endl;
        return;
    }
    poInputTextBox* C = (poInputTextBox*) container->getChild(s);
    
    C->valS = setString;
    
    C->shapeData->setText( C->valS );
    C->shapeData->doLayout();
    C->resize();
    
    poDictionary D;
    D.set("value", setString);
    
    if ( C->listener == NULL ) {
    }
    else
        C->listener->messageHandler( C->ID, D);
    
    D.set("control", C);
    D.set("valueType", "string");
    messageHandler("update_settings", D);
}
