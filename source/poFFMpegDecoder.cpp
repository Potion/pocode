//
//  video_decoder.cpp
//  curve
//
//  Created by Joshua Fisher on 1/10/13.
//  Copyright (c) 2013 Joshua Fisher. All rights reserved.
//

#include "poFFMpegDecoder.h"

namespace po {
	
	AVPacket PacketQueue::FlushPacket;
	
	void initPacketQueue() {
		av_init_packet(&PacketQueue::FlushPacket);
		PacketQueue::FlushPacket.data = (uint8_t*)"FLUSH";
	}

	PacketQueue::PacketQueue() {
		static boost::once_flag once;
		boost::call_once(once, initPacketQueue);
	}
	
	bool PacketQueue::isEmpty() {
		return packets.empty();
	}
	
	void PacketQueue::push(AVPacket pkt) {
		av_dup_packet(&pkt);
		packets.push_back(pkt);
	}
	
	int PacketQueue::pop(AVPacket* pkt) {
		if(!packets.empty()) {
			*pkt = packets.front();
			packets.pop_front();
			return 1;
		}
		return 0;
	}
	
	void PacketQueue::flush() {
		for(std::list<AVPacket>::iterator it=packets.begin(); it!=packets.end(); ++it) {
			if(!PacketQueue::isFlushPacket(*it))
				av_free_packet(&(*it));
		}
		packets.clear();
		packets.push_back(FlushPacket);
	}

	bool PacketQueue::isFlushPacket(AVPacket pkt) {
		return pkt.data == FlushPacket.data;
	}

	struct FFMpegInit {
		FFMpegInit() {
			avcodec_register_all();
			av_register_all();
			avformat_network_init();
		}
		~FFMpegInit() {
		}
	};
	static FFMpegInit ffmpegInit;
	
	Demuxer* Demuxer::open(const char* path) {
		AVFormatContext* format = NULL;
		
		try {
			// open the file
			if(avformat_open_input(&format, path, NULL, NULL) < 0)
				throw -1;
			// read thru the header
			if(avformat_find_stream_info(format, NULL) < 0) {
				throw -1;
			}
			
			// make a demuxer, return it
			Demuxer* de = new Demuxer();
			de->format = format;
			return de;
		}
		catch(...) {
			if(format) {
				// something went wrong, clean up
				avformat_close_input(&format);
				format = NULL;
			}
		}
		
		return NULL;
	}
	
	Demuxer::Demuxer()
	:	format(NULL)
	{}
	
	Demuxer::~Demuxer() {
		if(format) {
			avformat_close_input(&format);
			
			for(std::map<int,PacketQueue*>::iterator it=packetQueues.begin(); it!=packetQueues.end(); ++it)
				delete it->second;
			packetQueues.clear();
		}
	}
	
	AVFormatContext* Demuxer::getFormat() {
		return format;
	}

	AVStream* Demuxer::getStream(int idx) {
		if(idx < 0 || idx >= format->nb_streams)
			return NULL;
		return format->streams[idx];
	}
	
	int Demuxer::getStreamIndex(AVMediaType type) {
		// look up the best-guess stream index for media type
		int st = av_find_best_stream(format, type, -1, -1, NULL, 0);
		if(st == AVERROR_STREAM_NOT_FOUND ||
		   st == AVERROR_STREAM_NOT_FOUND)
		{
			return -1;
		}
		return st;
	}
	
	PacketQueue* Demuxer::getPacketQueue(int idx) {
		if(idx < 0 || idx >= format->nb_streams)
			return NULL;
		
		// check to see if we already made one
		if(packetQueues.count(idx))
			return packetQueues[idx];
		
		// guess not, get the stream
		AVStream* st = format->streams[idx];
		// and the codec context
		AVCodecContext* ctx = st->codec;
		// find the decoder
		AVCodec* codec = avcodec_find_decoder(ctx->codec_id);
		// initialize the decoder
		if(avcodec_open2(ctx, codec, NULL) < 0)
			return NULL;
		
		// go ahead and allocate some storage
		ctx->opaque = (uint64_t*)av_malloc(sizeof(uint64_t));
		
		// make a new packet queue, store it, return it
		PacketQueue* queue = new PacketQueue;
		packetQueues[idx] = queue;
		return queue;
	}

	void Demuxer::demux(int idx) {
		if(idx < 0 || idx >= format->nb_streams)
			return;
		
		// make sure we event care about this stream
		if(packetQueues.count(idx)) {
			AVPacket packet;
			// pull packets until we find the stream we want
			while(true) {
				// get the next packet
				if(av_read_frame(format, &packet) < 0)
					return;
				
				// check if its a stream we care about
				if(packetQueues.count(packet.stream_index)) {
					// store the packet
					packetQueues[packet.stream_index]->push(packet);
					// and return if it was the one we wanted
					if(packet.stream_index == idx)
						return;
				}
				else {
					// otherwise clean up and move on
					av_free_packet(&packet);
				}
			}
		}
	}
	
	void Demuxer::seekToTime(double time) {
		int64_t ts = time * AV_TIME_BASE - AV_TIME_BASE;
		
		avformat_seek_file(format, -1, INT64_MIN, ts, ts, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);

		for(std::map<int,PacketQueue*>::iterator it=packetQueues.begin(); it!=packetQueues.end(); ++it)
			it->second->flush();
	}

	VideoFrame::VideoFrame()
	:	outTime(0.0)
	,	width(0)
	,	height(0)
	,	numBytes(0)
	,	bytes(NULL)
	{}
	
	VideoFrame::~VideoFrame() {
		if(bytes)
			delete [] bytes;
	}
	
	VideoFrame::Ptr VideoFrame::create(double o, int w, int h, int sz, uint8_t* ptr) {
		VideoFrame* fr = new VideoFrame;
		fr->outTime = o;
		fr->width = w;
		fr->height = h;
		fr->numBytes = sz;
		fr->bytes = new uint8_t[sz];
		if(ptr) {
			memcpy(fr->bytes, ptr, sz);
		}
		return Ptr(fr);
	}
	
	AudioBuffer::AudioBuffer()
	:	outTime(0.0)
	,	sampleRate(0)
	,	numBytes(0)
	,	bytes(NULL)
	{}
	
	AudioBuffer::~AudioBuffer() {
		if(bytes)
			delete [] bytes;
	}
	
	AudioBuffer::Ptr AudioBuffer::create(double o, int sr, int sz, uint8_t* ptr) {
		AudioBuffer* buf = new AudioBuffer;
		buf->outTime = o;
		buf->sampleRate = sr;
		buf->numBytes = sz;
		buf->bytes = new uint8_t[sz];
		if(ptr) {
			memcpy(buf->bytes, ptr, sz);
		}
		return Ptr(buf);
	}
	

	// this crap is to help ffmpeg assure good packet ordering
	// but i think it isn't necessary anymore because now ffmpeg
	// does it internally (hopefully)
//	uint64_t avcontext_getOpaque(AVCodecContext* context) {
//		return *(uint64_t*)context->opaque;
//	}
//	
//	void avcontext_setOpaque(AVCodecContext* context, uint64_t i) {
//		*(uint64_t*)context->opaque = i;
//	}
//	
//	uint64_t avframe_getOpaque(AVFrame* frame) {
//		return *(uint64_t*)frame->opaque;
//	}
//	
//	void avframe_setOpaque(AVFrame* frame, uint64_t i) {
//		*(uint64_t*)frame->opaque = i;
//	}
//
//	int mp_create_video_buffer(AVCodecContext* c, AVFrame* pic) {
//		int ret = avcodec_default_get_buffer(c, pic);
//		pic->opaque = av_malloc(sizeof(uint64_t));
//		avframe_setOpaque(pic, avcontext_getOpaque(c));
//		return ret;
//	}
//	
//	void mp_release_video_buffer(AVCodecContext* c, AVFrame* pic) {
//		if(pic) av_freep(&pic->opaque);
//		avcodec_default_release_buffer(c, pic);
//	}

	VideoDecoder* VideoDecoder::open(Demuxer* de) {
		if(!de)
			return NULL;
		
		int st = de->getStreamIndex(AVMEDIA_TYPE_VIDEO);
		if(st < 0)
			return NULL;
		
		VideoDecoder* dec = new VideoDecoder();
		dec->demuxer = de;
		dec->packets = de->getPacketQueue(st);
		dec->streamIdx = st;
		dec->stream = de->getStream(st);
		dec->context = dec->stream->codec;
		dec->frame = avcodec_alloc_frame();
		dec->frameRGB = avcodec_alloc_frame();

		dec->numFrames = dec->stream->nb_frames;
		dec->frameRate = av_q2d(dec->stream->r_frame_rate);
		dec->bitRate = dec->context->bit_rate / 1000000.0;
//		printf("video stream:\n\t%lld frames\n\t%f frames/sec\n\t%f bits/sec\n", numFrames, frameRate, bitRate);
		
//		// look at link for why we need to override the frame creation and release functions
//		// http://dranger.com/ffmpeg/tutorial05.html
//		dec->context->get_buffer = mp_create_video_buffer;
//		dec->context->release_buffer = mp_release_video_buffer;
		
		dec->width = dec->context->width;
		dec->height = dec->context->height;
		dec->bytesPerFrame = avpicture_get_size(PIX_FMT_RGB24, dec->width, dec->height);
		
		dec->sws = sws_getCachedContext(dec->sws,
										dec->width,
										dec->height,
										dec->context->pix_fmt,
										dec->width,
										dec->height,
										PIX_FMT_RGB24,
										SWS_BILINEAR,
										NULL,
										NULL,
										NULL);

		return dec;
	}
	
	VideoDecoder::VideoDecoder()
	:	streamIdx(-1)
	,	context(NULL)
	,	frame(NULL)
	,	frameRGB(NULL)
	,	sws(NULL)
	,	width(0)
	,	height(0)
	,	bytesPerFrame(0)
	,	clock(0.0)
	,	nextFrameTime(0.0)
	,	currentFrame(0)
	,	lastFrame(false)
	{}
	
	VideoDecoder::~VideoDecoder() {
		av_free(frame);
		av_free(frameRGB);
		sws_freeContext(sws);
	}

	int VideoDecoder::getWidth() { return width; }
	int VideoDecoder::getHeight() { return height; }
	int VideoDecoder::getBytesPerFrame() { return bytesPerFrame; }
	bool VideoDecoder::isLastFrame() { return lastFrame; }

	VideoFrame::Ptr VideoDecoder::previousFrame() {
		int64_t pts = (currentFrame - 1);
		
		seekToFrame(pts);
		
		while(true) {
			VideoFrame::Ptr frame = nextFrame();
			if(!frame)
				break;
			
			if(currentFrame > pts) {
				break;
			}
			
			if(currentFrame == pts) {
				return frame;
			}
		}
		
		return VideoFrame::Ptr();
	}
	
	VideoFrame::Ptr VideoDecoder::nextFrame() {
		AVPacket packet;
		
		// keep decoding until we have a whole frame
		while(true) {
			if(packets->isEmpty()) {
				// fetch more packets
				demuxer->demux(streamIdx);
			}
			
			if(!packets->pop(&packet)) {
				// i guess we're out of packets
				lastFrame = true;
				break;
			}
			
			if(PacketQueue::isFlushPacket(packet)) {
				avcodec_flush_buffers(context);
				continue;
			}
			
			// decode next packet of video
			int complete = 0;
			int error = 0;
			error = avcodec_decode_video2(context, frame, &complete, &packet);
			if(error <= 0) {
				printf("ffmpeg video decoding error: %x\n", error);
			}
			
			// free allocated packet resources
			av_free_packet(&packet);

			// we decoded a whole frame
			if(complete) {
				clock = frame->pkt_pts * av_q2d(stream->time_base);
				currentFrame = clock * frameRate;
				
				double delay = 0.0;
				delay = av_q2d(context->time_base);
				delay += frame->repeat_pict * (delay * 0.5);
				nextFrameTime = clock + delay;
				
				VideoFrame::Ptr rez = VideoFrame::create(nextFrameTime, width, height, bytesPerFrame, NULL);
				avpicture_fill((AVPicture*)frameRGB, rez->bytes, PIX_FMT_RGB24, width, height);
				sws_scale(sws, (uint8_t const* const*)frame->data, frame->linesize, 0, height, frameRGB->data, frameRGB->linesize);
				return rez;
			}
		}
		
		return VideoFrame::Ptr();
	}
	
	int64_t VideoDecoder::getCurrentFrame() { return currentFrame; }
	int64_t VideoDecoder::getFrameCount() { return numFrames; }
	double VideoDecoder::getCurrentTime() { return clock; }
	double VideoDecoder::getNextTime() { return nextFrameTime; }

	void VideoDecoder::seekToFrame(int64_t frame) { demuxer->seekToTime(frame * av_q2d(stream->time_base)); }
	
	void VideoDecoder::seekToTime(double time) {
		demuxer->seekToTime(time);
		
		// pull the next frame
		
		
		lastFrame = false;
	}

	AudioDecoder::AudioDecoder()
	:	demuxer(NULL)
	,	packets(NULL)
	,	frame(NULL)
	,	frameSize(0)
	,	sampleRate(0)
	,	sampleSize(0)
	,	channels(0)
	,	lastBuffer(false)
	,	swr(NULL)
	{}
	
	AudioDecoder* AudioDecoder::open(Demuxer* de) {
		if(!de)
			return NULL;
		
		int st = de->getStreamIndex(AVMEDIA_TYPE_AUDIO);
		if(st < 0)
			return NULL;
		
		AudioDecoder* dec = new AudioDecoder();
		dec->demuxer = de;
		dec->packets = de->getPacketQueue(st);
		dec->streamIdx = st;
		dec->stream = de->getStream(st);
		dec->context = dec->stream->codec;
		dec->frame = avcodec_alloc_frame();

		dec->frameSize = av_samples_get_buffer_size(NULL, 2, 1024, AV_SAMPLE_FMT_S16, 1);
		dec->sampleRate = 44100;
		dec->sampleSize = 2;
		dec->channels = 2;
		
		dec->swr = swr_alloc_set_opts(NULL,
									  AV_CH_LAYOUT_STEREO,
									  AV_SAMPLE_FMT_S16,
									  dec->sampleRate,
									  dec->context->channel_layout,
									  dec->context->sample_fmt,
									  dec->context->sample_rate,
									  0,
									  NULL);		
		swr_init(dec->swr);
		
		return dec;
	}
	
	AudioDecoder::~AudioDecoder() {
		av_free(frame);
		swr_free(&swr);
	}
	
	bool AudioDecoder::isLastBuffer() const { return lastBuffer; }
	int AudioDecoder::getSampleRate() const { return sampleRate; }
	int AudioDecoder::getSampleSize() const { return sampleSize; }
	int AudioDecoder::getFrameSize() const { return frameSize; }
	int AudioDecoder::getChannelCount() const { return context->channels; }

	AudioBuffer::Ptr AudioDecoder::convert(AVFrame* frame) {
		double out = frame->pkt_dts * av_q2d(stream->time_base);
		AudioBuffer::Ptr buffer = AudioBuffer::create(out, sampleRate, frameSize, NULL);
		
		unsigned char* pointers[SWR_CH_MAX] = {NULL};
		pointers[0] = buffer->bytes;
		
		int samplesCount = swr_convert(swr,
									   pointers,
									   1024,
									   (const unsigned char**)frame->extended_data,
									   frame->nb_samples);
		
		if(samplesCount <= 0)
			return AudioBuffer::Ptr();
		
		return buffer;
	}
	
	AudioBuffer::Ptr AudioDecoder::nextBuffer() {
		AVPacket packet;
		AudioBuffer::Ptr buffer;
		
		int complete = 0;
		while(!complete) {
			if(packets->isEmpty()) {
				// fetch more packets
				demuxer->demux(streamIdx);
			}
			
			if(!packets->pop(&packet)) {
				// i guess we're out of packets
				lastBuffer = true;
				break;
			}

			if(PacketQueue::isFlushPacket(packet)) {
				avcodec_flush_buffers(context);
				continue;
			}

			AVPacket tmp = packet;
			while(tmp.size > 0) {
				int result = avcodec_decode_audio4(context, frame, &complete, &tmp);
				
				if(result < 0) {
					printf("ffmpeg audio decoding error %d\n", result);
					break;
				}
				
				if(complete == 0) {
					break;
				}
				
				buffer = convert(frame);
				
				tmp.size -= result;
				tmp.data += result;
			}
			
			av_free_packet(&packet);
		}

		return buffer;
	}
	
	void AudioDecoder::seekToTime(double time) {
		demuxer->seekToTime(time);
		lastBuffer = false;
	}

}