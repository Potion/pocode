#pragma once

/*	Created by Stephen Varga on 11/14/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "poWorker.h"

#include <boost/asio.hpp>

#include "boost/thread/locks.hpp"
class poWorker;

class poThreadCenter : public poObject {
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