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
//	updated by Jonathan Bobrow on 8/3/13.
//  Copyright 2012 Potion Design. All rights reserved.
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

namespace po {
	namespace control {
		

		class ControlPanel : public po::Object {

		public:
			ControlPanel( string _label, po::Color _color = po::Color( 1,1,1,.2 ), int textSize = 11, string _path = "" );
			
			virtual void update();
			virtual void eventHandler(po::Event *event);
			virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary());
			
			void addControl( po::Object* obj );
			void addSliderF( string _ID, float min=0, float max=1, po::Object* obj=NULL );
			void addKnob( string _ID, float min=0, float max=1, po::Object* obj=NULL );
			void addSliderI( string _ID, int min=0, int max=255, po::Object* obj=NULL );
			void addInputTextBox( string _ID, po::Object* obj=NULL );
			void addPointSlider( string _ID, po::Point min=po::Point(0,0), po::Point max=po::Point(1,1) ,po::Object* obj=NULL );
			void addColorSlider( string _ID, bool RGB=true ,po::Object* obj=NULL );
			void addRadio( string _ID , vector<string> names, po::Object* obj=NULL );
			void addButton( string _ID , po::Object* obj=NULL );
			void addToggle( string _ID, po::Object* obj=NULL );
			void autoResize();
			void saveSettings();
			void readSettings();
			
			void setSavePath( string s );
			
			void			setBool( string s, bool val );
			void			setInt( string s, int val );
			void			setFloat( string s, float val );
			void			setString( string s, string setString );
			void			setPoint( string s, po::Point val );
			
			bool			getBool( string s);
			int				getInt( string s);
			float			getFloat( string s);
			string			getString( string s);
			po::Point		getPoint( string s);
			po::Color		getColor( string s);
						
			po::Color		panelColor;

			po::Object*		container;
			po::Layout*		containerLayout;
			po::RectShape*	bar;
			po::RectShape*	save;
			po::RectShape*	box;
			po::RectShape*	hide;
			po::TextBox*	hideText;
			string			label;
			string			path;
			
			po::Point		pos;
			
			int				winWidth, winHeight;
			bool			isDragged;
			po::Point		prevPoint;
			float			r,g,b,a;
			
			po::Dictionary settings;
			
			bool isResized;
		};

	}
}