/*
 *  poCommon.h
 *  poFramework2
 *
 *  Created by Joshua Fisher on 9/20/07.
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 *
 */

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctype.h>
#include <cmath>
#include <ctime>
#include <cassert>
#include <cctype>

#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#else
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>

#include <Carbon/Carbon.h>
#include <ApplicationServices/ApplicationServices.h>
#include <tr1/unordered_map>
#endif

#include "poColor.h"
#include "poMath.h"

#ifndef _PO_COMMON_H_
#define _PO_COMMON_H_


enum poOrientation {
	PO_VERTICAL,
	PO_HORIZONTAL
};

enum poVertOrientation {
	PO_ALIGN_BOTTOM=0,
	PO_ALIGN_TOP,
	PO_ALIGN_VERT_CENTER
};

enum poHoriOrientation {
	PO_ALIGN_LEFT=0,
	PO_ALIGN_RIGHT,
	PO_ALIGN_HORI_CENTER
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
	PO_RTS_MATRIX, 
	PO_RST_MATRIX, 
	PO_TRS_MATRIX, 
	PO_TSR_MATRIX, 
	PO_SRT_MATRIX, 
	PO_STR_MATRIX 
};

struct poMatrixSet
{
	poMatrixSet() { memset(modelMatrix, 0, 16*sizeof(float)); memset(modelMatrix, 0, 16*sizeof(float)); memset(viewport, 0, 4*sizeof(int)); };

	bool		isZero() { return ( viewport[0]==0.0 && viewport[1]==0.0 && viewport[2]==0.0 && viewport[3]==0.0 ); };

    GLdouble    modelMatrix[16];
    GLdouble    projectMatrix[16];
    GLint       viewport[4];
};

struct poBoundingBox
{
	float left, right, top, bottom;
	
	poBoundingBox()
	: left(0), right(0), top(0), bottom(0) {}
	poBoundingBox(float l, float r, float t, float b)
	: left(l), right(r), top(t), bottom(b) {}
	
	poBoundingBox operator-(const poBoundingBox& rhs)
	{	poBoundingBox ret; 
		ret.left = left + rhs.left; ret.bottom = bottom + rhs.bottom; 
		ret.top = top - rhs.top; ret.right = right - rhs.right;
		return ret;
	}
	poBoundingBox operator+(const poBoundingBox& rhs)
	{	poBoundingBox ret; 
		ret.left = left - rhs.left; ret.bottom = bottom - rhs.bottom; 
		ret.top = top + rhs.top; ret.right = right + rhs.right;
		return ret;
	}
	
	void	initFarBounds() { left = 10000000; right = -10000000; bottom = 10000000; top = -10000000; };
};


#endif