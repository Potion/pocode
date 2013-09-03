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

namespace po {
	namespace control {
		
		ControlPanel::ControlPanel( string _label, po::Color _color, int textSize, string _path ) {
			label = _label;
			path = _path;
			
			readSettings();
			
			if ( settings.has(label) )
				pos = settings.getPoint(label);
			else
				pos = po::Point(0,0,0);
			
			bool showHide;
			if ( settings.has("showHide") )
				showHide = settings.getInt("showHide");
			else
				showHide = true;
			
			float padding = 10;
			
			panelColor = _color;
			
		//    addEvent(po::MOUSE_MOVE_EVENT, this);
			addEvent(po::MOUSE_UP_EVENT, this);
			addEvent(po::TOUCH_ENDED_EVENT, this);
			
			bar = new po::RectShape(240,20);
			bar->position = pos;
			bar->fillColor = panelColor;
			bar->addEvent(po::MOUSE_DOWN_INSIDE_EVENT, this);
			bar->addEvent(po::MOUSE_DRAG_INSIDE_EVENT, this);
			bar->addEvent(po::TOUCH_BEGAN_INSIDE_EVENT, this);
			bar->addEvent(po::TOUCH_MOVED_EVENT, this);
			addChild( bar );

			save = new po::RectShape(50,20);
			save->fillColor = panelColor;
			save->addEvent(po::MOUSE_DOWN_INSIDE_EVENT, this);
			save->addEvent(po::TOUCH_BEGAN_INSIDE_EVENT, this);
			save->position = po::Point( bar->getWidth()-save->getWidth() ,0 );
			bar->addChild( save );
			
			hide = new po::RectShape(50,20);
			hide->fillColor = panelColor;
			hide->addEvent(po::MOUSE_DOWN_INSIDE_EVENT, this);
			hide->addEvent(po::TOUCH_BEGAN_INSIDE_EVENT, this);
			hide->position = po::Point( bar->getWidth()-(save->getWidth()*2 + 2) ,0 );
			bar->addChild( hide );
			
			container = new po::Object();
			container->position = po::Point( MARGIN, bar->getBounds().height + MARGIN );
			container->visible = showHide;
			bar->addChild( container );
			
			containerLayout = new po::Layout(ORIENTATION_VERTICAL_DOWN);
			containerLayout->setSpacing(MARGIN);
			containerLayout->setRefreshEveryFrame(false);
			container->addModifier(containerLayout);
			
			box = new po::RectShape( bar->getWidth(),400 );
			box->fillColor = panelColor;
			box->visible = showHide;
			bar->addChild( box );
			
			float sWidth = container->getBounds().width - padding;
			
			po::TextBox* shapeData = new po::TextBox( bar->getWidth(), bar->getHeight() );
			shapeData->textColor = po::Color::white;
			shapeData->position.set( padding,3,0 );
			shapeData->setTextSize(12);
			shapeData->setText( label );
			shapeData->doLayout();  
			bar->addChild( shapeData );
			
			po::TextBox* saveText = new po::TextBox( save->getWidth(), save->getHeight() );
			saveText->textColor = po::Color::white;
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
			
			hideText = new po::TextBox( save->getWidth(), save->getHeight() );
			hideText->textColor = po::Color::white;
			hideText->position.set( padding,3,0 );
			hideText->setTextSize(12);
			hideText->setText( status );
			hideText->doLayout();
			hide->addChild( hideText );
			
			bar->moveChildToBack(box);
			
			isResized = false;
			isDragged = false;
		}


		void ControlPanel::addKnob( string _ID, float min, float max, po::Object* obj ) {
			string prop = _ID;
			float propVal; 
			
			if ( settings.has( prop )) {
				propVal = settings.getFloat(prop);
			}
			else {
				propVal = 0.0f;
				settings.set( prop, propVal );
			}
			Knob* K = new Knob( _ID, propVal, min, max, obj );
		//    K->position = poPoint( MARGIN,container->getHeight()+MARGIN );
			container->addChild( K );
		}

		void ControlPanel::addSliderF( string _ID, float min, float max, po::Object* obj  ) {
			string prop = _ID;
			float propVal; 
			
			if ( settings.has( prop )) {
				propVal = settings.getFloat(prop);
			}
			else {
				propVal = 0.0f;
				settings.set( prop, propVal );
			}
			SliderF* S = new SliderF( _ID, propVal, min, max, "" , obj );
		//    S->position = poPoint( MARGIN,container->getHeight()+MARGIN );
			container->addChild( S );
		}

		void ControlPanel::addSliderI( string _ID, int min, int max, po::Object* obj  ) {
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
			
			SliderI* S = new SliderI( _ID, propVal, min, max, obj );
		//    S->position = poPoint( MARGIN,container->getHeight()+MARGIN );
			container->addChild( S );
		}

		void ControlPanel::addToggle( string _ID, po::Object* obj ) {
			string prop = _ID;
			bool propVal;
			
			if ( settings.has( prop)) {
				propVal = settings.getBool(prop);
			}
			else {
				propVal = true;
				settings.set( prop, propVal );
			}
			
			ToggleBox* T = new ToggleBox( _ID, propVal, obj );
		//    T->position = poPoint( MARGIN,container->getHeight()+MARGIN );
			container->addChild( T );
		}

		void ControlPanel::addButton( string _ID, po::Object* obj ) {
			string prop = _ID;
			Button* M = new Button( _ID, obj );
		//    M->position = poPoint( MARGIN,container->getHeight()+MARGIN );
			container->addChild( M );
		}

		void ControlPanel::addPointSlider( string _ID, po::Point min, po::Point max ,po::Object* obj ) {
			string prop = _ID;
			po::Point propVal;
			
			if ( settings.has( prop)) {
				propVal = settings.getPoint(prop);
			} else 
			{
				propVal = po::Point(0,0,0);
				settings.set( prop, propVal );
			}
			
			PointSlider* P = new PointSlider( _ID, propVal, min, max, obj );
		//    P->position = poPoint( MARGIN,container->getHeight()+MARGIN );
			container->addChild( P );
		}

		void ControlPanel::addColorSlider( string _ID, bool RGB ,po::Object* obj ) {
			string prop = _ID;
			po::Color propVal;
			
			if ( settings.has(prop) ) {
				propVal = settings.getColor(prop);
			}
			else {
				propVal = po::Color(0,0,0,0);
				settings.set( prop, propVal );
			}
			
			ColorSlider* P = new ColorSlider( _ID, propVal, RGB, obj );
		//    P->position = poPoint( MARGIN,container->getHeight()+MARGIN );
			container->addChild( P );

		}

		void ControlPanel::addRadio( string _ID, vector<string> names, po::Object* obj ) {
			string prop = _ID;
			int propVal;
			
			if ( settings.has(prop) ) {
				propVal = settings.getInt(prop);
			} 
			else {
				propVal = 0;
				settings.set( prop,propVal );
			}
			
			Radio* R = new Radio( _ID, propVal, names, obj );
		//    R->position = poPoint( MARGIN,container->getHeight()+MARGIN );
			container->addChild( R );
		}


		void ControlPanel::addInputTextBox( string _ID,po::Object* obj ) {
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
			
			InputTextBox* T = new InputTextBox( _ID, propVal, obj );
		//    T->position = poPoint( MARGIN,container->getHeight()+MARGIN );
			container->addChild( T );
		}

		void ControlPanel::update() {
			if(!isResized) autoResize();
		}

		void ControlPanel::autoResize()
		{
			containerLayout->refresh();
			
			if ( container->getBounds().height > 300) {
				box->reshape( box->getWidth(),(container->position.y + container->getHeight() + MARGIN) );
			}
		}

		void ControlPanel::eventHandler(po::Event *event) {
			if ( event->source == save ) {
				if ( event->type == po::MOUSE_DOWN_INSIDE_EVENT || event->type == po::TOUCH_BEGAN_INSIDE_EVENT ) {
					saveSettings();
					save->fillColor = po::Color( 1,0,0,.2 );
				}
			}
			else if ( event->source == hide ) {
				if ( event->type == po::MOUSE_DOWN_INSIDE_EVENT || event->type == po::TOUCH_BEGAN_INSIDE_EVENT ) {
					box->visible = ! box->visible;
					container->visible = ! container->visible;
					hide->fillColor = po::Color( 1,0,0,.2 );
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
				if ( event->type == po::MOUSE_DOWN_INSIDE_EVENT || event->type == po::TOUCH_BEGAN_INSIDE_EVENT ) {
					isDragged = true;
					prevPoint = event->globalPosition;
				}
				
				else if ( event->type == po::MOUSE_DRAG_INSIDE_EVENT || event->type == po::TOUCH_MOVED_EVENT ) {
					if(isDragged) {
						po::Rect winBounds(0,0,1024,768); // TODO: replace  //getWindowWidth(),getWindowHeight());
						if(!winBounds.contains(event->globalPosition))
							return;
						bar->position += event->globalPosition - prevPoint;
						settings.set( label , bar->position );
						prevPoint = event->globalPosition;
					}
				}
			}
			
			if ( event->type == po::MOUSE_UP_EVENT || event->type == po::TOUCH_ENDED_EVENT ) {
				isDragged = false;
				if(save->fillColor != po::Color( 1,1,1,.2 )) save->fillColor = po::Color( 1,1,1,.2 );
				if(hide->fillColor != po::Color( 1,1,1,.2 )) hide->fillColor = po::Color( 1,1,1,.2 );
			}
		}

		void ControlPanel::messageHandler(const std::string &msg, const po::Dictionary& dict) {
			
			if( msg == "update_settings" ) {
				if( dict.has("control") ) {
					Control* C = (Control*) dict.getPtr("control");
					if ( C != NULL) {
						bool isChild = false;
						po::Object* child = (po::Object*) dict.getPtr("control");
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

		void ControlPanel::saveSettings() {
			char filepath[256];
			sprintf(filepath, "%s%s.xml", path.c_str(), label.c_str());
			settings.write( filepath );
		}

		void ControlPanel::readSettings() {
			char filepath[256];
			sprintf(filepath, "%s%s.xml", path.c_str(), label.c_str());
			FILE* p = fopen(filepath , "r");
			if(p) settings.read( filepath );
		}

		void ControlPanel::setSavePath(string s) {
			path = s;
		}

		bool ControlPanel::getBool( string s ) {
			Control* C = (Control*) container->getChild(s);
			if(C == NULL) {
				std::cout << "Control '" << s << "' does not exist, can't get its value..." << std::endl;
				return 1;
			}
			else if( C->type != TYPE_BOOL )
				std::cout << "Control '" << s << "' does not store a boolean..." << std::endl;
			
			return C->valB;
		}

		int ControlPanel::getInt( string s ) {
			Control* C = (Control*) container->getChild(s);
			if(C == NULL) {
				std::cout << "Control '" << s << "' does not exist, can't get its value..." << std::endl;
				return 0;
			}
			else if( C->type != TYPE_INT )
				std::cout << "Control '" << s << "' does not store an integer..." << std::endl;
			
			return C->valI;
		}

		float ControlPanel::getFloat( string s ) {
			Control* C = (Control*) container->getChild(s);
			if(C == NULL) {
				std::cout << "Control '" << s << "' does not exist, can't get its value..." << std::endl;
				return 0.f;
			}
			else if( C->type != TYPE_FLOAT )
				std::cout << "Control '" << s << "' does not store a float..." << std::endl;
			
			return C->valF;
		}

		string ControlPanel::getString( string s ) {
			Control* C = (Control*) container->getChild(s);
			if(C == NULL) {
				std::cout << "Control '" << s << "' does not exist, can't get its value..." << std::endl;
				return "";
			}
			else if( C->type != TYPE_TEXT )
				std::cout << "Control '" << s << "' does not store a string..." << std::endl;
			
			return C->valS;
		}

		po::Point ControlPanel::getPoint( string s ) {
			Control* C = (Control*) container->getChild(s);
			if(C == NULL) {
				std::cout << "Control '" << s << "' does not exist, can't get its value..." << std::endl;
				return po::Point(0,0,0);
			}
			else if( C->type != TYPE_POINT )
				std::cout << "Control '" << s << "' does not store a poPoint..." << std::endl;
			
			return C->valP;
		}

		po::Color ControlPanel::getColor( string s ) {
			Control* C = (Control*) container->getChild(s);
			if(C == NULL) {
				std::cout << "Control '" << s << "' does not exist, can't get its value..." << std::endl;
				return po::Color(0,0,0,0);
			}
			else if( C->type != TYPE_COLOR )
				std::cout << "Control '" << s << "' does not store a po::Color..." << std::endl;
			
			return C->valC;
		}

		void ControlPanel::setString( string s, string setString ) {
			Control* controlChild = (Control*) container->getChild(s);
			if(controlChild == NULL) {
				std::cout << "Control '" << s << "' does not exist, can't set its value..." << std::endl;
				return;
			}
			else if( controlChild->type != TYPE_TEXT ) {
				std::cout << "Control '" << s << "' is not a text input, can't setString..." << std::endl;
				return;
			}
			InputTextBox* C = (InputTextBox*) container->getChild(s);
			
			C->valS = setString;
			
			C->shapeData->setText( C->valS );
			C->shapeData->doLayout();
			C->resize();
			
			po::Dictionary D;
			D.set("value", setString);
			
			if ( C->listener == NULL ) {
			}
			else
				C->listener->messageHandler( C->ID, D);
			
			D.set("control", C);
			D.set("valueType", "string");
			messageHandler("update_settings", D);
		}

		void ControlPanel::setBool(string s, bool val) {
			ToggleBox* C = (ToggleBox*) container->getChild(s);
			C->valB = val;
			C->setValue(val);
		}

		void ControlPanel::setInt(string s, int val) {
			SliderI* C = (SliderI*) container->getChild(s);
			C->valI = val;
			C->setValue(val);
		}

		void ControlPanel::setFloat(string s, float val) {
			SliderF* C = (SliderF*) container->getChild(s);
			C->valF = val;
			C->setValue(val);
		}

		void ControlPanel::setPoint( string s, po::Point val ) {
			Control* C = (Control*) container->getChild(s);
			C->valP = val;
		}
	}
}