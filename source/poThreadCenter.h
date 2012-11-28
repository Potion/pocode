#pragma once

/*	Created by Stephen Varga on 11/14/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

class Worker;

namespace po {
    //Thread Center
    namespace ThreadCenter {
        void init();
        void shutdown();
        
        void update();
        
        void addItem(Worker *worker, Object *notify, std::string message = "", const Dictionary &dict = Dictionary());
        void workerDone(Worker *threadedObject);
    } /* End ThreadCenter namespace */




    //------------------------------------------------------------------
    //Base Class for all workers
    class Worker : public Object {
    public:
        Worker();
        virtual ~Worker();
        
        virtual void workerFunc();
        virtual void setWorkerParams(Object *notify, std::string message, const Dictionary &dict);
        
        void run();
        
        Object*   getWorkerNotify();
        std::string getWorkerMessage();
        
        bool workerShouldBeDeleted();
        
        Dictionary dict;
        float WorkerStartTime;
        
        bool workerAutoDelete;
        std::string workerMessage;
    private:
        //Worker params/info
        Object *notify;
    };
} /* End po namespace */