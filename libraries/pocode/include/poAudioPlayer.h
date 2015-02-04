//
//  audio.h
//  movieplayer
//
//  Created by Joshua Fisher on 1/28/13.
//  Copyright (c) 2013 Joshua Fisher. All rights reserved.
//

#pragma once

namespace po {
	
	class Demuxer;
	class AudioDecoder;
	
	class AudioPlayer {
	public:
		AudioPlayer();
		~AudioPlayer();
		
		bool open(const char*);
		bool open(Demuxer*);
		void close();
		void play();
		void pause();
		void stop();
		void seek(float f);
		
		void setLooping(bool b);
		void setVolume(float v); // 0 - 1
		
		float getTime() const;
		
		bool isPlaying() const;
		bool isPaused() const;
		bool isStopped() const;
		bool isFinished() const;
		bool isLooping() const;
		
	private:
		enum {
			Playing,
			Paused,
			Stopped,
			Finished
		} state;
		
		Demuxer* demuxer;
		AudioDecoder* audioDecoder;
		
		static const int BufferCount = 5;
		
		uint32_t uid;
		uint32_t buffers[BufferCount];
		uint32_t format;
		float clock;
		bool loop;
		
		void updateBuffers();
		boost::thread updateThread;
		bool ownsDemuxer;
	};
	
}