/*	Created by filippo on 2/13/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poLayout.h"

class layoutModifierApp : public poObject {
public:
	layoutModifierApp();
	virtual ~layoutModifierApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
	
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poObject* obj;
	
	int counter;
};

