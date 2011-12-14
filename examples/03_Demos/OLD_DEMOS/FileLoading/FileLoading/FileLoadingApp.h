/*	Created by Stephen Varga on 9/16/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

class FileLoadingApp : public poObject {
public:
	FileLoadingApp();
	virtual ~FileLoadingApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};