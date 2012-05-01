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

#include "poMatrixSet.h"
#include "poOpenGLState.h"
#include "poApplication.h"
//#include <glm/gtc/matrix_transform.hpp>

poMatrixSet::poMatrixSet() 
:	dirty(true)
,   camType(PO_CAMERA_NONE)
{}

void poMatrixSet::capture() {
	if(dirty) {
		poOpenGLState *ogl = poOpenGLState::get();
		projection = ogl->matrix.getProjection();
		modelview = ogl->matrix.getModelview();
		viewport = ogl->matrix.getViewport();
		dirty = false;
	}
}

poPoint poMatrixSet::globalToLocal(poPoint pt) const {
	using namespace glm;
	
	vec3 win(pt.x, pt.y, pt.z);
	vec4 viewp(viewport.x, viewport.y, getWindowWidth(), getWindowHeight());
	vec3 response = unProject(win, modelview, projection, viewp);
	return poPoint(response.x, response.y, response.z);
}

poPoint poMatrixSet::localToGlobal(poPoint pt) const {
	using namespace glm;
	
	vec3 obj(pt.x, pt.y, pt.z);
	vec4 viewp(viewport.x, viewport.y, viewport.width, viewport.height);
	vec3 response = project(obj, modelview, projection, viewp);
	return poPoint(response.x, response.y, response.z);
}

poPoint poMatrixSet::localToGlobal2(poPoint pt) const {
	using namespace glm;
	return value_ptr(inverse(modelview) * vec4(pt.x, pt.y, pt.z,1.f));
}
