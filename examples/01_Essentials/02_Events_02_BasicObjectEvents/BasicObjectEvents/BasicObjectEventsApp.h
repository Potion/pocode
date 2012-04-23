/////////////////////////////////////////
//
// pocode : Basic Object Events
//
/////////////////////////////////////////

#include "poObject.h"
#include "poShapeBasics2D.h"

class BasicObjectEventsApp : public poObject {
public:
	BasicObjectEventsApp();
	virtual ~BasicObjectEventsApp();
	
	virtual void update();
	
	virtual void draw();
	
	virtual void eventHandler(poEvent *event);
	
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* A;
	poRectShape* B;
	poRectShape* C;
	poRectShape* D;
};