/////////////////////////////////////////
//
// pocode : Simple drawing
//
/////////////////////////////////////////


#include "poObject.h"

class SimpleDrawingApp : public poObject {
public:
	SimpleDrawingApp();
	virtual ~SimpleDrawingApp();
	
	virtual void update();
	
	virtual void draw();
	
	virtual void eventHandler(poEvent *event);
	
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poTexture* bgTex;
	poTexture* rectTex;
	poTexture* polygonTex;
};