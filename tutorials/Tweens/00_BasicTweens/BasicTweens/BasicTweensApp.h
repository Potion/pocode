/////////////////////////////////////////
//
// LESSON : Basic Tweens
//
/////////////////////////////////////////

#include "poObject.h"
#include "poShapeBasics2D.h"

class BasicTweensApp : public poObject {
public:
	BasicTweensApp();
	virtual ~BasicTweensApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* A;
	poRectShape* B;
	poRectShape* C;
	poRectShape* D;
};