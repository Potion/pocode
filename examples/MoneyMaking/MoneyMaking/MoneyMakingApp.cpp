#include "MoneyMakingApp.h"
#include "poApplication.h"
#include "poCamera.h"

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
	addModifier(new poCamera2D(poColor::black));
	
	for(int i=0; i < 100; i++) {
		
		coinType type = coinType(int(poRand(0, 5)));
		
		Coin* coinObject = new Coin( type );
		addChild( coinObject );
	}
	
	poImageShape* jar = new poImageShape("jar.png");
	jar->position.set(450, 200, 0);
	addChild(jar);
	
	currentCount = 0.f;
	
	countText = new poTextBox(150, 100);
	countText->setFont(new poFont("Lucida Grande"));
	countText->setText("Money: 0.00");
	countText->setTextSize(20);
	countText->useTextBoundsAsBounds(true);
	countText->setAlignment(PO_ALIGN_CENTER_CENTER);
	countText->position.set(600, 50, 0);
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
		
		currentCount += dict.getDouble("value");
		
		char countString[32];
		sprintf(countString, "Money: %.2f", currentCount);
		
		countText->setText(countString);
		countText->doLayout();
	}
}
