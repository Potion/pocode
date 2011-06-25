//
//  poSpriteSheet.h
//  poFramework4
//
//  Created by Joshua Fisher on 6/25/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poObject.h"
#include "poTexture.h"

class poSpriteSheet : public poObject {
public:
	poSpriteSheet(fs::path anim_gif);
	poSpriteSheet(const std::vector<poImage*> &sprites, double trans_time);
	virtual ~poSpriteSheet();
	
	virtual void update();
	virtual void draw();
	void notifyOnComplete(poObject *obj, const std::string &msg, const poDictionary &dict=poDictionary());
	
	uint numSprites() const;
	
	double transitionTime() const;
	void transitionTime(double d);
	
	uint currentSprite() const;
	void currentSprite(uint i);
	
	bool isRunning() const;	
	void start();	
	void stop();
	
private:
	std::vector<poTexture*> textures;
	uint current;
	double trans_time, next;
	bool running;
	
	boost::function<void(const poDictionary&)> callback;
	poDictionary callback_dict;
};
