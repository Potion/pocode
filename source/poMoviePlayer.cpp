//
//  Movie.cpp
//  curve
//
//  Created by Joshua Fisher on 1/8/13.
//  Copyright (c) 2013 Joshua Fisher. All rights reserved.
//

#include "poHelpers.h"
#include "poOpenGLState.h"
#include "poSimpleDrawing.h"

#include "poMoviePlayer.h"
#include "poFFMpegDecoder.h"
#include "poAudioPlayer.h"

namespace po {
	
	bool uploadFrame(poTexture* texture, VideoFrame::Ptr frame) {
		if(!frame)
			return false;

		po::useTexture(texture->getUid(), false, GL_TEXTURE_2D, 0);
		texture->replace(frame->bytes);
		return true;
	}

	MoviePlayer::MoviePlayer()
	:	demuxer(NULL)
	,	videoDecoder(NULL)
	,	audioPlayer(NULL)
	,	state(Stopped)
	,	playStartTime(0)
	,	pauseStartTime(0)
	,	pauseElapsedTime(0)
	,	texture(NULL)
	{}
	
	MoviePlayer::~MoviePlayer() {
		close();
	}
	
	bool MoviePlayer::open(const char* path) {
		close();
	
		if(!(demuxer = Demuxer::open(path)))
			return false;
		
		if(!(videoDecoder = VideoDecoder::open(demuxer)))
		   return false;

		int st = demuxer->getStreamIndex(AVMEDIA_TYPE_AUDIO);
		
		if(st >= 0) {
			audioPlayer = new AudioPlayer;
			audioPlayer->open(demuxer);
		}

		rect.width = videoDecoder->getWidth();
		rect.height = videoDecoder->getHeight();
		texture = new poTexture(rect.width, rect.height, NULL, poTextureConfig(GL_RGB));
		
		uploadFrame(texture, videoDecoder->nextFrame());

		return true;
	}
	
	void MoviePlayer::close() {
		state = Stopped;
		if(videoDecoder) {
			delete videoDecoder;
			videoDecoder = NULL;
		}
		if(audioPlayer) {
			delete audioPlayer;
			audioPlayer = NULL;
		}
		if(demuxer) {
			delete demuxer;
			demuxer = NULL;
		}
		if(texture)	{
			delete texture;
			texture = NULL;
		}
	}
	
	void MoviePlayer::play() {
		if(state != Playing) {
			switch(state) {
				case Stopped:
					seek(0);
					break;
					
				case Paused:
					pauseElapsedTime += (poGetElapsedTime() - pauseStartTime);
					break;
					
				default:
					break;
			}
			
			state = Playing;
			
			if(audioPlayer)
				audioPlayer->play();
		}
	}
	
	void MoviePlayer::pause() {
		if(state != Paused) {
			switch(state) {
				case Playing:
					pauseStartTime = poGetElapsedTime();
					break;
					
				default:
					break;
			}
			
			state = Paused;
			
			if(audioPlayer)
				audioPlayer->pause();
		}
	}
	
	void MoviePlayer::stop() {
		if(state != Stopped) {
			state = Stopped;
			
			if(audioPlayer)
				audioPlayer->stop();
		}
	}
	
	void MoviePlayer::seek(float time) {
		if(audioPlayer) {
			// this needs to happen to reset the audio clock
			// TODO:	figure out fix so the audio decoder knows when
			//			it was flushed and can reset its clock
			audioPlayer->seek(time);
		}
		else {
			videoDecoder->seekToTime(time);
			double now = poGetElapsedTime();
			playStartTime = now + time;
			pauseStartTime = now;
			pauseElapsedTime = 0;
		}
		
		uploadFrame(texture, videoDecoder->nextFrame());
	}
	
	void MoviePlayer::previousFrame() {
		if(videoDecoder) {
			pause();
			uploadFrame(texture, videoDecoder->previousFrame());
		}
	}
	
	void MoviePlayer::nextFrame() {
		if(videoDecoder && state != Complete) {
			pause();
			if(!uploadFrame(texture, videoDecoder->nextFrame())) {
				if(videoDecoder->isLastFrame())
					state = Complete;
			}
		}
	}
    
    void MoviePlayer::setVolume(float f) {
        if(audioPlayer) {
            audioPlayer->setVolume(f);
        }
    }
	
	bool MoviePlayer::isPlaying() const {
		return state == Playing;
	}
	
	bool MoviePlayer::isPaused() const {
		return state == Paused;
	}
	
	bool MoviePlayer::isStopped() const {
		return state == Stopped;
	}
	
	bool MoviePlayer::isFinished() const {
		return state == Complete;
	}

	double MoviePlayer::getTime() const {
		return videoDecoder->getCurrentTime();
	}
	
	int MoviePlayer::getFrameNum() const {
		return videoDecoder->getCurrentFrame();
	}
	
	int MoviePlayer::getFrameCount() const {
		return videoDecoder->getFrameCount();
	}
	
	void MoviePlayer::setRect(float x, float y, float w, float h) {
		if(videoDecoder) {
			int vW = videoDecoder->getWidth();
			int vH = videoDecoder->getHeight();
			
			float vR = vH / (float)vW;
			float hNew = w * vR;
			float wNew = w;

			if(hNew > h) {
				float scale = h / hNew;
				wNew *= scale;
				hNew *= scale;
			}
			
			x += (w - wNew) / 2.f;
			y += (h - hNew) / 2.f;

			rect.x = x;
			rect.y = y;
			rect.width = wNew;
			rect.height = hNew;
		}
	}

	void MoviePlayer::update() {
		if(state == Playing) {
			double cur = videoDecoder->getNextTime();
			
			double target = 0.0;
			
			if(audioPlayer)
				target = audioPlayer->getTime();
			else
				target = poGetElapsedTime() - (playStartTime + pauseElapsedTime);
			
			while(cur <= target) {
				if(!uploadFrame(texture, videoDecoder->nextFrame())) {
					if(videoDecoder->isLastFrame()) {
						state = Complete;
					}
					break;
				}

				cur = videoDecoder->getNextTime();
			}
		}
	}

	void MoviePlayer::draw() {
        po::setColor(poColor::white);
		po::drawTexturedRect(texture, rect.getFlipped());
	}
}


