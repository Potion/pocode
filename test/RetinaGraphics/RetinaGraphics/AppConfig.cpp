/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////

#include "poApplication.h"
#include "RetinaGraphicsApp.h"

poObject *createObjectForID(uint uid) {
	return new RetinaGraphicsApp();}

void setupApplication() {
    //Set Default orientations for app startup
    poSetAutoRotateOrientations(PO_HORIZONTAL | PO_VERTICAL);
}

void cleanupApplication() {
}