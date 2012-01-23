/////////////////////////////////////////
//
// pocode : Basic Global Events
//
/////////////////////////////////////////

#include "poObject.h"
#include "poShapeBasics2D.h"

class BasicGlobalEventsApp : public poObject {
public:
	BasicGlobalEventsApp();
	virtual ~BasicGlobalEventsApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* A;
	poRectShape* B;
	poRectShape* C;
	poRectShape* D;
};