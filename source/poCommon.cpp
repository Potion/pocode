#include "poCommon.h"

poCommon *poCommon::get() {
	static poCommon *instance = NULL;
	if(!instance)
		instance = new poCommon();
	return instance;
}

