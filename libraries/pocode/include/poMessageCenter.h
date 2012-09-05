#pragma once

/*	Created by Steve Varga on 3/1/12.
 *	Copyright 2012 Potion Design. All rights reserved.
 */


#include "poObject.h"
namespace poMessageCenter {
    
    void addSubscriber(std::string msg, poObject* subscriber, poObject* sender=NULL);
    void removeSubscriber(std::string msg, poObject* subscriber);
    void removeAllSubscribers(std::string msg);
    
    void broadcastMessage(std::string msg, poObject* sender=NULL, const poDictionary& dict=poDictionary());
    
	void update();
    
    
};