#pragma once

/*	Created by Stephen Varga on 11/6/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include <curl/curl.h>


class poFileLoader {
public:
	poFileLoader();
	~poFileLoader();
    
    void getFile(std::string url, std::string filename="");
    std::string getFileAsString(std::string url);
	
//	virtual void update();
//	virtual void eventHandler(poEvent *event);
//	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};