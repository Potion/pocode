#include "AlphaTestTextureApp.h"

#include "Helpers.h"
#include "poCamera.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"
#include "poResourceStore.h"

extern const char *evt_name[];

poObject *createObjectForID(uint uid) {
	lookUpAndSetPath("resources");
	return new AlphaTestTextureApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "AlphaTestTexture", 100, 100, 800, 800);
}

void cleanupApplication() {
}

AlphaTestTextureApp::AlphaTestTextureApp() {
	addModifier(new poCamera2D(poColor::green));
	
	poRectShape *shape = addChild(new poRectShape(getImage("1_Dry Dock 1.png")->texture()));
	
	
	shape->position.set(100,100,0);
	shape->drawBounds = true;
	shape->alphaTestTexture = true;
	shape->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	shape->addEvent(PO_MOUSE_ENTER_EVENT, this);
	shape->addEvent(PO_MOUSE_LEAVE_EVENT, this);
}

void AlphaTestTextureApp::eventHandler(poEvent *event) {
	printf("%s: %.2f %.2f\n", evt_name[event->type], event->local_position.x, event->local_position.y);
}
