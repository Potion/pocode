/*	Created by Steve Varga on 3/1/12.
 *	Copyright 2012 Vargatron. All rights reserved.
 */

#include "poObject.h"
#include "Square.h"

class poMessageCenterExampleApp : public poObject {
public:
	poMessageCenterExampleApp();
	virtual ~poMessageCenterExampleApp();
	
    virtual void update();
    virtual void draw();
    virtual void eventHandler(poEvent *event);
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    
    Square* s;
};

