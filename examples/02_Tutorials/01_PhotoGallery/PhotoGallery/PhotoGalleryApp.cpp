/////////////////////////////////////////
//
// pocode : Photo Gallery
//
/////////////////////////////////////////

#include "PhotoGalleryApp.h"
#include "poApplication.h"
#include "poCamera.h"

PhotoGalleryApp::PhotoGalleryApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
    
	smallScale.set(160.f/350.f, 160.f/350.f, 1);

	int imgNum = 0;
	for(int y=0; y <= 190; y += 190) {
		for(int x=0; x <= 190; x += 190) {
			
			char imgUrl[16];
			sprintf(imgUrl, "img%d.jpg", imgNum);
			imgNum++;
			
			poPoint P(35,170);
			P.x += x;
			P.y += y;
			
			poImageShape* image = new poImageShape(imgUrl);
			image->position = P;
			image->scale = smallScale;
			image->positionTween.setTweenFunction(PO_TWEEN_QUAD_IN_FUNC);
			image->positionTween.setDuration(1.0);
			image->scaleTween.setTweenFunction(PO_TWEEN_QUAD_IN_FUNC);
			image->scaleTween.setDuration(1.0);
			image->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, "photo_clicked");
			addChild(image);
		}
	}
	
	selectedPhoto = NULL;
}

PhotoGalleryApp::~PhotoGalleryApp() {
}

void PhotoGalleryApp::update() {
}

void PhotoGalleryApp::eventHandler(poEvent *event) {
	
	if (event->message == "photo_clicked") {
		
		if (event->source == selectedPhoto || event->source->positionTween.isRunning())
			return;
		
		if(selectedPhoto != NULL) {
			selectedPhoto->positionTween.set(goBackPosition);
			selectedPhoto->positionTween.start();
			selectedPhoto->scaleTween.set(smallScale);
			selectedPhoto->scaleTween.start();
		}
		
		event->source->scaleTween.set(poPoint(1, 1, 1));
		event->source->scaleTween.start();
		event->source->positionTween.set(poPoint(415, 170));
		event->source->positionTween.start();
		moveChildToFront(event->source);
		
		selectedPhoto = event->source;
		goBackPosition = event->source->position;
	}
}

void PhotoGalleryApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
