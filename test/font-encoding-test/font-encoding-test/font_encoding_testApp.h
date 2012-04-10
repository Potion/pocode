/*	Created by Joshua Fisher on 4/10/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class font_encoding_testApp : public poObject {
public:
	font_encoding_testApp();
	virtual ~font_encoding_testApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
	
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};

