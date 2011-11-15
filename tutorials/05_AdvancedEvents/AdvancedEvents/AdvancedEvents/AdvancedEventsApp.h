/*	Created by filippo on 11/14/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poTextBox.h"

class AdvancedEventsApp : public poObject {
public:
	AdvancedEventsApp();
	virtual ~AdvancedEventsApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* redRect;
	poRectShape* greenRect;
	poRectShape* blueRect;
	poLineShape* hsvLine;
	
	poPoint	dragOffset;
};