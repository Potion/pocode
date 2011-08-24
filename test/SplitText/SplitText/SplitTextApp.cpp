#include "SplitTextApp.h"

#include "poCamera.h"
#include "poApplication.h"

poObject *createObjectForID(uint uid) {
	return new SplitTextApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "SplitText", 100, 100, 600, 300);
}

void cleanupApplication() {
}

SplitTextApp::SplitTextApp() 
{
	addModifier(new poCamera2D());
	
	std::string s = "In the late 1930s, Max Marcus ran a general merchandise auction house out of one of the basement storefronts at 97 Orchard Street. His entire life was shaped by doing business on the Lower East Side. By the late 1980s, Marcus was running a botanica at the nearby Essex Street indoor market.";
	
	t = new flyingText(s, 250, 800, 0, .3f, 3.f);
	t->position.set(200,25,0);
	addChild(t);
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void SplitTextApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT) {
		if(event->keyChar == ' ') {
			t->reset();
			t->fly();
		}
	}
}
