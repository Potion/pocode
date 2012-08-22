#include "poMessageCenter.h"


//Singleton Instance/Get Instance
poMessageCenter* poMessageCenter::pMessageCenter=NULL;

//------------------------------------------------------------------
poMessageCenter* poMessageCenter::get() {
	if(pMessageCenter==NULL) {
		pMessageCenter = new poMessageCenter();
	}
    
	return pMessageCenter;
}

poMessageCenter::poMessageCenter() {
}

poMessageCenter::~poMessageCenter() {
}


//------------------------------------------------------------------
void poMessageCenter::update() {
    //Go through queue, broadcasting messages
    for(std::vector<poMessage*>::iterator mIter = messageQueue.begin(); mIter != messageQueue.end(); ++mIter) {
        poMessage* m = (*mIter);
        
        //Go through subscribers for this message, checking to see if they need to be alerted
        for (std::vector<poMessageSubscriber* >::iterator sIter = subscribers[m->message].begin(); sIter != subscribers[m->message].end(); ++sIter) {
            poMessageSubscriber* thisSubscriber = (*sIter);
            
            if(thisSubscriber->sender == NULL || thisSubscriber->sender == m->sender) {
                (*sIter)->subscriber->messageHandler(m->message, m->dict);
            }
        }
        
        delete m; m=NULL;
    }
    
    messageQueue.clear();
}


//------------------------------------------------------------------
void poMessageCenter::addSubscriber(std::string msg, poObject* subscriber, poObject* sender) {
    subscribers[msg].push_back(new poMessageSubscriber());
    subscribers[msg].back()->sender       = sender;
    subscribers[msg].back()->subscriber   = subscriber;
}


//------------------------------------------------------------------
void poMessageCenter::removeSubscriber(std::string msg, poObject* subscriber) {
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
void poMessageCenter::removeAllSubscribers(std::string msg) {
    if(subscribers.find(msg) != subscribers.end()) {
        subscribers[msg].clear();
        
        std::map<std::string, std::vector<poMessageSubscriber* > >::iterator it;
        it = subscribers.find(msg);
        subscribers.erase(it);
    }
}


//------------------------------------------------------------------
void poMessageCenter::broadcastMessage(std::string msg, poObject* sender, const poDictionary& dict) {
    if(subscribers.find(msg) != subscribers.end()) {
        messageQueue.push_back(new poMessage());
        
        messageQueue.back()->sender     = sender;
        messageQueue.back()->message    = msg;
        messageQueue.back()->dict       = dict;
    }
}
