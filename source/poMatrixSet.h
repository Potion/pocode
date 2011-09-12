#pragma once

#include "poPoint.h"
#include <glm/glm.hpp>

struct poMatrixSet
{
	poPoint		globalToLocal(poPoint pt) const;
	poPoint		localToGlobal(poPoint pt) const;
	
	glm::mat4	modelview, projection;
	glm::vec4	viewport;
};
