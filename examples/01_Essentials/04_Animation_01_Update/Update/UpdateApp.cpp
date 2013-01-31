/////////////////////////////////////////
//
// pocode : Update function
//
/////////////////////////////////////////


#include "UpdateApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"						// Include poImageShape.h to be able to use poShapes


// APP CONSTRUCTOR. Create all objects here.
UpdateApp::UpdateApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    FILE* F = fopen("bg.jpg", "r");
	if(!F) {
		printf("RESOURCES NOT FOUND!\nPlease open the Xcode menu, click on 'Preferences' and select the 'Locations' tab. Click on 'Advanced' and make sure that the 'Legacy' option is checked. If it's not, check it and try running this example again.");
		exit(0);
	}
	else fclose(F);
	
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// A. Update scale ///////////////////////
	
	A = new poRectShape(5, 5);							// Draw a rectangle that scales up and down
	A->fillColor.set(0.6, 0.8, 0.4);
	A->position.set(125, 240, 0);
	A->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild(A);
	
	isScalingUp = true;									// Boolean to determine if the rectangle
														// is getting bigger or smaller
	
	// B. Update position ///////////////////////
	
	B = new poOvalShape(30, 30, 30);					// Draw a bouncing ball
	B->fillColor.set(0.6, 0.8, 0.4);
	B->position.set(310, 240, 0);
	B->setAlignment(PO_ALIGN_CENTER_CENTER);
	addChild(B);
	
	velocity = poPoint(0.5, 1, 0);						// Store the velocity (direction) of the ball
														// to add to the position at each frame
	
	// C. Update rotation ///////////////////////
	
	C = new poRectShape(4, 70);							// Draw a clock hand that rotates each second
	C->fillColor.set(0.6, 0.8, 0.4);
	C->position.set(490, 240, 0);
	C->setAlignment(PO_ALIGN_BOTTOM_CENTER);
	addChild(C);
    
	
	// D. Update text ///////////////////////
    
    D = new poTextBox(140, 140);						// Draw a text box that shows the current time
    D->setTextSize(25);
	D->textColor.set(0.6, 0.8, 0.4);
    D->position.set(674, 240, 0);
    D->useTextBoundsAsBounds( true );
    D->setAlignment(PO_ALIGN_CENTER_CENTER);
    D->doLayout();
    addChild(D);
}

// APP DESTRUCTOR. Delete all objects here.
UpdateApp::~UpdateApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void UpdateApp::update() {
	
	// A. Update scale ///////////////////////
	
	if(isScalingUp && A->scale.x >= 30)					// If the scale is more than 30 times bigger
		isScalingUp = false;							// start reducing the scale
	if(!isScalingUp && A->scale.x <= 0)					// Otherwise if the scale is less than zero,
		isScalingUp = true;								// start increasing the scale
	
	if(isScalingUp)										// Check if the rectangle is scaling up or down
		A->scale += poPoint(0.1, 0.1, 0);				// Increase the x and y scale
	else
		A->scale -= poPoint(0.1, 0.1, 0);				// Decrease the x and y scale
	
	
	// B. Update position ///////////////////////
	
	B->position += velocity;							// Change the position of the ball by adding velocity
	
														// Check the bounds
	if (B->position.x > 385 - B->getWidth()/2 || 
		B->position.x < 235 + B->getWidth()/2)			// If the ball hits the left or right wall
		velocity.x *= -1;								// Invert the x value of the velocity vector
	if (B->position.y > 315 - B->getHeight()/2 || 
		B->position.y < 165 + B->getHeight()/2)			// If the ball hits the top or bottom wall
		velocity.y *= -1;								// Invert the y value of the velocity vector
	
	
	// C. Update rotation ///////////////////////
	
	poTime currentTime = poGetCurrentTime();			// Get the current time (see the file poHelpers.h)
	int currentSecond = currentTime.seconds;			// Get the current second from the poTime
	
	C->rotation = 360/60 * currentSecond;				// Set the rotation of the clock hand
														// based on the current second
    
    // D. Update text ///////////////////////
    
    char timeString[32];								// Create a string
	sprintf(timeString, "%.2d:%.2d:%.2d",				// Append current hour, minutes and seconds to it
			currentTime.amPmHours, 
			currentTime.minutes, 
			currentTime.seconds);
    
    D->setText(timeString);								// Replace the string in the text box
    D->doLayout();										// Remember to call this to apply the changes
}

// DRAW. Called once per frame. Draw objects here.
void UpdateApp::draw() {
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void UpdateApp::eventHandler(poEvent *event) {
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void UpdateApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
