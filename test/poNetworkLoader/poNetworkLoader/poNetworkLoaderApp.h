/*	Created by Steve Varga on 8/23/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class poNetworkLoaderApp : public poObject {
public:
	poNetworkLoaderApp();
	virtual ~poNetworkLoaderApp();
	
    virtual void update();
    virtual void draw();
    
    virtual void eventHandler(poEvent *event);
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    
    poOvalShape *p;
};

