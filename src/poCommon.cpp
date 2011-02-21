/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */


#include "poCommon.h"

poMatrixSet::poMatrixSet() { 
	memset(modelMatrix, 0, 16*sizeof(GLdouble)); 
	memset(projectMatrix, 0, 16*sizeof(GLdouble)); 
	memset(viewport, 0, 4*sizeof(int)); 
}

bool poMatrixSet::isZero() const { 
	return (viewport[0]==0.0 && 
			viewport[1]==0.0 && 
			viewport[2]==0.0 && 
			viewport[3]==0.0 ); 
}

void poMatrixSet::capture() {
	glGetIntegerv( GL_VIEWPORT, viewport );
	glGetDoublev( GL_MODELVIEW_MATRIX, modelMatrix );
	glGetDoublev( GL_PROJECTION_MATRIX, projectMatrix );
}

poPoint poMatrixSet::globalToLocal(poPoint pt) const {
	GLdouble objX, objY, objZ;
	GLdouble winX = pt.x, winY = pt.y, winZ = 0;
	
	gluUnProject(winX, winY, winZ,
				 modelMatrix, projectMatrix, viewport, 
				 &objX, &objY, &objZ);
	
	return poPoint(objX, objY, objZ);
}

poPoint poMatrixSet::localToGlobal(poPoint pt) const {
	GLdouble objX = pt.x, objY = pt.y, objZ = pt.z;
	GLdouble winX, winY, winZ;
	
	gluProject(objX, objY, objZ ,
			   modelMatrix, projectMatrix, viewport, 
			   &winX, &winY, &winZ);
	
	return poPoint(winX, winY, winZ);
}

