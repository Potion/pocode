#include "poHelpers.h"
#include "poThreadCenter.h"

#include <boost/asio.hpp>
#include "boost/thread/locks.hpp"

using namespace boost::asio;

//------------------------------------------------------------------
//------------------------------------------------------------------
//Thread Center
#pragma mark poThreadCenter
namespace poThreadCenter {
    //ASIO for Threads
    namespace {
        boost::thread_group threads;
        boost::asio::io_service service;
        boost::asio::io_service::work work(service);

        boost::mutex mtx;
        std::list<poWorker *> completed;
    }

    //------------------------------------------------------------------
    //Start all threads
    void init() {
        //Create threads
        for(int i=0; i<10; i++) {
            threads.create_thread(boost::bind(&io_service::run, &service));
        }
    }
    
    //------------------------------------------------------------------
    //Stop all threads, clean up afterward
    void shutdown() {
        service.stop();
        threads.join_all();
    }


    //------------------------------------------------------------------
    //All notification takes place here
    void update() {
        //Gotta lock b/c we're resizing the vector
        boost::lock_guard<boost::mutex> lock(mtx);
        
        while (!completed.empty()) {
            double elapsedTime = poGetElapsedTime() - completed.front()->poWorkerStartTime;
            completed.front()->getWorkerNotify()->messageHandler(completed.front()->workerMessage, poDictionary()
                                                       .set("worker", completed.front())
                                                       .set("elapsed", (float)elapsedTime)
                                                       .append(completed.front()->dict));
            
            //Most of the time we want to autodelete the worker (asset loaders usually), 
            //so unless the worker says no, do it!
            if(completed.front()->workerAutoDelete) delete completed.front();
            
            completed.pop_front();
        }
    }

    //------------------------------------------------------------------
    void threadCenterWorkerFunc(poWorker *worker) {
        worker->poWorkerStartTime = poGetElapsedTime();
        worker->run();
    }
    
    //------------------------------------------------------------------
    void poThreadCenter::addItem(poWorker *worker, poObject *notify, std::string message, const poDictionary &dict) {
        if(!threads.size()) {
            init();
        }
        
        //Store everything we're passing in the worker
        worker->setWorkerParams(notify, message, dict);
        
        //Start threading the worker function
        service.post(boost::bind(threadCenterWorkerFunc, worker));
    }


    //------------------------------------------------------------------
    void poThreadCenter::workerDone(poWorker *worker) {
        //Gotta lock b/c we're resizing the vector
        boost::lock_guard<boost::mutex> lock(mtx);
        
        //Add to queue, don't notify from here because it is in the thread!
        completed.push_back(worker);
    }
}


//------------------------------------------------------------------
//------------------------------------------------------------------
//Base Worker Class
#pragma mark poWorker
poWorker::poWorker() {
    workerAutoDelete = true;
}

poWorker::~poWorker() {
}

//------------------------------------------------------------------
void poWorker::setWorkerParams(poObject *notify, std::string message, const poDictionary &dict) {
    this->notify = notify;
    this->workerMessage = message;
    this->dict.append(dict);
}

//------------------------------------------------------------------
void poWorker::run() {
    workerFunc();
    poThreadCenter::workerDone(this);
}

//------------------------------------------------------------------
void poWorker::workerFunc() {
    //Override this function for threaded functionality
}

//------------------------------------------------------------------
poObject* poWorker::getWorkerNotify() {
    return notify;
}