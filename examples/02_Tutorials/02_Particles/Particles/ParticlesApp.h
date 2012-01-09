/*	Created by Potion on 12/1/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class ParticlesApp : public poObject {
public:
	ParticlesApp();
	virtual ~ParticlesApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};