/*	Created by Steve Varga on 9/5/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class poAsyncImageLoaderExampleApp : public poObject {
public:
	poAsyncImageLoaderExampleApp();
	virtual ~poAsyncImageLoaderExampleApp();
	
    virtual void update();
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    
    poRectShape *imageHolder;
};

