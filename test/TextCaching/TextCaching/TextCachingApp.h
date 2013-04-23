/*	Created by Joshua Fisher on 10/18/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class TextCachingApp : public po::Object {
public:
	TextCachingApp();
	virtual ~TextCachingApp();
	
	virtual void eventHandler(po::Event *event);
	virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary());
};