//
//  MovieDecoder.h
//  curve
//
//  Created by Joshua Fisher on 1/10/13.
//  Copyright (c) 2013 Joshua Fisher. All rights reserved.
//

#pragma once

extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <libswresample/swresample.h>
}

namespace po {
	
	class PacketQueue {
		std::list<AVPacket> packets;
		
	public:
		PacketQueue();
		bool isEmpty();
		void push(AVPacket pkt);
		int pop(AVPacket* pkt);
		void flush();
		
		static AVPacket FlushPacket;
		static bool isFlushPacket(AVPacket pct);
	};
	
	class Demuxer {
	public:
		static Demuxer* open(const char*);
		~Demuxer();
		
		AVFormatContext* getFormat();
		int getStreamIndex(AVMediaType);
		AVStream* getStream(int streamIdx);
		PacketQueue* getPacketQueue(int streamIdx);
		
		// trigger loading packets into all registered queues
		void demux(int streamIndx);

		void seekToTime(double time);
		
	private:
		Demuxer();
		
		AVFormatContext* format;
		std::map<int,PacketQueue*> packetQueues;
	};
	
	// single frame of RGB video
	struct VideoFrame {
		typedef boost::shared_ptr<VideoFrame> Ptr;
		
		int width;
		int height;
		int numBytes;
		uint8_t* bytes;
		
		double outTime;
		
		~VideoFrame();
		static Ptr create(double o, int w, int h, int sz, uint8_t* ptr);
		
	private:
		VideoFrame();
		VideoFrame(const VideoFrame&) {}
		VideoFrame& operator=(const VideoFrame&) {};
	};
	
	struct AudioBuffer {
		typedef boost::shared_ptr<AudioBuffer> Ptr;
		
		int sampleRate;
		int numBytes;
		uint8_t* bytes;
		
		double outTime;
		
		~AudioBuffer();
		static Ptr create(double o, int sr, int sz, uint8_t* ptr);
		
	private:
		AudioBuffer();
		AudioBuffer(const AudioBuffer&) {};
		AudioBuffer& operator=(const AudioBuffer&) {};
	};

	class VideoDecoder  {
	public:
		static VideoDecoder* open(Demuxer*);
		~VideoDecoder();
		
		int getWidth();
		int getHeight();
		int getBytesPerFrame();
		bool isLastFrame();
		
		VideoFrame::Ptr previousFrame();
		VideoFrame::Ptr nextFrame();
		
		int64_t getCurrentFrame();
		double getCurrentTime();
		double getNextTime();
		
		void seekToFrame(int64_t frame);
		void seekToTime(double time);
		
	private:
		VideoDecoder();
		
		Demuxer* demuxer;
		PacketQueue* packets;
		
		int streamIdx;
		AVStream* stream;
		AVCodecContext* context;
		AVFrame *frame, *frameRGB;
		SwsContext* sws;
		
		double clock;
		double nextFrameTime;
		int64_t currentFrame;
		int64_t currentDts;
		int width, height, bytesPerFrame;
		bool lastFrame;
	};
	
	class AudioDecoder {
	public:
		static AudioDecoder* open(Demuxer*);
		~AudioDecoder();
		
		bool isLastBuffer() const;
		int getSampleRate() const;
		int getSampleSize() const;
		int getFrameSize() const;
		int getChannelCount() const;
		AudioBuffer::Ptr nextBuffer();
		
		void seekToTime(double time);
		
	private:
		AudioDecoder();
		AudioBuffer::Ptr convert(AVFrame*);
		
		Demuxer* demuxer;
		PacketQueue* packets;
		
		int streamIdx;
		AVStream* stream;
		AVCodecContext* context;
		AVFrame* frame;
		SwrContext* swr;
		
		int frameSize;
		int channels, sampleRate, sampleSize;
		
		bool lastBuffer;
	};

}

