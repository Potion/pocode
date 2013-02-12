
#include "poHelpers.h"
#include "poApplication.h"
#include "poCamera.h"

#include "pocodeApp.h"

#include "poMoviePlayer.h"
#include "poAudioPlayer.h"

#define MOVIE_URI "MSI_testdrive_noaudio.mp4"

pocodeApp::pocodeApp()
{
	poCamera2D *cam = new poCamera2D( poColor::black );
    addModifier( cam );

	player = new po::MoviePlayer;
	if(player->open(MOVIE_URI)) {
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

	addEvent(PO_KEY_DOWN_EVENT, this);
}

void pocodeApp::update()
{
	if(player->isPlaying()) {
		printf("%f %d/%d\n", player->getTime(), player->getFrameNum(), player->getFrameCount());
	}
}

void pocodeApp::draw()
{
}

void pocodeApp::eventHandler(poEvent* e)
{
	if(PO_KEY_DOWN_EVENT == e->type) {
		switch(e->keyChar) {
			case 'p':
				printf("play\n");
				player->play();
				break;
				
			case 'P':
				printf("pause\n");
				player->pause();
				break;
				
			case 's':
				printf("stop\n");
				player->stop();
				break;
				
			case 'r':
				printf("rewind\n");
				player->seek(0.f);
				break;
				
			case 'R':
				printf("reload\n");
				player->open(MOVIE_URI);
				player->setRect(0,0,getWindowWidth(),getWindowHeight());
				break;
		}
	}
	
}

void	pocodeApp::messageHandler(const std::string &msg, const poDictionary &dict )
{
}