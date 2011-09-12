#pragma once

/*	Created by Jonathan Bobrow on 8/9/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "TextLayout.h"

class flyingText : public poObject
{
public:
	flyingText(std::string text, int width, float start = 300.f, float end = 0.f, float del = .5f, float dur = 3.f, float shift = 20.f);
	virtual ~flyingText();
	
	virtual void draw();
	virtual void eventHandler(poEvent *event);
	
	void reset();
	void fly();
	void fadeTo(float alpha);
	
	TextBoxLayout layout;
	
	std::vector< float > offsets;
	std::vector< poTween<float> > tweens;
	
	float duration, delay;
	float h_shift;
	float start_y, end_y;
	float oVal;
};