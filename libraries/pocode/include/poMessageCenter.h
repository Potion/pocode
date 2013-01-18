#pragma once

/*	Created by Steve Varga on 3/1/12.
 *	Copyright 2012 Potion Design. All rights reserved.
 */


#include "poObject.h"
namespace po {
    namespace MessageCenter {
        void update();

        void addSubscriber(std::string msg, Object* subscriber, Object* sender=NULL);
        
        void removeSubscriber(Object* subscriber);
        void removeSubscriberForMessage(Object* subscriber, std::string msg);
        void removeAllSubscribersForMessage(std::string msg);
        
        void broadcastMessage(std::string msg, Object* sender=NULL, const Dictionary& dict=Dictionary());
    };
} /*End po namespace */