/////////////////////////////////////////
//
// LESSON : Update function
//
/////////////////////////////////////////

#include "poObject.h"

class UpdateApp : public poObject {
public:
	UpdateApp();
	virtual ~UpdateApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poShape2D* A;
	bool isScalingA;
	
	poShape2D* B;
	poPoint velocityB;
	
	poShape2D* C;
	
	poShape2D* D;
};