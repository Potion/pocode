//
//  Dot.h
//  Scribble
//
//  Created by Jonathan Bobrow on 8/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Scribble_Dot_h
#define Scribble_Dot_h

#define EASE 0.1

class Dot{
	/*
	 *  class to create dots
	 */
	
public:
	Dot(int _id, float _r, float _x, float _y);
	void display();
	void update(float xTarget, float yTarget);
	
	int id;
	float r, x, y, d;
	
};


#endif
