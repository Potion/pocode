/////////////////////////////////////////
//
// pocode application configuration Windows
//
/////////////////////////////////////////


#include "poApplication.h"
#include "AdvancedEventsApp.h"

#include <iostream>
using namespace std;

poObject *createObjectForID(uint uid) {
	return new AdvancedEventsApp();
}
  
void setupApplication() {
	fs::current_path("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "AdvancedEventsApp", 0, 0, 800, 600 );
}

void cleanupApplication() {
}

