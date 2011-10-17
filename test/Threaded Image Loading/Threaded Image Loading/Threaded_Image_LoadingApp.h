/*	Created by Joshua Fisher on 10/14/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "poImage.h"
#include "poTexture.h"
#include "ImageLoader.h"

class Threaded_Image_LoadingApp : public poObject {
public:
	Threaded_Image_LoadingApp();
	virtual ~Threaded_Image_LoadingApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	ImageLoader loader;
};