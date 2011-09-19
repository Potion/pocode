#define BOOST_TEST_MODULE potionLib test suite
#include <boost/test/unit_Test.hpp>

#include "poPoint.h"
#include "poWindow.h"

poObject*	createObjectForID(uint id) {return NULL;}

float		getWindowWidth()	{return 800;}
float		getWindowHeight()	{return 600;}

void		applicationMakeWindowCurrent(poWindow *win) {}
void		applicationMakeWindowFullscreen(poWindow *win, bool b) {}
void		applicationMoveWindow(poWindow *win, poPoint p) {}
poWindow*	applicationCurrentWindow() {return NULL;}
