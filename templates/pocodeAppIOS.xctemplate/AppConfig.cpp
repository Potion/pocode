/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////

#include "poApplication.h"
#include "___PROJECTNAMEASIDENTIFIER___App.h"

poObject *createObjectForID(uint uid) {
	return new ___PROJECTNAMEASIDENTIFIER___App();}

void setupApplication() {
    //Set Default orientations for app startup
    poSetAutoRotateOrientations(PO_HORIZONTAL | PO_VERTICAL);
}

void cleanupApplication() {
}