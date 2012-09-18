/*	Created by Joshua Fisher on 9/11/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class mask_testApp : public poObject {
public:
	mask_testApp();
	virtual ~mask_testApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
	
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};

