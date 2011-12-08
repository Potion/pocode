/*	Created by Jonathan Bobrow on 12/5/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "FlipCard.h"

class FlipCardsApp : public poObject {
public:
	FlipCardsApp();
	virtual ~FlipCardsApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());

    FlipCard*   card;
};