/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////

#include "poApplication.h"
#include "___PROJECTNAMEASIDENTIFIER___App.h"

namespace po {
    po::Object* createObjectForID(uint uid) {
        return new ___PROJECTNAMEASIDENTIFIER___App();
    }
    
    void setupApplication() {
        //Set Default orientations for app startup
        po::setAutoRotateOrientations(po::ORIENTATION_HORIZONTAL | po::ORIENTATION_VERTICAL);
    }
    
    void cleanupApplication() {
    }
}