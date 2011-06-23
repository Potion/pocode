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

template <typename T>
T map(T min1, T max1, T v, T min2, T max2) {
	float tmp = (v-min1) / (max1-min1);
	return tmp * (max2-min2) + min2;
}

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
	
	poFont font("Helvetica", 11);
	
	poTextBox *tb = new poTextBox(GUI_width,15);
	tb->textAlignment(PO_ALIGN_TOP_LEFT)
		.font(PO_FONT_REGULAR, &font)
		.text(n)
		.layout();
	addChild(tb);
	
	tb = new poTextBox(GUI_width,15);
	tb->textAlignment(PO_ALIGN_BOTTOM_RIGHT)
		.font(PO_FONT_REGULAR, &font)
		.text(valueStr())
		.layout();
	addChild(tb);

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
	filler.size.x *= map(min,max,value(),0.f,1.f);
	
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
			value(map(0.f,1.f,pct,min,max));

			poTextBox *tb = getChildAs<poTextBox>(this, 1);
			tb->text(valueStr()).layout();
			
			callback(value());
			break;
		}
	}
}



#pragma mark - poGUIPoint

poGUIPointer::poGUIPointer(const std::string &n, poPoint start, poRect rect, PointCallback cb)
:	_value(start)
,	rect(rect)
,	callback(cb)
{
	name(n);
	bounds(poRect(0,0,GUI_width,GUI_width));
	
	addEvent(PO_MOUSE_PRESS_EVENT, this);
	addEvent(PO_MOUSE_DRAG_EVENT, this);
}

poPoint poGUIPointer::value() const {return _value;}
void poGUIPointer::value(poPoint p) {_value = p;}
std::string poGUIPointer::valueStr() const {return _value.toString();}

void poGUIPointer::draw() {
	applyColor(GUI_fill);
	
	// draw the grid
	for(int i=0; i<10; i++) {
	}
	
	poPoint unmapped;
	getXY(&unmapped.x, &unmapped.y);
	drawRect(poRect(unmapped-2, poPoint(4,4)));
	
	applyColor(GUI_stroke);
	drawStroke(bounds());
}

void poGUIPointer::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_MOUSE_PRESS_EVENT:
		case PO_MOUSE_DRAG_EVENT:
		{
			float x = clamp(0.f,bounds().width(),event->local_position.x);
			float y = clamp(0.f,bounds().height(),event->local_position.y);
			
			setXY(x,y);
			callback(value());
			break;
		}
	}
}

void poGUIPointer::setXY(float x, float y) {
	poRect b = bounds();
	poPoint ll = rect.topLeft();
	poPoint ur = rect.bottomRight();

	x = map(0.f,b.width(), x, ll.x,ur.x);
	y = map(0.f,b.height(), y, ll.y,ur.y);

	_value.x = x;
	_value.y = y;
}

void poGUIPointer::getXY(float *x, float *y) {
	poPoint p = unmapPoint(_value);
	*x = p.x;
	*y = p.y;
}

poPoint poGUIPointer::unmapPoint(poPoint p) {
	poRect b = bounds();
	poPoint ll = rect.topLeft();
	poPoint ur = rect.bottomRight();
	
	poPoint pt;
	pt.x = map(ll.x,ur.x, p.x, 0.f,b.width());
	pt.y = map(ll.y,ur.y, p.y, 0.f,b.height());
	return pt;
}


