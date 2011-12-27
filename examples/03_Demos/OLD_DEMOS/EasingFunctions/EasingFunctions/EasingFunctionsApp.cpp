#include "EasingFunctionsApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTween.h"

poObject *createObjectForID(uint uid) {
	return new EasingFunctionsApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "EasingFunctions", 100, 100, 600, 300);
}

void cleanupApplication() {
}

EasingFunctionsApp::EasingFunctionsApp() {
	addModifier(new poCamera2D(poColor::black));
	
	for( int i = 0; i < 22; i++ ){
		addChild(new poLineShape(poPoint(25*(i+1.5), 50, 0), poPoint(25*(i+1.5), 250, 0)));
		
		s = new poOvalShape(10, 10, 50);
		s->position = poPoint(25*(i+1.5), 50, 0);
		addChild(s);
		
		s->position_tween
			.set(poPoint(25*(i+1.5), 250, 0))
			.setDuration(2.f)
			.setRepeat(PO_TWEEN_REPEAT_PINGPONG);
		
		switch (i) {
			case 0:
				s->position_tween
					.setTweenFunction(linearFunc)
					.start();
				break;
			case 1:
				s->position_tween
				.setTweenFunction(quadInFunc)
				.start();
				break;
			case 2:
				s->position_tween
				.setTweenFunction(quadOutFunc)
				.start();
				break;
			case 3:
				s->position_tween
				.setTweenFunction(quadInOutFunc)
				.start();
				break;
			case 4:
				s->position_tween
				.setTweenFunction(cubeInFunc)
				.start();
				break;
			case 5:
				s->position_tween
				.setTweenFunction(cubeOutFunc)
				.start();
				break;
			case 6:
				s->position_tween
				.setTweenFunction(cubeInOutFunc)
				.start();
				break;
			case 7:
				s->position_tween
				.setTweenFunction(quartInFunc)
				.start();
				break;
			case 8:
				s->position_tween
				.setTweenFunction(quartOutFunc)
				.start();
				break;
			case 9:
				s->position_tween
				.setTweenFunction(quartInOutFunc)
				.start();
				break;
			case 10:
				s->position_tween
				.setTweenFunction(quintInFunc)
				.start();
				break;
			case 11:
				s->position_tween
				.setTweenFunction(quintOutFunc)
				.start();
				break;
			case 12:
				s->position_tween
				.setTweenFunction(quintInOutFunc)
				.start();
				break;
			case 13:
				s->position_tween
				.setTweenFunction(sinusInFunc)
				.start();
				break;
			case 14:
				s->position_tween
				.setTweenFunction(sinusOutFunc)
				.start();
				break;
			case 15:
				s->position_tween
				.setTweenFunction(sinusInOutFunc)
				.start();
				break;
			case 16:
				s->position_tween
				.setTweenFunction(expoInFunc)
				.start();
				break;
			case 17:
				s->position_tween
				.setTweenFunction(expoOutFunc)
				.start();
				break;
			case 18:
				s->position_tween
				.setTweenFunction(expoInOutFunc)
				.start();
				break;
			case 19:
				s->position_tween
				.setTweenFunction(circInFunc)
				.start();
				break;
			case 20:
				s->position_tween
				.setTweenFunction(circOutFunc)
				.start();
				break;
			case 21:
				s->position_tween
				.setTweenFunction(circInOutFunc)
				.start();
				break;
			default:
				break;
		}
	}	
}

EasingFunctionsApp::~EasingFunctionsApp() {
}

void EasingFunctionsApp::eventHandler(poEvent *event) {
	
}

void EasingFunctionsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
