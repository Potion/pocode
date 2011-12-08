/*	Created by Brooklyn Navy Yard on 7/28/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class ImageBlurApp : public poObject {
public:
	ImageBlurApp();
	virtual ~ImageBlurApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};