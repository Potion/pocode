//
//  audio.cpp
//  movieplayer
//
//  Created by Joshua Fisher on 1/28/13.
//  Copyright (c) 2013 Joshua Fisher. All rights reserved.
//

#include "poAudioPlayer.h"

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#elif defined WIN32
#include <al.h>
#include <alc.h>
#endif

#include <cassert>
#include <algorithm>
#include <boost/chrono.hpp>

#include "poFFMpegDecoder.h"

namespace po {
	
	#define AL_ASSERT_NO_ERROR() { ALenum err = alGetError(); if(err != AL_NO_ERROR) {printf("OpenAL error: %X\n", err); assert(false);} }
	
	struct OpenALInit {
		OpenALInit() {
			alGetError();
			ALCdevice* device = alcOpenDevice(NULL);
			AL_ASSERT_NO_ERROR();
			ALCcontext* context = alcCreateContext(device, NULL);
			AL_ASSERT_NO_ERROR();
			alcMakeContextCurrent(context);
			AL_ASSERT_NO_ERROR();
		}
		~OpenALInit() {
//			ALCcontext* context = alcGetCurrentContext();
//			AL_ASSERT_NO_ERROR();
//			ALCdevice* device = alcGetContextsDevice(context);
//			AL_ASSERT_NO_ERROR();
//			alcMakeContextCurrent(NULL);
//			AL_ASSERT_NO_ERROR();
//			alcDestroyContext(context);
//			AL_ASSERT_NO_ERROR();
//			alcCloseDevice(device);
//			AL_ASSERT_NO_ERROR();
		}
	};
	static OpenALInit openALInit;
	
	AudioPlayer::AudioPlayer()
	:	demuxer(NULL)
	,	audioDecoder(NULL)
	,	uid(0)
	,	killUpdateThread(false)
	,	clock(0.f)
	,	loop(false)
	,	state(Stopped)
	,	ownsDemuxer(false)
	{}
	
	AudioPlayer::~AudioPlayer() {
		close();
	}
	
	bool AudioPlayer::open(const char* path)
	{
		Demuxer* de = Demuxer::open(path);
		ownsDemuxer = true;
		return open(de);
	}
	
	bool AudioPlayer::open(Demuxer* de) {
		close();

		demuxer = de;
		if(!demuxer)
			return false;
		
		ownsDemuxer = false;

		if(!(audioDecoder = AudioDecoder::open(demuxer)))
			return false;

		alGenSources(1, &uid);
		alSourcef(uid, AL_PITCH, 1.f);
		alSourcef(uid, AL_GAIN, 1.f);
		alSourcei(uid, AL_LOOPING, AL_FALSE);
		AL_ASSERT_NO_ERROR();

		alGenBuffers(BufferCount, buffers);
		AL_ASSERT_NO_ERROR();

		format = AL_FORMAT_STEREO16;
		clock = 0.f;
		
		return true;
	}

	void AudioPlayer::close() {
		if(updateThread.joinable()) {
			killUpdateThread = true;
			updateThread.join();
		}
		
		if(uid) {
			alSourceStop(uid);
			AL_ASSERT_NO_ERROR();
			alSourcei(uid, AL_BUFFER, 0);
			AL_ASSERT_NO_ERROR();
			alDeleteBuffers(BufferCount, buffers);
			AL_ASSERT_NO_ERROR();
			alDeleteSources(1, &uid);
			AL_ASSERT_NO_ERROR();
			
			uid = 0;
			memset(buffers, 0, sizeof(uint32_t)*BufferCount);
		}
		
		if(audioDecoder) {
			delete audioDecoder;
			audioDecoder = NULL;
		}
		if(demuxer && ownsDemuxer) {
			delete demuxer;
			demuxer = NULL;
		}
		
		state = Stopped;
	}
	
	void AudioPlayer::play() {
		if(state != Playing) {
			if(state == Stopped) {
				killUpdateThread = false;
				updateThread = boost::thread(boost::bind(&AudioPlayer::updateBuffers,this));

				for(int i=0; i<BufferCount; i++) {
					AudioBuffer::Ptr buf = audioDecoder->nextBuffer();
					if(buf) {
						alBufferData(buffers[i], format, buf->bytes, buf->numBytes, buf->sampleRate);
						AL_ASSERT_NO_ERROR();
					}
				}
				
				alSourceQueueBuffers(uid, BufferCount, buffers);
				AL_ASSERT_NO_ERROR();
			}
			
			alSourcePlay(uid);
			AL_ASSERT_NO_ERROR();
			
			state = Playing;
		}
	}
	
	void AudioPlayer::pause() {
		if(state != Paused) {
			alSourcePause(uid);
			AL_ASSERT_NO_ERROR();
			state = Paused;
		}
	}
	
	void AudioPlayer::stop() {
		if(state != Stopped) {
			killUpdateThread = true;
			updateThread.join();
			
			alSourceStop(uid);
			alSourcei(uid, AL_BUFFER, 0);
			AL_ASSERT_NO_ERROR();
			state = Stopped;
		}
	}
	
	void AudioPlayer::seek(float f) {
		bool wasPlaying = isPlaying();
		
		stop();
		
		clock = f;
		audioDecoder->seekToTime(f);
		
		if(wasPlaying)
			play();
	}
	
	void AudioPlayer::setLooping(bool b) { loop = b; }
	
	void AudioPlayer::setVolume(float f) {
		f = std::max(0.f, std::min(1.f, f));
		alSourcef(uid, AL_GAIN, f);
	}
	
	float AudioPlayer::getTime() const {
		float time = 0.f;
		alGetSourcef(uid, AL_SEC_OFFSET, &time);
		return clock + time;
	}
	
	bool AudioPlayer::isPlaying() const { return state == Playing; }
	bool AudioPlayer::isPaused() const { return state == Paused; }
	bool AudioPlayer::isStopped() const { return state == Stopped; }
	bool AudioPlayer::isFinished() const { return state == Finished; }
	bool AudioPlayer::isLooping() const { return loop; }

	void AudioPlayer::updateBuffers() {
		while(!killUpdateThread) {
			if(audioDecoder->isLastBuffer()) {
				if(loop) {
					audioDecoder->seekToTime(0.0);
					continue;
				}
				else {
					state = Finished;
					break;
				}
			}
			
			ALint processed = 0;
			alGetSourcei(uid, AL_BUFFERS_PROCESSED, &processed);
			AL_ASSERT_NO_ERROR();

			if(processed > 0) {
				ALuint buffs[BufferCount];
				alSourceUnqueueBuffers(uid, processed, buffs);
				AL_ASSERT_NO_ERROR();

				for(int i=0; i<processed; i++) {
					clock += (audioDecoder->getFrameSize() / audioDecoder->getSampleSize() / (float)audioDecoder->getChannelCount()) / (float)audioDecoder->getSampleRate();
					
					AudioBuffer::Ptr buf = audioDecoder->nextBuffer();
					if(!buf)
						break;
					
					alBufferData(buffs[i], format, buf->bytes, buf->numBytes, buf->sampleRate);
					AL_ASSERT_NO_ERROR();
				}
				
				alSourceQueueBuffers(uid, processed, buffs);
				AL_ASSERT_NO_ERROR();
			}
			
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

}


