/////////////////////////////////////////
//
// pocode : Particles
//
/////////////////////////////////////////

#include "poObject.h"

class ParticlesApp : public poObject {
public:
	ParticlesApp();
	virtual ~ParticlesApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};