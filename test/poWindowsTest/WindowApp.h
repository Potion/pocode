#pragma once
#include "poObject.h"

class WindowApp : public poObject {
public:
	WindowApp();
	void eventHandler(poEvent*);
	void messageHandler(const std::string &msg, const poDictionary &dict=poDictionary());
};