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


//------------------------------------------------------------------
//Image Loader Worker
//Loads an image from disk and then returns a poImage,
//Usefull when scrolling quickly/trying to load a lot of images from disk at once
static const std::string ImageLoadingCompleteMessage    = "IMAGE_LOADING_COMPLETE_MESSAGE";
static const std::string ImageLoadSuccessMessage        = "IMAGE_LOAD_SUCCESS_MESSAGE";
static const std::string ImageLoadFailureMessage        = "IMAGE_LOAD_FAILURE_MESSAGE";

class poImageLoaderWorker : public poWorker {
    public:
        poImageLoaderWorker(std::string url);
        virtual ~poImageLoaderWorker();
        
        void workerFunc();
    private:
        std::string url;
};
