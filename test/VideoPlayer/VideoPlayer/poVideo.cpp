//
//  Video.cpp
//  booth
//
//  Created by Joshua Fisher on 6/22/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "poVideo.h"
#include "poShader.h"
#include "poOpenGLState.h"

#include <boost/assign.hpp>
#include <QuickTime/QuickTime.h>

const char * vid_shader =
"	[[uniforms]]							\n"
"	uniform mat4 mvp;						\n"
"	uniform sampler2DRect tex;				\n"

"	[[varyings]]							\n"
"	varying vec2 texCoord;					\n"

"	[[vertex]]								\n"
"	attribute vec2 position;				\n"
"	attribute vec2 textureCoordinates;		\n"

"	void main() {							\n"
"		texCoord = textureCoordinates;		\n"
"		gl_Position = mvp * vec4(position,0.0,1.0);\n"
"	}										\n"

"	[[fragment]]							\n"
"	void main() {							\n"
"		vec4 texColor = texture2DRect(tex, texCoord);\n"
"		gl_FragColor = texColor;			\n"
"	}										\n";

static poShader *sh = NULL;

poVideoPlayer::poVideoPlayer()
:	movie(NULL)
,	context(NULL)
,	texture(NULL)
,	dur(0.f)
,	frame_count(-1)
,	width(0)
,	height(0)
,	looping(PO_TWEEN_REPEAT_NONE)
{}

poVideoPlayer::poVideoPlayer(const fs::path &uri)
:	movie(NULL)
,	context(NULL)
,	texture(NULL)
,	dur(0.f)
,	frame_count(-1)
,	width(0)
,	height(0)
,	looping(PO_TWEEN_REPEAT_NONE)
{
	load(uri);
}

poVideoPlayer::~poVideoPlayer() {
	cleanup();
}

bool poVideoPlayer::load(const fs::path &uri) {
	if(!sh) {
		std::istringstream vid(vid_shader);

		sh = new poShader;
		sh->loadSource(vid);
		sh->compile();
		sh->link();
	}
	
	cleanup();
	
	bool yes = true;
	NSString *nsstr = [NSString stringWithUTF8String:uri.c_str()];
	QTNewMoviePropertyElement props[] = {
		{kQTPropertyClass_DataLocation, kQTDataLocationPropertyID_CFStringNativePath, sizeof(CFStringRef), (void*)&nsstr, 0},
		{kQTPropertyClass_MovieInstantiation, kQTMovieInstantiationPropertyID_DontAskUnresolvedDataRefs, sizeof(bool), &yes, 0},
		{kQTPropertyClass_NewMovieProperty, kQTNewMoviePropertyID_Active, sizeof(bool), &yes, 0},
		{kQTPropertyClass_NewMovieProperty, kQTNewMoviePropertyID_DontInteractWithUser, sizeof(bool), &yes, 0}
	};
	OSStatus err;
	int prop_count = sizeof(props)/sizeof(QTNewMoviePropertyElement);
	err = NewMovieFromProperties(prop_count, props, 0, NULL, &movie);
	if(err != noErr) {
		printf("VideoPlayer: can't open file (osstatus = %ld)", err);
		return false;
	}
	
	PrerollMovie(movie, GetMovieTime(movie,NULL), GetMoviePreferredRate(movie));
	
	Rect movie_rect;
	GetMovieNaturalBoundsRect(movie, &movie_rect);
	width = movie_rect.right - movie_rect.left;
	height = movie_rect.bottom - movie_rect.top;
	dur = GetMovieDuration(movie) / (float)GetMovieTimeScale(movie);
	
	CGLContextObj ogl_context = CGLGetCurrentContext();
	CGLPixelFormatObj ogl_pixel_fmt = CGLGetPixelFormat(ogl_context);
	QTOpenGLTextureContextCreate(NULL, ogl_context, ogl_pixel_fmt, NULL, &context);
	SetMovieVisualContext(movie, context);
	
	return true;
}

void poVideoPlayer::play() {
	StartMovie(movie);
}

void poVideoPlayer::stop() {
	StopMovie(movie);
}

void poVideoPlayer::seek(float f) {
	if(f <= 0.f)
		GoToBeginningOfMovie(movie);
	// seek to end
	else if(f >= dur)
		GoToEndOfMovie(movie);
	// seek to beginning
	else
		SetMovieTimeValue(movie, TimeValue(f * GetMovieTimeScale(movie)));
}

void poVideoPlayer::volume(float f) {
	short s = short(glm::min(glm::max(f,0.f),1.f) * 255);
	SetMovieVolume(movie, s);
}

int poVideoPlayer::numFrames() {
	if(frame_count < 0) {
		TimeValue now = 0;
		OSType types[] = {VisualMediaCharacteristic};
		while(now >= 0) {
			GetMovieNextInterestingTime(movie, nextTimeStep, 1, types, now, fixed1, &now, NULL);
			frame_count++;
		}
	}
	return frame_count;
}

void poVideoPlayer::loop(poTweenRepeat type) {
	TimeBase tbase = GetMovieTimeBase(movie);
	long flags = GetTimeBaseFlags(tbase);
	
	switch(type) {
		case PO_TWEEN_REPEAT_NONE:
			flags &= ~loopTimeBase;
			flags &= ~palindromeLoopTimeBase;
			SetMoviePlayHints(movie, 0, hintsLoop | hintsPalindrome);
			break;
		case PO_TWEEN_REPEAT_REGULAR:
			flags |= loopTimeBase;
			flags &= ~palindromeLoopTimeBase;
			SetMoviePlayHints(movie, hintsLoop, hintsLoop);
			SetMoviePlayHints(movie, 0, hintsPalindrome);
			break;
		case PO_TWEEN_REPEAT_PINGPONG:
			flags |= loopTimeBase;
			flags |= palindromeLoopTimeBase;
			SetMoviePlayHints(movie, hintsLoop, hintsLoop);
			SetMoviePlayHints(movie, hintsPalindrome, hintsPalindrome);
			break;
	}
	
	SetTimeBaseFlags(tbase, flags);
}

void poVideoPlayer::stepForward() {
	TimeValue cur;
	TimeValue prev = GetMovieTime(movie, NULL);
	OSType types[] = {VisualMediaCharacteristic};
	GetMovieNextInterestingTime(movie, nextTimeStep, 1, types, prev, fixed1, &cur, NULL);
	if(cur != -1)
		SetMovieTimeValue(movie, cur);
	MoviesTask(movie, 0);
}

void poVideoPlayer::stepBackward() {
	TimeValue cur;
	TimeValue prev = GetMovieTime(movie, NULL);
	OSType types[] = {VisualMediaCharacteristic};
	GetMovieNextInterestingTime(movie, nextTimeStep, 1, types, prev, FixMul(Long2Fix(-1), fixed1), &cur, NULL);
	if(cur != -1)
		SetMovieTimeValue(movie, cur);
	MoviesTask(movie, 0);
}

float poVideoPlayer::framerate() {
	return dur > 0 ? numFrames() / dur : 0.f;
}

bool poVideoPlayer::isPlaying() {
	return GetMovieRate(movie) != 0;
}

bool poVideoPlayer::isDone() {
	return IsMovieDone(movie) != 0;
}

poPoint poVideoPlayer::dimensions() {
	return poPoint(width, height);
}

float poVideoPlayer::duration() {
	return dur;
}

bool poVideoPlayer::bindFrame() {
	if(texture) {
		GLenum target = CVOpenGLTextureGetTarget(texture);
		GLuint name = CVOpenGLTextureGetName(texture);
		glBindTexture(target,name);
	}
	return texture;
}

void poVideoPlayer::update() {
	MoviesTask(movie, 0);
	if(context) {
		QTVisualContextTask(context);
		if(QTVisualContextIsNewImageAvailable(context, nil)) {
			if(texture) {
				CVOpenGLTextureRelease(texture);
				texture = NULL;
			}
				
			OSStatus err = QTVisualContextCopyImageForTime(context, kCFAllocatorDefault, NULL, &texture);
			if(err == noErr && texture) {
//					CVOpenGLTextureGetCleanTexCoords(texture, 
//													 &tex_info.coords[0].x, 
//													 &tex_info.coords[1].x, 
//													 &tex_info.coords[2].x, 
//													 &tex_info.coords[3].x);
//					
//					tex_info.target = CVOpenGLTextureGetTarget(texture);
//					tex_info.name = CVOpenGLTextureGetName(texture);
				return true;
			}
		}
	}
	
	return false;
}

void poVideoPlayer::draw() {
	draw(poRect(poPoint(0,0), dimensions()));
}

void poVideoPlayer::draw(poRect r) {
	using namespace std;
	using namespace boost::assign;
	
	if(texture) {
		GLuint target = CVOpenGLTextureGetTarget(texture);
		GLuint name = CVOpenGLTextureGetName(texture);
		
		float ll[2], lr[2], ul[2], ur[2];
		CVOpenGLTextureGetCleanTexCoords(texture, ll, lr, ur, ul);

		poOpenGLState *ogl = poOpenGLState::get();

		ogl->pushShaderState();
		glUseProgram(sh->getUid());
		
		ogl->pushTextureState();
		glBindTexture(target, name);
		
		glUniformMatrix4fv(sh->uniformLocation("mvp"), 1, GL_FALSE, glm::value_ptr(ogl->matrix.transformation()));
		
		float x1 = r.x;
		float x2 = r.x + r.width;
		float y1 = r.y;
		float y2 = r.y + r.height;
		
		vector<float> points;
		points += x1, y1, x2, y1, x2, y2, x1, y2;
		
		vector<float> coords;
		coords += ul[0], ul[1], ur[0], ur[1], lr[0], lr[1], ll[0], ll[1];

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, &points[0]);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, &coords[0]);
		
		glDrawArrays(GL_QUADS, 0, 4);
		
		ogl->popShaderState();
		ogl->popTextureState();
	}
}

void poVideoPlayer::cleanup() {
	if(movie) {
		stop();
		
		if(texture) {
			CVOpenGLTextureRelease(texture);
			texture = NULL;
		}
		
		if(context) {
			SetMovieVisualContext(movie, NULL);
			QTVisualContextRelease(context);
			context = NULL;
		}
		
		DisposeMovie(movie);
		movie = NULL;
	}
}


