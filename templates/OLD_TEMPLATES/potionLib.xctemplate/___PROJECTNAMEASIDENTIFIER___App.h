/*	Created by ___FULLUSERNAME___ on ___DATE___.
 *	Copyright ___YEAR___ ___ORGANIZATIONNAME___. All rights reserved.
 */

#include "poObject.h"

class ___PROJECTNAMEASIDENTIFIER___App : public poObject {
public:
	___PROJECTNAMEASIDENTIFIER___App();
	virtual ~___PROJECTNAMEASIDENTIFIER___App();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};