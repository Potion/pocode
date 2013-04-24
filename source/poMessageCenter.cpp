#include "poMessageCenter.h"
#include "poObject.h"

namespace po {
    namespace MessageCenter {
        typedef struct {
            Object* sender;
            std::string message;
            Dictionary dict;
        } Message;
        
        typedef struct {
            Object* sender;
            Object* subscriber;
        } MessageSubscriber;
        
        namespace {
            std::map<std::string, std::list<MessageSubscriber* > > subscribers;
            std::list<Message* > messageQueue;
        }
        
        
        //------------------------------------------------------------------
        void update() {
            //Subscribers set to NULL when removed
            //To avoid deleting in nested iteration
            cleanupSubscribers();
            
            //Go through queue, broadcasting messages
            for(std::list<Message*>::iterator mIter = messageQueue.begin(); mIter != messageQueue.end(); ++mIter) {
                Message* m = (*mIter);
                
                //Go through subscribers for this message, checking to see if they need to be alerted
                for (std::list<MessageSubscriber* >::iterator sIter = subscribers[m->message].begin(); sIter != subscribers[m->message].end(); ++sIter) {
                    MessageSubscriber* thisSubscriber = (*sIter);
                    
                    if(thisSubscriber->sender == NULL || thisSubscriber->sender == m->sender) {
                        (*sIter)->subscriber->messageHandler(m->message, m->dict, m->sender);
                    }
                }
                
                delete m; m=NULL;
            }
            
            messageQueue.clear();
        }
        
        
        //------------------------------------------------------------------
        void addSubscriber(std::string msg, Object* subscriber, Object* sender) {
            subscribers[msg].push_back(new MessageSubscriber());
            subscribers[msg].back()->sender       = sender;
            subscribers[msg].back()->subscriber   = subscriber;
        }
        
        
        //------------------------------------------------------------------
        void removeSubscriber(Object* subscriber) {
            //Remove from Subscribers list
            for (std::map<std::string, std::list<MessageSubscriber* > >::iterator iter = subscribers.begin(); iter!=subscribers.end(); ++iter) {
                for(std::list<MessageSubscriber* >::iterator sIter = iter->second.begin(); sIter != iter->second.end(); ++sIter) {
                    if((*sIter)->subscriber == subscriber) {
                        (*sIter)->subscriber = NULL;
                    }
                }
            }
        }
        
        
        //------------------------------------------------------------------
        void removeSubscriberForMessage(Object* subscriber, std::string msg) {
            if(subscribers.find(msg) != subscribers.end()) {
                if(!subscribers[msg].empty()) {
                    //Find the subscriber and delete it
                    for(std::list<MessageSubscriber* >::iterator sIter = subscribers[msg].begin(); sIter != subscribers[msg].end(); ++sIter) {
                        MessageSubscriber* thisSubscriber = (*sIter);
                        if(thisSubscriber->subscriber == subscriber) {
                            delete (*sIter);
                            subscribers[msg].erase(sIter);
                        }
                    }
                }
            }
        }
        
        
        //------------------------------------------------------------------
        void removeAllSubscribersForMessage(std::string msg) {
            if(subscribers.find(msg) != subscribers.end()) {
                subscribers[msg].clear();
                
                std::map<std::string, std::list<MessageSubscriber* > >::iterator it;
                it = subscribers.find(msg);
                subscribers.erase(it);
            }
        }
        
        
        //------------------------------------------------------------------
        void cleanupSubscribers() {
            //Remove from Subscribers list
            for (std::map<std::string, std::list<MessageSubscriber* > >::iterator iter = subscribers.begin(); iter!=subscribers.end(); ++iter) {
                for(std::list<MessageSubscriber* >::iterator sIter = iter->second.begin(); sIter != iter->second.end(); ++sIter) {
                    if((*sIter)->subscriber == NULL) {
                        iter->second.erase(sIter);
                    }
                }
            }

        }
        
        //------------------------------------------------------------------
        void broadcastMessage(std::string msg, Object* sender, const Dictionary& dict) {
            if(subscribers.find(msg) != subscribers.end()) {
                messageQueue.push_back(new Message());
                
                messageQueue.back()->sender     = sender;
                messageQueue.back()->message    = msg;
                messageQueue.back()->dict       = dict;
            }
        }
    }
}
