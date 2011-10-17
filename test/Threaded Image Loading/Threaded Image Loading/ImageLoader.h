//
//  ImageLoader.h
//  Threaded Image Loading
//
//  Created by Joshua Fisher on 10/14/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poImage.h"
#include "poObject.h"

#include <boost/asio.hpp>
#include <boost/thread/shared_mutex.hpp>

static const std::string ImageLoadSuccessMessage = "IMAGE_LOAD_SUCCESS_MESSAGE";
static const std::string ImageLoadFailureMessage = "IMAGE_LOAD_FAILURE_MESSAGE";

class ImageLoader {
public:
	ImageLoader();
	~ImageLoader();
	
	/*	ImageLoadSuccessMessage: {'image':poImage*, 'elapsed':double}
	 *	ImageLoadFailureMessage: {'url':string, 'error':string, 'elapsed':double}
	 */
	void loadImage(const fs::path &url, poObject *notice, const poDictionary &dict=poDictionary());
	void update();
	
private:
	boost::thread_group threads;
	boost::asio::io_service service;
	boost::asio::io_service::work work;
	
	struct Worker {
		ImageLoader *loader;
		std::string url;
		poObject *toNotify;
        
        poDictionary dict;

		poImage *image;
		std::string error;
		double elapsed;
		
		Worker(ImageLoader *l, const std::string &u, poObject *n, const poDictionary &dict);
		void operator()();
	};

	boost::shared_mutex mtx;
	std::list<Worker> completed;
	
	void workerDone(Worker worker);
};



