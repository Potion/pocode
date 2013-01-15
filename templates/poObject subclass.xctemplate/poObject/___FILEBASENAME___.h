#pragma once

/*	Created by ___FULLUSERNAME___ on ___DATE___.
 *	Copyright ___YEAR___ ___ORGANIZATIONNAME___. All rights reserved.
 */

#include "poObject.h"

class ___FILEBASENAME___ : public po::Object
{
public:
	___FILEBASENAME___();
	virtual ~___FILEBASENAME___();
	
	virtual void update();
	virtual void eventHandler(po::Event *event);
	virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary(), const po::Object *sender=NULL);
};