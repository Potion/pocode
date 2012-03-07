/*	Created by filippo on 2/24/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poTextBox.h"
#include "poControlPanel.h"

class ControlPanelApp : public poObject {
public:
	ControlPanelApp();
	virtual ~ControlPanelApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
	
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poTextBox* text;
	poOvalShape* shape;
	
	poControlPanel* control;
};

