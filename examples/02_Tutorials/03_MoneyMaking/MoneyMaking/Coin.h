//
//  Coin.h
//  MoneyMaking
//
//  Created by filippo on 11/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef MoneyMaking_Coin_h
#define MoneyMaking_Coin_h

#include "poObject.h"
#include "poImageShape.h"
#include "poShapeBasics2D.h"

enum coinType { CENT, HALF_DIME, DIME, QUARTER, HALF_DOLLAR, DOLLAR};


class Coin : public poObject {
public:
	Coin( coinType type );
	virtual ~Coin();
	
	virtual void eventHandler( poEvent* event );
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poImageShape* coinImage;
	
	float value;
	
	bool	isDragged;
	poPoint dragOffset;
};

#endif
