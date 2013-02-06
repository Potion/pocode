
#include "poHelpers.h"
#include "poApplication.h"
#include "poCamera.h"

#include "pocodeApp.h"

#include "poMoviePlayer.h"
#include "poAudioPlayer.h"

pocodeApp::pocodeApp()
{
	poCamera2D *cam = new poCamera2D( poColor::black );
    addModifier( cam );

	player = new po::MoviePlayer;
	if(player->open("diehard5_1080p.mov")) {
		addChild(player);

		player->setRect(0,0,getWindowWidth(),getWindowHeight());
		player->play();
	}
	else {
		printf("can't open movie file\n");
	}

	//audio = new po::AudioPlayer;
	//if(audio->open("audio_stereo16.mp3")) {
	//	audio->play();
	//}
	//else {
	//	printf("can't open audio file\n");
	//}
}

void pocodeApp::update()
{
}

void pocodeApp::draw()
{
}

void pocodeApp::eventHandler(poEvent*)
{
}

void	pocodeApp::messageHandler(const std::string &msg, const poDictionary &dict )
{
}