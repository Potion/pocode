#pragma once

/*	Created by Stephen Varga on 11/14/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class poWorker;

//Thread Center
namespace poThreadCenter {
    void init();
    void shutdown();
    
	void update();
    
    void addItem(poWorker *worker, poObject *notify, std::string message = "", const poDictionary &dict = poDictionary());
	void workerDone(poWorker *threadedObject);
};




//------------------------------------------------------------------
//Base Class for all workers
class poWorker : public poObject {
public:
	poWorker();
	virtual ~poWorker();
	
    virtual void workerFunc();
    virtual void setWorkerParams(poObject *notify, std::string message, const poDictionary &dict);
    
    void run();
    
    poObject*   getWorkerNotify();
    std::string getWorkerMessage();
    
    bool workerShouldBeDeleted();
    
    poDictionary dict;
    float poWorkerStartTime;
    
    bool workerAutoDelete;
    std::string workerMessage;
private:
    //Worker params/info
    poObject *notify;
};
