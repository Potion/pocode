/////////////////////////////////////////
//
// LESSON : Dictionaries
//
/////////////////////////////////////////

#include "poObject.h"

class DictionariesApp : public poObject {
public:
	DictionariesApp();
	virtual ~DictionariesApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};