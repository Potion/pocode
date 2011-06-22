//
//  poGUI.h
//  poFramework4
//
//  Created by Joshua Fisher on 6/22/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poObject.h"

typedef boost::function<void(float)> FloatCallback;
typedef boost::function<void(poPoint)> PointCallback;

class poGUISlider : public poObject {
public:
	poGUISlider(const std::string &n, float start, float min, float max, float step, FloatCallback cb);	

	// [min .. max]
	float value() const;
	void value(float f);
	std::string valueStr() const;
	
	void draw();
	void eventHandler(poEvent *event);
	
private:
	float _value, min, max, step;
	FloatCallback callback;
};

class poGUIPointer : public poObject {
public:
	poGUIPointer(const std::string &n, poPoint start, poRect rect, PointCallback cb);
	
	poPoint value() const;
	void value(poPoint p);
	std::string valueStr() const;
	
	void draw();
	void eventHandler(poEvent *event);
	
private:
	void setup(const std::string &n, poPoint start, poRect rect, float zmin, float zmax, PointCallback cb);
	// will map from screen to rect range
	void setXY(float x, float y);
	// will map from rect range to screen
	void getXY(float *x, float *y);

	poPoint _value;
	poRect rect;
	PointCallback callback;
};
