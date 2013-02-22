/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////

#include "poApplication.h"
#include "RetinaGraphicsApp.h"

namespace po {
    po::Object* createObjectForID(uint uid) {
        return new RetinaGraphicsApp();
    }

    void setupApplication() {
        //Set Default orientations for app startup
        po::setAutoRotateOrientations(po::ORIENTATION_HORIZONTAL | po::ORIENTATION_VERTICAL);
    }

    void cleanupApplication() {
    }
}