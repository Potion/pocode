#include "poMessageCenter.h"
#include "poObject.h"

namespace po {
    namespace MessageCenter {
        typedef struct {
            Object* sender;
            std::string message;
            Dictionary dict;
        } Message;
        
        struct MessageSubscriber {
            MessageSubscriber() : bShouldDelete(false) {};
            Object* sender;
            Object* subscriber;
            bool bShouldDelete;
        } ;
        
        namespace {
            std::map<std::string, std::list<MessageSubscriber* > > subscribers;
            std::list<Message* > messageQueue;
        }
        
        
        //------------------------------------------------------------------
        void update() {
            //Go through queue, broadcasting messages
            std::list<Message*>::iterator msgIter = messageQueue.begin();
            for(; msgIter != messageQueue.end(); ++msgIter) {
                Message* thisMsg = (*msgIter);
                
                //Go through subscribers for this message, checking to see if they need to be alerted
                std::list<MessageSubscriber* >::iterator subscriberIter = subscribers[thisMsg->message].begin();
                for (; subscriberIter != subscribers[thisMsg->message].end(); ++subscriberIter) {
                    MessageSubscriber* thisSubscriber = (*subscriberIter);
                    
                    if(thisSubscriber->bShouldDelete == false) {
                        if(thisSubscriber->sender == NULL || thisSubscriber->sender == thisMsg->sender) {
                            thisSubscriber->subscriber->messageHandler(thisMsg->message, thisMsg->dict, thisMsg->sender);
                        }
                    }
                }
                
                delete thisMsg; thisMsg=NULL;
            }
            
            //Clear out all messages
            messageQueue.clear();
            
            //Get rid of any subscribers that are no longer listening
            cleanupSubscribers();
        }
        
        
        //------------------------------------------------------------------
        void addSubscriber(std::string msg, Object* subscriber, Object* sender) {
            //Subscribers can be added multiple times
            //Should we check for this and only add once or no?
//			bool found = false;
//			if(subscribers.count(msg) > 0) {
//				for(std::list<MessageSubscriber*>::iterator it = subscribers[msg].begin();
//					it != subscribers[msg].end();
//					++it)
//				{
//					MessageSubscriber* sub = *it;
//					if(sub->sender == sender && sub->subscriber == subscriber && !sub->bShouldDelete) {
//						found = true;
//						break;
//					}
//				}
//			}
//			if(!found) {
				subscribers[msg].push_back(new MessageSubscriber());
				subscribers[msg].back()->sender       = sender;
				subscribers[msg].back()->subscriber   = subscriber;
//			}
        }
        
        
        //------------------------------------------------------------------
        void removeSubscriber(Object* subscriber) {
            //Remove from Subscribers list
            //Go Through Messages
            std::map<std::string, std::list<MessageSubscriber* > >::iterator msgIter = subscribers.begin();
            for (; msgIter!=subscribers.end(); ++msgIter) {
                //Go through all subscribers for msg
                std::list<MessageSubscriber* >::iterator subscriberIter = msgIter->second.begin();
                for(; subscriberIter != msgIter->second.end(); ++subscriberIter) {
                    if((*subscriberIter)->subscriber == subscriber) {
                        (*subscriberIter)->bShouldDelete = true;
                    }
                }
            }
        }
        
        
        //------------------------------------------------------------------
        void removeSubscriberForMessage(Object* subscriber, std::string msg) {
            if(subscribers.find(msg) != subscribers.end()) {
                if(!subscribers[msg].empty()) {
                    //Find the subscriber and delete it
                    std::list<MessageSubscriber* >::iterator subscriberIter = subscribers[msg].begin();
                    for(; subscriberIter != subscribers[msg].end(); ++subscriberIter) {
                        MessageSubscriber *thisSubscriber = (*subscriberIter);
                        if(thisSubscriber->subscriber == subscriber) {
                            thisSubscriber->bShouldDelete = true;
                        }
                    }
                }
            }
        }
        
		void removeSubscriberForMessageAndSender(Object* subscriber, std::string msg, Object* sender)
		{
            if(subscribers.find(msg) != subscribers.end()) {
                if(!subscribers[msg].empty()) {
                    //Find the subscriber and delete it
                    for(std::list<MessageSubscriber* >::iterator sIter = subscribers[msg].begin(); sIter != subscribers[msg].end(); ++sIter) {
                        MessageSubscriber* thisSubscriber = (*sIter);
                        if(thisSubscriber->subscriber == subscriber && thisSubscriber->sender == sender) {
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
                std::list<MessageSubscriber* >::iterator subscriberIter = subscribers[msg].begin();
                for(; subscriberIter != subscribers[msg].end(); ++subscriberIter) {
                    (*subscriberIter)->bShouldDelete = true;
                }
            }
        }
        
        
        //------------------------------------------------------------------
        void cleanupSubscribers() {
            //Search Through lists of subscribers by message
            std::map<std::string, std::list<MessageSubscriber* > >::iterator msgIter = subscribers.begin();
            for ( ; msgIter!=subscribers.end(); ++msgIter) {
                //Search through each list for subscribers to delete
                std::list<MessageSubscriber* >::iterator subscriberIter = msgIter->second.begin();
                for(; subscriberIter != msgIter->second.end(); ++subscriberIter) {
                    if((*subscriberIter)->bShouldDelete) {
                        delete(*subscriberIter);
                        subscriberIter = msgIter->second.erase(subscriberIter);
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
