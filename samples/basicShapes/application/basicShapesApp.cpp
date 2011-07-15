#include "basicShapesApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new basicShapesApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 600, 300);
}

void cleanupApplication() {
}

basicShapesApp::basicShapesApp() {
	addModifier(new poCamera2D(poColor::black));
    
    // make custom shape, orange
    customShape = new poShape2D();
    customShape->addPoint( -30,-40 );
    customShape->addPoint( 60,-50 );
    customShape->addPoint( 50,30 );
    customShape->addPoint( -50,40 );
    customShape->position( 150,75 );
    customShape->fillColor( poColor::orange );
    addChild( customShape );
    
    // make rectangular shape, yellow
    rectShape = new poRectShape( 150,100 );
    rectShape->position( 450,75 );
    rectShape->fillColor( poColor::yellow );
    rectShape->alignment( PO_ALIGN_CENTER_CENTER );
    addChild( rectShape );
   
    // make rectangular shape with image + transparency
    rectShapeWithImage = new poRectShape( "apple.jpg" );
    rectShapeWithImage->position( 150,225 );
    rectShapeWithImage->scale( 0.5, 0.5,1 );
    rectShapeWithImage->alignment( PO_ALIGN_CENTER_CENTER );
    addChild( rectShapeWithImage );
    
    // make oval shape, green
    ovalShape = new poOvalShape( 100,100, 50 );
    ovalShape->position( 450,225 );
    ovalShape->fillColor( poColor::green );
    addChild( ovalShape );
}

basicShapesApp::~basicShapesApp() {
}