#pragma once

/*	Created by ___FULLUSERNAME___ on ___DATE___.
 *	Copyright ___YEAR___ ___ORGANIZATIONNAME___. All rights reserved.
 */

#include "poShape2D.h"

class ___FILEBASENAME___ : public poShape2D
{
public:
	___FILEBASENAME___();
	virtual ~___FILEBASENAME___();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};