#include "poApplication.h"
#include "WindowApp.h"

poObject *createObjectForID(uint uid) {
	return new WindowApp();
}

void setupApplication() {
	applicationCreateWindow(0,WINDOW_TYPE_NORMAL,"WindowsApp",100,100,800,800);
}

void cleanupApplication() {
}