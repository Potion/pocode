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
//	updated by Jonathan Bobrow on 8/3/13.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poImageShape.h"
#include "poTextBox.h"

#define PO_CONTROL_TEXT_SIZE 11
#define PO_CONTROL_SLIDER_HEIGHT 15
#define PO_CONTROL_SLIDER_WIDTH 150
#define PO_CONTROL_SPACER 5


using namespace std;

namespace po {
	namespace control {
		
		enum Type {
			TYPE_FLOAT,
			TYPE_INT,
			TYPE_POINT,
			TYPE_COLOR,
			TYPE_BOOL,
			TYPE_TEXT,
			TYPE_MESSAGE
		};
		
		class Control : public po::Object {
		public:
			
			Control( string _ID, po::Object* _listener = NULL );
			string ID;    
			po::Object* listener;
			
			Type type;
			
			float		valF;
			int			valI;
			bool		valB;
			po::Point	valP;
			po::Color	valC;
			string		valS;
			
			bool	bDragging;
		};

		class SliderF : public Control {
		public:
			
			SliderF( string _ID, float init, float _min = 0, float _max = 1, string _valName = "" ,po::Object* _listener = NULL );
			virtual void eventHandler(po::Event *event);
			void setValue(float val);
			
			po::RectShape*	sliderShape;
			po::RectShape*	sliderKnob;
			po::TextBox*	shapeData;
			po::TextBox*	shapeLabel;
			po::Point		dragOffset;
			
			float			min,max;
			string			valName;
		};

		class SliderI : public Control {
		public:
			
			SliderI( string _ID, int init, int _min = 0, int _max = 255, po::Object* _listener = NULL );
			virtual void eventHandler(po::Event *event);
			void setValue(int val);
			
			po::RectShape*	sliderShape;
			po::RectShape*	sliderKnob;
			po::TextBox*	shapeData;
			po::TextBox*	shapeLabel;
			po::Point		dragOffset;
			
			int				min,max;
			string			valName;
		};

		class PointSlider : public Control {
		public:
			
			PointSlider( string _ID, po::Point init, po::Point _min = po::Point(0,0), po::Point _max = po::Point(1,1), po::Object* _listener = NULL );
			virtual void eventHandler(po::Event *event);
			
			po::RectShape*	sliderShape;
			po::RectShape*	sliderPoint;
			po::TextBox*	shapeData;
			po::TextBox*	shapeLabel;
			po::Point		dragOffset;
			po::Point		min,max;
		};

		class ToggleBox : public Control {
		public:
			
			ToggleBox( string _ID, bool init, po::Object* _listener = NULL );
			virtual void eventHandler(po::Event *event);
			void setValue(bool val);

			po::RectShape* toggleShape;
			po::TextBox*   shapeLabel;
		};

		class Radio : public Control {
		public:
			
			Radio( string _ID, int init, vector<string> names ,po::Object* _listener = NULL );
			virtual void eventHandler(po::Event *event);
			virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary());
			vector <ToggleBox*> buttons;
			po::TextBox*   shapeLabel;
		};

		class InputTextBox : public Control {
		public:
			
			InputTextBox( string _ID, string init, po::Object* _listener = NULL );
			virtual void eventHandler(po::Event *event);
			
			void resize();

			po::RectShape*		back;
			po::TextBox*		shapeLabel;
			po::TextBox*		shapeData;
			po::TextBox*		inputBox;
			
			bool				isActive;
		};

		class ColorSlider : public Control {
		public:
			
			ColorSlider( string _ID, po::Color init, bool _RGB, po::Object* _listener = NULL );
			virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary());
			virtual void eventHandler(po::Event *event);
			po::Texture* calcTex ( po::Color input );

			po::RectShape*		colorBox;
			vector <SliderF*>	sliders;
			po::Point			min,max;
			po::TextBox*		shapeLabel;
			bool                RGB;
		};

		class Knob : public Control {
		public:
			
			Knob( string _ID, float init, float _min = 0, float _max = 1, po::Object* _listener = NULL );
			virtual void eventHandler(po::Event *event);
			
			po::RectShape* back;
			po::OvalShape* sliderShape;
			po::RectShape* sliderKnob;
			po::TextBox*   shapeData;
			po::TextBox*   shapeLabel;
			float min,max;
			po::Point dragOffset;
		};


		class Button : public Control {
		public:
			
			Button( string _ID, po::Object* _listener = NULL );
			virtual void eventHandler(po::Event *event);
			
			po::RectShape* back;
			po::TextBox*   shapeLabel;
		};
	}
}