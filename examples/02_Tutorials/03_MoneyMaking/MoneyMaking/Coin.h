/////////////////////////////////////////
//
// pocode : Money Making
//
/////////////////////////////////////////

#ifndef MoneyMaking_Coin_h
#define MoneyMaking_Coin_h

#include "poObject.h"
#include "poImageShape.h"

enum coinType { CENT, HALF_DIME, DIME, QUARTER, HALF_DOLLAR, DOLLAR };

static const poRect dropArea = poRect(537, 105, 140, 430);

class Coin : public poObject {
public:
	Coin( coinType type );
	
	virtual void eventHandler( poEvent* event );
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poImageShape* coinImage;
	
	float	value;
	
	bool	isDragged;
	poPoint dragOffset;
};

#endif
