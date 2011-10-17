//
//  ImageLoader.cpp
//  Threaded Image Loading
//
//  Created by Joshua Fisher on 10/14/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#include "ImageLoader.h"
#include "Helpers.h"

ImageLoader::Worker::Worker(ImageLoader *l, const std::string &u, poObject *n) 
:	loader(l)
,	url(u)
,	toNotify(n)
,	image(NULL)
{}
	
void ImageLoader::Worker::operator()() {
	double start =  getTime();

	image = new poImage(url);
	if(!image->isValid()) {
		delete image; image = NULL;
		error = std::string("Couldn't load Image: ") + url;
	}
	
	elapsed = getTime() - start;
	loader->workerDone(*this);
}

using namespace boost::asio;

ImageLoader::ImageLoader() 
:	work(service)
{
	for(int i=0; i<getNumCpus(); i++) {
		threads.create_thread(boost::bind(&io_service::run, &service));
	}
}

ImageLoader::~ImageLoader() {
	service.stop();
	threads.join_all();
}

void ImageLoader::loadImage(const fs::path &url, poObject *notice) {
	ImageLoader::Worker worker(this, url.c_str(), notice);
	service.post(worker);
}

void ImageLoader::update() {
	std::list<Worker>::iterator iter = completed.begin();
	while(iter != completed.end()) {
		if(iter->image) 
			iter->toNotify->messageHandler(ImageLoadSuccessMessage, 
										   poDictionary()
											   .setPtr("image", iter->image)
											   .setFloat("elapsed", iter->elapsed));
		else
			iter->toNotify->messageHandler(ImageLoadFailureMessage,
										   poDictionary()
											   .setString("url", iter->url)
											   .setString("error", iter->error)
											   .setFloat("elapsed", iter->elapsed));
		iter = completed.erase(iter);
	}
}

void ImageLoader::workerDone(Worker worker) {
	completed.push_back(worker);
}



