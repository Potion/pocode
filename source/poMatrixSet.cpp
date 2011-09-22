#include "poMatrixSet.h"
#include "poOpenGLState.h"
#include <glm/gtc/matrix_transform.hpp>

poMatrixSet::poMatrixSet() 
:	dirty(true) 
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
	vec4 viewp(viewport.x, viewport.y, viewport.width, viewport.height);
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
