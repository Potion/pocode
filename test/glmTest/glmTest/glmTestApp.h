/*	Created by Joshua Fisher on 7/29/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poVBO.h"
#include "poObject.h"

class glmTestApp : public poObject {
public:
	glmTestApp();
	void draw();

	// vbos[0] = a square
	// vbos[1] = the camera track
	poVBO vbos[2];
	
	float rot;
	std::vector<poPoint> track;
};