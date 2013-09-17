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
#include "poControl.h"
#include "poHelpers.h"
#include "poApplication.h"

//int po::control::PO_CONTROL_TEXT_SIZE = 12;

namespace po {
	namespace UI {
		
		Control::Control( string _ID, po::Object* _listener ) {
			ID = _ID;
			listener = _listener;
			bDragging = false;
		}

		// -----------------------------------------------------------------------------------
		// =============================== Slider Float ======================================
		#pragma mark SliderF
		
		//------------------------------------------------------------------------
		SliderF::SliderF( string _ID, float init, float _min, float _max, string _valName ,po::Object* _listener ) : Control( _ID, _listener ) {
			
			type = TYPE_FLOAT;
			
			this->name = ID;
			min = _min;
			max = _max;
			
			valF = po::Clamp(min, max, init);
			valName = _valName;

			char valString [256];
			sprintf( valString, "%s%.2f", valName.c_str(), valF );
			
			addEvent( po::MOUSE_DOWN_EVENT, this );
			addEvent( po::TOUCH_BEGAN_EVENT, this );
			
			sliderShape = new po::RectShape( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			sliderShape->fillColor = po::Color(1,1,1,0.2);
			sliderKnob = new po::RectShape( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			sliderKnob->fillColor = po::Color(1,1,1,0.2);
			sliderKnob->position.x = po::Mapf(min, max, valF, 0, PO_CONTROL_SLIDER_WIDTH);
			sliderKnob->addEvent( po::MOUSE_DRAG_INSIDE_EVENT, this );
			sliderKnob->addEvent( po::MOUSE_DOWN_INSIDE_EVENT, this );
			sliderKnob->addEvent( po::MOUSE_UP_EVENT, this);
			sliderKnob->addEvent( po::TOUCH_BEGAN_INSIDE_EVENT, this );
			sliderKnob->addEvent( po::TOUCH_MOVED_EVENT, this);
			sliderKnob->addEvent( po::TOUCH_ENDED_EVENT, this);
			
			addChild( sliderShape );
			sliderShape->addChild( sliderKnob );
			
			shapeData = new po::TextBox( PO_CONTROL_SLIDER_WIDTH,PO_CONTROL_SLIDER_HEIGHT );
			shapeData->textColor = po::Color::white;
			shapeData->position.set(0,0,0);
			shapeData->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeData->setText(valString);
			shapeData->doLayout();  
			addChild( shapeData );
			
			shapeLabel = new po::TextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			shapeLabel->textColor = po::Color::white;
			shapeLabel->setLeading(.6);
			shapeLabel->setTextAlignment(po::ALIGN_CENTER_LEFT);
			shapeLabel->position.set( PO_CONTROL_LABEL_X_POS,0,0);
			shapeLabel->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeLabel->setText( ID );
			shapeLabel->doLayout();
			addChild( shapeLabel );
		}

		void SliderF::eventHandler(po::Event *event) {
			
			if (event->type == po::MOUSE_DOWN_INSIDE_EVENT || event->type == po::TOUCH_BEGAN_INSIDE_EVENT ) {
				
				dragOffset = event->localPosition;
				bDragging = true;
			}

			if (event->type == po::MOUSE_DRAG_INSIDE_EVENT || event->type == po::TOUCH_MOVED_EVENT ) {
				
				if( bDragging ) {
					po::Point sOffset = sliderShape->localToGlobal( po::Point(0,0) );
					
					float sliderWidth = sliderShape->getWidth() - PO_CONTROL_SLIDER_HEIGHT;
					float knobWidth = sliderKnob->getWidth();
					float xMin = 0;
					float xMax = sliderWidth;
					float posX = event->globalPosition.x - dragOffset.x - sOffset.x;
					
					posX = po::Clamp( xMin, xMax, posX);
					valF =  sqrt( pow(posX/xMax,2) ); 
					
					float mappedVal = po::Mapf(0.0f,1, valF, min, max );
					
					sliderKnob->position.x = sliderWidth*valF;
					
					char valString [256];
					sprintf( valString, "%s%.2f", valName.c_str(), mappedVal );
					shapeData->setText( valString );
					shapeData->doLayout();
					
					valF = mappedVal;
					
					po::Dictionary D;
					D.set("value", valF);
					
					if ( listener == NULL ) {
						//getParent()->messageHandler( ID, D);
					}
					else
						listener->messageHandler( ID, D);
					
					D.set("control", this);
					D.set("valueType", "float");
					getParent()->messageHandler("update_settings", D);
				}
			}
			
			if (event->type == po::MOUSE_UP_EVENT || event->type == po::TOUCH_ENDED_EVENT ){
				bDragging = false;
			}
		}

		void SliderF::setValue(float val) {
			
			float sliderWidth = sliderShape->getWidth() - PO_CONTROL_SLIDER_HEIGHT;
			float xMin = 0;
			float xMax = sliderWidth;
			
			float mappedVal = po::Mapf(min, max, val, 0.f, 1.f );
			valF =  mappedVal;
			
			sliderKnob->position.x = sliderWidth*valF;
			
			char valString [32];
			sprintf( valString, "%.2f", val );
			shapeData->setText( valString );
			shapeData->doLayout();
		}

		// -----------------------------------------------------------------------------------
		// =============================== Slider Integer ====================================
		#pragma mark SliderI
		
		//------------------------------------------------------------------------
		SliderI::SliderI( string _ID, int init, int _min, int _max, po::Object* _listener ) : Control( _ID, _listener ) {
			
			type = TYPE_INT;
			
			this->name = ID;
			min = _min;
			max = _max;
			
			valI = po::Clamp(min, max, init);
			valF = (float)valI;
			
			char valString [256];
			sprintf( valString, "%i", valI );
			
			addEvent( po::MOUSE_DOWN_EVENT, this );
			addEvent( po::TOUCH_BEGAN_EVENT, this);
			
			sliderShape = new po::RectShape( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			sliderShape->fillColor = po::Color(1,1,1,0.2);
			sliderKnob = new po::RectShape( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			sliderKnob->fillColor = po::Color(1,1,1,0.2);
			sliderKnob->position.x = po::Mapf(min, max, valI, 0, PO_CONTROL_SLIDER_WIDTH);
			sliderKnob->addEvent( po::MOUSE_DRAG_INSIDE_EVENT, this );
			sliderKnob->addEvent( po::MOUSE_DOWN_INSIDE_EVENT, this );
			sliderKnob->addEvent( po::MOUSE_UP_EVENT, this );
			sliderKnob->addEvent( po::TOUCH_BEGAN_INSIDE_EVENT, this );
			sliderKnob->addEvent( po::TOUCH_MOVED_EVENT, this);
			sliderKnob->addEvent( po::TOUCH_ENDED_EVENT, this);
			
			addChild( sliderShape );
			sliderShape->addChild( sliderKnob );
			
			shapeData = new po::TextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			shapeData->textColor = po::Color::white;
			shapeData->position.set(0,0,0);
			shapeData->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeData->setText(valName+valString);
			shapeData->doLayout();  
			addChild( shapeData );
			
			shapeLabel = new po::TextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			shapeLabel->textColor = po::Color::white;
			shapeLabel->position.set( PO_CONTROL_LABEL_X_POS,0,0);
			shapeLabel->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeLabel->setText( ID );
			shapeLabel->doLayout();  
			addChild( shapeLabel );
		}

		void SliderI::eventHandler(po::Event *event) {
			
			
			if (event->type == po::MOUSE_DOWN_INSIDE_EVENT || event->type == po::TOUCH_BEGAN_INSIDE_EVENT ) {
				
				dragOffset = event->localPosition;
				bDragging = true;
			}
			
			
			if (event->type == po::MOUSE_DRAG_INSIDE_EVENT || event->type == po::TOUCH_MOVED_EVENT ) {
				
				if(bDragging) {
					po::Point sOffset = sliderShape->localToGlobal( po::Point(0,0) );
					float sliderWidth = sliderShape->getWidth() - PO_CONTROL_SLIDER_HEIGHT;
					float knobWidth = sliderKnob->getWidth();
					float xMin = 0;
					float xMax = sliderWidth;
					float posX = event->globalPosition.x - dragOffset.x - sOffset.x;
					
					posX = po::Clamp( xMin, xMax, posX);
					
					valF =  sqrt( pow(posX/xMax,2) ); 
					float mappedVal = po::Mapf(0.0f,1, valF, min, max );
					
					sliderKnob->position.x = sliderWidth*valF;
					po::Dictionary D;
					
					char valString [256];
					sprintf( valString, "%i", (int)mappedVal );
					shapeData->setText( valString );
					shapeData->doLayout();  
					
					D.set("value", (int)mappedVal );
					valI = (int)mappedVal;

					if ( listener == NULL ) {
						//getParent()->messageHandler( ID, D);
					}
					else
						listener->messageHandler( ID, D);
					
					D.set("control", this);
					D.set("valueType", "int");
					getParent()->messageHandler("update_settings", D);
				}
			}
			if (event->type == po::MOUSE_UP_EVENT || event->type == po::TOUCH_ENDED_EVENT ) {
				bDragging = false;
			}
		}

		void SliderI::setValue(int val) {
			
			float sliderWidth = sliderShape->getWidth() - PO_CONTROL_SLIDER_HEIGHT;
			float xMin = 0;
			float xMax = sliderWidth;
				
			float mappedVal = po::Mapf(min, max, val, 0.f, 1.f );
			valF =  mappedVal;
			
			sliderKnob->position.x = sliderWidth*valF;
			
			char valString [32];
			sprintf( valString, "%i", val );
			shapeData->setText( valString );
			shapeData->doLayout();
		}

		// -----------------------------------------------------------------------------------
		// =============================== Point Slider ======================================
		#pragma mark PointSlider
		
		//------------------------------------------------------------------------
		PointSlider::PointSlider( string _ID, po::Point init, po::Point _min, po::Point _max, po::Object* _listener ) : Control( _ID, _listener ) {
			
			type = TYPE_POINT;
			
			this->name = ID;
			min = _min;
			max = _max;
			
			min.z = 0;
			max.z = 0;
			
			valP.x = po::Clamp(min.x, max.x, init.x);
			valP.y = po::Clamp(min.y, max.y, init.y);
			valP.z = 0;
			
			char valString [256];
			sprintf( valString, "%.2lf,%.2lf", valP.x, valP.y );
			
			addEvent( po::MOUSE_DOWN_EVENT, this );
			addEvent( po::TOUCH_BEGAN_EVENT, this );
			
			po::RectShape* back = new po::RectShape( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT, PO_CONTROL_SLIDER_WIDTH );
			back->fillColor = po::Color(1,1,1,0.2);
			addChild( back );
			
			sliderShape = new po::RectShape( PO_CONTROL_SLIDER_WIDTH, PO_CONTROL_SLIDER_WIDTH - PO_CONTROL_SLIDER_HEIGHT );
			sliderShape->position = po::Point (PO_CONTROL_SLIDER_HEIGHT/2,PO_CONTROL_SLIDER_HEIGHT/2);
			sliderShape->fillColor = po::Color(0,0,0,0);
			sliderPoint = new po::RectShape( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			sliderPoint->position.x = po::Mapf(min.x, max.x, valP.x, 0, sliderShape->getWidth()); 
			sliderPoint->position.y = po::Mapf(min.y, max.y, valP.y, 0, sliderShape->getHeight()); 
			sliderPoint->fillColor = po::Color(1,0,0,0.5);
			sliderPoint->setAlignment(po::ALIGN_CENTER_CENTER);
			sliderPoint->addEvent( po::MOUSE_DRAG_INSIDE_EVENT, this );
			sliderPoint->addEvent( po::MOUSE_DOWN_INSIDE_EVENT, this );
			sliderPoint->addEvent( po::MOUSE_UP_EVENT, this );
			sliderPoint->addEvent( po::TOUCH_BEGAN_INSIDE_EVENT, this );
			sliderPoint->addEvent( po::TOUCH_MOVED_EVENT, this);
			sliderPoint->addEvent( po::TOUCH_ENDED_EVENT, this);
			
			addChild( sliderShape );
			sliderShape->addChild( sliderPoint );
			
			shapeData = new po::TextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			shapeData->textColor = po::Color::white;
			shapeData->position.set(0,0,0);
			shapeData->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeData->setText( valString );
			shapeData->doLayout();  
			addChild( shapeData );
			
			shapeLabel = new po::TextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			shapeLabel->textColor = po::Color::white;
			shapeLabel->position.set( PO_CONTROL_LABEL_X_POS,0,0);
			shapeLabel->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeLabel->setText( ID );
			shapeLabel->doLayout();  
			addChild( shapeLabel );
		}

		void PointSlider::eventHandler(po::Event *event) {
			
			if (event->type == po::MOUSE_DOWN_INSIDE_EVENT || event->type == po::TOUCH_BEGAN_INSIDE_EVENT ) {
				bDragging = true;
			}
			
			if (event->type == po::MOUSE_DRAG_INSIDE_EVENT || event->type == po::TOUCH_MOVED_EVENT ) {
				
				if(bDragging) {
					po::Point P = event->globalPosition;
					P.y = getWindowHeight() - P.y;
					po::Point pos = sliderShape->globalToLocal( P );
					sliderPoint->position = pos; 
						  
					float sliderWidth = sliderShape->getWidth();
					float boxWidth = sliderPoint->getWidth();
					float xMin = 0;
					float xMax = sliderWidth+boxWidth/2;

					float sliderHeight = sliderShape->getHeight();
					float yMin = 0;
					float yMax = sliderHeight+boxWidth/2;
			 
					pos.x = po::Clamp( xMin, xMax, pos.x);
					pos.y = po::Clamp( yMin, yMax, pos.y);

					valP.x =  sqrt( pow( pos.x/xMax,2 ) ); 
					valP.y =  sqrt( pow( pos.y/yMax,2 ) ); 

					sliderPoint->position.x = sliderWidth*valP.x; 
					sliderPoint->position.y = sliderHeight*valP.y;
					
					po::Point mappedVal;
					mappedVal.x = po::Mapf(0.0f,1, valP.x, min.x, max.x);
					mappedVal.y = po::Mapf(0.0f,1, valP.y, min.y, max.y);        
							
					char valString [256];
					sprintf( valString, "%.2lf,%.2lf", mappedVal.x, mappedVal.y );
					shapeData->setText( valString );
					shapeData->doLayout();  
					
					po::Dictionary D;
					D.set("value", mappedVal);
					valP = mappedVal;
					
					if ( listener == NULL ) {
						//getParent()->messageHandler( ID, D );
					}
					else
						listener->messageHandler( ID, D );
					
					D.set("control", this);
					D.set("valueType", "point");
					getParent()->messageHandler("update_settings", D);
				}
			}
			
			if (event->type == po::MOUSE_UP_EVENT || event->type == po::TOUCH_ENDED_EVENT ) {
				bDragging = false;
			}
		}

		// -----------------------------------------------------------------------------------
		// =============================== Toggle Box ========================================
		#pragma mark ToggleBox
		
		//------------------------------------------------------------------------
		ToggleBox::ToggleBox( string _ID, bool init, po::Object* _listener ) : Control( _ID, _listener ) {
			
			type = TYPE_BOOL;
			
			valB = init;
			this->name = ID;
			
			addEvent( po::MOUSE_DOWN_EVENT, this );
			addEvent( po::TOUCH_BEGAN_EVENT, this );
			
			po::RectShape* back = new po::RectShape( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT);
			back->fillColor = po::Color(1,1,1,0.2);
			addChild( back );
			
			toggleShape = new po::RectShape( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			toggleShape->fillColor = po::Color((int)valB,0,0,(int)valB);
			toggleShape->addEvent( po::MOUSE_DOWN_INSIDE_EVENT, this );
			toggleShape->addEvent( po::TOUCH_BEGAN_INSIDE_EVENT, this );
			addChild( toggleShape );
			
			shapeLabel = new po::TextBox( 300, PO_CONTROL_SLIDER_HEIGHT );
			shapeLabel->textColor = po::Color::white;
			shapeLabel->position.set( PO_CONTROL_SLIDER_HEIGHT,0,0);
			shapeLabel->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeLabel->setText( ID );
			shapeLabel->doLayout();  
			addChild( shapeLabel );
		}

		void ToggleBox::eventHandler(po::Event *event) {
		 
			if (event->type == po::MOUSE_DOWN_INSIDE_EVENT || event->type == po::TOUCH_BEGAN_INSIDE_EVENT ) {
				valB = !valB;
				toggleShape->fillColor = po::Color((int)valB,0,0,(int)valB);
				
				po::Dictionary D;
				D.set("value", valB);

				if ( listener == NULL ) {
					//getParent()->messageHandler( ID, D);
				}
				else {
					listener->messageHandler( ID, D);
				}
				
				D.set("control", this);
				D.set("valueType", "bool");
				getParent()->messageHandler("update_settings", D);
			}
		}

		void ToggleBox::setValue(bool val) {
			
			toggleShape->fillColor = po::Color((int)val,0,0,(int)val);
		}

		// --------------------------------------------------------------------------------
		// =============================== Radio ==========================================
		#pragma mark Radio
		
		//------------------------------------------------------------------------
		Radio::Radio( string _ID, int init, vector<string> names, po::Object* _listener ) : Control( _ID,_listener ) {
			
			type = TYPE_INT;
			
			valI = init;
			this->name = ID;
			
			for ( int i=0; i<names.size(); i++ ) {
				bool thisVal;
				if ( valI == i ) thisVal = true; else thisVal = false; 
						
				string uID = ID+"_"+names[i];
				ToggleBox* T = new ToggleBox( uID, thisVal, this );
				T->position = po::Point( 0,i*(PO_CONTROL_SLIDER_HEIGHT+PO_CONTROL_SPACER) );
				T->shapeLabel->setText( names[i] );
				T->shapeLabel->doLayout();
				addChild( T );
				buttons.push_back( T );
			}
			
			shapeLabel = new po::TextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			shapeLabel->textColor = po::Color::white;
			shapeLabel->position.set( PO_CONTROL_SLIDER_HEIGHT,0,0);
			shapeLabel->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeLabel->setText( ID );
			shapeLabel->doLayout();
			addChild( shapeLabel );
		}

		void Radio::eventHandler(po::Event *event) {
		}

		void Radio::messageHandler(const std::string &msg, const po::Dictionary& dict) {
			
			if(msg == "update_settings")
				return;
			else {
				for(int i=0 ; i<buttons.size(); i++) {
					ToggleBox* T = buttons[i];
					if ( T->name == msg ) {
						T->valB = true;
						T->toggleShape->fillColor = po::Color(1,0,0,1);
						valI = i;
					}
					else {
						T->valB = false;
						T->toggleShape->fillColor = po::Color(0,0,0,0);
					}
				}
				
				po::Dictionary D;
				D.set( "value",valI ); 
				
				if ( listener == NULL ) {
				}
				else
					listener->messageHandler( ID, D);
				
				D.set("control", this);
				D.set("valueType", "int");
				getParent()->messageHandler("update_settings", D);
			}
		}

		// -----------------------------------------------------------------------------------
		// =============================== Input Text Box ====================================
		#pragma mark InputTextBox
		
		//------------------------------------------------------------------------
		InputTextBox::InputTextBox( string _ID, string init, po::Object* _listener ) : Control( _ID, _listener ) {
			
			type = TYPE_TEXT;
			
			this->name = ID;
			string temp;
			temp = init;
			//cout << temp << endl;
			temp.erase(temp.find_last_not_of("'")+1);
			temp.erase(0,temp.find_first_not_of("'"));
			valS = temp;
				
			back = new po::RectShape( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT, 30 );
			back->fillColor = po::Color(1,1,1,0.2);
			back->strokeEnabled = false;
			back->addEvent(po::MOUSE_DOWN_INSIDE_EVENT, this);
			addChild( back );
			
			shapeData = new po::TextBox( PO_CONTROL_SLIDER_WIDTH,30 );
			shapeData->textColor = po::Color::white;
			shapeData->position.set(0,0,0);
			shapeData->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeData->setText( valS );
			shapeData->doLayout();

			addChild( shapeData );
			
			shapeLabel = new po::TextBox();
			shapeLabel->textColor = po::Color::white;
			shapeLabel->position.set( PO_CONTROL_LABEL_X_POS,0,0);
			shapeLabel->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeLabel->setText( ID );
			shapeLabel->doLayout();  
			addChild( shapeLabel );
			
			addEvent( po::KEY_DOWN_EVENT, this );
			
			isActive = false;
			
			resize();
		}

		void InputTextBox::eventHandler(po::Event *event) {
			
			if (event->type == po::MOUSE_DOWN_INSIDE_EVENT) {
				isActive = !isActive;
				back->strokeEnabled = isActive;
			}
			
			if (event->type == po::KEY_DOWN_EVENT && isActive ) {
				string S;
				if ( event->keyChar != 127 && event->keyChar != 13 ) {
					S = event->keyChar;
					valS = valS+S;
				}

				if ( event->keyChar == 127 && valS.length() > 0 ) {
					valS = valS.erase( valS.length()-1 );
				}
				
				if ( event->keyChar == 13 ) {
					valS = valS + "\n";
				}
				
				shapeData->setText( valS );
				shapeData->doLayout();
				
				resize();
				
				po::Dictionary D;
				D.set("value", valS);
				
				if ( listener == NULL ) {
					//getParent()->messageHandler( ID, D);
				}
				else
					listener->messageHandler( ID, D);
				
				D.set("control", this);
				D.set("valueType", "string");
				getParent()->messageHandler("update_settings", D);
			}
		}

		void InputTextBox::resize() {
			po::Rect newSize( 0, 0, shapeData->getWidth(), shapeData->getTextBounds().height );
			if(newSize.height < 30) newSize.height = 30;
			shapeData->reshape( newSize );
			back->reshape( back->getWidth(), newSize.height );
			back->generateStroke(1);
			back->strokeEnabled = isActive;
		}

		// -----------------------------------------------------------------------------------
		// =============================== Color Slider ======================================
		#pragma mark ColorSlider
		
		//------------------------------------------------------------------------
		ColorSlider::ColorSlider( string _ID, po::Color init, bool _RGB, po::Object* _listener ) : Control( _ID, _listener ) {
			
			type = TYPE_COLOR;
			
			RGB = _RGB;
			valC = init;
			
			float R,G,B,A;
			this->name = ID;
			int values = 4;
			this->position = po::Point(0,PO_CONTROL_SPACER);
			
			for ( int i=0; i<values ; i++ ) {
				float myValue;
				string color;
				
				
				if( i==0 ) {
					myValue = valC.R;
					
					if (RGB) {
						color = ID+"_R";
						SliderF* S = new SliderF( color, myValue, 0, 1, "R ", this );
						S->sliderShape->fillColor = po::Color(1,1,1,1);
						S->sliderShape->placeTexture( calcTex( po::Color( 1,0,0 )) );
						S->position = po::Point( 0,i*PO_CONTROL_SLIDER_HEIGHT );
						S->shapeLabel->visible = false;
						S->valName = "R ";
						addChild( S );
						sliders.push_back( S );
						
					} 
					else {
						color = ID+"_H";
						SliderF* S = new SliderF( color, myValue, 0, 1, "H ", this );
						S->sliderShape->fillColor = po::Color(1,1,1,1);
						S->sliderShape->placeTexture( calcTex( po::Color(-1,valC.G,valC.B) ));
						S->position = po::Point( 0,i*PO_CONTROL_SLIDER_HEIGHT );
						S->shapeLabel->visible = false;
						S->valName = "H ";
						addChild( S );
						sliders.push_back( S );
					}
				}
					
				if( i==1 ) {
					myValue = valC.G;
					
					if (RGB) {
						color = ID+"_G";
						SliderF* S = new SliderF( color, myValue, 0, 1,"G ", this );
						S->sliderShape->fillColor = po::Color(1,1,1,1);
						S->sliderShape->placeTexture( calcTex( po::Color( 0,1,0 )) );
						S->position = po::Point( 0,i*PO_CONTROL_SLIDER_HEIGHT);
						S->shapeLabel->visible = false;
						addChild( S );
						sliders.push_back( S );
						
						
					} 
					else {
						color = ID+"_S";
						SliderF* S = new SliderF( color, myValue, 0, 1,"S ", this );
										
						S->sliderShape->fillColor = po::Color(1,1,1,1);
						S->sliderShape->placeTexture( calcTex( po::Color( valC.R,-1,valC.B ) ));
						S->position = po::Point( 0,i*PO_CONTROL_SLIDER_HEIGHT );
						S->shapeLabel->visible = false;
						addChild( S );
						sliders.push_back( S );
					}
				}
				
				if( i==2 ) {
					myValue = valC.B;

					if (RGB) {
						color = ID+"_B";
						SliderF* S = new SliderF( color, myValue, 0, 1,"B ", this );
						S->sliderShape->fillColor = po::Color(1,1,1,1);
						S->sliderShape->placeTexture( calcTex( po::Color( 0,0,1 )) );
						S->position = po::Point( 0,i*PO_CONTROL_SLIDER_HEIGHT );
						S->shapeLabel->visible = false;
						addChild( S );
						sliders.push_back( S );
					} 
					else {
						color = ID+"_V";
						SliderF* S = new SliderF( color, myValue, 0, 1,"V ", this );
						S->sliderShape->fillColor = po::Color(1,1,1,1);
						S->sliderShape->placeTexture( calcTex( po::Color( valC.R,valC.G,-1 ) ));
						S->position = po::Point( 0,i*PO_CONTROL_SLIDER_HEIGHT );
						S->shapeLabel->visible = false;
						addChild( S );
						sliders.push_back( S );
					}
				}
				
				
				if( i==3 ) {
					myValue = valC.A;
					color = ID+"_A";
					SliderF* S = new SliderF( color, myValue, 0, 1,"A ", this );
					S->sliderShape->fillColor = po::Color(1,1,1,1);
					if (!RGB) {
						S->sliderShape->placeTexture( calcTex( po::Color( 0,0,1,-1 ) ));
					} else S->sliderShape->placeTexture( calcTex( po::Color( 1,1,1,-1 ) ));
					
					S->shapeLabel->visible = false;
					S->position = po::Point( 0,i*PO_CONTROL_SLIDER_HEIGHT );
					addChild( S );
					sliders.push_back( S );
				}
			}
			
			shapeLabel = new po::TextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			shapeLabel->textColor = po::Color::white;
			shapeLabel->position.set( PO_CONTROL_LABEL_X_POS,3*PO_CONTROL_SLIDER_HEIGHT,0);
			shapeLabel->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeLabel->setText( ID );
			shapeLabel->doLayout();  
			addChild( shapeLabel );
			
			colorBox = new po::RectShape( PO_CONTROL_SLIDER_HEIGHT*3,PO_CONTROL_SLIDER_HEIGHT*3 );
			colorBox->position.set( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SPACER*4,0,0);
			
			if (RGB) {
				colorBox->fillColor = valC;
			} else colorBox->fillColor.setHSV(valC.R, valC.G, valC.B);
			
			addChild( colorBox );
		}

		po::Texture* ColorSlider::calcTex ( po::Color input ) {
			
			po::Image* I = new po::Image( PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT,4,NULL );
			
			for ( int x=0 ; x<PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT; x++) {
				for ( int y=0 ; y<PO_CONTROL_SLIDER_HEIGHT; y++) {
					float S,H,V,A,VAL;
					po::Color C;
					VAL = (float)x/(float)(PO_CONTROL_SLIDER_WIDTH+PO_CONTROL_SLIDER_HEIGHT);
					
					if ( input.R == -1 ) H = VAL; else H = input.R;
					if ( input.G == -1 ) S = VAL; else S = input.G;
					if ( input.B == -1 ) V = VAL; else V = input.B;
					if ( input.A == -1 ) A = VAL; else A = input.A;
				 
					C.setHSV( H,S,V );
					C.A = A;
					
					if ( RGB ) {
						C = po::Color( H,S,V,VAL);
					}
					
					
					I->setPixel( po::Point(x,y,0), C);
				}
			}
			po::Texture *tex = new po::Texture(I);
			delete I;
			return tex;
		}

		void ColorSlider::eventHandler(po::Event *event) {
		}

		void ColorSlider::messageHandler(const std::string &msg, const po::Dictionary& dict) {
			
		//    cout << msg << endl;
			
			if ( RGB ) {
				if ( msg == ID+"_R" ) {
					valC.R = dict.getFloat( "value" );
					SliderF* sR = (SliderF*) getChild( msg );
					sR->sliderShape->placeTexture( calcTex( po::Color( 1,0,0 )) );
					//cout << sR->back->fillColor << endl;
				} 
				
				else if ( msg == ID+"_G" ) {
					valC.G = dict.getFloat( "value" );
					SliderF* sG = (SliderF*) getChild( msg );
					sG->sliderShape->placeTexture( calcTex( po::Color( 0,1,0 )) );
				}
				
				else if ( msg == ID+"_B" ) {
					valC.B = dict.getFloat( "value" );
					SliderF* sB = (SliderF*) getChild( msg );
					sB->sliderShape->placeTexture( calcTex( po::Color( 0,0,1 )) );
				}
				
				else if ( msg == ID+"_A" ) {
					valC.A = dict.getFloat( "value" );
				}
				
				SliderF* sA = (SliderF*) getChild( ID+"_A" );
				sA->sliderShape->placeTexture( calcTex( po::Color( 1,1,1,-1 )) );
				
				colorBox->fillColor = valC;
			}
			else {
				
				SliderF* sH = (SliderF*) getChild( ID+"_H" );
				SliderF* sS = (SliderF*) getChild( ID+"_S" );
				SliderF* sV = (SliderF*) getChild( ID+"_V" );
				SliderF* sA = (SliderF*) getChild( ID+"_V" );
				
				if ( msg == ID+"_H" ) {
					valC.R = dict.getFloat( "value" );
					valC.R = po::Clamp( 0.0001f, 0.9999f, valC.R);
				}
				
				if ( msg == ID+"_S" ) {
					valC.G = dict.getFloat( "value" );
				}
				
				if ( msg == ID+"_V" ) {
					valC.B = dict.getFloat( "value" );
				}
				
				else if ( msg == ID+"_A" ) {
					valC.A = dict.getFloat( "value" );
				}
				
				sH->sliderShape->placeTexture( calcTex( po::Color( -1,valC.G,valC.B )) ); 
				sS->sliderShape->placeTexture( calcTex( po::Color( valC.R,-1,valC.B )) );
				sV->sliderShape->placeTexture( calcTex( po::Color( valC.R,valC.G,-1 )) );
				
				SliderF* Sa = (SliderF*) getChild( ID+"_A" );
				Sa->sliderShape->placeTexture( calcTex( po::Color( 0,0,1,-1 )) );
				colorBox->fillColor.setHSV( valC.R,valC.G,valC.B );
			}
			
			po::Dictionary D;
			D.set("value", valC);
			
			if ( listener == NULL );
				//getParent()->messageHandler( ID, D);
			else
				listener->messageHandler( ID, D);
			
			D.set("control", this);
			D.set("valueType", "color");
			getParent()->messageHandler("update_settings", D);
		}

		// ---------------------------------------------------------------------------------
		// =============================== Knob ============================================
		#pragma mark Knob
		
		//------------------------------------------------------------------------
		Knob::Knob( string _ID, float init, float _min, float _max, po::Object* _listener ) : Control( _ID, _listener ) {
			
			type = TYPE_FLOAT;
			
			this->name = ID;
			min = _min;
			max = _max;
			
			valF = po::Clamp(min, max, init);
			
			char valString [256];
			sprintf( valString, "%.2lf", valF );
			
			addEvent( po::MOUSE_DOWN_EVENT, this );
			addEvent( po::TOUCH_BEGAN_EVENT, this );
			
			sliderShape = new po::OvalShape( PO_CONTROL_SLIDER_HEIGHT*3,PO_CONTROL_SLIDER_HEIGHT*3,30 );;
			sliderShape->fillColor = po::Color(1,1,1,0.2);
			sliderKnob = new po::RectShape( PO_CONTROL_SLIDER_HEIGHT*4/3,3);
			sliderKnob->fillColor = po::Color(1,0,0,0.5);
			sliderKnob->position = po::Point(0,0); 
			sliderKnob->setAlignment(po::ALIGN_CENTER_RIGHT);
			sliderKnob->rotation = valF;
			sliderShape->addEvent( po::MOUSE_DRAG_INSIDE_EVENT, this );
			sliderShape->addEvent( po::MOUSE_DOWN_INSIDE_EVENT, this );
			sliderShape->addEvent( po::MOUSE_UP_EVENT, this);
			sliderShape->addEvent( po::TOUCH_BEGAN_INSIDE_EVENT, this );
			sliderShape->addEvent( po::TOUCH_MOVED_EVENT, this);
			sliderShape->addEvent( po::TOUCH_ENDED_EVENT, this);
			
			addChild( sliderShape );
			sliderShape->addChild( sliderKnob );
			sliderShape->position = po::Point( sliderShape->getWidth()/2,sliderShape->getWidth()/2 ) ;
			
			shapeData = new po::TextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			shapeData->textColor = po::Color::white;
			shapeData->position.set(PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT*3,0);
			shapeData->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeData->setTextAlignment(po::ALIGN_CENTER_CENTER);
			shapeData->setText(valString);
			shapeData->doLayout();  
			addChild( shapeData );
			
			shapeLabel = new po::TextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			shapeLabel->textColor = po::Color::white;
			shapeLabel->position.set( PO_CONTROL_LABEL_X_POS,0,0);
			shapeLabel->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeLabel->setText( ID );
			shapeLabel->doLayout();  
			addChild( shapeLabel );
		}

		void Knob::eventHandler(po::Event *event) {
			
			if (event->type == po::MOUSE_DOWN_INSIDE_EVENT || event->type == po::TOUCH_BEGAN_INSIDE_EVENT ) {
				bDragging = true;
			}
			
			if (event->type == po::MOUSE_DRAG_INSIDE_EVENT || event->type == po::TOUCH_MOVED_EVENT ) {
				
				if(bDragging) {
					po::Point P = event->globalPosition;
					P.y = getWindowHeight() - P.y;
					po::Point pos = sliderShape->globalToLocal( P );
					
					//angle to mouse
					float ang = po::rad2deg( atan2( pos.y, pos.x ) );
					ang = 180+ang;
					
					sliderKnob->rotation = ang;
					
					valF = po::Mapf(0.f, 360.f, ang, min, max);
					
					char valString [256];
					sprintf( valString, "%.2lf", valF );
					shapeData->setText( valString );
					shapeData->doLayout();  
					
					po::Dictionary D;
					D.set("value", valF);
					
					if ( listener == NULL ) {
						//getParent()->messageHandler( ID, D);
					}
					else
						listener->messageHandler( ID, D);
					
					D.set("control", this);
					D.set("valueType", "float");
					getParent()->messageHandler("update_settings", D);
				}
			}
			
			if (event->type == po::MOUSE_UP_EVENT || event->type == po::TOUCH_ENDED_EVENT ) {
				bDragging = false;
			}
		}

		// ---------------------------------------------------------------------------------
		// =============================== Button ==========================================
		#pragma mark Button
		
		//------------------------------------------------------------------------
		Control_Button::Control_Button( string _ID, po::Object* _listener ) : Control( _ID, _listener ) {
			
			type = TYPE_MESSAGE;
			
			this->name = ID;
			addEvent( po::MOUSE_DOWN_EVENT, this );
			addEvent( po::TOUCH_BEGAN_EVENT, this);
			
			shapeLabel = new po::TextBox( PO_CONTROL_SLIDER_HEIGHT,PO_CONTROL_SLIDER_HEIGHT );
			shapeLabel->textColor = po::Color::white;
			shapeLabel->position.set( 0,0,0);
			shapeLabel->setPadding( PO_CONTROL_SPACER );
			shapeLabel->setTextSize(PO_CONTROL_TEXT_SIZE);
			shapeLabel->setText( ID );
			shapeLabel->doLayout();  
			addChild( shapeLabel );
			
			back = new po::RectShape( shapeLabel->getTextBounds().width+PO_CONTROL_SPACER*2,shapeLabel->getTextBounds().height+PO_CONTROL_SPACER*2 );
			back->position = po::Point(PO_CONTROL_SPACER/2,PO_CONTROL_SPACER/2);
			back->fillColor = po::Color(1,1,1,0.2);
			back->addEvent( po::MOUSE_DOWN_INSIDE_EVENT, this );
			back->addEvent( po::TOUCH_BEGAN_INSIDE_EVENT, this);
			shapeLabel->addChild( back );
			
			po::RectShape* space = new po::RectShape( back->getWidth(), back->getHeight() );
			space->fillColor = po::Color(0,0,0,0);
			addChild( space );
		}

		void Control_Button::eventHandler(po::Event *event) {
			
			if (event->type == po::MOUSE_DOWN_INSIDE_EVENT || event->type == po::TOUCH_BEGAN_INSIDE_EVENT ) {
				
				po::Dictionary D;
				D.set("value", ID);
				
				if ( listener == NULL ) {
					//getParent()->messageHandler( ID, D);
				}
				else
					listener->messageHandler( ID, D);
			}
		}
	}
}