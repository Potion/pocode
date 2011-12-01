#include "PhotoGalleryApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new PhotoGalleryApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "PhotoGallery", 100, 100, 800, 600);
}

void cleanupApplication() {
}

PhotoGalleryApp::PhotoGalleryApp() {
	addModifier(new poCamera2D(poColor::black));
    
    smallScale.set(160.f/350.f, 160.f/350.f, 1);
	
	int imgNum = 0;
	for(int i=0; i < 2; i++) {
		for(int j=0; j < 2; j++) {
			
			poPoint P(35,100);
			P.x += 190 * j;
			P.y += 190 * i;
			
			char imgUrl[16];
			sprintf(imgUrl, "img%d.jpg", imgNum);
			imgNum++;
			
			poImageShape* image = new poImageShape(imgUrl);
			image->position = P;
			image->positionTween.setTweenFunction(PO_TWEEN_QUAD_IN_FUNC).setDuration(1.0);
			image->scale = smallScale;
			image->scaleTween.setTweenFunction(PO_TWEEN_QUAD_IN_FUNC).setDuration(1.0);
			image->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, "photo clicked");
			addChild(image);
			
			images.push_back(image);
		}
	}
	
	selectedPhoto = NULL;
}

PhotoGalleryApp::~PhotoGalleryApp() {
}

void PhotoGalleryApp::update() {
}

void PhotoGalleryApp::eventHandler(poEvent *event) {
	
	if (event->message == "photo clicked") {
		
		if (event->source->scaleTween.isRunning())
			return;
		
		if(selectedPhoto != NULL) {
			selectedPhoto->positionTween.set(goBackPosition).start();
			selectedPhoto->scaleTween.set(smallScale).start();
		}
		
		moveChildToFront(event->source);
		event->source->scaleTween.set(poPoint(1, 1, 1)).start();
		event->source->positionTween.set(poPoint(415, 100)).start();
		
		selectedPhoto = (poImageShape*) event->source;
		goBackPosition = event->source->position;
	}
}

void PhotoGalleryApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
