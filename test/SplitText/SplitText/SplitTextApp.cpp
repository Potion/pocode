#include "SplitTextApp.h"

#include "poCamera.h"
#include "poApplication.h"

poObject *createObjectForID(uint uid) {
	return new SplitTextApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "SplitText", 100, 100, 800, 800);
}

void cleanupApplication() {
}

SplitTextApp::SplitTextApp() 
{
	addModifier(new poCamera2D());
	
	std::string s = "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo.";
	
	t = new flyingText( s, 200 );
	t->position.set(300,100,0);
	addChild(t);
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void SplitTextApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT) {
		if(event->keyChar == ' ') {
			t->restartDynamicEntrance();
		}
	}
}

