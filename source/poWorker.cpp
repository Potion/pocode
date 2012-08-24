//
//  poWorkers.cpp
//  poNetworkLoader
//
//  Created by Steve Varga on 8/23/12.
//
//

#include "poWorker.h"

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
    poThreadCenter::get()->workerDone(this);
}

//------------------------------------------------------------------
void poWorker::workerFunc() {
    //Override this function for threaded functionality
}

//------------------------------------------------------------------
poObject* poWorker::getWorkerNotify() {
    return notify;
}


//------------------------------------------------------------------
//------------------------------------------------------------------
//Image Loader Worker
#pragma mark poImageLoaderWorker
poImageLoaderWorker::poImageLoaderWorker(std::string url) {
    this->url = url;
}

poImageLoaderWorker::~poImageLoaderWorker() {
}


//------------------------------------------------------------------
void poImageLoaderWorker::workerFunc() {
    std::string status;
    
    //Load image, check for error
    poImage*    image = new poImage(url);
    if(!image->isValid()) {
		delete image; image = NULL;
        status = ImageLoadFailureMessage;
	} else {
        status = ImageLoadSuccessMessage;
    }
    
    workerMessage = ImageLoadingCompleteMessage;
    
    //Set Dictionary
    dict.set("status", status);
    dict.set("image", image);
    dict.set("url", url);
}
