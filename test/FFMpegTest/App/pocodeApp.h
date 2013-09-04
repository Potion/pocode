#pragma once
#include "poObject.h"

namespace po { 
	class MoviePlayer; 
	class AudioPlayer;
}

class pocodeApp : public po::Object {
public:
	pocodeApp();

	virtual void	update();
	virtual void	draw();
	virtual void	eventHandler(po::Event*);
	virtual void	messageHandler(const std::string &msg, const po::Dictionary &dict=po::Dictionary());

	po::MoviePlayer* player;
	po::AudioPlayer* audio;
};