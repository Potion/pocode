#pragma once

/*	Created by Stephen Varga on 11/14/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */



#include <boost/asio.hpp>
#include "boost/thread/locks.hpp"

#include "poObject.h"

class poWorker;

//Thread Center
class poThreadCenter {
public:
	static poThreadCenter* get();
    
	void update();
    void addItem(poWorker *worker, poObject *notify, std::string message = "", const poDictionary &dict = poDictionary());
    
	void workerDone(poWorker *threadedObject);
    
private:
	poThreadCenter();
	virtual ~poThreadCenter();
    
	boost::thread_group threads;
	boost::asio::io_service service;
	boost::asio::io_service::work work;
    
    boost::mutex mtx;
	std::list<poWorker *> completed;
    
    static poThreadCenter* pThreadCenter;
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
