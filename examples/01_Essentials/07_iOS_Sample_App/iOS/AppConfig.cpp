/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////

#include "poApplication.h"
#include "iOSApp.h"

poObject *createObjectForID(uint uid) {
	return new iOSApp();}

void setupApplication() {
    //Set Default orientations for app startup
    poSetAutoRotateOrientations(PO_HORIZONTAL | PO_VERTICAL);
}

void cleanupApplication() {
}