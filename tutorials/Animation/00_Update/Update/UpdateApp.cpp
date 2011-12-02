/////////////////////////////////////////
//
// poCode : Update function
//
/////////////////////////////////////////


#include "UpdateApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"


// APP CONSTRUCTOR.
// Create all objects here.
UpdateApp::UpdateApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
   	addChild( BG );
	
	
	A = new poRectShape(5, 5);
	A->fillColor.set(0.6, 0.8, 0.4);
	A->position.set(125, 240, 0);
	A->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild(A);
	
	isScalingUp = true;
	
	
	B = new poOvalShape(30, 30, 30);
	B->fillColor.set(0.6, 0.8, 0.4);
	B->position.set(310, 240, 0);
	B->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild(B);
	
	velocity = poPoint(0.5, 1, 0);
	
	
	C = new poRectShape(4, 70);
	C->fillColor.set(0.6, 0.8, 0.4);
	C->position.set(490, 240, 0);
	C->setAlignment(PO_ALIGN_BOTTOM_CENTER);
	addChild(C);
    
    
    D = new poTextBox(140, 140);
    D->setFont(new poFont("Lucida Grande"));
    D->setTextSize(25);
	D->textColor.set(0.6, 0.8, 0.4);
    D->position.set(674, 240, 0);
    D->useTextBoundsAsBounds( true );
    D->setAlignment(PO_ALIGN_CENTER_CENTER);
    D->doLayout();
    addChild(D);
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
UpdateApp::~UpdateApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void UpdateApp::update() {
	
	
	// A. Update scale manually ///////////////////////
	
	if(isScalingUp)
		A->scale += poPoint(0.1, 0.1, 0);
	else
		A->scale -= poPoint(0.1, 0.1, 0);
	
	if(isScalingUp && A->scale.x >= 30) isScalingUp = false;
	if(!isScalingUp && A->scale.x <= 0) isScalingUp = true;
	
	
	// B. Update position manually ///////////////////////
	
	B->position += velocity;
	
	if (B->position.x > 385 - 15 || B->position.x < 235 + 15) velocity.x *= -1;
	if (B->position.y > 315 - 15 || B->position.y < 165 + 15) velocity.y *= -1;
	
	
	// C. Update rotation manually ///////////////////////
	
	poTime currentTime = poGetCurrentTime();
	int currentSecond = currentTime.seconds;
	
	C->rotation = 360/60 * currentSecond;
    
    
    // C. Update text manually ///////////////////////
    
    char timeString[32];
    sprintf(timeString, "%.2d:%.2d:%.2d", currentTime.amPmHours, currentTime.minutes, currentTime.seconds);
    
    D->setText(timeString);
    D->doLayout();
}


// EVENT HANDLER
// Respond to user events here.
void UpdateApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void UpdateApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
