/*	Created by Stephen Varga on 9/18/13.
 *	Copyright 2013 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class RichtTextCachingApp : public po::Object {
public:
	RichtTextCachingApp();
	virtual ~RichtTextCachingApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(po::Event *event);
	
    virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary(), const po::Object *sender=NULL);
};

