#pragma once

#include "poRect.h"
#include "poPoint.h"
#include <glm/glm.hpp>

struct poMatrixSet
{
				poMatrixSet();
	void		capture();
	poPoint		globalToLocal(poPoint pt) const;
	poPoint		localToGlobal(poPoint pt) const;
	
	glm::mat4	modelview, projection;
	poRect		viewport;
	
	bool		dirty;
};
