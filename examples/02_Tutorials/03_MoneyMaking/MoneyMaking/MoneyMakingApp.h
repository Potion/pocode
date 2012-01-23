/////////////////////////////////////////
//
// pocode : Money Making
//
/////////////////////////////////////////

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