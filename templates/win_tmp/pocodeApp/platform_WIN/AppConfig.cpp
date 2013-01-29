#include "poApplication.h"
#include "pocodeApp.h"

#include <iostream>
using namespace std;
 
poObject *createObjectForID(uint uid) {
	return new pocodeApp();
}
  
void setupApplication() {
	//fs::current_path("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "pocodeApp", 0, 0, 1024, 768 );
}

void cleanupApplication() {
}

