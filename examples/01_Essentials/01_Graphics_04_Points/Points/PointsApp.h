/////////////////////////////////////////
//
// pocode : Points
//
/////////////////////////////////////////

#include "poObject.h"

class PointsApp : public poObject {
public:
	PointsApp();
	virtual ~PointsApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};