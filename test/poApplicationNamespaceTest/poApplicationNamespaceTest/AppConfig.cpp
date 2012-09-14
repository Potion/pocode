/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "poApplicationNamespaceTestApp.h"

namespace po {
    poObject *createObjectForID(uint uid) {
        return new poApplicationNamespaceTestApp();
    }

    void setupApplication() {
        po::applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "poApplicationNamespaceTest", 100, 100, 1024, 768);
    }

    void cleanupApplication() {
    }
}