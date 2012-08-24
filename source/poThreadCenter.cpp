#include "poHelpers.h"
#include "poThreadCenter.h"

using namespace boost::asio;

//Singleton Instance/Get Instance
poThreadCenter* poThreadCenter::pThreadCenter=NULL;

//------------------------------------------------------------------
poThreadCenter* poThreadCenter::get() {
	if(pThreadCenter==NULL) {
		pThreadCenter = new poThreadCenter();
	}
    
	return pThreadCenter;
}

//------------------------------------------------------------------
poThreadCenter::poThreadCenter() 
: work(service) {
    //Create threads
    for(int i=0; i<10; i++) {
		threads.create_thread(boost::bind(&io_service::run, &service));
	}
}

poThreadCenter::~poThreadCenter() {
    service.stop();
	threads.join_all();
}


//------------------------------------------------------------------
//All notification takes place here
void poThreadCenter::update() {
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