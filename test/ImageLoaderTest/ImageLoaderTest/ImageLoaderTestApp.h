/*	Created by Stephen Varga on 9/4/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class ImageLoaderTestApp : public poObject {
public:
	ImageLoaderTestApp();
	virtual ~ImageLoaderTestApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
	
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};

