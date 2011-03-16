#include "poCommon.h"

poCommon *poCommon::get() {
	static poCommon *instance = NULL;
	if(!instance)
		instance = new poCommon();
	return instance;
}

void poCommon::read(const fs::path &url) {
	if(!fs::exists(url) || !fs::is_regular_file(url))
		return;
}

void poCommon::write(const fs::path &url) {
	if(!fs::exists(url) || !fs::is_regular_file(url))
		return;
}