//
//  poSpriteSheet.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/25/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poSpriteSheet.h"

#include "Helpers.h"
#include "SimpleDrawing.h"

poSpriteSheet::poSpriteSheet(fs::path anim_gif) {}

poSpriteSheet::poSpriteSheet(const std::vector<poImage*> &sprites, double trans_time)
:	trans_time(trans_time)
{
	for(int i=0; i<sprites.size(); ++i) {
		textures.push_back(new poTexture(sprites[i]));
	}
}

poSpriteSheet::~poSpriteSheet() {
	BOOST_FOREACH(poTexture *tex, textures) {
		delete tex;
	}
}

void poSpriteSheet::update() {
	if(!running)
		return;
	
	double now = getTime();
	if(now > next) {
		current += 1;
		if(current >= textures.size()) {
			callback_dict.setBool("complete",true);
			stop();
		}
		else {
			next = now + trans_time;
		}
	}
}

void poSpriteSheet::draw()	{
	drawRect(textures[current]);
}

void poSpriteSheet::notifyOnComplete(poObject *obj, const std::string &msg, const poDictionary &dict) {
	callback = boost::bind(&poObject::messageHandler, obj, msg, _1);
	callback_dict = dict;
	callback_dict.setPtr("spriteSheet", this);
}

uint poSpriteSheet::numSprites() const	{return textures.size();}

double poSpriteSheet::transitionTime() const {return trans_time;}
void poSpriteSheet::transitionTime(double d) {trans_time = d;}

uint poSpriteSheet::currentSprite() const {return current;}
void poSpriteSheet::currentSprite(uint i) {current = i;}

bool poSpriteSheet::isRunning() const {return running;}

void poSpriteSheet::start() {
	current = 0;
	next = getTime() + trans_time;
	running = true;
	callback_dict.setBool("complete",false);
}

void poSpriteSheet::stop() {
	running = false;
	callback(callback_dict);
}
