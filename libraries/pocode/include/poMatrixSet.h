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


#include "poRect.h"
#include "poPoint.h"
#include <glm/glm.hpp>

struct poMatrixSet {
				poMatrixSet();
	void		capture();
	poPoint		globalToLocal(poPoint pt) const;
	poPoint		localToGlobal(poPoint pt) const;
	
	poPoint		localToGlobal2(poPoint pt) const;
	
	glm::mat4	modelview, projection;
	poRect		viewport;
	
	bool		dirty;
};
