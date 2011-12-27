/*	Created by filippo on 11/30/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poTextBox.h"
#include "Coin.h"

class MoneyMakingApp : public poObject {
public:
	MoneyMakingApp();
	virtual ~MoneyMakingApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poTextBox* countText;
	float	currentCount;
};