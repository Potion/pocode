#pragma once

#include "poPoint.h"

struct poMatrixSet
{
	poMatrixSet();
	bool		isZero() const;
	void		capture();
	poPoint		globalToLocal(poPoint pt) const;
	poPoint		localToGlobal(poPoint pt) const;
	
    GLdouble    modelMatrix[16];
    GLdouble    projectMatrix[16];
    GLint       viewport[4];
};
