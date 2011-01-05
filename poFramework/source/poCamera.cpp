#include "poCamera.h"
#include "poObject.h"
#include "poApp.h"

poCamera::poCamera()
:	rotation(0), rotationAxis(poPoint(0,0,1)), position(0,0,0), scale(1), scale2D(1,1,1)
{
	registry.addSeparator( getClassName() );
	registry.add("position", &position);
	registry.add("rotation",  &rotation);
	registry.add("rotationAxis", &rotationAxis);
	registry.add("scale", &scale);
}

void poCamera2D::setup(poObject* obj)
{
	glViewport(0, 0, poApp::appW, poApp::appH);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, poApp::appW, 0, poApp::appH);
	glTranslatef( position.x, position.y, 0 );
	glRotatef( rotation, 0,0,1 );
	//glScalef( scale, scale, scale );
	glScalef( scale2D.x, scale2D.y, 1 );
	glMatrixMode(GL_MODELVIEW);
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

void poCamera2D::setdown(poObject* obj)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

poCamera3D::poCamera3D()
:	fov(60), nearPlane(0.1), farPlane(100000),
	up(poPoint(0,1,0)),	forward(poPoint(0,0,-1)),
	position(poPoint(0,0,0)), lookAtPoint(poPoint(0,0,0))
{
}

poCamera3D::poCamera3D(float fov, float N, float F)
:	fov(fov), nearPlane(N), farPlane(F),
	up(poPoint(0,1,0)),	forward(poPoint(0,0,-1)),
	position(poPoint(0,0,0)), lookAtPoint(poPoint(0,0,0))
{}

void poCamera3D::lookat(float x, float y, float z)
{
	lookat(poPoint(x,y,z));
}

void poCamera3D::lookat(const poPoint& p)
{
	poPoint view = (p - position).normalize();
	poPoint right = view.cross(up).normalize();
	poPoint viewUp = right.cross(view).normalize();

	float matrix[16];
	memset(matrix, 0, sizeof(float)*16);
	matrix[0] = right.x;	matrix[1] = viewUp.x;	matrix[2] = -view.x;
	matrix[4] = right.y;	matrix[5] = viewUp.y;	matrix[6] = -view.y;
	matrix[8] = right.z;	matrix[9] = viewUp.z;	matrix[10] = -view.z;
	matrix[15] = 1;
	
	poPoint xlate;
	xlate.x = matrix[0]*position.x + matrix[1]*position.y + matrix[2]*position.z;
	xlate.y = matrix[4]*position.x + matrix[5]*position.y + matrix[6]*position.z;
	xlate.z = matrix[8]*position.x + matrix[9]*position.y + matrix[10]*position.z;
	
	matrix[12] = -xlate.x;
	matrix[13] = -xlate.y;
	matrix[14] = -xlate.z;
	
	glMatrixMode(GL_MODELVIEW);
	glMultMatrixf(matrix);
}

void poCamera3D::setup(poObject* obj)
{
	glEnable(GL_DEPTH_TEST);
		
	glPushMatrix();
	glViewport(0, 0, poApp::appW, poApp::appH);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (float)poApp::appW/poApp::appH, nearPlane, farPlane);
	
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(position.x, position.y, position.z);
	glRotatef(rotation, rotationAxis.x, rotationAxis.y, rotationAxis.z);
	

}

void poCamera3D::setdown(poObject* obj)
{
	glPopMatrix();
	
	glDisable(GL_DEPTH_TEST);
}