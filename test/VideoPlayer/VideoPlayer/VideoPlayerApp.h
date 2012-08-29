/*	Created by Joshua Fisher on 8/8/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class VideoPlayerApp : public poObject {
public:
	VideoPlayerApp();
	virtual ~VideoPlayerApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
	
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};

