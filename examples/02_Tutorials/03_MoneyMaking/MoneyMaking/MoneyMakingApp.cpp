#include "MoneyMakingApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poImageShape.h"
#include "poShapeBasics2D.h"


MoneyMakingApp::MoneyMakingApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	
	poRectShape* dropRect = new poRectShape(dropArea.width, dropArea.height);
	dropRect->fillColor.set(1,0.8,0.75);
	dropRect->alpha = 0.5f;
	dropRect->position.set( dropArea.x, dropArea.y, 0 );
	addChild(dropRect);
	
//	float radius = 20;
//	float angle = 0;
//	float radiusInc = 8;
//	float angleInc = 90;
//	for(int i=0; i < 50; i++) {
//		
//		coinType type = coinType(floor(poRand(0, 5.99)));
//		
//		Coin* coinObject = new Coin( type );
//		addChild( coinObject );
//		
//		float x = 200 + radius * cos(angle);
//		float y = 370 + radius * sin(angle);
//		coinObject->coinImage->position.set(x, y, 0);
//		
//		radius += radiusInc;
//		angle += angleInc;
//		if(angle > 360) {
//			angle -= 360;
//			angleInc /= 2;
//			radiusInc /= 2;
//		}
//	}
	
	coinType type;
	poPoint P;
	for(int i=0; i < 60; i++) {
		
		if( i == 0 ) {
			type = CENT;
			P.set(60, 170, 0);
		}
		else if ( i == 10) {
			type = HALF_DIME;
			P.set(60, 230, 0);
		}
		else if ( i == 20){
			type = DIME;
			P.set(60, 290, 0);
		}
		else if ( i == 30) {
			type = QUARTER;
			P.set(60, 355, 0);
		}
		else if ( i == 40) { 
			type = HALF_DOLLAR;
			P.set(60, 435, 0);
		}
		else if ( i == 50) {
			type = DOLLAR;
			P.set(60, 520, 0);
		}
		
		Coin* coinObject = new Coin( type );
		addChild( coinObject );
		
		coinObject->coinImage->position = P;
		
		P.x += 30;
	}
	
	poImageShape* pig = new poImageShape("pig.png");
	pig->position.set(355, 150, 0);
//	pig->alpha = 0.5;
	addChild(pig);
	
	currentCount = 0.f;
	
	countText = new poTextBox(300, 100);
	countText->setFont(new poFont("Helvetica", "Bold"));
	countText->setText("$0.00");
	countText->setTextSize(55);
	countText->textColor.set(1,0.8,0.75);
	countText->alpha = 0.7;
	countText->useTextBoundsAsBounds(true);
	countText->setAlignment(PO_ALIGN_CENTER_CENTER);
	countText->position.set(605, 380, 0);
	countText->doLayout();
	addChild(countText);
}

MoneyMakingApp::~MoneyMakingApp() {
}

void MoneyMakingApp::update() {
}

void MoneyMakingApp::eventHandler(poEvent *event) {
}

void MoneyMakingApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
	if( msg == "coin dropped" ) {
		
		currentCount += dict.getFloat("value");
		
		char countString[32];
		sprintf(countString, "$%.2f", currentCount);
		
		countText->setText(countString);
		countText->doLayout();
	}
}
