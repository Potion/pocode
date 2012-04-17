#pragma once

/*	Created by Steve Varga on 3/1/12.
 *	Copyright 2012 Potion Design. All rights reserved.
 */

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

class poMessageCenter : public poObject {
public:
	static poMessageCenter* get();
    
    void addSubscriber(std::string msg, poObject* subscriber, poObject* sender=NULL);
    void removeSubscriber(std::string msg, poObject* subscriber);
    void removeAllSubscribers(std::string msg);
    
    void broadcastMessage(std::string msg, poObject* sender=NULL, const poDictionary& dict=poDictionary());
    
	virtual void update();
private:
	poMessageCenter();
	virtual ~poMessageCenter();
    static poMessageCenter* pMessageCenter;
    
    std::map<std::string, std::vector<poMessageSubscriber* > > subscribers;
    
    std::vector<poMessage* > messageQueue;
};