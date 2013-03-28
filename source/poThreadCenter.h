#pragma once

/*	Created by Stephen Varga on 11/14/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

namespace po {
    //------------------------------------------------------------------
    //Base Class for all workers
    
    
    //Thread Center
    namespace ThreadCenter {
        class Worker;
        
        void init();
        void shutdown();
        
        void update();
        
        void addItem(Worker *worker, Object *notify, std::string message = "", const Dictionary &dict = Dictionary());
        void workerDone(Worker *threadedObject);
        
        class Worker : public Object {
        public:
            Worker();
            virtual ~Worker();
            
            virtual void workerFunc();
            virtual void setWorkerParams(Object *notify, std::string message, const Dictionary &dict);
            
            void run();
            std::string getWorkerMessage();
            
            
            Object* getNotify();
            void setNotify(po::Object *notify);
            
            Dictionary dict;
            float WorkerStartTime;
            
            bool workerAutoDelete;
            std::string workerMessage;
        private:
            //Worker params/info
            Object *notify;
        };
    } /* End ThreadCenter namespace */
} /* End po namespace */