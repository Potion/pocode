/*	Created by Steve Varga on 4/10/12.
 *	Copyright 2012 Vargatron. All rights reserved.
 */

#include "poObject.h"

class RichTextExampleApp : public poObject {
public:
	RichTextExampleApp();
	virtual ~RichTextExampleApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
	
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};

