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

#include "poObject.h"


// CLASS NOTES
// poLayout rearranges the children of a poObject to create a layout
// It does that by changing the position of each child, not their alignment
// Currently a poLayout can place objects in line horizontally or vertically.
// Soon there will be the option to arrange objects on a grid.


class poLayout : public poObjectModifier
{
public:
	poLayout(poOrientation orientation);
	
	void				setSpacing(float f);
	void				setSpacing(float w, float h);
	
	poOrientation		getOrientation();
	void				setOrientation(poOrientation orient);
	
	bool				doesRefreshEveryFrame();
	void				setRefreshEveryFrame(bool b);
	
	void				refresh();
	
	virtual				~poLayout();
	
protected:
	virtual void	doSetUp(poObject*);
	
	void			doLayout(poObject*);
	
	poOrientation	layoutOrientation;
	
	enum {
		SPACING_H,
		SPACING_V
	};
	float spacing[2];
	
private:
	bool refreshEveryFrame;
	bool layoutDone;
};
