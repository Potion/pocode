/////////////////////////////////////////
//
// pocode : Colors
//
/////////////////////////////////////////

#include "poObject.h"

class ColorsApp : public poObject {
public:
	ColorsApp();
	virtual ~ColorsApp();
	
	virtual void update();
	
	virtual void draw();
	
	virtual void eventHandler(poEvent *event);
	
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};