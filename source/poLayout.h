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

class poLayout : public poObjectModifier
{
public:
	poLayout(poOrientation orientation);
	
	void				setSpacing(float f);
	void				setSpacing(float w, float h);
	
	bool				doesRefreshEveryFrame();
	void				setRefreshEveryFrame(bool b);
	
	virtual				~poLayout();
	
protected:
	virtual void	doSetUp(poObject*);
	
	void			doLayout( poObject* obj );
	
	poOrientation	layoutOrientation;
	
	enum {
		SPACING_H,
		SPACING_V
	};
	float spacing[2];
	
	bool refreshEveryFrame;
	bool layoutDone;
};
