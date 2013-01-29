/////////////////////////////////////////
//
// pocode application configuration Windows
//
/////////////////////////////////////////


#include "poApplication.h"
#include "BasicGlobalEventsApp.h"

#include <iostream>
using namespace std;

poObject *createObjectForID(uint uid) {
	return new BasicGlobalEventsApp();
}
  
void setupApplication() {
	fs::current_path("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "BasicGlobalEventsApp", 0, 0, 800, 600 );
}

void cleanupApplication() {
}

