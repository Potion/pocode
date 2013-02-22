/*	Created by ___FULLUSERNAME___ on ___DATE___.
 *	Copyright ___YEAR___ ___ORGANIZATIONNAME___. All rights reserved.
 */

#include "poObject.h"

class ___PROJECTNAMEASIDENTIFIER___App : public po::Object {
public:
	___PROJECTNAMEASIDENTIFIER___App();
	virtual ~___PROJECTNAMEASIDENTIFIER___App();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(po::Event *event);
	
    virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary(), const po::Object *sender=NULL);
};

