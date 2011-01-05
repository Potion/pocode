/*
 *  poAudio.cpp
 *  poFramework_static_lib
 *
 *  Created by Young Cho on 8/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "poAudioPlayer.h"

poAudioPlayer*	poAudioPlayer::audioPlayer = NULL;

int poAudioPlayer::NUM_BUFFERS = 0;
int poAudioPlayer::NUM_SOURCES = 0;

ALuint* poAudioPlayer::buffer = NULL;
std::vector<poAudioProperty*> poAudioPlayer::sourceList;

poAudioPlayer::poAudioPlayer()
{
	initAudio();
}

void poAudioPlayer::initAudio()
{
	alutInit( NULL, 0 );
	poAudioPlayer::buffer = (ALuint*)malloc( 0*sizeof(ALuint) );
	
	// set listener properties
	listenerPos[0]=listenerPos[1]=listenerPos[2] = 0.0;
	listenerVel[0]=listenerVel[1]=listenerVel[2] = 0.0;
	listenerOri[0]=listenerOri[1]=listenerOri[3]=listenerOri[5] = 0.0;
	listenerOri[2]=listenerOri[4] = 1.0;
	
	alListenerfv(AL_POSITION, listenerPos );
	alListenerfv(AL_VELOCITY,listenerVel);
	alListenerfv(AL_ORIENTATION,listenerOri);
}

poAudioPlayer*	poAudioPlayer::getAudioPlayer()
{
	if( !poAudioPlayer::audioPlayer )
	{
		poAudioPlayer::audioPlayer = new poAudioPlayer();
	}
	return poAudioPlayer::audioPlayer;
}

void killData()
{
	// delete source and buffer
	for( int i=0; i< poAudioPlayer::sourceList.size(); i++ )
		alDeleteSources( 1, &(poAudioPlayer::sourceList[i]->soundID) );
	alDeleteBuffers( poAudioPlayer::NUM_BUFFERS, poAudioPlayer::buffer );
}

void poAudioPlayer::addSound( char* filename )
{	
	poAudioPlayer::buffer = (ALuint*)realloc( poAudioPlayer::buffer, sizeof(ALuint)*(poAudioPlayer::NUM_BUFFERS+1) );
	alGenBuffers( 1, &poAudioPlayer::buffer[poAudioPlayer::NUM_BUFFERS] );
	
	ALsizei size,freq;
	ALenum	format;
	ALvoid	*data;

#ifdef WIN32
	alutLoadWAVFile( filename, &format, &data, &size, &freq, false );
#else
	alutLoadWAVFile( filename, &format, &data, &size, &freq );
#endif

	alBufferData( poAudioPlayer::buffer[poAudioPlayer::NUM_BUFFERS], format, data, size, freq );
	alutUnloadWAV(format, data, size, freq );
	
	if( alGetError() != AL_NO_ERROR )
		printf( "error : poAudioPlayer : failed to add sound \n");
	
	poAudioPlayer::NUM_BUFFERS++;
	
	// killdata will be called when exiting program
	atexit( killData );
}

ALuint poAudioPlayer::playBuffer(int S, poAudioProperty &P )
{
	ALuint sound = 0;
	alGenSources( 1, &sound );
	alSourcei(sound, AL_BUFFER, poAudioPlayer::buffer[S]);
	
	poAudioProperty* property = new poAudioProperty();
	property->soundID = sound;
	property->LOOP = P.LOOP;
	property->PITCH = P.PITCH;
	property->GAIN = P.GAIN;
	property->MAX_GAIN = P.MAX_GAIN;
	property->FADEMODE = P.FADEMODE;
	property->FADESPEED = P.FADESPEED;
	property->pos = P.pos;
	property->vel = P.vel;
	
	alSourcef(sound, AL_PITCH, property->PITCH);
	alSourcef(sound, AL_GAIN, property->GAIN );
	
	alSourcefv(sound, AL_POSITION, (ALfloat*)&(property->pos) );
	alSource3f(sound, AL_VELOCITY, property->vel.x, property->vel.y, property->vel.z );
	
	if( P.LOOP )
		alSourcei(sound, AL_LOOPING, AL_TRUE);
	else
		alSourcei(sound, AL_LOOPING, AL_FALSE);
	
	poAudioPlayer::sourceList.push_back( property );
	alSourcePlay( sound );
	
	return sound;
}

ALuint poAudioPlayer::play( int S, bool LOOP, float GAIN )
{
	poAudioProperty P;
	P.LOOP = LOOP;
	P.GAIN = GAIN;
	
	return playBuffer( S, P );
}

ALuint poAudioPlayer::play( int S, poAudioProperty &P )
{
	return playBuffer( S, P );
}

void poAudioPlayer::stop( ALuint soundID )
{
	alDeleteSources( 1, &soundID );
	if( alGetError() == AL_INVALID_NAME )
		printf( "----------------error: audioPlayer deleting audio : AL_INVALID_NAME\n", soundID );
	else if( alGetError()== AL_INVALID_OPERATION )
		printf( "----------------error: audioPlayer deleting audio : AL_INVALID_OPERATION\n");
	
	std::vector<poAudioProperty*>::reverse_iterator It;
	for( It=poAudioPlayer::sourceList.rbegin(); It != poAudioPlayer::sourceList.rend(); It++ ){
		poAudioProperty* P = *It;
		if( P->soundID == soundID ){
			poAudioPlayer::sourceList.erase( It.base()-1 );
			delete P;
		}
	}
}

void poAudioPlayer::stopAll()
{
	std::vector<poAudioProperty*>::reverse_iterator It;
	for ( It=poAudioPlayer::sourceList.rbegin(); It != poAudioPlayer::sourceList.rend(); It++ ){
		poAudioProperty* P = *It;
		stop( P->soundID );
	}
}

void poAudioPlayer::pause( ALuint soundID )
{
	alSourcePause( soundID );
}

void poAudioPlayer::resume( ALuint soundID )
{
	alSourcePlay( soundID );
}

void poAudioPlayer::setListenerPos( float x, float y, float z )
{
	alListener3f(AL_POSITION, x, y, z );
}

poAudioProperty poAudioPlayer::getAudioProperty( ALuint S )
{
	poAudioProperty P;
	alGetSourcef( S, AL_PITCH, &(P.PITCH) );
	alGetSourcef( S, AL_GAIN, &(P.GAIN) );
	alGetSource3f( S, AL_POSITION, &(P.pos.x), &(P.pos.y), &(P.pos.z) );
	alGetSource3f( S, AL_VELOCITY, &(P.vel.x), &(P.vel.y), &(P.vel.z) );
	return P;
}

void poAudioPlayer::setAudioProperty( ALuint S, poAudioProperty &P )
{
	alSourcef( S, AL_LOOPING, P.LOOP );
	alSourcef( S, AL_PITCH, P.PITCH );
	alSourcef( S, AL_GAIN, P.GAIN );
	alSource3f( S, AL_POSITION, P.pos.x, P.pos.y, P.pos.z );
	alSource3f( S, AL_VELOCITY, P.vel.x, P.vel.y, P.vel.z );
	alSourcePlay( S );
}


void poAudioPlayer::update()
{
	//printf("sourceList.size(); %d\n", sourceList.size() );
	std::vector<poAudioProperty*>::reverse_iterator It;
	for( It = poAudioPlayer::sourceList.rbegin(); It != poAudioPlayer::sourceList.rend(); It++ )
	{
		poAudioProperty* P = *It;
		
		int play = 0;
		alGetSourcei( P->soundID, AL_SOURCE_STATE, &play );
		if( play == AL_STOPPED ){
			stop( P->soundID );
			continue;
		}else if( play == AL_PAUSED ){
			continue;
		}
		
		if( P->FADEMODE == PO_FADE_IN )
		{
			float gain = 0;
			alGetSourcef( P->soundID, AL_GAIN, &gain );
			gain += P->FADESPEED;
			alSourcef( P->soundID, AL_GAIN, gain );
			if( gain >= P->MAX_GAIN )
			{
				gain = P->MAX_GAIN;
				alSourcef( P->soundID, AL_GAIN, gain );
			}
		}
		else if( P->FADEMODE == PO_FADE_OUT )
		{
			float gain = 0;
			alGetSourcef( P->soundID, AL_GAIN, &gain );
			gain -= P->FADESPEED;
			alSourcef( P->soundID, AL_GAIN, gain );
			if( gain <= P->MIN_GAIN )
			{
				gain = P->MIN_GAIN;
				alSourcef( P->soundID, AL_GAIN, gain );
				if( P->MIN_GAIN == 0.0 ){
					stop( P->soundID );
				}
			}
		}else if( P->FADEMODE == PO_FADE_NONE )
		{
		}
	}
}