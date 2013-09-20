#include "poHelpers.h"
#include "poThreadCenter.h"

#include <boost/asio.hpp>
#include "boost/thread/locks.hpp"

#include "poURLLoader.h"


using namespace boost::asio;

namespace po {
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: Thread Center =============================
    #pragma mark - ThreadCenter -
    
    namespace ThreadCenter {
        
        //ASIO for Threads
        namespace {
            boost::thread_group threads;
            boost::asio::io_service service;
            boost::asio::io_service::work work(service);

            boost::mutex mtx;
            std::list<Worker *> completed;
        }
        
        
        //------------------------------------------------------------------------
        //Start all threads
        void init() {
            curl_global_init(CURL_GLOBAL_ALL);
            po::URLLoader::initSSL(); //Initialize threadsafe SSL
            
            //Create threads
            for(int i=0; i<4; i++) {
                threads.create_thread(boost::bind(&io_service::run, &service));
            }
        }
        
        
        //------------------------------------------------------------------------
        //Stop all threads, clean up afterward
        void shutdown() {
            service.stop();
            threads.join_all();
            po::URLLoader::finishSSL();
        }
        
        
        //------------------------------------------------------------------------
        //All notification takes place here
        void update() {
            //Gotta lock b/c we're resizing the vector
            boost::lock_guard<boost::mutex> lock(mtx);
            
            while (!completed.empty()) {
                double elapsedTime = po::getElapsedTime() - completed.front()->WorkerStartTime;
                
                if(completed.front()->getNotify() != NULL) {
                    completed.front()->getNotify()->messageHandler(completed.front()->workerMessage, Dictionary()
                                                               .set("worker", completed.front())
                                                               .set("elapsed", (float)elapsedTime)
                                                               .append(completed.front()->dict));
                }
            
                //Most of the time we want to autodelete the worker (asset loaders usually), 
                //so unless the worker says no, do it!
                if(completed.front()->workerAutoDelete) delete completed.front();
                
                completed.pop_front();
            }
        }
        
        
        //------------------------------------------------------------------------
        void threadCenterWorkerFunc(Worker *worker) {
            worker->WorkerStartTime = po::getElapsedTime();
            worker->run();
        }
        
        
        //------------------------------------------------------------------------
        void addItem(Worker *worker, Object *notify, std::string message, const Dictionary &dict) {
            if(!threads.size()) {
                init();
            }
            
            //Store everything we're passing in the worker
            worker->setWorkerParams(notify, message, dict);
            
            //Start threading the worker function
            service.post(boost::bind(threadCenterWorkerFunc, worker));
        }
        
        
        //------------------------------------------------------------------------
        void workerDone(Worker *worker) {
            //Gotta lock b/c we're resizing the vector
            boost::lock_guard<boost::mutex> lock(mtx);
            
            //Add to queue, don't notify from here because it is in the thread!
            completed.push_back(worker);
        }
    } /* End ThreadCenter namespace */


    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: Worker ====================================
    #pragma mark - Worker -
    
    ThreadCenter::Worker::Worker() {
        workerAutoDelete = true;
    }

    ThreadCenter::Worker::~Worker() {
    }

    //------------------------------------------------------------------
    void ThreadCenter::Worker::setWorkerParams(Object *notify, std::string message, const Dictionary &dict) {
        this->notify = notify;
        this->workerMessage = message;
        this->dict.append(dict);
    }

    
    //------------------------------------------------------------------
    void ThreadCenter::Worker::run() {
        workerFunc();
        ThreadCenter::workerDone(this);
    }

    
    //------------------------------------------------------------------
    void ThreadCenter::Worker::workerFunc() {
        //Override this function for threaded functionality
    }

    
    //------------------------------------------------------------------
    Object* ThreadCenter::Worker::getNotify() {
        return notify;
    }
    
    
    //------------------------------------------------------------------
    void ThreadCenter::Worker::setNotify(po::Object *notify) {
        this->notify = notify;
    }
} /* End po namespace */
