/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "poMatrixStack.h"
#include "poApplication.h"

using namespace glm;


poMatrixStack::poMatrixStack() {
	projection.push(ortho<float>(-1, 1, -1, 1, -1, 1));
	modelview.push(mat4());
	viewport.push(getWindowBounds());
}

void poMatrixStack::pushModelview()	{
	modelview.push(modelview.top());
}

void poMatrixStack::pushModelview(const mat4 &mat) {
	modelview.push(mat);
}

void poMatrixStack::pushProjection() {
	projection.push(projection.top());
}

void poMatrixStack::pushProjection(const mat4 &mat) {
	projection.push(mat);
}

void poMatrixStack::pushViewport() {
	viewport.push(viewport.top());
}

void poMatrixStack::pushViewport(poRect r) {
	viewport.push(r);
	glViewport(r.x, r.y, r.width, r.height);
}

// restore the previous one
void poMatrixStack::popModelview() {
	modelview.pop();
}

void poMatrixStack::popProjection()	{
	projection.pop();
}

void poMatrixStack::popViewport() {
	viewport.pop(); 

	poRect vp = viewport.top();
	glViewport(vp.x, vp.y, vp.width/poGetScale(), vp.height/poGetScale()); //Viewport size needs to be divided by the scale, pushing it scaled in camera 
}

// get the current top
glm::mat4 poMatrixStack::getModelview()	{
	return modelview.top();
}

glm::mat4 poMatrixStack::getProjection(){
	return projection.top();
}

poRect poMatrixStack::getViewport() {
	return viewport.top();
}

glm::mat4 poMatrixStack::transformation() {
	return projection.top() * modelview.top();
}

// modify the top modelview
void poMatrixStack::translate(poPoint t) {
	modelview.top() *= glm::translate(mat4(), vec3(t.x,t.y,t.z));
}

void poMatrixStack::rotate(float r, poPoint axis) {
	modelview.top() *= glm::rotate(mat4(), r, vec3(axis.x,axis.y,axis.z));
}

void poMatrixStack::scale(poPoint s) {
	modelview.top() *= glm::scale(mat4(), vec3(s.x,s.y,s.z));
}


