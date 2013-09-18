/////////////////////////////////////////
//
// pocode application configuration
//
/////////////////////////////////////////


#include "poApplication.h"
#include "poHelpers.h"
#include "pocodeApp.h"

namespace po {
	
	Object *createObjectForID(uint uid) {
		return new pocodeApp();
	}
	  
	void setupApplication() {
		lookUpAndSetPath("resources");
		applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "pocodeApp", 0, 0, 1024, 768 );
	}

	void cleanupApplication() {
	}

}