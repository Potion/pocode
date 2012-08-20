//
//  Video.h
//  booth
//
//  Created by Joshua Fisher on 6/22/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#ifdef POTION_MAC

#include "poTween.h"
#include "poObject.h"

typedef struct MovieType** Movie;
typedef struct OpaqueQTVisualContext* QTVisualContextRef;
typedef struct __CVBuffer* CVOpenGLTextureRef;

class poVideoPlayer : public poObject {
public:
	poVideoPlayer();
	poVideoPlayer(const fs::path &uri);
	~poVideoPlayer();
	
	bool load(const fs::path &uri);
	
	void play();
	void stop();
	void seek(float f);
	void stepForward();
	void stepBackward();
	void volume(float f);
	void loop(poTweenRepeat type);

	int numFrames();
	float framerate();
	bool isPlaying();
	bool isDone();
	poPoint dimensions();
	float duration();

	bool bindFrame();

	void update();
	void draw();
	void draw(poRect r);

private:
	void cleanup();
	
	Movie movie;
	QTVisualContextRef context;
	CVOpenGLTextureRef texture;

	float dur;
	int frame_count;
	int width, height;
	poTweenRepeat looping;
};

#endif