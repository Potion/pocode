/*	Created by Stephen Varga on 4/30/13.
 *	Copyright 2013 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class RetinaOSXTestApp : public po::Object {
public:
	RetinaOSXTestApp();
	virtual ~RetinaOSXTestApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(po::Event *event);
	
    virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary(), const po::Object *sender=NULL);
};

