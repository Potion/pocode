#include "BoundsApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new BoundsApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Bounds", 100, 100, 600, 300);
}

void cleanupApplication() {
}

BoundsApp::BoundsApp() {
	addModifier(new poCamera2D(poColor::black));
	
	pop = new poObject();
    pop->drawBounds = 1;
	
	addChild(pop);
	
	kid1 = new poRectShape( 100.f, 100.f );
	kid1->position = poPoint( 100.f, 50.f );
	pop->addChild(kid1);

	kid2 = new poRectShape( 100.f, 100.f );
	kid2->position = poPoint( 400.f, 150.f );
	kid2->rotation = 20.f;
	pop->addChild(kid2); 
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

BoundsApp::~BoundsApp() {
}

void BoundsApp::draw() {
    po::setColor(poColor::green);
    po::drawRect(pop->getFrame());
    std::cout << pop->getFrame() << std::endl;
}

void BoundsApp::eventHandler(poEvent *event) {
}

void BoundsApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
