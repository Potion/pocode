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

#include "poLayout.h"

poLayout::poLayout(poOrientation orientation) : 
layoutOrientation(orientation),
refreshEveryFrame(true),
layoutDone(false)
{}

poLayout::~poLayout() {}

void poLayout::setSpacing(float f) {
	setSpacing(f, f);
}

void poLayout::setSpacing(float w, float h) {
	spacing[SPACING_H] = w;
	spacing[SPACING_V] = h;
}

bool poLayout::doesRefreshEveryFrame() {
	return refreshEveryFrame;
}

void poLayout::setRefreshEveryFrame( bool b ) {
	refreshEveryFrame = b;
}

void poLayout::doLayout( poObject* obj ) {
	poPoint pos;
	for(int i=0; i < obj->getNumChildren(); i++) {
		
		poObject* child = obj->getChild(i);
		child->position = pos;
		
		if( layoutOrientation == PO_HORIZONTAL ){
			child->position.x -= child->offset.x;
			pos.x += child->getBounds().width;
			pos.x += spacing[SPACING_H];
		}
		else if( layoutOrientation == PO_VERTICAL ) {
			child->position.y -= child->offset.y;
			pos.y += child->getBounds().height;
			pos.y += spacing[SPACING_V];
		}
	}
}

void poLayout::doSetUp( poObject* obj ) {
	if(refreshEveryFrame || !layoutDone) {
		doLayout( obj );
		layoutDone = true;
	}
}


