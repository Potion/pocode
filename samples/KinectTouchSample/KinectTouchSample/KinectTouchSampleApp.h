/*	Created by Jonathan Bobrow on 8/30/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class KinectTouchSampleApp : public poObject {
public:
	KinectTouchSampleApp();
	virtual ~KinectTouchSampleApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};