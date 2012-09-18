/*	Created by Joshua Fisher on 9/5/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class fontTestApp : public poObject {
public:
	fontTestApp();
	virtual ~fontTestApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
	
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};

