#include "colorPickerApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "Helpers.h"

poObject *createObjectForID(uint uid) {
	return new colorPickerApp();
}

void setupApplication() {
    setCurrentPath("../../colorPicker");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "colorPicker", 100, 100, 600, 600);
}

void cleanupApplication() {
}

colorPickerApp::colorPickerApp() {
	addModifier(new poCamera2D(poColor::black));
    
    theImage = new poImage( "images/bird.png" );
    theRect = new poRectShape( theImage );
    theRect->position( getWindowWidth()/2.0, getWindowHeight()/2.0 );
    //theRect->scale( 0.5, 0.5,1 );
    theRect->alignment( PO_ALIGN_CENTER_CENTER );
    addChild( theRect );
    
    theRect->addEvent( PO_MOUSE_MOVE_EVENT, this );
    
    indicator = new poOvalShape( 100,100,100 );
    indicator->position( 200,200 );
    indicator->alignment( PO_ALIGN_CENTER_CENTER );
    indicator->enableFill( false);
    indicator->enableStroke( true);
    indicator->strokeColor( poColor::white);
    indicator->generateStroke( 12);
    addChild( indicator);
    
    rgbLabel = new poRectShape( 150,24 );
    rgbLabel->alignment( PO_ALIGN_CENTER_CENTER );
    rgbLabel->position( getWindowWidth()/2.0, getWindowHeight()-10 );
    rgbLabel->fillColor( poColor::black );
    addChild( rgbLabel );
    
    poTextBox *rgbText = new poTextBox( 150,24 );
    rgbText->alignment( PO_ALIGN_CENTER_CENTER );
    rgbText->font( "Helvetica" );
    rgbText->textAlignment( PO_ALIGN_CENTER_CENTER );
    rgbText->position( 0,-3 );
    rgbText->layout();
    rgbText->name("text");
    rgbLabel->addChild( rgbText );
}

colorPickerApp::~colorPickerApp() {
}

void colorPickerApp::eventHandler(poEvent *event) {
    //printf("%f,%f \n",event->local_position.x, event->local_position.y);
    poColor pixelColor = theImage->getPixel( event->local_position );
    printf("%f,%f,%f,%f \n",pixelColor.R,pixelColor.G,pixelColor.B,pixelColor.A);
    indicator->strokeColor( poColor( pixelColor.B,pixelColor.G,pixelColor.R,0.8 ) );
    indicator->position( event->position );
    
    char rgbTextBuffer [100];
    sprintf( rgbTextBuffer, "%d,%d,%d",(int)(pixelColor.B*255),(int)(pixelColor.G*255),(int)(pixelColor.R)*255 );
    poTextBox *rgbText = (poTextBox *)rgbLabel->getChild("text");
    rgbText->textColor( poColor::white );
    rgbText->text( rgbTextBuffer );
    rgbText->layout();
	
}

void colorPickerApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
