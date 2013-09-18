/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "RichtTextCachingApp.h"

namespace po {
    po::Object *createObjectForID(uint uid) {
        return new RichtTextCachingApp();
    }

    void setupApplication() {
        applicationCreateWindow(0, po::WINDOW_TYPE_NORMAL, "RichtTextCaching", 100, 100, 1024, 768);
    }

    void cleanupApplication() {
    }
} /*End po Namespace*/