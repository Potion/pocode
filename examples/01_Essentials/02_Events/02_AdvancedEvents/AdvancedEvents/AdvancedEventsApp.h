/////////////////////////////////////////
//
// pocode : Advanced Events
//
/////////////////////////////////////////

#include "poObject.h"
#include "poImageShape.h"
#include "poShapeBasics2D.h"

class AdvancedEventsApp : public poObject {
public:
	AdvancedEventsApp();
	virtual ~AdvancedEventsApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* Abottom;
	poRectShape* Atop;
	
	std::vector<poRectShape*> B;
	
	poImageShape* C;
	poImage* img;
	poImage* img_over;
};