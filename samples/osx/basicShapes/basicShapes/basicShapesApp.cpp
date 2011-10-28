#include "basicShapesApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "Helpers.h"

#include "poResourceLoader.h"
#include "SimpleDrawing.h"

poObject *createObjectForID(uint uid) {
	return new basicShapesApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow", 100, 100, 600, 300);
}

void cleanupApplication() {
}

basicShapesApp::basicShapesApp() {
	addModifier(new poCamera2D(poColor(.2,.2,.2)));
	
	// make custom shape, orange
    customShape = new poShape2D();
    customShape->addPoint( -30, -40 );
    customShape->addPoint( 60, -50 );
    customShape->addPoint( 50, 30 );
    customShape->addPoint( -50, 40 );
    customShape->position.set( 150, 75, 0 );
    customShape->fillColor = poColor(poColor::orange);
    addChild( customShape );

    // make rectangular shape, yellow
    rectShape = new poRectShape( 150,100 );
    rectShape->position = poPoint( 450, 75, 0 );
    rectShape->fillColor = poColor::yellow;
    rectShape->alignment( PO_ALIGN_CENTER_CENTER );
    addChild( rectShape );
   
    // make rectangular shape with image + transparency
    rectShapeWithImage = new poRectShape( "apple.jpg" );
    rectShapeWithImage->position.set( 150, 225, 0 );
    rectShapeWithImage->scale.set( 0.5, 0.5, 1 );
    rectShapeWithImage->alignment( PO_ALIGN_CENTER_CENTER );
	rectShapeWithImage->rotationTween.set(360).setTweenFunction(linearFunc).setDuration(1.0).setRepeat(PO_TWEEN_REPEAT_REGULAR).start();
    addChild( rectShapeWithImage );
	
    // make oval shape, green
    ovalShape = new poOvalShape( 100,100, 50 );
    ovalShape->position.set( 450, 225, 0 );
    ovalShape->fillColor = poColor::green;
    addChild( ovalShape );
    
    //ovalShape->addEvent(PO_MOUSE_OVER_EVENT, this);
	addEvent(PO_KEY_DOWN_EVENT, this);
}

basicShapesApp::~basicShapesApp() {
}


void basicShapesApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT) {
		for(int i=0; i<100; i++) {
		
		fs::directory_iterator iter("testimages");
		while(iter != fs::directory_iterator()) {
			if(fs::is_regular_file(iter->status())) {
				getImage(iter->path().string(), 100);
			}
			iter++;
		}
		deleteImageGroup(100);
			
		}
	}
}




