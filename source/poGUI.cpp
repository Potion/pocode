//
//  poGUI.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/22/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poGUI.h"

#include "poTextBox.h"
#include "poSimpleDrawing.h"

#pragma mark Helpers and globals

const poColor GUI_stroke(.5,.5,.5);
const poColor GUI_fill(.8,.8,.8);
const float GUI_width = 100;

poPoint operator-(poPoint p, float val) {
	return poPoint(p.x-val, p.y-val);
}

#pragma mark - poGUISlider

poGUISlider::poGUISlider(const std::string &n, float start, float min, float max, float step, boost::function<void(float)> cb)
:	callback(cb)
,	min(min)
,	max(max)
,	step(step)
{ 
	name(n);
	bounds(poRect(0,0,GUI_width,15));
	value(start);
	
	addEvent(PO_MOUSE_PRESS_EVENT, this);
	addEvent(PO_MOUSE_DRAG_EVENT, this);
}

float poGUISlider::value() const {return _value;}
void poGUISlider::value(float f) {_value = clamp(min, max, f);}

std::string poGUISlider::valueStr() const {
	return (boost::format("%.2f")%(value())).str();
}

void poGUISlider::draw() { 
	poRect filler = bounds();
	filler.size.x *= poMapf(min,max,value(),0.f,1.f);
	
	applyColor(GUI_fill);
	drawRect(filler);
	
	applyColor(GUI_stroke);
	drawStroke(bounds());
}

void poGUISlider::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_MOUSE_PRESS_EVENT:
		case PO_MOUSE_DRAG_EVENT:
		{
			float pct = clamp(0.f, bounds().width(), event->local_position.x) / bounds().width();
			value(poMapf(0.f,1.f,pct,min,max));
			
			callback(value());
			break;
		}
	}
}



#pragma mark - poGUIPoint

poGUIPointer::poGUIPointer(const std::string &n, poPoint start, poRect rect, PointCallback cb)
:	_value(start)
,	callback(cb)
,	rect(rect)
,	pad(new poObject())
,	slider(NULL)
{
	name(n);
	bounds(poRect(0,0,GUI_width,GUI_width-20));

	pad->bounds(poRect(0,0,GUI_width-20,GUI_width-20));
	pad->addEvent(PO_MOUSE_PRESS_EVENT, this);
	pad->addEvent(PO_MOUSE_DRAG_EVENT, this);
	addChild(pad);
}

poGUIPointer::poGUIPointer(const std::string &n, poPoint start, poRect rect, float zmin, float zmax, PointCallback cb) 
:	_value(start)
,	callback(cb)
,	rect(rect)
,	pad(new poObject())
,	slider(new poObject())
,	zmin(zmin)
,	zmax(zmax)
{
	name(n);
	bounds(poRect(0,0,GUI_width,GUI_width-20));

	pad->bounds(poRect(0,0,GUI_width-20,GUI_width-20));
	pad->addEvent(PO_MOUSE_PRESS_EVENT, this);
	pad->addEvent(PO_MOUSE_DRAG_EVENT, this);
	addChild(pad);
	
	slider->bounds(poRect(GUI_width-15,0,15,GUI_width-20));
	slider->addEvent(PO_MOUSE_PRESS_EVENT, this);
	slider->addEvent(PO_MOUSE_DRAG_EVENT, this);
	addChild(slider);
}

poPoint poGUIPointer::value() const {return _value;}
void poGUIPointer::value(poPoint p) {_value = p;}
std::string poGUIPointer::valueStr() const {return _value.toString();}

void poGUIPointer::draw() {
	poPoint unmapped;
	getXY(&unmapped.x, &unmapped.y);

	applyColor(GUI_fill);
	drawRect(poRect(unmapped-2, poPoint(4,4)));
	
	applyColor(GUI_stroke);
	drawStroke(pad->bounds());
	
	if(slider) {
		poRect b = slider->bounds();
		float h = getZ();
		
		applyColor(GUI_fill);
		drawRect(poRect(b.origin.x, b.height()-h, 15, h));
		
		applyColor(GUI_stroke);
		drawStroke(b);
	}
}

void poGUIPointer::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_MOUSE_PRESS_EVENT:
		case PO_MOUSE_DRAG_EVENT:
		{
			if(event->source == pad) {
				float x = clamp(0.f,pad->bounds().width(),event->local_position.x);
				float y = clamp(0.f,pad->bounds().height(),event->local_position.y);
				
				setXY(x,y);
				callback(value());
			}
			else if(event->source == slider) {
				poRect b = slider->bounds();
				float z = b.height() - clamp(b.origin.y,b.origin.y+b.size.y,event->local_position.y);

				setZ(z);
				callback(value());
			}
			break;
		}
	}
}

void poGUIPointer::setXY(float x, float y) {
	poPoint p = rect.remap(pad->bounds(), poPoint(x,y));
	_value.x = p.x;
	_value.y = p.y;
}

void poGUIPointer::getXY(float *x, float *y) {
	poPoint p = pad->bounds().remap(rect, _value);
	*x = p.x;
	*y = p.y;
}

void poGUIPointer::setZ(float z) {
	poRect b = slider->bounds();
	_value.z = poMapf(b.origin.y,b.origin.y+b.size.y, z, zmin, zmax);
}

float poGUIPointer::getZ() {
	poRect b = slider->bounds();
	return poMapf(zmin, zmax, _value.z, b.origin.y,b.origin.y+b.size.y);
}

