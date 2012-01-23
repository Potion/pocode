/////////////////////////////////////////
//
// pocode : Alignment
//
/////////////////////////////////////////

#include "poObject.h"

class AlignmentApp : public poObject {
public:
	AlignmentApp();
	virtual ~AlignmentApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};