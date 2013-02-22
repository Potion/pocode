#include "poMessageCenter.h"
#include "poObject.h"

typedef struct {
    poObject* sender;
    std::string message;
    poDictionary dict;
} poMessage;

typedef struct {
    poObject* sender;
    poObject* subscriber;
} poMessageSubscriber;


namespace poMessageCenter {
    namespace {
        std::map<std::string, std::vector<poMessageSubscriber* > > subscribers;
        std::vector<poMessage* > messageQueue;
    }
    
    //------------------------------------------------------------------
    void update() {
        //Go through queue, broadcasting messages
		std::vector<poMessage*>::iterator it = messageQueue.begin();
		while(it != messageQueue.end()) {
			poMessage* m = *it;
			
            //Go through subscribers for this message, checking to see if they need to be alerted
            for (std::vector<poMessageSubscriber* >::iterator sIter = subscribers[m->message].begin(); sIter != subscribers[m->message].end(); ++sIter) {
                poMessageSubscriber* thisSubscriber = (*sIter);
                
                if(thisSubscriber->sender == NULL || thisSubscriber->sender == m->sender) {
                    (*sIter)->subscriber->messageHandler(m->message, m->dict);
                }
            }
			
			delete m;
			it = messageQueue.erase(it);
		}
		
		
//        for(std::vector<poMessage*>::iterator mIter = messageQueue.begin(); mIter != messageQueue.end(); ++mIter) {
//            poMessage* m = (*mIter);
//            
//            //Go through subscribers for this message, checking to see if they need to be alerted
//            for (std::vector<poMessageSubscriber* >::iterator sIter = subscribers[m->message].begin(); sIter != subscribers[m->message].end(); ++sIter) {
//                poMessageSubscriber* thisSubscriber = (*sIter);
//                
//                if(thisSubscriber->sender == NULL || thisSubscriber->sender == m->sender) {
//                    (*sIter)->subscriber->messageHandler(m->message, m->dict);
//                }
//            }
//            
//            delete m; m=NULL;
//        }
//        
//        messageQueue.clear();
    }
    
    
    //------------------------------------------------------------------
    void addSubscriber(std::string msg, poObject* subscriber, poObject* sender) {
        subscribers[msg].push_back(new poMessageSubscriber());
        subscribers[msg].back()->sender       = sender;
        subscribers[msg].back()->subscriber   = subscriber;
    }
    
    
    //------------------------------------------------------------------
    void removeSubscriber(std::string msg, poObject* subscriber) {
        if(subscribers.find(msg) != subscribers.end()) {
            if(!subscribers[msg].empty()) {
                //Find the subscriber and delete it
                for(std::vector<poMessageSubscriber* >::iterator sIter = subscribers[msg].begin(); sIter != subscribers[msg].end(); ++sIter) {
                    poMessageSubscriber* thisSubscriber = (*sIter);
                    if(thisSubscriber->subscriber == subscriber) {
                        delete (*sIter);
                        subscribers[msg].erase(sIter);
                    }
                }
            }
        }
    }
    
    
    //------------------------------------------------------------------
    void removeAllSubscribers(std::string msg) {
        if(subscribers.find(msg) != subscribers.end()) {
            subscribers[msg].clear();
            
            std::map<std::string, std::vector<poMessageSubscriber* > >::iterator it;
            it = subscribers.find(msg);
            subscribers.erase(it);
        }
    }
    
    
    //------------------------------------------------------------------
    void broadcastMessage(std::string msg, poObject* sender, const poDictionary& dict) {
        if(subscribers.find(msg) != subscribers.end()) {
            messageQueue.push_back(new poMessage());
            
            messageQueue.back()->sender     = sender;
            messageQueue.back()->message    = msg;
            messageQueue.back()->dict       = dict;
        }
    }
}

