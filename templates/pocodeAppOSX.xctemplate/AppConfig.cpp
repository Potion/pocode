/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "___PROJECTNAMEASIDENTIFIER___App.h"

namespace po {
    po::Object *createObjectForID(uint uid) {
        return new ___PROJECTNAMEASIDENTIFIER___App();
    }

    void setupApplication() {
        applicationCreateWindow(0, po::WINDOW_TYPE_NORMAL, "___PROJECTNAMEASIDENTIFIER___", 100, 100, 1024, 768);
    }

    void cleanupApplication() {
    }
} /*End po Namespace*/