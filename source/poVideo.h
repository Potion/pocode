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

//
//  poVideo.h
//  poFramework4
//
//  Created by Joshua Fisher on 6/29/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once
#include "common_include.h"

class poVideoCapture {
public:
	poVideoCapture();
	~poVideoCapture();
	
private:
	void *impl;
};

class poVideo {
	
private:
	void *impl;
};