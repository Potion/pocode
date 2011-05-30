#include "poCommon.h"

poCommon *poCommon::get() {
	static poCommon *instance = NULL;
	if(!instance) {
		instance = new poCommon();
		instance->read("common.xml");
	}
	return instance;
}

