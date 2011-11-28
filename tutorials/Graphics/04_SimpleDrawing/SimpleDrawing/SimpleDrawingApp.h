/////////////////////////////////////////
//
// LESSON : Simple drawing
//
/////////////////////////////////////////


#include "poObject.h"

class SimpleDrawingApp : public poObject {
public:
	SimpleDrawingApp();
	virtual ~SimpleDrawingApp();
	
	virtual void draw();
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poTexture* bgTex;
	poTexture* rectTex;
	poTexture* polygonTex;
};