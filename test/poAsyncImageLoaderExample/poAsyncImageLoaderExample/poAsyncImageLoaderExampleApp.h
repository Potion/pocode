/*	Created by Steve Varga on 9/5/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class poAsyncImageLoaderExampleApp : public po::Object {
public:
	poAsyncImageLoaderExampleApp();
	virtual ~poAsyncImageLoaderExampleApp();
	
    virtual void update();
    virtual void draw();
	
    virtual void eventHandler(po::Event *event);
    virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary());
    
    po::RectShape *imageHolder;
};

