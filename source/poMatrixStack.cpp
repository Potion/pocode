#include "poMatrixStack.h"
#include "poApplication.h"

using namespace glm;


poMatrixStack::poMatrixStack() {
	projection.push(ortho<float>(-1, 1, -1, 1, -1, 1));
	modelview.push(mat4());
	viewport.push(getWindowBounds());
	load();
	
	glEnable(GL_SCISSOR_TEST);
}

void poMatrixStack::pushModelview()		{modelview.push(modelview.top()); load();}
void poMatrixStack::pushModelview(const mat4 &mat) {modelview.push(mat); load();}
void poMatrixStack::pushProjection()	{projection.push(projection.top()); load();}
void poMatrixStack::pushProjection(const mat4 &mat) {projection.push(mat); load();}
void poMatrixStack::pushViewport(poRect r) {viewport.push(r); load();}

// restore the previous one
void poMatrixStack::popModelview()		{modelview.pop(); load();}
void poMatrixStack::popProjection()		{projection.pop(); load();}
void poMatrixStack::popViewport()		{viewport.pop(); load();}

// get the current top
glm::mat4 poMatrixStack::getModelview()	{return modelview.top();}
glm::mat4 poMatrixStack::getProjection(){return projection.top();}
poRect poMatrixStack::getViewport()		{return viewport.top();}

glm::mat4 poMatrixStack::transformation() {return projection.top() * modelview.top();}

// set it to opengl
void poMatrixStack::load() {
	poRect vp = viewport.top();

	glScissor(vp.x, vp.y, vp.width, vp.height);
	glViewport(vp.x, vp.y, vp.width, vp.height);
}

// modify the top modelview
void poMatrixStack::translate(poPoint t) {
	modelview.top() *= glm::translate(mat4(), vec3(t.x,t.y,t.z));
	load();
}

void poMatrixStack::rotate(float r, poPoint axis) {
	modelview.top() *= glm::rotate(mat4(), r, vec3(axis.x,axis.y,axis.z));
	load();
}

void poMatrixStack::scale(poPoint s) {
	modelview.top() *= glm::scale(mat4(), vec3(s.x,s.y,s.z));
	load();
}


