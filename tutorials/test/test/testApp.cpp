#include "testApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"

poObject *createObjectForID(uint uid) {
	return new testApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "test", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

testApp::testApp() {
	addModifier(new poCamera2D(poColor::white));
    
    poTextBox* T = new poTextBox(200,300);
    T->setFont(poGetFont("Helvetica", "Regular"));
    T->setText(" Hello I'm a textbox");
    T->textColor =  poColor::black;
    T->position.set(20,20,0);
    T->doLayout();
    T->drawBounds = PO_TEXT_BOX_STROKE_TEXT_BOUNDS;

    addChild(T);
}

testApp::~testApp() {
}

void testApp::eventHandler(poEvent *event) {
	
}

void testApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
