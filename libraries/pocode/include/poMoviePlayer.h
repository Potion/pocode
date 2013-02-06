//
//  Movie.h
//  curve
//
//  Created by Joshua Fisher on 1/8/13.
//  Copyright (c) 2013 Joshua Fisher. All rights reserved.
//

#pragma once

#include "poObject.h"
#include "poTexture.h"

namespace po {
	
	class Demuxer;
	class VideoDecoder;
	class AudioPlayer;
	
	class MoviePlayer : public poObject {
	public:
		MoviePlayer();
		~MoviePlayer();

		bool open(const char* path);
		void close();
		void play();
		void pause();
		void stop();
		void seek(float sec);
		void previousFrame();
		void nextFrame();
		
		bool isPlaying() const;
		bool isPaused() const;
		bool isStopped() const;
		bool isFinished() const;
		
		void setRect(float x, float y, float w, float h);

		void draw();
		void update();
		
	private:
		Demuxer* demuxer;
		VideoDecoder* videoDecoder;
		AudioPlayer* audioPlayer;
		
		enum {
			Playing,
			Paused,
			Stopped,
			Complete
		} state;
		
		poRect rect;

		uint32_t playStartTime;
		uint32_t pauseStartTime, pauseElapsedTime;

		poTexture* texture;
	};

}