#pragma once

#include "poPoint.h"

enum poMatrixOrder
{
	// standard order
	PO_MATRIX_ORDER_TRS,
	// orbit order
	PO_MATRIX_ORDER_RST
};

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
