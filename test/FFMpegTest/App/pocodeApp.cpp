
#include "poHelpers.h"
#include "poApplication.h"
#include "poCamera.h"

#include "pocodeApp.h"

#include "poMoviePlayer.h"
#include "poAudioPlayer.h"

//#define MOVIE_URI "diehard5_1080p.mov"
//#define MOVIE_URI "MSI_test.fdriveTrack_uncompressed_FINAL_compressed.mp4"
#define MOVIE_URI "tmp.mov"

pocodeApp::pocodeApp()
:	player(NULL)
,	audio(NULL)
{
	po::Camera2D *cam = new po::Camera2D( po::Color::black );
    addModifier( cam );

//	player = new po::MoviePlayer;
//	if(player->open(MOVIE_URI)) {
//		addChild(player);
//
//		player->setRect(0,0,po::getWindowWidth(),po::getWindowHeight());
//		player->play();
//	}
//	else {
//		printf("can't open movie file\n");
//	}

	audio = new po::AudioPlayer;
	if(audio->open("rally.mp3")) {
		audio->play();
	}
	else {
		printf("can't open audio file\n");
	}

	addEvent(po::KEY_DOWN_EVENT, this);
}

void pocodeApp::update()
{
	if(player && player->isPlaying()) {
		printf("%f %d/%d\n", player->getTime(), player->getFrameNum(), player->getFrameCount());
	}
}

void pocodeApp::draw()
{
}

void pocodeApp::eventHandler(po::Event* e)
{
	if(po::KEY_DOWN_EVENT == e->type) {
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
				player->setRect(0,0,po::getWindowWidth(),po::getWindowHeight());
				break;
		}
	}
	
}

void	pocodeApp::messageHandler(const std::string &msg, const po::Dictionary &dict )
{
}