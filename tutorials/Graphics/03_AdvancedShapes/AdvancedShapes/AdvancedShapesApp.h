/////////////////////////////////////////
//
// LESSON : Advanced Shapes
//
/////////////////////////////////////////

#include "poObject.h"

class AdvancedShapesApp : public poObject {
public:
	AdvancedShapesApp();
	virtual ~AdvancedShapesApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};