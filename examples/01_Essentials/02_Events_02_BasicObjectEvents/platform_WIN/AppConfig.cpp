/////////////////////////////////////////
//
// pocode application configuration Windows
//
/////////////////////////////////////////


#include "poApplication.h"
#include "BasicObjectEventsApp.h"

#include <iostream>
using namespace std;

poObject *createObjectForID(uint uid) {
	return new BasicObjectEventsApp();
}
  
void setupApplication() {
	fs::current_path("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "BasicObjectEventsApp", 0, 0, 800, 600 );
}

void cleanupApplication() {
}

