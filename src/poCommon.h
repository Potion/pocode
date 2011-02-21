/*
 *  Copyright 2011 Potion Design LLC. All rights reserved.
 */

#pragma once

#include "poInclude.h"

enum poOrientation {
	PO_VERTICAL,
	PO_HORIZONTAL
};

enum poAlignment {
	PO_ALIGN_TOP_LEFT,
	PO_ALIGN_TOP_CENTER,
	PO_ALIGN_TOP_RIGHT,
	PO_ALIGN_CENTER_LEFT,
	PO_ALIGN_CENTER_CENTER,
	PO_ALIGN_CENTER_RIGHT,
	PO_ALIGN_BOTTOM_LEFT,
	PO_ALIGN_BOTTOM_CENTER,
	PO_ALIGN_BOTTOM_RIGHT,
	
	PO_ALIGN_NUM_OPTIONS
};

enum poTextureScaleOption {
	PO_TEX_OBJ_RESHAPE=0,
	PO_TEX_NO_FIT=1,
	PO_TEX_FIT_H=2,
	PO_TEX_FIT_V=3,
	PO_TEX_FIT_MIN=4,
	PO_TEX_FIT_MAX=5
};

enum poStrokePlacementOption {
	PO_STROKE_INSIDE=0,
	PO_STROKE_OUTSIDE,
	PO_STROKE_CENTER
};

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

#define CHECK_DELETE(ptr)					\
			if(ptr != NULL) delete (ptr);	\
			ptr = NULL

