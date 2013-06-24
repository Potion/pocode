/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "RetinaOSXTestApp.h"

namespace po {
    po::Object *createObjectForID(uint uid) {
        return new RetinaOSXTestApp();
    }

    void setupApplication() {
        applicationCreateWindow(0, po::WINDOW_TYPE_NORMAL, "RetinaOSXTest", 100, 100, 1024, 768);
    }

    void cleanupApplication() {
    }
} /*End po Namespace*/