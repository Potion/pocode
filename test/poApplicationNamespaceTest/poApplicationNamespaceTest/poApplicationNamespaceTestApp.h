/*	Created by Steve Varga on 9/14/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class poApplicationNamespaceTestApp : public poObject {
public:
	poApplicationNamespaceTestApp();
	virtual ~poApplicationNamespaceTestApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
	
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};

