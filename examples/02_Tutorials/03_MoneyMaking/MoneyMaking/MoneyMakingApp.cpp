#include "MoneyMakingApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poImageShape.h"

poObject *createObjectForID(uint uid) {
	return new MoneyMakingApp();
}

void setupApplication() {
	poRandSeed();
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MoneyMaking", 100, 100, 800, 600);
}

void cleanupApplication() {
}

MoneyMakingApp::MoneyMakingApp() {
	
	// Add a camera
	addModifier(new poCamera2D(poColor::black));
	
	// Show poCode lesson image in the background
    poImageShape* BG = new poImageShape("bg.jpg");
    addChild( BG );
	
	
	poRectShape* dropArea = new poRectShape(255, 400);
	dropArea->fillColor = poColor::orange;
	dropArea->alpha = 0.5f;
	dropArea->position.set( 480, 105, 0 );
	addChild(dropArea);
	
	for(int i=0; i < 1; i++) {
		
		coinType type = coinType(int(poRand(0, 5)));
		
		Coin* coinObject = new Coin( type );
		addChild( coinObject );
		
		printf("%d\n", int(type));
	}
	
	poImageShape* jar = new poImageShape("jar.png");
	jar->position.set(430, 200, 0);
	addChild(jar);
	
	currentCount = 0.f;
	
	countText = new poTextBox(300, 100);
	countText->setFont(new poFont("Lucida Grande"));
	countText->setText("Money: 0.00");
	countText->setTextSize(30);
	countText->textColor = poColor::black;
	countText->useTextBoundsAsBounds(true);
	countText->setAlignment(PO_ALIGN_CENTER_CENTER);
	countText->position.set(610, 400, 0);
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
		sprintf(countString, "Money: %.2f", currentCount);
		
		countText->setText(countString);
		countText->doLayout();
	}
}
