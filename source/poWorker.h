//
//  poWorkers.h
//  Threaded Functionality for poCode
//
//  Created by Steve Varga on 8/23/12.
//
//

#pragma once

#include "poThreadCenter.h"

class poThreadCenter;

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
