//
//  poTextLayout.h
//  poFramework4
//
//  Created by Joshua Fisher on 5/28/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poFont.h"
#include "poRect.h"
#include "poTexture.h"

class poTextLayout {
public:
	poTextLayout();
	virtual ~poTextLayout();
	
	// set these 
	std::string text;
	poFontMap *fonts;
	// call this
	virtual void render() = 0;
	// these are now set
	std::string stripped;
	poTexture *rendered;
	poRect actualBounds;
	
protected:
	void stage1();
	void *attrib;

private:
	void cleanup();
	TiXmlDocument *doc;
	std::vector<void*> userd;
};

class poBoxLayout 
:	public poTextLayout 
{
public:
	poBoxLayout();
	virtual ~poBoxLayout();
	
	// 1
	poRect bounds;
	// 2
	virtual void render();
	// 3
	poPoint baseline;
	
private:
	void cleanup();
	void *frame;
};

class poCurveLayout
:	public poTextLayout
{
public:
	struct Helper {
		float angle;
		poRect bounds;
	};
	
	poCurveLayout();
	virtual ~poCurveLayout();
	
	// 1
	std::vector<poPoint> curve;
	// 2
	virtual void render();
	// 3
	
private:
	std::vector<Helper> layout;
};