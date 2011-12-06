/////////////////////////////////////////
//
// poCode : Advanced Events
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
	
	poRectShape* Bone;
	poRectShape* Btwo;
	poRectShape* Bthree;
	
	poImageShape* C;
	poImage* img;
	poImage* img_over;
};