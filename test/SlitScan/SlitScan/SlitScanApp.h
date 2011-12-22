/*	Created by Jonathan Bobrow on 12/18/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class SlitScanApp : public poObject {
public:
	SlitScanApp();
	virtual ~SlitScanApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};