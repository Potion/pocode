#pragma once
#include "poObject.h"

class pocodeApp : public poObject {
public:
	pocodeApp();

	virtual void	update();
	virtual void	draw();
	virtual void	eventHandler(poEvent*);
	virtual void	messageHandler(const std::string &msg, const poDictionary &dict=poDictionary());
};