#include "WindowResizeTestApp.h"
#include "poApplication.h"
#include "poCamera.h"

class MouseTracker : public poObject {
public:
	MouseTracker() : poObject("Mouse Tracker") {}
	
	virtual void draw() {
		glColor3f(1,0,0);
		mouse = globalToLocal(getWindowInvMousePosition());
		glRectf(mouse.x-5, mouse.y-5, mouse.x+5, mouse.y+5);
	}
	
	poPoint mouse;
};

poObject *createObjectForID(uint uid) {
	return new WindowResizeTestApp();
}

void setupApplication() {
	// making the window too tall to fit on screen
	// os x should resize it down
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "WindowResizeTest", 100, 100, 800, 2000);
}

void cleanupApplication() {
}

WindowResizeTestApp::WindowResizeTestApp() {
	addModifier(new poCamera2D());
	addChild(new MouseTracker());
}

void WindowResizeTestApp::update() {
//	GLint viewport[4];
//	glGetIntegerv(GL_VIEWPORT, viewport);
//	// viewport is actually saved and restored in the draw look so this is meaningless
//	printf("%d %d\n", viewport[2], viewport[3]);
	// but the window isn't getting resized properly
	printf("%.0f %.0f\n", getWindowWidth(), getWindowHeight());
}