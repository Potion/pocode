/////////////////////////////////////////
//
// pocode : Basic Global Events
//
/////////////////////////////////////////

#include "poObject.h"
#include "poShapeBasics2D.h"							// Include poImageShape.h to be able to use poShapes

class BasicGlobalEventsApp : public poObject {
public:
	BasicGlobalEventsApp();
	virtual ~BasicGlobalEventsApp();
	
	virtual void update();
	
	virtual void draw();
	
	virtual void eventHandler(poEvent *event);
	
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* A;
	poRectShape* B;
	poRectShape* C;
	poRectShape* D;
};