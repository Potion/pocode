/////////////////////////////////////////
//
// pocode : Parents and children
//
/////////////////////////////////////////

#include "poObject.h"

class ParentsChildrenApp : public poObject {
public:
	ParentsChildrenApp();
	virtual ~ParentsChildrenApp();
	
	virtual void update();
	
	virtual void draw();
	
	virtual void eventHandler(poEvent *event);
	
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};