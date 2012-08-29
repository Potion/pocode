#pragma once

/*	Created by Stephen Varga on 11/14/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "poThreadCenter.h"

class poThreadCenter;

class poThreadCenterWorker : public poObject
{
public:
	poThreadCenterWorker();
	virtual ~poThreadCenterWorker();
	
    virtual void workerFunc();
    virtual void setWorkerParams(poObject *notify, std::string message, const poDictionary &dict);
    
    void run();
    
    poObject*   getWorkerNotify();
    std::string getWorkerMessage();
    
    bool workerShouldBeDeleted();
    
    poDictionary dict;
    float poThreadCenterWorkerStartTime;
    
    bool workerAutoDelete;
    std::string workerMessage;
private:
    //Worker params/info
    poObject *notify;
};