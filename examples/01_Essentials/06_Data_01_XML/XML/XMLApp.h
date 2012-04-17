/////////////////////////////////////////
//
// pocode : XML
//
/////////////////////////////////////////

#include "poObject.h"

class XMLApp : public poObject {
public:
	XMLApp();
	virtual ~XMLApp();
	
	virtual void update();
	
	virtual void draw();
	
	virtual void eventHandler(poEvent *event);
	
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};