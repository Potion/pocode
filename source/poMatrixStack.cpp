#include "poMatrixStack.h"
#include "poApplication.h"

using namespace glm;

static boost::thread_specific_ptr<poMatrixStack> instance;

poMatrixStack *poMatrixStack::get() {
	if(!instance.get())
		instance.reset(new poMatrixStack());
	return instance.get();
}

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
	glScissor(vp.origin.x, vp.origin.y, vp.size.x, vp.size.y);
	glViewport(vp.origin.x, vp.origin.y, vp.size.x, vp.size.y);
//	glLoadMatrixf(value_ptr(projection.top() * modelview.top()));
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


