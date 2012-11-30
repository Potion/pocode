/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "poAsyncImageLoaderExampleApp.h"

namespace po {
    Object *createObjectForID(uint uid) {
        return new poAsyncImageLoaderExampleApp();
    }

    void setupApplication() {
        po::applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "poAsyncImageLoaderExample", 2050, 100, 1024, 768);
    }

    void cleanupApplication() {
    }
}