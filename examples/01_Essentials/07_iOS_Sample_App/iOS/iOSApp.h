/////////////////////////////////////////
//
// pocode : iOS App
//
/////////////////////////////////////////

#include "poObject.h"

class iOSApp : public poObject {
public:
	iOSApp();
	virtual ~iOSApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
	
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};

