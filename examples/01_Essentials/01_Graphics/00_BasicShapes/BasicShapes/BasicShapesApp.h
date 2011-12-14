/////////////////////////////////////////
//
// poCode : Basic Shapes
//
/////////////////////////////////////////

#include "poObject.h"

class BasicShapesApp : public poObject {
public:
	BasicShapesApp();
	virtual ~BasicShapesApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};