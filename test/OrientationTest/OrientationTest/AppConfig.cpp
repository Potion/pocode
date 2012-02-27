/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////

#include "poApplication.h"
#include "OrientationTestApp.h"

poObject *createObjectForID(uint uid) {
    return new OrientationTestApp();
}

void setupApplication() {
    //Set Default orientations for app startup
    poSetAutoRotateOrientations(PO_VERTICAL | PO_HORIZONTAL);
}

void cleanupApplication() {
}