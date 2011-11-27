/////////////////////////////////////////
//
// LESSON : Points
//
/////////////////////////////////////////

#include "PointsApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poShapeBasics2D.h"
#include "poTextBox.h"


// APP CONSTRUCTOR.
// Create all objects here.
PointsApp::PointsApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
	
	
	// A. Common poPoints: position and scale ///////////////////////
	
	poPoint pointA = poPoint(50, 50);
	poPoint scaleA = poPoint(2, 2, 1);
	
	// Create a holder to draw inside the white rectangle A
	poObject* holderA = new poObject();
	holderA->position.set(50, 225, 0);
	addChild(holderA);
	
	// Draw pointA as an oval
	poOvalShape* oval = new poOvalShape(5,5,10);
	oval->fillColor = poColor::black;
	oval->position = pointA;
	oval->scale = scaleA;
	holderA->addChild(oval);
	
	
	// B. poPoints: + and - ///////////////////////
	
	poPoint pointB = poPoint(50, 25);
	poPoint diff = pointA - pointB;
	poPoint sum = pointA + pointB;
	
	poObject* holderB = new poObject();
	holderB->position.set(233, 225, 0);
	addChild(holderB);
	
	// Draw pointA as an oval
	poOvalShape* ovalA = new poOvalShape(5, 5, 10);
	ovalA->fillColor = poColor::black;
	ovalA->position = pointA;
	holderB->addChild(ovalA);
	
	// Draw pointB as an oval
	poOvalShape* ovalB = new poOvalShape(5, 5, 10);
	ovalB->fillColor = poColor::black;
	ovalB->position = pointB;
	holderB->addChild(ovalB);
	
	// Draw diff as a line
	poLineShape* diffLine = new poLineShape(poPoint(0,0), diff);
	diffLine->generateStroke(2);
	diffLine->strokeColor = poColor::blue;
	holderB->addChild(diffLine);
	
	poTextBox* diffText = new poTextBox();
	diffText->setFont( new poFont("Lucida Grande") );
	diffText->setText("Diff");
	diffText->setTextSize(13);
	diffText->textColor = poColor::blue;
	diffText->doLayout();
	diffText->position = diff;
	holderB->addChild(diffText);
	
	// Draw sum as a line
	poLineShape* sumLine = new poLineShape(poPoint(0,0), sum);
	sumLine->generateStroke(2);
	sumLine->strokeColor = poColor::red;
	holderB->addChild(sumLine);
	
	poTextBox* sumText = new poTextBox();
	sumText->setFont( new poFont("Lucida Grande") );
	sumText->setText("Sum");
	sumText->setTextSize(13);
	sumText->textColor = poColor::red;
	sumText->doLayout();
	sumText->position = sum;
	holderB->addChild(sumText);
	
	
	// C. poPoints: * and / ///////////////////////
	
	poPoint pointC = poPoint(40, 60, 0);
	poPoint mult = pointC * 2;
	poPoint div = pointC / 2;
	
	poObject* holderC = new poObject();
	holderC->position.set(417, 225, 0);
	addChild(holderC);
	
	// Draw pointC as an oval
	poOvalShape* ovalC = new poOvalShape(5, 5, 10);
	ovalC->fillColor = poColor::black;
	ovalC->position = pointC;
	holderC->addChild(ovalC);
	
	// Draw mult as a line
	poLineShape* multLine = new poLineShape(poPoint(0,0), mult);
	multLine->generateStroke(2);
	multLine->strokeColor = poColor::red;
	holderC->addChild(multLine);
	
	poTextBox* multText = new poTextBox();
	multText->setFont( new poFont("Lucida Grande") );
	multText->setText("Mult");
	multText->setTextSize(13);
	multText->textColor = poColor::red;
	multText->doLayout();
	multText->position = mult;
	holderC->addChild(multText);
	
	// Draw div as a line
	poLineShape* divLine = new poLineShape(poPoint(0,0), div);
	divLine->generateStroke(2);
	divLine->strokeColor = poColor::blue;
	holderC->addChild(divLine);
	
	poTextBox* divText = new poTextBox();
	divText->setFont( new poFont("Lucida Grande") );
	divText->setText("Div");
	divText->setTextSize(13);
	divText->textColor = poColor::blue;
	divText->doLayout();
	divText->position = div;
	holderC->addChild(divText);
	
	
    // D. Common poPoints: position and scale ///////////////////////
	
	poObject* holderD = new poObject();
	holderD->position.set(600, 225, 0);
	addChild(holderD);
}


// APP DESTRUCTOR
// Delete all objects here. (optional)
PointsApp::~PointsApp() {
}


// UPDATE
// Animate objects here. This is called after every frame is drawn.
void PointsApp::update() {
}


// EVENT HANDLER
// Respond to user events here.
void PointsApp::eventHandler(poEvent *event) {
}


// MESSAGE HANDLER
// Receive inter-object messages here.
void PointsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
