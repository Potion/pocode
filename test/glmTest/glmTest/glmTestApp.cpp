#include "glmTestApp.h"

#include "poCamera.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"

#include "poMatrixStack.h"

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace glm;
using namespace boost::assign;

// max track points
static const int NUM_POINTS = 100;
// the distance of the camera from the center
static const float radius = 10.f;

poObject *createObjectForID(uint uid) {
	return new glmTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "glmTestApp", 100, 100, 800, 800);
}

void cleanupApplication() {
}

glmTestApp::glmTestApp()
:	rot(0)
{
	// the points of the square
	vector<poPoint> square = list_of(poPoint(-1,-1))(poPoint(1,-1))(poPoint(1,1))(poPoint(-1,1));
	vector<poColor>	colors = list_of(poColor::red)(poColor::blue)(poColor::dk_grey)(poColor::magenta);

	int stride = sizeof(poPoint) + sizeof(poColor);
	
	// load in the first vbo
	vbos[0].bind();
	vbos[0].bufferData(stride*4, NULL, GL_STATIC_DRAW);
	
	char *ptr = (char*)vbos[0].map(GL_WRITE_ONLY);
	for(int i=0; i<4; i++) {
		*(poPoint*)(ptr + i*stride)					= square[i];
		*(poColor*)(ptr + i*stride+sizeof(poPoint))	= colors[i];
	}
	vbos[0].unmap();

	// and reserve space for the second one
	vbos[1].bind();
	vbos[1].bufferData(sizeof(poPoint)*NUM_POINTS, NULL, GL_STREAM_DRAW);
}

void glmTestApp::draw() {
	// grab the pointer to the matrix stack
	poMatrixStack *stack = poMatrixStack::get();

	// CLEAR IT
	glClear(GL_COLOR_BUFFER_BIT);

	// the camera position
	vec3 eye(cos(rot)*radius, sin(rot*2.f)*2.f, sin(rot)*radius);
	// rotate the camera around a bit
	rot += (M_PI/180.f);

	// save the track so we can draw it later
	track.push_back(poPoint(eye.x, eye.y, eye.z));
	// cap the track to <= NUM_POINTS
	if(track.size() > NUM_POINTS)
		track.erase(track.begin());
	
	// the stock projection matrix, we'll keep the same aspect ratio
	stack->pushProjection(perspective(65.f, getWindowAspect(), 0.01f, 100.f));
	// define the camera transform (eye, center, up)
	stack->pushModelview(lookAt(eye, vec3(0,0,0), vec3(0,1,0)));
	stack->rotate(rot*180/M_PI, poPoint(1,0,0));

	// make sure we can write vertexs
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	// bind the square
	vbos[0].bind();
	// and tell the card: element size, type size, stride, start
	int stride = sizeof(poPoint) + sizeof(poColor);
	glVertexPointer(3, GL_FLOAT, stride, 0);
	glColorPointer(4, GL_FLOAT, stride, (void*)sizeof(poPoint));

	glColor3f(1,1,1);
	// draw the square in the middle
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	stack->popModelview();
	
	// NOW TIME FOR THE SUB!WINDOW!
	
	// figure out the placement
	float hw = getWindowWidth() / 4.f;
	float hh = getWindowHeight() / 4.f;
	float x = getWindowWidth() - hw;
	float y = getWindowHeight() - hh;

	// redefine the viewport
	stack->pushViewport(poRect(x,y,hw,hh));
	// and clear it out
	glClear(GL_COLOR_BUFFER_BIT);

	// lets draw a square around the window, kind of an overlay
	stack->pushModelview(ortho<float>(-1, 1, -1, 1, -1, 1));
	// draw it using the SAME SQUARE
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	stack->popModelview();

	// then set up a camera similiar to the first but further away so we can see the first camera
	stack->pushModelview(lookAt(vec3(0,10,25), vec3(0,0,0), vec3(0,1,0)));

	stack->pushModelview();
	stack->rotate(rot*180/M_PI, poPoint(1,0,0));
	// DRAW IT
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	stack->popModelview();
	glDisableClientState(GL_COLOR_ARRAY);
	
	glColor3f(.5,.5,.5);
	// bind the track vbo
	vbos[1].bind();
	
	// update the points in there
	vbos[1].bufferSubData(0, sizeof(poPoint)*track.size(), &track[0]);

	// alternatively 
//	void *points = vbos[1].map(GL_WRITE_ONLY);
//	memcpy(points, &track[0], sizeof(poPoint)*track.size());
//	vbos[1].unmap();
	
	// you've to set the write pointer
	glVertexPointer(3, GL_FLOAT, 0, 0);
	// then draw however many we've got
	glDrawArrays(GL_LINE_STRIP, 0, track.size());

	stack->popModelview();
	stack->popProjection();
	stack->popViewport();

	// and be tidy
	vbos[1].unbind();
	glDisableClientState(GL_VERTEX_ARRAY);
}
