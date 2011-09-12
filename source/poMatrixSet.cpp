#include "poMatrixSet.h"
#include <glm/gtc/matrix_transform.hpp>

poPoint poMatrixSet::globalToLocal(poPoint pt) const {
	using namespace glm;
	
	vec3 win(pt.x, pt.y, pt.z);
	vec3 response = unProject(win, modelview, projection, viewport);
	return poPoint(response.x, response.y, response.z);
}

poPoint poMatrixSet::localToGlobal(poPoint pt) const {
	using namespace glm;
	
	vec3 obj(pt.x, pt.y, pt.z);
	vec3 response = project(obj, modelview, projection, viewport);
	return poPoint(response.x, response.y, response.z);
}
