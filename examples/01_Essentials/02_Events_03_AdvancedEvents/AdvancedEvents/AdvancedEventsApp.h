/////////////////////////////////////////
//
// pocode : Advanced Events
//
/////////////////////////////////////////

#include "poObject.h"
#include "poImageShape.h"							// Include poImageShape.h to be able to use poImageShapes
#include "poShapeBasics2D.h"							// Include poImageShape.h to be able to use poShapes

class AdvancedEventsApp : public poObject {
public:
	AdvancedEventsApp();
	virtual ~AdvancedEventsApp();
	
	virtual void update();
	
	virtual void draw();
	
	virtual void eventHandler(poEvent *event);
	
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* Abottom;
	poRectShape* Atop;
	
	std::vector<poRectShape*> B;
	
	poImageShape* C;
	poImage* img;
	poImage* img_over;
};