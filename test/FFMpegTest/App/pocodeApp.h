#pragma once
#include "poObject.h"

namespace po { 
	class MoviePlayer; 
	class AudioPlayer;
}

class pocodeApp : public poObject {
public:
	pocodeApp();

	virtual void	update();
	virtual void	draw();
	virtual void	eventHandler(poEvent*);
	virtual void	messageHandler(const std::string &msg, const poDictionary &dict=poDictionary());

	po::MoviePlayer* player;
	po::AudioPlayer* audio;
};